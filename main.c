//Atividade Avaliativa de ED1
//Dupla: Michel Lima/ Arthur Romano

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
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
            printf("Endereço: %s, %d\n", no->funcionarios.endereco.rua, no->funcionarios.endereco.numero);
            printf("Data de nascimento: %d/%d/%d\n\n", no->funcionarios.dataNascimento.dia, no->funcionarios.dataNascimento.mes, no->funcionarios.dataNascimento.ano);
        }
        pesquisaSalario(no->proximo, min, max);
    }
}


void menuArvore(Lista *lista){
    int opcao;
    while (opcao != 4){
        printf("Escolha:\n1- Mostrar elementos da arvore por Faixa Salarial\n2- Listar Elementos em Ordem crescente de dias vividos\n3- Listar Elementos em Ordem decrescente de dias vividos\n4- Sair\n");
        scanf("%d",&opcao);
        switch(opcao){
            case 1:

            break;
            case 2:
            break;
            case 3:
            break;
            case 4:
                limparJanela();
                exit(1);
            break;
            default:
                limparJanela();
                printf("Opcao Invalida!\n");
        }
    }
}

int main () {
    Lista lista;
    inicializaLista(&lista);
    CarregarCSV(&lista);
    int opcao,menu;
    limparJanela();
    while (opcao != 6){
        setbuf(stdin,NULL);
        printf("Escolha: \n1- Listar Pessoas\n2- Listar A-Z\n3- Listar Z-A\n4- Pesquisar Salario\n5- Transformar Arvore AVL\n6- Sair\n");
        scanf("%d",&opcao);
        switch(opcao){
            case 1:
                limparJanela();
                imprimir(&lista);
                printf("digite 1 para voltar ao menu:");
                scanf("%d",&menu);
                if (menu != 1){
                    imprimir(&lista);
                }else{
                    menu = 0;
                    limparJanela();
                    break;
                }
                break;
            case 2:
                limparJanela();
                ListarAZlista(&lista);
                printf("digite 1 para voltar ao menu:");
                scanf("%d",&menu);
                if (menu != 1){
                    ListarAZlista(&lista);
                }else{
                    menu = 0;
                    limparJanela();
                    break;
                }
                break;
            case 3:
                limparJanela();
                ListarZAlista(&lista);
                printf("digite 1 para voltar ao menu:");
                scanf("%d",&menu);
                if (menu != 1){
                    ListarZAlista(&lista);
                }else{
                    menu = 0;
                    limparJanela();
                    break;
                }
                break;
            case 4:
                limparJanela();
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
                    limparJanela();
                    break;
                }
                break;
            case 5:
                int confirmar;
                limparJanela();
                printf("Ao fazer isso a lista Deixara de existir e se tornara uma Arvore AVL\nseja proseguir com a transformacao?\n");
                printf("1-SIM 2-NAO\n");
                scanf("%d",&confirmar);
                if (confirmar == 1){
                    limparJanela();
                    menuArvore(&lista);
                }else{
                    limparJanela();
                    break;
                }
                break;
            case 6:
                exit(1);
                break;
            default:
                limparJanela();
                printf("Opcao Invalida!\n");
        }
    }

    return 0;   
}