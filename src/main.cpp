#include <iostream>

#include "game.h"
#include "human-player.h"
#include "ai-player.h"

int main() {
    std::array<std::shared_ptr<Player>, 2> players;
    std::array<std::string, 2> _ = {"first", "second"};

    std::cout << "\033[s";

    for (int i = 0; i < 2; i++) {

        // Print the main menu.
        std::cout << "Choose the " << _[i] << " player\n";
        std::cout << "1) Human\n";
        std::cout << "2) AI\n";
        std::cout << "Choice: ";

        int choice = 0;

        std::cin >> choice;

        std::cout << "\033[u";
        std::cout << "\033[0J";

        // Create players by the choice the constructor of the players propmts for the necessary options.
        if (choice == 1) {
            players[i] = std::make_shared<HumanPlayer>(HumanPlayer());
        } else if (choice == 2) {
            players[i] = std::make_shared<AIPlayer>(AIPlayer());
        } else {
            throw std::invalid_argument("No.");
        }
    }

    // Create and start the game with the players.
    Game game = Game(players[0], players[1]);
    game.start_game();
}