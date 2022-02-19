#include <tokenizer.h>
#include <stdio.h>
#include <string.h>
#include <visual/token_converter.h>
#include <parser.h>



int main()
{
    const char* expression = "1 + 1 * 4 - ( 4 * 6.342 )";


    Token *pToken = tokenize_expression(expression);
    char buffer[50];

//#define ITERATE_TOKENS
#ifdef ITERATE_TOKENS
    Token t;
    while (1)
    {
        if (t.tokenType == TOK_END) break;
        t = *pToken;
        memset(buffer, 0xff, sizeof(buffer));
        token_to_string_ex(t, buffer, sizeof(buffer));
        fprintf(stdout,"%s\n",buffer);
        fflush(stdout);
        pToken++;
    }
    while (t.tokenType != TOK_END);
#endif
    Node* tree = create_tree(pToken);


    return 0;
}