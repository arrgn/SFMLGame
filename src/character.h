#pragma once

#include "SFML/Graphics.hpp"

class Character {
public:
    Character();

    Character(sf::Texture& texture, float x = 100, float y = 100);

    void SetPosition(float x, float y);

    void SetPosition(sf::Vector2f position);

    sf::Vector2f GetPosition();

    sf::Sprite GetSprite();

private:
    sf::Vector2f position;
    sf::Sprite sprite;
};