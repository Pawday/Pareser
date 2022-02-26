#include "visual/node_dumper.h"
#include "visual/token_dumper.h"
#include <string.h>

void node_to_string_ex(MathNode node, char* out_str, size_t str_capacity)
{
    switch (node.type)
    {
        case NODE_OPERATION:
            NodeOperationContent* operationContent = (NodeOperationContent*) node.nodeContent;
            if (str_capacity < 2) return;

            switch (operationContent->operationType)
            {
                case OP_ADDITION:       out_str[0] = '+'; out_str[1] = 0; return;
                case OP_DIVISION:       out_str[0] = '/'; out_str[1] = 0; return;
                case OP_MULTIPLICATION: out_str[0] = '*'; out_str[1] = 0; return;
                case OP_SUBTRACTION:    out_str[0] = '-'; out_str[1] = 0; return;
            }
        case NODE_NEGATION:
        {
            if (str_capacity < 2) return;
            out_str[0] = '-';
            out_str[1] = 0;
            return;
        }
        case NODE_SUBTREE:
        {
            if (str_capacity <= 3)
            {
                memset(out_str, 0, str_capacity);
                return;
            }
            memcpy_s(out_str, str_capacity - 1, "SUB", 3);
            out_str[3] = 0;
            return;
        }
        case NODE_NUMBER:
        {
            number_to_string(*((NodeNumberContent*) node.nodeContent)->numberContent, out_str, str_capacity);
            return;
        }
    }
}
