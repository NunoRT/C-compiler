//Autores
//Bruno Baptista 2018278008
//Nuno Tiago 2017276208

%{

  int print_tokens = 0;
  int print_erros = 0;
  int num_colunas=1;
  int num_linhas=1;
  int linha_coment;
  int coluna_coment;

%}

%X COMENTARIO_BLOCO
%X COMENTARIO_LINHA

RESERVED (auto|break|case|const|continue|default|do|enum|extern|float|for|goto|inline|long|register|restrict|signed|sizeof|static|struct|switch|typedef|union|unsigned|volatile|_Bool|_Complex|_Imaginary|"["|"]"|"++"|"--")
ERRO \'[^\n\'][^\n\']+\'
ERRO2 \'[][]+\'
ID [A-Za-z_]+[A-Za-z_0-9]*
INTLIT [0-9]+
CASECHRLIT \\([0-7]{1,3})
REALLIT ({INTLIT}+("."){INTLIT}*(["e""E"][+-]?{INTLIT}+)?)|({INTLIT}*("."){INTLIT}+(["e""E"][+-]?{INTLIT}+)?)|({INTLIT}+(["e""E"][+-]?{INTLIT}+))
CHRLIT  ([^\n'\\]|\\n|\\t|\\\'|\\\\|\\\"|{CASECHRLIT})
INVALID \r\n(\r\n)
IGNORE (\t|" "|\f)
NEWLINE (\r|\n|\r\n)

%%

{RESERVED}    {if(print_tokens==1)printf("RESERVED(%s)\n",yytext);num_colunas+=yyleng;}
{NEWLINE}     {num_colunas=1;num_linhas++;}

"&"           {if(print_tokens==1)printf("BITWISEAND\n");num_colunas+=yyleng;}
"|"           {if(print_tokens==1)printf("BITWISEOR\n");num_colunas+=yyleng;}
"^"           {if(print_tokens==1)printf("BITWISEXOR\n");num_colunas+=yyleng;}
"&&"          {if(print_tokens==1)printf("AND\n");num_colunas+=yyleng;}
"="           {if(print_tokens==1)printf("ASSIGN\n");num_colunas+=yyleng;}
"*"           {if(print_tokens==1)printf("MUL\n");num_colunas+=yyleng;}
","           {if(print_tokens==1)printf("COMMA\n");num_colunas+=yyleng;}
"/"           {if(print_tokens==1)printf("DIV\n");num_colunas+=yyleng;}
"=="          {if(print_tokens==1)printf("EQ\n");num_colunas+=yyleng;}
">="          {if(print_tokens==1)printf("GE\n");num_colunas+=yyleng;}
">"           {if(print_tokens==1)printf("GT\n");num_colunas+=yyleng;}
"{"           {if(print_tokens==1)printf("LBRACE\n");num_colunas+=yyleng;}
"<="          {if(print_tokens==1)printf("LE\n");num_colunas+=yyleng;}
"("           {if(print_tokens==1)printf("LPAR\n");num_colunas+=yyleng;}
"<"           {if(print_tokens==1)printf("LT\n");num_colunas+=yyleng;}
"-"           {if(print_tokens==1)printf("MINUS\n");num_colunas+=yyleng;}
"%"           {if(print_tokens==1)printf("MOD\n");num_colunas+=yyleng;}
"!="          {if(print_tokens==1)printf("NE\n");num_colunas+=yyleng;}
"!"           {if(print_tokens==1)printf("NOT\n");num_colunas+=yyleng;}
"||"          {if(print_tokens==1)printf("OR\n");num_colunas+=yyleng;}
"+"           {if(print_tokens==1)printf("PLUS\n");num_colunas+=yyleng;}
"}"           {if(print_tokens==1)printf("RBRACE\n");num_colunas+=yyleng;}
")"           {if(print_tokens==1)printf("RPAR\n");num_colunas+=yyleng;}
";"           {if(print_tokens==1)printf("SEMI\n");num_colunas+=yyleng;}
"char"        {if(print_tokens==1)printf("CHAR\n");num_colunas+=yyleng;}
"else"        {if(print_tokens==1)printf("ELSE\n");num_colunas+=yyleng;}
"while"       {if(print_tokens==1)printf("WHILE\n");num_colunas+=yyleng;}
"if"          {if(print_tokens==1)printf("IF\n");num_colunas+=yyleng;}
"int"         {if(print_tokens==1)printf("INT\n");num_colunas+=yyleng;}
"short"       {if(print_tokens==1)printf("SHORT\n");num_colunas+=yyleng;}
"double"      {if(print_tokens==1)printf("DOUBLE\n");num_colunas+=yyleng;}
"return"      {if(print_tokens==1)printf("RETURN\n");num_colunas+=yyleng;}
"void"        {if(print_tokens==1)printf("VOID\n");num_colunas+=yyleng;}

\'{CHRLIT}\'                                               {if(print_tokens==1)printf("CHRLIT(%s)\n",yytext);num_colunas+=yyleng;}
\'(\\[^\n\r]|[^\'\\\r\n])*\'                               {printf("Line %d, col %d: invalid char constant (%s)\n",num_linhas,num_colunas,yytext);num_colunas+=yyleng;}
\'[^\r\n\']*\'?                                            {printf("Line %d, col %d: unterminated char constant\n",num_linhas,num_colunas);}
{ID}                                                       {if(print_tokens==1)printf("ID(%s)\n",yytext);num_colunas+=yyleng;}
{INTLIT}                                                   {if(print_tokens==1)printf("INTLIT(%s)\n",yytext);num_colunas+=yyleng;}
{REALLIT}                                                  {if(print_tokens==1)printf("REALLIT(%s)\n",yytext);num_colunas+=yyleng;}

"//"                            {BEGIN COMENTARIO_LINHA;linha_coment=num_linhas;coluna_coment=num_colunas;num_colunas += yyleng;}
<COMENTARIO_LINHA>.             {;}
<COMENTARIO_LINHA>{NEWLINE}     {num_colunas=1;num_linhas++;BEGIN 0;}


"/*"                            {BEGIN COMENTARIO_BLOCO;linha_coment=num_linhas;coluna_coment=num_colunas;num_colunas += yyleng;}
<COMENTARIO_BLOCO><<EOF>>	      {printf("Line %d, col %d: unterminated comment\n",linha_coment,coluna_coment);return 0;}
<COMENTARIO_BLOCO>{NEWLINE} 	  {num_colunas=1;num_linhas++;}
<COMENTARIO_BLOCO>.             {num_colunas += yyleng;}
<COMENTARIO_BLOCO>"*/"          {BEGIN 0;num_colunas += yyleng;}

{IGNORE}      {num_colunas+=yyleng;}
.	            {printf("Line %d, col %d: illegal character (%c)\n",num_linhas,num_colunas,yytext[0]);num_colunas++;}

%%

int main(int argc, char *argv[]){
  if(argc > 1){
    if (strcmp(argv[1], "-l") == 0){
      print_tokens = 1;
      print_erros = 1;
    }
    else if (strcmp(argv[1], "-e1") == 0){
      print_erros = 1;
    }
    else{
      return 0;
    }
  }
  if (argc == 1){
    print_erros = 1;
  }
  yylex();
  return 0;
}

int yywrap()
{
return 1;
}
