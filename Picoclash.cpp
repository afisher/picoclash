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
    SDL_FreeSurface(Tile::move_image);
    SDL_FreeSurface(Tile::attack_image);
    SDL_FreeSurface(Tile::heal_image);

    SDL_FreeSurface(RockTile::default_image);
    SDL_FreeSurface(RockTile::alt_image);
    SDL_FreeSurface(RockTile::selected_image);
    SDL_FreeSurface(RockTile::move_image);
    SDL_FreeSurface(RockTile::attack_image);
    SDL_FreeSurface(RockTile::heal_image);

    SDL_FreeSurface(Grid::grid_image);
    SDL_FreeSurface(Grid::sidebar);
    SDL_FreeSurface(surface);
    SDL_FreeSurface(screen);
    
    Grid::clean_up();
    Sound::clean_up();
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

    CheckButton::box_checked   = Util::load_image("sprites/box_checked.png");
    CheckButton::box_unchecked = Util::load_image("sprites/box_unchecked.png");

    if (screen == NULL)   return 1;
    if (TTF_Init() == -1) return 1;

    Grid::font = TTF_OpenFont("fonts/04B-03/04B_03__.TTF", 14);
    if (Grid::font == NULL) return 1;

    // make the sidebar surface
    Grid::sidebar = Util::load_image("sprites/sidebar-bg.png");

    StateMachine::init();
    Sound::init();

    // make the title screen surface
    SDL_Surface* title_screen = Util::load_image("sprites/title.png");
    Util::update_screen(title_screen, screen);

    while (quit == false) {
        if (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            } else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN) {
                break;
            }
        }
    }

    // make the map selector
    MapSelector* selector = new MapSelector();
    Util::update_screen(selector->get_surface(), screen);

    bool game_started = false;


    // make surfaces for win screens
    SDL_Surface* win_blue = Util::load_image("sprites/win_blue.png");
    SDL_Surface* win_red = Util::load_image("sprites/win_red.png");
    SDL_Surface* win_screen = NULL;

    while (quit == false) {

        int start_ticks = SDL_GetTicks();


        while (SDL_PollEvent(&event)) {

            if (event.type == SDL_QUIT) {
                quit = true;
            } else if (event.type == SDL_MOUSEBUTTONDOWN && !game_started) {
                if (event.button.button == SDL_BUTTON_LEFT) {
                    int x = event.button.x;
                    int y = event.button.y;
                    
                    MapButton* selected_button = selector->get_selected_button(x, y);
                    if (selected_button != NULL) {
                        string filename = selector->get_selected_button(x, y)->get_filename();

                        Grid::load_file(filename);
                        Grid::draw_grid(surface);

                        Grid::draw_sidebar(surface);

                        Util::update_screen(surface, screen);

                        game_started = true;
                    }
                }
            } else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_LEFT && !game_started) {
                selector->previous_page();
            } else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RIGHT && !game_started) {
                selector->next_page();
            } else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN && game_started && Grid::game_over()) {
                game_started = false;
            } else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_BACKSPACE && game_started) {
                game_started = false;
            } else if (game_started && Grid::get_game_type() != Constants::CPU_V_CPU) {
                StateMachine::execute(event, surface, screen);
            }
        }

        if (game_started && !Grid::game_over() && Grid::get_game_type() == Constants::CPU_V_CPU) {
            Grid::play_ai_turn(surface, screen);
        }

        int ticks = SDL_GetTicks() - start_ticks;
        if (ticks < 1000 / Constants::FRAMES_PER_SECOND) {
            SDL_Delay((1000 / Constants::FRAMES_PER_SECOND) - ticks);
        }

        if (Grid::game_over() && game_started) {
            if (Grid::get_player_characters().size() == 0) {
                win_screen = win_red;
            } else if (Grid::get_enemy_characters().size() == 0) {
                win_screen = win_blue;
            }

            Util::update_screen(win_screen, screen);
        } else if (game_started) {
            Util::update_screen(surface, screen);
        } else {
            Util::update_screen(selector->get_surface(), screen);
        }
    }

    delete selector;
    clean_up();
    SDL_Quit();

    return 0;
}
