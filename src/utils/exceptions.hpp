#include <cstdint>
#include <stdexcept>
#include <typeindex>
#include <string>

namespace EXCEPTIONS {
    class ComponentNotFoundException : public std::runtime_error {
    public:
        explicit ComponentNotFoundException(const uint32_t entity_id, const std::type_index& component_type)
            : std::runtime_error("Component <" + std::string(component_type.name()) + "> not found for entity: " + std::to_string(entity_id)) {}
    };
} // namespace EXCEPTIONS