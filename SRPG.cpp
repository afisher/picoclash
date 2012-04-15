#include "Header.h"

using namespace std;

SDL_Surface* screen = NULL;
SDL_Surface* surface = NULL;
SDL_Surface* sidebar = NULL;

TTF_Font* font = NULL;

SDL_Event event;

// temporary home for the possible states
/*static enum States { IDLE, SELECTED, MOVING, ATTACKING, HEALING };

static int state = IDLE;*/

static void draw_sidebar() {
    SDL_Color text_color = { 255, 255, 255 };
    SDL_Color grey_color = { 130, 130, 130 };

    Util::apply_surface(480, 0, sidebar, surface);

    Tile* selected_tile = StateMachine::get_selected_tile();
    Tile* inspected_tile = StateMachine::get_inspected_tile();

    if (selected_tile == NULL || inspected_tile == NULL) return;

    Character* selected_character = selected_tile->get_character();
    Character* inspected_character = inspected_tile->get_character();

    if (inspected_character != NULL) {
        // Build stat info
        stringstream health;
        stringstream strength;
        stringstream mobility;
        stringstream range;

        health   << "Health: "   << inspected_character->get_health()
                 << " / "        << inspected_character->get_max_health();
        strength << "Strength: " << inspected_character->get_strength();
        mobility << "Mobility: " << inspected_character->get_mobility();
        range    << "Range: "    << inspected_character->get_range();

        SDL_Surface* health_stats   = TTF_RenderText_Solid(font, health.str().c_str(),   text_color);
        SDL_Surface* strength_stats = TTF_RenderText_Solid(font, strength.str().c_str(), text_color);
        SDL_Surface* mobility_stats = TTF_RenderText_Solid(font, mobility.str().c_str(), text_color);
        SDL_Surface* range_stats    = TTF_RenderText_Solid(font, range.str().c_str(),    text_color);

        Util::apply_surface(486, 10, health_stats, surface);
        Util::apply_surface(486, 30, strength_stats, surface);
        Util::apply_surface(486, 50, mobility_stats, surface);
        Util::apply_surface(486, 70, range_stats, surface);

        SDL_FreeSurface(health_stats);
        SDL_FreeSurface(strength_stats);
        SDL_FreeSurface(mobility_stats);
        SDL_FreeSurface(range_stats);


        if (StateMachine::get_selected_tile() != NULL && selected_character != NULL) {
            // Build controls info
            string move   = "z - Move";
            string attack = "x - Attack";
            string heal   = "c - Heal";

            SDL_Color color;
            if (selected_character->get_moved_this_turn()) {
                color = grey_color;
            } else {
                color = text_color;
            }
            SDL_Surface* move_control = TTF_RenderText_Solid(font, move.c_str(), color);

            if (selected_character->get_attacked_this_turn()) {
                color = grey_color;
            } else {
                color = text_color;
            }
            SDL_Surface* attack_control = TTF_RenderText_Solid(font, attack.c_str(), color);

            if (!selected_character->can_heal() || selected_character->get_attacked_this_turn()) {
                color = grey_color;
            } else {
                color = text_color;
            }
            SDL_Surface* heal_control = TTF_RenderText_Solid(font, heal.c_str(), color);

            Util::apply_surface(486, 200, move_control, surface);
            Util::apply_surface(486, 220, attack_control, surface);
            Util::apply_surface(486, 240, heal_control, surface);

            SDL_FreeSurface(move_control);
            SDL_FreeSurface(attack_control);
            SDL_FreeSurface(heal_control);
        }
    }

    string end = "v - End turn";
    SDL_Surface* end_control = TTF_RenderText_Solid(font, end.c_str(), text_color);
    Util::apply_surface(486, 260, end_control, surface);
    SDL_FreeSurface(end_control);

    string cancel = "Esc - Cancel";
    SDL_Surface* cancel_control = TTF_RenderText_Solid(font, cancel.c_str(), text_color);
    Util::apply_surface(486, 280, cancel_control, surface);
    SDL_FreeSurface(cancel_control);

    string grid = "Shift - Toggle grid";
    SDL_Surface* grid_control = TTF_RenderText_Solid(font, grid.c_str(), text_color);
    Util::apply_surface(486, 300, grid_control, surface);
    SDL_FreeSurface(grid_control);

    string turn_str = "";
    int current_player = Grid::get_current_player();
    if (current_player == 1) {
        turn_str = "Blue turn";
    } else {
        turn_str = "Red turn";
    }
    SDL_Surface* turn_info = TTF_RenderText_Solid(font, turn_str.c_str(), text_color);
    Util::apply_surface(486, 440, turn_info, surface);
    SDL_FreeSurface(turn_info);

    string state_str = StateMachine::get_current_state()->sidebar_tip();
    SDL_Surface* state_info = TTF_RenderText_Solid(font, state_str.c_str(), text_color);
    Util::apply_surface(486, 460, state_info, surface);
    SDL_FreeSurface(state_info);
}

static void clean_up() {
    SDL_FreeSurface(Tile::default_image);
    SDL_FreeSurface(Tile::alt_image);
    SDL_FreeSurface(Tile::selected_image);
    SDL_FreeSurface(Grid::grid_image);
    SDL_FreeSurface(sidebar);
    SDL_FreeSurface(surface);
    SDL_FreeSurface(screen);
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

    font = TTF_OpenFont("fonts/04B-03/04B_03__.TTF", 14);
    if (font == NULL) return 1;

    Grid::load_file();
    Grid::draw_grid(surface);
    Util::update_screen(surface, screen);

    sidebar = Util::load_image("sprites/sidebar-bg.png");
    draw_sidebar();

    bool success;

    StateMachine::init();

    while (quit == false) {

        int start_ticks = SDL_GetTicks();

        while (SDL_PollEvent(&event)) {

            if (event.type == SDL_QUIT) {
                cout << "Quit Event" << endl;
                quit = true;
            } else {
                StateMachine::execute(event, surface, screen);
            }
        }
        draw_sidebar();


        int ticks = SDL_GetTicks() - start_ticks;
        if (ticks < 1000 / Constants::FRAMES_PER_SECOND) {
            SDL_Delay((1000 / Constants::FRAMES_PER_SECOND) - ticks);
        }

        Util::update_screen(surface, screen);
    }

    clean_up();
    SDL_Quit();

    return 0;
}
