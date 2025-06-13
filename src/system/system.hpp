namespace SYSTEM {
    // System interface for processing entities
    class System {
    public:
        virtual ~System() = default;

        // Process function to be implemented by derived systems
        virtual void process() = 0;
    };
}