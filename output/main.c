//Atividade Avaliativa de ED1
//Dupla: Michel Lima/ Arthur Romano
gaygaygay
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct Data{
    int mes,dia,ano;
}Data;

typedef struct Funcionarios{
    char nome[40];
    char endereco[40];
    Data datanascimento;
    float salario;
}Funcionarios;

typedef struct No{
    Funcionarios funcionarios;
    struct No *proximo, *anterior;
}No;

typedef struct Lista{
    No* inicio;
    int tamanhoLista;
}Lista;

void inicializaLista(Lista* lista){
    lista->inicio = NULL;
    lista->tamanhoLista = 0;
}

int main (int argc ,const char *argv[]){
    Lista lista;
    printf("\n%s\n",argv[1]);
    inicializaLista(&lista);
    return 0;

}