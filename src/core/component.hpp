#pragma once

#include "../utils/exceptions.hpp"

#include <cstdint>
#include <exception>
#include <string>
#include <vector>
#include <unordered_map>

namespace CORE {
    using EntityId = uint32_t;

    // Pls inherit me (,,>﹏<,,)
    struct Component {
        virtual ~Component() = default;
    };

    // Aggregate Component
    template <typename T>
    class ComponentCollection {
    private:
        std::vector<T> components {};
        std::unordered_map<EntityId, size_t> entity_to_index{}; // BLAZINGLY Fast query index number for entity
        std::unordered_map<size_t, EntityId> index_to_entity{}; // BLAZINGLY Fast query entity id for index

    public:
        T& add(uint32_t entity_id) {
            auto idx = components.size();
            components.emplace_back(); // *usb connected*
            entity_to_index[entity_id] = idx;
            index_to_entity[idx] = entity_id;
            return components[idx];
        }

        T& get(uint32_t entity_id) {
            auto it = entity_to_index.find(entity_id);
            if (it == entity_to_index.end()) {
                throw EXCEPTIONS::ComponentNotFoundException(entity_id, typeid(T));
            }
            return components[it->second];
        }

        void remove(uint32_t entity_id) {
            auto it = entity_to_index.find(entity_id);
            if (it == entity_to_index.end()) {
                throw EXCEPTIONS::ComponentNotFoundException(entity_id, typeid(T));
            }

            // swap to last and pop_back [sex sounds here] (mushie: ya haram bro)
            size_t last_idx = components.size() - 1;
            auto idx = entity_to_index[entity_id];
            if (last_idx != idx) {
                std::swap(components[idx], components[last_idx]);
                EntityId last_entity = it->second;
                entity_to_index[last_entity] = idx;
                index_to_entity[idx] = last_entity;
            }
            components.pop_back(); // *usb disconnected*
            entity_to_index.erase(entity_id);
            index_to_entity.erase(last_idx);
        }
    };
}