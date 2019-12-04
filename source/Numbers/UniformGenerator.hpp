#ifndef UniformGenerators_hpp
#define UniformGenerators_hpp

#include <random>

namespace actlib { namespace Numbers {
    class UniformGenerator
    {
    public:
        UniformGenerator(int rangeStart, int rangeEnd);
        ~UniformGenerator() {};
        int getNumber();
        void setDistribution(int rangeStart, int rangeEnd);

    private:
        std::uniform_int_distribution<int> uniformDistribution;
    };
}}

#endif /* UniformGenerators_hpp */
