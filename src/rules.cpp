#include <iostream>
#include <iomanip>
#include <unordered_set>

#include "include/rules.hpp"

//============================================================
// Node / Group
//============================================================
void Group::capture()
{
    for (auto n : elements)
    {
        n.get().type = NodeType::EMPTY;
        n.get().group = nullptr;
    }
}

void Group::merge(std::shared_ptr<Group> other)
{
    freedom += (other->freedom - 1);
    auto sp = elements.front().get().group;
    for (auto n : other->elements)
    {
        elements.push_back(n);
        n.get().group = sp;
    }
}
//============================================================
// Game
//============================================================
//------------------------------------------------------------
// Constructors, Destructors, Copy, Move, Etc.
//------------------------------------------------------------
Game::Game(BoardSize n, int handicap) : turn(Turn::W)
{
    size = static_cast<size_t>(n);
    // Should move instead of copy
    board = std::vector<Node>(
        size * size,
        Node{NodeType::EMPTY, nullptr});
    captured = std::make_pair(0, 0);
    if (handicap)
    {
        place_handicap(n, handicap);
    }
}

//------------------------------------------------------------
// Getters
//------------------------------------------------------------
const std::vector<Node> &
Game::get_board() const noexcept
{
    return board; //const reference
}

size_t Game::get_size() const noexcept
{
    return size; //copy
}

std::pair<int, int>
Game::get_captured() const noexcept
{
    return captured; //copy
}

Turn Game::get_turn() const noexcept
{
    return turn; //copy
}

//------------------------------------------------------------
// Helpers
//------------------------------------------------------------
size_t Game::pos(size_t col, size_t row) const noexcept
{
    return (row - 1) * size + (col - 1);
}

void Game::place_handicap(BoardSize b, int h)
{
    if (h == 1)
    {
        turn = Turn::B;
    }
    else
    {
        auto bs = static_cast<size_t>(b);
        auto left = 4;
        auto right = bs - 3;
        auto mid = (bs + 1) / 2;
        if (b == BoardSize::SMALL)
        {
            if (h >= 2)
            {
                play(3, 3, NodeType::BLACK);
                play(7, 7, NodeType::BLACK);
            }
            if (h >= 3)
            {
                play(7, 3, NodeType::BLACK);
            }
            if (h >= 4)
            {
                play(3, 7, NodeType::BLACK);
            }
            if (h >= 5)
            {
                play(5, 5, NodeType::BLACK);
            }
        }
        else
        {
            if (h >= 2)
            {
                play(left, left, NodeType::BLACK);
                play(right, right, NodeType::BLACK);
            }
            if (h >= 3)
            {
                play(right, left, NodeType::BLACK);
            }
            if (h >= 4)
            {
                play(left, right, NodeType::BLACK);
            }
            if (h >= 5)
            {
                play(right, mid, NodeType::BLACK);
            }
            if (h >= 6)
            {
                play(left, mid, NodeType::BLACK);
            }
            if (h >= 7)
            {
                play(mid, right, NodeType::BLACK);
            }
            if (h >= 8)
            {
                play(mid, left, NodeType::BLACK);
            }
            if (h >= 9)
            {
                play(mid, mid, NodeType::BLACK);
            }
        }
    }
}

cNodeList Game::check_neighbors(size_t col, size_t row)
    const noexcept
{
    cNodeList new_list;
    std::vector<std::pair<int, int>> offset =
        {std::make_pair(-1, 0),
         std::make_pair(1, 0),
         std::make_pair(0, -1),
         std::make_pair(0, 1)};
    for (auto &o : offset)
    {
        size_t ncol = col + o.second;
        size_t nrow = row + o.first;
        if ((ncol <= 0) || (nrow <= 0) ||
            (ncol >= size) || (nrow >= size))
        {
            continue;
        }
        else
        {
            size_t nind = pos(ncol, nrow);
            // Node rvalue copied then moved.
            new_list.push_back(std::cref(board[nind]));
        }
    }
    return new_list;
}

//------------------------------------------------------------
// Affect Game State
//------------------------------------------------------------
bool Game::play(size_t col, size_t row, NodeType color)
{
    size_t ind = pos(col, row);
    auto selected = std::ref(board[ind]);
    if (selected.get().type != NodeType::EMPTY)
    {
        return false;
    }
    else
    {
        NodeType opponent = (color == NodeType::WHITE)
                                ? NodeType::BLACK
                                : NodeType::WHITE;
        // Collect neighbor information
        cNodeList neigh_list = check_neighbors(col, row);
        std::unordered_set<std::shared_ptr<Group>> opp_groups;
        std::unordered_set<std::shared_ptr<Group>> ally_groups;
        uint freedom = 0;
        for (auto &n : neigh_list)
        {
            if (n.get().type == opponent)
            {
                opp_groups.insert(n.get().group);
            }
            else if (n.get().type == color)
            {
                ally_groups.insert(n.get().group);
            }
            else
            {
                ++freedom;
            }
        }

        // When a piece is played, check for enemy capture
        for (auto &og : opp_groups)
        {
            if (og->freedom == 1)
            {
                if (opponent == NodeType::WHITE)
                {
                    captured.first += og->elements.size();
                }
                else
                {
                    captured.second += og->elements.size();
                }
                og->capture();
            }
            else
            {
                --(og->freedom);
            }
        }

        // Then combine into groups and update/check freedom
        NodeList e;
        e.push_back(selected);
        selected.get().group = std::make_shared<Group>(
            Group{freedom, std::move(e)});

        for (auto &ag : ally_groups)
        {
            selected.get().group->merge(ag);
        }
        // Play the stone
        selected.get().type = color;
        return true;
    }
}

void Game::next_turn() noexcept
{
    turn = (turn == Turn::W) ? Turn::B : Turn::W;
}

//------------------------------------------------------------
// Other
//------------------------------------------------------------
void Game::render() const
{
    std::cout << "\x1b[2J";
    for (size_t r = size; r > 0; r--)
    {
        std::cout << std::setw(2) << std::left << r;
        for (size_t c = 1; c <= size; c++)
        {
            switch (board[pos(c, r)].type)
            {
            case NodeType::EMPTY:
                std::cout << "\u2795";
                break;
            case NodeType::WHITE:
                std::cout << "\u26AA";
                break;
            case NodeType::BLACK:
                std::cout << "\u26AB";
                break;
            default:
                std::cerr << "\nBoard not initialized properly\n";
                throw;
            }
        }
        std::cout << "\n";
    }
    std::cout << "  ";
    for (size_t i = 1; i <= size; i++)
    {
        std::cout << std::setw(2) << std::right << i;
    }
    std::cout << "\n";
}