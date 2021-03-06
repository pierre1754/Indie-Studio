/*
** EPITECH PROJECT, 2022
** Animation.hpp
** File description:
** Animation
*/

#ifndef ANIMATION_HPP
    #define ANIMATION_HPP

    #include "includes.hpp"
    #include "Bone.hpp"
    #include "Model.hpp"

    #include <glm/glm.hpp>
    #include <glm/gtc/matrix_transform.hpp>
    #include <assimp/Importer.hpp>
    #include <assimp/scene.h>
    #include <assimp/postprocess.h>

namespace neo
{
    typedef struct AssimpNodeData_s {
        glm::mat4 transformation;
        std::string name;
        int childrenCount;
        std::vector<AssimpNodeData_s> children;
    } AssimpNodeData;

    class Animation
    {
        private:
            float _duration;
            int _tickPerSecond;
            std::vector<Bone> _bones;
            AssimpNodeData _rootNode;
            std::map<std::string, BoneInfo> _boneInfoMap;

        public:
            Animation() = default;
            Animation(const std::string &path, Model &model);
            ~Animation() = default;

            float getTicksPerSecond() const;
            float getDuration() const;
            const AssimpNodeData &getRootNode() const;
            const std::map<std::string, BoneInfo> &getBoneInfoMap() const;

            std::unique_ptr<Bone> findBone(const std::string &name);
            void readMissingBones(const aiAnimation &animation, Model &model);
            void readHierarchyData(AssimpNodeData &dest, const aiNode &src);
    };
}

#endif // ANIMATION_HPP
