#pragma once

#include <tokenizer.h>

typedef enum NodeType
{
    NODE_OPERATION,
    NODE_NEGATION,
    NODE_SUBTREE,
    NODE_NUMBER,
} NodeType;

typedef struct Node
{
    NodeType type;
    void* nodeContent;
} Node;

typedef struct NodeOperationContent
{
    OperationType operationType;
    Node* left;
    Node* right;
} NodeOperationContent;

typedef struct NodeNumberContent
{
    NumberContent* numberContent;
} NodeNumberContent;

typedef struct NodeSubtreeContent
{
    Node* subtreeRootNode;
} NodeSubtreeContent;

typedef struct NodeNegationContent
{
    Node* negatedTreeRootNode;
} NodeNegationContent;

Node* create_tree(Token* token_list);

Node* create_number_node(Token number_token);