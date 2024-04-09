#include "SFML/Graphics.hpp"

#include "character.h"
#include "game.h"

namespace render {

unsigned int CalcSizeX(const sf::Sprite& sprite) {
    return sprite.getTexture()->getSize().x * sprite.getScale().x;
}

unsigned int CalcSizeY(const sf::Sprite& sprite) {
    return sprite.getTexture()->getSize().y * sprite.getScale().y;
}

sf::Vector2u CalcSize(const sf::Sprite& sprite) {
    return sf::Vector2u(CalcSizeX(sprite), CalcSizeY(sprite));
}

void RenderBackground(sf::RenderWindow* window, sf::Sprite& bg_sprite) {
    auto size = CalcSize(bg_sprite);
    for (unsigned int x = 0; x * size.x < window->getSize().x; x++) {
        for (unsigned int y = 0; y * size.y < window->getSize().y; y++) {
            bg_sprite.setPosition(x * size.x, y * size.y);
            window->draw(bg_sprite);
        }
    }
}

void RenderCharacter(sf::RenderWindow* window, Character& character) {
    window->draw(character.GetSprite());
}

void RenderEnemies(sf::RenderWindow* window, std::map<std::string, Character>& enemies) {
    for (auto& enemy : enemies) {
        RenderCharacter(window, enemy.second);
    }
}

void RenderThread(sf::RenderWindow* window, Game* game) {
    sf::Texture pt;
    if (!pt.loadFromFile("imgs/AnimationSheet_Character.png", sf::IntRect(0, 0, 32, 32)))
            abort();

    sf::Texture sand_texture;

    if (!sand_texture.loadFromFile("imgs/sand.png"))
        abort();

    sf::Sprite sand_sprite(sand_texture);

    sand_sprite.setScale(4.0f, 4.0f);

    window->setActive(true);

    while (window->isOpen()) {
        window->clear();

        RenderBackground(window, sand_sprite);

        RenderCharacter(window, game->GetPlayer());
        RenderEnemies(window, game->GetEnemies());

        window->display();
    }
}

}