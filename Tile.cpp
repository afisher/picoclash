#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "Util.h"
#include "Tile.h"
#include "PlayerWarrior.h"
#include "PlayerArcher.h"
#include "PlayerHealer.h"
#include "EnemyWarrior.h"
#include "EnemyArcher.h"
#include "EnemyHealer.h"

Tile::Tile() {
    image = Util::load_image("sprites/grass2.png");
    character = NULL;
}

Tile::Tile(int type) {
    image = Util::load_image("sprites/grass2.png");
    
    switch(type) {
        case Util::PLAYER_WARRIOR:
            character = new PlayerWarrior(); break;
        case Util::PLAYER_ARCHER:
            character = new PlayerArcher(); break;
        case Util::PLAYER_HEALER:
            character = new PlayerHealer(); break;
        case Util::ENEMY_WARRIOR:
            character = new EnemyWarrior(); break;
        case Util::ENEMY_ARCHER:
            character = new EnemyArcher(); break;
        case Util::ENEMY_HEALER:
            character = new EnemyHealer(); break;
        default:
            character = NULL;
    }
}

SDL_Surface* Tile::get_image()     { return image;     }
Character*   Tile::get_character() { return character; }
