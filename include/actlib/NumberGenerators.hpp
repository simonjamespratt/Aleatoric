//
//  NumberGenerators.hpp
//  AleatoricCompositionTools
//
//  Created by Simon Pratt on 29/10/2019.
//  Copyright © 2019 Simon Pratt. All rights reserved.
//

#ifndef NumberGenerators_hpp
#define NumberGenerators_hpp

#include <random>

namespace actlib { namespace NumberGenerators {

    struct SelectionRange
    {
        int start;
        int end;
        int offset;
        int size;
    };

    // Number Generator ===========================================
    // Base class from which interface classes can inherit
    class NumberGeneratorInterface
    {
    public:
        NumberGeneratorInterface(int rangeStart, int rangeEnd);
        virtual ~NumberGeneratorInterface();
        // virtual void reset();

    protected:
        // virtual void initialise();
        SelectionRange range;

    private:
    };

    // UniformNumberGenerator ====================================
    // Class for getting random numbers according to a uniform distribution
    class UniformNumberGenerator
    {
    public:
        UniformNumberGenerator(int rangeStart, int rangeEnd);
        ~UniformNumberGenerator();
        virtual int getNumber(); // virtual for mocking in tests
    private:
        std::uniform_int_distribution<int> uniformDistribution;
    };

    // DiscreteNumberGenerator ===================================
    // Class for getting random numbers according to a discrete distribution
    class DiscreteNumberGenerator
    {
    public:
        DiscreteNumberGenerator();
        DiscreteNumberGenerator(std::vector<double> distribution);
        ~DiscreteNumberGenerator();
        virtual int getNumber(); // virtual for mocking in tests
        void updateDistribution(std::vector<double> distribution);
    private:
        std::discrete_distribution<int> discreteDistribution;
        // void setUniformDistribution(double value);
        // void setEqualProbability();
    };

    // Series ===================================================
    // xRandom from RTC
    class Serial : public NumberGeneratorInterface
    {
    public:
        Serial(int rangeStart, int rangeEnd);
        ~Serial();
        int getNumber();
    private:
        std::vector<double> distributionArray;
        DiscreteNumberGenerator generator;
        void setEqualProbability();
        bool seriesIsComplete();
    };
} // namespace NumberGenerators
} // namespace actlib
#endif /* NumberGenerators_hpp */
