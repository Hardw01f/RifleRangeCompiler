#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>

//represent Inputed Token type
typedef enum {
    TK_RESERVED, // Symbol
    TK_NUM, //Integer number
    TK_EOF, //End of Token
} TokenKind;

typedef struct Token Token;

// Token Type struct
struct Token {
    TokenKind kind; // Token type
    Token *next; // Next inputed token
    int val; // if kind is TK_NUM, the value
    char *str; // String of token
};

// declare Token practical used
Token *token;

char *user_input;

void error_at(char *loc, char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);

    int pol = loc - user_input;
    fprintf(stderr, "%s\n", user_input);
    fprintf(stderr, "%*s", pol, "");
    fprintf(stderr, "^ ");
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    exit(1);
}

// Func of report error
void error(char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    exit(1);
}

// Check of whether token is equal with op args
// if it was equal, token value was changed to Next token value and return true
// if not, return false
bool consume(char op) {
    if (token->kind != TK_RESERVED || token->str[0] != op)
        return false;
    token = token->next;
    return true;
}

// Check of whether token is equal with op args
// if it was equal, token value was changed to Next token value
void expect(char op) {
    if (token->kind != TK_RESERVED || token->str[0] != op)
       error_at(token->str, "Token is NOT '%c'", op);
    token = token->next;
}

// Check of whether token value is integer
// if so, store the integer value to token val value and return it
// if so, token value was changed to Next token value
int expect_number() {
    if (token->kind != TK_NUM)
        error_at(token->str, "Not integer value");
    int val = token->val;
    token = token->next;
    return val;
}

// Check of whether token value is EOF
bool at_eof(){
    return token->kind == TK_EOF;
}

// Create New Token and bind Next linked List
Token *new_token(TokenKind kind, Token *cur, char *str) {
    Token *tok = calloc(1, sizeof(Token));
    tok->kind = kind;
    tok->str = str;
    cur->next = tok;
    return tok;
}

// Tokenize inputed string p 
// if p is space, skip
// if p is symbol, create new token and consume configured process
// if p is integer or digit, create new token and store value
// if p is EOF, return last Linked list head
Token *tokenize() {
    char *p = user_input;
    Token head;
    head.next = NULL;
    Token *cur = &head;

    while (*p) {
        if (isspace(*p)) {
            p++;
            continue;
        }

        if (*p == '+' || *p == '-') {
            cur = new_token(TK_RESERVED, cur, p++);
            continue;
        }

        //strtol : convert input to value
        //p -> Decimal number value (10)
        //if strtol couldn't convert input, return position of unconvert value
        //&p -> '+' or '-' or etc
        if (isdigit(*p)) {
            cur = new_token(TK_NUM, cur, p);
            cur->val = strtol(p, &p, 10);
            continue;
        }

        error_at(p, "Unable to tokenize");
    }

    new_token(TK_EOF, cur, p);
    return head.next;
}

int main(int argc, char **argv){

    user_input = argv[1];

    token = tokenize();

	printf(".intel_syntax noprefix\n");
	printf(".globl main\n");
	printf("main:\n");

    printf("  mov rax, %d\n", expect_number());

    while (!at_eof()) {
        if (consume('+')) {
            printf("  add rax, %d\n", expect_number());
            continue;
        }

        expect('-');
        printf("  sub rax, %d\n", expect_number());
    }

    printf("  ret\n");
    return 0;
}
