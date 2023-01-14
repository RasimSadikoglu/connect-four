#include "ai-player.h"

#include "board.h"
#include "game.h"

AIPlayer::AIPlayer():
    Player("AI")
{
}

void AIPlayer::make_move(Game &game) const {
    auto current_board = std::make_shared<Board>(Board(*game.get_board()));
    game.make_move(this->find_next_move(current_board));
}

uint8_t AIPlayer::find_next_move(__attribute__((unused)) std::shared_ptr<Board> board) const {
    return 0;
}