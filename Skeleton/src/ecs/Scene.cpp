#include "Scene.h"
#include "Entity.h"

namespace ECS {

    Scene::Scene(const std::string& sce_name) {
        name = sce_name;
    }

    Scene::~Scene() {
        for (auto e : entities) {
            delete e;
        }
        entities.clear();
    }

    void Scene::init() {
        for (auto e : entities) {
            e->init();
        }
    }

    void Scene::update() {

        for (auto it = entities.begin(); it != entities.end(); it++) {
            Entity* e = *it;

            if (e->isActive() && !e->isRemoved())
                e->udpate();
            else if (e->isRemoved())
                entitiesRemoved.push_back(it);
        }
    }

    void Scene::fixedUpdate() {
        for (auto e : entities) {
            e->fixedUpdate();
        }
    }

    void Scene::onDestroy() {
        for (auto it : entitiesRemoved) {
            (*it)->onDestroy();
            delete* it;
            entities.erase(it);
        }
        entitiesRemoved.clear();
    }

    void Scene::onSceneUp() {
        for (auto e : entities) {
            e->onSceneUp();
        }
    }

    void Scene::onSceneDown() {
        for (auto e : entities) {
            e->onSceneDown();
        }
    }

    std::string Scene::getName() {
        return name;
    }

    void Scene::setName(const std::string& sce_name) {
        name = sce_name;
    }

    Entity* Scene::createEntity(const std::string& name) {
        
        Entity* e = new Entity(name, this);
        entities.push_back(e);

        return e;

    }

    void Scene::destroyEntity(Entity* ent) {
        ent->removeEntity();
    }

    Entity* Scene::findEntityByName(const std::string& ent_name) {
        for (auto e : entities) {
            if (e->name == name)  return e;
        }

        return nullptr;
    }
}
