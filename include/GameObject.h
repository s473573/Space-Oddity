#pragma once
#include <Mesh.h>
#include <glm/glm.hpp>
#include "Material.h"
#include "Transform.h"
#include "AABB.h"

namespace so
{
    class GameObject
    {
        protected:
            std::shared_ptr<Mesh> meshPtr;
            GameObject *parent;
            std::vector<GameObject*> children;
            glm::mat4 transformMatrix;
            int classType;
            static GLuint modelLocation;
            
            void constructAABB();
            void recalculateAABB();
        
        public:
            Transform transform;
            // Rigidbody rigidbody;
            Material material;
            AABB aabb;
            std::vector<GameObject *> getChildren();
            bool isStatic;
            

            /** \param mesh The mesh of the GameObject.
             * \param material The material coating the mesh.
             * \param parent The transform parent of the GameObject.
             * \param isStatic Decides whether the GameObject has physics applied or not.
             * \param type The class type of the GameObject, a value of CLASSES.
             */
            GameObject(std::shared_ptr<Mesh> mesh, Material material,
                       GameObject *parent = nullptr, bool isStatic = false, int type = 1);
            
            /** \param mesh The mesh of the GameObject.
            * \param material The material coating the mesh.
            * \param parent The transform parent of the GameObject.
            * \param isStatic Decides whether the GameObject has physics applied or not.
            * \param type The class type of the GameObject, a value of CLASSES.
            */
            GameObject(Mesh mesh, Material material, GameObject *parent = nullptr,
                      bool isStatic = false, int type = 1);
            
            GameObject *getParent();
            void setParent(GameObject *parent);

            std::shared_ptr<Mesh> getMesh();
            void setMesh(std::shared_ptr<Mesh> mesh);

            glm::mat4 applyTransform();
            virtual void draw();
            
            // virtual void fixedUpdate(float deltatime);
            
            void addChild(GameObject *gameObject);
            
            int getClass();
            
            virtual void start() = 0;
            virtual void update() = 0;
            
            static void setModelLocation(GLuint location);
    };
}