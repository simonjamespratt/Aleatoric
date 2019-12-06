#include "Engine.hpp"

namespace actlib { namespace RNG {
    std::random_device rd;
    const std::mt19937 engine(rd());
}}
