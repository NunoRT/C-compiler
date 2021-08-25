//Autores
//Bruno Baptista 2018278008
//Nuno Tiago 2017276208

#include "functionsa.h"
#include "Table_genisis.h"
#include "errors.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int active;
int decl;

//ponteiro para tabela global

extern table* global;

char* lowercase(char* target){
	char* lowercase=strdup(target);
	for(int i = 0; target[i]; i++){
  		lowercase[i] = tolower(target[i]);
	}
	return lowercase;
}

int getpeso(element*last){

	element*aux=global->declarations;
	int peso=2;
	while(aux!=NULL){

		if(strcmp(aux->tipo,last->tipo)!=0 && aux->parameters!=NULL){

			peso++;
		}
		else if (strcmp(aux->tipo,last->tipo)==0){
			return peso;
		}

		aux=aux->next;
	}
	return peso;

}


table * create_table(char* nome, char* tipo,int peso){
	//printf("---------------tabela------------ %s\n",nome);
	table* tabela=(table*)malloc(sizeof(table));
	tabela->nome=strdup(nome);
	tabela->tipo=strdup(tipo);
	tabela->declarations=NULL;
	tabela->peso=peso;
	tabela->next=NULL;
	return tabela;

}


element* procura_tabela(No* id,table*tabela){

	//primeiro procuramos na tabela local
	element*aux=tabela->declarations;
	while(aux!=NULL){

		if(strcmp(aux->tipo,id->valor)==0){
			return aux;
		}
		//percorremos parametros caso hajam
		element*aux2=aux->parameters;
		while(aux2!=NULL){

			if(strcmp(aux2->tipo,id->valor)==0){
				return aux2;
			}
			aux2=aux2->next;
		}


		aux=aux->next;

	}


	//se nao encontrou na tabela local vamos procurar na tabela global

	element* global_aux=global->declarations;
	while(global_aux!=NULL){
		//aux->tipo nome da variavel  id->valor nome da variavel
		if(strcmp(global_aux->tipo,id->valor)==0){
			return global_aux;
		}
		//percorremos parametros caso hajam
		element* global_aux2=global_aux;
		while(global_aux2!=NULL){
			if(strcmp(global_aux->tipo,id->valor)==0){
				return global_aux2;
			}
			global_aux2=global_aux2->next;
		}


		global_aux=global_aux->next;

	}



	return NULL;//Chega aqui se nao encontrar nenhuma declaracao
}



element* cria_elemento(char* nome, char* tipo){
	element* elemento=(element*)malloc(sizeof(element));
	elemento->nome=strdup(nome);
	elemento->tipo=strdup(tipo);
	elemento->parameters=NULL;
	elemento->next=NULL;
	return elemento;
}
//devolve 1 se existir
//devolve 0 se nao encontrar nada
int procura_declaration(char*nome){
	element*aux=global->declarations;
	while(aux!=NULL){

		if(strcmp(aux->tipo,nome)==0){
			return 1;
		}

		aux=aux->next;
	}
	return 0;
}


//procura o nome da funcao na tabela global
element* procura_definition(char* nome){
	element*aux=global->declarations;
	while(aux!=NULL){
		//printf("comp %s e %s\n",aux->tipo,nome);
		if(strcmp(aux->tipo,nome)==0 && aux->parameters!=NULL){
			return aux;
		}

		aux=aux->next;
	}
	return NULL;
}

// recebe um is_dec=1 se for uma declaracao e um is dec=0 se for uma dfinicao
element* insert_declaration(table* tabela,char* nome, char* tipo,int is_dec){

	element* elemento=cria_elemento(lowercase(nome),tipo);
	elemento->is_dec=is_dec;
	element* aux=tabela->declarations;


	element* check=insert_declaration_error(tipo,tabela);

	//se já existir devolvemos o ponteiro do que já está lá
	//Se for NULL ainda n foi inserida
	if(check==NULL){


		if(tabela->declarations==NULL){
			tabela->declarations=elemento;

			return tabela->declarations;
		}
		else{

			while(aux->next!=NULL) aux=aux->next;
			aux->next=elemento;

			return aux->next;
		}
	}
	else{

		return NULL;
	}
	return NULL;//NUNCA CHEGA AQUI

}




table* insert_table(table *tabela,char *tipo,char* nome,int peso){
	table* nova_tabela=create_table(nome,tipo,peso);
	table* aux=global;
	table *aux2=aux->next;
	//a global fica sempre na mesma posição, passamos á tabela seguinte

	//se ainda sõ tiver a global insere só
	if(aux2==NULL){
		aux->next=nova_tabela;
		return aux->next;
	}

	while(aux2!=NULL){


		if(((nova_tabela->peso<aux2->peso) || aux2->peso==0) && (nova_tabela->peso!=0)){

			aux->next=nova_tabela;
			nova_tabela->next=aux2;
			return aux->next;

		}

		aux=aux->next;
		aux2=aux->next;
	}
	//se chegar aqui é porque estamos a inserir algo com peso 0, e só metemos no final
	aux->next=nova_tabela;
	return aux->next;


}

element* devolve_tabela(No* call){

	//procurar na tabela global a declaração desta funcao

	element*aux=global->declarations;
	No* id=call->son;
	while(aux!=NULL){
		//procurar nome da funcao na tabela global
		if(strcmp(aux->tipo,id->valor)==0 && aux->parameters!=NULL){

			return aux;

		}

		aux=aux->next;



	}//se nao encontrar devolve null -> mensagem de erro

	return NULL;

}






void print_tabela(table* tabela){

	table* iter=tabela;
	//percorre tabelas
	//printf("putchar parameters: %s\n",global->declarations->next->parameters->nome);
	global->declarations->parameters->next=NULL;
	iter->nome=strdup("Globale");

	while(iter!=NULL){
		element*aux=iter->declarations;

		if(strcmp(iter->nome,"Globale")==0)
			printf("===== Global Symbol Table =====\n");
		else{
			printf("\n");
			printf("===== Function %s Symbol Table =====\n",iter->nome);

		}
		if(aux==NULL){

		}
		else{
			//percorre declaracoes
			while(aux!=NULL) {
				element*aux2=aux->parameters;

				if(aux2==NULL){


						printf("%s\t%s\n",aux->tipo,aux->nome);
				}
				else{
					if(strcmp(iter->nome,"Globale")==0){


							printf("%s\t%s(",aux->tipo,aux->nome);

						//percorre parametros
						while(aux2!=NULL){
							//se for o ultimo param
							if(aux2->next==NULL){
								if(strcmp(aux2->tipo,"null")==0)
									printf("%s)\n",aux2->nome);
								else
									printf("%s)\n",aux2->nome);
							}
							//se for um parametro do meio
							else {

								if(strcmp(aux2->tipo,"null")==0)
									printf("%s,",aux2->nome);
								else
									printf("%s,",aux2->nome);
							}
							aux2=aux2->next;
						}
					}
					else{
						printf("%s\t%s\n",aux->tipo,aux->nome);
						while(aux2!=NULL){
							if(strcmp(aux2->tipo,"null")!=0)
								printf("%s\t%s\tparam\n",aux2->tipo,aux2->nome);

							aux2=aux2->next;
						}

					}



				}//else

				aux=aux->next;
			}//while aux
		}
	iter=iter->next;
	}
	printf("\n");
}


table* create_global(){
	table* global=create_table("Global","null",1);
	insert_declaration(global,"int","putchar",1);
	element*parameter=cria_elemento("int","null");
	global->declarations->parameters=parameter;
	insert_declaration(global,"int","getchar",1);
	element*parameter2=cria_elemento("void","null");
	global->declarations->next->parameters=parameter2;
	return global;
}





char* return_type(char* tipo1,char* tipo2){

	if(strcmp(tipo1,"undef")==0 || strcmp(tipo2,"undef")==0 || strcmp(tipo1,"void")==0 || strcmp(tipo2,"void")==0){
		return "undef";
	}


	//int treatment (int + char; int + short; int + int) ou (char + int; short + int; int + int) return int
	if((strcmp(lowercase(tipo1),"int")==0 && strcmp(lowercase(tipo2),"double")!=0)  ||   (strcmp(lowercase(tipo2),"int")==0 && strcmp(lowercase(tipo1),"double")!=0)){
		return "int";
	}
	//int treatment (double +int)  || (int + double)
	if((strcmp(lowercase(tipo1),"int")==0 && strcmp(lowercase(tipo2),"double")==0)    || (strcmp(lowercase(tipo2),"int")==0 && strcmp(lowercase(tipo1),"double")==0)){
		return "double";

	}

	//char treatment (char + char) = char unico caso
	if(strcmp(lowercase(tipo1),"char")==0 && strcmp(lowercase(tipo2),"char")==0){
		return "char";
	}
	//char+int char+double char+short
	if(strcmp(lowercase(tipo1),"char")==0) return tipo2;
	//int+char double+char short+char
	if(strcmp(lowercase(tipo2),"char")==0) return tipo1;

	//double treatment

	if(strcmp(lowercase(tipo1),"double")==0 || strcmp(lowercase(tipo2),"double")==0) return "double";

	//short treatment (short +short)
	if(strcmp(lowercase(tipo1),"short")==0 && strcmp(lowercase(tipo2),"short")==0) return "short";

	//short+char=short
	if((strcmp(lowercase(tipo1),"short")==0 && strcmp(lowercase(tipo2),"char")==0)  || (strcmp(lowercase(tipo2),"short")==0 && strcmp(lowercase(tipo1),"char")==0)) return "short";




	//short+int (feito no int)
	//short +double (feito no double)

	return "undef"; //nunca chega



}











//first node is program
char* Genisis_Table(No* node,table* tabela,element*last){
	if(node==NULL){
		return NULL;
	}
	if(tabela!=NULL)
		//printf("sou %s o da tabela %s\n",node->tipo,tabela->nome);
	//printf("sou o %s\n",node->tipo);
	if(strcmp(node->tipo,"Program")==0){
		//printf("entri\n");
		checkprogram(node,tabela);
	}

	if(strcmp(node->tipo,"FuncDeclaration")==0){
		//printf("entri\n");
		checkfuncdeclaration(node,tabela);
	}
	else if(strcmp(node->tipo,"Declaration")==0){
		//printf("entri\n");
		checkdeclaration(node,tabela);
	}
	else if(strcmp(node->tipo,"FuncDefinition")==0){
		//printf("entri\n");
		checkfuncdefinition(node,tabela);
	}
	else if(strcmp(node->tipo,"ParamList")==0){
		//printf("entri\n");
		checkparamlist(node,tabela,last);
	}
	else if(strcmp(node->tipo,"ParamDeclaration")==0){
		//printf("entri\n");
		checkparamdeclaration(node,last);
	}
	else if(strcmp(node->tipo,"FuncBody")==0){
		//printf("entri\n");
		checkfuncbody(node,tabela);
	}
	else if(strcmp(node->tipo,"Return")==0){
		//printf("entri\n");
		checkreturn(node,tabela);
	}
	else if(strcmp(node->tipo,"While")==0){
		//printf("entri\n");
		checkwhile(node,tabela);
	}
	else if(strcmp(node->tipo,"If")==0){
		//printf("entri\n");
		checkif(node,tabela);
	}
	else if(strcmp(node->tipo,"Else")==0){
		//printf("entri\n");
		checkelse(node,tabela);
	}
	else if(strcmp(node->tipo,"StatList")==0){
		//printf("entri\n");
		checkstatlist(node,tabela);
	}
	else if(strcmp(node->tipo,"Id")==0){
		//printf("entri\n");
		char*tipo=strdup(checkid(node,tabela));
		add_sem(node,tipo);
		return tipo;
	}

	else if(strcmp(lowercase(node->tipo),"intlit")==0 || strcmp(lowercase(node->tipo),"chrlit")==0){
		//printf("entri\n");
		char*tipo="int";
		add_sem(node,tipo);

		return tipo;
	}
	else if(strcmp(lowercase(node->tipo),"reallit")==0){
		//printf("entri\n");
		char*tipo="double";
		add_sem(node,tipo);
		return tipo;
	}

	else if(strcmp(node->tipo,"Add")==0 || strcmp(node->tipo,"Sub")==0 || strcmp(node->tipo,"Comma")==0 || strcmp(node->tipo,"Store")==0 || strcmp(node->tipo,"Mul")==0 || 			    	strcmp(node->tipo,"Div")==0 ){
		if((strcmp(node->tipo,"Store")==0 && strcmp(lowercase(node->son->tipo),"intlit")==0 ) || (strcmp(node->tipo,"Store")==0 && strcmp(lowercase(node->son->tipo),"reallit")==0)  || (strcmp(node->tipo,"Store")==0 && strcmp(lowercase(node->son->tipo),"chrlit")==0)) printf("Line %d, col %d: Lvalue required\n",node->son->linha,node->son->coluna);
		//printf("entri\n");
		char*tipo=strdup(checkop(node,tabela));

		add_sem(node,tipo);
		return tipo;
	}
	else if(strcmp(node->tipo,"Or")==0 || strcmp(node->tipo,"And")==0 || strcmp(node->tipo,"BitWiseAnd")==0 || strcmp(node->tipo,"BitWiseOr")==0 || strcmp(node->tipo,"BitWiseXor")==0 || 			strcmp(node->tipo,"Eq")==0 || strcmp(node->tipo,"Ne")==0 || strcmp(node->tipo,"Le")==0 || strcmp(node->tipo,"Ge")==0 || strcmp(node->tipo,"Lt")==0 || strcmp(node->tipo,"Gt")==0 || strcmp(node->tipo,"Mod")==0){

		//printf("entri\n");
		char*tipo=strdup(checkop3(node,tabela));
		add_sem(node,tipo);

		return tipo;
	}
	else if(strcmp(node->tipo,"Plus")==0 || strcmp(node->tipo,"Minus")==0 || strcmp(node->tipo,"Not")==0 ){
		//printf("entri\n");
		char*tipo=strdup(checkop2(node,tabela));
		add_sem(node,tipo);
		return tipo;
	}
	else if(strcmp(node->tipo,"Call")==0){
		//printf("entri\n");
		char*tipo=strdup(checkcall(node,tabela));
		add_sem(node,tipo);
		return tipo;
	}
	else if(strcmp(node->tipo,"Null")==0){
		//printf("entri\n");
		checknull(node,tabela);

	}

	else{
		Genisis_Table(node->brosky,tabela,last);

		}

	return "null";

}


void checknull(No* null, table* tabela){
	No*null_son=null->son;
	No* aux=null_son;
	while(aux!=NULL){
		Genisis_Table(aux,tabela,NULL);
		aux=aux->brosky;
	}



}


char* checkcall(No* call,table* tabela){

	char*array=malloc(sizeof(char)*2000);
	//vamos procurar a tabela correspondente
	element* dec=devolve_tabela(call);
	int num_args=get_num_args(call);
	int defined=0;
	if(dec==NULL && (strcmp(lowercase(call->son->tipo),"getchar")!=0 ||  strcmp(lowercase(call->son->tipo),"putchar")!=0 )){

		No* son=call->son;

		call_errors(dec,call,tabela);
		active=1;


		while(son){
			Genisis_Table(son,tabela,NULL);
			son=son->brosky;
		}
		if(num_args!=0)
			printf("Line %d, col %d: Wrong number of arguments to function %s (got %d, required %d)\n",call->son->linha,call->son->coluna,call->son->valor,num_args,defined);


		if(procura_tabela(call->son,tabela)==NULL)
			return "undef";
		else return procura_tabela(call->son,tabela)->nome;
	}
	//para vermos se o numero de argumentos bate certo

	call_errors(dec,call,tabela);

	active=1;
	//vamos ao no return buscar a lista de parametros (tabela->declarations, porque o return é o primeiro)
	//o nosso dec vai ser um element com um nome(void,int,short,...) e um tipo(nome da funcao) e vai ter uma lista de parametros
	element* aux=dec;
	//isto é o id, ou seja o nome da funcao
	No*call_son=call->son;

	//adiciona esta propriedade ao no call
	add_sem(call,aux->nome);
	 //adicionar ao no nome da func
	strcpy(array,aux->nome);
	strcat(array,"(");
	No* call_son_save=call->son;
	//printf("aqui\n");
	//elemento a seguir ao id (primeiro argumento)
	call_son=call_son->brosky;

	element* parameters=aux->parameters;



	while(parameters!=NULL){

		if(parameters->next==NULL){

			strcat(array,parameters->nome);
			strcat(array,")");
		}

		else{

			strcat(array,parameters->nome);
			strcat(array,",");

			}

		parameters=parameters->next;
		if(call_son!=NULL)
			call_son=call_son->brosky;
	}

	add_sem(call_son_save,array);
	//parametros da funcao

	No* son=call->son->brosky;

	while(son){
		active=1;

		Genisis_Table(son,tabela,NULL);
		son=son->brosky;
	}


	active=0;
	return aux->nome;





}






char *checkop3(No* node,table*tabela){

	No* son1=node->son;
	No* son2=node->son->brosky;


	//erro por exemplo operator + cant be applied to types int,int(void)
	//ver se o filho é um id de uma função






	char *tipo1=strdup(Genisis_Table(son1,tabela,NULL));



	char *tipo2=strdup(Genisis_Table(son2,tabela,NULL));

	char*erro=strdup(invalid_operator(node,son1,son2));
	if(strcmp(erro,"undef")==0) return "undef";



	//falta fazer a checkcompatibility

	compatibility_error(node,tipo1,tipo2);

	tipo2="int";


	return tipo2;


}


char *checkop2(No* node,table*tabela){

	No* son1=node->son;
	char *tipo1=strdup(Genisis_Table(son1,tabela,NULL));



	compatibility_error(node,tipo1,"null");


	strdup(invalid_operator(node,son1,NULL));



	//se for um no Not é sempre int
	if(strcmp(node->tipo,"Not")==0){
		tipo1=strdup("int");
	}

	return tipo1;


}









char *checkop(No* add,table*tabela){

	No* son1=add->son;
	No* son2=add->son->brosky;
	char *tipo1=strdup(Genisis_Table(son1,tabela,NULL));


	char *tipo2=strdup(Genisis_Table(son2,tabela,NULL));



	//falta fazer a checkcompatibility ver isto melhor

	if(strcmp(lowercase(add->tipo),"comma")!=0)
	compatibility_error(add,tipo1,tipo2);
	if(strcmp(lowercase(add->tipo),"comma")!=0)
		tipo2=strdup(return_type(tipo1,tipo2));

	//se algum deles for undef o tipo do check op tambem vai ser undef



	if(strcmp(lowercase(add->tipo),"store")==0){
		tipo2=strdup(tipo1);

	}
	char*erro=strdup(invalid_operator(add,son1,son2));

	if(strcmp(erro,"undef")==0) return "undef";

	return tipo2;


}


char* checkid(No* id,table*tabela){

	char *c_id=(char *)malloc(1000);
	//procuramos o id na tabela global e na tabela local
	//devolvemos um ponteiro para a tabela se esse elemento existir
	//printf("id: %s act:%d\n",id->valor,active);
	element* find=procura_tabela(id,tabela);
	if(find!=NULL){//se ele encontrar esse elemento na tabela
		char* tipo=strdup(find->nome); //o tipo vai ser o tipo da tabela
		element*aux=find->parameters;
		if(aux!=NULL){
			strcat(c_id,tipo);
			strcat(c_id,"(");
			while(aux!=NULL){
				strcat(c_id,aux->nome);
				if(aux->next==NULL){

					strcat(c_id,")");

				}
				else{
					strcat(c_id,",");
				}

				aux=aux->next;
			}
		active=0;
		decl=0;

		return c_id;

		}
		else{

		//printf("tipo do ID:: 	%s\n",find->nome);
		active=0;
		decl=0;
		return tipo;
		}
	}
	else{

		//printf("active %d\n",active);
		if(procura_declaration(id->valor)==0 && active==0 && decl==0){

			printf("Line %d, col %d: Unknown symbol %s\n",id->linha,id->coluna,id->valor);
		}
		active=0;
		decl=0;
		return "undef" ;
	}
}

char* checkStore(No* store,table*tabela){ //store tem sempre dois filhos
	No* son1=store->son;
	No* son2=store->son->brosky;
	strdup(Genisis_Table(son1,tabela,NULL));

	char *tipo2=strdup(Genisis_Table(son2,tabela,NULL));


	//meter no no da arvore
	return tipo2;


}



//o if tem sempre um filho que é expr(temos de ver se este filho é undef, se for msg de erro)
void checkif(No* node,table*tabela){
	No* if_expr=node->son;
	//vamos ver se o que esta dentro do if(.) é uma expr válida

	char* tipo=strdup(Genisis_Table(if_expr,tabela,NULL));

	if_while_error(if_expr,tipo);// aqui dá a msg de erro

	No*if_son=node->son->brosky;
	No* aux=if_son;
	while(aux!=NULL){

		Genisis_Table(aux,tabela,NULL);
		aux=aux->brosky;
	}



}

void checkstatlist(No* node,table*tabela){
	No*stat_son=node->son;
	No* aux=stat_son;
	while(aux!=NULL){
		Genisis_Table(aux,tabela,NULL);
		aux=aux->brosky;
	}



}


void checkelse(No* node,table*tabela){
	No*else_son=node->son;
	No* aux=else_son;
	while(aux!=NULL){
		Genisis_Table(aux,tabela,NULL);
		aux=aux->brosky;
	}



}

void checkwhile(No* node,table*tabela){
	No* while_expr=node->son;
	//vamos ver se o que esta dentro do while(.) é uma expr válida
	char* tipo=strdup(Genisis_Table(while_expr,tabela,NULL));
	if_while_error(while_expr,tipo);// aqui dá a msg de erro

	No*while_son=node->son->brosky;
	No* aux=while_son;
	while(aux!=NULL){
		Genisis_Table(aux,tabela,NULL);
		aux=aux->brosky;
	}



}


void checkreturn(No* node,table*tabela){
	No*return_son=node->son;
	//precisamos do tipo do filho do return para ver se coincide com o tipo da funcção
	char *tipo=strdup(Genisis_Table(return_son,tabela,NULL));//aqui temos o tipo do filho do return
	return_error(node,tipo,tabela);



	No* aux=return_son->brosky;


	while(aux!=NULL){
		Genisis_Table(aux,tabela,NULL);
		aux=aux->brosky;
	}

}


void checkprogram(No* node,table*tabela){
	No* son=node->son;//vai ser funcDeclaration ou funcdefinition ou declaration
	No* aux=son;
	while(aux!=NULL){
		Genisis_Table(aux,tabela,NULL);
		aux=aux->brosky;

	}

}

void checkfuncbody(No*node,table*tabela){
	No*func_son=node->son;
	No* aux=func_son;
	while(aux!=NULL){

		Genisis_Table(aux,tabela,NULL); //DECLARATIONS
		aux=aux->brosky;
	}

	}


void checkfuncdefinition(No* son ,table* tabela){


	No* sons=son->son;//typespec (tipo da funçao)
	element* last;
	element*last2=NULL;

	int erro=0;
	int peso=0;
	table *new_table=NULL;

	//se last 1 for null já inserimos esta definicao ou declaracao de variavel com o mesmo nome
	element* last1=insert_declaration(tabela,sons->tipo,sons->brosky->valor,1);//typespec e id ->last é para sabermos qual foi o ultimo para metermos nesse a lista de parametros

	//vemos se o simbolo já foi definido: se não exisste nenhuma variavel nem nenhuma declaracao com esse nome;
	if(last1==NULL && procura_tabela(sons->brosky,tabela)!=NULL && procura_definition(sons->brosky->valor)!=NULL && procura_definition(sons->brosky->valor)->is_dec==0){

		 printf("Line %d, col %d: Symbol %s already defined\n",sons->brosky->linha,sons->brosky->coluna,sons->brosky->valor);


	}
	int voide=void_declaration_error(son->son->brosky->brosky->son);


	//se for null já foi inserido ou seja vamos inserir nada na tabela global
	if(last1==NULL){

		//Vamos procurar entao o que já temos na tabela para criarmos a tabela nova,aqui o last tem de ter valor
		last=procura_definition(sons->brosky->valor);//procuramos a definicao pode nao existir e ser apenas uma variavel


		if(last!=NULL)
			peso=getpeso(last);

		//vamos ver se os tipos batem certo se baterem certo criamos e inserimos a nova tabela, caso contrario nao fazemos nada
		//vamos dar print dos erros (conflicting types) se a tabela nao tiver sido declarada


			if(last1==NULL && procura_tabela(sons->brosky,tabela)!=NULL && procura_definition(sons->brosky->valor)!=NULL && procura_definition(sons->brosky->valor)->is_dec==0){

			}
			else{

		 	erro=redeclaration_error(sons,sons->brosky->valor);

			}



	}
	else{


		last=last1;
		peso=getpeso(last);

	}



	if(last!=NULL&&  erro==0 && last->is_dec==1 && voide==0 ){
	//printf("ola \n");
	if(last->is_dec==1)
		last->is_dec=0;
	//criamos a tabela da nova funcao
		//ver qual é a posiçao do last na tabela global, vai ser o peso

		new_table=insert_table(tabela,sons->tipo,sons->brosky->valor,peso);

		//element return
		last2=insert_declaration(new_table,sons->tipo,"return",1);




		No* paramlist=sons->brosky->brosky;
		//chamar o paramlist e os seus irmaos (os filhos de funcdefinition)
		No*aux=paramlist;
		while(aux!=NULL){
			//se for paramlist passamos a tabela antiga e a nova
			if(strcmp(aux->tipo,"ParamList")==0){
				//printf("sou paramlist\n");
				if(last1!=NULL){
					Genisis_Table(aux,tabela,last);//para inserir na global
					}
				//printf("passei2\n");
				Genisis_Table(aux,new_table,last2);
				//printf("passei3\n");
			}
			//se nao for paramlist, tem de ser funcbody ai passamos a tabela nova
			else{
				//printf("n sou paramlist\n");
				Genisis_Table(aux,new_table,last);
				}

			aux=aux->brosky;
		}

	 }
	//printf("sai\n");

}


void checkdeclaration(No* declaration,table*table){
	//verifica se nenhuma declaration é void
	//se alguma for da erro e não coloca na tabela global
	//SE ELE TIVER PELO MENOS 3 FILHOS É UMA ATRIBUICAO (p.ex a=b ou a=3+b-2...)
	No* eq=declaration->son->brosky->brosky;
	if(eq!=NULL){
		//decl=1;

		char*tipo=strdup(Genisis_Table(eq,table,NULL)); // tipo do id ou int
		if((strcmp(lowercase(tipo),"double")==0 && strcmp(lowercase(declaration->son->tipo),"double")!=0)){
			printf("Line %d, col %d: Conflicting types (got %s, expected %s)\n",declaration->linha,declaration->coluna,tipo,lowercase(declaration->son->tipo));
		}



	}


	//aqui vemo se a declaracao é void se for nao metemos na tabela
	int voide=void_declaration_error(declaration);

	//aqui vemos se ja foi declarada
	int redeclaration=check_dec_existence(declaration->son->brosky,table);

	//aqui vemos se foi declarada novamente com outro tipo somente se nao tiver sido ja declarada
	procura_tabela(declaration->son->brosky,table);


	//###############################################################################erros########################################################
	if(voide==0 && table!=NULL && redeclaration==1)
		insert_declaration(table,declaration->son->tipo,declaration->son->brosky->valor,1);//vamos colocar uma lista ligada de declaraçoes no elemento do id(nome da funcao)


}

void checkparamdeclaration(No* paramdeclaration,element*last){

	No*aux=paramdeclaration;
	element*procura=NULL;
	element*params=NULL;
	int exist=0;
	if(aux==NULL)return;
	//vamos ver se ja foi declarada essa variavel na globa
	if(last!=NULL){
		if(aux->son->brosky) {//se tiver id
			procura=procura_tabela(paramdeclaration->son->brosky,global); //se for null ainda n existe na global
		//vamos ainda verificar os parametross anteriores
		 params=last->parameters;
		while(params!=NULL){

			if(strcmp(params->tipo,aux->son->brosky->valor)==0) exist=1;
			params=params->next;
		}

		}
	}



	if(last!=NULL&& exist==0 ){
		if(aux->son->brosky) {//se tiver id
			insert_parameters(last,aux->son->tipo,aux->son->brosky->valor);//vamos colocar uma lista ligada de declaraçoes no elemento do id(nome da funcao)
		}
		else{

			insert_parameters(last,aux->son->tipo,"null");
		}
	}

}


void checkparamlist(No*paramlist,table*tabela,element*last){

	//checkar se tem lista de parametros
	if(paramlist!=NULL){

		No* paramlist_son=paramlist->son; //no paramdeclaration
		//vai aos filhos do paramlist podem ser paramdeclaration_declaration...
		No* aux=paramlist_son;
		while(aux!=NULL){

			Genisis_Table(aux,tabela,last);
			aux=aux->brosky;
		}

	}

}


void checkfuncdeclaration(No* son,table* tabela){

	No* sons=son->son;//typespec
	char* funcname=sons->brosky->valor;
	//verifica se nenhuma declaration é void
	//se alguma for da erro e não coloca na tabela global
	No* paramdeclaration=sons->brosky->brosky->son;

	int voide=void_declaration_error(paramdeclaration);


	//vamos ver se a função já foi declarada(se já existe o seu id na tabela global)

	if(voide!=1)
	redeclaration_error(sons,funcname);


	//vamos ver se a sumbolo já foi declarado




	element* last=NULL;

	//aux pode ser NULL, quer dizer que ainda n inserimos nada
	if(voide==0){
		last=insert_declaration(tabela,sons->tipo,sons->brosky->valor,1);//typespec e id ->last é para sabermos qual foi o ultimo para metermos nesse a lista de parametros


		//SE nao tivermos inserido nada, vamos apontar para a cabeça da lista
		if(last==NULL){
			last=tabela->declarations;
		}

		No* funcdecl_son=sons->brosky->brosky;
		No*aux=funcdecl_son;
		while(aux!=NULL){
			Genisis_Table(aux,tabela,last);//vamos chamar a funcao para um possivel paramlist funcbody...

			aux=aux->brosky;
		}
	}

}

void insert_parameters(element*id,char* nome,char* tipo){
	element* elemento=NULL;
	if(tipo==NULL)
		elemento=cria_elemento(lowercase(nome),"null");
	else
		elemento=cria_elemento(lowercase(nome),tipo);
	element*aux=id->parameters;

	if(aux==NULL){

		id->parameters=elemento;

	}
	else{
		while(aux->next!=NULL) aux=aux->next;
		aux->next=elemento;

	}
}
