#include "Engine.hpp"

#include <random>

namespace aleatoric {
Engine::Engine() : m_engine(pcg_extras::seed_seq_from<std::random_device>())
{}

pcg32 &Engine::getEngine()
{
    return m_engine;
}
} // namespace aleatoric
