#include <stddef.h>
#include <math_functions.h>
#include <number_parser.h>

unsigned int parse_int(const char* const cstr, short* const parsed_length)
{
    char is_end = 0;
    char last_char;
    short int_len_local = 0;

    while (!is_end)
    {
        last_char = cstr[int_len_local];
        switch (last_char)
        {
            case '0':case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
                int_len_local++;
                continue;
            default: is_end = 1;
        }
    }


    if (parsed_length != NULL) *parsed_length = int_len_local;

    unsigned int ret = 0;

    for (size_t i = 0; i < int_len_local; i++)
        ret += exp_ten_based((unsigned char) int_len_local - i - 1) * (cstr[i] - '0');



    return ret;
}

double parse_double(const char *cstr, short* parsed_length)
{
    unsigned int integer_part = 0;
    unsigned int decimal_part = 0;

    short integer_part_len;
    short decimal_part_len;

    integer_part = parse_int(cstr, &integer_part_len);
    decimal_part = parse_int(cstr + integer_part_len + 1, &decimal_part_len);

    if (parsed_length != NULL) *parsed_length = (signed short)(integer_part_len + decimal_part_len + 1);

    return integer_part + decimal_part / (double) exp_ten_based(decimal_part_len);
}