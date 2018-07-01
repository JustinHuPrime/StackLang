#ifndef DIGITARRAY_H
#define DIGITARRAY_H

#include <vector>

namespace Util
{
namespace Math
{
using std::vector;

/**
 * An array of decmial digits, one nybble per digit
 */
class DigitArray
{
  public:
    DigitArray ();

  private:
    vector< unsigned char > digits;
}; // namespace Math// namespace Math
} // namespace Math
} // namespace Util

#endif /* DIGITARRAY_H */