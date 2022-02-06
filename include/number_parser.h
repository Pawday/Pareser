#pragma once

/**
 * @param[in] cstr
 * @param[out] parsed_length
 * @return
 */
unsigned int parse_int(const char* cstr, short* parsed_length);

double parse_double(const char* cstr, short* parsed_length);