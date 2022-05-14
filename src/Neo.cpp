/*
** EPITECH PROJECT, 2022
** Neo.cpp
** File description:
** Neo
*/

#include "Neo.hpp"

using namespace neo;

Neo::Neo() :
_messageBus(std::make_shared<MessageBus>()),
_audio(_messageBus),
_game(_messageBus),
_graphics(_messageBus),
_input(_messageBus),
_loader(_messageBus)
{
    std::cout << "Engine created" << std::endl;
}

void Neo::run()
{
    while (!WindowShouldClose()) {
        this->_input.update();
        this->_game.update();
        this->_graphics.draw();

        this->_messageBus->notify();
    }
}