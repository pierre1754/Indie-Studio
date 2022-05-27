/*
** EPITECH PROJECT, 2022
** SpriteObj.hpp
** File description:
** SpriteObj
*/

#ifndef SPRITE_OBJ_HPP
    #define SPRITE_OBJ_HPP

    #include "includes.hpp"
    #include "GraphicObject.hpp"

namespace neo
{
    class SpriteObj : public GraphicObject
    {
        private:
        public:
            SpriteObj(std::string name, glm::vec3 pos);
            SpriteObj(std::string name, glm::vec3 pos, glm::vec3 size);
            ~SpriteObj() = default;

            void draw(neo::Shader &shader);
    };
}

#endif // SPRITE_OBJ_HPP
