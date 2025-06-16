#pragma once

#include "entity_manager.hpp"

#include <cstdint>
#include <bitset>

namespace CORE {
    constexpr uint64_t MAX_ENTITIES = 1000000000;
    constexpr uint64_t ILLEGAL_ENTITY = MAX_ENTITIES + 1;
    constexpr uint16_t MAX_COMPONENTS = 64;

    class Entity {
    private:
        uint32_t id;
        static uint32_t next_entity_id;
        static uint32_t component_type_id;
        std::bitset<MAX_COMPONENTS> component_mask;
        EntityManager* entityMan {};

        // we are friends with the kitty man ≽^•⩊•^≼
        friend class EntityManager;

        // Some sort of Entity manager*
    public:
        Entity(EntityManager* em) : id(next_entity_id++), entityMan(em) {}

        uint64_t get_id() const{
            return this->id;
        }

        // Bary wishes he was this cool 
        template <typename T> 
        bool has_component() const{
            return component_mask[get_component_type<T>()];
        }

        template <typename T>
        T& get_component() const {
            return entityMan->get_component<T>(this->id);
        }

        template <typename T>
        T& add_component() {
            this->component_mask.set(get_component_type<T>());
            return entityMan->add_component<T>(this->id);
        }

        template <typename T>
        void remove_component() {
            this->component_mask.reset(get_component_type<T>());
            entityMan->remove_component<T>(this->id);
        }

        // Generate component_map index for new component type
        template<typename T>
        static uint32_t get_component_type() {
            static uint8_t type_id = component_type_id++;
            return type_id;
        }
    };
    
    uint32_t Entity::next_entity_id    = 0;
    uint32_t Entity::component_type_id = 0;



} // namespace CORE