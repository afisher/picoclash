/*
 * This file is a part of Picoclash
 * Copyright (C) 2012 Ashley S. Fisher
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "Header.h"

using namespace std;

SDL_Surface* RockTile::default_image  = NULL;
SDL_Surface* RockTile::alt_image      = NULL;
SDL_Surface* RockTile::selected_image = NULL;
SDL_Surface* RockTile::move_image     = NULL;
SDL_Surface* RockTile::attack_image   = NULL;
SDL_Surface* RockTile::heal_image     = NULL;

RockTile::RockTile(int i, int j) : Tile(i, j) {
    update_image();
    update_overlay_image();
}

void RockTile::update_overlay_image() {
    if (selected) {
        overlay_image = selected_image;
    } else if (move_on) {
        overlay_image = move_image;
    } else if (attack_on) {
        overlay_image = attack_image;
    } else if (heal_on) {
        overlay_image = heal_image;
    } else {
        overlay_image = NULL;
    }
}

void RockTile::update_image() {
    if (use_alt) {
        image = alt_image;
    } else {
        image = default_image;
    }
}

bool RockTile::is_standable() { return false; }

SDL_Surface* RockTile::get_image()         { return image;     }
SDL_Surface* RockTile::get_overlay_image() { return overlay_image; }

RockTile::~RockTile() {
    SDL_FreeSurface(default_image);
    SDL_FreeSurface(alt_image);     
    SDL_FreeSurface(selected_image);
    SDL_FreeSurface(move_image);    
    SDL_FreeSurface(attack_image);  
    SDL_FreeSurface(heal_image);    
}
