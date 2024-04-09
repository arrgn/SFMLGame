#include "game.h"

Game::Game() {
    if (!player_texture.loadFromFile("imgs/AnimationSheet_Character.png", sf::IntRect(0, 0, 32, 32)))
        abort();

    player = Character(player_texture);

    speed = 100;
}

Character& Game::GetPlayer() {
    return player;
}

std::map<std::string, Character>& Game::GetEnemies() {
    return enemies;
}

void Game::Update(sf::Time dt) {
    sf::Vector2f delta = { 0, 0 };

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        delta.y = -1;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        delta.y = 1;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        delta.x = -1;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        delta.x = 1;

    if (focused && (delta.x != 0 || delta.y != 0)) {
        player.SetPosition(player.GetPosition() + delta * speed * dt.asSeconds());
    }
}

void Game::AddEnemy(float x, float y, std::string name) {
    enemies[name] = Character(player_texture, x, y);
}

void Game::RemoveEnemy(std::string name) {
    enemies.erase(name);
}

void Game::UpdateEnemy(std::string name, float x, float y) {
    enemies[name].SetPosition(x, y);
}

void Game::SetFocused(bool focused) {
    this->focused = focused;
}