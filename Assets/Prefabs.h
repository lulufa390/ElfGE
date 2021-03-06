//
// Created by 张程易 on 30/10/2017.
//

#ifndef ELFGE_PREFABS_H
#define ELFGE_PREFABS_H

#include <UI/Image.h>
#include "GameEngine.h"
#include "StandardAssets.h"
#include <random>
#include "Prefab/ScrollBar.h"
#include "Prefab/ImageButton.h"

namespace Prefabs {

    extern StandardMaterial material;



    class PointLight : public Prefab {

        glm::vec3 position;
    public:
        PointLight(const glm::vec3 &position) : position(position) {}

        GameObject *instantiate(Scene *scene) override {
            auto result = scene->createGameObject()
                    ->createComponent<PointLighting>();
            result->transform.setPosition(position);
            return result;
        }
    };
    class Cube : public Prefab {
        glm::vec3 position;
    public:
        Cube(const glm::vec3 &position) : position(position) {}

        GameObject *instantiate(Scene *scene) override {
            static int num = 0;
            auto result = scene->createGameObject("cube" + std::to_string(num++))
                    ->createComponent<DefaultModel>(
                            new CubeMesh("bricks2.jpg", "bricks2.jpg", "bricks2_normal.jpg", "bricks2_disp.jpg"))
                    ->createComponent<Renderer>(
                            &material, ShaderManager::getShader("light_ds_pm"))
                    ->createComponent<AABBCollider>(true);

            result->transform.setPosition(position);
            return result;
        }
    };

    class Bomb : public Prefab {
        glm::vec3 position;
    public:
        Bomb(const glm::vec3 &position) : position(position) {}

        GameObject *instantiate(Scene *scene) override {
            static int num = 0;
            auto result = scene->createGameObject("cube" + std::to_string(num++))
                    ->createComponent<DefaultModel>(
                            Utility::RESOURCE_PREFIX + "Models/bomb/dynamite.obj"
                    )
                    ->createComponent<Renderer>(
                            &material, ShaderManager::getShader("light_with_directional_shadow"));
            result->transform.setScale(glm::vec3(0.005f));
            result->transform.rotate(Transform::forward, -glm::half_pi<float>());
            result->transform.setPosition(position);
            return result;
        }
    };

    class Plane : public Prefab {
        glm::vec3 position;
    public:
        Plane(const glm::vec3 &position) : position(position) {}

        GameObject *instantiate(Scene *scene) override {

            auto result = scene->createGameObject()
//                    ->createComponent<DefaultModel>(new PlaneMesh("brickwall.jpg", "brickwall.jpg", "brickwall_normal.jpg"))
                    ->createComponent<DefaultModel>(new PlaneMesh("", "", ""))
                    ->createComponent<Renderer>(
                            nullptr, ShaderManager::getShader("simple"));
            result->transform.setPosition(position);
            return result;
        }
    };


    class Camera: public Prefab{
    public:
        GameObject *instantiate(Scene *scene) override {
            auto camera = scene->createGameObject()
                    ->createComponent<FirstPlayerCamera>();

            scene->setCamera(camera->getComponent<FirstPlayerCamera>());
            return camera;
        }
    };




};

#endif //ELFGE_PREFABS_H
