#pragma once

typedef enum TokenType
{
    TOK_OPERATION,
    TOK_NUMBER,
    TOK_PARENTHESIS,
    TOK_END
} TokenType;

typedef struct Token
{
    TokenType tokenType;
    void* content;
} Token;

typedef enum OperationType
{
    OP_ADDITION,
    OP_SUBTRACTION,
    OP_MULTIPLICATION,
    OP_DIVISION
} OperationType;

typedef enum ParenthesisContent
{
    PARENTHESIS_OPEN,
    PARENTHESIS_CLOSE
} ParenthesisContent;

typedef enum NumberType
{
    NUM_INTEGER,
    NUM_RATIONAL
} NumberType;

typedef struct NumberContent
{
    NumberType number_type;
    void* number_data;
} NumberContent;

typedef struct OperationContent
{
    OperationType operation_type;
} OperationContent;

Token* tokenize_expression(const char* expression_cstr);
TokenType get_token_type(char first_char);

/**
 *
 * @param[in]  expression_cstr
 * @param[in]  offset
 * @param[out] token
 * @param[out] token_string_len can be NULL
 */
void get_token_at
(
        const char* expression_cstr,
        size_t offset,
        Token* token,
        short* token_string_len
);

void token_content_set_operation(OperationContent* content, char operation_char);
/**
 *
 * @param[out] content
 * @param[in]  number_cstr
 * @param[out] number_str_length amount of chars represents it number (floating point included)
 */
void token_content_set_number(NumberContent* content, const char* number_cstr, short* number_str_length);

NumberType detect_number_type(const char* number_cstr);

char* discard_whitespaces(const char* expr);