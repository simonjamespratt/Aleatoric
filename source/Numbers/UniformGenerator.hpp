#ifndef UniformGenerators_hpp
#define UniformGenerators_hpp

#include <random>

namespace actlib { namespace Numbers {
    class UniformGenerator
    {
    public:
        UniformGenerator(std::mt19937& engine, int rangeStart, int rangeEnd);
        ~UniformGenerator();
        int getNumber();
        void setDistribution(int rangeStart, int rangeEnd);

    private:
        std::mt19937 _engine;
        std::uniform_int_distribution<int> _distribution;
    };
}}

#endif /* UniformGenerators_hpp */
