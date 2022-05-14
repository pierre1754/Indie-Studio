/*
** EPITECH PROJECT, 2022
** Game.cpp
** File description:
** Game
*/

#include "Game.hpp"

using namespace neo;

Game::Game(std::shared_ptr<MessageBus> messageBus) : Node(messageBus)
{
    std::cout << "Game module created" << std::endl;
}

void Game::onNotify(Message message)
{
    Packet data = message.getData();
    float nb;

    data >> nb;
    std::cout << nb << std::endl;
    // from data, handle player movement/bombs etc
}

void Game::update()
{

}