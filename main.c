#include "main.h"

// ----------------------
// Tokenizer
// ----------------------

//represent Inputed Token type
typedef enum {
    tk_reserved, // symbol
    tk_num, //integer number
    tk_eof, //end of token
} TokenKind;


typedef struct Token Token;

char *user_input;

// declare Token practical used
Token *token;


// Func of report error
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

// Check of whether token is equal with op args
// if it was equal, token value was changed to Next token value and return true
// if not, return false
bool consume(char *op) {
    if (token->kind != TK_RESERVED || strlen(op) != token->len || memcmp(token->str, op, token->len))
        return false;
    token = token->next;
    return true;
}

// Check of whether token is equal with op args
// if it was equal, token value was changed to Next token value
void expect(char *op) {
    if (token->kind != TK_RESERVED || strlen(op) != token->len || memcmp(token->str, op, token->len))
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
Token *new_token(enum TokenKind kind, struct Token *cur, char *str, int len) {
    Token *tok = calloc(1, sizeof(Token));
    tok->kind = kind;
    tok->str = str;
    tok->len = len;
    cur->next = tok;
    return tok;
}

// check strings on Tokenize
bool startswitch(char *p, char *q) {
    return memcmp(p, q, strlen(q)) == 0;
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
        // space is Skip
        if (isspace(*p)) {
            p++;
            continue;
        }

        // Token is 2 digits operater, create New Token
        if (startswitch(p, "==") || startswitch(p, "!=") || startswitch(p, "<=") || startswitch(p, ">=")) {
            cur = new_token(TK_RESERVED, cur, p, 2);
            p += 2;
            continue;
        }

        // Token is single digit operater, create New Token
        if (strchr("+-*/()<>", *p)){
            cur = new_token(TK_RESERVED, cur, p++, 1);
            continue;
        }

        //strtol : convert input to value
        //p -> Decimal number value (10)
        //if strtol couldn't convert input, return position of unconvert value
        //&p -> '+' or '-' or etc
        //arg is 0, that's why digits values length is unknown, so calculate and store it
        if (isdigit(*p)) {
            cur = new_token(TK_NUM, cur, p, 0);
            char *q = p;
            cur->val = strtol(p, &p, 10);
            cur->len = p - q;
            continue;
        }

        error_at(p, "Unable to tokenize");
    }

    new_token(TK_EOF, cur, p, 0);
    return head.next;
}

// -------------------
// Parser
// -------------------

// AST node type
typedef struct Node Node;

// Create New Node
// return created new node
Node *new_node(enum Nodekind kind, Node *left, Node *right) {
    Node *node = calloc(1, sizeof(Node));
    node->kind = kind;
    node->right = right;
    node->left = left;
    return node;
}

// Create New node has integer
// return created new node
Node *new_node_num(int val){
    Node *node = calloc(1, sizeof(Node));
    node->kind = ND_NUM;
    node->val = val;
    return node;
}

// prototype declare
Node *expr();
Node *equality();
Node *relational();
Node *add();
Node *mul();
Node *unary();
Node *primary();

// expr = mul ("+" mul | "-" mul)*
// The Seventh priority
Node *expr() {
    return equality();
}

// equality   = relational ( "==" relational | "!=" relational )*
// The Sixth priority
Node *equality() {
    Node *node = relational();

    for(;;) {
        if (consume("=="))
            node = new_node(ND_EQ, node, relational());
        else if (consume("!="))
           node = new_node(ND_NE, node, relational());
        else
            return node;
    }
}

// relational = add ( "<" add | "<=" add | ">" add | ">=" add)*
// The Fifth priority
Node *relational() {
    Node *node = add();

    for(;;) {
        if (consume("<"))
            node = new_node(ND_LT, node, add());
        else if (consume("<="))
            node = new_node(ND_LE, node, add());
        else if (consume(">"))
            node = new_node(ND_LT, add(), node);
        else if (consume(">="))
            node = new_node(ND_LE, add(), node);
        else
            return node;
    }
}

// add = mul ("+" mul | "-" mul)*
// The Fouth priority
Node *add() {
    Node *node = mul();
     
    for(;;) {
        if (consume("+"))
            node = new_node(ND_ADD, node, mul());
        else if (consume("-"))
            node = new_node(ND_SUB, node, mul());
        else
            return node;
    }
}

// mul = unary ("*" unary | "/" unary)*
// The Third priority
Node *mul() {
    Node *node = unary();

    for(;;) {
        if (consume("*"))
            node = new_node(ND_MUL, node, unary());
        else if (consume("/"))
            node = new_node(ND_DIV, node, unary());
        else
            return node;
    }
}

// unary = ("+" | "-")? unary
//         | primary
// The Second priority
Node *unary() {
    if (consume("+"))
        return unary();
    if (consume("-"))
        return new_node(ND_SUB, new_node_num(0), unary());
    return primary();
}

// primary = num | "(" expr ")"
// The First Priority
Node *primary() {
    if (consume("(")) {
        Node *node = expr();
        expect(")");
        return node;
    }

    return new_node_num(expect_number());
}

// --------------------
// Code Generate Part
// --------------------

// Output assembly code by node->kind
void gen(Node *node){
    if (node->kind == ND_NUM) {
        printf("  push %d\n", node->val);
        return;
    }

    gen(node->left);
    gen(node->right);

    printf("  pop rdi\n");
    printf("  pop rax\n");

    switch (node->kind) {
        case ND_ADD:
            printf("  add rax, rdi\n");
            break;
        case ND_SUB:
            printf("  sub rax, rdi\n");
            break;
        case ND_MUL:
            printf("  imul rax, rdi\n");
            break;
        case ND_DIV:
            printf("  cqo\n");
            printf("  idiv rdi\n");
            break;
        case ND_EQ:
            printf("  cmp rax, rdi\n");
            printf("  sete al\n");
            printf("  movzb rax, al\n");
            break;
        case ND_NE:
            printf("  cmp rax, rdi\n");
            printf("  setne al\n");
            printf("  movzb rax, al\n");
            break;
        case ND_LT:
            printf("  cmp rax, rdi\n");
            printf("  setl al\n");
            printf("  movzb rax, al\n");
            break;
        case ND_LE:
            printf("  cmp rax, rdi\n");
            printf("  setle al\n");
            printf("  movzb rax, al\n");
            break;
    }

    printf("  push rax\n");
}
    
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
