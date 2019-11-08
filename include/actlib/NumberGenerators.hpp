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



namespace actlib { namespace NumberGenerators
    
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
        
        virtual int getNumber();
        virtual int getUniformNumber();
        virtual int getDiscreteNumber();
        
        virtual void reset();
        
    protected:
        virtual void initialise();
        SelectionRange range;
        
    private:
        //uniform
        std::uniform_int_distribution<int> uniformDistribution;
        
        // discrete
        std::discrete_distribution<int> discreteDistribution;
        std::vector<double> distributionArray; // rename this
        void setUniformDistribution(double value);
        void setEqualProbability();
    }
}
    

    
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
        RandomNumber randomNumber;
        
    private:
    };


#endif /* NumberGenerators_hpp */
