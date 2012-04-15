#include "Header.h"

using namespace std;

Tile* grid[Constants::GRID_HEIGHT][Constants::GRID_WIDTH];
vector<Character*> player_characters;
vector<Character*> enemy_characters;
int current_player = 1;

SDL_Surface* Grid::grid_image = NULL;
bool grid_on = true;

// loads the test map into the grid
void Grid::load_file() {
    std::ifstream file("testmap.txt");
    std::string line;

    for (int j = 0; j < Constants::GRID_HEIGHT; j++) {
        std::getline(file, line);
        for (int i = 0; i < Constants::GRID_WIDTH; i++) {
            char c = line.at(i);
            std::cout << c;

            switch(c) {
                case 'w':
                    grid[j][i] = new Tile(i, j, Constants::PLAYER_WARRIOR); break;
                case 'a':
                    grid[j][i] = new Tile(i, j, Constants::PLAYER_ARCHER);  break;
                case 'h':
                    grid[j][i] = new Tile(i, j, Constants::PLAYER_HEALER);  break;
                case 'W':
                    grid[j][i] = new Tile(i, j, Constants::ENEMY_WARRIOR);  break;
                case 'A':
                    grid[j][i] = new Tile(i, j, Constants::ENEMY_ARCHER);   break;
                case 'H':
                    grid[j][i] = new Tile(i, j, Constants::ENEMY_HEALER);   break;
                case 'o':
                    grid[j][i] = new RockTile(i, j); break;
                default:
                    grid[j][i] = new Tile(i, j);
            }
        }
        std::cout << "\n";
    }

    file.close();
    grid_image = Util::load_image("sprites/grid.png");

}

// draw the grid -- assumes the file has been loaded
void Grid::draw_grid(SDL_Surface* surface) {
    for (int j = 0; j < Constants::GRID_HEIGHT; j++) {
        for (int i = 0; i < Constants::GRID_WIDTH; i++) {
            draw_tile(get(i, j), surface);
        }
    }
}

void Grid::draw_tile(Tile* tile, SDL_Surface* surface) {
    int i = tile->get_x();
    int j = tile->get_y();

    int x = i * Constants::SPRITE_SIZE;
    int y = j * Constants::SPRITE_SIZE;

    Util::apply_surface(x, y, get(i, j)->get_image(), surface);

    if (get(i, j)->get_overlay_image() != NULL) {
        Util::apply_surface(x, y, get(i, j)->get_overlay_image(), surface);
    }

    if (get(i, j)->get_character() != NULL) {
        Util::apply_surface(x, y, get(i, j)->get_character()->get_image(), surface);
    }

    SDL_Rect clip;
    clip.x = x;
    clip.y = y;
    clip.w = Constants::SPRITE_SIZE;
    clip.h = Constants::SPRITE_SIZE;

    if (grid_on) {
        Util::apply_surface(x, y, grid_image, surface, &clip);
    }
}

void Grid::draw_lines(SDL_Surface* surface) {
    if (grid_on) {
        Util::apply_surface(0, 0, grid_image, surface);
    }
}

void Grid::toggle_show_lines(SDL_Surface* surface) {
    grid_on = !grid_on;
    draw_grid(surface);
}

Tile* Grid::get(int i, int j)  { return grid[j][i];     }
int Grid::get_current_player() { return current_player; }

vector<Character*> Grid::get_player_characters() { return player_characters; }
vector<Character*> Grid::get_enemy_characters() { return enemy_characters; }

void Grid::add_player_character(Character* c) {
    player_characters.push_back(c);
}

void Grid::add_enemy_character(Character* c) {
    enemy_characters.push_back(c);
}

bool Grid::show_move_tiles(int i, int j, SDL_Surface* surface, bool show) {
    Character* selected_character = grid[j][i]->get_character();
    //if (selected_character->get_player() != current_player) return false;

    int mobility = 0;

    if (selected_character != NULL) {
        mobility = selected_character->get_mobility();
        select_move_tiles(i, j, mobility, show);
    } else return false;

    draw_grid(surface);

    return true;
}

bool Grid::show_attack_tiles(int i, int j, SDL_Surface* surface, bool show) {
    Character* selected_character = grid[j][i]->get_character();
    //if (selected_character->get_player() != current_player) return false;

    int range = 0;

    if (selected_character != NULL) {
        range = selected_character->get_range();
        select_attack_tiles(i, j, range, show);
    } else return false;

    draw_grid(surface);

    return true;
}

bool Grid::show_heal_tiles(int i, int j, SDL_Surface* surface, bool show) {
    Character* selected_character = grid[j][i]->get_character();
    //if (selected_character->get_player() != current_player) return false;

    int range = 0;

    if (selected_character != NULL) {
        range = selected_character->get_range();
        select_heal_tiles(i, j, range, show);
    } else return false;

    draw_grid(surface);

    return true;
}

void Grid::select_tiles(int i, int j, int range, bool show) {
    // interate over the range*range square
    for (int x = i - range; x <= i + range; x++) {
        for (int y = j - range; y <= j + range; y++) {
            // if the tile is within the range, light it up
            if (distance(i, j, x, y) <= range && x < Constants::GRID_WIDTH && y < Constants::GRID_HEIGHT
                                              && x >= 0 && y >= 0) {
                grid[y][x]->set_selected(show);
            }
        }
    }
}

void Grid::select_move_tiles(int i, int j, int range, bool show) {
    /*// interate over the range*range square
    for (int x = i - range; x <= i + range; x++) {
        for (int y = j - range; y <= j + range; y++) {
            // if the tile is within the range, light it up
            if (distance(i, j, x, y) <= range && x < Constants::GRID_WIDTH && y < Constants::GRID_HEIGHT
                                              && x >= 0 && y >= 0) {
                grid[y][x]->set_move_on(show);
            }
        }
    }*/

    vector<Tile*> move_tiles = get_move_tiles(get(i, j), range);
    for (int k = 0; k < move_tiles.size(); k++) {
        move_tiles[k]->set_move_on(show);
    }
}

void Grid::select_attack_tiles(int i, int j, int range, bool show) {
    // interate over the range*range square
    for (int x = i - range; x <= i + range; x++) {
        for (int y = j - range; y <= j + range; y++) {
            // if the tile is within the range, light it up
            if (distance(i, j, x, y) <= range && x < Constants::GRID_WIDTH && y < Constants::GRID_HEIGHT
                                              && x >= 0 && y >= 0) {
                grid[y][x]->set_attack_on(show);
            }
        }
    }
}

void Grid::select_heal_tiles(int i, int j, int range, bool show) {
    // interate over the range*range square
    for (int x = i - range; x <= i + range; x++) {
        for (int y = j - range; y <= j + range; y++) {
            // if the tile is within the range, light it up
            if (distance(i, j, x, y) <= range && x < Constants::GRID_WIDTH && y < Constants::GRID_HEIGHT
                                              && x >= 0 && y >= 0) {
                grid[y][x]->set_heal_on(show);
            }
        }
    }
}

vector<Tile*> Grid::get_character_tiles(int player) {
    vector<Tile*> ret;

    // find all of the characters that belong to the AI
    for (int j = 0; j < Constants::GRID_HEIGHT; j++) {
        for (int i = 0; i < Constants::GRID_WIDTH; i++) {
            Character* cur_char = grid[j][i]->get_character();

            // if an AI character, add to the list
            if (cur_char != NULL && cur_char->get_player() == player) {
                ret.push_back(grid[j][i]);
            }
        }
    }

    return ret;
}

vector<Tile*> Grid::get_neighbors(Tile* tile) {
    vector<Tile*> ret;

    int i = tile->get_x();
    int j = tile->get_y();

    // get left
    if (i > 0) ret.push_back(get(i-1, j));

    // get right
    if (i < Constants::GRID_WIDTH - 1) ret.push_back(get(i+1, j));

    // get up
    if (j > 0) ret.push_back(get(i, j-1));

    // get down
    if (j < Constants::GRID_HEIGHT - 1) ret.push_back(get(i, j+1));

    return ret;
}

vector<Tile*> Grid::get_range_tiles(Tile* character_tile, int range) {
    vector<Tile*> ret;

    int i = character_tile->get_x();
    int j = character_tile->get_y();

    // interate over the range*range square
    for (int x = i - range; x <= i + range; x++) {
        for (int y = j - range; y <= j + range; y++) {
            // if the tile is within the range, add it to the list of tiles we can move to
            if (distance(i, j, x, y) <= range && x < Constants::GRID_WIDTH && y < Constants::GRID_HEIGHT
                                              && x >= 0 && y >= 0) {
                ret.push_back(grid[y][x]);
            }
        }
    }

    return ret;
}

vector<Tile*> Grid::get_move_tiles(Tile* character_tile, int range) {
    set<Tile*> move_tiles;
    generate_move_tiles(character_tile, character_tile, range, &move_tiles);

    vector<Tile*> ret;
    copy(move_tiles.begin(), move_tiles.end(), back_inserter(ret));
    return ret;
}

// recursively generate all of the tiles a character can move to
void Grid::generate_move_tiles(Tile* character_tile, Tile* current_tile, int range, set<Tile*>* move_tiles) {
    int dist = distance(character_tile, current_tile);
    if (dist > range) return;

    if (current_tile->is_standable() && move_tiles->count(current_tile) == 0) {
        move_tiles->insert(current_tile);

        vector<Tile*> nbrs = get_neighbors(current_tile);
        for (int i = 0; i < nbrs.size(); i++) {
            generate_move_tiles(character_tile, nbrs[i], range, move_tiles);
        }
    }

}

void Grid::play_ai_turn(SDL_Surface* surface, SDL_Surface* screen) {
    for (int i = 0; i < enemy_characters.size(); i++) {
        Character* character = enemy_characters[i]; 
        if (character != NULL) {
            character->play_turn(surface, screen);
        }
    }
}

bool Grid::move(int i, int j, int x, int y, SDL_Surface* surface) {
    Character* cur_char = grid[j][i]->get_character();
    if (cur_char == NULL) { return false; }
    if (cur_char->get_player() != current_player) return false;

    int mobility = cur_char->get_mobility();
    vector<Tile*> move_tiles = get_move_tiles(get(i, j), mobility);


    Tile* selected_tile = grid[y][x];

    // move if we picked an empty square
    if (selected_tile->get_character() == NULL) {
        for (int k = 0; k < move_tiles.size(); k++) {
            if (move_tiles[k] == selected_tile) {
                selected_tile->set_character(cur_char);
                grid[j][i]->set_character(NULL);

                cur_char->set_moved_this_turn(true);

                return true;
            }
        }
    }

    return false;

    //grid[i][j]->get_character()->move(i, j, x, y, surface);
}

bool Grid::attack(int i, int j, int x, int y, SDL_Surface* surface) {
    Character* character1 = grid[j][i]->get_character();
    Character* character2 = grid[y][x]->get_character();

    if (character1->get_player() != current_player) return false;
    if (character1 == NULL || character2 == NULL) return false;

    int range = character1->get_range();

    if (distance(i, j, x, y) <= range && !character1->get_attacked_this_turn()
        && character1->get_player() != character2->get_player()) {
        character2->take_damage(character1->get_strength());
        if (character2->get_health() <= 0) {
            grid[y][x]->character_died();

            for (int i = 0; i < player_characters.size(); i++) {
                if (player_characters[i]->get_x() == x && player_characters[i]->get_y() == y) {
                    player_characters.erase(player_characters.begin()+i);
                }
            }

            for (int i = 0; i < enemy_characters.size(); i++) {
                if (enemy_characters[i]->get_x() == x && enemy_characters[i]->get_y() == y) {
                    enemy_characters.erase(enemy_characters.begin()+i);
                }
            }
        }

        get(i, j)->set_selected(false);
        select_attack_tiles(i, j, range, false);

        character1->set_attacked_this_turn(true);

        draw_grid(surface);

        return true;
    } else return false;
}

bool Grid::heal(int i, int j, int x, int y, SDL_Surface* surface) {
    Healer* character1 = (Healer*)(grid[j][i]->get_character());
    Character* character2 = grid[y][x]->get_character();

    if (character1 == NULL || character2 == NULL) return false;
    if (character1->get_player() != current_player) return false;
    if (character2->get_health() == character2->get_max_health()) return false;

    int range = character1->get_range();

    if (distance(i, j, x, y) <= range && !character1->get_attacked_this_turn()
        && character1->get_player() == character2->get_player()) {

        // heal for less if we're healing ourselves
        if (distance(i, j, x, y) == 0) {
            character2->gain_health(character1->get_magic() / 2);
        } else {
            character2->gain_health(character1->get_magic());
        }
        character1->set_healed_this_turn(true); // tell the first character that it just healed

        select_tiles(i, j, range, false);

        character1->set_attacked_this_turn(true);

        draw_grid(surface);

        return true;
    } else return false;
}

int Grid::distance(int i, int j, int x, int y) {
    return abs(i - x) + abs(j - y);
}

int Grid::distance(Tile* tile1, Tile* tile2) {
    return abs(tile1->get_x() - tile2->get_x()) + abs(tile1->get_y() - tile2->get_y());
}

void Grid::new_turn() {
    for (int j = 0; j < Constants::GRID_HEIGHT; j++) {
        for (int i = 0; i < Constants::GRID_WIDTH; i++) {
            Character* curChar = grid[j][i]->get_character();

            if (curChar != NULL) {
                curChar->set_moved_this_turn(false);
                curChar->set_attacked_this_turn(false);
                if (curChar->can_heal()) {
                    ((Healer*)curChar)->set_healed_this_turn(false);
                }
            }
        }
    }

    if (current_player == 1) current_player = 2;
    else current_player = 1;
}
