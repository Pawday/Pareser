#pragma once

#include <tokenizer.h>

typedef enum NodeType
{
    NODE_OPERATION,
    NODE_NEGATION,
    NODE_SUBTREE,
    NODE_NUMBER,
} NodeType;

typedef struct MathNode
{
    NodeType type;
    void* nodeContent;
} MathNode;

typedef struct NodeOperationContent
{
    OperationType operationType;
    MathNode* left;
    MathNode* right;
} NodeOperationContent;

typedef struct NodeNumberContent
{
    NumberContent* numberContent;
} NodeNumberContent;

typedef struct NodeSubtreeContent
{
    MathNode* subtreeRootNode;
} NodeSubtreeContent;

typedef struct NodeNegationContent
{
    MathNode* negatedTreeRootNode;
} NodeNegationContent;

MathNode* create_tree(Token* token_list);

MathNode* create_number_node(Token number_token);

void free_tree(MathNode* treeRoot);