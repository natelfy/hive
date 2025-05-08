#ifndef AISTRATEGY_H
#define AISTRATEGY_H

#include "../../pieceSet.h"

namespace Hive {

    class Move;
    class Player;

    class AiStrategy {
    private:
        Player* ai;

    public:
        Player* getAi() {return ai;};
        PieceSet *pieceSet;

        explicit AiStrategy(PieceSet *pieceSet, Player* ai) : pieceSet(pieceSet), ai(ai) {};

        virtual ~AiStrategy() = default;

        virtual Move generateMove(int counter) = 0;
    };

    class AiStrategyEasy : public AiStrategy {
    public:
        explicit AiStrategyEasy(PieceSet *pieceSet, Player* ai) : AiStrategy(pieceSet, ai) {};

        Move generateMove(int counter) override;
    };

    class AiStrategyMedium : public AiStrategy {
    public:
        Move generateMove(int counter);
        int evaluate(Piece* piece, const std::tuple<int, int>& move);
        int getDistance(const std::tuple<int, int>& pos1, const std::tuple<int, int>& pos2);
        bool isThreat(Piece* piece);
    };

    class AiStrategyHard : public AiStrategy {
    public:
        Move generateMove(int counter) override;
    };


}

#endif //AISTRATEGY_H
