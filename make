#!/bin/bash
g++ main.cpp graphics.cpp physics.cpp util.cpp map.cpp parser.cpp assets.cpp game.cpp collisions.cpp -I ./include -o sfml-app -L ./lib -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lsfml-network -ltinyxml2
