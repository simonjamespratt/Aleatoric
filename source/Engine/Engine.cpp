#include "Engine.hpp"

namespace actlib { namespace RNG {
    Engine* Engine::_instance = 0;

    Engine::Engine(std::random_device::result_type seed) : engine(seed)
    {}

    Engine* Engine::Instance() {
        if (_instance == 0)
        {
            std::random_device rd;
            _instance = new Engine(rd());
        }

        return _instance;
    }

    std::mt19937& Engine::getEngine() {
        return engine;
    }
}}
