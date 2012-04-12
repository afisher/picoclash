#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"

#include <string>
#include <vector>
#include <iostream>
#include <climits>
#include <cmath>
#include <fstream>
#include <sstream>

using namespace std;

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
        virtual bool attack    (SDL_Surface* surface);
};

class Tile {
    private:
        SDL_Surface* image;
        SDL_Surface* overlay_image;

        Character* character;

        bool selected;
        bool move_on;
        bool attack_on;

        bool use_alt; // whether or not this tile uses the alt image

        int x;
        int y;

        void update_image();
        void update_overlay_image();

    public:
        Tile(int i, int j);
        Tile(int i, int j, int type);

        void set_selected(bool s);
        void set_move_on(bool s);
        void set_attack_on(bool s);
        void set_character(Character* c);

        bool         get_selected();
        SDL_Surface* get_image();
        SDL_Surface* get_overlay_image();
        Character*   get_character();

        int get_x();
        int get_y();

        static SDL_Surface* selected_image; 
        static SDL_Surface* move_image; 
        static SDL_Surface* attack_image; 
        static SDL_Surface* alt_image; 
        static SDL_Surface* default_image;  

        void character_died();
};

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

class Archer : public Character {
    protected:
        void set_values(int p, int lvl);

    public:
        Archer(int p);
        Archer(int p, int lvl);

        virtual void move(SDL_Surface* surface);
        virtual bool attack(SDL_Surface* surface);
};

class AttackingState : public State {
    public:
        void execute(SDL_Event event, SDL_Surface* surface);
        std::string sidebar_tip();
};


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

class Grid {
    public:
        static SDL_Surface* grid_image;

        static void load_file();
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

        static void select_tiles       (int i, int j, int range, bool show);
        static void select_move_tiles  (int i, int j, int range, bool show);
        static void select_attack_tiles(int i, int j, int range, bool show);

        static bool show_move_tiles  (int i, int j, SDL_Surface* surface, bool show);
        static bool show_attack_tiles(int i, int j, SDL_Surface* surface, bool show);

        static std::vector<Tile*> get_character_tiles(int player);
        static std::vector<Tile*> get_range_tiles(Tile* character_tile, int range);

        static void play_ai_turn(SDL_Surface* surface, SDL_Surface* screen);

        static bool move  (int i, int j, int x, int y, SDL_Surface* surface);
        static bool attack(int i, int j, int x, int y, SDL_Surface* surface);
        static bool heal  (int i, int j, int x, int y, SDL_Surface* surface);

        static void new_turn();
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
        virtual bool attack(SDL_Surface* surface);
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

class Util {
    private:
        static void scale  (SDL_Surface* source, SDL_Surface* destination);
        static void bilinear_scale  (SDL_Surface* source, SDL_Surface* destination);
        static SDL_Surface* scale2x(SDL_Surface* source);
    public:
        static SDL_Surface* init_screen(int width, int height, int bpp);
        static void update_screen(SDL_Surface* source, SDL_Surface* destination);

        static SDL_Surface* load_image(std::string filename);
        static void apply_surface(int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip = NULL);
};

class Warrior : public Character {
    protected:
        void set_values(int p, int lvl);

    public:
        Warrior(int p);
        Warrior(int p, int lvl);

        virtual void move(SDL_Surface* surface);
        virtual bool attack(SDL_Surface* surface);
};
