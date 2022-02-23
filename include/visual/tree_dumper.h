#pragma once
#include "math_parser.h"


typedef struct IndexedMathNode
{
    unsigned node_index;
    MathNode* node;
} IndexedMathNode;


int get_node_amount(MathNode* tree_root);

/**
 *  Maps nodes from tree to node_list
 * @param[in] tree_root
 * @param[out] node_list
 * @param[in] list_capacity
 */
void collect_nodes_ex(MathNode* tree_root, MathNode* node_list, int list_capacity);

/**
 * allocates memory and map nodes from tree to contiguous array
 *
 * @param tree_root
 * @return mapped node list
 */
MathNode* collect_nodes(MathNode* tree_root);

IndexedMathNode* index_nodes(MathNode* node_list, int node_list_length);

unsigned int get_index_by_node(IndexedMathNode* indexed_node_list, size_t node_list_length, MathNode* node);