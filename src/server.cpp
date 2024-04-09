#include "server.h"

ABSL_FLAG(uint16_t, port, 50051, "port to listen on");

class ChainServiceImpl : public Chain::Service {
public:
    grpc::Status Gaming(grpc::ServerContext* context, grpc::ServerReaderWriter<Block, Block>* stream) override {
        Block block;

        if (!stream->Read(&block))
            return grpc::Status(grpc::StatusCode::INTERNAL, "failed to read block");

        if (!block.has_join())
            return grpc::Status(grpc::StatusCode::INTERNAL, "invalid block");

        {
            std::lock_guard<std::mutex> lock(mutex_);

            if (streams_.find(block.name()) != streams_.end())
                return grpc::Status(grpc::StatusCode::INTERNAL, "duplicate name");

            streams_[block.name()] = stream;
        }

        std::string name = block.name();
        std::cout << "Gaming " << name << std::endl;

        while (stream->Read(&block)) {
            if (block.has_quit()) {
                this->broadcast(block);

                {
                    std::lock_guard<std::mutex> lock(mutex_);

                    streams_.erase(name);
                }

                break;
            }

            if (block.has_join())
                continue;

            broadcast(block);
        }

        std::cout << "Leaving " << name << std::endl;

        return grpc::Status::OK;
    }

    void broadcast(const Block& block) {
        for (auto& stream : streams_) {
            stream.second->Write(block);
        }
    }

private:
    std::map<std::string, grpc::ServerReaderWriter<Block, Block>*> streams_;
    std::mutex mutex_;
};

void RunServer(uint16_t port) {
    std::string server_address = absl::StrFormat("localhost:%d", port);
    ChainServiceImpl service;

    grpc::ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);

    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;

    server->Wait();
}

int main(int argc, char** argv) {
    absl::ParseCommandLine(argc, argv);
    RunServer(absl::GetFlag(FLAGS_port));

    return 0;
}