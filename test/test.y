%token ID
%token INT
%token REAL
%token EMPTY

%left '+' '-'
%left '*' '/'
%right UMINUS
%%

lines   :   lines EMPTY expr
        |   expr
        ;
expr    :   expr '+' expr
        |   expr '-' expr
        |   expr '*' expr
        |   expr '/' expr
        |   '(' expr ')'
        |   '-' expr %prec UMINUS
        |   number
        ;
number  :   INT
        |   REAL
        ;