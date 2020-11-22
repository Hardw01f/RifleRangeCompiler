#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>

//represent Inputed Token type
enum TokenKind {
    TK_RESERVED, // Symbol
    TK_NUM, //Integer number
    TK_EOF, //End of Token
}; 

// Token Type struct
struct Token {
    enum TokenKind kind; // Token type
    struct Token *next; // Next inputed token
    int val; // if kind is TK_NUM, the value
    char *str; // String of token
    int len; // Length of Token
};

void error_at(char *loc, char *fmt, ...);

bool consume(char *op);

void expect(char *op);

int expect_number();

bool at_eof();

struct Token *new_token(enum TokenKind kind, struct Token *cur, char *str, int len);

bool startswitch(char *p, char *q);

struct Token *tokenize();

// type of Parser
enum Nodekind {
    ND_ADD, // +
    ND_SUB, // -
    ND_MUL, // *
    ND_DIV, // /
    ND_EQ,  // ==
    ND_NE,  // !=
    ND_LT,  // <
    ND_LE,  // <=
    ND_NUM, // Integer
};


struct Node {
    enum Nodekind kind;
    struct Node *right;
    struct Node *left;
    int val;
};



struct Node *new_node(enum Nodekind kind, struct Node *left, struct Node *right);

struct Node *new_node_num(int val);

struct Node *expr();

struct Node *equality();

struct Node *relational();

struct Node *add();

struct Node *mul();

struct Node *unary();

struct Node *primary();

void gen(struct Node *node);
