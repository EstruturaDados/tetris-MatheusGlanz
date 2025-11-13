#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_FILA 5   // capacidade da fila circular
#define MAX_PILHA 3  // capacidade da pilha de reserva

// ============================================================
// ==================== ESTRUTURA PEÇA ========================
// ============================================================

// Representa uma peça do jogo
typedef struct {
    char nome; // tipo: 'I', 'O', 'T', 'L'
    int id;    // identificador único
} Peca;

// ============================================================
// ==================== ESTRUTURA FILA ========================
// ============================================================

typedef struct {
    Peca elementos[MAX_FILA];
    int inicio;
    int fim;
    int tamanho;
} Fila;

// Inicializa a fila circular
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

// Adiciona uma peça ao final da fila
void enfileirar(Fila *f, Peca p) {
    if (filaCheia(f)) {
        printf("Fila cheia! Não é possível adicionar nova peça.\n");
        return;
    }
    f->elementos[f->fim] = p;
    f->fim = (f->fim + 1) % MAX_FILA;
    f->tamanho++;
}

// Remove e retorna a peça da frente da fila
Peca desenfileirar(Fila *f) {
    Peca removida = {'-', -1};
    if (filaVazia(f)) {
        printf("Fila vazia! Não há peças para remover.\n");
        return removida;
    }
    removida = f->elementos[f->inicio];
    f->inicio = (f->inicio + 1) % MAX_FILA;
    f->tamanho--;
    return removida;
}

// Acessa a peça da frente da fila (sem remover)
Peca *primeiraPeca(Fila *f) {
    if (filaVazia(f)) return NULL;
    return &f->elementos[f->inicio];
}

// Exibe o conteúdo atual da fila
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

// ============================================================
// ==================== ESTRUTURA PILHA =======================
// ============================================================

typedef struct {
    Peca elementos[MAX_PILHA];
    int topo;
} Pilha;

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

// Adiciona uma peça no topo da pilha
void empilhar(Pilha *p, Peca pec) {
    if (pilhaCheia(p)) {
        printf("Pilha cheia! Não é possível reservar mais peças.\n");
        return;
    }
    p->elementos[++p->topo] = pec;
}

// Remove e retorna a peça do topo da pilha
Peca desempilhar(Pilha *p) {
    Peca removida = {'-', -1};
    if (pilhaVazia(p)) {
        printf("Pilha vazia! Não há peças reservadas.\n");
        return removida;
    }
    removida = p->elementos[p->topo--];
    return removida;
}

// Acessa a peça do topo (sem remover)
Peca *topoPilha(Pilha *p) {
    if (pilhaVazia(p)) return NULL;
    return &p->elementos[p->topo];
}

// Exibe o conteúdo atual da pilha
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

// ============================================================
// ==================== GERAÇÃO DE PEÇAS ======================
// ============================================================

Peca gerarPeca(int id) {
    char tipos[] = {'I', 'O', 'T', 'L'};
    Peca nova;
    nova.nome = tipos[rand() % 4];
    nova.id = id;
    return nova;
}

// ============================================================
// ==================== TROCAS ENTRE ESTRUTURAS ===============
// ============================================================

// Troca a peça da frente da fila com o topo da pilha
void trocarSimples(Fila *f, Pilha *p) {
    if (filaVazia(f) || pilhaVazia(p)) {
        printf("Não há peças suficientes para troca simples.\n");
        return;
    }
    Peca *frente = primeiraPeca(f);
    Peca *topo = topoPilha(p);
    Peca temp = *frente;
    *frente = *topo;
    *topo = temp;
    printf("🔄 Troca simples realizada entre a frente da fila e o topo da pilha.\n");
}

// Troca as 3 primeiras peças da fila com as 3 da pilha
void trocarMultiplas(Fila *f, Pilha *p) {
    if (f->tamanho < 3 || p->topo < 2) {
        printf("São necessárias pelo menos 3 peças em cada estrutura para a troca múltipla.\n");
        return;
    }

    for (int i = 0; i < 3; i++) {
        int indexFila = (f->inicio + i) % MAX_FILA;
        int indexPilha = p->topo - i;
        Peca temp = f->elementos[indexFila];
        f->elementos[indexFila] = p->elementos[indexPilha];
        p->elementos[indexPilha] = temp;
    }

    printf("Troca múltipla realizada entre as 3 primeiras da fila e as 3 da pilha.\n");
}

// ============================================================
// ==================== EXIBIÇÃO GERAL ========================
// ============================================================

void exibirEstado(Fila *f, Pilha *p) {
    printf("\n==================== ESTADO ATUAL ====================\n");
    exibirFila(f);
    exibirPilha(p);
    printf("======================================================\n");
}

// ============================================================
// ==================== FUNÇÃO PRINCIPAL ======================
// ============================================================

int main() {
    Fila fila;
    Pilha pilha;
    int opcao;
    int contadorId = 0;

    srand(time(NULL));

    inicializarFila(&fila);
    inicializarPilha(&pilha);

    // Preenche a fila inicial com 5 peças
    for (int i = 0; i < MAX_FILA; i++) {
        enfileirar(&fila, gerarPeca(contadorId++));
    }

    // Loop principal do menu
    do {
        exibirEstado(&fila, &pilha);

        printf("\nOpções disponíveis:\n");
        printf("1 - Jogar peça da frente da fila\n");
        printf("2 - Enviar peça da fila para a pilha de reserva\n");
        printf("3 - Usar peça da pilha de reserva\n");
        printf("4 - Trocar peça da frente da fila com o topo da pilha\n");
        printf("5 - Trocar os 3 primeiros da fila com as 3 peças da pilha\n");
        printf("0 - Sair\n");
        printf("Opção escolhida: ");
        scanf("%d", &opcao);
        printf("\n");

        switch (opcao) {
            case 1: { // Jogar peça
                Peca jogada = desenfileirar(&fila);
                if (jogada.id != -1)
                    printf("Peça jogada: [%c %d]\n", jogada.nome, jogada.id);
                break;
            }

            case 2: { // Reservar peça
                if (!filaVazia(&fila) && !pilhaCheia(&pilha)) {
                    Peca reservada = desenfileirar(&fila);
                    empilhar(&pilha, reservada);
                    printf("Peça reservada: [%c %d]\n", reservada.nome, reservada.id);
                } else {
                    printf("Não foi possível reservar a peça.\n");
                }
                break;
            }

            case 3: { // Usar peça da reserva
                Peca usada = desempilhar(&pilha);
                if (usada.id != -1)
                    printf("Peça usada: [%c %d]\n", usada.nome, usada.id);
                break;
            }

            case 4: { // Troca simples
                trocarSimples(&fila, &pilha);
                break;
            }

            case 5: { // Troca múltipla
                trocarMultiplas(&fila, &pilha);
                break;
            }

            case 0:
                printf("Encerrando o programa...\n");
                break;

            default:
                printf("Opção inválida! Tente novamente.\n");
        }

        // Após cada ação (exceto sair), gera nova peça para manter a fila cheia
        if (opcao != 0 && !filaCheia(&fila)) {
            enfileirar(&fila, gerarPeca(contadorId++));
        }

    } while (opcao != 0);

    return 0;
}
