#include "Piece/piece.h"
#include "board.h"
#include <cassert>
#include <sstream>
#include <tuple>
#include <stdexcept>
#include <cctype>
#include <string>


using namespace Hive;
using namespace std;

Board* Board::instance = nullptr;

Board::Board() {
    grid = new Piece**[sizeMax];
    for (size_t i = 0; i < sizeMax; ++i) {
        grid[i] = new Piece*[sizeMax];
        for (int j = 0; j < sizeMax; ++j) {
            grid[i][j] = nullptr;
        }
    }
}

Board::~Board() {
    if (instance) {
        for (size_t i = 0; i < sizeMax; ++i) {
            delete[] grid[i];
            /*La destruction des pi�ces ne doit pas �tre g�r� par le board
            (cas de fin de partie o� les pi�ces ne sont pas toutes pos�es sur le board)*/
        }
        delete[] grid;
    }
}

const string Board::repeat(char c, size_t n) const{return string(n, c);}
const string Board::repeatSpace(size_t n) const { return string(n, ' '); }

Board& Board::getBoard(){
    if (!instance) {instance = new Board();}
    return *instance;
}

void Board::addPiece(Piece& piece, std::tuple<int, int> position) {
    int x = get<0>(position);
    int y = get<1>(position);

    assert(!piece.getIsPlaced());
    assert(x >= 0 && x < sizeMax && y >= 0 && y < sizeMax);
    assert(grid[x][y] == nullptr);

    piece.setPosition(position);
    piece.setIsPlaced(true);
    grid[x][y] = &piece;
}

void Board::movePiece(Piece& piece, std::tuple<int, int> position){
    assert(piece.getIsPlaced());

    tuple<int, int> oldPosition = piece.getPosition();
    int oldX = get<0>(oldPosition);
    int oldY = get<1>(oldPosition);
    int newX = get<0>(position);
    int newY = get<1>(position);
    assert(newX >= 0 && newX < Board::sizeMax && newY >= 0 && newY < sizeMax);

    if (piece.getCode() == "B") {
        Beetle* beetle = dynamic_cast<Beetle*>(&piece);
        if (beetle->hiddenPiece != nullptr) {grid[oldX][oldY] = beetle->hiddenPiece; beetle->hiddenPiece->setBeetleOnTop(false); beetle->hiddenPiece= nullptr;}
        else{grid[oldX][oldY] = nullptr;}
        if (grid[newX][newY] != nullptr) {beetle->hiddenPiece = grid[newX][newY]; beetle->hiddenPiece->setBeetleOnTop(true);}
        grid[newX][newY] = beetle;
        beetle->setPosition(position);
    }

    if (piece.getCode() == "M") {
        Mosquito* mosquito = dynamic_cast<Mosquito*>(&piece);
        if (mosquito->hiddenPiece != nullptr) {grid[oldX][oldY] = mosquito->hiddenPiece; mosquito->hiddenPiece->setMosquitoOnTop(false); mosquito->hiddenPiece= nullptr;}
        else{grid[oldX][oldY] = nullptr;}
        if (grid[newX][newY] != nullptr) {mosquito->hiddenPiece = grid[newX][newY]; mosquito->hiddenPiece->setMosquitoOnTop(true);}
        grid[newX][newY] = mosquito;
        mosquito->setPosition(position);
    }

    else {
        if (grid[newX][newY] == nullptr) {
            grid[oldX][oldY] = nullptr;
            grid[newX][newY] = &piece;
            piece.setPosition(position);
        }
        else {throw HiveException("Essaye de déplacer une pièce sur une case du plateau qui n'est pas vide.");}
    }
}

void Board::removePiece(Piece& piece) {
    int x = get<0>(piece.getPosition());
    int y = get<1>(piece.getPosition());
    grid[x][y] = nullptr;
    piece.setPosition({-1, -1});
    piece.setIsPlaced(false);
}

vector<std::tuple<int, int>> Board::neighboursSlots(std::tuple<int, int> position) const {
    vector<std::tuple<int, int>> res;

    int x = get<0>(position);
    int y = get<1>(position);

    vector<tuple<int, int>> nPositions;
    if (x % 2 == 1) {
        nPositions = {{-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, 0}, {1, 1}};
    }
    else {
        nPositions = {{-1, -1}, {-1, 0}, {0, -1}, {0, 1}, {1, -1}, {1, 0}};
    }

    for (tuple<int,int> pos : nPositions) {
        int nx = x + get<0>(pos);
        int ny = y + get<1>(pos);

        if (nx >= 0 && nx < sizeMax && ny >= 0 && ny < sizeMax){
            res.push_back(std::tuple(nx, ny));
        }
    }

    return res;
}


vector<Piece*> Board::neighboursPieces(std::tuple<int, int> position){

  vector<tuple<int, int>> nslots = Board::neighboursSlots(position);
  vector<Piece*> res ;

  for (auto n : nslots){
      int nx = get<0>(n);
      int ny = get<1>(n);
      Piece* neighbour = grid[nx][ny];
      if (neighbour != nullptr) {
        res.push_back(neighbour);
      }
    }
    return res;
}

bool Board::Iterator::hasNext() const { return row < board.getSizeMax() && col < board.getSizeMax(); }

const std::string Board::toString(vector<tuple<int, int>> pos, int xp, int yp ) const {

    stringstream sb;
    char space = ' ';
    const string emptyCell = repeatSpace(2);
    const string spaceBetwCells = repeatSpace(2);
    const string offsetOddRow = repeatSpace(3);
    const string offsetRow = repeatSpace(4);
    const string title = "HIVE GAME BOARD";
    int pink = 0;
    int red = 0;

    int gridDisplayLength = 1 + offsetOddRow.length() + offsetRow.length()
        + sizeMax * (emptyCell.length() + spaceBetwCells.length() + 2)
        - spaceBetwCells.length();

    sb << repeatSpace(gridDisplayLength / 2 - title.length() / 2) << title << '\n';
    sb << repeat('-', gridDisplayLength);
    sb << "\n";

    sb << ' ' << offsetRow << offsetOddRow;
    for (int n = 1; n <= sizeMax; ++n) {
        sb << ' ';
        if (n < 10) { sb << ' ' << n; }
        else { sb << n; }
        sb << repeatSpace(3);
    }
    sb << "\n\n";

    for (size_t i = 0; i < sizeMax; ++i) {
        sb << (char)('A' + i) << offsetRow;

        if (i % 2 == 1) {
            sb << offsetOddRow;
        }

        for (size_t j = 0; j < sizeMax; ++j) {
            // si la position appartient aux vecteur passé en paramètre
            if (std::find(pos.begin(), pos.end(), std::tuple(i,j)) != pos.end())
                {sb<< "\033[31m" ; red = 1;} // red
            // si la position correspond à celle passée en paramètre
            if (xp == i && yp == j)
                {sb << "\033[35m"; pink = 1;} // pink

            Piece* piece = grid[i][j];
            sb << "(";
            if (piece) {
                if (piece->getColor() == Color::blue){sb << "\033[34m";}
                else {sb << "\033[32m";}
                sb << piece->toStringForBoard();
                sb<< "\033[0m";
            }
            else { sb << emptyCell; }

            if (pink){sb << "\033[35m"; pink = 0;}
            if (red){sb << "\033[31m"; red = 0;}
            sb << ")" << repeatSpace(2);
            sb<< "\033[0m";
        }
        sb << '\n';
        if (i != sizeMax - 1) {
            sb << " " << offsetRow << offsetOddRow;
            for (unsigned int k = 0; k < sizeMax; ++k) { sb << " --" << repeatSpace(3); }
            sb << '\n';
        }
    }

    return sb.str();
}


void Board::Iterator::next() {
    ++col;
    if (col >= board.getSizeMax()) { col = 0; row++;}
}

Piece* Board::Iterator::current() const { return board.grid[row][col]; }
/* Possibilité que la case soit vide! Avec un pointeur, la valeur sera égale à nullptr.
Si on souhaite renvoyer une référence, dans le cas d'une case vide, il faudra trouver une
alternative pour être sur de renvoyer une référence qu'en cas de case non vide */


std::vector<std::tuple<int, int>> Board::nextFreeSlotLine(std::tuple<int, int> position){
    vector<tuple<int, int>> nslots = Board::neighboursSlots(position);
    vector<tuple<int, int>> res ;

    // 0
    tuple<int, int> n0 = nslots[0];
    int nx0 = get<0>(n0);
    int ny0 = get<1>(n0);
    int i0 = 0;

    while (nx0 >= 0 && nx0 < sizeMax && ny0 >= 0 && ny0 < sizeMax && grid[nx0][ny0] != nullptr){
        i0 ++;
        if (nx0 % 2 == 1) { nx0 -= 1; }
        else {
            nx0 -= 1;
            ny0 -= 1;
        }
    }
    if (nx0 >= 0 && nx0 < sizeMax && ny0 >= 0 && ny0 < sizeMax && i0 >= 1) {res.push_back(std::tuple(nx0, ny0));}

    // 1
    tuple<int, int> n1 = nslots[1];
    int nx1 = get<0>(n1);
    int ny1 = get<1>(n1);
    int i1 = 0;

    while (nx1 >= 0 && nx1 < sizeMax && ny1 >= 0 && ny1 < sizeMax && grid[nx1][ny1] != nullptr){
        i1 ++;
        if (nx1 % 2 == 1) {
            nx1 -= 1;
            ny1 += 1;
        }
        else { nx1 -= 1; }
    }
    if (nx1 >= 0 && nx1 < sizeMax && ny1 >= 0 && ny1 < sizeMax && i1 >=1) {res.push_back(std::tuple(nx1, ny1));}

    // 2
    tuple<int, int> n2 = nslots[2];
    int nx2 = get<0>(n2);
    int ny2 = get<1>(n2);
    int i2 = 0;

    while (ny2 < sizeMax && grid[nx2][ny2] != nullptr){
        i2 ++;
        ny2 -= 1 ;
    }
    if (ny2 < sizeMax && i2 >=1 ) {res.push_back(std::tuple(nx2, ny2));}

    // 3
    tuple<int, int> n3 = nslots[3];
    int nx3 = get<0>(n3);
    int ny3 = get<1>(n3);
    int i3 = 0;

    while (ny3 >= 0 && grid[nx3][ny3] != nullptr){
        i3 ++;
        ny3 += 1 ;
    }
    if (ny3 >= 0 && i3 >=1) {res.push_back(std::tuple(nx3, ny3));}

    // 4
    tuple<int, int> n4 = nslots[4];
    int nx4 = get<0>(n4);
    int ny4 = get<1>(n4);
    int i4 = 0;

    while (nx4 >= 0 && nx4 < sizeMax && ny4 >= 0 && ny4 < sizeMax && grid[nx4][ny4] != nullptr){
        i4 ++;
        if (nx4 % 2 == 1) { nx4 += 1; }
        else {
            nx4 += 1;
            ny4 -= 1;
        }
    }
    if (nx4 >= 0 && nx4 < sizeMax && ny4 >= 0 && ny4 < sizeMax && i4 >= 1) {res.push_back(std::tuple(nx4, ny4));}

    // 5
    tuple<int, int> n5 = nslots[5];
    int nx5 = get<0>(n5);
    int ny5 = get<1>(n5);
    int i5 = 0;

    while (nx5 >= 0 && nx5 < sizeMax && ny5 >= 0 && ny5 < sizeMax && grid[nx5][ny5] != nullptr){
        i5 ++;
        if (nx5 % 2 == 1) {
            nx5 += 1;
            ny5 += 1;
        }
        else { nx5 += 1; }
    }
    if (nx5 >= 0 && nx5 < sizeMax && ny5 >= 0 && ny5 < sizeMax && i5 >=1) {res.push_back(std::tuple(nx5, ny5));}

  return res;
}

std::vector<std::tuple<int,int>> Board::freeContiguousSlots(std::tuple<int, int> position){
    vector<tuple<int, int>> res ;
    for (auto slot : Board::neighboursSlots(position)){
        if (grid[get<0>(slot)][get<1>(slot)] == nullptr){
            res.push_back(slot);
        }
    }
    return res;
}


bool Board::keepsHiveTogether(Piece& piece) {
    if (piece.getCode() == "B"){
        Beetle* beetle = dynamic_cast<Beetle*>(&piece);
        if (beetle->hiddenPiece != nullptr){return false;}
    }

    if (piece.getCode() == "M"){
        Mosquito* mosquito = dynamic_cast<Mosquito*>(&piece);
        if (mosquito->hiddenPiece != nullptr){return false;}
    }

    if (neighboursPieces(piece.getPosition()).size() <= 1) {
        return false;
    }

    // Get position
    tuple<int, int> position = piece.getPosition();
    int x = get<0>(position);
    int y = get<1>(position);

    // Remove piece from the board for the test
    Piece* temp = grid[x][y];
    grid[x][y] = nullptr;

    // Get pieces that are on the board at the moment
    vector<tuple<int, int>> remainingPieces;
    for (int i = 0; i < sizeMax; ++i) {
        for (int j = 0; j < sizeMax; ++j) {
            if (grid[i][j] != nullptr) {
                remainingPieces.push_back({i, j});
            }
        }
    }

    vector<tuple<int, int>> visited;
    bool connected = false;

    // Starting point
    for (int i = 0; i < sizeMax && !connected; ++i) {
        for (int j = 0; j < sizeMax; ++j) {
            if (grid[i][j] != nullptr) {
                checkFullConnectivity(i, j, visited, remainingPieces);
                connected = true;
                break;
            }
        }
    }

    // Check every piece has been visited by the tree search
    bool keepsGridTogether = visited.size() != remainingPieces.size();

    // Restore original grid state and return result
    grid[x][y] = temp;

    return keepsGridTogether;
}

std::vector<std::tuple<int,int>> Board::freeSlotsToAddPiece(Color color, int counter) {
    vector<tuple<int, int>> legals = {};

    if (counter == 1) {
        legals = {make_tuple(7,7)};
    }

    if (counter == 2) {
        legals = neighboursSlots(std::make_tuple(7,7));
    }

    // TODO itérer juste sur le pieceset
    else {
        for (int i = 0 ; i < getSizeMax(); i++) {
            for (int j = 0 ; j < getSizeMax(); j++) {
                if (grid[i][j] == nullptr) {
                    std::vector<Piece*> neighbors = neighboursPieces(make_tuple(i,j));
                    int c = 0;
                    if (neighbors.size() > 0){
                        for (auto neighbor : neighbors) {
                            if (neighbor->getColor() != color)
                                c ++;
                        }
                        if (c==0) {legals.emplace_back(i,j);}
                    }
                }
            }
        }
    }
    return legals;
}


bool Board::checkFullConnectivity(int x, int y, std::vector<std::tuple<int, int>>& visited, std::vector<std::tuple<int, int>>& remainingPieces) {
    visited.push_back({x, y});
    std::vector<Piece*> neighbors = neighboursPieces({x, y});

    for (Piece* neighbor : neighbors) {
        int nx = std::get<0>(neighbor->getPosition());
        int ny = std::get<1>(neighbor->getPosition());

        // TODO errors here, use tuple methods instead
        if (std::find(remainingPieces.begin(), remainingPieces.end(), std::make_tuple(nx, ny)) != remainingPieces.end() &&
            std::find(visited.begin(), visited.end(), std::make_tuple(nx, ny)) == visited.end()) {
            checkFullConnectivity(nx, ny, visited, remainingPieces);
        }
    }

    return true; // You might want to adjust this based on the actual functionality.
}


std::tuple<int, int> convertToTuple(const std::string& coord) {
    if (coord.length() < 2) {
        throw std::invalid_argument("Coordonnée invalide : elle doit contenir une lettre suivie d'un chiffre.");
    }

    // Extraction de la lettre et du chiffre
    char rowChar = coord[0];
    std::string colStr = coord.substr(1);

    // Vérification si les entrées sont valides
    if (!std::isalpha(rowChar) || !std::isdigit(colStr[0])) {
        throw std::invalid_argument("Coordonnée invalide : elle doit être au format 'LettreChiffre'.");
    }

    // Conversion
    int row = std::toupper(rowChar) - 'A';
    int col = std::stoi(colStr)-1;

    return std::make_tuple(row, col);
}

std::string Board::convertToString(const std::tuple<int, int>& coord){
    int row = get<0>(coord);
    int col = get<1>(coord)+1;
    char rowChar = 'A' + row;
    std::string colStr = std::to_string(col);
    return std::string(1, rowChar) + colStr;
}

bool Board::isEmpty() const { // TODO potentially useless
    // Create an iterator for the board
    Iterator it = iterator();

    // Iterate over the board using the iterator
    while (it.hasNext()) {
        // Check if the current slot has a piece
        if (it.current() != nullptr) {
            return false;  // If any piece is found, the board is not empty
        }
        it.next();  // Move to the next position
    }

    return true;  // If no pieces were found, the board is empty
}

