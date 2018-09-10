#include <catch/catch.hpp>

#include "../src/include/rules.hpp"

TEST_CASE("Verify board construction", "[rules]")
{
    Game test(BoardSize::SMALL);

    SECTION("Board is in empty state")
    {
        auto board = test.get_board();
        bool isEmpty = true;
        for (auto &n : board)
        {
            if (n.type != NodeType::EMPTY)
            {
                isEmpty = false;
            }
        }
        REQUIRE(isEmpty == true);
    }
}