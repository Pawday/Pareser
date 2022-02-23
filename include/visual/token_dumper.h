#pragma once

#include "tokenizer.h"

const char* token_to_string(Token t);

void token_to_string_ex(Token t, char* buffer, size_t buffer_size);

/**
 *
 * @param content
 * @param buffer
 * @param buffer_size
 * @return amount of printed chars
 */

size_t number_to_string(NumberContent content, char* buffer, size_t buffer_size);