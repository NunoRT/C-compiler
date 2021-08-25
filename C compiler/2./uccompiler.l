//Autores
//Bruno Baptista 2018278008
//Nuno Tiago 2017276208

%{
  #include "y.tab.h"
  #include "functionsa.h"

  int print_tokens = 0;
  int print_erros = 0;
  int num_colunas=1;
  int num_linhas=1;
  int linha_coment=1;
  int coluna_coment=1;
  int coluna_chrlit=1;
	int erro_eof=0;
  extern int erros;

%}

%X COMENTARIO_BLOCO
%X COMENTARIO_LINHA
%X CHRLIT_STATE



RESERVED (auto|break|case|const|continue|default|do|enum|extern|float|for|goto|inline|long|register|restrict|signed|sizeof|static|struct|switch|typedef|union|unsigned|volatile|_Bool|_Complex|_Imaginary|"["|"]"|"++"|"--")
ERRO \'[^\n\'][^\n\']+\'
ERRO2 \'[][]+\'
ID [A-Za-z_]+[A-Za-z_0-9]*
INTLIT [0-9]+
CASECHRLIT \\([0-7]{1,3})
REALLIT ({INTLIT}+("."){INTLIT}*(["e""E"][+-]?{INTLIT}+)?)|({INTLIT}*("."){INTLIT}+(["e""E"][+-]?{INTLIT}+)?)|({INTLIT}+(["e""E"][+-]?{INTLIT}+))
CHRLIT ([^\n\'\\]|\\n|\\t|\\\'|\\\\|\\\"|{CASECHRLIT})
IGNORE (\t|" "|\f)
NEWLINE \r|\n|\r\n

%%

"/*"                            {BEGIN COMENTARIO_BLOCO;linha_coment=num_linhas;coluna_coment=num_colunas;num_colunas += yyleng;}
<COMENTARIO_BLOCO><<EOF>>	      {printf("Line %d, col %d: unterminated comment\n",linha_coment,coluna_coment);return 0;}
<COMENTARIO_BLOCO>{NEWLINE} 	  {num_colunas=1;num_linhas++;}
<COMENTARIO_BLOCO>.             {num_colunas += yyleng;}
<COMENTARIO_BLOCO>"*/"          {BEGIN 0;num_colunas += yyleng;}



"//"                            {BEGIN COMENTARIO_LINHA;linha_coment=num_linhas;coluna_coment=num_colunas;num_colunas += yyleng;}
<COMENTARIO_LINHA>.             {;} {num_colunas++;}
<COMENTARIO_LINHA>{NEWLINE}     {num_colunas=1;num_linhas++;BEGIN 0;}



{RESERVED} {if(print_tokens==1)printf("RESERVED(%s)\n",yytext);num_colunas+=yyleng;yylval.id=(char*)strdup(yytext);return RESERVED;}
<<EOF>> {erro_eof=1;return 0;}
{NEWLINE} {num_colunas=1;num_linhas++;}


"&" {if(print_tokens==1)printf("BITWISEAND\n");num_colunas+=yyleng;return BITWISEAND;}
"|" {if(print_tokens==1)printf("BITWISEOR\n");num_colunas+=yyleng;return BITWISEOR;}
"^" {if(print_tokens==1)printf("BITWISEXOR\n");num_colunas+=yyleng;return BITWISEXOR;}
"&&" {if(print_tokens==1)printf("AND\n");num_colunas+=yyleng;return AND;}
"=" {if(print_tokens==1)printf("ASSIGN\n");num_colunas+=yyleng;return ASSIGN;}
"*" {if(print_tokens==1)printf("MUL\n");num_colunas+=yyleng;return MUL;}
"," {if(print_tokens==1)printf("COMMA\n");num_colunas+=yyleng;return COMMA;}
"/" {if(print_tokens==1)printf("DIV\n");num_colunas+=yyleng;return DIV;}
"==" {if(print_tokens==1)printf("EQ\n");num_colunas+=yyleng;return EQ;}
">=" {if(print_tokens==1)printf("GE\n");num_colunas+=yyleng;return GE;}
">" {if(print_tokens==1)printf("GT\n");num_colunas+=yyleng;return GT;}
"{" {if(print_tokens==1)printf("LBRACE\n");num_colunas+=yyleng;return LBRACE;}
"<=" {if(print_tokens==1)printf("LE\n");num_colunas+=yyleng;return LE;}
"(" {if(print_tokens==1)printf("LPAR\n");num_colunas+=yyleng;return LPAR;}
"<" {if(print_tokens==1)printf("LT\n");num_colunas+=yyleng;return LT;}
"-" {if(print_tokens==1)printf("MINUS\n");num_colunas+=yyleng;return MINUS;}
"%" {if(print_tokens==1)printf("MOD\n");num_colunas+=yyleng;return MOD;}
"!=" {if(print_tokens==1)printf("NE\n");num_colunas+=yyleng;return NE;}
"!" {if(print_tokens==1)printf("NOT\n");num_colunas+=yyleng;return NOT;}
"||" {if(print_tokens==1)printf("OR\n");num_colunas+=yyleng;return OR;}
"+" {if(print_tokens==1)printf("PLUS\n");num_colunas+=yyleng;return PLUS;}
"}" {if(print_tokens==1)printf("RBRACE\n");num_colunas+=yyleng;return RBRACE;}
")" {if(print_tokens==1)printf("RPAR\n");num_colunas+=yyleng;return RPAR;}
";" {if(print_tokens==1)printf("SEMI\n");num_colunas+=yyleng;return SEMI;}
"char" {if(print_tokens==1)printf("CHAR\n");num_colunas+=yyleng;return CHAR;}
"else" {if(print_tokens==1)printf("ELSE\n");num_colunas+=yyleng;return ELSE;}
"while" {if(print_tokens==1)printf("WHILE\n");num_colunas+=yyleng;return WHILE;}
"if" {if(print_tokens==1)printf("IF\n");num_colunas+=yyleng;return IF;}
"int" {if(print_tokens==1)printf("INT\n");num_colunas+=yyleng;return INT;}
"short" {if(print_tokens==1)printf("SHORT\n");num_colunas+=yyleng;return SHORT;}
"double" {if(print_tokens==1)printf("DOUBLE\n");num_colunas+=yyleng;return DOUBLE;}
"return" {if(print_tokens==1)printf("RETURN\n");num_colunas+=yyleng;return RETURN;}
"void" {if(print_tokens==1)printf("VOID\n");num_colunas+=yyleng;return VOID;}

{ID} {if(print_tokens==1)printf("ID(%s)\n",yytext);num_colunas+=yyleng;yylval.id=(char*)strdup(yytext);return ID;}

\'{CHRLIT}\'	{if(print_tokens==1) printf("CHRLIT(%s)\n",yytext);num_colunas+=yyleng;yylval.id=(char*)strdup(yytext);return CHRLIT;}

\'(\\[^\n\r]|[^\'\\\r\n])*\'                               {printf("Line %d, col %d: invalid char constant (%s)\n",num_linhas,num_colunas,yytext);num_colunas+=yyleng;}
\'[^\r\n\']*\'?                                            {printf("Line %d, col %d: unterminated char constant\n",num_linhas,num_colunas);}


{INTLIT} {if(print_tokens==1)printf("INTLIT(%s)\n",yytext);num_colunas+=yyleng;/*yylval.value=atoi(yytext);*/yylval.id=(char*)strdup(yytext);return INTLIT;}

{REALLIT} {if(print_tokens==1)printf("REALLIT(%s)\n",yytext);num_colunas+=yyleng;/*yylval.value=atoi(yytext);*/yylval.id=(char*)strdup(yytext);return REALLIT;}

{IGNORE} {num_colunas+=yyleng;}

.	{if(print_erros==1) printf("LINE %d, col %d: illegal character (%c)\n",num_linhas,num_colunas,yytext[0]);num_colunas++;/*while(strcmp(yytext," ")!=0||strcmp(yytext,"\n")!=0) yytext++;*/}

%%

int main(int argc, char *argv[]){
  if(argc > 1){
    if (strcmp(argv[1], "-l") == 0){
      print_tokens = 1;
      print_erros = 1;
      yylex();
    }
    else if (strcmp(argv[1], "-e1") == 0){
      print_erros = 1;
      yylex();
    }
     else if (strcmp(argv[1], "-e2") == 0){
      print_erros = 1;
      yyparse();
    }
    else if (strcmp(argv[1], "-t") == 0){
      yyparse();
      if(erros==0){
          print_tree(head,0);
      free(head);
	    }
	  }
    else{
      return 0;
    }


  }

  if(argc == 1){
    print_erros = 1;
    yyparse();
  }

  return 0;
}

int yywrap()
{
return 1;
}
