//Autores
//Bruno Baptista 2018278008
//Nuno Tiago 2017276208


%{
#include <stdlib.h>
#include "functionsa.h"
#include <stdio.h>
#include <string.h>
#include <stdio.h>
int yylex(void);


void yyerror (char *s);

extern int num_colunas;
extern int num_linhas;
extern char* yytext;
extern int print_erros;
extern int erro_eof;

int erros=0;


%}

%left	COMMA

%right ASSIGN

%left	OR

%left	AND

%left	BITWISEOR

%left	BITWISEXOR

%left	BITWISEAND

%left	EQ NE

%left   LT GT LE GE

%left   PLUS MINUS

%left	MUL DIV MOD

%right NOT

%left	LPAR RPAR

%nonassoc IF

%nonassoc ELSE



%token <id> CHRLIT
%token <id> ID

%token <id> INTLIT
%token <id> REALLIT

%token <id> RESERVED

%token  BITWISEAND
%token  BITWISEOR
%token  BITWISEXOR
%token  AND
%token ASSIGN
%token  MUL
%token COMMA
%token  DIV
%token EQ
%token  GE
%token  GT
%token  LBRACE
%token  LE
%token  LPAR
%token  LT
%token MINUS
%token  MOD
%token  NE
%token  NOT
%token  OR
%token PLUS
%token  RBRACE
%token  RPAR
%token  SEMI
%token  CHAR
%token  ELSE
%token  WHILE
%token  IF
%token  INT
%token  SHORT
%token  DOUBLE
%token RETURN
%token  VOID

%type <No> Program FunctionsAndDeclarations FunctionDefinition FunctionBody DeclarationsAndStatements FunctionDeclaration FunctionDeclarator ParameterList ParameterDeclaration Declaration Declarationaux TypeSpec Declarator Statement Statementaux Expraux Expr StatementNew ParameterListaux

%union{
struct no* No;
char* id;
}



%%

Program: FunctionsAndDeclarations {head=create_node("Program",NULL);
					add_son(head,$1);
					}

;

FunctionsAndDeclarations: FunctionDefinition    {$$=$1;}
			| FunctionDeclaration 	{$$=$1;}
			| Declaration {$$=$1;}
			| FunctionDefinition FunctionsAndDeclarations {add_brosky($1,$2);
									$$=$1;
									}
			| FunctionDeclaration FunctionsAndDeclarations {add_brosky($1,$2);
									$$=$1;
									}
			| Declaration FunctionsAndDeclarations {add_brosky($1,$2);
									$$=$1;
									}
;
FunctionDefinition: TypeSpec FunctionDeclarator FunctionBody  {
								No *FuncDefinition=create_node("FuncDefinition",NULL);
								add_brosky($1,$2);
								add_brosky($2,$3);
								add_son(FuncDefinition,$1);
								$$=FuncDefinition;
								}
;

FunctionBody: LBRACE DeclarationsAndStatements RBRACE   {No *FuncBody=create_node("FuncBody",NULL);
							if ($2==NULL) $$=FuncBody;
							else{
							    add_son(FuncBody,$2);
							    $$=FuncBody;
							}

							}
	     |LBRACE RBRACE {No *FuncBody=create_node("FuncBody",NULL);$$=FuncBody;}
;

DeclarationsAndStatements: Statement DeclarationsAndStatements {if ($1==NULL && $2!=NULL){
								    $$=$2;
								}
								else if($1!=NULL && $2!=NULL){
								add_brosky($1,$2);
								$$=$1;
									}
								else if($1!=NULL && $2==NULL){
									$$=$1;
									}

								else if($1==NULL && $2==NULL){
									$$=NULL;
									}
								}
			| Statement {	if ($1==NULL) $$=NULL;
					else $$=$1;

			}
			|Declaration DeclarationsAndStatements {if ($2==NULL){
								    $$=$1;
								}
								else{
								    add_brosky($1,$2);
								    $$=$1;
								}
}
			| Declaration { $$=$1;

					}

;

FunctionDeclaration: TypeSpec FunctionDeclarator SEMI {No *FuncDeclaration=create_node("FuncDeclaration",NULL);
							add_brosky($1,$2);
							add_son(FuncDeclaration,$1);
							$$=FuncDeclaration;
							}
;

FunctionDeclarator: ID LPAR ParameterList RPAR {No *Id=create_node("Id",$1);

						add_brosky(Id,$3);
						$$=Id;
						}
;
ParameterList: ParameterDeclaration ParameterListaux {No *ParamList=create_node("ParamList",NULL);
					if($2!=NULL){
						add_brosky($1,$2);
					}
					add_son(ParamList,$1);
					$$=ParamList;
					}
;

ParameterListaux: COMMA ParameterDeclaration ParameterListaux {
								if($3!=NULL){
									add_brosky($2,$3);
								}
								$$=$2;
							}
		| 			{$$=NULL;}
;

ParameterDeclaration: TypeSpec   {No *ParamDeclaration=create_node("ParamDeclaration",NULL);
					add_son(ParamDeclaration,$1);
					$$=ParamDeclaration;
				}
		    | TypeSpec ID  {No *ParamDeclaration=create_node("ParamDeclaration",NULL);
				    No *Id=create_node("Id",$2);
				    add_brosky($1,Id);
				    add_son(ParamDeclaration,$1);
				    $$=	ParamDeclaration;
						}
;
Declaration: TypeSpec Declarator Declarationaux SEMI{
							struct no*new=NULL;
							if($3!=NULL){
								add_brosky($2,$3);
								new=$2;
							}
							else{
								new=$2;
								}
							No* primeiro =NULL;
							No* next =NULL;


								while(new!=NULL){
								//criamos nos declarations
								No*Declaration=create_node("Declaration",NULL);
								struct no*temp=(No*)malloc(sizeof(No));
								//copiamos o valor do typespec
								temp->tipo=strdup($1->tipo);
								temp->valor=$1->valor;
							    	temp->son=NULL;
							    	temp->brosky=NULL;
								//começamos a criar os filhos do declaration
								add_son(Declaration,temp);//adicionamos o typespec
								next=new->brosky;
								new->brosky=NULL;

								add_brosky(temp,new); //adicionamos  o proximo elemento e cortamos os outros (id)
								//se houver proximo e este for diferente de id, ou então situação do tipo int id=id
								if((next && strcmp(next->tipo,"Id")) || (next && strcmp(next->tipo,"Ide")==0) )	{
									if(strcmp(next->tipo,"Ide")==0) strcpy(next->tipo,"Id"); //voltamos a colocar o tipo correto
									new=next;
									next=new->brosky;
									new->brosky=NULL;
									add_brosky(temp,new);
								}
								if (primeiro==NULL) primeiro = Declaration;
								else add_brosky(primeiro,Declaration);

								new=next;

								}
							$$=primeiro;



							}

;
Declarationaux:           {$$=NULL;}
	      | COMMA Declarator Declarationaux {
						if($3!=NULL){
						   add_brosky($2,$3);
						  	  $$=$2;

						}

						  else{$$=$2;}

						}
;


TypeSpec: CHAR {$$=create_node("Char",NULL);}
	| INT {$$=create_node("Int",NULL);}
	| VOID {$$=create_node("Void",NULL);}
	| SHORT {$$=create_node("Short",NULL);}
	| DOUBLE {$$=create_node("Double",NULL);}
;
Declarator: ID {$$=create_node("Id",$1);}
	  |ID ASSIGN Expr {
			No* Id=create_node("Id",$1);
			if(strcmp($3->tipo,"Id")==0) strcpy($3->tipo,"Ide"); //mudamos o tipo para conseguir diferenciar a,b de a=b,nas declarations no segundo caso só criamos uma declaration

			add_brosky(Id,$3);
			$$=Id;
			}
;
Statement: SEMI   {$$=NULL;}
	|Expr SEMI {$$=$1;}
;
Statement: LBRACE Statementaux RBRACE {//Lista de statements
					if($2!=NULL && $2->brosky!=NULL){
					    No* StatList = create_node("StatList",NULL);
					    add_son(StatList,$2);
					    $$=StatList;
					}
					else{
                                            $$ = $2; //inclui o null
                                        }
					}

	|LBRACE RBRACE {$$=NULL;}

;
Statementaux: StatementNew Statementaux {
					if($1==NULL && $2==NULL) $$=NULL;
					else if ($1!=NULL && $2==NULL) $$=$1;
					else if ($1==NULL && $2!=NULL) $$=$2;
					else if ($1!=NULL && $2!=NULL){
					    add_brosky($1,$2);
					    $$=$1;
					}
			}

	    |StatementNew{
			if ($1==NULL) $$=NULL;
			else $$=$1;
			}


;
Statement: IF LPAR Expr RPAR Statement %prec IF    {No* If= create_node("If",NULL);
						    No* if_block=create_node("Null",NULL);
						    No* else_block=create_node("Null",NULL);
						   if ($5!=NULL){
							add_brosky($3,$5);
							add_brosky($5,else_block);
							add_son(If,$3);
							$$=If;
							}

						   else{
							add_brosky($3,if_block);
							add_brosky(if_block,else_block);
							add_son(If,$3);
							$$=If;
							}

							}

	  |IF LPAR Expr RPAR Statement ELSE Statement { No* If= create_node("If",NULL);
						    	No* if_block=create_node("Null",NULL);
						   	No* else_block=create_node("Null",NULL);
							if($7==NULL){
							    if($5!=NULL){
							        add_brosky($3,$5);
								add_brosky($5,else_block);
							    }
							    else{ //ambos null
							        add_brosky($3,if_block);
							        add_brosky(if_block,else_block);

							    }
							}
							else{ //ambos não null
							    if($5!=NULL){
							    	add_brosky($3,$5);
							    	add_brosky($5,$7);

							    }
							    else{
								add_brosky($3,if_block);
								add_brosky(if_block,$7);
								}
							}
							add_son(If,$3);
							$$=If;
							}
;
Statement: WHILE LPAR Expr RPAR Statement {No* While= create_node("While",NULL);
			   //apesar de statement não ter produção vazia, pode ser Null quando tem por exemplo um ; ou {} ,porque estes não vão para a árvore
			   if($5!=NULL){
				 add_brosky($3,$5);

			    }
			   else{
				No* Vazio= create_node("Null",NULL);
				add_brosky($3,Vazio);
			    }
			    add_son(While,$3);

			   $$=While;}
;
Statement: RETURN SEMI  {No* Return= create_node("Return",NULL);
			No* Vazio= create_node("Null",NULL);
			add_son(Return,Vazio);
			$$=Return;}
	 |RETURN Expr SEMI {No* Return= create_node("Return",NULL);
			   add_son(Return,$2);
			   $$=Return;}
;
Expr: Expr ASSIGN Expr  {No* Store=create_node("Store",NULL);
		add_brosky($1,$3);
		add_son(Store,$1);
		$$=Store;}
    |Expr COMMA Expr  {No* Comma=create_node("Comma",NULL);
		add_brosky($1,$3);
		add_son(Comma,$1);
		$$=Comma;}
;
Expr: Expr PLUS Expr  {No* Add=create_node("Add",NULL);
		add_brosky($1,$3);
		add_son(Add,$1);
		$$=Add;}
    |Expr MINUS Expr  {No* Sub=create_node("Sub",NULL);
		add_brosky($1,$3);
		add_son(Sub,$1);
		$$=Sub;}
    |Expr MUL Expr  {No* Mul=create_node("Mul",NULL);
		add_brosky($1,$3);
		add_son(Mul,$1);
		$$=Mul;}
    |Expr DIV Expr  {No* Div=create_node("Div",NULL);
		add_brosky($1,$3);
		add_son(Div,$1);
		$$=Div;}
    |Expr MOD Expr  {No* Mod=create_node("Mod",NULL);
		add_brosky($1,$3);
		add_son(Mod,$1);
		$$=Mod;}
;
Expr: Expr OR Expr {No* Or=create_node("Or",NULL);
		add_brosky($1,$3);
		add_son(Or,$1);
		$$=Or;}
    |Expr AND Expr {No* And=create_node("And",NULL);
		add_brosky($1,$3);
		add_son(And,$1);
		$$=And;}
    |Expr BITWISEAND Expr {No* Bitwiseand=create_node("BitWiseAnd",NULL);
		add_brosky($1,$3);
		add_son(Bitwiseand,$1);
		$$=Bitwiseand;}
    |Expr BITWISEOR Expr {No* Bitwiseor=create_node("BitWiseOr",NULL);
		add_brosky($1,$3);
		add_son(Bitwiseor,$1);
		$$=Bitwiseor;}
    |Expr BITWISEXOR Expr {No* Bitwisexor=create_node("BitWiseXor",NULL);
		add_brosky($1,$3);
		add_son(Bitwisexor,$1);
		$$=Bitwisexor;}
;
Expr: Expr EQ Expr  {No* Eq=create_node("Eq",NULL);
		add_brosky($1,$3);
		add_son(Eq,$1);
		$$=Eq;}
    |Expr NE Expr    {No* Ne=create_node("Ne",NULL);
		add_brosky($1,$3);
		add_son(Ne,$1);
		$$=Ne;}
    |Expr LE Expr    {No* Le=create_node("Le",NULL);
		add_brosky($1,$3);
		add_son(Le,$1);
		$$=Le;}
    |Expr GE Expr    {No* Ge=create_node("Ge",NULL);
		add_brosky($1,$3);
		add_son(Ge,$1);
		$$=Ge;}
    |Expr LT Expr    {No* Lt=create_node("Lt",NULL);
		add_brosky($1,$3);
		add_son(Lt,$1);
		$$=Lt;}
    |Expr GT Expr    {No* Gt=create_node("Gt",NULL);
		add_brosky($1,$3);
		add_son(Gt,$1);
		$$=Gt;}
;
Expr: PLUS Expr %prec NOT {No* Plus=create_node("Plus",NULL);
		add_son(Plus,$2);
		$$=Plus;}
    |MINUS Expr %prec NOT {No* Minus=create_node("Minus",NULL);
		add_son(Minus,$2);
		$$=Minus;}
    |NOT Expr   {No* Not=create_node("Not",NULL);
		add_son(Not,$2);
		$$=Not;}
;
Expr: ID LPAR RPAR  {No* Call=create_node("Call",NULL);
		     No* Id=create_node("Id",$1);
		     add_son(Call,Id);
		     $$=Call;}
    |ID LPAR Expr Expraux RPAR  {No* Call=create_node("Call",NULL);
		     No* Id=create_node("Id",$1);
		     if($4!=NULL){
			add_brosky(Id,$3);
			add_brosky($3,$4);
                      }
		     else{
			add_brosky(Id,$3);

		     }
		     add_son(Call,Id);
		     $$=Call;}
;
Expraux:  %prec COMMA    {$$=NULL;}
	|Expraux COMMA Expr {
				if($1!=NULL){
					add_brosky($1,$3);
					$$=$1;
				}
				else $$=$3;


				}
;
Expr: ID      {$$=create_node("Id",$1);}
    | INTLIT  {$$=create_node("IntLit",$1);}
    | CHRLIT  {$$=create_node("ChrLit",$1);}
    | REALLIT {$$=create_node("RealLit",$1);}
    | LPAR Expr RPAR {$$=$2;}

;

Declaration: error SEMI		{$$=NULL;}
;
StatementNew: error SEMI   {$$=NULL;}
            |Statement	 	{if ($1==NULL) $$=NULL;
				else{
				    $$=$1;
				}

				}
;

Statement: LBRACE error RBRACE       {$$=NULL;}
;
Expr: ID LPAR error RPAR      {$$=NULL;}
;
Expr: LPAR error RPAR        {$$=NULL;}
;


%%
void yyerror ( char* s ) {


	if(erro_eof==1)
	printf ( "Line %d, col %d: %s: %s\n", num_linhas,(int)num_colunas,s, yytext );
	else printf ( "Line %d, col %d: %s: %s\n", num_linhas,(int)(num_colunas-strlen(yytext)),s, yytext );
	erros++;
}
