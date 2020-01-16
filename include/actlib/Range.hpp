#ifndef Range_hpp
#define Range_hpp

namespace actlib { namespace Numbers { namespace Steps {
/*! @brief Describes the properties of a defined range within which to create
 * numbers */
struct Range {
    /*! @brief Specify the start and end of the range to be instantiated
     *
     * @param rangeStart start of the range
     * @param rangeEnd end of the range
     */
    Range(int rangeStart, int rangeEnd);

    /*! @brief range start */
    int start;

    /*! @brief range end */
    int end;

    /*! @brief range offset, essentially the range start */
    int offset;

    /*! @brief range size. This is inclusive of the start and end */
    int size;
};
}}} // namespace actlib::Numbers

#endif /* Range_hpp */
