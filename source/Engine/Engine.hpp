#ifndef Engine_hpp
#define Engine_hpp

#include <random>

namespace actlib { namespace RNG {
    /** ENGINE SINGLETON CLASS
     * Uses the more traditional singleton approach as per GoF Design Patterns.
     * Thus it relies on pointers and heap allocation.
     * Incoporation of the mt19937 and random_device is based somewhat on this SO post:
     * https://stackoverflow.com/questions/33795478/how-to-use-c11-random-library-in-a-large-application
     * Another, potentially better (more modern), solution is the Meyers Singleton.
     * This version is stack allocated and potentially thread safe.
     *
     * Also note that the decision was taken to encourage a long lived singleton by requiring
     * the mt19937 engine to be passed down through classes from the outermost interface,
     * with the Engine class having been instantiated at the start of the library use by the end user (programmer).
     * See the yUML diagrams for more details of how this occurs.
     */
    class Engine
    {
    public:
        static Engine* Instance();
        std::mt19937& getEngine();
    protected:
        Engine(std::random_device::result_type seed);
    private:
        static Engine* _instance;
        std::mt19937 engine;
    };


}}

#endif /* Engine_hpp */
