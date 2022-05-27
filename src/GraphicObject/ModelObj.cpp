/*
** EPITECH PROJECT, 2022
** ModelObj.cpp
** File description:
** ModelObj
*/

#include "ModelObj.hpp"

using namespace neo;

ModelObj::ModelObj(std::string name, Vector2 pos) : GraphicObject(name, pos)
{
    this->_model = std::make_unique<Model>(name);
}

ModelObj::ModelObj(std::string name, Vector2 pos, Vector2 size) : GraphicObject(name, pos, size)
{
    this->_model = std::make_unique<Model>(name);
}

void ModelObj::draw(neo::Shader &shader)
{
    this->_model->draw(shader);
}
