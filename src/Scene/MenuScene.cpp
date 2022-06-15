/*
** EPITECH PROJECT, 2022
** MenuScene.cpp
** File description:
** MenuScene
*/

#include "MenuScene.hpp"

using namespace neo;

MenuScene::MenuScene(std::shared_ptr<MessageBus> messageBus)
{
    this->_messageBus = messageBus;
    this->_objects.insert(std::make_pair(0, std::make_unique<GameObject>(0, "Asteroid", glm::vec3(0.0f, -5.0f, -1.5f), glm::vec3(0.5f))));
    this->_objects.insert(std::make_pair(1, std::make_unique<GameObject>(0, "Asteroid", glm::vec3(0.0f, -5.0f, 0.0f), glm::vec3(0.5f))));
    this->_objects.insert(std::make_pair(2, std::make_unique<GameObject>(0, "Asteroid", glm::vec3(0.0f, -5.0f, 1.5f), glm::vec3(0.5f))));
    this->_objects.insert(std::make_pair(3, std::make_unique<GameObject>(0, "Asteroid", glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.5f))));
    this->_objects.insert(std::make_pair(4, std::make_unique<GameObject>(0, "SphereBackground", glm::vec3(0.0f), glm::vec3(70.0f), false)));

    this->_selectedButton = -1;
}

MenuScene::~MenuScene()
{
    for (auto &object : this->_objects)
        object.second.reset();
    for (auto &button : this->_buttons)
        button.second.reset();
    this->_objects.clear();
    this->_buttons.clear();
}

void MenuScene::update()
{
    Vector2 pos = GetMousePosition();

    Packet packet;
    packet << pos.x << pos.y << 0;
    this->_messageBus->sendMessage(Message(packet, GraphicsCommand::SET_CAMERA_OFFSET, Module::GRAPHICS));
}

void MenuScene::loadScene()
{
    Packet packet;

    for (auto &object : this->_objects)
        packet << object.second->getType() << object.first << *object.second;
    for (auto &button : this->_buttons)
        packet << button.second->getType() <<  button.first << *button.second;
    this->_messageBus->sendMessage(Message(packet, GraphicsCommand::LOAD, Module::GRAPHICS));

    packet.clear();
    packet << 1 << glm::vec3(10.0f, 0.0f, 10.0f);
    this->_messageBus->sendMessage(Message(packet, GraphicsCommand::SET_CAMERA_POS, Module::GRAPHICS));
    packet.clear();
    packet << 0 << glm::vec3(10.0f, 0.0f, 3.0f);
    this->_messageBus->sendMessage(Message(packet, GraphicsCommand::SET_CAMERA_NEXT_POS, Module::GRAPHICS));
}

void MenuScene::handleKeyPressed(int playerNb, std::string action)
{
    if (playerNb != 0)
        return;
    this->handleBackPressed(playerNb, action);
    this->handleMainPressed(playerNb, action);
    this->handleMovePressed(playerNb, action);
}

void MenuScene::handleKeyReleased(int playerNb, std::string action)
{
}

void MenuScene::handleMainPressed(int playerNb, std::string action)
{
    if (action == "Main") {
        Packet packet;
        packet << this->_selectedButton << 0;
        this->_messageBus->sendMessage(Message(packet, GraphicsCommand::SELECT_BUTTON, Module::GRAPHICS));
        this->_selectedButton = -1;
        return;
    }
}

void MenuScene::handleBackPressed(int playerNb, std::string action)
{
    if (action == "Back") {
        if (this->_selectedButton == -1)
            return;
        if (this->_selectedButton == 0)
            return;
        Packet packet;
        packet << this->_selectedButton;
        this->_messageBus->sendMessage(Message(packet, CoreCommand::CHANGE_SCENE, Module::CORE));
        this->_selectedButton = -1;
    }
}

void MenuScene::handleMovePressed(int playerNb, std::string action)
{
    if (action == "MoveUp" || action == "MoveDown") {
        Packet packet;
        packet << this->_selectedButton << 0;
        this->_messageBus->sendMessage(Message(packet, GraphicsCommand::SELECT_BUTTON, Module::GRAPHICS));
        packet.clear();
        if (action == "MoveUp")
            this->_selectedButton = (this->_selectedButton + 1) % 3;
        else {
            if (this->_selectedButton <= 0)
                this->_selectedButton = 3;
            this->_selectedButton = (this->_selectedButton - 1) % 3;
        }
        packet << this->_selectedButton << 1;
        this->_messageBus->sendMessage(Message(packet, GraphicsCommand::SELECT_BUTTON, Module::GRAPHICS));
    }
}
