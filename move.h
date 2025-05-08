#ifndef MOVE_H
#define MOVE_H

#include "pieceSet.h"
#include "Piece/piece.h"
#include "Player/player.h"

namespace Hive {

    class Player;

    class Move {
        private:
            Piece* piece;
            Player* player;
            PieceSet* pieceSetOrigin; // If null,
            std::tuple<int, int> piecePositionOrigin;
            std::tuple<int, int> destination;

        public:
            Move(Piece* piece, Player* player, PieceSet* origin, std::tuple <int, int> destination);
            Move() : piece(nullptr), player(nullptr), pieceSetOrigin(nullptr), piecePositionOrigin({-1, -1}),
                    destination{-1, -1}{};
            void doMove();
            void undoMove();
    };
}

#endif //MOVE_H
