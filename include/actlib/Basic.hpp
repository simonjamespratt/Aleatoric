#ifndef Basic_hpp
#define Basic_hpp

#include "IUniformGenerator.hpp"
#include "ProtocolSteps.hpp"
#include "Range.hpp"

namespace actlib { namespace Numbers { namespace Steps {
/*!
    @brief A protocol for producing a basic random number

    A concrete implementation of the Protocol interface which forms part of a
   [Strategy](https://en.wikipedia.org/wiki/Strategy_pattern) design pattern
   (see Protocol for more information).

    No constraints are placed on the production of numbers. For example,
   repetition of previously selected numbers is allowed.
 */
class Basic : public Protocol {
  public:
    /*!
        @brief Takes a UniformGenerator derived from the IUniformGenerator and a
        Range

        @param generator Should be an instance of UniformGenerator. Default
       construction is fine.

        @param range The range within which to produce numbers.
    */
    Basic(IUniformGenerator &generator, Range &range);
    ~Basic();
    /*! @brief returns a random number */
    int getNumber() override;
    /*! @brief resets the instance to initial settings */
    void reset() override;

  private:
    Range &m_range;
    IUniformGenerator &m_generator;
};
}}} // namespace actlib::Numbers::Steps

#endif /* Basic_hpp */
