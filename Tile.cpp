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

SDL_Surface* Tile::default_image  = NULL;
SDL_Surface* Tile::selected_image = NULL;

Tile::Tile() {
    character = NULL;

    set_selected(false);
}

Tile::Tile(int type) {
    switch(type) {
        case Util::PLAYER_WARRIOR:
            character = new PlayerWarrior(); break;
        case Util::PLAYER_ARCHER:
            character = new PlayerArcher();  break;
        case Util::PLAYER_HEALER:
            character = new PlayerHealer();  break;
        case Util::ENEMY_WARRIOR:
            character = new EnemyWarrior();  break;
        case Util::ENEMY_ARCHER:
            character = new EnemyArcher();   break;
        case Util::ENEMY_HEALER:
            character = new EnemyHealer();   break;
        default:
            character = NULL;
    }

    set_selected(false);
}

void Tile::set_selected(bool s) {
    selected = s;
    update_image();
}

bool Tile::get_selected() {
    return selected;
}

void Tile::update_image() {
    if (selected) {
        image = selected_image;
    } else {
        image = default_image;
    }
}

SDL_Surface* Tile::get_image()     { return image;     }
Character*   Tile::get_character() { return character; }

void Tile::character_died() { character = NULL; }
