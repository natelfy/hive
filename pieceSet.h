#ifndef SETPIECES_H
#define SETPIECES_H
#include <map>
#include "Piece/piece.h"

namespace Hive {

    class PieceSet {
        private:
    		const string namePlayer;
            const Color color;
            int totalNbPiece = 0 ;
    		std::map<std::string, std::unique_ptr<Piece>> pieces;
    		const Piece* queenBee ;

		public :
    		class Iterator {
			public:
				void next();
				bool isDone() const;
				Piece* currentItem() const;
				explicit Iterator(PieceSet& ps) : parent(ps), i(parent.pieces.begin()){}

			private:
				PieceSet& parent;
				std::map<std::string, std::unique_ptr<Piece>>::iterator i ;
			};

    		const string repeat(char c, size_t n) const ;
    		const string repeatSpace(size_t n) const ;
    		const std::string toString();
    		const Piece* getQueenBee() const;
    		const int getTotalNbPiece() const;
    		PieceSet(string name, Color color, bool extensionMosquito, bool extensionLadyBug, bool extensionPillbug);
    		Iterator getIterator();

    };
}
#endif //SETPIECES_H
