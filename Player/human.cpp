#include "player.h"
#include <iostream>
#include <tuple>
#include <vector>

#include "../board.h"
#include "../menu.h"

using namespace Hive;

// Implémentation de getMove()
Move Human::getMove(int counter) {
    // Affichage du pièce set avec les pièce disponibles en blanc et celles déjà posées en marron
    std::cout << pieceSet->toString() << std::endl;

    // Saisie de la pièce via un numéro
    int selectedPieceNumber = 0;
    Piece* selectedPiece = nullptr;
    InputResult input = {};

    std::cout << getName() << " it's your turn !" << std::endl;

    while (true) {
        if (counter == 1) {
            std::cout << "Select the first piece you would like to place on the board (1-" << pieceSet->getTotalNbPiece() << "): ";
        } else if (counter <= 8 && !pieceSet->getQueenBee()->getIsPlaced()) {
            std::cout << "Select the piece you would like to place on the board (1-" << pieceSet->getTotalNbPiece() << "): ";
        } else {
            std::cout << "Select the piece you would like to play (place on the board or move around) (1-" << pieceSet->getTotalNbPiece() << "): ";
        }

        input = Menu::getInput();

        // Vérifier si l'entrée est un numéro unique
        if (input.getType() == SINGLE_NUMERIC) {
            selectedPieceNumber = input.getSingleValue();

            // Vérifier si le numéro sélectionné est valide
            if (selectedPieceNumber >= 1 && selectedPieceNumber <= pieceSet->getTotalNbPiece()) {
                PieceSet::Iterator it = pieceSet->getIterator();

                for (int i = 0; i < selectedPieceNumber - 1; i++) {
                    it.next();
                }
                selectedPiece = it.currentItem();

                if ((counter == 7 || counter == 8) && (!pieceSet->getQueenBee()->getIsPlaced()) && selectedPiece != pieceSet->getQueenBee()) {
                    std::cout << "You need to place your Queen on the board now." << std::endl;
                    continue;
                }

                if (counter <= 8 && !pieceSet->getQueenBee()->getIsPlaced() && selectedPiece->getIsPlaced()) {
                    std::cout << "You need to place you Queen before moving any other piece." << std::endl;
                    continue;
                }

                break;
            } else {
                std::cout << "Invalied number. Try again." << std::endl;
            }
        } else {
            std::cout << "Invalid input." << std::endl;
        }
    }

    while (true) {
        // Affichage des déplacements possibles
        auto legalMoves = selectedPiece->legalMoves(counter, false);
        int x = get<0>(selectedPiece->getPosition());
        int y = get<1>(selectedPiece->getPosition());
        std::cout << std::endl << std::endl << Board::getBoard().toString(legalMoves, x, y).c_str() << std::endl << std::endl;
        // Affichage du pièce set avec les pièce disponibles en blanc et celles déjà posées en marron
        std::cout << pieceSet->toString() << std::endl;

        if (legalMoves.empty()) {
            if (selectedPiece->getIsPlaced()) {
                if (selectedPiece->getBeetleOnTop()) {
                    std::cout << "This piece has a beetle on top. Choose another one." << std::endl;
                } else if (selectedPiece->getMosquitoOnTop()) {
                    std::cout << "This piece has a Mosquito on top. Choose another one." << std::endl;
                } else {
                    std::cout << "No legal move for this piece. Choose another one." << std::endl;
                }
            }

            return getMove(counter);
        }

        // Saisie de la destination
        std::tuple<int, int> destination;
        std::cout << "(Enter a single value to change the selected piece)\n";
        std::cout << "Enter two numbers for where to place the piece :  ";
        input = Menu::getInput();

        if (input.getType() == TUPLE_NUMERIC) {
            destination = input.getTupleValue();

            if (std::find(legalMoves.begin(), legalMoves.end(), destination) != legalMoves.end()) {
                // Créer et retourner le mouvement
                if (selectedPiece->getIsPlaced()) {
                    return {selectedPiece, this, nullptr, destination};
                } else {
                    return {selectedPiece, this, pieceSet, destination};
                }
            }
            std::cout << "You can't move your piece there. Select another position from those marked in red." << std::endl;
        } else if (input.getType() == SINGLE_NUMERIC) {
            selectedPieceNumber = input.getSingleValue();

            if (selectedPieceNumber >= 1 && selectedPieceNumber <= pieceSet->getTotalNbPiece()) {
                PieceSet::Iterator it = pieceSet->getIterator();

                for (int i = 0; i < selectedPieceNumber - 1; i++) {
                    it.next();
                }
                selectedPiece = it.currentItem();

                // Affichage du pièce set avec les pièce disponibles en blanc et celles déjà posées en marron
                continue;
            } else {
                std::cout << "Invalid number. Try again." << std::endl;
            }
        } else {
            std::cout << "Invalid input." << std::endl;
        }
    }
}


