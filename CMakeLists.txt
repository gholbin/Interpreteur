cmake_minimum_required(VERSION 3.10)
project(M3105_TP5___Copie)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(M3105_TP5___Copie
        ArbreAbstrait.cpp
        Interpreteur.cpp
        Lecteur.cpp
        Symbole.cpp
        SymboleValue.cpp
        TableSymboles.cpp
        main.cpp)
