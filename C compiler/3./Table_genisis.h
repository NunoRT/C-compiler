//Autores
//Bruno Baptista 2018278008
//Nuno Tiago 2017276208

typedef struct elem_table{
	char *nome;//tipo da funcao p exe
	char *tipo;//nome da funcao ou global
	int is_dec;
	struct elem_table* parameters;// se não tiver parametros é só uma declaraçao, caso contrario é uma declaraçao de funçoes
	struct elem_table* next;
} element;




typedef struct table{
	char *nome;//nome da funcao ou global
	char *tipo;//tipo da funcao p exe
	int peso;
	struct elem_table* declarations;
	struct table* next;
} table;


int procura_declaration(char*nome);
char* Genisis_Table(No* node,table* tabela,element*last);
table* create_global();
table * create_table(char* nome, char* tipo,int peso);
element* insert_declaration(table* tabela,char* nome, char* tipo,int);
element* cria_elemento(char* nome, char* tipo);
void print_tabela();
void checkfuncdeclaration(No* son,table* tabela);
void insert_parameters(element *id,char* nome,char* tipo);
void checkdeclaration(No* son ,table* tabela);
void checkfuncdefinition(No* son ,table* tabela);
char* lowercase(char* target);
table* insert_table(table *tabela,char *tipo,char* nome,int peso);
element* insert_declaration(table* tabela,char* nome, char* tipo,int);
void checkparamdeclaration(No* paramdeclaration,element *last);
void checkparamlist(No*paramlist,table* tabela,element*last);
void checkfuncbody(No* node,table*tabela);
void checkprogram(No* node,table* tabela);
void checkreturn(No* node,table*tabela);
void checkwhile();
char* checkid(No* id,table*tabela);
char* checkStore(No* store,table*tabela);
element* procura_tabela(No* id,table*tabela);
char *checkop(No* add,table*tabela);
char *checkop2(No* node,table*tabela);
char *checkop3(No* node,table*tabela);
char* checkcall(No* call,table* tabela);
element* procura_definition(char* nome);
element* devolve_tabela(No* call);
void checkif(No* node,table*tabela);
void checkelse(No* node,table*tabela);
void checkstatlist(No* node,table*tabela);
void checknull();
char* return_type(char* tipo1,char* tipo2);
