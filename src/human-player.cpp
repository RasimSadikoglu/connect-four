#include "human-player.h"

#include "game.h"

#include "iostream"

HumanPlayer::HumanPlayer():
    Player("Human")
{
    std::cout << "Player name: ";
    std::string name;
    std::cin >> name;

    std::cout << "\033[u";
    std::cout << "\033[0J";
}

void HumanPlayer::make_move(Game &game) {
    std::cout << "Select a column: ";
    
    int column;
    std::cin >> column;

    game.make_move(static_cast<uint8_t>(column - 1));
}