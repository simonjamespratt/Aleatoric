#include "NumberProtocol.hpp"

#include "AdjacentSteps.hpp"
#include "Basic.hpp"
#include "Cycle.hpp"
#include "DiscreteGenerator.hpp"
#include "GranularWalk.hpp"
#include "GroupedRepetition.hpp"
#include "NoRepetition.hpp"
#include "Periodic.hpp"
#include "Precision.hpp"
#include "Ratio.hpp"
#include "Serial.hpp"
#include "Subset.hpp"
#include "UniformGenerator.hpp"
#include "Walk.hpp"

namespace aleatoric {
std::unique_ptr<NumberProtocol> NumberProtocol::create(Type type)
{
    switch(type) {
    case Type::adjacentSteps:
        return std::make_unique<AdjacentSteps>(
            std::make_unique<DiscreteGenerator>());
    case Type::basic:
        return std::make_unique<Basic>(std::make_unique<UniformGenerator>());
    case Type::cycle:
        return std::make_unique<Cycle>();
    case Type::granularWalk:
        return std::make_unique<GranularWalk>(
            std::make_unique<UniformGenerator>());
    case Type::groupedRepetition:
        return std::make_unique<GroupedRepetition>(
            std::make_unique<DiscreteGenerator>(),
            std::make_unique<DiscreteGenerator>());
    case Type::noRepetition:
        return std::make_unique<NoRepetition>(
            std::make_unique<DiscreteGenerator>());
    case Type::periodic:
        return std::make_unique<Periodic>(
            std::make_unique<DiscreteGenerator>());
    case Type::precision:
        return std::make_unique<Precision>(
            std::make_unique<DiscreteGenerator>());
    case Type::ratio:
        return std::make_unique<Ratio>(std::make_unique<DiscreteGenerator>());
    case Type::serial:
        return std::make_unique<Serial>(std::make_unique<DiscreteGenerator>());
    case Type::subset:
        return std::make_unique<Subset>(std::make_unique<UniformGenerator>(),
                                        std::make_unique<DiscreteGenerator>());
    case Type::walk:
        return std::make_unique<Walk>(std::make_unique<UniformGenerator>());

    default:
        throw std::invalid_argument("Protocol type not recognised");
    }
}
} // namespace aleatoric
