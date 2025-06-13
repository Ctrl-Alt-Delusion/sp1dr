#include "../component/component.hpp"

#include <vector>
#include <memory>

namespace ENTITY {
    struct Entity {
        int id; // Unique identifier for the entity
        std::vector<std::shared_ptr<COMPONENT::Component>> components; // Components associated with the entity
    };
    
    // Function to create a new entity
    inline Entity createEntity(int id) {
        return Entity{id};
    }
    
    // Function to get the ID of an entity
    inline int getEntityId(const Entity& entity) {
        return entity.id;
    }
}