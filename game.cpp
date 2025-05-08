#include "game.h"
#include "Player/player.h"
#include "menu.h"

using namespace Hive;

Game* Game::instance = nullptr;

Game::Game(GameMode mode, AiLevel ai1, AiLevel ai2, unsigned int returnsLeft, bool mosquito, bool ladybug,
bool pillbug) : gameMode(mode), ai1Level(ai1), ai2Level(ai2), numberOfReturns(returnsLeft),
numberOfReturnsLeft(returnsLeft), winner(nullptr),
ladybugExtension(ladybug), mosquitoExtension(mosquito), pillbugExtension(pillbug) {

    if (gameMode != GameMode::_2_humans) {
        std::cout << "Mode not supported yet!" << std::endl;
        return;
    }

    board = new Board();

    // Get player names using getInput
    std::string player1Name, player2Name;

    // Get Player 1 Name
    while (true) {
        std::cout << "\nEnter name for Player 1 : ";
        InputResult input = Menu::getInput();
        if (input.getType() == STRING) {
            player1Name = input.getStringValue();
            break;
        }
        std::cout << "Invalid input. Please provide a valid name." << std::endl;
    }

    // Get Player 2 Name
    while (true) {
        std::cout << "Enter name for Player 2 : ";
        InputResult input = Menu::getInput();
        if (input.getType() == STRING) {
            player2Name = input.getStringValue();
            break;
        }
        std::cout << "Invalid input. Please provide a valid name." << std::endl;
    }

    // Initialize players
    player1 = new Human(player1Name, mosquitoExtension, ladybugExtension, pillbugExtension, Color::blue);
    player2 = new Human(player2Name, mosquitoExtension, ladybugExtension, pillbugExtension, Color::green);
}


// Singleton menuInstance management
Game& Game::getGame(GameMode mode, AiLevel ai1, AiLevel ai2, unsigned int returns, const Board* b,
                    bool mosquito, bool ladybug, bool pillbug) {
    if (instance == nullptr) {
        instance = new Game(mode, ai1, ai2, returns, mosquito, ladybug, pillbug);
    }
    return *instance;
}

void Game::endGame() { // Called by the menu to delete the game menuInstance
    delete instance; // if null, deleting a pointer has no effect
    instance = nullptr;
}

// Game loop
Player* Game::gameLoop() {
    Move currentMove;
    InputResult input = {};

    while (true) { // Loop stops when the game ends
        input = handleUserInput();
        if (input.getType() == PAUSE) {
            return nullptr; // here, ended == false
        }

        if (input.getType() == UNDO) {
            undoMove();
            continue;
        }

        if (counter % 2 == 1) {
            // Player 1's turn
            if (playTurn(player1)) {
                return winner;
            }
        } else {
            // Player 2's turn
            if (playTurn(player2)) {
                return winner;
            }
        }
    }
}

InputResult Game::handleUserInput() {
    InputResult input = {};
    cout << "Enter c to continue, p to pause, u to undo : ";
    while (input.getType() != CONTINUE && input.getType() != PAUSE && input.getType() != UNDO) {
        input = Menu::getInput();
    }
    return input;
}

bool Game::playTurn(Player* player) {
    printBoard();

    Move currentMove = player->play(counter++);
    currentMove.doMove();
    lastMoves.push(currentMove);

    printBoard();

    // Check if game has ended
    if (checkWinCondition()) {
        ended = true;
        return true;
    }

    return false;
}

void Game::printBoard() const {
    cout << endl << endl << Board::getBoard().toString({}, -1, -1).c_str() << endl << endl;
}

bool Game::checkWinCondition() {
    bool player1Dead = false;
    bool player2Dead = false;

    const Piece* player1QueenBee = player1->pieceSet->getQueenBee();
    std::vector<Piece*> player1QueenBeeNeighbours = Piece::neighbours(player1QueenBee->getPosition());
    if (player1QueenBeeNeighbours.size() == 6) {
        player1Dead = true;
    }

    const Piece* player2QueenBee = player2->pieceSet->getQueenBee();
    std::vector<Piece*> player2QueenBeeNeighbours = Piece::neighbours(player2QueenBee->getPosition());
    if (player2QueenBeeNeighbours.size() == 6) {
        player2Dead = true;
    }

    if (player1Dead && player2Dead) {
        return true;
    }

    if (player1Dead) {
        winner = player2;
        return true;
    }

    if (player2Dead) {
        winner = player1;
        return true;
    }

    return false;
}


void Game::undoMove() {
    if (numberOfReturnsLeft == 0) {
        // do nothing
        return;
    }
    if (!lastMoves.empty()) {
        Move lastMove = lastMoves.top();
        lastMove.undoMove();
        lastMoves.pop();
        numberOfReturnsLeft -= 1;
        counter--;
        cout<<"\nUndone one move !\n";
    }
}

