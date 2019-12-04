#ifndef DiscreteGenerators_hpp
#define DiscreteGenerators_hpp

#include <vector>
#include <random>

namespace actlib { namespace Numbers {
    class DiscreteGenerator
    {
    public:
        DiscreteGenerator(std::vector<double> distribution);
        ~DiscreteGenerator() {};
        int getNumber();
        void setDistribution(std::vector<double> distribution);

        // TODO: add the static methods for distribution manipulation here

    private:
        std::discrete_distribution<int> discreteDistribution;
    };
}}

#endif /* DiscreteGenerators_hpp */
