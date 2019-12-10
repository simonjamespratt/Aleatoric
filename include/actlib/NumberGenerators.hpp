//
//  NumberGenerators.hpp
//  AleatoricCompositionTools
//
//  Created by Simon Pratt on 29/10/2019.
//  Copyright © 2019 Simon Pratt. All rights reserved.
//

#ifndef NumberGenerators_hpp
#define NumberGenerators_hpp

#include <vector>
#include "DiscreteGenerator.hpp"

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

    // Series ===================================================
    // xRandom from RTC
    class Serial : public NumberGeneratorInterface
    {
    public:
        Serial(std::mt19937& engine, int rangeStart, int rangeEnd);
        ~Serial();
        int getNumber();
    private:
        actlib::Numbers::DiscreteGenerator generator;
        bool seriesIsComplete();
    };
} // namespace NumberGenerators
} // namespace actlib
#endif /* NumberGenerators_hpp */
