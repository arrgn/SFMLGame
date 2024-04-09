#pragma once

#include <map>
#include <string>

#include "SFML/Graphics.hpp"

#include "character.h"

class Game {
public:
    Game();

    Character& GetPlayer();

    std::map<std::string, Character>& GetEnemies();

    void Update(sf::Time);
    
    void AddEnemy(float x, float y, std::string name);

    void RemoveEnemy(std::string name);

    void UpdateEnemy(std::string name, float x, float y);

    void SetFocused(bool focused);

private:
    sf::Texture player_texture;
    std::map<std::string, Character> enemies;
    Character player;
    float speed;
    bool focused;
};