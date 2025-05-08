#ifndef PIECE_H
#define PIECE_H

#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>

using namespace  std;

namespace Hive {

    enum class Color{blue, green};

    class HiveException {
        public:
            HiveException(const std::string& i) :info(i) {}
            std::string getInfo() const { return info; }
        private:
            std::string info;
    };

    class Piece {
        private:
            bool isPlaced = false;
            bool bettleOnTop = false;
            bool mosquitoOnTop = false;
            std::tuple<int,int> position = {-1, -1};
            const Color color;
            const std::string description ;
            const std::string code;

        public:

            Piece(Color color, std::string code, std::string description);
            bool getBeetleOnTop() const {return bettleOnTop;}
            bool getMosquitoOnTop() const {return mosquitoOnTop;}
            void setBeetleOnTop(bool b) {bettleOnTop = b;}
            void setMosquitoOnTop(bool b){mosquitoOnTop = b;}
            bool getIsPlaced() const ;
            void setIsPlaced(bool b);
            bool canMove();
            std::string getCode()const;
            Color getColor() const;
            std::string getDescription() const {return description;}
            std::tuple<int,int> getPosition() const;
            void setPosition(std::tuple<int, int>);
            std::string colorToString() const;
            virtual ~Piece();
            static std::vector<Piece*> neighbours(std::tuple<int, int> pos);
            bool validitySlot(std::tuple<int, int> positionDestination);
            virtual std::vector<std::tuple<int, int>> legalMoves(int counter, bool canMoveOverride) = 0;
            string toStringForBoard() const;
    };



    class QueenBee : public Piece {
    public:
        explicit QueenBee(Color color, const std::string description = "Queen bee: can move only one space per turn.") : Piece(color, "♕", description) {}
        std::vector<std::tuple<int, int>> legalMoves(int counter, bool canMoveOverride) override;
    };

    class Beetle : public Piece {
    public:
        Piece* hiddenPiece = nullptr ;
        explicit Beetle(Color color, const std::string description = "Beetle: can move a single space or climb onto any adjacent piece. Pieces under a beetle cannot move.") : Piece(color, "B", description) {}
        std::vector<std::tuple<int, int>> legalMoves(int counter, bool canMoveOverride) override;
    };

    class Grasshopper : public Piece{
    public:
        explicit Grasshopper(Color color, const std::string description = "Grasshopper: moves by jumping over one or more pieces, in any of six directions.") : Piece(color, "G", description) {}
        std::vector<std::tuple<int, int>> legalMoves(int counter, bool canMoveOverride) override;
    };

    class Spider : public Piece {
    public:
        explicit Spider(Color color, const std::string description = "Spider: can move 3 spaces per turn, no more, no less.") : Piece(color, "S", description) {}
        void findPath(tuple<int, int> originPosition, vector<tuple<int, int>>& visited, vector <tuple<int, int>>& legals, int targetDepth);
        vector <tuple<int, int>> legalMoves(int counter, bool canMoveOverride) override;
    };

    class SoldierAnt : public Piece {
    public:
        explicit SoldierAnt(Color color, const std::string description = "Soldier ant: can move to any space around the hive.") : Piece(color, "A", description) {}
        void findPath(tuple<int, int> originPosition, vector <tuple<int, int>>& legals);
        std::vector<std::tuple<int, int>> legalMoves(int counter, bool canMoveOverride) override;
    };

    class Mosquito : public Piece {
    public:
        Piece* hiddenPiece = nullptr ;
        explicit Mosquito(Color color, const std::string description = "Mosquito: takes the movement mode of any adjacent piece.") : Piece(color, "M", description) {}
        std::vector<std::tuple<int, int>> legalMoves(int counter, bool canMoveOverride) override;
    };

    class Ladybug : public Piece {
        public:
            void findPath(tuple<int, int> originPosition, vector <tuple<int, int>>& legals, int targetDepth);
            explicit Ladybug(Color color, const std::string description = "Ladybug: moves three spaces; two above the hive, then one on the way down.") : Piece(color, "L", description) {}
            std::vector<std::tuple<int, int>> legalMoves(int counter, bool canMoveOverride) override;
            };


    class Pillbug : public Piece {
        public:
            explicit Pillbug(Color color, const std::string description = "Pillbug: can move one space or move an adjacent piece to another free space adjacent to it.") : Piece(color, "P", description) {}
            std::vector<std::tuple<int, int>> legalMoves(int counter, bool canMoveOverride) override;
        };

}


#endif //PIECE_H
