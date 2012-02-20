#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include "Util.h"
#include "Grid.h"
#include "PlayerWarrior.h"
#include "PlayerArcher.h"
#include "PlayerHealer.h"

const int SCREEN_WIDTH  = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP    = 32;

SDL_Surface* screen = NULL;

TTF_Font* font = NULL;
SDL_Color textColor = { 255, 255, 255 };

SDL_Event event;

using namespace std;

int main(int argc, char* args[]) {
    bool quit = false;

    screen = Util::init_screen(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP);

    if (screen == NULL)   return 1;
    if (TTF_Init() == -1) return 1;

    font = TTF_OpenFont("fonts/04B-03/04B_03__.TTF", 16);
    if (font == NULL) return 1;

    //load_file();
    Grid grid;
    grid.draw_grid(screen);
    SDL_Flip(screen);

    SDL_Surface* sidebar = Util::load_image("sprites/sidebar-bg.png"); 

    while (quit == false) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                cout << "Quit Event" << endl;
                quit = true;
            } else if (event.type == SDL_MOUSEBUTTONDOWN &&
                       event.button.button == SDL_BUTTON_LEFT) {

                // integer division -- round down to the nearest multiple of SPRITE_SIZE
                int x = (event.button.x / Util::SPRITE_SIZE);
                int y = (event.button.y / Util::SPRITE_SIZE);

                Tile* selectedTile = grid.get(y, x);
                Character* selectedCharacter = selectedTile->get_character();

                if (selectedCharacter != NULL) {
                    Util::apply_surface(480, 0, sidebar, screen);
                    SDL_Flip(screen);

                    stringstream health;
                    stringstream strength;
                    stringstream mobility;
                    stringstream range;

                    health   << "Health: "   << selectedCharacter->get_health();   //<< "\n";
                    strength << "Strength: " << selectedCharacter->get_strength(); //<< "\n";
                    mobility << "Mobility: " << selectedCharacter->get_mobility(); //<< "\n";
                    range    << "Range: "    << selectedCharacter->get_range();    //<< "\n";

                    cout << "Health: " << selectedCharacter->get_health() << endl;
                    cout << "Strength: " << selectedCharacter->get_strength() << endl;
                    cout << "Mobility: " << selectedCharacter->get_mobility() << endl;
                    cout << "Range: " << selectedCharacter->get_range() << endl;

                    SDL_Surface* health_stats = TTF_RenderText_Solid(font, health.str().c_str(), textColor); 
                    SDL_Surface* strength_stats = TTF_RenderText_Solid(font, strength.str().c_str(), textColor); 
                    SDL_Surface* mobility_stats = TTF_RenderText_Solid(font, mobility.str().c_str(), textColor); 
                    SDL_Surface* range_stats = TTF_RenderText_Solid(font, range.str().c_str(), textColor); 

                    Util::apply_surface(500, 10, health_stats, screen);
                    Util::apply_surface(500, 30, strength_stats, screen);
                    Util::apply_surface(500, 50, mobility_stats, screen);
                    Util::apply_surface(500, 70, range_stats, screen);
                    SDL_Flip(screen);
                }
            }
        }
    }

    // TODO create cleanup function to free ALL the surfaces
    //SDL_FreeSurface(image);
    SDL_Quit();

    return 0;
}
