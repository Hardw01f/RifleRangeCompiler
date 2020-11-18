Priority: process

==== 2020/11/15 ====

```
4: expr    = mul ("+" mul | "-" mul)*
3: mul     = unary ("*" unary | "/" unary)*
2: unary   = ("+" | "-")? primary
1: primary = num | "(" expr ")"
```


==== 2020/11/18 =====

```
7: expr       = equality
6: equality   = relational ( "==" relational | "!=" relational )*
5: relational = add ( "<" add | "<=" add | ">" add | ">=" add)*
4: add        = mul ( "+" mul | "-" mul )*
3: mul        = unary ( "*" unary | "/" unary)*
2: unary      = ("+" | "-")? primary
1: primary    = num | "(" expr ")"
```
