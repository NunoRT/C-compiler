//Autores
//Bruno Baptista 2018278008
//Nuno Tiago 2017276208


typedef struct no{
	char *tipo;
	char *valor;
	struct no* son;
	struct no* brosky;
} No;

No* head;

No* create_node(char *tipo,char *valor);
void add_brosky(No* b1, No* b2);
void add_son(No *papi,No* son);

void print_tree(No* n, int nivel);
void free_tree(No* n);
