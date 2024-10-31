<Prog> ::= <Statement>*

<Statement> ::= <ExitStatement> | <VarStatement>;

<ExitStatement> ::= exit(<Expr>);

<VarDeclaration> ::= int <Identifier> = <Expr>;

<Expr> ::= <int_lit>
