#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>

// ----------------------
// Tokenizer
// ----------------------

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

// Func of report error
void error_at(char *loc, char *fmt, ...);

// Check of whether token is equal with op args
// if it was equal, token value was changed to Next token value and return true
// if not, return false
bool consume(char *op);

// Check of whether token is equal with op args
// if it was equal, token value was changed to Next token value
void expect(char *op);

// Check of whether token value is integer
// if so, store the integer value to token val value and return it
// if so, token value was changed to Next token value
int expect_number();

// Check of whether token value is EOF
bool at_eof();

// Create New Token and bind Next linked List
struct Token *new_token(enum TokenKind kind, struct Token *cur, char *str, int len);

// check strings on Tokenize
bool startswitch(char *p, char *q);

// Tokenize inputed string p 
// if p is space, skip
// if p is symbol, create new token and consume configured process
// if p is integer or digit, create new token and store value
// if p is EOF, return last Linked list head
struct Token *tokenize();

// -------------------
// Parser
// -------------------

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



// Create New Node
// return created new node
struct Node *new_node(enum Nodekind kind, struct Node *left, struct Node *right);

// Create New node has integer
// return created new node
struct Node *new_node_num(int val);

// expr = mul ("+" mul | "-" mul)*
// The Seventh priority
struct Node *expr();

// equality   = relational ( "==" relational | "!=" relational )*
// The Sixth priority
struct Node *equality();

// relational = add ( "<" add | "<=" add | ">" add | ">=" add)*
// The Fifth priority
struct Node *relational();

// add = mul ("+" mul | "-" mul)*
// The Fouth priority
struct Node *add();

// mul = unary ("*" unary | "/" unary)*
// The Third priority
struct Node *mul();

// unary = ("+" | "-")? unary
//         | primary
// The Second priority
struct Node *unary();

// primary = num | "(" expr ")"
// The First Priority
struct Node *primary();

// --------------------
// Code Generate Part
// --------------------

// Output assembly code by node->kind
void gen(struct Node *node);
