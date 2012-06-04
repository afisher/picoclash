#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"
#include "SDL/SDL_mixer.h"

#include <string>
#include <vector>
#include <set>
#include <iterator>
#include <iostream>
#include <climits>
#include <cmath>
#include <fstream>
#include <sstream>
#include <algorithm>

using namespace std;

/* --------------------- *
 * Global game constants *
 * --------------------- */
namespace Constants {
    const int FRAMES_PER_SECOND = 60;

    const int SCREEN_BPP    = 32;

    const int PLAYER_WARRIOR = 1;
    const int PLAYER_ARCHER  = 2;
    const int PLAYER_HEALER  = 3;
    const int ENEMY_WARRIOR  = 4;
    const int ENEMY_ARCHER   = 5;
    const int ENEMY_HEALER   = 6;

    const int P_V_CPU   = 1;
    const int P_V_P     = 2;
    const int CPU_V_CPU = 3;

    const int GRID_WIDTH  = 30;
    const int GRID_HEIGHT = 30;

    const int SPRITE_SIZE = 16;

    const int WIDTH  = 640;
    const int HEIGHT = 480;

};

/* ----------------- *
 * Character classes *
 * ----------------- */
class Character {
    protected:
        int player;
        int level;
        int mobility;
        int health;
        int strength;
        int range;
        int increment;

        int x;
        int y;

        bool moved_this_turn;
        bool attacked_this_turn;

        std::string name;

        SDL_Surface* image;
        SDL_Surface* grey_image;

    public:
        Character();
        ~Character();

        int get_player();
        int get_level();
        int get_mobility();
        int get_health();
        int get_strength();
        int get_range();

        int get_max_health();

        int get_x();
        int get_y();
        void set_x(int new_x);
        void set_y(int new_y);

        void take_damage(int d);
        void gain_health(int h);

        bool get_moved_this_turn();
        bool get_attacked_this_turn();
        virtual bool get_healed_this_turn();

        virtual bool can_heal();

        std::string get_name();
        virtual SDL_Surface* get_image();

        void set_moved_this_turn(bool moved);
        void set_attacked_this_turn(bool attacked);

        // for AI
        virtual void play_turn (SDL_Surface* surface, SDL_Surface* screen);
        virtual void move      (SDL_Surface* surface);
        virtual bool attack    (SDL_Surface* surface, SDL_Surface* screen);
};

class Archer : public Character {
    protected:
        void set_values(int p, int lvl);

    public:
        Archer(int p);
        Archer(int p, int lvl);

        virtual void move(SDL_Surface* surface);
        virtual bool attack(SDL_Surface* surface, SDL_Surface* screen);
};

class Warrior : public Character {
    protected:
        void set_values(int p, int lvl);

    public:
        Warrior(int p);
        Warrior(int p, int lvl);

        virtual void move(SDL_Surface* surface);
        virtual bool attack(SDL_Surface* surface, SDL_Surface* screen);
};

class Healer : public Character {
    protected:
        int magic;
        bool healed_this_turn;
        void set_values(int p, int lvl);

    public:
        Healer(int p);
        Healer(int p, int lvl);

        int get_magic();

        virtual bool get_healed_this_turn();
        void set_healed_this_turn(bool h);

        virtual bool can_heal();

        virtual void move(SDL_Surface* surface);
        virtual bool attack(SDL_Surface* surface, SDL_Surface* screen);
};

/* ------------ *
 * Tile classes *
 * ------------ */
class Tile {
    protected:
        SDL_Surface* image;
        SDL_Surface* overlay_image;

        Character* character;

        bool selected;
        bool move_on;
        bool attack_on;
        bool heal_on;

        bool use_alt; // whether or not this tile uses the alt image

        int x;
        int y;

        virtual void update_image();
        virtual void update_overlay_image();

    public:
        Tile(int i, int j);
        Tile(int i, int j, int type);
        ~Tile();

        void set_selected(bool s);
        void set_move_on(bool s);
        void set_attack_on(bool s);
        void set_heal_on(bool s);
        void set_character(Character* c);

        bool get_selected();
        virtual SDL_Surface* get_image();
        virtual SDL_Surface* get_overlay_image();
        Character* get_character();

        virtual bool is_standable();

        int get_x();
        int get_y();

        static SDL_Surface* selected_image; 
        static SDL_Surface* move_image; 
        static SDL_Surface* attack_image; 
        static SDL_Surface* heal_image; 
        static SDL_Surface* alt_image; 
        static SDL_Surface* default_image;  

        void character_died();

        bool operator<(Tile other);
        bool operator>(Tile other);
        bool operator==(Tile other);
};

class RockTile : public Tile {
    public:
        RockTile(int i, int j);
        ~RockTile();

        virtual bool is_standable();

        virtual void update_image();
        virtual void update_overlay_image();
        virtual SDL_Surface* get_image();
        virtual SDL_Surface* get_overlay_image();

        static SDL_Surface* selected_image; 
        static SDL_Surface* move_image; 
        static SDL_Surface* attack_image; 
        static SDL_Surface* heal_image; 
        static SDL_Surface* alt_image; 
        static SDL_Surface* default_image;  
};

/* --------------------- * 
 * State Machine classes *
 * --------------------- */
class State {
    public:
        // does appropriate stuff based on the event, and returns new state
        virtual void execute(SDL_Event event, SDL_Surface* surface) {};
        virtual std::string sidebar_tip() { return ""; };
};

class StateMachine {
    public:
        static void init();
        static void execute(SDL_Event event, SDL_Surface* surface, SDL_Surface* screen);

        static void set_current_state(State* s);
        static void set_previous_state(State* s);

        static void set_selected_tile(Tile* t);
        static void set_inspected_tile(Tile* t);

        static State* get_current_state();
        static State* get_previous_state();

        static Tile* get_selected_tile();
        static Tile* get_inspected_tile();
};

class AttackingState : public State {
    public:
        void execute(SDL_Event event, SDL_Surface* surface);
        std::string sidebar_tip();
};

class HealingState : public State {
    public:
        void execute(SDL_Event event, SDL_Surface* surface);
        std::string sidebar_tip();
};

class IdleState : public State {
    public:
        void execute(SDL_Event event, SDL_Surface* surface);
        std::string sidebar_tip();
};

class MovingState : public State {
    public:
        void execute(SDL_Event event, SDL_Surface* surface);
        std::string sidebar_tip();
};

class SelectedState : public State {
    public:
        void execute(SDL_Event event, SDL_Surface* surface);
        std::string sidebar_tip();
};


/* ---------------------- * 
 * The Grid / game screen *
 * ---------------------- */
class Grid {
    public:
        static SDL_Surface* grid_image;
        static SDL_Surface* sidebar;

        static TTF_Font* font;

        static void set_game_type(int type);
        static int get_game_type();

        static void load_file(std::string filename);
        static void draw_grid(SDL_Surface* surface);
        static void draw_tile(Tile* tile, SDL_Surface* surface);
        static void draw_lines(SDL_Surface* surface);

        static void toggle_show_lines(SDL_Surface* surface);

        static Tile* get(int i, int j);

        static int get_current_player();

        static vector<Character*> get_player_characters();
        static vector<Character*> get_enemy_characters();

        static void add_player_character(Character* c);
        static void add_enemy_character(Character* c);

        static int distance(int i, int j, int x, int y);
        static int distance(Tile* tile1, Tile* tile2);
        static double sqrt_distance(Tile* tile1, Tile* tile2);

        static void select_tiles       (int i, int j, int range, bool show);
        static void select_move_tiles  (int i, int j, int range, bool show);
        static void select_attack_tiles(int i, int j, int range, bool show);
        static void select_heal_tiles(int i, int j, int range, bool show);

        static bool show_move_tiles  (int i, int j, SDL_Surface* surface, bool show);
        static bool show_attack_tiles(int i, int j, SDL_Surface* surface, bool show);
        static bool show_heal_tiles(int i, int j, SDL_Surface* surface, bool show);

        static std::vector<Tile*> get_neighbors(Tile* tile);
        static std::vector<Tile*> get_character_tiles(int player);
        static std::vector<Tile*> get_range_tiles(Tile* character_tile, int range);
        static std::vector<Tile*> get_move_tiles(Tile* character_tile, int range);

        static void generate_move_tiles(Tile* character_tile, Tile* current_tile, int range, std::set<Tile*>* move_tiles);

        static bool has_path(Tile* character_tile, Tile* destination, int range);

        static void play_ai_turn(SDL_Surface* surface, SDL_Surface* screen);

        static bool move  (int i, int j, int x, int y, SDL_Surface* surface);
        static bool attack(int i, int j, int x, int y, SDL_Surface* surface);
        static bool heal  (int i, int j, int x, int y, SDL_Surface* surface);

        static vector<Tile*> path_search(Tile* start, Tile* end);
        static vector<Tile*> reconstruct_path(std::vector<std::vector<Tile*> > came_from, Tile* current);

        static void new_turn();

        static bool game_over();

        static void draw_sidebar(SDL_Surface* surface);

        static void clean_up();
};


/* ---------------------- * 
 * Graphics utility class *
 * ---------------------- */
class Util {
    public:
        static double X_RATIO;
        static double Y_RATIO;

        static SDL_Surface* init_screen(int width, int height, int bpp);
        static void update_screen(SDL_Surface* source, SDL_Surface* destination);

        static void update_ratios(SDL_Surface* screen);

        static SDL_Surface* load_image(std::string filename);
        static void apply_surface(int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip = NULL);

        static void scale  (SDL_Surface* source, SDL_Surface* destination);
        static void bilinear_scale  (SDL_Surface* source, SDL_Surface* destination);
        static SDL_Surface* scale2x(SDL_Surface* source);
};


/* -------------------- *
 * Map selector classes *
 * -------------------- */
class MapButton {
    private:
        std::string name;
        std::string filename;
        SDL_Surface* preview;
        SDL_Surface* name_surface;
        SDL_Surface* surface;
        int width;
        int height;
    public:
        MapButton(std::string filename);
        ~MapButton();

        int get_width();
        int get_height();

        SDL_Surface* get_button();
        std::string get_filename();
};

class CheckButton {
    private:
        SDL_Surface* check_box;
        SDL_Surface* surface;

        int width;
        int height;
    public:
        CheckButton(std::string label_string, bool checked);
        ~CheckButton();

        int get_width();
        int get_height();

        SDL_Surface* get_button();

        void set_checked();
        void set_unchecked();

        static SDL_Surface* box_checked;
        static SDL_Surface* box_unchecked;
};

class MapSelector {
    private:
        std::vector<MapButton*> buttons;
        std::vector<SDL_Surface*> pages;

        int x_padding;
        int y_padding;

        int current_page;
        int buttons_per_page;

        CheckButton* p_vs_cpu_button;
        CheckButton* p_vs_p_button;
        CheckButton* cpu_vs_cpu_button;

        CheckButton* previous_check_button;

        bool in_bounds(int x, int y, MapButton* button, int button_number);
    public:
        MapSelector();
        ~MapSelector();

        SDL_Surface* get_surface();
        MapButton* get_selected_button(int x, int y);

        void next_page();
        void previous_page();

        void p_vs_cpu_clicked();
        void p_vs_p_clicked();
        void cpu_vs_cpu_clicked();

        void draw_check_buttons();
};

/* ----------- *
 * Sound class *
 * ----------- */
class Sound {
    public:
        static void init();
        static void clean_up();
        
        static void play_hit();
        static void play_heal();
        static void play_death();
        static void play_move();
};
