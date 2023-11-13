//Atividade Avaliativa de ED1
//Dupla: Michel Lima/ Arthur Romano

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <time.h>
#include <math.h>

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
    struct No *direita, *esquerda;
}No;

typedef struct Lista{
    No* inicio;
    int tamanhoLista;
}Lista;


//limpar janela pra ficar bunito
void limparJanela(){
    #ifdef _WIN32
    system("cls");
    #elif __linux__
    system("clear");
    #endif
}

//inicializar lista
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

//ler arquivo CSV
void CarregarCSV(Lista* lista){
    FILE* arquivo = fopen("dados.csv","r");//abrir arquivo
    if (arquivo == NULL){
        printf("Erro ao ler o arquivo!\n");
        exit(1);
    }
    char linha[256];
    fgets(linha, sizeof(linha), arquivo); // Ignora a primeira linha
    while(fgets(linha,sizeof(linha),arquivo)){
        No* aux = (No*) malloc (sizeof(No)); //alocar memoria
        char *token = strtok(linha,",");//ler linha ate a virgula
        if (token == NULL){
            printf("Erro ao processar linha do Codigo\n");
            continue;
        }
        strcpy(aux->funcionarios.nome, token);//salvar nome
        token = strtok(NULL,"/");//ler linha ate a barra
        if (token == NULL){
            printf("Erro ao processar linha do Codigo\n");
            continue;
        }
        aux->funcionarios.dataNascimento.dia = atoi(token);//salvar dia da data de nascimento
        
        token = strtok(NULL,"/");//ler linha ate a barra
        if (token == NULL){
            printf("Erro ao processar linha do Codigo\n");
            continue;
        }
        aux->funcionarios.dataNascimento.mes = atoi(token);//salvar mes da data de nascimento

        token = strtok(NULL,",");//ler linha ate a virgula
        if (token == NULL){
            printf("Erro ao processar linha do Codigo\n");
            continue;
        }
        aux->funcionarios.dataNascimento.ano = atoi(token);//salvar ano da data de nescimento

        token = strtok(NULL,",");//ler linha ate a virgula
        if (token == NULL){
            printf("Erro ao processar linha do Codigo\n");
            continue;
        }
        strcpy(aux->funcionarios.endereco.rua,token);//salvar o nome da rua do funcionario

        token = strtok(NULL,",");//ler linha ate a virgula
        if (token == NULL){
            printf("Erro ao processar linha do Codigo\n");
            continue;
        }
        aux->funcionarios.endereco.numero = atoi(token);//salvar numero do endereco
        token = strtok(NULL,"\n");//ler linha ate o final
        char salario[40], formatado[40]; 
        if (token == NULL){
            printf("Erro ao processar linha do codigo\n");
            continue;
        }
        strcpy(salario,token);//salvar salario em uma string
        AjeitarFormatoFloat(salario, formatado);//ajeitar string "R$ 1.000,00" para o padrao float 1000.00
        aux->funcionarios.salario = atof(formatado);//salvar salario em formato de float sem erros
        
        //inserir na lista duplamente encadeada pelo inicio
        aux->proximo = lista->inicio;
        aux->anterior = NULL;
        if(lista->inicio != NULL){
            lista->inicio->anterior = aux;
        }
        lista->inicio = aux;
    }
}

void imprimir(Lista *lista){
    No* listar = lista->inicio;
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

void diferencaData(No *no) {

    struct tm dataNo = {0};
    time_t tempoAtual = time(NULL);
    struct tm *dataAtual = localtime(&tempoAtual);

    dataNo.tm_year = no->funcionarios.dataNascimento.ano;
    dataNo.tm_mon = no->funcionarios.dataNascimento.mes;
    dataNo.tm_mday = no->funcionarios.dataNascimento.dia;

    //dataAtual->tm_year -= 1900;
    //dataAtual->tm_mon -= 1;
    dataNo.tm_year -= 1900;
    dataNo.tm_mon -= 1;

    double diferencaSegundos = difftime(mktime(dataAtual), mktime(&dataNo));
    printf("Nome: %s\n", no->funcionarios.nome);
    printf("Diferença de dias: %f\n", floor(diferencaSegundos / (60 * 60 * 24)));

}

int main () {
    
    Lista lista;
    inicializaLista(&lista);
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

    diferencaData(lista.inicio->proximo->proximo->proximo);

    return 0;   
}