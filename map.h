#ifndef MAP_H
#define MAP_H

#include <vector>

struct Entity;

extern char** map_data;
extern int map_rows;
extern int map_cols;
extern int map_player_start_row;
extern int map_player_start_col;

void load_map(int difficulty, int level);

void free_map();

char get_map_char_at(int row, int col);

bool position_walkable(int row, int col);

bool at_exit_position(int row, int col);

void print_map(int player_row, int player_col, const std::vector<Entity>& enemies);

#endif
