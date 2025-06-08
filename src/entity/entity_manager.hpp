#pragma once

#include "entity.hpp"

#include <vector>
#include <iostream>
#include <algorithm>

namespace ENTITY {

    class EntityManager {
    public:
        EntityManager() = default;
        ~EntityManager() = default;

        // Add an entity to the manager
        void add_entity(Entity* entity) {
            if (entity) {
                entities.push_back(entity);
            } else {
                std::cerr << "Attempted to add a null entity!" << std::endl;
            }
        }

        // Remove an entity from the manager
        void remove_entity(Entity* entity) {
            auto it = std::remove(entities.begin(), entities.end(), entity);
            if (it != entities.end()) {
                entities.erase(it, entities.end());
            } else {
                std::cerr << "Entity not found in manager!" << std::endl;
            }
        }

        // Get all entities
        const std::vector<Entity*>& get_entities() const {
            return entities;
        }

    private:
        std::vector<Entity*> entities; // Store pointers to entities
    };

} // namespace ENTITY