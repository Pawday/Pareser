#include <tokenizer.h>
#include <stdio.h>
#include <visual/token_converter.h>
#include <vcruntime_string.h>


int main()
{
    const char* expression = "1 + 1 * 4 - ( 4 * 6.342 )";


    Token *pToken = tokenize_expression(expression);
    Token *pTokenCp = pToken;

    char buffer[400];


    Token t;
    while (1)
    {
        if (t.tokenType == TOK_END) break;
        t = *pTokenCp;
        memset(buffer, 0xff, sizeof(buffer));
        token_to_string_ex(t, buffer, sizeof(buffer));
        fprintf(stdout,"%s\n",buffer);
        fflush(stdout);
        pTokenCp++;
    }
    while (t.tokenType != TOK_END);

    return 0;
}