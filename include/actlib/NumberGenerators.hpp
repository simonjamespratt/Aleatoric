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
    
    // Base class from which all other classes inherit
    class NumberGenerator
    {
    public:
        NumberGenerator(int rangeStart, int rangeEnd);
        virtual ~NumberGenerator();
        // virtual void reset();
        
    protected:
        // virtual void initialise();
        SelectionRange range;
        
    private:
    };
    
    // Class for getting random numbers according to a uniform distribution
    class UniformNumberGenerator: public NumberGenerator
    {
    public:
        UniformNumberGenerator(int rangeStart, int rangeEnd);
        ~UniformNumberGenerator();
        virtual int getNumber();
    private:
        std::uniform_int_distribution<int> uniformDistribution;
    };
    
    // Class for getting random numbers according to a discrete distribution
    class DiscreteNumberGenerator
    {
    public:
        DiscreteNumberGenerator(int rangeStart, int rangeEnd);
        ~DiscreteNumberGenerator();
        virtual int getNumber();
    private:
        std::vector<double> distributionArray; // rename this
        std::discrete_distribution<int> discreteDistribution;
        // void setUniformDistribution(double value);
        // void setEqualProbability();
    };
} // namespace NumberGenerators
} // namespace actlib
#endif /* NumberGenerators_hpp */
