#include "Header.h"

Mix_Chunk* hit    = NULL;
Mix_Chunk* heal   = NULL;
Mix_Chunk* death  = NULL;
Mix_Chunk* hover  = NULL;
Mix_Chunk* move   = NULL;

void Sound::init() {
    Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096);

    hit    = Mix_LoadWAV("sounds/hit.wav");
    heal   = Mix_LoadWAV("sounds/heal.wav");
    death  = Mix_LoadWAV("sounds/dead.wav");
    hover  = Mix_LoadWAV("sounds/hover.wav");
    move   = Mix_LoadWAV("sounds/hover.wav");
}

void Sound::clean_up() {
    Mix_FreeChunk(hit);
    Mix_FreeChunk(heal);
    Mix_FreeChunk(death);
    Mix_FreeChunk(hover);
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

void Sound::play_hover() {
    Mix_PlayChannel(-1, hover, 0);
}

void Sound::play_move() {
    Mix_PlayChannel(-1, move, 0);
}