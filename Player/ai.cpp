#include "player.h"
#include "AiStrategy/aiStrategy.h"
#include <iostream>

using namespace Hive;

/* TODO
 * Les cout ne devraient pas être ici ils seront géré par la gameloop (parfois ia vs ia !)
 * Le constructeur ne doit pas prendre tout ça, par exemple le joueur est responsable du cycle de vie du pieceset donc il le créee
 * Il faut implémenter generate move, etc...
*/
//Constructeur d'Ai, appelant le constructeur de la classe parent
Ai::Ai(unsigned int id, PieceSet& pieceSet, AiStrategy* strategy, AiLevel level) : Player(id, pieceSet), aiStrategy(strategy), level(level) {}

//Destructeur de Ai, libère la mémoire de la stratégie
Ai::~Ai() {
  delete aiStrategy;
}

//Implémentation de getMove()
Move Ai::getMove() {
  std::cout << "=== Au tour de votre adversaire ===" << std::endl;

  try {
    //Générer mouvement selon la stratégie
    Move move = aiStrategy->generateMove();
    std::cout << "L'adversaire choisit une pièce: " << move.getPiece()->getSymbol()
              << " la déplace sur (" << std::get<0>(move.getDestination())
              << ", " << std::get<1>(move.getDestination()) << ")"
              << std::endl;
    return move;
  } catch (const std::exception& e) {
    std::cerr << "Une erreur est survenu avec votre adversaire: " << e.what() << std::endl;
    throw;
  }
}