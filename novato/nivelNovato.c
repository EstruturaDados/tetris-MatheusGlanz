#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_FILA 5 // tamanho máximo da fila

// Estrutura que representa uma peça
typedef struct {
    char nome;
    int id;
} Peca;

// Estrutura da fila circular
typedef struct {
    Peca elementos[MAX_FILA];
    int inicio;
    int fim;
    int tamanho;
} Fila;

// ---------- Funções para manipulação da fila ----------

// Inicializa a fila
void inicializarFila(Fila *f) {
    f->inicio = 0;
    f->fim = 0;
    f->tamanho = 0;
}

// Verifica se a fila está cheia
int filaCheia(Fila *f) {
    return f->tamanho == MAX_FILA;
}

// Verifica se a fila está vazia
int filaVazia(Fila *f) {
    return f->tamanho == 0;
}

// Gera uma nova peça automaticamente
Peca gerarPeca(int id) {
    char tipos[] = {'I', 'O', 'T', 'L'};
    Peca nova;
    nova.nome = tipos[rand() % 4];
    nova.id = id;
    return nova;
}

// Insere uma nova peça no final da fila (enqueue)
void enfileirar(Fila *f, Peca p) {
    if (filaCheia(f)) {
        printf("Fila cheia! Não é possível adicionar nova peça.\n");
        return;
    }
    f->elementos[f->fim] = p;
    f->fim = (f->fim + 1) % MAX_FILA;
    f->tamanho++;
}

// Remove a peça da frente da fila (dequeue)
void desenfileirar(Fila *f) {
    if (filaVazia(f)) {
        printf("Fila vazia! Não há peças para jogar.\n");
        return;
    }
    printf("Peça jogada: [%c %d]\n", f->elementos[f->inicio].nome, f->elementos[f->inicio].id);
    f->inicio = (f->inicio + 1) % MAX_FILA;
    f->tamanho--;
}

// Exibe o estado atual da fila
void exibirFila(Fila *f) {
    printf("\nFila de peças:\n");
    if (filaVazia(f)) {
        printf("(vazia)\n");
        return;
    }
    for (int i = 0; i < f->tamanho; i++) {
        int index = (f->inicio + i) % MAX_FILA;
        printf("[%c %d] ", f->elementos[index].nome, f->elementos[index].id);
    }
    printf("\n");
}

// ---------- Função principal ----------
int main() {
    Fila fila;
    int opcao;
    int contadorId = 0; // controla o ID único de cada peça

    srand(time(NULL));
    inicializarFila(&fila);

    // Inicializa com 5 peças
    for (int i = 0; i < MAX_FILA; i++) {
        enfileirar(&fila, gerarPeca(contadorId++));
    }

    do {
        exibirFila(&fila);
        printf("\nOpções de ação:\n");
        printf("1 - Jogar peça (dequeue)\n");
        printf("2 - Inserir nova peça (enqueue)\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                desenfileirar(&fila);
                break;
            case 2:
                enfileirar(&fila, gerarPeca(contadorId++));
                break;
            case 0:
                printf("Saindo do jogo...\n");
                break;
            default:
                printf("Opção inválida! Tente novamente.\n");
        }

    } while (opcao != 0);

    return 0;
}
