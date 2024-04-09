#include "character.h"

Character::Character() {
    sprite.setPosition(100, 200);
}

Character::Character(sf::Texture& texture, float x, float y) {
    sprite.setTexture(texture);
    sprite.setPosition(x, y);
}

void Character::SetPosition(float x, float y) {
    sprite.setPosition(x, y);
}

void Character::SetPosition(sf::Vector2f position) {
    sprite.setPosition(position);
}

sf::Vector2f Character::GetPosition() {
    return sprite.getPosition();
}

sf::Sprite Character::GetSprite() {
    return sprite;
}