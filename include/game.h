#pragma once

class Board;
class Player;

#include <array>
#include <memory>

class Game {
private:
    std::shared_ptr<Board> board;
    std::array<std::shared_ptr<Player>, 2> players;
    bool turn;
public:
    Game(std::shared_ptr<Player> odd_player, std::shared_ptr<Player> even_player);
    void start_game();
    void make_move(uint8_t column);

    std::shared_ptr<Board> get_board() const;
};