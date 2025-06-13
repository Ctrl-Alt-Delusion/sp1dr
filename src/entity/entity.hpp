#pragma once

#include "../component/component.hpp"

#include <vector>
#include <memory>

namespace ENTITY {
    class Entity {
    private:
        int id; // Unique identifier for the entity
        std::vector<std::shared_ptr<COMPONENT::Component>> components; // Components associated with the entity

        static int entity_count;

    public:
        explicit Entity() {
            id = entity_count++;
            components.reserve(10);
        }

        int get_id() const {
            return id;
        }

        bool add_component(std::shared_ptr<COMPONENT::Component> component) {
            components.push_back(component);
            return true;
        }

        template<typename T>
        bool remove_component_by_type() {
            for (auto it = components.begin(); it != components.end(); ++it) {
                if (std::dynamic_pointer_cast<T>(*it)) {
                    components.erase(it);
                    return true;
                }
            }
            return false;
        }
    };
}