#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "tokenizer.h"
#include "number_parser.h"

char* discard_whitespaces(const char* const expr)
{
    size_t len = strlen(expr) + 1;
    char* ret = malloc(len);
    memset(ret,0,len);

    size_t res_length = 0;
    for (int i = 0; i < len; i++)
    {
        switch (expr[i])
        {
            case ' ':
            case '\n':
            case '\t':
            break;
            default: ret[res_length] = expr[i]; res_length++;
        }
    }

    char* exp_with_zero_tail = ret;
    size_t exp_cleaned_len = res_length + 1;
    char* exp_cleaned = malloc(exp_cleaned_len);
    memset(exp_cleaned, 0, exp_cleaned_len);

    strcpy_s(exp_cleaned, exp_cleaned_len, exp_with_zero_tail);

    free(exp_with_zero_tail);

    ret = exp_cleaned;

    return ret;

}

/**
 *
 * @param expression_cstr_cleaned - expression string without white spaces
 * @return amount of tokens in expression_cstr_cleaned
 */

size_t count_tokens_in_expr(const char* const expression_cstr_cleaned)
{
    size_t tokens_amount = -1;
    size_t parse_offset = 0;

    Token t;
    short last_token_len;
    do
    {
        get_token_at(expression_cstr_cleaned,parse_offset,&t,&last_token_len);
        parse_offset += last_token_len;
        free_token_content(t);
        tokens_amount++;
    }
    while (t.tokenType != TOK_END);

    return tokens_amount;
}

Token* tokenize_expression(const char* const expression_cstr)
{
    char* expr_without_whites = discard_whitespaces(expression_cstr);


    size_t tokens_amount = count_tokens_in_expr(expr_without_whites);

    Token* tokens = malloc(sizeof(Token) * (tokens_amount + 1));

    size_t parse_offset = 0;
    short last_token_len = 0;

    Token t;

    for (int i = 0; i < tokens_amount; i++)
    {
        get_token_at(expr_without_whites, parse_offset, &t, &last_token_len);
        parse_offset += last_token_len;
        tokens[i] = t;
    }

    tokens[tokens_amount].tokenType = TOK_END;
    tokens[tokens_amount].content = NULL;

    return tokens;
}

TokenType get_token_type(char first_char)
{
    switch (first_char)
    {
        case '0':case '1':case '2':case '3':case '4':case '5':case '6':case '7':case '8':case '9':
            return TOK_NUMBER;
        case '+':
        case '-':
        case '*':
        case '/':
            return TOK_OPERATION;
        case '(':
        case ')':
            return TOK_PARENTHESIS;
        case 0:
            return TOK_END;

        default: return TOK_UNKNOWN; //syntax_error
    }
}

void get_token_at
(
    const char* expression_cstr,
    size_t offset,
    Token* token,
    short* token_string_len
)
{
    Token ret;
    const char* expr_part = expression_cstr + offset;

    ret.tokenType = get_token_type(expr_part[0]);

    switch (ret.tokenType)
    {
        case TOK_OPERATION:
            ret.content = malloc(sizeof(OperationContent));
            token_content_set_operation(ret.content, expr_part[0]);
            if (token_string_len != NULL) *token_string_len = 1;
            break;
        case TOK_NUMBER:
            ret.content = malloc(sizeof(NumberContent));
            token_content_set_number(ret.content,expr_part,token_string_len);
            break;
        case TOK_PARENTHESIS:
            ret.content = malloc(sizeof(ParenthesisContent));
            switch (expr_part[0])
            {
                case '(': *((ParenthesisContent*) ret.content) = PARENTHESIS_OPEN; break;
                case ')': *((ParenthesisContent*) ret.content) = PARENTHESIS_CLOSE; break;
            }
            if (token_string_len != NULL) *token_string_len = 1;
            break;
        case TOK_END:
            break;
        case TOK_UNKNOWN:
            if (token_string_len != NULL) *token_string_len = 1;
            break;
    }

    *token = ret;
}
void token_content_set_operation(OperationContent* content, char operation_char)
{
    switch (operation_char)
    {
        case '+': content->operation_type = OP_ADDITION; return;
        case '-': content->operation_type = OP_SUBTRACTION; return;
        case '*': content->operation_type = OP_MULTIPLICATION; return;
        case '/': content->operation_type = OP_DIVISION; return;

        default: assert(0); // should be unreachable
    }
}

void token_content_set_number(NumberContent* content, const char* number_cstr, short* number_str_length)
{
    content->number_type = detect_number_type(number_cstr);

    switch (content->number_type)
    {
        case NUM_INTEGER:
            content->number_data = malloc(sizeof(int));
            * (unsigned int*) content->number_data = parse_int(number_cstr, number_str_length);
            break;
        case NUM_RATIONAL:
            content->number_data = malloc(sizeof(double));
            * (double*) content->number_data = parse_double(number_cstr,number_str_length);
            break;
    }
}

NumberType detect_number_type(const char *number_cstr)
{
    short digits_amount = 0;
    char last_char;
    char is_end = 0;

    while (!is_end)
    {
        last_char = number_cstr[digits_amount];
        switch (last_char)
        {
            case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
                digits_amount++;
                continue;
            default: is_end = 1;
        }
    }

    const char* str_after_skip_digits = number_cstr + digits_amount;

    if (*str_after_skip_digits == '.' || *str_after_skip_digits == ',')
        return NUM_RATIONAL;
    else
        return NUM_INTEGER;
}

void free_token_content(Token token)
{
    switch (token.tokenType)
    {
        case TOK_NUMBER:
        {
            NumberContent *numberContent = (NumberContent *) token.content;
            free(numberContent->number_data);
            free(numberContent);
            break;
        }
        case TOK_PARENTHESIS:
        case TOK_OPERATION:
            free(token.content);
        case TOK_END:
        case TOK_UNKNOWN:
            break;
    }
}
