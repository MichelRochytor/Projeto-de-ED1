//Atividade Avaliativa de ED1
//Dupla: Michel Lima/ Arthur Romano

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

void ListarAZlista(Lista* lista){
    Lista listaOrdenada;
    listaOrdenada.inicio = NULL;
    No* listar = lista->inicio;
    while (listar != NULL){
        No* aux = (No*) malloc (sizeof(No));
        No* atual;
        No* anterior = NULL;
        if(aux == NULL){
            printf("erro!");
            exit(1);
        }
        aux->funcionarios = listar->funcionarios;
        aux->proximo = NULL;
        if(listaOrdenada.inicio == NULL){
            listaOrdenada.inicio = aux;
        }else{
            atual = listaOrdenada.inicio;
            while(atual != NULL && strcmp(atual->funcionarios.nome, aux->funcionarios.nome) < 0){
                anterior = atual;
                atual = atual->proximo;
            }
            if(anterior == NULL){
                aux->proximo = listaOrdenada.inicio;
                listaOrdenada.inicio = aux;
            }else{
                anterior->proximo = aux;
                aux->proximo = atual;
            }
        }
        listar = listar->proximo;
    }
    imprimir(&listaOrdenada);
}

void ListarZAlista(Lista* lista){
    Lista listaOrdenada;
    listaOrdenada.inicio = NULL;
    No* listar = lista->inicio;
    while (listar != NULL){
        No* aux = (No*) malloc (sizeof(No));
        No* atual;
        No* anterior = NULL;
        if(aux == NULL){
            printf("erro!");
            exit(1);
        }
        aux->funcionarios = listar->funcionarios;
        aux->proximo = NULL;
        if(listaOrdenada.inicio == NULL){
            listaOrdenada.inicio = aux;
        }else{
            atual = listaOrdenada.inicio;
            while(atual != NULL && strcmp(atual->funcionarios.nome, aux->funcionarios.nome) > 0){
                anterior = atual;
                atual = atual->proximo;
            }
            if(anterior == NULL){
                aux->proximo = listaOrdenada.inicio;
                listaOrdenada.inicio = aux;
            }else{
                anterior->proximo = aux;
                aux->proximo = atual;
            }
        }
        listar = listar->proximo;
    }
    imprimir(&listaOrdenada);
}


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


int main () {
    Lista lista;
    inicializaLista(&lista);
    printf("Inicializou com sucesso\n");
    CarregarCSV(&lista);
    printf("Carregou com sucesso\n");
    int opcao,menu;
    system("clear");
    while (opcao != 5){
        printf("Escolha: \n1- Listar Pessoas\n2- Listar A-Z\n3- Listar Z-A\n4- Pesquisar Salario\n5- Sair\n");
        scanf("%d",&opcao);
        switch(opcao){
            case 1:
                system("clear");
                imprimir(&lista);
                printf("digite 1 para voltar ao menu:");
                scanf("%d",&menu);
                if (menu != 1){
                    imprimir(&lista);
                }else{
                    menu = 0;
                    system("clear");
                    break;
                }
                break;
            case 2:
                ListarAZlista(&lista);
                break;
            case 3:
                ListarZAlista(&lista);
                break;
            case 4:
            system("clear");
                float min, max;
                printf("Digite a margem de pesquisa: ");
                scanf("%f%f",&min,&max);
                pesquisaSalario(lista.inicio,min,max);
                printf("digite 1 para voltar ao menu:");
                scanf("%d",&menu);
                if (menu != 1){
                    printf("Digite a margem de pesquisa: ");
                    scanf("%f%f",&min,&max);
                    pesquisaSalario(lista.inicio,min,max);
                }else{
                    menu = 0;
                    system("clear");
                    break;
                }
                break;
            case 5:
                exit(1);
                break;
            default:
                printf("Opcao Invalida!\n");
                system("clear");
        }
    }

    return 0;
    
}