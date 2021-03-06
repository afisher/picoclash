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

Mix_Chunk* hit    = NULL;
Mix_Chunk* heal   = NULL;
Mix_Chunk* death  = NULL;
Mix_Chunk* move   = NULL;

void Sound::init() {
    const int frequency = 22050;
    const int channels = 2;
    const int sample_size = 4096;

    Mix_OpenAudio(frequency, MIX_DEFAULT_FORMAT, channels, sample_size);

    hit    = Mix_LoadWAV("sounds/hit.wav");
    heal   = Mix_LoadWAV("sounds/heal.wav");
    death  = Mix_LoadWAV("sounds/dead.wav");
    move   = Mix_LoadWAV("sounds/move.wav");
}

void Sound::clean_up() {
    Mix_FreeChunk(hit);
    Mix_FreeChunk(heal);
    Mix_FreeChunk(death);
    Mix_FreeChunk(move);

    Mix_CloseAudio();
}

void Sound::play_hit() {
    Mix_PlayChannel(-1, hit, 0);
}

void Sound::play_heal() {
    Mix_PlayChannel(-1, heal, 0);
}

void Sound::play_death() {
    Mix_PlayChannel(-1, death, 0);
}

void Sound::play_move() {
    Mix_PlayChannel(-1, move, 0);
}
