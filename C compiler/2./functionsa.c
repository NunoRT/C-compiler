//Autores
//Bruno Baptista 2018278008
//Nuno Tiago 2017276208



#include "functionsa.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//tipo é a string que vamos passaar, valor ou vai ser null ou vai ser o valor da pilha
No* create_node(char *tipo,char *valor){
    No* node = (No*) malloc(sizeof(No));
    if(node == NULL) exit(0);
    node->tipo=strdup(tipo);
    node->valor=valor;
    node->son=NULL;
    node->brosky=NULL;
    return node;

}

//lista ligada de nos que vai ter um pai só(passado para topo da pilha)
void add_brosky(No* b1, No* b2){
    if (b1==NULL || b2==NULL){
        //printf("error bro1 or bro2 Null");
        return;
    }
    No* aux=b1;
    while(aux->brosky!=NULL){
        aux=aux->brosky;
    }
    aux->brosky=b2;
}



void add_son(No *papi,No* son){

    if (papi==NULL || son==NULL){

        return;
    }

    papi->son=son;

}

void print_tree(No* n, int nivel) {
    int i;
	if(n==NULL) return;
	for(i=0; i<nivel;i++) printf("..");
	if(n->valor == NULL) printf("%s\n", n->tipo);
	else printf("%s(%s)\n", n->tipo, n->valor);

	//imprime filhos primeiro
	print_tree(n->son, nivel+1);
	//imprime irmaos
	print_tree(n->brosky, nivel);
 }

void free_tree(No* n){
	if (n == NULL) return;
	//apagar filhos
	if (n->son != NULL){
		free_tree(n->son);
	}
	//apagar irmaos
	if (n->brosky != NULL){
		free_tree(n->brosky);
	}
	free(n->tipo);
	free(n);
}
