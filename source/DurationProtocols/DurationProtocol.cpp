#include "DurationProtocol.hpp"

#include "Geometric.hpp"
#include "Multiples.hpp"
#include "Prescribed.hpp"
#include "UniformGenerator.hpp"

namespace aleatoric {
std::unique_ptr<DurationProtocol>
DurationProtocol::createPrescribed(std::vector<int> durations)
{
    return std::make_unique<Prescribed>(durations);
}

std::unique_ptr<DurationProtocol>
DurationProtocol::createMultiples(int baseIncrement, Range range)
{
    return std::make_unique<Multiples>(baseIncrement, range);
}

std::unique_ptr<DurationProtocol> DurationProtocol::createMultiples(
    int baseIncrement, Range range, double deviationFactor)
{
    return std::make_unique<Multiples>(baseIncrement,
                                       range,
                                       deviationFactor,
                                       std::make_unique<UniformGenerator>());
}

std::unique_ptr<DurationProtocol>
DurationProtocol::createMultiples(int baseIncrement,
                                  std::vector<int> multipliers)
{
    return std::make_unique<Multiples>(baseIncrement, multipliers);
}

std::unique_ptr<DurationProtocol> DurationProtocol::createMultiples(
    int baseIncrement, std::vector<int> multipliers, double deviationFactor)
{
    return std::make_unique<Multiples>(baseIncrement,
                                       multipliers,
                                       deviationFactor,
                                       std::make_unique<UniformGenerator>());
}

std::unique_ptr<DurationProtocol>
DurationProtocol::createGeometric(Range range, int collectionSize)
{
    return std::make_unique<Geometric>(range, collectionSize);
}
} // namespace aleatoric
