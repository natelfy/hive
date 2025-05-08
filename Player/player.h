#ifndef PLAYER_H
#define PLAYER_H

#include "AiStrategy/aiStrategy.h"
#include "../pieceSet.h"
#include "../move.h"

namespace Hive {
    class Move;

    class AiStrategy;

    class AiStrategyEasy;

    class AiStrategyMedium;

    class AiStrategyHard;

    enum class AiLevel {
        easy, medium, hard
    };

    class Player {
    protected:
        const string name;
        Color color;
        PieceSet *pieceSet;

        virtual ~Player() {
            delete pieceSet;
        };

    private:
        friend class Game;

    public:
        string getName();

        Move play(int counter);

        virtual Move getMove(int counter) = 0;

        Player(string name, bool extensionMosquito, bool extensionLadyBug, bool extensionPillbug, Color color)
                : name(name), color(color) {
            pieceSet = new PieceSet(name, color, extensionMosquito, extensionLadyBug, extensionPillbug);
        };
    };

    class Human : public Player {
    public:
        Human(string name, bool extensionMosquito, bool extensionLadyBug, bool extensionPillbug, Color color) :
                Player(name, extensionMosquito, extensionLadyBug, extensionPillbug, color) {};

        Move getMove(int compteur) override;
    };

    class Ai : public Player {
    private:
        AiStrategy *aiStrategy;
        AiLevel level;

    public:
        Ai(string name, bool extensionMosquito, bool extensionLadyBug, bool extensionPillbug, AiLevel level,
           Color color) :
                level(level),
                Player(name, extensionMosquito, extensionLadyBug, extensionPillbug, color) {
            if (level == AiLevel::easy) {
                aiStrategy = new AiStrategyEasy(pieceSet, this);
            } else {
                throw std::runtime_error("Only the easy level is supported right now.");
            }
        };

        ~Ai() override {
            delete aiStrategy;
        };

        Move getMove(int compteur) override;
    };

}
#endif //PLAYER_H
