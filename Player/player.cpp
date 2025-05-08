#include "player.h"
#include "../move.h"
#include "../pieceSet.h"
#include "AiStrategy/aiStrategy.h"
#include <iostream>

namespace Hive {


    string Player::getName() {
        return name;
    }

    Move Player::play(int counter) {
        return getMove(counter);
    }

}