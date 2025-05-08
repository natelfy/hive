#include "piece.h"
#include "../board.h"
#include "../pieceSet.h"
using namespace std;

namespace Hive {

    Piece::~Piece() = default;

    Piece::Piece(Color c, string cd, string d) : color(c), code(std::move(cd)), description(std::move(d)) {}

    bool Piece::getIsPlaced() const { return isPlaced; }

    void Piece::setIsPlaced(bool b) {isPlaced = b ;}

    tuple<int, int> Piece::getPosition() const { return position; }

    string Piece::getCode() const {return code;}

    void Piece::setPosition( std::tuple<int, int> pos) {position = pos;}

    vector<Piece*> Piece::neighbours(tuple<int, int> pos) {

        return  Board::getBoard().neighboursPieces(pos);
    }

    bool Piece::canMove() {
        Hive::Board &board = Hive::Board::getBoard();
        return (!board.keepsHiveTogether(*this) && !getBeetleOnTop() && !getMosquitoOnTop());
    }

    static bool isBlockedByNeighbours(tuple<int, int> origin, tuple<int, int> destination) {
        vector<Piece*> selfNeighbours = Piece::neighbours(origin);
        vector<Piece*> positionNeighbours = Piece::neighbours(destination);

        for (auto sN : positionNeighbours) {
            if (sN->getPosition() == origin) {
                vector<Piece*> intersection;

                std::sort(selfNeighbours.begin(), selfNeighbours.end());
                std::sort(positionNeighbours.begin(), positionNeighbours.end());
                intersection.resize(std::min(selfNeighbours.size(), positionNeighbours.size()));

                auto it = set_intersection(selfNeighbours.begin(),
                                 selfNeighbours.end(),
                                 positionNeighbours.begin(),
                                 positionNeighbours.end(),
                                 intersection.begin());

                intersection.resize(std::distance(intersection.begin(), it));

                if (intersection.size() >= 2) { // Not possible to go there
                    return true;
                }
            }
        }
        return false;
    }

    bool Piece::validitySlot(tuple<int, int> positionDestination) {
        // Checks if the slot is contiguous to the hive (i.e. has one or more neighbours other than the moved piece)
        Board &board = Board::getBoard();
        vector<Piece*> npieces = board.neighboursPieces(positionDestination);
        auto it = std::ranges::find(npieces, this);
        if (it != npieces.end()) {
            npieces.erase(it);
        }
        int a = npieces.size();
        return (a >= 1);
    }

    Color Piece::getColor() const {
        return color;
    }

    std::string Piece::colorToString() const {
        if (color == Hive::Color::blue) {
            return "●";
        }
        return "◆";
    }

    string Piece::toStringForBoard() const {
        if (getIsPlaced() && !getBeetleOnTop()) {
            return getCode() + colorToString();
        }
        return "";
    }

    vector <tuple<int, int>> QueenBee::legalMoves(int counter, bool canMoveOverride) {
        Hive::Board &board = Hive::Board::getBoard();
        if (getIsPlaced()) {
            vector<tuple<int, int>> legals = {};
            // Check if the Piece can move
            if (canMove() || canMoveOverride) {
                // check if more than two neighbours in common with destinations
                for (auto position: board.freeContiguousSlots(getPosition())) {
                    // Not possible to go there
                    if (!isBlockedByNeighbours(getPosition(), position) && validitySlot(position)){
                        legals.push_back(position);
                    }
                }
            }
            return legals;
        }
        return board.freeSlotsToAddPiece(getColor(), counter);
    }


    vector <tuple<int, int>> Beetle::legalMoves(int counter, bool canMoveOverride) {
        Hive::Board &board = Hive::Board::getBoard();
        if (getIsPlaced()) {
            vector<tuple<int, int>> legals = {};
            // Check if the Piece can move
            if (canMove() || canMoveOverride) {
                for (auto position: board.neighboursSlots(getPosition())) {
                    if (validitySlot(position)){
                        legals.push_back(position);
                    }
                }
            }
            return legals;
        }
        return board.freeSlotsToAddPiece(getColor(), counter);
    }
    

    void SoldierAnt::findPath(tuple<int, int> originPosition, vector <tuple<int, int>>& legals) {
        // Get free contiguous slots
        Hive::Board& board = Hive::Board::getBoard();
        vector<tuple<int, int>> freeSlots = board.freeContiguousSlots(originPosition);
        for (auto slot: freeSlots) {
            // Not possible to go there
            bool blocked = isBlockedByNeighbours(getPosition(), slot);
            bool alreadyAdded = (count(legals.begin(), legals.end(), slot) > 0);
            //bool isCurrentPosition = (count(legals.begin(), legals.end(), getPosition()) > 0);
            if (!blocked && !alreadyAdded && validitySlot(slot) /*&& !isCurrentPosition*/) {
                legals.push_back(slot);
                findPath(slot, legals);
            }
        }
    }

    vector <tuple<int, int>> SoldierAnt::legalMoves(int counter, bool canMoveOverride) {
        if (getIsPlaced()) {
            vector<tuple<int, int>> legals = {};
            // Check if the Piece can move
            if (canMove() || canMoveOverride) {
                findPath(getPosition(), legals);
            }
            return legals;
        }
        Hive::Board &board = Hive::Board::getBoard();
        return board.freeSlotsToAddPiece(getColor(), counter);
    }

    
    void Spider::findPath(tuple<int, int> originPosition, vector <tuple<int, int>>& legals, vector<tuple<int, int>>& visited, int targetDepth) {
        Hive::Board& board = Hive::Board::getBoard();
        if (targetDepth < 1) {
            return;
        }
        // Get free contiguous slots
        vector<tuple<int, int>> freeSlots = board.freeContiguousSlots(originPosition);
        for (auto slot: freeSlots) {
            // Not possible to go there
            bool blocked = isBlockedByNeighbours(getPosition(), slot);
            bool alreadyAdded = (count(legals.begin(), legals.end(), slot) > 0);
            bool alreadyVisited = (count(visited.begin(), visited.end(), slot) > 0);
            //bool isCurrentPosition = (count(legals.begin(), legals.end(), getPosition()) > 0);
            if (!blocked && !alreadyAdded && !alreadyVisited && validitySlot(slot)) {
                std::cout<<"Slot : "<<get<0>(slot)<<","<<get<1>(slot)<<"Depth : "<<targetDepth<<"\n";
                visited.push_back(slot);
                if (targetDepth == 1) {
                    legals.push_back(slot);
                }
                findPath(slot, legals, visited, targetDepth-1);
            }
        }
    }

    vector <tuple<int, int>> Spider::legalMoves(int counter, bool canMoveOverride) {
        if (getIsPlaced()) {
            vector<tuple<int, int>> legals = {};
            vector<tuple<int, int>> visited = {};

            int targetDepth = 3;

            // Check if the Piece can move
            if (canMove() || canMoveOverride) {
                findPath(getPosition(), legals, visited, targetDepth);
            }
            return legals;
        }
        Hive::Board &board = Hive::Board::getBoard();
        return board.freeSlotsToAddPiece(getColor(), counter);
    }

    vector <tuple<int, int>> Grasshopper::legalMoves(int counter, bool canMoveOverride) {
        Hive::Board& board = Hive::Board::getBoard();
        if (getIsPlaced()) {
            vector<tuple<int, int>> legals = {};
            // Check if the Piece can move
            if (canMove() || canMoveOverride) {
                for (auto position : board.nextFreeSlotLine(getPosition())) {
                    if (validitySlot(position)){
                        legals.push_back(position);
                    }
                }
            }
            return legals;
        }
        return board.freeSlotsToAddPiece(getColor(), counter);
    }

    vector <tuple<int, int>> Mosquito::legalMoves(int counter, bool canMoveOverride) {
        Hive::Board& board = Hive::Board::getBoard();
        if (getIsPlaced()) {
            vector<tuple<int, int>> legals = {};
            if (canMove()) {
                auto mosquitoPosition = getPosition();
                auto neighbours = board.neighboursPieces(mosquitoPosition);
                if (neighbours.size()== 1 && neighbours[0]->getCode() == "M" ) {
                    return legals;
                }

                board.removePiece(*this);
                std::unique_ptr<Piece> ghost;
                if (hiddenPiece != nullptr) {
                    ghost = std::make_unique<Beetle>(getColor());
                    board.addPiece(*ghost, mosquitoPosition);
                    legals = ghost->legalMoves(counter, true);
                    board.removePiece(*ghost);
                    board.addPiece(*this,mosquitoPosition);
                    return legals;
                }

                for (auto n : neighbours) {
                    string code = n->getCode();
                    if (code == "B") ghost = std::make_unique<Beetle>(getColor());
                    else if (code == "G") ghost = std::make_unique<Grasshopper>(getColor());
                    else if (code == "♕") ghost = std::make_unique<QueenBee>(getColor());
                    else if (code == "A") ghost = std::make_unique<SoldierAnt>(getColor());
                    else if (code == "S") ghost = std::make_unique<Spider>(getColor());
                    else if (code == "L") ghost = std::make_unique<Ladybug>(getColor());
                    else if (code == "M") continue;
                    else throw HiveException("Error : code not recognised.");

                    board.addPiece(*ghost, mosquitoPosition);
                    auto moves = ghost->legalMoves(counter, true);
                    legals.insert( legals.end(), moves.begin(), moves.end() );

                    board.removePiece(*ghost);
                }
                board.addPiece(*this,mosquitoPosition);
            }
            return legals;
        }
        return board.freeSlotsToAddPiece(getColor(), counter);
    }

    vector <tuple<int, int>> Ladybug::legalMoves(int counter, bool canMoveOverride) {
        if (getIsPlaced()) {
            vector<tuple<int, int>> legals = {};
            vector<tuple<int, int>> visited = {};
            int targetDepth = 3;
            if (canMove() || canMoveOverride) {
                findPath(getPosition(), legals, targetDepth);
            }
            return legals;

        }
        Hive::Board& board = Hive::Board::getBoard();
        return board.freeSlotsToAddPiece(getColor(), counter);
    }

    void Ladybug::findPath(tuple<int, int> originPosition, vector <tuple<int, int>>& legals, int targetDepth) {
        Hive::Board& board = Hive::Board::getBoard();

        if (targetDepth == 1) {
            vector<tuple<int, int>> freeSlots = board.freeContiguousSlots(originPosition);
            for (auto slot: freeSlots) {
                bool alreadyAdded = (count(legals.begin(), legals.end(), slot) > 0);
                //bool isCurrentPosition = (count(legals.begin(), legals.end(), getPosition()) > 0);
                if (!alreadyAdded && validitySlot(slot)) {
                    std::cout<<"Slot : "<<get<0>(slot)<<","<<get<1>(slot)<<" Depth : "<<targetDepth<<"\n";
                    legals.push_back(slot);
                }
            }
            return;
        }

        // Get continguous pieces
        vector<Piece*> nPieces = board.neighboursPieces(originPosition);
        for (auto piece: nPieces) {
            if (piece == this) continue;

            tuple<int, int> position = piece->getPosition();
            cout << "Pièce " << piece->getCode() << "depth : " << targetDepth << endl;
            findPath(position, legals, targetDepth-1);
            }
    }


    vector <tuple<int, int>> Pillbug::legalMoves(int counter, bool canMoveOverride) {
        Hive::Board& board = Hive::Board::getBoard();
        if (getIsPlaced()) {
            vector<tuple<int, int>> legals = {};
            return legals;
        }
        return board.freeSlotsToAddPiece(getColor(), counter);
    }
}