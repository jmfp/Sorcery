#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Transform {
    public:
        Transform();
        ~Transform();
        // Local space
        glm::vec3 position = {0.0f, 0.0f, 0.0f};
        glm::vec3 rotation = {0.0f, 0.0f, 0.0f};
        glm::vec3 scale = {1.0f, 1.0f, 1.0f};

        // Global space (world space)
        glm::mat4 modelMatrix = glm::mat4(1.0f);

        bool m_isDirty = true;

        void computeModelMatrix()
        {
            modelMatrix = getLocalModelMatrix();
            m_isDirty = false;
        }

        void computeModelMatrix(const glm::mat4& parentGlobalModelMatrix)
        {
            modelMatrix = parentGlobalModelMatrix * getLocalModelMatrix();
            m_isDirty = false;
        }

        void setLocalPosition(const glm::vec3& newPosition)
        {
            position = newPosition;
            m_isDirty = true;
        }
        const glm::vec3& getLocalPosition()
        {
            return position;
        }
        const glm::mat4& getModelMatrix()
        {
            return modelMatrix;
        }

        bool isDirty()
        {
            return m_isDirty;
        }

        void SetPosition(const glm::vec3& pos) {
            position = pos;
            m_isDirty = true;
        }

        void SetRotation(const glm::vec3& rot) {
            rotation = rot;
            m_isDirty = true;
        }

        void SetScale(const glm::vec3& scl) {
            scale = scl;
            m_isDirty = true;
        }

    protected:
        glm::mat4 getLocalModelMatrix()
        {
            const glm::mat4 transformX = glm::rotate(glm::mat4(1.0f),
                        glm::radians(rotation.x),
                        glm::vec3(1.0f, 0.0f, 0.0f));
            const glm::mat4 transformY = glm::rotate(glm::mat4(1.0f),
                        glm::radians(rotation.y),
                        glm::vec3(0.0f, 1.0f, 0.0f));
            const glm::mat4 transformZ = glm::rotate(glm::mat4(1.0f),
                        glm::radians(rotation.z),
                        glm::vec3(0.0f, 0.0f, 1.0f));

            // Y * X * Z
            const glm::mat4 roationMatrix = transformY * transformX * transformZ;

            // translation * rotation * scale (also know as TRS matrix)
            return glm::translate(glm::mat4(1.0f), position) *
                        roationMatrix *
                        glm::scale(glm::mat4(1.0f), scale);
        }
};