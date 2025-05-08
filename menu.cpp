#include "menu.h"
#include "game.h"
#include "board.h"
#include <string>
#include <iostream>
#include <sstream>

using namespace Hive;



// Initialisation de l’menuInstance Singleton
Menu* Menu::menuInstance = nullptr;


// Constructeur privé, car c’est un singleton
Menu::Menu()
    : gameInstance(Game::instance),// Initialise une référence à l'menuInstance unique de la classe Game via son propre singleton.
      selected_mode(GameMode::_2_humans),// Définit le mode de jeu par défaut à AI vs AI.
      selected_ai1level(AiLevel::easy),// Configure les niveaux de difficulté des IA à Easy par défaut.
      selected_ai2level(AiLevel::easy) {}


// Destructeur
Menu::~Menu()
{
    delete menuInstance;// Détruit l'menuInstance unique de Menu et libère la mémoire allouée.
}




// Retourne une référence à l'menuInstance unique de Menu. Si elle n'existe pas encore, elle est créée.
Menu& Menu::getMenu()
{
    if (menuInstance == nullptr)// Vérifie si l'menuInstance existe déjà.
    {
        menuInstance = new Menu();// Si l'menuInstance n'existe pas, elle est cree avec le constructeur privé.
    }
    return *menuInstance; // Retourne une référence à l'menuInstance unique de Menu.
}




// Supprime l'menuInstance unique de Menu et libère la mémoire associée.
void Menu::deleteMenu()
{
    if (menuInstance != nullptr)// Vérifie si l'menuInstance a déjà été créée avant d'essayer de la supprimer.
    {
        delete Game::instance;
        delete menuInstance;// Libère la mémoire associée à l'menuInstance unique.
        menuInstance = nullptr;// Réinitialise le pointeur menuInstance pour indiquer qu'aucune menuInstance n'existe plus.
    }
}




void Menu::toggleExtensionMosquito()
{
    mosquitoExtension = !mosquitoExtension;//Active extension mosquito
    if (mosquitoExtension) std::cout << "The Mosquito extension has been enabled.\n";
    else std::cout << "The Mosquito Eetension has been disabled.\n";
}


// Activation de l'extension Ladybug
void Menu::toggleExtensionLadybug()
{
    ladybugExtension = !ladybugExtension;//Active extension mosquito
    if (ladybugExtension) std::cout << "The Ladybug extension has been enabled .\n";
    else std::cout << "The Ladybug extension has been disabled.\n";
}

void Menu::askAndToggleExtensions() {
    std::cout << "Select extension to toggle : \n";

    std::cout << "1. Mosquito";
    if (mosquitoExtension) std::cout << " (activated)\n";
    else std::cout << " (deactiated)\n";

    std::cout << "2. Ladybug";
    if (ladybugExtension) std::cout << " (activated)\n";
    else std::cout << " (deactiated)\n";

    InputResult input = getInput();
    if (input.getType() != SINGLE_NUMERIC) {
        throw HiveException("Please provide a number.");
    }

    switch (input.getSingleValue())
    {
        case 1:
            toggleExtensionMosquito();
            break;
        case 2:
            toggleExtensionLadybug();
            break;
        default:
            cout<<"Unknown value.";
    }
}


// String util
InputResult Menu::getInput() {
    std::string input;
    std::cin.clear();
    std::getline(std::cin, input);

    if (input.empty()) {
        return {};
    }

    if (input == "c") {
        return {CONTINUE};
    }
    if (input == "p") {
        return {PAUSE};
    }
    if (input == "u") {
        return {UNDO};
    }

    std::istringstream ss(input);
    int num1, num2;

    if (ss >> num1) {
        if (ss >> num2) {
            return {num1, num2};
        } else {
            return {num1};
        }
    }

    return {input}; // as string
}


// Définir le mode de jeu
void Menu::askAndSetGameMode()
{
    InputResult choix;
    std::cout << "Select game mode: \n";
    std::cout << "1. 0 humain (AI vs AI)\n";
    std::cout << "2. 1 humain (Player vs AI)\n";
    std::cout << "3. 2 humains (Player vs Player)\n";

    choix = getInput();
    if (choix.getType() != SINGLE_NUMERIC) {
        throw HiveException("Please provide a number.");
    }

    switch (choix.getSingleValue())
    {
    case 1:
        selected_mode = GameMode::_0_human;
        break;
    case 2:
        selected_mode = GameMode::_1_human;
        break;
    case 3:
        selected_mode = GameMode::_2_humans;
        break;
    default:
        std::cout << "Choix invalide. Veuillez réessayer.\n";
        return;
    }
    std::cout << "Mode de jeu défini avec succés.\n";
}



// Commencer une nouvelle partie
void Menu::playNewGame()
{
    if (gameInstance != nullptr) {
        throw HiveException("Une partie est en cours. Veuillez la terminer avant d'en commencer une nouvelle.");
        return;
    }

    if (selected_mode == GameMode::_2_humans)
    {
        gameInstance = &Game::getGame(selected_mode, selected_ai1level, selected_ai2level, 3, nullptr, mosquitoExtension, ladybugExtension, pillbugExtension);
    }
    else if (selected_mode == GameMode::_1_human)
    {
        gameInstance = &Game::getGame(selected_mode, selected_ai1level, selected_ai2level, 3, nullptr, mosquitoExtension, ladybugExtension, pillbugExtension);
    }
    else
    {
        gameInstance = &Game::getGame(selected_mode, selected_ai1level, selected_ai2level, 3, nullptr, mosquitoExtension, ladybugExtension, pillbugExtension);
    }

    if (gameInstance == nullptr) {
        throw runtime_error("Game could'nt be created.");
    }

    std::cout << "\n\nNew game created !\n";
    gameInstance->gameLoop();
}

// Reprendre une partie en pause
void Menu::resumeGame()
{
    if (gameInstance != nullptr)   //Vérifie si la partie n'est pas terminée.
    {
        std::cout << "Unpausing game...\n";
        gameInstance->gameLoop();// Relance la boucle principale du jeu pour continuer la partie.
    }
    else {
        std::cout << "The game is already finished.\n";
    }
}

// Mettre fin à la partie actuelle
void Menu::endGame()
{
    Game::endGame();// Met fin à la partie en cours en supprimant l'menuInstance unique de Game
    gameInstance = nullptr;
}



// Affichage des paramètres actuels
void Menu::displaySettings()
{
    std::cout << "Game Settings:\n";
    std::cout << "  Extension Mosquito: " << (mosquitoExtension ? "On" : "Off") << "\n";
    std::cout << "  Extension Ladybug : " << (ladybugExtension ? "On" : "Off") << "\n";
    std::cout << "  Extension Pillbug: " << (pillbugExtension ? "On" : "Off") << "\n";
    std::cout << "  Mode de jeux: " << (selected_mode == GameMode::_0_human ? "AI vs AI" :
                                        selected_mode == GameMode::_1_human ? "Player vs AI" : "Player vs Player") << "\n";

    if (selected_mode != GameMode::_2_humans)
    {
        std::cout << "  AI 1 Level: " << (selected_ai1level == AiLevel::easy ? "Easy" :
                                          selected_ai1level == AiLevel::medium ? "Medium" : "Hard") << "\n";
        std::cout << "  AI 2 Level: " << (selected_ai2level == AiLevel::easy ? "Easy" :
                                          selected_ai2level == AiLevel::medium ? "Medium" : "Hard") << "\n";
    }
}

void Menu::terminalInterface() {
    std::cout << "--- Welcome to Hive Game ---\n";
    while (true) {
        std::cout << "\n\n\n";
        std::cout << "You can choose options to interact with this menu :\n";
        std::cout << "0. Quit application\n";
        std::cout << "1. Display selected settings\n";
        std::cout << "2. Set game mode\n";
        std::cout << "3. Toggle extensions\n";
        std::cout << "4. Play new game\n";
        if (gameInstance != nullptr) {
            std::cout << "5. Continue current game\n";
        }
        std::cout << "Your choice : ";
        InputResult input;
        input = getInput();
        if (input.getType() != SINGLE_NUMERIC) {
            cout<<input.getType();
            //cout<<input.getSingleValue();

            cout<<"Please provide a numeric input.";
            continue;
        }

        switch (input.getSingleValue()){
            case 0:
                // TODO deallocate
                exit(0);
            case 1:
                displaySettings();
                break;
            case 2:
                askAndSetGameMode();
                break;
            case 3:
                askAndToggleExtensions();
                break;
            case 4:
                playNewGame();
                if (gameInstance->ended) {
                    cout<<"Game ended successfully with result : ";
                    if (gameInstance->winner == nullptr) {
                        cout<<"Tie!";
                    }
                    else {
                        cout<<gameInstance->winner->getName() << "won !";
                    }
                    endGame();
                }
                else {
                    cout<<"Game paused. See you soon !";
                }
                break;
            case 5:
                if (gameInstance != nullptr) {
                    resumeGame();
                    if (gameInstance->ended) {
                        cout<<"Game ended successfully with result : ";
                        if (gameInstance->winner == nullptr) {
                            cout<<"Tie!";
                        }
                        else {
                            cout<<gameInstance->winner->getName() << "won !";
                        }
                        endGame();
                    }
                    else {
                        cout<<"Game paused. See you soon !";
                    }
                }
            default:
                cout<<"Unknown choice.";

        }
    }
}
