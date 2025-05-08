#include "pieceSet.h"
#include <sstream>

namespace Hive {

    /*

    Exemple d'utilisation de la classe PieceSet

    Hive::Color c = Hive::Color::black;
    Hive::PieceSet ps = Hive::PieceSet(c, false, false, false) ;
    Hive::PieceSet::Iterator i = ps.getIterator();

    for (int j = 0; j < 11 ; j++) {
        std::unique_ptr<Hive::Piece> a = i.currentItem();
        a->PlacePiece({8,9});
        printf("Piece %d \n", j);
        printf("String %s\n", a->toStringForBoard().c_str() );
        printf("Is_Done %d\n", i.isDone());
        std:tuple<int,int> p = a->getPosition();
        printf("pos : (%d %d)\n", get<0>(p), get<1>(p));
        printf("IsPlaced : (%d)\n", a->getIsPlaced());
        i.next();
    }

    */

    PieceSet::PieceSet(string name, const Color color, bool extensionMosquito, bool extensionLadyBug, bool extensionPillbug) :
        color(color), namePlayer(name)
    {
        totalNbPiece = 11 + extensionMosquito + extensionLadyBug + extensionPillbug  ;
        pieces["queen-bee"] = std::make_unique<QueenBee>(color);
        pieces["spider1"] = std::make_unique<Spider>(color);
        pieces["spider2"] = std::make_unique<Spider>(color);
        pieces["beetle1"] = std::make_unique<Beetle>(color);
        pieces["beetle2"] = std::make_unique<Beetle>(color);
        pieces["soldier-ant1"] = std::make_unique<SoldierAnt>(color);
        pieces["soldier-ant2"] = std::make_unique<SoldierAnt>(color);
        pieces["soldier-ant3"] = std::make_unique<SoldierAnt>(color);
        pieces["grasshopper1"] = std::make_unique<Grasshopper>(color);
        pieces["grasshopper2"] = std::make_unique<Grasshopper>(color);
        pieces["grasshopper3"] = std::make_unique<Grasshopper>(color);

        queenBee = pieces.find("queen-bee")->second.get();

        if (extensionMosquito) {
            pieces["~mosquito"] = std::make_unique<Mosquito>(color);
        }

        if (extensionPillbug) {
            pieces["~pillbug"] = std::make_unique<Pillbug>(color);
        }

        if (extensionLadyBug) {
            pieces["~ladybug"] = std::make_unique<Ladybug>(color);
        }
    }

    const Piece* PieceSet::getQueenBee() const {
        return queenBee;
    }

    const int PieceSet::getTotalNbPiece() const {
        return totalNbPiece;
    }



    const string PieceSet::repeat(char c, size_t n) const{return string(n, c);}
    const string PieceSet::repeatSpace(size_t n) const { return string(n, ' '); }


    const std::string PieceSet::toString()  {
        stringstream sb;
        const string title = "HIVE PIECE SET " + namePlayer ;
        int gridDisplayLength = totalNbPiece * 2 + (totalNbPiece - 1);
        sb << repeatSpace(gridDisplayLength / 2 - title.length() / 2) << title << '\n';
        sb << repeat('-', gridDisplayLength);
        sb << endl ;

        for (int i = 1; i < totalNbPiece + 1 ; i++) {
            sb << i << " " ;
            if (std::to_string(i).size()== 1){ sb << " ";}
        }

        sb << endl;

        for (Iterator it = getIterator(); !it.isDone(); it.next()) {
            Piece* p = it.currentItem();
             if (p->getIsPlaced()){
                 sb << "\033[33m" ;
                 sb << p->getCode() << p->colorToString() << " " ;
                 sb << "\033[0m";
                }
            else {
                sb << p->getCode() << p->colorToString() << " " ;
            }
        }

        sb << endl << endl;

        return sb.str();
    }

    bool PieceSet::Iterator::isDone() const {
        return i == parent.pieces.end();
    }

    void PieceSet::Iterator::next() {
        if (isDone())
            throw HiveException("Iterator at the end of sequence.");
        i++;
    }

    Piece* PieceSet::Iterator::currentItem() const {
        if (isDone()) {
            throw HiveException("Iterator at the end of sequence.");
        }
        return (i->second).get();
    }

    PieceSet::Iterator PieceSet::getIterator(){
        return Iterator(*this); // Create an iterator linked to the actual PieceSet object
    }

}







