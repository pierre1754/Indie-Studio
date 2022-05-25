/*
** EPITECH PROJECT, 2022
** MenuScene.cpp
** File description:
** MenuScene
*/

#include "MenuScene.hpp"

using namespace neo;

MenuScene::MenuScene()
{
    this->_rectangle = {25, 25, 100, 100};
    this->_modelPos = {0, 0};
}

void MenuScene::update()
{
}

void MenuScene::loadScene(std::shared_ptr<MessageBus> messageBus)
{
    Packet packet;

    packet << "loadScene";
    messageBus->sendMessage(Message(packet, 0, 2));
}

void MenuScene::handleKeyPressed(int playerNb, std::string action)
{
}

void MenuScene::handleKeyRelease(int playerNb, std::string action)
{
}
