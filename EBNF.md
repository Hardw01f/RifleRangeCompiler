Priority: process

4: expr    = mul ("+" mul | "-" mul)*
3: mul     = unary ("*" unary | "/" unary)*
2: unary   = ("+" | "-")? primary
1: primary = num | "(" expr ")"
