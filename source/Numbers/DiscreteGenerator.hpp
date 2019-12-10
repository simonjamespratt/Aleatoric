#ifndef DiscreteGenerators_hpp
#define DiscreteGenerators_hpp

#include <vector>
#include <random>

namespace actlib { namespace Numbers {
    class DiscreteGenerator
    {
    public:
        DiscreteGenerator(std::mt19937& engine, std::vector<double> distribution);
        DiscreteGenerator(std::mt19937& engine, int vectorSize, double uniformValue);
        ~DiscreteGenerator();
        int getNumber();
        void setDistributionVector(std::vector<double> distributionVector);
        void setDistributionVector(int vectorSize, double uniformValue);
        void updateDistributionVector(int index, double newValue);
        void updateDistributionVector(double uniformValue);
        std::vector<double> getDistributionVector();

    private:
        std::mt19937 _engine;
        std::vector<double> _distributionVector;
        std::discrete_distribution<int> _distribution;
        void setDistribution();
    };
}}

#endif /* DiscreteGenerators_hpp */
