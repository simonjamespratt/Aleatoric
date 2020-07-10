#ifndef Basic_hpp
#define Basic_hpp

#include "IUniformGenerator.hpp"
#include "NumberProtocol.hpp"
#include "Range.hpp"

#include <memory>

namespace aleatoric {
/*!
    @brief A protocol for producing a basic random number

    A concrete implementation of the Protocol interface which forms part of a
   [Strategy](https://en.wikipedia.org/wiki/Strategy_pattern) design pattern
   (see Protocol for more information).

    No constraints are placed on the production of numbers. For example,
   repetition of previously selected numbers is allowed.
 */
class Basic : public NumberProtocol {
  public:
    /*!
        @brief Takes a UniformGenerator derived from the IUniformGenerator and a
        Range

        @param generator Should be an instance of UniformGenerator. Default
       construction is fine.

        @param range The range within which to produce numbers.
    */
    Basic(std::unique_ptr<IUniformGenerator> generator,
          std::unique_ptr<Range> range);
    ~Basic();
    /*! @brief returns a random number */
    int getIntegerNumber() override;

    double getDecimalNumber() override;

    /*! @brief resets the instance to initial settings */
    void reset() override;

  private:
    std::unique_ptr<Range> m_range;
    std::unique_ptr<IUniformGenerator> m_generator;
};
} // namespace aleatoric

#endif /* Basic_hpp */
