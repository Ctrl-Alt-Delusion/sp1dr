#pragma once
#include "entity.hpp"
#include <vector>
#include <memory>

namespace ENTITY {
    class EntityManager {
    private:
        std::vector<std::shared_ptr<Entity>> entities;

    public:
        void add_entity(const std::shared_ptr<Entity>& entity) {
            entities.push_back(entity);
        }

        const std::vector<std::shared_ptr<Entity>>& get_entities() const {
            return entities;
        }
    };
} // namespace ENTITY
