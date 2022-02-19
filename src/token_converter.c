#include <visual/token_converter.h>
#include <string.h>

static const char* token_type_map[] =
{
    [TOK_OPERATION]      = "NODE_OPERATION",
    [TOK_NUMBER]         = "NODE_NUMBER",
    [TOK_PARENTHESIS]    = "PARENTHESIS",
    [TOK_END]            = "END",
    [TOK_UNKNOWN]        = "UNKNOWN"
};


const char* token_to_string(Token t)
{
    return token_type_map[t.tokenType];
}

void token_to_string_ex(Token t, char *buffer, size_t buffer_size)
{
    size_t offset = 0;

    switch (t.tokenType)
    {
        case TOK_PARENTHESIS:
        {
            size_t name_length = strlen(token_type_map[t.tokenType]);
            memcpy_s(buffer,buffer_size,token_type_map[t.tokenType],name_length);

            offset += name_length;

            if (buffer_size < offset + 6)
            {
                buffer[offset] = 0;
                return;
            }

            buffer[offset++] = ' ';
            buffer[offset++] = '-';
            buffer[offset++] = '>';
            buffer[offset++] = ' ';

            switch (*((ParenthesisContent*) t.content))
            {
                case PARENTHESIS_OPEN: buffer[offset++] = '('; break;
                case PARENTHESIS_CLOSE: buffer[offset++] = ')'; break;
            }
            buffer[offset] = 0;

        }
        return;
        case TOK_OPERATION:
        {
            size_t name_length = strlen(token_type_map[t.tokenType]);
            memcpy_s(buffer,buffer_size,token_type_map[t.tokenType],name_length);
            offset += name_length;

            if (buffer_size < offset + 6)
            {
                buffer[offset] = 0;
                return;
            }

            buffer[offset++] = ' ';
            buffer[offset++] = '-';
            buffer[offset++] = '>';
            buffer[offset++] = ' ';

            switch (((OperationContent *) t.content)->operation_type)
            {
                case OP_ADDITION: buffer[offset++] = '+'; break;
                case OP_SUBTRACTION: buffer[offset++] = '-'; break;
                case OP_MULTIPLICATION: buffer[offset++] = '*'; break;
                case OP_DIVISION: buffer[offset++] = '/'; break;
            }
            buffer[offset] = 0;
        }
        return;


        case TOK_END:
        {
            size_t name_length = strlen(token_type_map[t.tokenType]);
            memcpy_s(buffer,buffer_size,token_type_map[t.tokenType],name_length);

            offset += name_length;

            buffer[offset] = 0;

        }
        return;

        case TOK_UNKNOWN:
        {
            size_t name_length = strlen(token_type_map[t.tokenType]);
            memcpy_s(buffer,buffer_size,token_type_map[t.tokenType],name_length);

            offset += name_length;

            buffer[offset] = 0;
        }
        return;


        case TOK_NUMBER:
        {
            size_t name_length = strlen(token_type_map[t.tokenType]);
            memcpy_s(buffer,buffer_size,token_type_map[t.tokenType],name_length);

            offset += name_length;

            if (offset + 4 >= buffer_size)
            {
                buffer[offset] = 0;
                return;
            }

            buffer[offset++] = ' ';
            buffer[offset++] = '-';
            buffer[offset++] = '>';
            buffer[offset++] = ' ';


            offset += number_to_string(*((NumberContent*)t.content),buffer + offset, buffer_size - offset - 1);

            buffer[offset] = 0;
        }
        return;

    }
}

size_t number_to_string(NumberContent content, char *buffer, size_t buffer_size)
{
    if (content.number_type == NUM_INTEGER)
    {
        int number = *(int*) content.number_data;

        size_t offset = 0;

        int num_len = 0;


        //get num of digits in ten based number system by devising number by 10 until it hits 0
        {
            int divisible_number = number;
            do
            {
                divisible_number /= 10;
                num_len++;
            } while (divisible_number != 0);
        }

        // scroll number if its ten based representation don't fit into buffer
        /*
         * For example:
         * {
         *     buffer_size = 3
         *     number = 12345
         *     12345 -> 1234 -> 123
         * }
         */
        if (num_len > buffer_size)
        {
            for (int i = 0; i < num_len - buffer_size; i++)
                number /= 10;
        }


        //Devising number by ten and check the remainder
        int int_division_res;
        int remainder;
        do
        {

            if (buffer_size < (offset + 1))
                return offset;

            int_division_res = number / 10;
            remainder = number - int_division_res * 10;
            number = int_division_res;


            switch (remainder)
            {
                case 0: buffer[offset] = '0'; break;
                case 1: buffer[offset] = '1'; break;
                case 2: buffer[offset] = '2'; break;
                case 3: buffer[offset] = '3'; break;
                case 4: buffer[offset] = '4'; break;
                case 5: buffer[offset] = '5'; break;
                case 6: buffer[offset] = '6'; break;
                case 7: buffer[offset] = '7'; break;
                case 8: buffer[offset] = '8'; break;
                case 9: buffer[offset] = '9'; break;
                default: break; // remainder greater than divisor... YES
            }
            offset++;
        }
        while (number != 0);

        //reverse char order in buffer
        for (int i = 0; i < (offset / 2); i++)
        {
            char tmp = buffer[i];
            buffer[i] = buffer[offset - i - 1];
            buffer[offset - i - 1] = tmp;
        }

        return offset;
    }

    if (content.number_type == NUM_RATIONAL)
    {
        size_t offset = 0;
        double number = *((double *)content.number_data);

        unsigned long int_part = (long) number / 1;

        NumberContent internal_content;

        internal_content.number_type = NUM_INTEGER;
        internal_content.number_data = &int_part;

        unsigned long int_part_len = number_to_string(internal_content,buffer,buffer_size);

        offset = int_part_len;

        if (buffer_size < (offset + 1))  return offset;

        buffer[offset++] = '.';

        double remainder;
        do
        {
            remainder = number - (int) ((int) number / 1);
            number = remainder;
            number *= 10;

            if (buffer_size < (offset + 1))  return offset;

            switch ((int) number / 1)
            {
                case 0: buffer[offset++] = '0';break;
                case 1: buffer[offset++] = '1';break;
                case 2: buffer[offset++] = '2';break;
                case 3: buffer[offset++] = '3';break;
                case 4: buffer[offset++] = '4';break;
                case 5: buffer[offset++] = '5';break;
                case 6: buffer[offset++] = '6';break;
                case 7: buffer[offset++] = '7';break;
                case 8: buffer[offset++] = '8';break;
                case 9: buffer[offset++] = '9';break;
            }
        }
        while (remainder > 0);

        return offset;
    }

    return 0;
}
