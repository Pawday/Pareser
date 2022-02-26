#include <math_parser.h>
#include <stddef.h>
#include <stdlib.h>
#include <assert.h>


static void free_operation_tree(MathNode* operationTreeRoot);
static void free_number_tree(MathNode* numberTreeRoot);
MathNode* create_operation_tree(OperationType operationType, MathNode* leftOperand, MathNode* rightOperand);
MathNode* parse_operand_node(Token* token_list, size_t token_offset, size_t* parsed_tokens_amount);

MathNode* create_tree_ex(Token* token_list, size_t token_offset, size_t* parsed_tokens_amount)
{
    Token* process_token_list = token_list + token_offset;

    MathNode* current_root = NULL;

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
                MathNode* number_node = create_number_node(current_token);

                current_root = number_node;
                processed_token_index++;
                continue;
            }

            case TOK_OPERATION:
            {
                //TODO: handle operation precedence


                OperationContent* operation_content = (OperationContent *) current_token.content;

                if (operation_content->operation_type == OP_SUBTRACTION && current_root == NULL)
                {
                    MathNode* negation_node = malloc(sizeof(MathNode));
                    negation_node->type = NODE_NEGATION;

                    NodeNegationContent* negation_content = malloc(sizeof(NodeNegationContent));
                    negation_node->nodeContent = negation_content;

                    size_t negated_operand_token_am;

                    negation_content->negatedTreeRootNode = parse_operand_node(process_token_list, processed_token_index + 1, &negated_operand_token_am);

                    processed_token_index += negated_operand_token_am;
                    processed_token_index++;

                    current_root = negation_node;
                    break;
                }

                MathNode* left_operand = current_root;
                size_t right_operand_token_am;

                MathNode* right_operand = parse_operand_node(process_token_list, processed_token_index + 1, &right_operand_token_am);

                if (right_operand == NULL)
                {
                    //TODO: invalid right operand
                    assert(0);
                }

                MathNode* operation_node = create_operation_tree(operation_content->operation_type, left_operand, right_operand);

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

                    MathNode* sub_tree_node = malloc(sizeof(MathNode));

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

MathNode* parse_operand_node(Token* token_list, size_t token_offset, size_t* parsed_tokens_amount)
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
            MathNode* ret = malloc(sizeof(MathNode));
            ret->type = NODE_SUBTREE;
            MathNode* subtree_root = create_tree_ex(process_token_list, 1, parsed_tokens_amount);
            ret->nodeContent = malloc(sizeof(NodeSubtreeContent));
            ((NodeSubtreeContent*) ret->nodeContent)->subtreeRootNode = subtree_root;
            *parsed_tokens_amount += 1;
            return ret;
        }
        case TOK_OPERATION:
        {
            // handle negation
            OperationContent* op_content = (OperationContent*) process_token_list[0].content;

            if (op_content->operation_type == OP_SUBTRACTION)
            {
                MathNode* negation_node = malloc(sizeof(MathNode));
                negation_node->type = NODE_NEGATION;

                NodeNegationContent* negation_content = malloc(sizeof(NodeNegationContent));
                negation_content->negatedTreeRootNode = parse_operand_node(token_list, token_offset + 1, parsed_tokens_amount);;

                negation_node->nodeContent = negation_content;

                (*parsed_tokens_amount)++;

                return negation_node;
            }
        }


        case TOK_UNKNOWN:
        case TOK_END:
            return NULL; //syntax error
    }
}


MathNode* create_tree(Token* token_list)
{
    return create_tree_ex(token_list, 0, NULL);
}

MathNode* create_number_node(Token number_token)
{
    MathNode* number_node = malloc(sizeof(MathNode));

    number_node->type = NODE_NUMBER;
    number_node->nodeContent = malloc(sizeof(NodeNumberContent));

    ((NodeNumberContent*)(number_node->nodeContent))->numberContent = number_token.content;
    return number_node;
}

MathNode* create_operation_tree(OperationType operationType, MathNode* leftOperand, MathNode* rightOperand)
{
    MathNode* ret = malloc(sizeof(MathNode));

    ret->type = NODE_OPERATION;

    NodeOperationContent* nodeOperationContent = malloc(sizeof(NodeOperationContent));

    nodeOperationContent->operationType = operationType;

    nodeOperationContent->left = leftOperand;
    nodeOperationContent->right = rightOperand;

    ret->nodeContent = nodeOperationContent;

    return ret;
}

void free_tree(MathNode* treeRoot)
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
            MathNode* subtree = ((NodeNegationContent*) treeRoot->nodeContent)->negatedTreeRootNode;
            free_tree(subtree);
            free(treeRoot);
            return;
        }
        case NODE_SUBTREE:
        {
            MathNode* subtree = ((NodeSubtreeContent*) treeRoot->nodeContent)->subtreeRootNode;
            free_tree(subtree);
            free(treeRoot);
            return;
        }
    }
}

void free_number_tree(MathNode* numberTreeRoot)
{
    assert(numberTreeRoot->type == NODE_NUMBER);

    NodeNumberContent* numberContent = numberTreeRoot->nodeContent;

    free(numberContent->numberContent->number_data);
    free(numberContent->numberContent);
    free(numberTreeRoot);
}

void free_operation_tree(MathNode* operationTreeRoot)
{
    assert(operationTreeRoot->type == NODE_OPERATION);

    NodeOperationContent* operationContent = (NodeOperationContent*) operationTreeRoot->nodeContent;

    free_tree(operationContent->left);
    free_tree(operationContent->right);

    free(operationContent);

    free(operationTreeRoot);
}