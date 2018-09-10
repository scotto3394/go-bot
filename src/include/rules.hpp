#ifndef RULES
#define RULES

#include <functional>
#include <memory>
#include <vector>

#include "bot.hpp"

enum class NodeType
{
    WHITE,
    BLACK,
    EMPTY
};

enum class BoardSize : size_t
{
    SMALL = 9,
    MEDIUM = 13,
    LARGE = 19,
};

enum class Turn : bool
{
    W,
    B
};

struct Group;

struct Node
{
    NodeType type = NodeType::EMPTY;
    std::shared_ptr<Group> group = nullptr;
};

using NodeList =
    std::vector<std::reference_wrapper<Node>>;
using cNodeList =
    std::vector<std::reference_wrapper<const Node>>;

struct Group
{
    uint freedom;
    NodeList elements;

    void capture();
    void merge(std::shared_ptr<Group> other);
};

class Game
{
    friend class AiBot;

    std::vector<Node> board;
    size_t size;
    Turn turn;
    std::pair<int, int> captured;

    // Helpers
    size_t pos(size_t col, size_t row) const noexcept;
    void place_handicap(BoardSize b, int h);
    cNodeList check_neighbors(size_t col, size_t row)
        const noexcept;

  public:
    // Constructors, Destructors, Copy, Move, Etc.
    Game(BoardSize n, int handicap = 0);

    // Getters
    const std::vector<Node> &get_board() const noexcept;
    size_t get_size() const noexcept;
    std::pair<int, int> get_captured() const noexcept;
    Turn get_turn() const noexcept;

    // Affect GameState
    bool play(size_t col, size_t row, NodeType color);
    void next_turn() noexcept;

    // Other
    void render() const;
};

#endif