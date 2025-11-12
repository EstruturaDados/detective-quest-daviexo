#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// -------------------- ESTRUTURAS --------------------

// Estrutura da árvore binária de salas
typedef struct Sala {
    char nome[50];
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

// Estrutura da árvore de busca binária (BST) de pistas
typedef struct Pista {
    char texto[100];
    struct Pista *esq;
    struct Pista *dir;
} Pista;

// Estrutura do suspeito (usado na hash)
typedef struct Suspeito {
    char nome[50];
    char pistas[5][100]; // até 5 pistas associadas
    int qtdPistas;
    struct Suspeito *prox; // para tratar colisões
} Suspeito;

// -------------------- VARIÁVEIS GLOBAIS --------------------
#define TAM_HASH 10
Suspeito *tabelaHash[TAM_HASH];
Pista *raizPistas = NULL;

// -------------------- FUNÇÕES DE SALAS --------------------
Sala* criarSala(char nome[]) {
    Sala *nova = (Sala*)malloc(sizeof(Sala));
    strcpy(nova->nome, nome);
    nova->esquerda = NULL;
    nova->direita = NULL;
    return nova;
}

void explorarSalas(Sala *atual) {
    char opcao;

    while (1) {
        printf("\nVocê está na sala: %s\n", atual->nome);
        printf("Ir para (e) esquerda, (d) direita, (s) sair: ");
        scanf(" %c", &opcao);

        if (opcao == 's')
            break;
        else if (opcao == 'e' && atual->esquerda != NULL)
            atual = atual->esquerda;
        else if (opcao == 'd' && atual->direita != NULL)
            atual = atual->direita;
        else
            printf("Caminho inválido!\n");
    }
}

// -------------------- FUNÇÕES DA ÁRVORE DE PISTAS --------------------
Pista* criarPista(char texto[]) {
    Pista *nova = (Pista*)malloc(sizeof(Pista));
    strcpy(nova->texto, texto);
    nova->esq = nova->dir = NULL;
    return nova;
}

Pista* inserirPista(Pista *raiz, char texto[]) {
    if (raiz == NULL) return criarPista(texto);

    if (strcmp(texto, raiz->texto) < 0)
        raiz->esq = inserirPista(raiz->esq, texto);
    else if (strcmp(texto, raiz->texto) > 0)
        raiz->dir = inserirPista(raiz->dir, texto);

    return raiz;
}

void listarPistas(Pista *raiz) {
    if (raiz != NULL) {
        listarPistas(raiz->esq);
        printf("- %s\n", raiz->texto);
        listarPistas(raiz->dir);
    }
}

// -------------------- FUNÇÕES DE HASH --------------------
int funcaoHash(char nome[]) {
    return nome[0] % TAM_HASH; // usa a primeira letra como chave simples
}

void inicializarHash() {
    for (int i = 0; i < TAM_HASH; i++)
        tabelaHash[i] = NULL;
}

void inserirHash(char nome[], char pista[]) {
    int pos = funcaoHash(nome);
    Suspeito *atual = tabelaHash[pos];

    // Verifica se já existe o suspeito
    while (atual != NULL) {
        if (strcmp(atual->nome, nome) == 0) {
            if (atual->qtdPistas < 5) {
                strcpy(atual->pistas[atual->qtdPistas], pista);
                atual->qtdPistas++;
            }
            return;
        }
        atual = atual->prox;
    }

    // Caso não exista, cria novo suspeito
    Suspeito *novo = (Suspeito*)malloc(sizeof(Suspeito));
    strcpy(novo->nome, nome);
    strcpy(novo->pistas[0], pista);
    novo->qtdPistas = 1;
    novo->prox = tabelaHash[pos];
    tabelaHash[pos] = novo;
}

void listarAssociacoes() {
    printf("\n===== SUSPEITOS E SUAS PISTAS =====\n");
    for (int i = 0; i < TAM_HASH; i++) {
        Suspeito *atual = tabelaHash[i];
        while (atual != NULL) {
            printf("Suspeito: %s\n", atual->nome);
            for (int j = 0; j < atual->qtdPistas; j++)
                printf("   -> %s\n", atual->pistas[j]);
            atual = atual->prox;
        }
    }
}

// -------------------- FUNÇÃO PRINCIPAL --------------------
int main() {
    // Inicializa hash
    inicializarHash();

    // Cria o mapa fixo da mansão
    Sala *hall = criarSala("Hall de Entrada");
    Sala *biblioteca = criarSala("Biblioteca");
    Sala *cozinha = criarSala("Cozinha");
    Sala *sotao = criarSala("Sótão");

    hall->esquerda = biblioteca;
    hall->direita = cozinha;
    cozinha->direita = sotao;

    int opcao;
    do {
        printf("\n====== DETECTIVE QUEST ======\n");
        printf("1 - Explorar salas\n");
        printf("2 - Coletar pistas\n");
        printf("3 - Mostrar pistas\n");
        printf("4 - Relacionar pista com suspeito\n");
        printf("5 - Mostrar suspeitos e pistas\n");
        printf("0 - Sair\n");
        printf("=============================\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        if (opcao == 1) {
            explorarSalas(hall);
        } 
        else if (opcao == 2) {
            char novaPista[100];
            printf("Digite a nova pista: ");
            getchar(); // limpa buffer
            fgets(novaPista, 100, stdin);
            novaPista[strcspn(novaPista, "\n")] = '\0';
            raizPistas = inserirPista(raizPistas, novaPista);
            printf("Pista adicionada com sucesso!\n");
        } 
        else if (opcao == 3) {
            printf("\nPistas coletadas:\n");
            listarPistas(raizPistas);
        } 
        else if (opcao == 4) {
            char nome[50], pista[100];
            printf("Nome do suspeito: ");
            getchar();
            fgets(nome, 50, stdin);
            nome[strcspn(nome, "\n")] = '\0';
            printf("Pista associada: ");
            fgets(pista, 100, stdin);
            pista[strcspn(pista, "\n")] = '\0';
            inserirHash(nome, pista);
            printf("Associação registrada!\n");
        } 
        else if (opcao == 5) {
            listarAssociacoes();
        }

    } while (opcao != 0);

    printf("\nEncerrando o jogo... 🕵️\n");
    return 0;
}
