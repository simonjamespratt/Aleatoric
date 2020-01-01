#include "AdjacentSteps.hpp"

// TODO: Think about whether there is a need for an internal and external range.
// This class brings it into focus. See getNumber() and the addition or
// subtraction of range.offset depending on whether we're talking about the
// number within the context of the range of in the context of the generator's
// vector and the numbers the generator returns. Not sure what the right answer
// is though!

namespace actlib { namespace Numbers {
AdjacentSteps::AdjacentSteps(IDiscreteGenerator &generator, Range &range) :
        _range(range), _generator(generator) {
    _haveInitialSelection = false;
    _haveRequestedFirstNumber = false;
}
AdjacentSteps::AdjacentSteps(IDiscreteGenerator &generator,
                             Range &range,
                             int initialSelection) :
        AdjacentSteps(generator, range) {
    // TODO: check that the supplied initialSelection is WITHIN the range. Throw
    // an exception if it is not.
    _initialSelection = initialSelection;
    _haveInitialSelection = true;
}

AdjacentSteps::~AdjacentSteps() {
}

int AdjacentSteps::getNumber() {
    if(_haveInitialSelection && !_haveRequestedFirstNumber) {
        // step configuring in preparation for next call
        prepareStepBasedDistribution(_initialSelection,
                                     _initialSelection - _range.offset);

        // this only needs to be in here for now as it is only used in the above
        // if statement
        _haveRequestedFirstNumber = true;

        return _initialSelection;
    }

    auto generatedNumber = _generator.getNumber();
    auto numberPlacedWithinRange = generatedNumber + _range.offset;

    // step configuring in preparation for next call
    prepareStepBasedDistribution(numberPlacedWithinRange, generatedNumber);

    return numberPlacedWithinRange;
}

void AdjacentSteps::reset() {
    _generator.updateDistributionVector(1.0);
    _haveRequestedFirstNumber = false;
}

void AdjacentSteps::prepareStepBasedDistribution(int number, int vectorIndex) {
    _generator.updateDistributionVector(0.0);

    if(number == _range.start) {
        _generator.updateDistributionVector(vectorIndex + 1, 1.0);

    } else if(number == _range.end) {
        _generator.updateDistributionVector(vectorIndex - 1, 1.0);

    } else {
        _generator.updateDistributionVector(vectorIndex + 1, 1.0);
        _generator.updateDistributionVector(vectorIndex - 1, 1.0);
    }
}
}} // namespace actlib::Numbers
