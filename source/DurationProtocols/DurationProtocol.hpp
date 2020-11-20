#ifndef DurationProtocol_hpp
#define DurationProtocol_hpp

#include "Range.hpp"

#include <memory>
#include <vector>

namespace aleatoric {

class DurationProtocol {
  public:
    virtual int getCollectionSize() = 0;

    virtual int getDuration(int index) = 0;

    virtual ~DurationProtocol() = default;

    static std::unique_ptr<DurationProtocol>
    createPrescribed(const std::vector<int> &durations);

    static std::unique_ptr<DurationProtocol>
    createMultiples(int baseIncrement, const Range &range);

    static std::unique_ptr<DurationProtocol> createMultiples(
        int baseIncrement, const Range &range, double deviationFactor);

    static std::unique_ptr<DurationProtocol>
    createMultiples(int baseIncrement, const std::vector<int> &multipliers);

    static std::unique_ptr<DurationProtocol>
    createMultiples(int baseIncrement,
                    const std::vector<int> &multipliers,
                    double deviationFactor);

    static std::unique_ptr<DurationProtocol>
    createGeometric(const Range &range, int collectionSize);
};
} // namespace aleatoric

#endif /* DurationProtocol_hpp */
