#pragma once

#include "SFML/Graphics.hpp"

#include "character.h"
#include "game.h"

namespace render {

unsigned int CalcSizeX(const sf::Sprite& sprite);

unsigned int CalcSizeY(const sf::Sprite& sprite);

sf::Vector2u CalcSize(const sf::Sprite& sprite);

void RenderThread(sf::RenderWindow* window, Game* game);

void RenderBackground(sf::RenderWindow* window, sf::Sprite& sprite);

void RenderCharacter(sf::RenderWindow* window, Character& character);

void RenderEnemies(sf::RenderWindow* window, std::vector<Character>& enemies);

}
