#ifndef _GAME_H_
#define _GAME_H_

#include "map.h"
#include "cuss.h"
#include "worldmap.h"
#include "monster.h"
#include "player.h"
#include "keybind.h"
#include "pathfind.h"
#include "time.h"

// Identical messages within MESSAGE_GAP turns of each other are combined
#define MESSAGE_GAP 3

struct Game_message
{
  std::string text;
  int turn;
  int count;
  Game_message() { turn = 0; count = 1; text = ""; }
  Game_message(std::string T, int TN) : text (T), turn (TN) { count = 1; }
};

class Game
{
public:
  Game();
  ~Game();

// Setup - Called only once
  bool setup();

// Engine - Main loop functions
  bool main_loop();
  void do_action(Interface_action act);
  void move_entities();
  void clean_up_dead_entities();
  void handle_player_activity();
  void complete_player_activity();

// Engine - Called-as-needed
  void shift_if_needed();  // Shift the map, if the player's not in the center
  void make_sound(std::string desc, Tripoint pos);
  void make_sound(std::string desc, Point pos);
  void make_sound(std::string desc, int x, int y);

// Pass NULL as shooter and Item() as it
  void launch_projectile(Ranged_attack attack,
                         Tripoint origin, Tripoint target);
// Pass NULL as shooter
  void launch_projectile(Item it, Ranged_attack attack,
                         Tripoint origin, Tripoint target);
// Pass Item() as it
  void launch_projectile(Entity* shooter, Ranged_attack attack,
                         Tripoint origin, Tripoint target);
// This one is the *real* one
  void launch_projectile(Entity* shooter, Item it, Ranged_attack attack,
                         Tripoint origin, Tripoint target);

  void player_move(int xdif, int ydif); // Handles all aspects of moving player
  void player_move_vertical(int zdif);
  void add_msg(std::string msg, ...);

// UI - Output functions
  void draw_all();
  void update_hud();
  void print_messages();

// UI - Special screens
  void pickup_items(Tripoint pos);
  void pickup_items(Point    pos);
  void pickup_items(int posx, int posy);
// TODO: Both are limited in that they can not return a point that the player
//       cannot currently see (they return Tripoint() instead).
  Tripoint target_selector(int startx = -1, int starty = -1,
                           int range  = -1, bool target_entites = false);
  std::vector<Tripoint> path_selector(int startx = -1, int starty = -1,
                                      int range  = -1,
                                      bool target_entities = false);

// Data - Universal access functions
  int get_item_uid();
  bool minute_timer(int minutes); // Returns true once every $minutes minutes
  bool turn_timer(int turns);     // Returns true once every $turns turns
  int get_light_level();          // Current light distance, based on the time

  Map*        map;
  Worldmap*   worldmap;
  Player*     player;
  Entity_pool entities;

  Time time;

  Generic_map scent_map;

private:
  Window *w_map;
  Window *w_hud;
  cuss::interface i_hud;
  std::vector<Game_message> messages;
  int last_target;
  int new_messages;
  int next_item_uid;
  bool game_over;
};

#endif
