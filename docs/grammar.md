<Prog> ::= <Statement>*

<Statement> ::= <ExitStatement> | <VarStatement>;

<ExitStatement> ::= exit(<Expr>);

<VarDeclaration> ::= int <Identifier> = <Expr>;

<Expr> ::= <Int_literal> | <Identifier>
