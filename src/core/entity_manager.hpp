#pragma once

#include "component.hpp"
#include "entity.hpp"

#include <unordered_map>
#include <typeindex>
#include <algorithm>
#include <vector>
#include <memory>

namespace CORE {
    class EntityManager;
    static EntityManager entityMan = EntityManager();

    class EntityManager {
    private:
        std::unordered_map<std::type_index, std::shared_ptr<Component>> component_storages;
        std::vector<std::unique_ptr<Entity>> entity_list;
        /* For future add vector of systems here like so 
            - add_system(BIT_MASK) -> this will work on all components with matching bit masks
        */
    public:
        EntityManager() {
            entity_list.reserve(MAX_ENTITIES); // eat this
        }
        ~EntityManager() = default;

        // this shi goes hard frfr
        template<typename T>
        ComponentCollection<T>& getComponentCollection() {
            auto ti = std::type_index(typeid(T));
            if (component_storages.find(ti) == component_storages.end()) {
                component_storages[ti] = std::make_shared<ComponentCollection<T>>();
            }
            return *std::static_pointer_cast<ComponentCollection<T>>(component_storages[ti]);
        }

        // CKUF API (no anagram here frfr) [danqzq: ..ohh fuck]
        Entity& create_entity() {
            auto entity = std::make_unique<Entity>(this);
            Entity& e_ref = *entity;
            entity_list.push_back(std::move(entity));
            return e_ref;
        }

        void kill_entity(EntityId entity_id) {
            auto it = std::find_if(entity_list.begin(), entity_list.end(), [entity_id](const Entity& e) {
                return entity_id == e.id;
            });
            if (it != entity_list.end()) {
                entity_list.erase(it);
            }
        }

        template <typename T>
        T& add_component(EntityId id) {
            auto component_collection = getComponentCollection<T>();
            return component_collection.add(id);
        }

        template <typename T>
        T& get_component(EntityId id) {
            auto component_collection = getComponentCollection<T>();
            return component_collection.get(id);
        }

        template <typename T>
        void remove_component(EntityId id) {
            auto component_collection = getComponentCollection<T>();
            component_collection.remove(id);
        }
    };
}