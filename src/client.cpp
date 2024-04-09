#include "client.h"

ABSL_FLAG(std::string, host, "localhost:50051", "host:port");
ABSL_FLAG(std::string, name, "user", "user name");
ABSL_FLAG(uint16_t, delay, 1000, "delay between requests");

class ChainClient {
public:
    ChainClient(std::shared_ptr<grpc::Channel> chhannel, std::string name, Game* game, uint16_t delay)
        : stub_(Chain::NewStub(chhannel)), name(name), game(game), delay(delay) {}

    void MakeRequests(std::shared_ptr<grpc::ClientReaderWriter<Block, Block>> stream) {
        //! error somewhere there
        {
            Block join_block;
            JoinGame* join = new JoinGame();
            join->set_x(100);
            join->set_y(100);

            join_block.set_name(name);
            join_block.set_allocated_join(join);
            stream->Write(join_block);
        }
        std::cout << "Joining game..." << std::endl;

        while (true) {
            Block block;
            Transfer* trans = new Transfer();
            auto pos = game->GetPlayer().GetPosition();

            trans->set_x(pos.x);
            trans->set_y(pos.y);

            block.set_name(name);
            block.set_allocated_trans(trans);
            stream->Write(block);

            std::this_thread::sleep_for(std::chrono::milliseconds(delay));
        }

        {
            Block quit_block;
            QuitGame* quit = new QuitGame();
            quit_block.set_name(name);
            quit_block.set_allocated_quit(quit);

            stream->Write(quit_block);
        }

        stream->WritesDone();
    }

    void ProcessResponses(std::shared_ptr<grpc::ClientReaderWriter<Block, Block>> stream) {
        Block block;
        std::cout << "Players:" << std::endl;
        while (stream->Read(&block)) {
            std::cout << block.name() << std::endl;
            if (block.has_join() && block.name() != name) {} // We need this plug otherwise enemy wont be displayed; this bug still unsolved but we know about it
                game->AddEnemy(block.join().x(), block.join().y(), block.name());

            if (block.has_trans() && block.name() != name)
                game->UpdateEnemy(block.name(), block.trans().x(), block.trans().y());

            if (block.has_quit() && block.name() != name)
                game->RemoveEnemy(block.name());
        }
    }

    void StartClient() {
        grpc::ClientContext context;
        std::shared_ptr<grpc::ClientReaderWriter<Block, Block>> stream(stub_->Gaming(&context));

        std::thread writer(&ChainClient::MakeRequests, this, stream);
        ProcessResponses(stream);

        writer.join();

        grpc::Status status = stream->Finish();
        if (status.ok())
            std::cout << "Disconnected successfully" << std::endl;
        else
            std::cout << status.error_code() << ": " << status.error_message() << std::endl;
    }

private:
    std::shared_ptr<Chain::Stub> stub_;
    std::string name;
    Game* game;
    uint16_t delay;
};

int main(int argc, char** argv) {
    absl::ParseCommandLine(argc, argv);

    std::string host = absl::GetFlag(FLAGS_host);
    std::string name = absl::GetFlag(FLAGS_name);
    uint16_t delay = absl::GetFlag(FLAGS_delay);

    sf::RenderWindow window(sf::VideoMode(1280, 720), "SFML works!", sf::Style::Titlebar | sf::Style::Close);

    window.setFramerateLimit(60);
    window.setActive(false);

    Game game;

    std::thread render_thread(render::RenderThread, &window, &game);

    ChainClient client(grpc::CreateChannel(host, grpc::InsecureChannelCredentials()), name, &game, delay);
    std::thread chainer(&ChainClient::StartClient, &client);

    sf::Clock clock;
    while (window.isOpen()) {
        game.Update(clock.restart());

        sf::Event event;
        while (window.pollEvent(event)) {
            switch (event.type) {
                case sf::Event::Closed:
                    window.close();
                    break;
                
                case sf::Event::GainedFocus:
                    game.SetFocused(true);
                    break;

                case sf::Event::LostFocus:
                    game.SetFocused(false);
                    break;

                default:
                    break;
            }
        }
    }

    chainer.join();
    render_thread.join();

    return 0;
}