//
//  NumberGenerators.cpp
//  AleatoricCompositionTools
//
//  Created by Simon Pratt on 29/10/2019.
//  Copyright © 2019 Simon Pratt. All rights reserved.
//

#include "NumberGenerators.hpp"

namespace actlib::NumberGenerators {
    
    namespace random_number
    {
        std::random_device rd;
        std::mt19937 engine(rd());
    } // namespace random_number
    
    RandomNumber::RandomNumber(int rangeStart, int rangeEnd) : dist(rangeStart, rangeEnd) {}
    
    RandomNumber::~RandomNumber() {}
    
    int RandomNumber::getNumber()
    {
        return dist(random_number::engine);
    }
    
    // RANDOM NUMBER SELECTOR ==============================================================================
    RandomNumberSelector::RandomNumberSelector(int rangeStart, int rangeEnd)
    {
        range.start = rangeStart;
        range.end = rangeEnd;
        offset = rangeStart;
        rangeSize = rangeEnd - rangeStart + 1;
    }
    
    RandomNumberSelector::~RandomNumberSelector() {}
    
    // DISCRETE DISTRIBUTION RANDOM NUMBER =================================================================
    DiscreteDistributionRandomNumber::DiscreteDistributionRandomNumber(int rangeStart, int rangeEnd) : RandomNumberSelector(rangeStart, rangeEnd) {}
    
    DiscreteDistributionRandomNumber::~DiscreteDistributionRandomNumber() {}
    
    void DiscreteDistributionRandomNumber::setUniformDistribution(double value)
    {
        discreteDist.clear();
        for (int i = 0; i < rangeSize; i++)
        {
            discreteDist.push_back(value);
        }
    }
    
    void DiscreteDistributionRandomNumber::setEqualProbability()
    {
        setUniformDistribution(1.0);
    }
    
    // RANDOM WITHIN RANGE =====================================================
    // range is inclusive, so range is: end - start + 1
    RandomNumberWithinRange::RandomNumberWithinRange(int rangeStart, int rangeEnd) : RandomNumberSelector(rangeStart, rangeEnd) {}
    
    RandomNumberWithinRange::~RandomNumberWithinRange() {}
    
    int RandomNumberWithinRange::getNumber()
    {
        // select a number in range - uniform dist
        RandomNumber randomNumber(range.start, range.end);
        return randomNumber.getNumber();
    }
    
    void RandomNumberWithinRange::reset() {}
    
    void RandomNumberWithinRange::initialize() {}
}
