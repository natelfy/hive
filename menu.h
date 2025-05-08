#ifndef MENU_H
#define MENU_H

#include "game.h"

namespace Hive{

    enum InputType {
        PAUSE,
        UNDO,
        SINGLE_NUMERIC,
        TUPLE_NUMERIC,
        STRING,
        UNKNOWN,
        CONTINUE
    };

    class InputResult {
        InputType type = UNKNOWN;
        std::tuple<int, int> tupleValue = {-1, -1};
        int singleValue = -1;
        string stringValue = "";
    public:
        InputResult(std::tuple<int, int> tuple) : type(TUPLE_NUMERIC), tupleValue(tuple){};
        InputResult(int a, int b) : type(TUPLE_NUMERIC), tupleValue({a, b}){};

        InputResult(string str) : type(STRING), stringValue(str){};
        InputResult(int value) : type(SINGLE_NUMERIC), singleValue(value){};
        InputResult(InputType TYPE) : type(TYPE){};
        InputType getType() const { return type;};
        std::tuple<int, int> getTupleValue() const { return tupleValue; }
        int getSingleValue() const { return singleValue; };
        string getStringValue() const { return stringValue; };
        InputResult() = default;
    };


    class Menu {
        private :
            bool mosquitoExtension = true;
            bool ladybugExtension = true;
            bool pillbugExtension = false;
            Menu();
            ~Menu();
            Game* gameInstance;
            static Menu* menuInstance;

            GameMode selected_mode = GameMode::_2_humans;
            AiLevel selected_ai1level;
            AiLevel selected_ai2level;
            void askAndToggleExtensions();
            void toggleExtensionMosquito();
            void toggleExtensionLadybug();
            void askAndSetGameMode();
            void playNewGame();
            void resumeGame();
            void endGame();
            void displaySettings();


        public:
            Menu(const Menu&) = delete;
            Menu& operator=(const Menu&) = delete ;
            static Menu& getMenu();
            static void deleteMenu();
            void terminalInterface();
            static InputResult getInput();
    };
}

#endif //MENU_H
