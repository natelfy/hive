#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <vector>
#include "Piece/piece.h"


namespace Hive {

    class Board {
    private:
        static Board *instance;
        const size_t sizeMax = 15;

        Board();
        ~Board();

        Piece ***grid;

        const std::string repeat(char c, size_t n) const;

        const std::string repeatSpace(size_t n) const;

        class Iterator {
        private:
            unsigned int row;
            unsigned int col;
            const Board &board;
        public:
            Iterator(const Board &b) : board(b), row(0), col(0) {};
            bool hasNext() const;
            void next();
            Piece *current() const;
        };

        friend class Game;

    public:
        Board &operator=(const Board &) = delete;

        Board(const Board &) = delete;

        static Board &getBoard();

        void addPiece(Piece &piece, std::tuple<int, int> position);

        void movePiece(Piece &piece, std::tuple<int, int> position);

        void removePiece(Piece &piece);

        std::vector<std::tuple<int, int>> neighboursSlots(std::tuple<int, int> position) const;

        std::vector<Piece *> neighboursPieces(std::tuple<int, int> position);

        Iterator iterator() const { return Iterator(*this); }

        const size_t getSizeMax() const { return sizeMax; }

        std::vector<std::tuple<int, int>> nextFreeSlotLine(std::tuple<int, int> position);

        std::vector<std::tuple<int, int>> freeContiguousSlots(std::tuple<int, int> position);

        const std::string toString(vector<tuple<int, int>> pos, int xp, int yp) const;

        bool checkFullConnectivity(int x, int y, vector<tuple<int, int>> &visited,
                                   vector<tuple<int, int>> &remainingPieces);

        bool keepsHiveTogether(Piece &piece);

        std::vector<std::tuple<int,int>> freeSlotsToAddPiece(Color color, int counter);

        std::tuple<int, int> convertToTuple(const std::string &coord);

        std::string convertToString(const std::tuple<int, int> &coord);

        bool isEmpty() const;
    };
}

#endif //BOARD_H
