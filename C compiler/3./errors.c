
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


extern table* global;
extern int active;
//devolve 1 se encontrar
int procura_symbol(No* id,table*tabela){
	//printf("###############procura table##################\n");
	//primeiro procuramos na tabela local
	element*aux=tabela->declarations;
	element *params=NULL;
	if(strcmp(lowercase(tabela->nome),"global")!=0){
		params=aux->parameters; // se for funcao estes vao ser os parametros

		while(aux!=NULL){

			//vamos aos parametros da funcao
			if(params!=NULL){

				while(params!=NULL){

					if(strcmp(params->tipo,id->valor)==0) return 1;
					params=params->next;
				}

			}


			//aux->tipo nome da variavel  id->valor nome da variavel
			//printf("tipo: %s id%s\n",aux->tipo,id->valor);
			if(strcmp(aux->tipo,id->valor)==0){
				return 1;
			}



			aux=aux->next;


		}
		}
	return 0;
}


int get_num_args(No* call){
	//primeiro temos sempre o filho id
	No* id=call->son;
	//depois vamos contar quantos filhos tem a mais(estes vão ser os argumentos)
	int conta=0;
	No*aux=id->brosky;
	while(aux!=NULL && strcmp(lowercase(aux->tipo),"void")!=0){
		conta++;
		aux=aux->brosky;
	}
return conta;


}

//chama esta função se existir a tabela da chamada da funçao
//o element* dec aponta para a declaração que queremos da tabela global, só temos de percorrer os parametros

int get_num_args_defined(element* dec){
	int conta=0;
	//printf("args defined com %s\n",dec->tipo);
	element* parameters=dec->parameters;
	while(parameters!=NULL){
		if(strcmp(lowercase(parameters->nome),"void")!=0)
			conta++;
		parameters=parameters->next;

	}
return conta;

}

//devolve 1 se NÃO existir,caso contrario é uma redifinicao
int check_dec_existence(No* dec_id,table* tabela){
	//vamos ver se esse simbolo já existe:
	//procura alguma definicao

	int one= procura_symbol(dec_id,tabela);



	if(one==0){
		return 1;
	}
	printf("Line %d, col %d: Symbol %s already defined\n",dec_id->linha,dec_id->coluna,dec_id->valor);
	return 0;

}


int compare_declarations(No* node,element* declaration){


	//percorrer os parametros

	element*parameters=declaration->parameters;
	if(parameters!=NULL){
	No* id=node->brosky;
	No* paramlist=id->brosky;

	No* paramdeclaration=paramlist->son;

	while(paramdeclaration!=NULL){

		//SE algum n for igual devolve 0=false
		if(strcmp(lowercase(paramdeclaration->son->tipo),lowercase(parameters->nome))!=0){
			return 0;
		}
		if(parameters->next==NULL && paramdeclaration->brosky!=NULL){
			//NUMERO incoerente de parametros devolve 0=false
			return 0;
		}
		parameters=parameters->next;
		paramdeclaration=paramdeclaration->brosky;
	}

	//verficar se por acaso acabaram as declarations do no, mas existem mais parametros na tabela
	if(parameters!=NULL && paramdeclaration==NULL){
		//printf("este caso\n");
		return 0;
	}
	}
	//se chegar aqui está tudo bem
	return 1;






}


void print_conflicting_types(No* node,element*declaration){



	//printf("######################################Print function##########################\n");
	if(strcmp(lowercase(node->tipo),"null")==0){
		printf("linha: %s\n",node->valor);
	}
	//printf("sou o %s\n",node->tipo);

	printf("Line %d, col %d: Conflicting types (got %s(",node->brosky->linha,node->brosky->coluna,lowercase(node->tipo));
	No* id=node->brosky;
	No* paramlist=id->brosky;
	No* paramdeclaration=paramlist->son;

	while(paramdeclaration!=NULL){
		//se for o ultimo
		if(paramdeclaration->brosky==NULL)
			printf("%s),",lowercase(paramdeclaration->son->tipo));
		else{
			printf("%s,",lowercase(paramdeclaration->son->tipo));
		}
		paramdeclaration=paramdeclaration->brosky;

	}

	element* parameters= declaration->parameters;
	if(parameters==NULL) printf(" expected %s)\n",lowercase(declaration->nome));
	else{
	printf(" expected %s(",lowercase(declaration->nome));

	while(parameters!=NULL){
		if(parameters->next==NULL)
			printf("%s))\n",lowercase(parameters->nome));
		else{
			printf("%s,",lowercase(parameters->nome));
		}
		parameters=parameters->next;
	}

	}
	//printf("######################################Print function##########################\n");



}

//check if types are confliting
int redeclaration_error(No* node,char *nome){

	//procura na global algum nome igual
	element* declaration=procura_definition(nome);

	//SE for null ainda n foi inserido
	// se for !=null já lá está declarado

	if(declaration==NULL){

		declaration=procura_tabela(node->brosky,global);
	}

	if(declaration!=NULL && declaration->parameters==NULL){

	print_conflicting_types(node,declaration);
	}


	else if(declaration!=NULL && declaration->parameters!=NULL){

		//vamos ver se há conflito entre os tipos;

		if(!compare_declarations(node,declaration)){


			print_conflicting_types(node,declaration);
			return 1;
		}
	}


	return 0;
}

void return_error(No* node,char* tipo,table *tabela){
	//1º elemento tabela é o return
	//se nao bater certo

	No* son=node->son;
	char* tipo2;
	if(son==NULL) tipo2=strdup("null");
	else if(node->son->sem==NULL) tipo2=strdup("void");
	else{
		tipo2=strdup(node->son->sem);

	}
	tipo=tabela->declarations->nome;

	if((strcmp(tipo2,"double")==0 && strcmp(tipo,"double")!=0) || ((strcmp(tipo2,"void")==0 && strcmp(tipo,"void")!=0 )|| ((strcmp(tipo2,"void")!=0 && strcmp(tipo,"void")==0 )|| strcmp(tipo,"undef")==0) || strcmp(tipo2,"undef")==0) ){
		//printf("aqui\n");
		if(strcmp(tipo,"null")==0){
			printf("Line %d, col %d: Conflicting types (got %s, expected %s)\n",node->son->linha,node->son->coluna,tipo2,tabela->declarations->nome);
		}
		else
			printf("Line %d, col %d: Conflicting types (got %s, expected %s)\n",node->son->linha,node->son->coluna,tipo2,tabela->declarations->nome);
	}
	//printf("aqu2i\n");

}





//devolve 1 se já existir a declaração na tabela global, ou seja não vamos inserir a definition
element* insert_declaration_error(char*nome,table*tabela){

	//Caso primeira inserçao
	if(global==NULL) return NULL;
	element* aux=tabela->declarations;

	while(aux!=NULL){

		if(strcmp(aux->tipo,nome)==0){
			return aux;
		}

		aux=aux->next;

	}
	return NULL;

}

//devolve 1 se der erro
int void_declaration_error(No* declaration){
	//O No recebido pode ser declaration ou paramdeclaration
	//Se for paramdeclaration percorremos logo todos os irmaos, pq ele só entra aqui uma vez
	//Se for declaration, como ele entra aqui para cada declaration, fazemos só o caso atual
	//printf("####################################################################################\n");


	if(strcmp(lowercase(declaration->tipo),"declaration")==0){
		if(strcmp(lowercase(declaration->son->tipo),"void")==0){

			//Invalid use of void type in declaration
			printf("Line %d, col %d: Invalid use of void type in declaration\n",declaration->son->brosky->linha,declaration->son->brosky->coluna);
			return 1;

		}



	}
	else{
		//aqui estamos nos paramdeclaration
		No* aux= declaration;

		//PERCORREMOS OS DECLARATIONS E VEMOS SE EXISTE ALGUM VOID COM ID
		int i=1;
		while(aux!=NULL){

				if((strcmp(lowercase(aux->son->tipo),"void")==0 && i!=1) || (strcmp(lowercase(aux->son->tipo),"void")==0 && i==1 && aux->son->brosky!=NULL) ||(strcmp(lowercase(aux->son->tipo),"void")==0 && aux->brosky!=NULL && strcmp(lowercase(aux->brosky->tipo),"paramdeclaration")==0)){

					//Invalid use of void type in declaration
					printf("Line %d, col %d: Invalid use of void type in declaration\n",aux->son->linha,aux->son->coluna);
					return 1;


				}
			i++;

			aux=aux->brosky;

		}
	}


	//printf("####################################################################################\n");
	return 0;
}


void if_while_error(No* node,char *tipo){
	//printf("sou o node %s\n",node->tipo);
	if(strcmp(lowercase(tipo),"undef")==0 || strcmp(lowercase(tipo),"double")==0){
		printf("Line %d, col %d: Conflicting types (got %s, expected int)\n",node->linha,node->coluna,tipo);
	}



}

void call_errors(element* dec,No*call,table* tabela){
	int defined=0;
	//vamos ver quantos argumentos recebeu a chamada da função
	int num_args=get_num_args(call);

	//se dec for null então essa tabela não existe na tabela global(nao foi declarada)
	if(dec==NULL){
		defined=0;

		//ja sabemos que o nome da funcao nao existe, mas pode haver outro simbolo (declaracao)
		if(procura_tabela(call->son,tabela)==NULL){

			printf("Line %d, col %d: Unknown symbol %s\n",call->son->linha,call->son->coluna,call->son->valor);
			}
		else if(procura_declaration(call->son->valor)==1){

		 printf("Line %d, col %d: Symbol %s is not a function\n",call->son->linha,call->son->coluna,call->son->valor);

		}
	}
	else{


		//percorremos os parametros todos para ver se algum é undefined
		No* per=call->son->brosky;
		active=1;
		if(per!=NULL){ // SE TIVER ARGUMENTOS



			while(per!=NULL){

				if(strcmp(lowercase(per->tipo),"id")==0){

					if(procura_tabela(per,tabela)==NULL){
					active=1;
					 printf("Line %d, col %d: Unknown symbol %s\n",per->linha,per->coluna,per->valor);
					}

				}
				per=per->brosky;

			}
		}







		//caso contrário vamos ver se o número de argumentos corresponde
		defined=get_num_args_defined(dec);
		No*aux=call->son->brosky;


	element* aux2=dec->parameters;


	if(num_args==defined){

	//AUX->NUM_ARGS  AUX2->DEFINED
	while(aux!=NULL){
		active=1;
		char* tipo=Genisis_Table(aux,tabela,NULL);
		active=0;

			 if ((strcmp(aux2->nome,"double")!=0 && strcmp(tipo,"double")==0) || (strcmp(lowercase(aux2->nome),"void")==0 || strcmp(lowercase(tipo),"void")==0)      || (strcmp(lowercase(aux2->nome),"undef")==0 || strcmp(lowercase(tipo),"undef")==0)){

				if(strcmp(lowercase(aux->tipo),"call")==0){
					printf("Line %d, col %d: Conflicting types (got %s, expected %s)\n",aux->son->linha,aux->son->coluna,tipo,aux2->nome);

				}
				else
					printf("Line %d, col %d: Conflicting types (got %s, expected %s)\n",aux->linha,aux->coluna,tipo,aux2->nome);
			}

			if(aux2->next!=NULL)aux2=aux2->next;
		aux=aux->brosky;

	}
	}





	if(defined!=num_args)//se nao coincidirem da erro
				printf("Line %d, col %d: Wrong number of arguments to function %s (got %d, required %d)\n",call->son->linha,call->son->coluna,dec->tipo,num_args,defined);


}
}

//devolve 0 se forem compativeis
//devolve 1 se forem imcompativeis
int compatibility_error(No* no,char* tipo1,char* tipo2){

		if(strcmp(lowercase(no->tipo),"intlit")!=0 && strcmp(lowercase(no->tipo),"reallit")!=0   && strcmp(lowercase(no->tipo),"chrlit")!=0  && strcmp(lowercase(no->tipo),"id")!=0){

			char* token;


	    		if(strcmp(no->tipo,"Store")==0)
	      			token=strdup("=");
	      		else if(strcmp(no->tipo,"Comma")==0)
				token=strdup(",");
			else if(strcmp(no->tipo,"Add")==0)
				token=strdup("+");
			else if(strcmp(no->tipo,"Sub")==0)
	      			token=strdup("-");
	      		else if(strcmp(no->tipo,"Mul")==0)
				token=strdup("*");
			else if(strcmp(no->tipo,"Div")==0)
	      			token=strdup("/");
	      		else if(strcmp(no->tipo,"Mod")==0)
				token=strdup("%");
			else if(strcmp(no->tipo,"Or")==0)
	      			token=strdup("||");
	      		else if(strcmp(no->tipo,"And")==0)
				token=strdup("&&");
			else if(strcmp(no->tipo,"BitWiseAnd")==0)
	      			token=strdup("&");
	      		else if(strcmp(no->tipo,"BitWiseOr")==0)
				token=strdup("|");
			else if(strcmp(no->tipo,"BitWiseXor")==0)
	      			token=strdup("^");
	      		else if(strcmp(no->tipo,"Eq")==0)
				token=strdup("==");
			else if(strcmp(no->tipo,"Ne")==0)
	      			token=strdup("!=");
	      		else if(strcmp(no->tipo,"Lt")==0){
				token=strdup("<");

		}
			else if(strcmp(no->tipo,"Gt")==0)
	      			token=strdup(">");
	      		else if(strcmp(no->tipo,"Plus")==0)
				token=strdup("+");
			else if(strcmp(no->tipo,"Minus")==0)
	      			token=strdup("-");
	      		else if(strcmp(no->tipo,"Not")==0)
				token=strdup("!");
			else if(strcmp(no->tipo,"Ge")==0)
				token=strdup(">=");
			else if(strcmp(no->tipo,"Le")==0)
				token=strdup("<=");
			else{
				token=strdup(no->tipo);
			}





		if((strcmp(lowercase(tipo1),"undef")==0 || strcmp(lowercase(tipo2),"undef")==0) && ( strcmp(lowercase(token),"||")==0|| strcmp(lowercase(token),"&&")==0|| strcmp(lowercase(token),"&")==0|| strcmp(lowercase(token),"|")==0|| strcmp(lowercase(token),"^")==0|| strcmp(lowercase(token),"!=")==0|| strcmp(lowercase(token),"<=")==0|| strcmp(lowercase(token),">=")==0|| strcmp(lowercase(token),"<")==0|| strcmp(lowercase(token),">")==0|| strcmp(lowercase(token),"%")==0)){

		printf("Line %d, col %d: Operator %s cannot be applied to types %s, %s\n",no->linha,no->coluna,token,tipo1,tipo2);
		return 1;
		}
		else if((strcmp(lowercase(tipo1),"undef")==0 && strcmp(lowercase(tipo2),"undef")==0) && strcmp(lowercase(token),"==")==0) return 0;


		if(strcmp(lowercase(tipo1),"undef")==0 || strcmp(lowercase(tipo2),"undef")==0 || strcmp(lowercase(tipo1),"void")==0 || strcmp(lowercase(tipo2),"void")==0){
			if(strcmp(tipo2,"null")!=0)
				printf("Line %d, col %d: Operator %s cannot be applied to types %s, %s\n",no->linha,no->coluna,token,tipo1,tipo2);
			else
				printf("Line %d, col %d: Operator %s cannot be applied to type %s\n",no->linha,no->coluna,token,tipo1);


			return 1;
		}
		//printf("aqui2\n");

		//incompativeis
		//= nao pode ser x=double, a nao ser que x seja double
		if(strcmp(lowercase(token),"=")==0){
			if(strcmp(lowercase(tipo2),"double")==0 && strcmp(lowercase(tipo1),"double")!=0){
				printf("Line %d, col %d: Operator = cannot be applied to types %s, %s\n",no->linha,no->coluna,tipo1,tipo2);
				return 1;
			}
		}
		else if(strcmp(lowercase(token),"%")==0 || strcmp(lowercase(token),"||")==0 || strcmp(lowercase(token),"&&")==0 || strcmp(lowercase(token),"&")==0 || strcmp(lowercase(token),"|")==0 || strcmp(lowercase(token),"^")==0){

			if (strcmp(lowercase(token),"^")==0 && strcmp(lowercase(tipo1),"double")==0 && strcmp(lowercase(tipo2),"double")!=0){
				printf("Line %d, col %d: Operator %s cannot be applied to types %s, %s\n",no->linha,no->coluna,token,tipo1,tipo2);
				return 1;

			}
			else if(strcmp(lowercase(tipo2),"double")==0 ){

				printf("Line %d, col %d: Operator %s cannot be applied to types %s, double\n",no->linha,no->coluna,token,tipo1);
				return 1;

			}
		}


	}

	return 0;



}


char * convert_(No* no){

	char* token=NULL;


	if(strcmp(no->tipo,"Store")==0)
		token=strdup("=");
	else if(strcmp(no->tipo,"Comma")==0)
		token=strdup(",");
	else if(strcmp(no->tipo,"Add")==0)
		token=strdup("+");
	else if(strcmp(no->tipo,"Sub")==0)
		token=strdup("-");
	else if(strcmp(no->tipo,"Mul")==0)
		token=strdup("*");
	else if(strcmp(no->tipo,"Div")==0)
		token=strdup("/");
	else if(strcmp(no->tipo,"Mod")==0)
		token=strdup("%");
	else if(strcmp(no->tipo,"Or")==0)
		token=strdup("||");
	else if(strcmp(no->tipo,"And")==0)
		token=strdup("&&");
	else if(strcmp(no->tipo,"BitWiseAnd")==0)
		token=strdup("&");
	else if(strcmp(no->tipo,"BitWiseOr")==0)
		token=strdup("|");
	else if(strcmp(no->tipo,"BitWiseXor")==0)
		token=strdup("^");
	else if(strcmp(no->tipo,"Eq")==0)
		token=strdup("==");
	else if(strcmp(no->tipo,"Ne")==0)
		token=strdup("!=");
	else if(strcmp(no->tipo,"Lt")==0){
		token=strdup("<");

}
	else if(strcmp(no->tipo,"Gt")==0)
		token=strdup(">");
	else if(strcmp(no->tipo,"Plus")==0)
		token=strdup("+");
	else if(strcmp(no->tipo,"Minus")==0)
		token=strdup("-");
	else if(strcmp(no->tipo,"Not")==0)
		token=strdup("!");
	else if(strcmp(no->tipo,"Ge")==0)
		token=strdup(">=");
	else if(strcmp(no->tipo,"Le")==0)
		token=strdup("<=");


return token;

}



char* invalid_operator(No* node,No* son1,No* son2){
	//passamos por exemplo add para +
	char* convert=convert_(node);
	//caso do operador só com o primeiro filho

	if(son2==NULL){

		if(strcmp(lowercase(son1->tipo),"id")==0){
			element* id=procura_tabela(son1,global);
			if(id!=NULL && id->parameters!=NULL){
			printf("Line %d, col %d: Operator %s cannot be applied to types %s(",node->linha,node->coluna,convert,lowercase(id->nome));
				element*aux=id->parameters;
				while(aux!=NULL){
					if(aux->next==NULL) printf("%s)\n",lowercase(aux->nome));
					else printf("%s,",aux->nome);
					aux=aux->next;
				}

			}
		return "undef";
		}

	return "clear";
	}











	//se o filho 1 for id e o segundo também
	if(strcmp(lowercase(son1->tipo),"id")==0 && strcmp(lowercase(son2->tipo),"id")==0 ){

		//procuramos na tabela global por este id
		element* id=procura_tabela(son1,global);
		element* id2=procura_tabela(son2,global);
		//se o primeiro tiver parâmetros e o segundo não
		if(id!=NULL && id->parameters!=NULL && id2!=NULL && id2->parameters==NULL){

			printf("Line %d, col %d: Operator %s cannot be applied to types %s(",node->linha,node->coluna,convert,lowercase(id->nome));
			element*aux=id->parameters;
			while(aux!=NULL){
				if(aux->next==NULL) printf("%s), %s\n",lowercase(aux->nome),lowercase(son2->sem));
				else printf("%s,",aux->nome);
				aux=aux->next;
			}

		return "undef";

		}
		//se o primeiro não tiver parâmetros e o segundo tiver
		else if(id!=NULL && id->parameters==NULL && id2!=NULL && id2->parameters!=NULL){

			printf("Line %d, col %d: Operator %s cannot be applied to types %s, %s(",node->linha,node->coluna,convert,lowercase(son1->sem),lowercase(id2->nome));
			element*aux=id2->parameters;
			while(aux!=NULL){
				if(aux->next==NULL) printf("%s)\n",lowercase(aux->nome));
				else printf("%s,",aux->nome);
				aux=aux->next;
			}

		return "undef";
		}
		//se o ambos tiverem parametros
		else if(id!=NULL && id->parameters!=NULL && id2!=NULL && id2->parameters!=NULL){

			printf("Line %d, col %d: Operator %s cannot be applied to types %s(",node->linha,node->coluna,convert,lowercase(id->nome));


		element*aux=id->parameters;
		while(aux!=NULL){
			if(aux->next==NULL) printf("%s), %s(",lowercase(aux->nome),lowercase(id2->nome));
			else printf("%s,",aux->nome);
			aux=aux->next;
		}
		element*aux2=id2->parameters;
		while(aux2!=NULL){
			if(aux2->next==NULL) printf("%s)\n",lowercase(aux2->nome));
			else printf("%s,",aux2->nome);
			aux2=aux2->next;
		}

		return "undef";
		}


	}


	//se o filho 1 for id e o segundo não


	if(strcmp(lowercase(son1->tipo),"id")==0 && strcmp(lowercase(son2->tipo),"id")!=0 ){

		element* id=procura_tabela(son1,global);


		if(id!=NULL && id->parameters!=NULL){
			printf("Line %d, col %d: Operator %s cannot be applied to types %s(",node->linha,node->coluna,convert,lowercase(id->nome));
			element*aux=id->parameters;
			while(aux!=NULL){
				if(aux->next==NULL) printf("%s), %s\n",lowercase(aux->nome),lowercase(son2->sem));
				else printf("%s,",aux->nome);
				aux=aux->next;
			}

		return "undef";

		}

	}
	//se o filho 2 for id e o primeiro não

	//se o primeiro não tiver parâmetros e o segundo tiver

		else if(strcmp(lowercase(son1->tipo),"id")!=0 && strcmp(lowercase(son2->tipo),"id")==0 ){

			 element* id2=procura_tabela(son2,global);
			 if(id2!=NULL && id2->parameters!=NULL){

				printf("Line %d, col %d: Operator %s cannot be applied to types %s, %s(",node->linha,node->coluna,convert,lowercase(son1->sem),lowercase(id2->nome));
				element*aux=id2->parameters;
				while(aux!=NULL){
					if(aux->next==NULL) printf("%s)\n",lowercase(aux->nome));
					else printf("%s,",aux->nome);
					aux=aux->next;
				}

			return "undef";
			}

		}



	return "clear";



}
