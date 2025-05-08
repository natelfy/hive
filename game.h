#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <stack>
#include "board.h"
#include "Player/player.h"

namespace Hive {

    class InputResult;

    enum class GameMode {
        _0_human, _1_human, _2_humans
    };


    class Game {

        int counter = 1;

        friend class Menu;

        const GameMode gameMode;
        const AiLevel ai1Level;
        const AiLevel ai2Level;
        const unsigned int numberOfReturns;
        unsigned int numberOfReturnsLeft = numberOfReturns;
        static Game *instance;

        Game(GameMode mode, AiLevel ai1, AiLevel ai2, unsigned int returnsLeft, bool mosquito, bool ladybug,
             bool pillbug);


        ~Game() {
            delete board;
            delete player1;
            delete player2;
        };
        const bool mosquitoExtension = false;
        const bool ladybugExtension = false;
        const bool pillbugExtension = false;
        bool ended = false;
        const Board *board;
        Player *player1;
        Player *player2;
        Player *winner;
        std::stack<Move> lastMoves;
        static InputResult handleUserInput();
        bool playTurn(Player* player);
        void printBoard() const;

    public:
        Game(const Game &) = delete;

        Game &operator=(const Game &) = delete;

        static void endGame();

        bool checkWinCondition();

        Player *gameLoop();

        void undoMove();

        static Game &getGame(GameMode mode, AiLevel ai1, AiLevel ai2, unsigned int returns, const Board *b,
                             bool mosquito, bool ladybug, bool pillbug);
    };
}

#endif //GAME_H
