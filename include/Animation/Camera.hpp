/*
** EPITECH PROJECT, 2022
** Camera.hpp
** File description:
** Camera
*/

#ifndef CAMERA_HPP
    #define CAMERA_HPP

    #include "includes.hpp"
    #include "Shader.hpp"
    #include "Animator.hpp"

    #include <GL/glew.h>
    #include <glm/glm.hpp>
    #include <glm/gtc/matrix_transform.hpp>

namespace neo
{
    class Camera
    {
        private:
            glm::mat4 _view;
            glm::vec3 _pos;
            glm::vec3 _front;
            glm::vec3 _up;
            glm::mat4 _projection;
            glm::mat4 _model;
            Shader _shader;
            bool _rotating;
            glm::vec3 _nextPos;
            glm::vec3 _nextFront;
            glm::vec3 _rotation;

        public:
            Camera();
            ~Camera() = default;
            Shader &getShader();
            void update();
            void setMovement(glm::vec3 const &nextPos, glm::vec3 const &nextFront);
            void setPos(glm::vec3 const &pos);
            void setRotating(bool rotating);
            void setRotation(glm::vec3 const &rotation);
            void setShader(float time);
            void setOnModel(glm::vec3 pos, glm::vec3 scale, glm::vec3 rotation);
            void setOnAnimatedModel(glm::vec3 pos, glm::vec3 scale, glm::vec3 rotation, Animator &animator);
    };
}

#endif // CAMERA_HPP
