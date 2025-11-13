#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_FILA 5   // Tamanho máximo da fila de peças futuras
#define MAX_PILHA 3  // Tamanho máximo da pilha de peças reservadas

// Estrutura que representa uma peça do Tetris
typedef struct {
    char nome;  // Tipo da peça ('I', 'O', 'T', 'L')
    int id;     // Identificador único da peça
} Peca;

// ---------- Estrutura da FILA (circular) ----------
typedef struct {
    Peca elementos[MAX_FILA];
    int inicio;
    int fim;
    int tamanho;
} Fila;

// ---------- Estrutura da PILHA (linear) ----------
typedef struct {
    Peca elementos[MAX_PILHA];
    int topo;
} Pilha;

// ===========================================================
// =================== FUNÇÕES DA FILA ========================
// ===========================================================

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

// Enfileira (adiciona) uma peça no final da fila
void enfileirar(Fila *f, Peca p) {
    if (filaCheia(f)) {
        printf("Fila cheia! Não é possível adicionar nova peça.\n");
        return;
    }
    f->elementos[f->fim] = p;
    f->fim = (f->fim + 1) % MAX_FILA;
    f->tamanho++;
}

// Desenfileira (remove) a peça da frente
Peca desenfileirar(Fila *f) {
    Peca removida = {'-', -1}; // Valor padrão caso fila esteja vazia
    if (filaVazia(f)) {
        printf("Fila vazia! Não há peças para jogar.\n");
        return removida;
    }
    removida = f->elementos[f->inicio];
    f->inicio = (f->inicio + 1) % MAX_FILA;
    f->tamanho--;
    return removida;
}

// Exibe o estado atual da fila
void exibirFila(Fila *f) {
    printf("Fila de peças\t");
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

// ===========================================================
// =================== FUNÇÕES DA PILHA =======================
// ===========================================================

// Inicializa a pilha
void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

// Verifica se a pilha está cheia
int pilhaCheia(Pilha *p) {
    return p->topo == MAX_PILHA - 1;
}

// Verifica se a pilha está vazia
int pilhaVazia(Pilha *p) {
    return p->topo == -1;
}

// Empilha (coloca) uma peça no topo
void empilhar(Pilha *p, Peca pec) {
    if (pilhaCheia(p)) {
        printf("Pilha cheia! Não é possível reservar mais peças.\n");
        return;
    }
    p->elementos[++p->topo] = pec;
}

// Desempilha (retira) a peça do topo
Peca desempilhar(Pilha *p) {
    Peca removida = {'-', -1};
    if (pilhaVazia(p)) {
        printf("Pilha vazia! Não há peças reservadas.\n");
        return removida;
    }
    removida = p->elementos[p->topo--];
    return removida;
}

// Exibe o estado atual da pilha
void exibirPilha(Pilha *p) {
    printf("Pilha de reserva\t(Topo -> Base): ");
    if (pilhaVazia(p)) {
        printf("(vazia)\n");
        return;
    }
    for (int i = p->topo; i >= 0; i--) {
        printf("[%c %d] ", p->elementos[i].nome, p->elementos[i].id);
    }
    printf("\n");
}

// ===========================================================
// =================== GERAÇÃO DE PEÇAS =======================
// ===========================================================

// Gera automaticamente uma nova peça com tipo aleatório e id único
Peca gerarPeca(int id) {
    char tipos[] = {'I', 'O', 'T', 'L'};
    Peca nova;
    nova.nome = tipos[rand() % 4];
    nova.id = id;
    return nova;
}

// ===========================================================
// =================== FUNÇÃO PRINCIPAL =======================
// ===========================================================

int main() {
    Fila fila;
    Pilha pilha;
    int opcao;
    int contadorId = 0;

    srand(time(NULL)); // inicializa o gerador de números aleatórios

    inicializarFila(&fila);
    inicializarPilha(&pilha);

    // Preenche a fila inicial com 5 peças
    for (int i = 0; i < MAX_FILA; i++) {
        enfileirar(&fila, gerarPeca(contadorId++));
    }

    // Loop principal do menu
    do {
        printf("\n==================== ESTADO ATUAL ====================\n");
        exibirFila(&fila);
        exibirPilha(&pilha);
        printf("======================================================\n");

        printf("\nOpções de Ação:\n");
        printf("1 - Jogar peça\n");
        printf("2 - Reservar peça\n");
        printf("3 - Usar peça reservada\n");
        printf("0 - Sair\n");
        printf("Opção: ");
        scanf("%d", &opcao);
        printf("\n");

        switch (opcao) {
            case 1: {
                // Jogar a peça (remove da fila)
                Peca jogada = desenfileirar(&fila);
                if (jogada.id != -1)
                    printf("Peça jogada: [%c %d]\n", jogada.nome, jogada.id);
                break;
            }

            case 2: {
                // Reservar peça (da frente da fila para o topo da pilha)
                if (!filaVazia(&fila) && !pilhaCheia(&pilha)) {
                    Peca reservada = desenfileirar(&fila);
                    empilhar(&pilha, reservada);
                    printf("📦 Peça reservada: [%c %d]\n", reservada.nome, reservada.id);
                } else {
                    printf("Não foi possível reservar a peça.\n");
                }
                break;
            }

            case 3: {
                // Usar peça reservada (remove do topo da pilha)
                Peca usada = desempilhar(&pilha);
                if (usada.id != -1)
                    printf("Peça usada: [%c %d]\n", usada.nome, usada.id);
                break;
            }

            case 0:
                printf("Encerrando o jogo...\n");
                break;

            default:
                printf("Opção inválida! Tente novamente.\n");
        }

        // Após cada ação (exceto sair), gera nova peça e mantém fila cheia
        if (opcao != 0 && !filaCheia(&fila)) {
            enfileirar(&fila, gerarPeca(contadorId++));
        }

    } while (opcao != 0);

    return 0;
}

