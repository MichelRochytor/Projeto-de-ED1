//Atividade Avaliativa de ED1
//Dupla: Michel Lima/ Arthur Romano

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// typedef struct Arvore {

//     No *raiz;

// }Arvore;

// typedef struct No {

//     int chave;
//     struct No *esquerda, *direita;

// }No;

typedef struct Endereco{
    char rua[100];
    int numero;
}Endereco;

typedef struct Data{
    int mes,dia,ano;
}Data;

typedef struct Funcionarios{
    char nome[100];
    Endereco endereco;
    Data dataNascimento;
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
void AjeitarFormatoFloat(char salario[], char formatado[]){
    int j = 0;
    int tamanho = strlen(salario);
    for (int i = 0; i < tamanho; i++) {
        if (salario[i] != '\"' && salario[i] != 'R' && salario[i] != '$') {
            if (salario[i] == ',') {
                formatado[j++] = '.';
            } else if (salario[i] != '.') {
                formatado[j++] = salario[i];
            }
        }
    }
    formatado[j] = '\0';
}

void CarregarCSV(Lista* lista){
    FILE* arquivo = fopen("dados.csv","r");
    if (arquivo == NULL){
        printf("Erro ao ler o arquivo\n");
        exit(1);
    }
    char linha[256];
    fgets(linha, sizeof(linha), arquivo); // Ignora a primeira linha
    while(fgets(linha,sizeof(linha),arquivo)){
        No* aux = (No*) malloc (sizeof(No)); 
        char *token = strtok(linha,",");
        if (token == NULL){
            printf("Erro ao processar linha do Codigo\n");
            continue;
        }
        strcpy(aux->funcionarios.nome, token);
        token = strtok(NULL,"/");
        if (token == NULL){
            printf("Erro ao processar linha do Codigo\n");
            continue;
        }
        aux->funcionarios.dataNascimento.dia = atoi(token);
        
        token = strtok(NULL,"/");
        if (token == NULL){
            printf("Erro ao processar linha do Codigo\n");
            continue;
        }
        aux->funcionarios.dataNascimento.mes = atoi(token);

        token = strtok(NULL,",");
        if (token == NULL){
            printf("Erro ao processar linha do Codigo\n");
            continue;
        }
        aux->funcionarios.dataNascimento.ano = atoi(token);
        //printf("%d\n",aux->funcionarios.dataNascimento.ano);

        token = strtok(NULL,",");
        if (token == NULL){
            printf("Erro ao processar linha do Codigo\n");
            continue;
        }
        strcpy(aux->funcionarios.endereco.rua,token);

        token = strtok(NULL,",");
        if (token == NULL){
            printf("Erro ao processar linha do Codigo\n");
            continue;
        }
        aux->funcionarios.endereco.numero = atoi(token);
        token = strtok(NULL,"\n");
        char salario[40], formatado[40]; 
        if (token == NULL){
            printf("Erro ao processar linha do codigo\n");
            continue;
        }
        strcpy(salario,token);
        AjeitarFormatoFloat(salario, formatado);
        aux->funcionarios.salario = atof(formatado);
        aux->proximo = lista->inicio;
        aux->anterior = NULL;
        if(lista->inicio != NULL){
            lista->inicio->anterior = aux;
        }
        lista->inicio = aux;
    }
}

// void pesquisaAZ(No *no) {

//     No auxNo;

//     if(no == NULL || no->proximo == NULL) {  //condição de parada se chegar no final da lista

//         return;

//     }
//     else {

//         if(no->anterior == NULL) {  // condição pra pegar o primeiro nó da lista

//             if(strcmp(no->funcionarios.nome, no.proximo.funcionarios.nome) == 1) {

//                 auxNo = no;
//                 no = no->proximo;
//                 no->proximo = auxNo;

//             }

//         }
//         else {  //condições se estiver no meio da lista

//             if(strcmp(no->anterior->funcionarios.nome, no->funcionarios.nome) == 1) {  

//                 auxNo = no->anterior;
//                 no->anterior = no;
//                 no = auxNo;
                
//             }

//             if(strcmp(no->funcionarios.nome, no->proximo.funcionarios.nome) == 1) {

//                 auxNo = no;
//                 no = no->proximo;
//                 no->proximo = auxNo;

//             }

//             pesquisaAZ(no->proximo);

//         }

//     }

// }

// void pesquisaZA(Lista *lista) {


// }

void pesquisaSalario(No *no, float min, float max) {
    if(no == NULL) {
        return;
    }
    else {
        if(no->funcionarios.salario > min && no->funcionarios.salario < max) {

            printf("Funcionario: %s\n", no->funcionarios.nome);
            printf("Salário: %.2f\n", no->funcionarios.salario);
            printf("Endereço: %s, %i\n", no->funcionarios.endereco.rua, no->funcionarios.endereco.numero);
            printf("Data de nascimento: %i/%i/%i\n\n", no->funcionarios.dataNascimento.dia, no->funcionarios.dataNascimento.mes, no->funcionarios.dataNascimento.ano);

        }

        pesquisaSalario(no->proximo, min, max);

    }

}

// void transformaLista(Lista *lista, Arvore *arvore) {


// }


void imprimir(Lista *lista){
    No *listar = lista->inicio;
    printf("Lista: \n");
    while (listar != NULL){
        printf("Nome: %s\n", listar->funcionarios.nome);
        printf("Data de Nascimento %d/%d/%d\n", listar->funcionarios.dataNascimento.dia
                                              , listar->funcionarios.dataNascimento.mes
                                              , listar->funcionarios.dataNascimento.ano);
        printf("Endereço: %s\n", listar->funcionarios.endereco.rua);
        printf("Endereço numero: %d\n", listar->funcionarios.endereco.numero);
        printf("Salario: %.2f\n\n",listar->funcionarios.salario);
        listar = listar->proximo;
    }
    printf("\n\n");
} 

int main () {
    Lista lista;
    // Arvore arvore;
    // arvore.raiz = NULL;
    inicializaLista(&lista);
    printf("Inicializou com sucesso\n");
    CarregarCSV(&lista);
    printf("Carregou com sucesso\n");
    //imprimir(&lista);
    pesquisaSalario(lista.inicio, 2000.00, 2500.00);

    //  do {

    //     int escolha;
    //     printf("Digite o que deseja fazer:\n");
    //     printf("1 - Pesquisar funcionários por intervalo salarial\n");
    //     printf("2 - Exibir lista de funcionários ordenados por nome de A a Z\n");
    //     printf("3 - Exibir a lista de funcionários ordenados por nome de Z a A\n");
    //     printf("4 - Transformar a lista duplamente encadeada em árvore AVL\n");
    //     printf("R: ");
    //     scanf("%i", &escolha);

    //     switch(escolha) {

    //         case 1:
    //         int min, max;
    //         do {

    //             printf("Digite abaixo o valor mínimo do intervalo: ");
    //             scanf("%i", &min);
    //             printf("Digite abaixo o valor máximo do intervalo: ");
    //             scanf("%i", &max);

    //             if(min > max) {

    //                 printf("Valores mínimo e máximo inválidos! Digite novamente\n");

    //             }

    //         }while(min > max);

    //         pesquisaSalario(&lista, min, max);
    //         break;

    //         case 2:
    //         pesquisaAZ(&lista);
    //         break;

    //         case 3:
    //         pesquisaZA(&lista);
    //         break;

    //         case 4:
    //         transformaLista(&lista, &arvore);
    //         break;

    //         default:
    //         printf("Opção inválida\n");
    //         break;


    //     }while(1);
    //  }

    return 0;
    
}