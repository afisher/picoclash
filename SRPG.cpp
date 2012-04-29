#include "Header.h"

using namespace std;

SDL_Surface* screen = NULL;
SDL_Surface* surface = NULL;
SDL_Surface* sidebar = NULL;


SDL_Event event;


static void clean_up() {
    SDL_FreeSurface(Tile::default_image);
    SDL_FreeSurface(Tile::alt_image);
    SDL_FreeSurface(Tile::selected_image);
    SDL_FreeSurface(Grid::grid_image);
    SDL_FreeSurface(Grid::sidebar);
    SDL_FreeSurface(surface);
    SDL_FreeSurface(screen);
    
    Grid::clean_up();
}

int main(int argc, char* args[]) {
    bool quit = false;

    screen = Util::init_screen(Constants::SCREEN_WIDTH,
                               Constants::SCREEN_HEIGHT,
                               Constants::SCREEN_BPP);

    surface = SDL_CreateRGBSurface(SDL_SWSURFACE,
                                   Constants::WIDTH,
                                   Constants::HEIGHT,
                                   Constants::SCREEN_BPP, 0, 0, 0, 0);

    Tile::default_image  = Util::load_image("sprites/grass-plain.png");
    Tile::alt_image      = Util::load_image("sprites/grass-alt.png");
    Tile::selected_image = Util::load_image("sprites/overlay-selected.png");
    Tile::move_image     = Util::load_image("sprites/overlay-move.png");
    Tile::attack_image   = Util::load_image("sprites/overlay-attack.png");
    Tile::heal_image     = Util::load_image("sprites/overlay-heal.png");

    RockTile::default_image  = Util::load_image("sprites/rock.png");
    RockTile::alt_image      = Util::load_image("sprites/rock.png");
    RockTile::selected_image = Util::load_image("sprites/overlay-selected.png");
    RockTile::move_image     = Util::load_image("sprites/overlay-move.png");
    RockTile::attack_image   = Util::load_image("sprites/overlay-attack.png");
    RockTile::heal_image     = Util::load_image("sprites/overlay-heal.png");

    if (screen == NULL)   return 1;
    if (TTF_Init() == -1) return 1;

    Grid::font = TTF_OpenFont("fonts/04B-03/04B_03__.TTF", 14);
    if (Grid::font == NULL) return 1;

    StateMachine::init();

    MapSelector* selector = new MapSelector();
    Util::update_screen(selector->get_surface(), screen);

    bool game_started = false;

    while (quit == false) {

        int start_ticks = SDL_GetTicks();

        while (SDL_PollEvent(&event)) {

            if (event.type == SDL_QUIT) {
                cout << "Quit Event" << endl;
                quit = true;
            } else if (event.type == SDL_MOUSEBUTTONDOWN && !game_started) {
                //If the left mouse button was pressed
                if (event.button.button == SDL_BUTTON_LEFT) {
                    int x = event.button.x;
                    int y = event.button.y;
                    
                    MapButton* selected_button = selector->get_selected_button(x, y);
                    if (selected_button != NULL) {
                        string filename = selector->get_selected_button(x, y)->get_filename();

                        Grid::load_file(filename);
                        Grid::draw_grid(surface);
                        Util::update_screen(surface, screen);

                        Grid::sidebar = Util::load_image("sprites/sidebar-bg.png");
                        Grid::draw_sidebar(surface);

                        game_started = true;
                    }
                }
            }

            if (game_started) {
                StateMachine::execute(event, surface, screen);

            }
        }

        int ticks = SDL_GetTicks() - start_ticks;
        if (ticks < 1000 / Constants::FRAMES_PER_SECOND) {
            SDL_Delay((1000 / Constants::FRAMES_PER_SECOND) - ticks);
        }

        if (game_started) {
            Grid::draw_sidebar(surface);
            Util::update_screen(surface, screen);
        }
    }

    delete selector;
    clean_up();
    SDL_Quit();

    return 0;
}
