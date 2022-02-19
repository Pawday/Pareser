#include <parser.h>
#include <stddef.h>
#include <stdlib.h>
#include <assert.h>


static void free_operation_tree(Node* operationTreeRoot);
static void free_number_tree(Node* numberTreeRoot);
Node* create_operation_tree(OperationType operationType, Node* leftOperand,Node* rightOperand);
Node* parse_operand_node(Token* token_list, size_t token_offset, size_t* parsed_tokens_amount);

Node* create_tree_ex(Token* token_list, size_t token_offset, size_t* parsed_tokens_amount)
{
    Token* process_token_list = token_list + token_offset;

    Node* current_root = NULL;

    if (token_list == NULL) return NULL;

    size_t processed_token_index = 0;

    Token current_token;

    while (current_token.tokenType != TOK_END)
    {
        current_token = process_token_list[processed_token_index];

        switch (current_token.tokenType)
        {
            case TOK_NUMBER:
            {
                Node* number_node = create_number_node(current_token);

                current_root = number_node;
                processed_token_index++;
                continue;
            }

            case TOK_OPERATION:
            {
                //TODO: handle negation here
                //TODO: handle operation precedence


                OperationContent* operation_content = (OperationContent *) current_token.content;

                Node* left_operand = current_root;
                size_t right_operand_token_am;

                Node* right_operand = parse_operand_node(process_token_list, processed_token_index + 1, &right_operand_token_am);

                if (right_operand == NULL)
                {
                    //TODO: invalid right operand
                    assert(0);
                }

                Node* operation_node = create_operation_tree(operation_content->operation_type, left_operand, right_operand);

                current_root = operation_node;
                processed_token_index += right_operand_token_am;
                processed_token_index++;
                break;
            }

            case TOK_PARENTHESIS:
            {
                ParenthesisContent current_token_parenthesis_content = *(ParenthesisContent*) current_token.content;

                if (current_token_parenthesis_content == PARENTHESIS_OPEN)
                {
                    assert(current_root == NULL); //TODO: syntax error or implicit OP_MULTIPLICATION

                    Node* sub_tree_node = malloc(sizeof(Node));

                    sub_tree_node->type = NODE_SUBTREE;

                    NodeSubtreeContent* sub_tree_content = malloc(sizeof(NodeSubtreeContent));

                    size_t subtree_tokens_am;
                    sub_tree_content->subtreeRootNode = create_tree_ex(process_token_list, processed_token_index + 1, &subtree_tokens_am);

                    sub_tree_node->nodeContent = sub_tree_content;

                    current_root = sub_tree_node;
                    processed_token_index += subtree_tokens_am + 1;
                    continue;
                }

                if (current_token_parenthesis_content == PARENTHESIS_CLOSE)
                {
                    processed_token_index += 1; // PARENTHESIS_CLOSE token
                    *parsed_tokens_amount = processed_token_index;
                    return current_root;
                }

                break;
            }
        }
    }

    return current_root;
}

Node* parse_operand_node(Token* token_list, size_t token_offset, size_t* parsed_tokens_amount)
{
    Token* process_token_list = token_list + token_offset;

    switch (process_token_list[0].tokenType) {
        case TOK_NUMBER:
        {
            *parsed_tokens_amount = 1;
            return create_number_node(process_token_list[0]);
        }
        case TOK_PARENTHESIS:
        {
            Node* ret = create_tree_ex(process_token_list, 1, parsed_tokens_amount);
            *parsed_tokens_amount += 1;
            return ret;
        }
        case TOK_OPERATION: //  like "1 + + 1"
        case TOK_UNKNOWN:
        case TOK_END:
            return NULL; //syntax error
    }
}


Node* create_tree(Token* token_list)
{
    return create_tree_ex(token_list, 0, NULL);
}

Node* create_number_node(Token number_token)
{
    Node* number_node = malloc(sizeof(Node));

    number_node->type = NODE_NUMBER;
    number_node->nodeContent = malloc(sizeof(NodeNumberContent));

    ((NodeNumberContent*)(number_node->nodeContent))->numberContent = number_token.content;
    return number_node;
}

Node* create_operation_tree(OperationType operationType, Node* leftOperand, Node* rightOperand)
{
    Node* ret = malloc(sizeof(Node));

    ret->type = NODE_OPERATION;

    NodeOperationContent* nodeOperationContent = malloc(sizeof(NodeNumberContent));

    nodeOperationContent->operationType = operationType;

    nodeOperationContent->left = leftOperand;
    nodeOperationContent->right = rightOperand;

    ret->nodeContent = nodeOperationContent;

    return ret;
}

void free_tree(Node* treeRoot)
{
    switch (treeRoot->type)
    {
        case NODE_OPERATION:
            free_operation_tree(treeRoot);
            //free(treeRoot) -> not needed
            return;
        case NODE_NUMBER:
            free_number_tree(treeRoot);
            //free(treeRoot) -> not needed
            break;
        case NODE_NEGATION:
        {
            Node* subtree = ((NodeNegationContent*) treeRoot->nodeContent)->negatedTreeRootNode;
            free_tree(subtree);
            free(treeRoot);
            return;
        }
        case NODE_SUBTREE:
        {
            Node* subtree = ((NodeSubtreeContent*) treeRoot->nodeContent)->subtreeRootNode;
            free_tree(subtree);
            free(treeRoot);
            return;
        }
    }
}

void free_number_tree(Node* numberTreeRoot)
{
    assert(numberTreeRoot->type == NODE_NUMBER);

    NodeNumberContent* numberContent = numberTreeRoot->nodeContent;

    free(numberContent->numberContent->number_data);
    free(numberContent->numberContent);
    free(numberTreeRoot);
}

void free_operation_tree(Node* operationTreeRoot)
{
    assert(operationTreeRoot->type == NODE_OPERATION);

    NodeOperationContent* operationContent = (NodeOperationContent*) operationTreeRoot->nodeContent;

    free_tree(operationContent->left);
    free_tree(operationContent->right);

    free(operationContent);

    free(operationTreeRoot);
}
