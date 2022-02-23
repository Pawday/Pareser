#include <math_parser.h>
#include "visual/tree_dumper.h"
#include "stddef.h"
#include "stdlib.h"
static void get_node_amount_recursive(MathNode* tree_root, int* amount)
{
    if (tree_root == NULL) return;
    switch (tree_root->type)
    {
        case NODE_OPERATION:
        {
            (*amount)++;
            NodeOperationContent* operation_node_content = (NodeOperationContent *) tree_root->nodeContent;
            
            MathNode* left_operand = operation_node_content->left;
            MathNode* right_operand = operation_node_content->right;
            get_node_amount_recursive(left_operand, amount);
            get_node_amount_recursive(right_operand, amount);
            return;
        }

        case NODE_SUBTREE: 
        {
            (*amount)++;
            NodeSubtreeContent* content = (NodeSubtreeContent*) tree_root->nodeContent;
            get_node_amount_recursive(content->subtreeRootNode, amount);
            return;
        }

        case NODE_NUMBER:
        {
            (*amount)++;
            return;
        }
        case NODE_NEGATION:
        {
            (*amount)++;
            NodeNegationContent* content = (NodeNegationContent*) tree_root->nodeContent;
            get_node_amount_recursive(content->negatedTreeRootNode, amount);
            return;
        }
    }
}

int get_node_amount(MathNode* tree_root)
{
    if (tree_root == NULL) return 0;
    int node_amount = 0;
    get_node_amount_recursive(tree_root, &node_amount);

    return node_amount;
}

static void collect_nodes_recursive(MathNode* tree_root, MathNode* node_list, int list_capacity, int* next_free_spot_in_list)
{
    if (tree_root == NULL) return;
    switch (tree_root->type)
    {
        case NODE_OPERATION:
        {
            node_list[*next_free_spot_in_list] = *tree_root; (*next_free_spot_in_list)++;
            NodeOperationContent* operation_content = (NodeOperationContent*) tree_root->nodeContent;
            collect_nodes_recursive(operation_content->left, node_list, list_capacity, next_free_spot_in_list);
            collect_nodes_recursive(operation_content->right, node_list, list_capacity, next_free_spot_in_list);
            return;
        }
        case NODE_NUMBER:
        {
            node_list[*next_free_spot_in_list] = *tree_root; (*next_free_spot_in_list)++;
            return;
        }
        case NODE_NEGATION:
        {
            node_list[*next_free_spot_in_list] = *tree_root; (*next_free_spot_in_list)++;
            NodeNegationContent* negation_content = (NodeNegationContent*) tree_root->nodeContent;
            collect_nodes_recursive(negation_content->negatedTreeRootNode, node_list, list_capacity, next_free_spot_in_list);
            return;
        }
        case NODE_SUBTREE:
        {
            node_list[*next_free_spot_in_list] = *tree_root; (*next_free_spot_in_list)++;
            NodeSubtreeContent* subtree_content = (NodeSubtreeContent*) tree_root->nodeContent;
            collect_nodes_recursive(subtree_content->subtreeRootNode, node_list, list_capacity, next_free_spot_in_list);
            return;
        }
    }
}

MathNode* collect_nodes(MathNode *tree_root)
{
    int node_am = get_node_amount(tree_root);
    
    MathNode* node_list = calloc(node_am, sizeof(MathNode));

    collect_nodes_ex(tree_root, node_list, node_am);

    return node_list;
}



void collect_nodes_ex(MathNode* tree_root, MathNode* node_list, int list_capacity)
{
    int collected_node_am = 0;

    collect_nodes_recursive(tree_root, node_list, list_capacity, &collected_node_am);
}

IndexedMathNode* index_nodes(MathNode *node_list, int node_list_length)
{
    IndexedMathNode* indexed_node_list = calloc(node_list_length, sizeof(IndexedMathNode));

    for (int i = 0; i < node_list_length; i++)
    {
        indexed_node_list[i].node = node_list + i;
        indexed_node_list[i].node_index = i;
    }
    return indexed_node_list;
}



unsigned int get_index_by_node(IndexedMathNode* indexed_node_list, size_t node_list_length, MathNode *node)
{
    for (size_t i = 0; i < node_list_length; i++)
    {
        IndexedMathNode current_indexed_node = indexed_node_list[i];

        // compare nodes by their contents (dont bite me pls)
        if (current_indexed_node.node->nodeContent == node->nodeContent) return current_indexed_node.node_index;
    }
    return -1;
}

