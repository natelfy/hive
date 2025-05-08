#include "aiStrategy.h"
#include "../../move.h"
#include "../../pieceSet.h"
#include "../../board.h"
#include <iterator>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iostream>

#include "../../board.h"
#include "../../game.h"

using namespace Hive;

Move AiStrategyEasy::generateMove(int counter) {
    // Initialisation du générateur de nombres aléatoires
    std::srand(std::time(nullptr));

    // Récupérer les pièces du joueur via l'itérateur
    auto piecesIterator = pieceSet->getIterator();

    // Garder que les pièces avec des déplacements possibles
    std::vector<Piece*> movablePieces;
    while (!piecesIterator.isDone()) {
        Piece* piece = piecesIterator.currentItem();

        // Vérifier si la pièce est placée et a des déplacements légaux
        if (piece->getIsPlaced() && !piece->legalMoves(counter, false).empty()) {
            movablePieces.push_back(piece);
        }

        piecesIterator.next();  // Passer à la pièce suivante
    }

    // Si aucune pièce => exception
    if (movablePieces.empty()) {
        throw std::runtime_error("Votre adversaire ne peut plus jouer.");
    }

    // Choix aléatoire de pièce parmi celles possibles
    int pieceIndex = std::rand() % movablePieces.size();
    Piece* selectedPiece = movablePieces[pieceIndex];

    // Récupérer les possibilités de déplacement
    auto legals = selectedPiece->legalMoves(counter, false);

    // Choisir destination aléatoire parmi les mouvements
    int moveIndex = std::rand() % legals.size();
    std::tuple<int, int> destination = legals[moveIndex];

    return Move(selectedPiece, getAi(), pieceSet, destination);
}


Move AiStrategyMedium::generateMove(int counter) {
    // Initialisation du générateur de nombres aléatoires
    std::srand(std::time(nullptr));

    // Récupérer les pièces du joueur via l'itérateur
    auto piecesIterator = pieceSet->getIterator();

    // Garder que les pièces avec des déplacements possibles
    std::vector<Piece*> movablePieces;
    while (!piecesIterator.isDone()) {
        Piece* piece = piecesIterator.currentItem();

        // Vérifier si la pièce est placée et a des déplacements légaux
        if (piece->getIsPlaced() && !piece->legalMoves(counter, false).empty()) {
            movablePieces.push_back(piece);
        }

        piecesIterator.next();  // Passer à la pièce suivante
    }

    // Si aucune pièce -> exception
    if (movablePieces.empty()) {
        throw std::runtime_error("Votre adversaire ne peut plus jouer.");
    }

    //Variable pour stocker la meilleure pièce et le meilleur mouvement
    Piece* bestPiece = nullptr;
    std::tuple<int, int> bestMove;

    //Evaluation des mouvements pour chaque pièce
    int scoreMax = -1; //Score pour la meilleure pièce

    for (Piece* piece : movablePieces)
    {
        auto legals = piece->legalMoves(counter, false);

        //Pour chaque move possible, on doit lui donner une note
        for (const auto& move : legals)
        {
            //Evaluer impact du mouvement (par exemple, protéger la reine)
            int score = evaluate(piece, move);

            //Si note plus élevé que la précédente, on garde ce mouvement
            if (score > scoreMax)
            {
                bestPiece = piece;
                bestMove = move;
                scoreMax = score;
            }
        }
    }

    //Si on n'a trouvé aucun bon move, on en prend un aléatoirement
    if (bestPiece == nullptr)
    {
        int pieceIndex = std::rand() % movablePieces.size();
        bestPiece = movablePieces[pieceIndex];
        auto legals = bestPiece->legalMoves(counter, false);
        int moveIndex = std::rand() % legals.size();
        bestMove = legals[moveIndex];
    }

    //Retourner le move choisi
    return Move(bestPiece, getAi(), pieceSet, bestMove);
}

//Fonction pour évaluer l'impact d'un move
int AiStrategyMedium::evaluate(Piece* piece, const std::tuple<int, int>& move)
{
    int score = 0;
    std::tuple<int, int> playerQueen = {-1, -1}; //Position initiale invalide

    //Trouver la Reine sur le plateau en utilisant l'itérateur
    Board& board = Board::getBoard();
    Board::Iterator it = board.iterator();

    //Parcourir le plateau
    while (it.hasNext())
    {
        Piece* currentPiece = it.current();
        it.next(); //Avancer à la pièce suivante

        if (currentPiece->getCode() == "♕" && currentPiece->getColor() != this->pieceSet->getQueenBee()->getColor())
        {
            //Si la pièce est la Reine de l'adversaire
            playerQueen = currentPiece->getPosition();
            break; //Une fois la reine trouvée, on arrête la recherche
        }
    }

    //1. Proximité avec la Reine du joueur
    if (playerQueen != std::make_tuple(-1, -1))
    {
        int distance = getDistance(move, playerQueen);
        if (distance == 1)
        {
            //Si mouvement met pièce à côté de la Reine adverse
            score += 30;
        } else if (distance <= 2)
        {
            //Moindre pour les mouvements proches
            score += 15;
        }
    }

    //2. Protéger sa propre Reine
    const Piece* aiQueen = this->pieceSet -> getQueenBee();
    if (aiQueen != nullptr)
    {
        int myDistance = getDistance(move, aiQueen->getPosition());
        if (myDistance == 1)
        {
            // Plus pour renforcer défense autour Reine
            score += 20;
        }
    }

    //3. Blocage des pièces adverses
    auto neighbours = Piece::neighbours(move);
    for (Piece* neighbour : neighbours)
    {
        if (neighbour != nullptr && neighbour->getColor() != this->pieceSet->getQueenBee()->getColor())
        {
            //Plus si piece bloque piece joueur
            score += 25;
        }
    }

    //4. Mobilité des pièces
    switch (piece->getCode()[0])
    {
        case 'A': //Soldier Ant
            score += 20; //Grande mobilité
            break;
        case 'B': //Beetle
            score += 15; //Bloque les pièces
            break;
        case 'G': //Grasshopper
            score += 10; //Saut stratégique
            break;
        case '♕': //Queen Bee
            score += 30; //Pièce critique
            break;
        case 'S': //Spider
            score += 10; //Mouvement limité
            break;
        case 'M': //Mosquito
            score += 25; //Mobilité adaptable
            break;
        case 'L': //Ladybug
            score += 15; //Traverse le plateau
            break;
        case 'P': //Pillbug
            score += 15; //Déplacement stratégique
            break;
    }

    //5. Préserver les pièces en danger
    if (isThreat(piece))
    {
        score += 10; //Récompenser les moves éloignant une pièce d'une pos vulnérable
    }

    return score;
}

int AiStrategyMedium::getDistance(const std::tuple<int, int>& pos1, const std::tuple<int, int>& pos2)
{
    auto [x1, y1] = pos1;
    auto [x2, y2] = pos2;

    return abs(x1 - x2) + abs(y1 - y2);
}

bool AiStrategyMedium::isThreat(Piece* piece)
{
    auto neighbours = Piece::neighbours(piece->getPosition());
    int count = 0;

    for (Piece* neighbour : neighbours)
    {
        if (neighbour != nullptr && neighbour->getColor() != piece->getColor())
        {
            count++;
        }
    }

    return count > 2; //Une pièce est menacée si elle est entourée par 3 adv ou plus
}


Move AiStrategyHard::generateMove(int counter) {
    // TODO
    // Minimax ou Monte Carlo Tree Search afin d'évaluer les meilleures options.
    return Move(nullptr, nullptr, nullptr, {-1, -1});
}

