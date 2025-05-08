cmake_minimum_required(VERSION 3.29)
project(lo21_hive)

set(CMAKE_CXX_STANDARD 20)

add_executable(lo21_hive main.cpp
g        game.h
        board.h
        menu.h
        Piece/piece.h
        Piece/piece.cpp
        Player/player.h
        pieceSet.h
        pieceSet.cpp
        move.h
        Player/AiStrategy/aiStrategy.h
        board.cpp
        game.cpp
        move.cpp
        Player/player.cpp
        menu.cpp
        Player/AiStrategy/aiStrategy.cpp
        Player/human.cpp)

