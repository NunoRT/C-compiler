//Autores
//Bruno Baptista 2018278008
//Nuno Tiago 2017276208

element* insert_declaration_error(char*,table*tabela);
int void_declaration_error(No* declaration);

int compatibility_error(No* pai, char* tipo1,char* tipo2);
void if_while_error(No* node,char *tipo);
void call_errors(element* dec,No*call,table* tabela);
int get_num_args(No* call);
int get_num_args_defined(element* dec);
void return_error(No* node,char* tipo,table *tabela);
void print_conflicting_types(No* node,element*declaration);
int redeclaration_error(No* node,char *nome);

int compare_declarations(No* node,element* declaration);
int check_dec_existence(No* dec_id,table* tabela);
int procura_symbol(No* id,table*tabela);
char* invalid_operator(No* node,No* son1,No* son2);
