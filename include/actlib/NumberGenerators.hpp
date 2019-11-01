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



namespace actlib::NumberGenerators {
    
    struct RandomSelectionRange
    {
        int start;
        int end;
    };
    
    class RandomNumber
    {
    public:
        RandomNumber(int rangeStart, int rangeEnd);
        ~RandomNumber();
        
        int getNumber();
        
    private:
        std::uniform_int_distribution<int> dist;
    };
    
    // Abstract class for derived classes working with ranges, offsets and offering the ability to get a random number
    class RandomNumberSelector
    {
    public:
        RandomNumberSelector(int rangeStart, int rangeEnd);
        virtual ~RandomNumberSelector();
        
        // NB: removed as pure virtual method because cannot return different types when method has no params
        // virtual int getNumber() = 0;
        virtual void reset() = 0;
        
    protected:
        RandomSelectionRange range;
        int offset;
        int rangeSize;
        virtual void initialize() = 0;
        
    private:
    };
    
    // Abstract Base class for derived classes using a discrete distribution
    class DiscreteDistributionRandomNumber : public RandomNumberSelector
    {
    public:
        DiscreteDistributionRandomNumber(int rangeStart, int rangeEnd);
        virtual ~DiscreteDistributionRandomNumber();
        
    protected:
        std::vector<double> discreteDist;
        void setUniformDistribution(double value);
        void setEqualProbability();
        
    private:
    };
    
    // Between from RTC
    class RandomNumberWithinRange : RandomNumberSelector
    {
    public:
        RandomNumberWithinRange(int rangeStart, int rangeEnd);
        ~RandomNumberWithinRange();
        int getNumber();
        void reset() override;
        
    protected:
        void initialize() override;
        
    private:
    };
}

#endif /* NumberGenerators_hpp */
