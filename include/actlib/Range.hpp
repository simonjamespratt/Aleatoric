#ifndef Range_hpp
#define Range_hpp

namespace actlib { namespace Numbers {
struct Range {
    Range(int rangeStart, int rangeEnd);
    int start;
    int end;
    int offset;
    int size;
};
}} // namespace actlib::Numbers

#endif /* Range_hpp */
