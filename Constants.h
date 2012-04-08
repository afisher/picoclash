#pragma once

namespace Constants {
    const int FRAMES_PER_SECOND = 20;

    const int SCREEN_WIDTH  = 1280;
    const int SCREEN_HEIGHT = 960;
    const int SCREEN_BPP    = 32;

    const int PLAYER_WARRIOR = 1;
    const int PLAYER_ARCHER  = 2;
    const int PLAYER_HEALER  = 3;
    const int ENEMY_WARRIOR  = 4;
    const int ENEMY_ARCHER   = 5;
    const int ENEMY_HEALER   = 6;

    const int GRID_WIDTH  = 30;
    const int GRID_HEIGHT = 30;

    const int SPRITE_SIZE = 16;

    const int WIDTH  = 640;
    const int HEIGHT = 480;
    const double fWIDTH  = 640;
    const double fHEIGHT = 480;

    const double X_RATIO = WIDTH  / (double)SCREEN_WIDTH;
    const double Y_RATIO = HEIGHT / (double)SCREEN_HEIGHT;
};
