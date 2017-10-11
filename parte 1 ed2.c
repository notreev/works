/******************************************************************************************************
 *	Trabalho de Estrutura de Dados II                                                                 *
 *	Integrantes do Grupo: Everton de M. Camacho, Gustavo F. Balbino, Vinícius B. Vechiato, Eric Rocha *
 *	Data de entrega: 27/09/2017                                                                       *
*******************************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>


//Variavel global
unsigned int idPagina = 0;

typedef struct matricula {
	unsigned int num;
	char nome[100];
	int idade;
	char sexo;
	struct matricula* prox;
} MATRICULA;

typedef struct hashTable {
	MATRICULA* matricula;
	int numAlunos;
} HASHTABLE;

int funcaoHash(int num) {
	int n = num % 4;
	return n;
}

typedef struct pagina{
	
	MATRICULA* m1;
	MATRICULA* m2;
	MATRICULA* m3;	
	
	int idPagina;
	int pinCount ;
	int dirty;
	
}PAGINA;

typedef struct pool{
	PAGINA* p1;
	PAGINA* p2;
	PAGINA* p3;
	PAGINA* p4;

	int count ; // verificar se atingiu o limite de 4 páginas por pool
	
}POOL;

typedef struct listaPaginas{
	
	struct listaPaginas* prox;
	PAGINA *pagina;
	
}LISTAPAGINAS;

void insereHash(MATRICULA* mat, HASHTABLE* tabela[]) { 
	
	int n = funcaoHash(mat -> num);
	tabela[n]->numAlunos += 1;

	if (tabela[n]->matricula == NULL) { //Insere no primeira no primeiro nó da lista
		tabela[n] -> matricula = mat;
	}else{ //Percorre a lista que está na posição da tabela[n] para ver onde deve inserir o novo nó
		MATRICULA* aux = tabela[n]->matricula;
		
		while(aux->prox != NULL){
			aux = aux->prox;
		}
		aux->prox = mat; //Insere no fim da lista
	} 

}

MATRICULA* buscarAluno(int num, HASHTABLE* tabela[]) { //função de busca.
	
	int n = funcaoHash(num);
	MATRICULA* aux = tabela[n]->matricula;
	
	while (aux) {
		if (num == aux -> num) break;
		aux = aux -> prox;
	}
	
	return aux;
}

void imprimirHash(HASHTABLE* tabela[]){
	
	int k = 0, e = 0;
	
	puts("\n************************************************************");
	while(k != 4){
		MATRICULA* aux = tabela[k]->matricula;
		
		if(aux == NULL){
			e++;
		}else{
			printf("\nAlunos cadastrados na posição HashTable[%d] = %d\n",k,tabela[k]->numAlunos);
		}
		
		while(aux){
			puts("");
			puts("---------------------------------------------");
			printf("Nome: %s\n", aux->nome);
			printf("Idade: %d\n", aux->idade);
			printf("Sexo: %c\n", aux->sexo);
			printf("Número de matrícula: %d\n", aux->num);
			puts("---------------------------------------------");
			puts("");
			aux = aux->prox;
		}
		k++;
	}
	if(e == k){ 
		printf("\nNão há alunos cadastrados no sistema!\n\n");
	}
	puts("************************************************************\n");
}

void imprimirInfoAluno(MATRICULA* aluno){
	
		puts("\nAluno:");
		puts("---------------------------------------------");
		printf("Nome: %s\n", aluno->nome);
		printf("Idade: %d\n", aluno->idade);
		printf("Sexo: %c\n", aluno->sexo);
		printf("Número de matrícula: %d\n", aluno->num);
		puts("---------------------------------------------");
		puts("\n");
}


void removerAluno(MATRICULA* aluno, HASHTABLE* tabela[]){
	
	int n = funcaoHash(aluno->num);
	tabela[n]->numAlunos -= 1;
	
	MATRICULA* aux = tabela[n]->matricula;
	MATRICULA* ant = NULL;

	while(aux != NULL){ //Se encontrar um aluno prepara os apontamentos para remover...
		if(aux == aluno){
			if(aux == tabela[n]->matricula){ // Se vai remover o primeiro nó
				tabela[n]->matricula = tabela[n]->matricula->prox;
				free(aux);
				return; 
			}else{ // Se vai remover um nó do meio ou do fim
				ant->prox = aux->prox;
				free(aux);
				return;
			}
		}
		else{ // Avançar o aux
			ant = aux;
			aux = aux->prox;
		}
	}
}

MATRICULA* novaMatricula(char nome[], int idade, char sexo, unsigned int numMatricula){
	
	MATRICULA* mat = (MATRICULA*) malloc(sizeof (MATRICULA));
	
	strcpy(mat->nome, nome);
	mat->idade = idade;
	mat->sexo = sexo;
	mat->num = numMatricula;
	
	mat->prox = NULL;
	return mat;
}


void alocarHashTable(HASHTABLE* tabela[]) {
	
	int k = 0;
	
	while(k!=4){
		tabela[k] =  malloc(sizeof (HASHTABLE));
		tabela[k]->numAlunos = 0;	
		tabela[k]->matricula = NULL;
		k++;
	}

}

PAGINA* iniciarPaginas(){
	
	PAGINA* p = (PAGINA*) malloc(sizeof(PAGINA));
	
	p->m1 = NULL;
	p->m2 = NULL;
	p->m3 = NULL;
	
	p->idPagina += 1; 
	
	p->pinCount = 0;
	p->dirty = 0;
	
	return p;
}

POOL* iniciarPool(){
	
	POOL* p = (POOL*) malloc(sizeof(POOL));
		
	p->p1 = NULL;
	p->p2 = NULL;
	p->p3 = NULL;
	p->p4 = NULL;
	
	return p;
}

int validarNumero(int x){
	
	if (x > 0){
		return 1;
	}else{
		return 0;
	}
}


MATRICULA* verificaEspacoNaPagina(LISTAPAGINAS* l, MATRICULA* m){
	LISTAPAGINAS* aux = l->prox;
	
	if(aux == NULL){ //Nao há paginas ainda na lista páginas
		if(aux->pagina->m1 == NULL){ // verifica se há espaço para uma nova matrícula na página em questão
			aux->pagina->m1 = m;
		}
		if(aux->pagina->m2 == NULL){
			aux->pagina->m2 = m;
		}
		if(aux->pagina->m3 == NULL){
		
			aux->pagina->m3 = m;
		}		
	}else{
		while(aux != NULL){ //percorre as páginas 
			if(aux->pagina->m1 == NULL){ // verifica se há espaço para uma nova matrícula na página em questão
				aux->pagina->m1 = m;
			}
			if(aux->pagina->m2 == NULL){
				aux->pagina->m2 = m;
			}
			if(aux->pagina->m3 == NULL){
			
				aux->pagina->m3 = m;
			}
			aux = aux->prox; // se nao retornar verifica nas proximas paginas se há espaco
		} 
	}	
	
	
}



int main() {
	setlocale(LC_ALL, "portuguese");
	int op, idade, validar = 0;
	char nome[100];
	char sexo;
	unsigned int numMatricula;
	
	MATRICULA* mat = NULL;
	MATRICULA* aluno = NULL;

	HASHTABLE* tabela[4];
	alocarHashTable(tabela);
	
	//Inicia as lista de páginas 
	LISTAPAGINAS* listaVazia = (LISTAPAGINAS*) malloc(sizeof(LISTAPAGINAS*));
	listaVazia->prox = NULL;
	
	LISTAPAGINAS* listaOcupada = (LISTAPAGINAS*) malloc(sizeof(LISTAPAGINAS*));
	listaOcupada->prox = NULL;


	//Pool de buffers
	POOL* pool = iniciarPool(pool);	
	
	PAGINA* pagina = (PAGINA*) malloc(sizeof(PAGINA));
	
	puts("------------------------------------------------------------");
	puts("Bem-vindo ao gerenciador de matrículas da Universidade X");
	puts("------------------------------------------------------------\n");
	do {
		validar = 0;
		printf("| ------------------- |\n");
		printf("|         MENU        |\n");
		printf("| 1) Cadastrar aluno  |\n");
		printf("| 2) Remover aluno    |\n");
		printf("| 3) Modificar aluno  |\n");
		printf("| 4) Consultar aluno  |\n");
		printf("| 5) Mostrar alunos   |\n");
		printf("| ------------------- |\n");
		printf("| 0) Sair do programa |\n");
		
		puts("\nInforme a opção desejada:\n");
		scanf("%d", &op);

		
		fflush(stdin);
		switch (op) {
			case 1:
				printf("\nDigite o nome do aluno:\n");
				scanf("%[^\n]", nome);
				fflush(stdin);
				
				while(validar != 1){
					printf("\nDigite a idade:\n");
					scanf("%d", &idade);
					validar = validarNumero(idade);
					if (validar == 0){
						puts("\nInforme uma idade válida! (Maior do que zero)!");
					}					
				}
				validar = 0;
				
				fflush(stdin);
				while(validar != 1){
					printf("\nDigite o sexo (M / F):\n");
					scanf("%c", &sexo);
					fflush(stdin);
					
					if(sexo == 'M' || sexo == 'm' || sexo == 'F' || sexo == 'f'){
						validar = 1;
					}else{
						printf("\nInforme M ou F para o sexo da pessoa!\n");
					}
				}
				validar = 0;
				
				while(validar != 1){
					printf("\nDigite o número da matrícula:\n");
					scanf("%d", &numMatricula);
					validar = validarNumero(numMatricula);
					if (validar == 0){
						puts("\nInforme um número de matrícula válido! (Maior do que zero)!");
					}
				}
				validar = 0;
				
				aluno = buscarAluno(numMatricula, tabela);
				
				if(aluno){
					printf("\nO número de matrícula informado já consta no sistema!\n\n");
				}else{
				mat = novaMatricula(nome, idade, sexo, numMatricula);
				
				//Inserir a nova matricula na pagina e verificar se a mesma está na pool
				
				
				verificaEspacoNaPagina(listaOcupada, mat); // percorre a lista de pagina ocupada e no primeiro espaco de pagina que achar retorna o ponteiro
				
				printf("TESTEE: %s", listaOcupada->pagina->m1->nome);
				
				//Percorrer a lista de páginas ocupada e verificar se ainda há espaço para uma nova matrícula
	
				insereHash(mat, tabela);
				
				puts("\nAluno cadastrado com sucesso!\n");
				}
			
				break;
			case 2:
				while(validar != 1){
					printf("\nInforme a matrícula do aluno que deseja remover:\n");
					scanf("%d", &numMatricula);
					validar = validarNumero(numMatricula);
					if (validar == 0){
						puts("\nInforme um número de matrícula válido! (Maior do que zero)!");
					}
				}
				validar = 0;
				
				aluno = buscarAluno(numMatricula, tabela);
				
				if(aluno == NULL){
					printf("\nO aluno não foi encontrado!\n\n");
				}else{
					removerAluno(aluno, tabela); 
					puts("\nAluno removido com sucesso!\n");
				}
				
				break;
			case 3:
				while(validar != 1){
					printf("\nInforme a matrícula do aluno que deseja modificar:\n");
					scanf("%d", &numMatricula);
					fflush(stdin);
					validar = validarNumero(numMatricula);
					if (validar == 0){
						puts("\nInforme um número de matrícula válido! (Maior do que zero)!");
					}
				}
				validar = 0;
				
				aluno = buscarAluno(numMatricula, tabela);
				
				if(aluno == NULL){
					printf("\nO aluno não foi encontrado!\n\n");
				}else{
					printf("\nDigite o novo nome:\n");
					scanf("%[^\n]", nome);
					strcpy(aluno->nome, nome);
					fflush(stdin);
					printf("\nDigite a nova idade:\n");
					scanf("%d", &idade);
					aluno->idade = idade;
					fflush(stdin);
					printf("\nDigite o sexo (M / F):\n");
					scanf("%c", &sexo);
					aluno->sexo = sexo;
					fflush(stdin);		
				}
				break;
			
			case 4:
				while(validar != 1){
					printf("\nInforme a matrícula do aluno que deseja consultar:\n");
					scanf("%d", &numMatricula);
					validar = validarNumero(numMatricula);
					if (validar == 0){
						puts("\nInforme um número de matrícula válido! (Maior do que zero)!");
					}
				}
				validar = 0;
				
				aluno = buscarAluno(numMatricula, tabela);
				
				if(aluno == NULL){
					printf("\nO aluno não foi encontrado!\n\n");
				}else{
					imprimirInfoAluno(aluno); 
				}
				
				break;
				
			case 5:	
			
				imprimirHash(tabela);
				break;
				
			case 0:
				break;
			default:
				puts("\nOpção inválida.\n");
		}
	} while (op);
	
	puts("\n************************************************************");
	puts("\nSistema encerrado!");
	puts("\n************************************************************");
}
