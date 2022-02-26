#include <tokenizer.h>
#include <stdio.h>
#include <memory.h>
#include <math_parser.h>
#include <visual/tree_dumper.h>
#include <visual/node_dumper.h>


int main()
{
    const char* expression = "1 + 1";


    Token* token_list = tokenize_expression(expression);
    char buffer[8];

    memset(buffer, 0, sizeof(buffer));

//#define ITERATE_TOKENS
#ifdef ITERATE_TOKENS
    Token t;
    Token* iterable_tokenp = token_list;
    while (1)
    {
        if (t.tokenType == TOK_END) break;
        t = *iterable_tokenp;
        memset(buffer, 0xff, sizeof(buffer));
        token_to_string_ex(t, buffer, sizeof(buffer));
        fprintf(stdout,"%s\n",buffer);
        fflush(stdout);
        iterable_tokenp++;
    }
    while (t.tokenType != TOK_END);
#endif
    MathNode* tree = create_tree(token_list);

    int am = get_node_amount(tree);

    MathNode* node_list = collect_nodes(tree);

    IndexedMathNode* index_node_list = index_nodes(node_list, am);

    printf("digraph {\n");

    for (int i = 0; i < am; i++)
    {
        IndexedMathNode n = index_node_list[i];
        node_to_string_ex(*n.node, buffer, sizeof(buffer));
        printf("node_%d [label = \"%s\"]; \n", i, buffer);
    }

    printf("\n");

    for (int i = 0; i < am; i++)
    {
        IndexedMathNode n = index_node_list[i];

        switch (n.node->type)
        {
            case NODE_SUBTREE:
            {
                unsigned int subtree_node_index = get_index_by_node(index_node_list, am, ((NodeSubtreeContent*)n.node->nodeContent)->subtreeRootNode);
                printf("node_%d -> node_%d \n", i, subtree_node_index);
                break;
            }

            case NODE_NEGATION:
            {
                unsigned int negated_subtree_node_index = get_index_by_node(index_node_list, am, ((NodeNegationContent *)n.node->nodeContent)->negatedTreeRootNode);
                printf("node_%d -> node_%d \n", i, negated_subtree_node_index);
                break;
            }

            case NODE_OPERATION:
            {
                NodeOperationContent* operation_content = (NodeOperationContent *) n.node->nodeContent;

                MathNode* left_operand = operation_content->left;
                MathNode* right_operand = operation_content->right;

                unsigned int left_node_index = get_index_by_node(index_node_list, am, left_operand);
                unsigned int right_node_index = get_index_by_node(index_node_list, am, right_operand);

                printf("node_%d -> node_%d \n", i, left_node_index);
                printf("node_%d -> node_%d \n", i, right_node_index);
            }
        }
    }

    printf("}\n");

    free_tree(tree);

    return 0;
}