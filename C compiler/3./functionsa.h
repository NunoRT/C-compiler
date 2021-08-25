//Autores
//Bruno Baptista 2018278008
//Nuno Tiago 2017276208

typedef struct no{
	char *tipo;
	char *valor;
	char *sem;
	int linha;
	int coluna;
	struct no* son;
	struct no* brosky;
} No;

typedef struct Token{
	char *nome;
	int linha;
	int coluna;
} token;

No* head;

No* create_node(char *tipo,char *valor,int linha,int coluna);
token* create_token(char *nome,int linha,int coluna);
void add_brosky(No* b1, No* b2);
void add_son(No *papi,No* son);
void add_sem(No* node,char*insere);
void print_tree(No* n, int nivel);
void free_tree(No* n);
