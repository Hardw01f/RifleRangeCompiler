#include "main.h"

typedef struct Token Token;
Token *token;

typedef struct Node Node;

char *user_input;
    
int main(int argc, char **argv){

    // Get user inputs
    user_input = argv[1];

    // Tokenize inputs
    token = tokenize();

    // Create AST using Tokenized Token by priority
    Node *node = expr();

	printf(".intel_syntax noprefix\n");
	printf(".globl main\n");
	printf("main:\n");

    // Output assembly code
    gen(node);

    printf("  pop rax\n");
    printf("  ret\n");
    return 0;
}
