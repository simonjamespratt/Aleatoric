#ifndef ProtocolGranular_hpp
#define ProtocolGranular_hpp

namespace actlib { namespace Numbers { namespace Granular {

/*! @brief Interface to which concrete protocol classes that produce random
 * numbers must conform
 *
 * It is the _strategy_ interface which forms part of a
 * [Strategy](https://en.wikipedia.org/wiki/Strategy_pattern) design pattern
 *
 * The Producer class (_context_ in the strategy pattern) holds a reference to
 * concrete protocols via this interface.
 */
class Protocol {
  public:
    /*! Pure virtual method for getting random numbers from a protocol */
    virtual double getNumber() = 0;

    /*! Pure virtual method for resetting a protocol to its initial state */
    virtual void reset() = 0;

    virtual ~Protocol() = default;
};
}}} // namespace actlib::Numbers::Granular

#endif /* ProtocolGranular_hpp */
