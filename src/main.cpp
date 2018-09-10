#include <iostream>
#include "include/rules.hpp"
#include "include/helper.hpp"

int main(int argc, char **argv)
{
    std::ios::sync_with_stdio(false);
    Game game(BoardSize::SMALL, atoi(argv[1]));
    while (true)
    {
        size_t c, r;
        game.render();

        input_play(game, game.get_turn());
        if (game.get_captured().first != 0)
        {
            break;
        }
        game.next_turn();
    }
}