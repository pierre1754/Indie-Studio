/*
** EPITECH PROJECT, 2022
** BotEngine.cpp
** File description:
** BotEngine
*/

#include "BotEngine.hpp"

using namespace neo;

BotEngine::BotEngine()
{
    this->_visited = std::vector<std::vector<bool>>(20, std::vector<bool>(20, false));
}

void BotEngine::doAction(GameScene *gameScene, const int &bot_key, std::string action, bool isPressed)
{
    Packet data;

    data << bot_key << action;
    gameScene->getMessageBus()->sendMessage(Message(data, isPressed ? CoreCommand::KEY_PRESSED : CoreCommand::KEY_RELEASED, Module::CORE));
}

bool BotEngine::canMoveToPos(GameScene *gameScene, glm::vec3 pos)
{
    for (auto &[wall_key, wall] : gameScene->getWalls())
        if (pos.x + 0.5f == wall->getPos().x && pos.y - 0.5f == wall->getPos().y)
            return false;
    for (auto &[bomb_key, bomb] : gameScene->getBombs())
        if (pos.x + 0.5f == bomb->getPos().x && pos.y - 0.5f == bomb->getPos().y)
            return false;
    return true;
}

int BotEngine::getNeighbor(GameScene *gameScene, glm::vec3 pos)
{
    if (canMoveToPos(gameScene, glm::vec3(pos.x + 1.0f, pos.y, pos.z)) &&
        this->_visited[-pos.y + gameScene->getMapGenerator().getHeight() / 2][pos.x + gameScene->getMapGenerator().getWidth() / 2 + 1] == false) {
        return 1;
    }
    else if (canMoveToPos(gameScene, glm::vec3(pos.x, pos.y + 1.0f, pos.z)) &&
        this->_visited[-pos.y + gameScene->getMapGenerator().getHeight() / 2 - 1][pos.x + gameScene->getMapGenerator().getWidth() / 2] == false) {
        return 2;
    }
    else if (canMoveToPos(gameScene, glm::vec3(pos.x - 1.0f, pos.y, pos.z)) &&
        this->_visited[-pos.y + gameScene->getMapGenerator().getHeight() / 2][pos.x + gameScene->getMapGenerator().getWidth() / 2 - 1] == false) {
        return 3;
    }
    else if (canMoveToPos(gameScene, glm::vec3(pos.x, pos.y - 1.0f, pos.z)) &&
        this->_visited[-pos.y + gameScene->getMapGenerator().getHeight() / 2 + 1][pos.x + gameScene->getMapGenerator().getWidth() / 2] == false) {
        return 4;
    }
    return 0;
}

void BotEngine::checkEnd(GameScene *gameScene, glm::vec3 pos, const int &bot_key)
{
    this->_visited[-pos.y + gameScene->getMapGenerator().getHeight() / 2][pos.x + gameScene->getMapGenerator().getWidth() / 2] = true;
    for (auto &[bomb_key, bomb] : gameScene->getBombs())
        if (pos.x + 0.5f == bomb->getPos().x &&
            bomb->getPos().y + 2.5f + 1.0f * bomb->getFireUp() > pos.y &&
            pos.y > bomb->getPos().y - 2.5f - 1.0f * bomb->getFireUp() ||
            pos.y - 0.5f == bomb->getPos().y &&
            bomb->getPos().x + 2.5f + 1.0f * bomb->getFireUp() > pos.x &&
            pos.x > bomb->getPos().x - 2.5f - 1.0f * bomb->getFireUp())
            return;
    this->_founds[bot_key] = true;
}

void BotEngine::recursive(GameScene *gameScene, glm::vec3 pos, const int &bot_key)
{
    int neighbor = getNeighbor(gameScene, pos);

    this->checkEnd(gameScene, pos, bot_key);
    while (neighbor && !this->_founds[bot_key]) {
        switch (neighbor) {
            case 1: this->recursive(gameScene, glm::vec3(pos.x + 1.0f, pos.y, pos.z), bot_key);
                break;
            case 2: this->recursive(gameScene, glm::vec3(pos.x, pos.y + 1.0f, pos.z), bot_key);
                break;
            case 3: this->recursive(gameScene, glm::vec3(pos.x - 1.0f, pos.y, pos.z), bot_key);
                break;
            case 4: this->recursive(gameScene, glm::vec3(pos.x, pos.y - 1.0f, pos.z), bot_key);
                break;
            default:
                break;
        }
        neighbor = getNeighbor(gameScene, pos);
    }
    if (this->_founds[bot_key])
        this->_paths[bot_key].push_back(glm::vec3(pos.x + 0.5f, pos.y - 0.5f, pos.z));
}

void BotEngine::dodgeBombs(GameScene *gameScene, const int &bot_key, std::unique_ptr<Player> &bot)
{
    for (auto &[bomb_key, bomb] : gameScene->getBombs()) {
        if (std::floor(bot->getPos().x) + 0.5f == bomb->getPos().x &&
            bomb->getPos().y + 2.5f + 1.0f * bomb->getFireUp() > bot->getPos().y &&
            bot->getPos().y > bomb->getPos().y - 2.5f - 1.0f * bomb->getFireUp() ||
            std::floor(bot->getPos().y) + 0.5f == bomb->getPos().y &&
            bomb->getPos().x + 2.5f + 1.0f * bomb->getFireUp() > bot->getPos().x &&
            bot->getPos().x > bomb->getPos().x - 2.5f - 1.0f * bomb->getFireUp()) {
            if (this->_paths[bot_key].empty() && !this->_founds[bot_key]) {
                this->_founds[bot_key] = false;
                recursive(gameScene, glm::vec3(std::floor(bot->getPos().x), std::ceil(bot->getPos().y), bot->getPos().z), bot_key);
                for (size_t i = 0; i < this->_visited.size(); i++)
                    for (size_t j = 0; j < this->_visited.size(); j++)
                        this->_visited[i][j] = false;
                if (!this->_paths[bot_key].empty()) {
                    this->_paths[bot_key].pop_back();
                    this->_founds[bot_key] = false;
                }
                else
                    this->_founds[bot_key] = true;
            }
        }
        if (!this->_paths[bot_key].empty()) {
            if (!this->_founds[bot_key]) {
                if (_paths[bot_key].back().x == std::floor(bot->getPos().x) + 1.5f) {
                    doAction(gameScene, bot_key, "MoveRight", true);
                    this->_directions[bot_key] = RIGHT;
                }
                if (_paths[bot_key].back().y == std::floor(bot->getPos().y) + 1.5f) {
                    doAction(gameScene, bot_key, "MoveUp", true);
                    this->_directions[bot_key] = UP;
                }
                if (_paths[bot_key].back().x == std::floor(bot->getPos().x) - 0.5f) {
                    doAction(gameScene, bot_key, "MoveLeft", true);
                    this->_directions[bot_key] = LEFT;
                }
                if (_paths[bot_key].back().y == std::floor(bot->getPos().y) - 0.5f) {
                    doAction(gameScene, bot_key, "MoveDown", true);
                    this->_directions[bot_key] = DOWN;
                }
                this->_founds[bot_key] = true;
            }
            else {
                if (this->_directions[bot_key] == RIGHT && bot->getPos().x > _paths[bot_key].back().x ||
                    this->_directions[bot_key] == UP && bot->getPos().y > _paths[bot_key].back().y ||
                    this->_directions[bot_key] == LEFT && bot->getPos().x < _paths[bot_key].back().x ||
                    this->_directions[bot_key] == DOWN && bot->getPos().y < _paths[bot_key].back().y) {
                    if (this->_directions[bot_key] == RIGHT)
                        doAction(gameScene, bot_key, "MoveRight", false);
                    if (this->_directions[bot_key] == UP)
                        doAction(gameScene, bot_key, "MoveUp", false);
                    if (this->_directions[bot_key] == LEFT)
                        doAction(gameScene, bot_key, "MoveLeft", false);
                    if (this->_directions[bot_key] == DOWN)
                        doAction(gameScene, bot_key, "MoveDown", false);
                    this->_paths[bot_key].pop_back();
                    this->_founds[bot_key] = false;
                }
            }
        }
    }
}

void BotEngine::updateBot(GameScene *gameScene)
{
    for (auto &[player_key, player] : gameScene->getPlayers()) {
        if (!player->isBot())
            continue;
        dodgeBombs(gameScene, player_key, player);
    }
}
