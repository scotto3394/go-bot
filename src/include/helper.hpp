#ifndef HELPER
#define HELPER

#include <unordered_set>
#include "rules.hpp"

std::stringstream get_input(
    std::string output,
    std::unordered_set<std::string> options);
Game start_game(char **argv);
void menu(Game &game);
void input_play(Game &game, Turn current);
#endif