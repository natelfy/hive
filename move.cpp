#include "move.h"
#include "Piece/piece.h"
#include "pieceSet.h"
#include "board.h"
#include <tuple>

namespace Hive {

    Move::Move(Piece* piece, Player* player, PieceSet* origin, std::tuple <int, int> destination): piece(piece), player(player), pieceSetOrigin(origin), destination(destination) {
        // Stockage position d'origine pour annuler déplacement
        piecePositionOrigin = piece->getPosition();
    }
    
    void Move::doMove() {
        // Déplacer pièce vers destination
        Board& board = Board::getBoard();
        if (pieceSetOrigin != nullptr) {
            board.addPiece(*piece, destination);
        }
        else {
            board.movePiece(*piece, destination);
        }
    }

    void Move::undoMove() {
        Board& board = Board::getBoard();
        if (pieceSetOrigin != nullptr) {
            board.removePiece(*piece);
        }
        else {
            board.movePiece(*piece, piecePositionOrigin);
        }
    }
}