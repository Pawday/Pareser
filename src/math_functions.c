#include <math_functions.h>
unsigned int exp_ten_based(unsigned char exponent)
{
    if (exponent == 0) return 1;

    unsigned int ret = 10;
    for (int i = 1; i < exponent; i++) ret *= 10;

    return ret;
}