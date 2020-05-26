// Interface
// TODO: RE-NAME: This is lazy naming. Consider how this interface and its
// abstraction/implementation naming could be better

#ifndef IUniformGenerator_hpp
#define IUniformGenerator_hpp

namespace aleatoric {
/*! @brief An interface abstract class from which the UniformGenerator class is
 * derived */
class IUniformGenerator {
  public:
    /*! @brief pure virtual method for returning generated numbers */
    virtual int getNumber() = 0;
    /*! @brief pure virtual method for setting the distribution for the uniform
     * generator */
    virtual void setDistribution(int rangeStart, int rangeEnd) = 0;
    virtual ~IUniformGenerator() = default;
};
} // namespace aleatoric

#endif /* IUniformGenerator_hpp */
