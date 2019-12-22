// Interface
// TODO: This is lazy naming. Consider how this interface and its
// abstraction/implementation naming could be better

#ifndef IUniformGenerator_hpp
#define IUniformGenerator_hpp

namespace actlib { namespace Numbers {
class IUniformGenerator {
  public:
    virtual int getNumber() = 0;
    virtual void setDistribution(int rangeStart, int rangeEnd) = 0;
    virtual ~IUniformGenerator() = default;
};
}} // namespace actlib::Numbers

#endif /* IUniformGenerator_hpp */
