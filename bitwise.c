#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define CMD_LEN 10

typedef unsigned long long ull;

typedef struct {
    unsigned int n;         // Number of operators
    unsigned int dim;       // Dimension
    unsigned int opd_len;   // Operand length
    char* operators;        // Array to store operators
    int*  operands;         // Array to store operands
} Solution;

char getOperator(unsigned int code) {
    switch (code) {
        case 0: return '+';
        case 1: return '-';
        case 2: return '*';
        case 3: return '/';
        default: return '?'; // Unknown operator
    }
}

int computeSolution(Solution* solution) {
    int result = solution->operands[0];
    // Perform operations based on the operators
    for (int i = 0; i < solution->n; ++i) {
        switch (solution->operators[i]) {
            case '+':
                result += solution->operands[i + 1];
                break;
            case '-':
                result -= solution->operands[i + 1];
                break;
            case '*':
                result *= solution->operands[i + 1];
                break;
            case '/':
                result /= solution->operands[i + 1];
                break;
            default:
                return -1; // Unknown operator encountered
        }
    }

    return result;
}

void decode_instruction(Solution* solution, unsigned int code) {
    // Extracting the number of operators from the code
    const int op_bits_start = 29;
    // Extract the 3 bits for the number of operators
    solution->n = (code >> op_bits_start) & 0x7;
    ++solution->n; // +1 - as the problem says

    solution->operators = (char*)malloc(sizeof(char) * solution->n);

    // Extracting operators from the code
    for (int i = 0; i < solution->n; ++i) {
        // Extracting 2 bits for each operator
        int op_code = (code >> (op_bits_start - (2 * (i + 1)))) & 0x3;
        solution->operators[i] = getOperator(op_code);
    }

    // Extracting Dim from its dynamic position.

    // Position of dim based on the number of operators
    int dim_position = op_bits_start - (2 * solution->n);
    // The dim's 4 bits follow immediately after the operators.
    solution->dim = (code >> (dim_position - 4)) & 0xF;
    ++solution->dim; // match the actual dimension range of 1 to 16

    // Print N, operators, and dim for verification
    printf("N : %u\nOps : ", solution->n);
    for (int i = 0; i < solution->n; ++i) {
        printf("%c ", solution->operators[i]);
    }
    printf("\nDim : %u\n", solution->dim);
}

void execute_instruction(Solution* solution, unsigned int code) {
    // Extracting the number of operators from the code
    const int msb_start = 31;
    int msb_mask = (1 << msb_start) | (1 << (msb_start - 1)) | (1 << (msb_start - 2));
    solution->n = code & msb_mask;
    solution->n >>= msb_start - 2;
    ++solution->n;

    solution->operators = (char*)malloc(sizeof(char) * solution->n);
    solution->operands = (int*)malloc(sizeof(int) * (solution->n + 1));

    int start_pos = msb_start - 3;
    // Extract operators from the code
    for (int i = 0; i < solution->n; ++i) {
        solution->operators[i] = getOperator((code >> (start_pos - 1)) & 3); // Extract operator code
        start_pos -= 2; // Move to the next operator position
    }

    // Extracting the dimension from the code
    const int lsb_start = start_pos;
    int lsb_mask = (1 << lsb_start) | (1 << (lsb_start - 1)) | (1 << (lsb_start - 2)) | (1 << (lsb_start - 3));
    solution->dim = code & lsb_mask;
    solution->dim >>= lsb_start - 3;
    ++solution->dim;

    // Calculate the number of operands to be entered
    solution->opd_len = (unsigned int)ceilf((float)(solution->dim * (solution->n + 1)) / (float) 16);

    // Initialize variables for operand extraction
    start_pos = 0;
    unsigned int numbers_inserted = solution->n + 1;
    int numbers_per_block = 16 / (int)solution->dim;

    // Loop to extract operands
    for (int i = 0; i < solution->opd_len && numbers_inserted; ++i) {
        unsigned short opd = 0;
        printf("Enter block:\n");
        scanf("%hu", &opd);

        unsigned int start_pos_in_block = 16;

        // Loop to extract numbers from each block
        for (int j = 0; j < numbers_per_block && numbers_inserted; ++j) {
            unsigned int end_pos_in_block = start_pos_in_block - solution->dim;
            unsigned int mask = 0;
            // Create a mask to extract the number from the block
            for (unsigned int k = end_pos_in_block; k < start_pos_in_block; ++k) {
                mask |= 1 << k;
            }

            // Extract the number using the mask
            solution->operands[start_pos++] = (opd & mask) >> (end_pos_in_block);
            start_pos_in_block = end_pos_in_block;
            --numbers_inserted; // Decrement the count of remaining numbers
        }
    }

    // Print the extracted operands for debugging
    printf("operands: ");
    for (int i = 0; i < solution->n + 1; ++i) {
        printf("%d ", solution->operands[i]);
    }
}

int main(void) {
    char cmd[CMD_LEN];
    unsigned int code;

    scanf("%s", cmd);
    scanf("%u", &code);

    Solution* solution = malloc(sizeof(Solution));
    if (strcmp(cmd, "decode") == 0) {
        printf("Command: decode\nCode: %u\n", code);
        decode_instruction(solution, code);
    } else if (strcmp(cmd, "execute") == 0) {
        printf("Command: execute\nCode: %u\n", code);
        execute_instruction(solution, code);
        printf("\nAnswer: %d", computeSolution(solution));
    } else {
        printf("Invalid cmd. Please enter 'decode' or 'execute'.\n");
    }

    free(solution->operators);
    free(solution->operands);
    free(solution);

    return EXIT_SUCCESS;
}
