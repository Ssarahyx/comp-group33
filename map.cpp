#include "map.h"
#include "entity.h"
#include <iostream>
#include <vector>
#include <cstdlib>
#include <cmath>

using namespace std;

#define color_reset  "\033[0m"
#define color_player "\033[34m"
#define color_enemy  "\033[31m"
#define color_exit   "\033[93m"
#define color_wall   "\033[37m"
#define color_text   "\033[0m"

char** map_data = nullptr;
int map_rows = 0;
int map_cols = 0;

int map_player_start_row = 0;
int map_player_start_col = 0;

//clear_map function frees all dynamic memory used by the map and resets its size.
//The function has no inputs.
//Output is that map_data becomes nullptr and map_rows & map_cols are set to 0.
static void clear_map() {
    if (map_data != nullptr) {
        for (int row = 0; row < map_rows; ++row) {
            delete[] map_data[row];
        }
        delete[] map_data;
        map_data = nullptr;
    }
    map_rows = 0;
    map_cols = 0;
}

//get_map_parameters function sets the map size and the percentages of walls and enemies.
//Inputs are difficulty (1 = easy, 2 = normal, 3 = hard) and level (1–3).
//Outputs are rows, cols, wall_percent, and enemy_percent used to generate the map.
static void get_map_parameters(int difficulty, int level,
                               int& rows, int& cols,
                               int& wall_percent, int& enemy_percent) {
    if (difficulty == 1) {
        rows = 4;
        cols = 8;
        wall_percent  = 4 + (level - 1);
        enemy_percent = 25 + (level - 1) * 4;
    }
    else if (difficulty == 2) {
        rows = 8;
        cols = 15;
        wall_percent  = 8 + (level - 1);
        enemy_percent = 22 + (level - 1) * 4;
    }
    else {
        rows = 10;
        cols = 20;
        wall_percent  = 10 + (level - 1);
        enemy_percent = 25 + (level - 1) * 5;
    }
}

//add_border_walls function sets the outer border cells of the map to walls using "#".
//The function has no inputs.
//Output is that the first and last row and column in map_data are all "#"".
static void add_border_walls() {
    for (int row = 0; row < map_rows; ++row) {
        map_data[row][0] = '#';
        map_data[row][map_cols - 1] = '#';
    }
    for (int col = 0; col < map_cols; ++col) {
        map_data[0][col] = '#';
        map_data[map_rows - 1][col] = '#';
    }
}

//pick_exit_far_from_player function chooses an exit location on the outer border far from the player.
//Inputs are start_row, start_col, and difficulty.
//Outputs are exit_row and exit_col, which give the exit position on the border.
static void pick_exit_far_from_player(int start_row, int start_col, int difficulty,
                                      int& exit_row, int& exit_col) {
    struct border_position {
        int row;
        int col;
        int distance;
    };

    vector<border_position> border_list;

    for (int col = 1; col < map_cols - 1; ++col) {
        border_list.push_back({0, col, 0});
        border_list.push_back({map_rows - 1, col, 0});
    }
    for (int row = 1; row < map_rows - 1; ++row) {
        border_list.push_back({row, 0, 0});
        border_list.push_back({row, map_cols - 1, 0});
    }

    if (border_list.empty()) {
        exit_row = 0;
        exit_col = 0;
        return;
    }

    int max_distance = 0;
    for (auto& b : border_list) {
        b.distance = std::abs(b.row - start_row) + std::abs(b.col - start_col);
        if (b.distance > max_distance) {
            max_distance = b.distance;
        }
    }

    double ratio;
    if (difficulty == 1)
        ratio = 0.6;
    else if (difficulty == 2)
        ratio = 0.7;
    else
        ratio = 0.8;

    int min_distance = static_cast<int>(max_distance * ratio);
    if (min_distance < 1) min_distance = 1;

    vector<border_position> candidates;

    for (const auto& b : border_list) {
        if (b.distance >= min_distance)
            candidates.push_back(b);
    }

    if (candidates.empty()) {
        for (const auto& b : border_list) {
            if (b.distance == max_distance)
                candidates.push_back(b);
        }
    }

    int index = rand() % candidates.size();
    exit_row = candidates[index].row;
    exit_col = candidates[index].col;
}

//create_map_memory function allocates a new 2D array for the map.
//Inputs are rows and cols specifying the map size.
//Output is that map_data is allocated as rows × cols and map_rows/map_cols are updated.
static void create_map_memory(int rows, int cols) {
    clear_map();
    map_rows = rows;
    map_cols = cols;

    map_data = new char*[map_rows];
    for (int row = 0; row < map_rows; ++row) {
        map_data[row] = new char[map_cols];
    }
}

//load_map function generates a map with a random player start, random exit, a safe path, and random obstacles.
//Inputs are difficulty (1–3) and level (1–3).
//Outputs are map_data filled with characters, and map_player_start_row/col set.
void load_map(int difficulty, int level) {
    int rows, cols;
    int wall_percent, enemy_percent;
    get_map_parameters(difficulty, level, rows, cols, wall_percent, enemy_percent);

    create_map_memory(rows, cols);

    for (int row = 0; row < map_rows; ++row) {
        for (int col = 0; col < map_cols; ++col) {
            map_data[row][col] = '.';
        }
    }

    add_border_walls();
    
    vector<vector<bool>> safe_route(map_rows, vector<bool>(map_cols, false));

    int start_row = 1 + rand() % (map_rows - 2);
    int start_col = 1 + rand() % (map_cols - 2);

    map_player_start_row = start_row;
    map_player_start_col = start_col;

    int exit_row, exit_col;
    pick_exit_far_from_player(start_row, start_col, difficulty, exit_row, exit_col);

    map_data[exit_row][exit_col] = 'E';
    safe_route[start_row][start_col] = true;

    int target_row = exit_row;
    int target_col = exit_col;

    if (exit_row == 0) {
        target_row = 1;
    } else if (exit_row == map_rows - 1) {
        target_row = map_rows - 2;
    } else if (exit_col == 0) {
        target_col = 1;
    } else if (exit_col == map_cols - 1) {
        target_col = map_cols - 2;
    }

    int path_row = start_row;
    int path_col = start_col;

    while (path_row != target_row || path_col != target_col) {
        safe_route[path_row][path_col] = true;
        map_data[path_row][path_col] = '.';

        int d_row = target_row - path_row;
        int d_col = target_col - path_col;

        if (std::abs(d_row) >= std::abs(d_col)) {
            if (d_row > 0)      ++path_row;
            else if (d_row < 0) --path_row;
        } else {
            if (d_col > 0)      ++path_col;
            else if (d_col < 0) --path_col;
        }
    }

    safe_route[path_row][path_col] = true;
    map_data[path_row][path_col] = '.';

    for (int row = 1; row < map_rows - 1; ++row) {
        for (int col = 1; col < map_cols - 1; ++col) {
            if (safe_route[row][col]) continue;
            if (row == start_row && col == start_col) continue;
            if (row == exit_row && col == exit_col) continue;

            int r = rand() % 100;

            if (r < wall_percent) {
                map_data[row][col] = '#';
            } else {
                int enemy_roll = rand() % 100;
                if (enemy_roll < enemy_percent) {
                    int type = rand() % 3;
                    if (type == 0)      map_data[row][col] = 'T';
                    else if (type == 1) map_data[row][col] = 'F';
                    else                map_data[row][col] = 'S';
                }
            }
        }
    }
}

//free_map function frees all memory used by the current map.
//The function has no inputs.
//Output is that map_data is cleared and map_rows/map_cols reset.
void free_map() {
    clear_map();
}

//get_map_char_at function returns the character at a given position on the map.
//Inputs are row and col.
//Output is the map character at (row, col), or '#' if out of bounds.
char get_map_char_at(int row, int col) {
    if (map_data == nullptr) return '#';
    if (row < 0 || row >= map_rows || col < 0 || col >= map_cols) return '#';
    return map_data[row][col];
}

//position_walkable function checks whether a position is not a wall.
//Inputs are row and col.
//Output is true if walkable, false otherwise.
bool position_walkable(int row, int col) {
    return get_map_char_at(row, col) != '#';
}

//at_exit_position function checks whether a position is an exit tile.
//Inputs are row and col.
//Output is true if the tile contains 'E', false otherwise.
bool at_exit_position(int row, int col) {
    return get_map_char_at(row, col) == 'E';
}

//print_map function prints the map along with the player and enemies displayed on top.
//Inputs are the player's coordinates and the list of enemies.
//Output is printed map output to the terminal.
void print_map(int player_row, int player_col, const vector<Entity>& enemies) {
    if (map_data == nullptr) {
        cout << "map not ready" << endl;
        return;
    }

    for (int row = 0; row < map_rows; ++row) {
        for (int col = 0; col < map_cols; ++col) {
            char base_char = map_data[row][col];

            bool player_here = (row == player_row && col == player_col);
            bool enemy_here = false;
            char enemy_char = ' ';

            if (!player_here) {
                for (const auto& e : enemies) {
                    if (!e.active) continue;
                    if (e.y == row && e.x == col) {
                        enemy_here = true;
                        enemy_char = e.type;
                        break;
                    }
                }
            }

            if (player_here) {
                cout << color_player << 'P' << color_reset;
            }
            else if (enemy_here) {
                cout << color_enemy << enemy_char << color_reset;
            }
            else if (base_char == '#') {
                cout << color_wall << '#' << color_reset;
            }
            else if (base_char == 'E') {
                cout << color_exit << 'E' << color_reset;
            }
            else if (base_char == 'T' || base_char == 'F' || base_char == 'S') {
                cout << color_enemy << base_char << color_reset;
            }
            else {
                cout << color_text << base_char << color_reset;
            }
        }
        cout << endl;
    }

    cout << "map size: " << map_rows << " x " << map_cols << endl;
}
