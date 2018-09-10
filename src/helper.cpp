#include <iostream>
#include <sstream>

#include "include/helper.hpp"
#include "include/rules.hpp"

std::stringstream get_input(
    std::string output,
    std::unordered_set<std::string> options)
{
    std::string buffer;
    std::cout << output;
    std::cin >> buffer;
    while (!options.count(buffer))
    {
        std::cout << "Not a valid option. Please try again: ";
        std::cin >> buffer;
    }

    return std::stringstream(buffer);
}

Game start_game(char **argv)
{
    std::cout << "Welcome to go-bot!\n";
    std::cout << "For all future prompts, please type options as listed.";
}

void menu(Game &game)
{
    while (true)
    {
        std::cout << "Please choose from the menu."
                  << "(Type out an option as shown)\n";
    }
}

void input_play(Game &game, Turn current)
{
    size_t c, r;
    NodeType piece;
    std::string color;
    if (current == Turn::W)
    {
        color = "(White)";
        piece = NodeType::WHITE;
    }
    else
    {

        color = "(Black)";
        piece = NodeType::BLACK;
    }
    do
    {

        std::cout << color << " Enter a move \"x y\": ";

        std::cin >> c >> r;
    } while (!game.play(c, r, piece));
}