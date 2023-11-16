//Atividade Avaliativa de ED1
//Dupla: Michel Lima/ Arthur Romano

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

/*
    Observações:
    -Pode ocorrer bugs na biblioteca time.h para a obtenção dos dias vividos no windows 
    mas no linux funciona normalmente em caso de nao conseguir no windows deve ser problema 
    de data e hora que nao esta vinculado a internet(entre varios fatores)entao se em caso de 
    nao obter os dias vividos dos funcionarios no windows use no linux ou conecte o vsCode ao 
    WSL caso use no windows facilite a vida de vossos corações :)
    -Todos os requisitos concluidos
    -a funcao limparjanela() funciona nos dois sistemas linux e windows
    -Comentarios explicando cada topico da atividade em cada função
    
    Ass. Michel Lima
*/

//declaração das estruturas cumprindo a sugestão de organização de informações
//mostrado pela Figura 1
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
    struct No *proximo, *anterior;//ou direita e esquerda na arvore AVL
    int altura;//altura da arvore
}No;

typedef struct Lista{
    No* inicio;//raiz da lista encadeada para nao perder o nó na transformação
    No* raiz; //raiz da arvore quando transformar
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

//inicializar lista Duplamente Encadeada e Arvore AVL
void inicializaLista(Lista* lista){
    lista->inicio = NULL;
    lista->raiz = NULL;
    lista->tamanhoLista = 0;
}

//função para consertar a string salario que nao esta no formato adequado para ler em float
//tem a explicacao no CarregarCSV abaixo o porque da existencia dessa funcao
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

//função para obter os dias vividos de cada nó
//LEIA AS OBSERVACOES EM CASO DE PROBLEMA!
float diasVividos(No *no) {
    struct tm dataNo = {0};
    dataNo.tm_sec = 0;
    dataNo.tm_min = 0;
    dataNo.tm_hour = 0;
    dataNo.tm_isdst = -1;
    dataNo.tm_year = no->funcionarios.dataNascimento.ano - 1900;
    dataNo.tm_mon = no->funcionarios.dataNascimento.mes - 1;
    dataNo.tm_mday = no->funcionarios.dataNascimento.dia;
    time_t birth_time = mktime(&dataNo);
    time_t tempoAtual = time(NULL);
    double diferencaSegundos = difftime(tempoAtual, birth_time); 
    double diasVividos = diferencaSegundos/ 60 / 60 / 24;
    return diasVividos;
}

//ler arquivo CSV e salvar na lista duplamente encadeada
//cumprimento do requisito da atividade
/*
    "Primeiramente, ao iniciar o programa, deve ser lido um arquivo.csv
    contendo as informações sobre os funcionários, como nome completo, endereço, 
    data de nascimento e salário. Em seguida, você armazenar ́a essas informações 
    em uma lista duplamente encadeada ordenada por nome"
*/
//----------------------------------------------------------------
//LISTA DUPLAMENTE ENCADEADA
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
        

        //fazer inserção na lista duplamente encadeada ordenada pelo nome 
        if(lista->inicio == NULL || strcmp(aux->funcionarios.nome, lista->inicio->funcionarios.nome) < 0) {
            aux->proximo = lista->inicio;
            aux->anterior = NULL;
            if(lista->inicio != NULL){
                lista->inicio->anterior = aux;
            }
            lista->inicio = aux;
        }else {
            No* atual = lista->inicio;
            while(atual->proximo != NULL && strcmp(aux->funcionarios.nome, atual->proximo->funcionarios.nome) > 0) {
                atual = atual->proximo;
            }
            aux->proximo = atual->proximo;
            aux->anterior = atual;
            if(atual->proximo != NULL) {
                atual->proximo->anterior = aux;
            }
            atual->proximo = aux;
        }
        lista->tamanhoLista++;
    }
}

//funcão para impressão de dados
void imprimir(No* listar){
    printf("Nome: %s\n", listar->funcionarios.nome);
    printf("Data de Nascimento %d/%d/%d\n", listar->funcionarios.dataNascimento.dia
                                              , listar->funcionarios.dataNascimento.mes
                                              , listar->funcionarios.dataNascimento.ano);
    printf("Endereco: %s\n", listar->funcionarios.endereco.rua);
    printf("Endereco numero: %d\n", listar->funcionarios.endereco.numero);
    printf("Salario: %.2f\n\n",listar->funcionarios.salario);
}

//imprimir lista de A a Z 
//cumprimento do requisito da atividade
/*
    "Exibir a lista de funcionarios ordenados por nome de A-Z: exibir em ordem alfabetica 
    o nome e todas as informacoes (endereco, salario, nascimento) dos funcionarios"
*/
void imprimirAZ(Lista *lista){
    No* listar = lista->inicio;
    printf("Lista: \n");
    while (listar != NULL){
        imprimir(listar);
        listar = listar->proximo;
    }
    printf("\n\n");
    printf("Tamanho Lista: %d\n",lista->tamanhoLista);
}

//imprimir lista de Z a A 
//cumprimento do requisito da atividade
/*
    Exibir a lista de funcionarios ordenados por nome de Z-A: similar a especificacao 
    anterior, porem em ordem Z-A;
*/
void ImprimirZA(Lista* lista){
    No* listar = lista->inicio;
    while(listar->proximo != NULL){
        listar = listar->proximo;
    }
    while(listar != NULL){
        imprimir(listar);    
        listar = listar->anterior;
    }
    printf("\n\n");
    printf("Tamanho Lista: %d\n",lista->tamanhoLista);
}

//Função para pesquisar faixa de salario LISTA DUPLAMENTE ENCADEADA(feito por recursão)
//cumprimento do requisito da atividade
/*
    "Pesquisar os funcionarios na lista por intervalo salarial: o usuario deve digitar 
    o valor mınimo e o maximo (valormin ≤ valormax), e, com isso, deve-se imprimir em tela 
    as informacoes (nome,endereco,salario, nascimento) dos funcionarios que recebem o 
    salario dentro desta faixa de valor"
*/
void pesquisaSalario(No *no, float min, float max) {
    if(no == NULL) {
        return;
    }
    else {
        if(no->funcionarios.salario > min && no->funcionarios.salario < max) {
            imprimir(no);
        }
        pesquisaSalario(no->proximo, min, max);
    }
}
//--------------------------------------------------------------------------
//ARVORE AVL
/*
    O algoritmo envolvido para passar a lista duplamente encadeada par arvore AVL foi removendo
    cada no da lista de um a um no qual a lista->inicio fica a lista e vai passando para a 
    lista->raiz aonde esta a arvore, e faz a inserção no padrão AVL assim cumprimdo o requisito
    da atividade 

    "Transformar a lista duplamente encadeada em uma árvore AVL (organizando-a por dias 
    vividos de cada pessoa): caso escolha esta opção, a sua lista deixará de existir, pois 
    cada elemento da lista se transformará em um nó/elemento da AVL (veja o exemplo da 
    Figura 3). Para realizar esta implementação você NÃO deve alocar memória novamente, 
    apenas ocorrerá uma reorganização dos elementos."
*/


//calcular a altura do no
int altura(No* calcular){
    if (calcular == NULL)
        return 0;
    return calcular->altura;
}

//calcular maior indice 
int maximo(int a , int b){
    return (a > b)? a: b;
}

//rotacao a direita na arvore avl
No *rotacaoDireita(No *y) {
    No *x = y->anterior;
    No *T2 = x->proximo;
    x->proximo = y;
    y->anterior = T2;
    y->altura = maximo(altura(y->anterior), altura(y->proximo))+1;
    x->altura = maximo(altura(x->anterior), altura(x->proximo))+1;
    return x;
}

//rotacao a esquerda na arvore avl
No *rotacaoEsquerda(No *x) {
    No *y = x->proximo;
    No *T2 = y->anterior;
    y->anterior = x;
    x->proximo = T2;
    x->altura = maximo(altura(x->anterior), altura(x->proximo))+1;
    y->altura = maximo(altura(y->anterior), altura(y->proximo))+1;
    return y;
}

//calcular fator balanceamento
int estabalanceado(No *N) {
    if (N == NULL)
        return 0;
    return altura(N->anterior) - altura(N->proximo);
}

//pega o no removido da lista e coloca na arvore sem alocar memoria adicional
void inserirNaArvore(No** raiz, No* no) {
    if (*raiz == NULL) {
        *raiz = no;
    } else if (diasVividos(no) < diasVividos((*raiz))) {
        inserirNaArvore(&(*raiz)->anterior, no);
    } else {
        inserirNaArvore(&(*raiz)->proximo, no);
    }
    (*raiz)->altura = 1 + maximo(altura((*raiz)->anterior), altura((*raiz)->proximo));
    int balance = estabalanceado(*raiz);
    if (balance > 1 && diasVividos(no) < diasVividos((*raiz)->anterior))
        *raiz = rotacaoDireita(*raiz);
    if (balance < -1 && diasVividos(no) > diasVividos((*raiz)->proximo))
        *raiz = rotacaoEsquerda(*raiz);
    if (balance > 1 && diasVividos(no) > diasVividos((*raiz)->anterior)) {
        (*raiz)->anterior =  rotacaoEsquerda((*raiz)->anterior);
        *raiz = rotacaoDireita(*raiz);
    }
    if (balance < -1 && diasVividos(no) < diasVividos((*raiz)->proximo)) {
        (*raiz)->proximo = rotacaoDireita((*raiz)->proximo);
        *raiz = rotacaoEsquerda(*raiz);
    }
}

//remoção da lista duplamente encadeada
No* removerLista(Lista* lista){
    No* remover = lista->inicio;
    lista->inicio = lista->inicio->proximo;

    if(lista->inicio != NULL){
        lista->inicio->anterior = NULL;
    }
    remover->proximo = NULL;
    return remover;
}

//mover nó's removidos para a funcao inserirNaArvore
void moverNosParaArvore(Lista* lista) {
    while (lista->inicio != NULL) {
        No* noRemovido = removerLista(lista);
        noRemovido->proximo = NULL;
        noRemovido->anterior = NULL;
        inserirNaArvore(&(lista->raiz), noRemovido);
    }
}

void imprimirArvoreDecresente(No* listar){
    if (listar != NULL){
        imprimirArvoreDecresente(listar->proximo);
        imprimir(listar); 
        imprimirArvoreDecresente(listar->anterior);
    }
}

void imprimirArvoreCresente(No* listar){
    if (listar != NULL){
        imprimirArvoreCresente(listar->anterior);
        imprimir(listar); 
        imprimirArvoreCresente(listar->proximo);
    }
}

void menuArvore(Lista *lista){
    int opcao;
    char menu;
    while (opcao != 4){
        printf("Escolha:\n1- Mostrar elementos da arvore por Faixa Salarial\n2- Listar Elementos em Ordem crescente de dias vividos\n3- Listar Elementos em Ordem decrescente de dias vividos\n4- Sair\n");
        scanf("%d",&opcao);
        switch(opcao){
            case 1:

            break;
            case 2:
                limparJanela();
                imprimirArvoreCresente(lista->raiz);
                printf("digite qualquer coisa para voltar ao menu:");
                scanf("%s",&menu);
                limparJanela();
            break;
            case 3:
                limparJanela();
                imprimirArvoreDecresente(lista->raiz);
                printf("digite qualquer coisa para voltar ao menu:");
                scanf("%s",&menu);
                limparJanela();
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

//-----------------------------------------------------------------------------

//main
int main () {
    Lista lista; // Declarar Lista
    inicializaLista(&lista);//Inicializar Lista
    CarregarCSV(&lista);//Salvar na memoria dados do CSV
    //variaveis de controle do menu
    int opcao;
    char menu;
    limparJanela();//limpar Janela
    while (opcao != 5){
        printf("Escolha: \n1- Listar A-Z\n2- Listar Z-A\n3- Pesquisar Salario\n4- Transformar Arvore AVL\n5- Sair\n");
        scanf("%d",&opcao);
        switch(opcao){
            case 1:
                limparJanela();
                imprimirAZ(&lista);
                printf("digite qualquer coisa para voltar ao menu:");
                scanf("%s",&menu);
                limparJanela();
                break;
            case 2:
                limparJanela();
                ImprimirZA(&lista);
                printf("digite qualquer coisa para voltar ao menu:");
                scanf("%s",&menu);
                limparJanela();
                break;
            case 3:
                limparJanela();
                float min, max;
                printf("Digite a margem de pesquisa: ");
                scanf("%f%f",&min,&max);
                pesquisaSalario(lista.inicio,min,max);
                printf("digite qualquer coisa para voltar ao menu:");
                scanf("%s",&menu);
                limparJanela();
                break;
            case 4:
                int confirmar;
                limparJanela();
                printf("Ao fazer isso a lista Deixara de existir e se tornara uma Arvore AVL\nseja proseguir com a transformacao?\n");
                printf("1-SIM 2-NAO\n");
                scanf("%d",&confirmar);
                if (confirmar == 1){
                    limparJanela();
                    moverNosParaArvore(&lista);
                    menuArvore(&lista);
                }else{
                    limparJanela();
                    break;
                }
                break;
            case 5:
                limparJanela();
                exit(1);
                break;
            default:
                limparJanela();
                printf("Opcao Invalida!\n");
        }
    }
    return 0;   
}//main