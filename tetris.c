#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_FILA 5
#define MAX_PILHA 3

// Estrutura que representa uma peça
typedef struct {
    char nome; // Tipo da peça ('I', 'O', 'T', 'L')
    int id;    // Identificador único
} Peca;

// Estrutura da fila circular
typedef struct {
    Peca itens[MAX_FILA];
    int inicio;
    int fim;
    int total;
} Fila;

// Estrutura da pilha
typedef struct {
    Peca itens[MAX_PILHA];
    int topo;
} Pilha;

// Protótipos
void inicializarFila(Fila *f);
void inicializarPilha(Pilha *p);
int filaCheia(Fila *f);
int filaVazia(Fila *f);
int pilhaCheia(Pilha *p);
int pilhaVazia(Pilha *p);
void inserirFila(Fila *f, Peca p);
Peca removerFila(Fila *f);
void push(Pilha *p, Peca e);
Peca pop(Pilha *p);
Peca gerarPeca(int id);
void mostrarEstado(Fila *f, Pilha *p);

// Função principal
int main() {
    Fila fila;
    Pilha pilha;
    int opcao, idAtual = MAX_FILA;

    srand(time(NULL));
    inicializarFila(&fila);
    inicializarPilha(&pilha);

    // Inicializa a fila com 5 peças
    for (int i = 0; i < MAX_FILA; i++) {
        inserirFila(&fila, gerarPeca(i));
    }

    do {
        printf("\n========== ESTADO ATUAL ==========\n");
        mostrarEstado(&fila, &pilha);

        printf("\nOpções:\n");
        printf("1 - Jogar peça\n");
        printf("2 - Reservar peça\n");
        printf("3 - Usar peça reservada\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: { // Jogar peça
                Peca jogada = removerFila(&fila);
                if (jogada.id != -1) {
                    printf("Peça jogada: [%c %d]\n", jogada.nome, jogada.id);
                    inserirFila(&fila, gerarPeca(idAtual++));
                }
                break;
            }
            case 2: { // Reservar peça
                if (pilhaCheia(&pilha)) {
                    printf("A pilha de reserva está cheia!\n");
                } else {
                    Peca reservada = removerFila(&fila);
                    if (reservada.id != -1) {
                        push(&pilha, reservada);
                        printf("Peça [%c %d] reservada com sucesso!\n", reservada.nome, reservada.id);
                        inserirFila(&fila, gerarPeca(idAtual++));
                    }
                }
                break;
            }
            case 3: { // Usar peça reservada
                if (pilhaVazia(&pilha)) {
                    printf("Nenhuma peça reservada para usar.\n");
                } else {
                    Peca usada = pop(&pilha);
                    printf("Peça usada: [%c %d]\n", usada.nome, usada.id);
                }
                break;
            }
            case 0:
                printf("Encerrando o jogo...\n");
                break;
            default:
                printf("Opção inválida.\n");
        }

    } while (opcao != 0);

    return 0;
}

// Inicializa a fila
void inicializarFila(Fila *f) {
    f->inicio = 0;
    f->fim = 0;
    f->total = 0;
}

// Inicializa a pilha
void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

// Verificações
int filaCheia(Fila *f) { return f->total == MAX_FILA; }
int filaVazia(Fila *f) { return f->total == 0; }
int pilhaCheia(Pilha *p) { return p->topo == MAX_PILHA - 1; }
int pilhaVazia(Pilha *p) { return p->topo == -1; }

// Gera uma nova peça aleatória
Peca gerarPeca(int id) {
    Peca nova;
    char tipos[] = {'I', 'O', 'T', 'L'};
    nova.nome = tipos[rand() % 4];
    nova.id = id;
    return nova;
}

// Insere peça na fila (enqueue)
void inserirFila(Fila *f, Peca p) {
    if (filaCheia(f)) {
        printf("Fila cheia. Não é possível inserir.\n");
        return;
    }
    f->itens[f->fim] = p;
    f->fim = (f->fim + 1) % MAX_FILA;
    f->total++;
}

// Remove peça da fila (dequeue)
Peca removerFila(Fila *f) {
    Peca vazio = {'-', -1};
    if (filaVazia(f)) {
        printf("Fila vazia. Não há peças para jogar.\n");
        return vazio;
    }
    Peca removida = f->itens[f->inicio];
    f->inicio = (f->inicio + 1) % MAX_FILA;
    f->total--;
    return removida;
}

// Adiciona peça na pilha (push)
void push(Pilha *p, Peca e) {
    if (pilhaCheia(p)) {
        printf("A pilha está cheia. Não é possível reservar mais.\n");
        return;
    }
    p->itens[++p->topo] = e;
}

// Remove peça da pilha (pop)
Peca pop(Pilha *p) {
    Peca vazio = {'-', -1};
    if (pilhaVazia(p)) {
        printf("A pilha está vazia.\n");
        return vazio;
    }
    return p->itens[p->topo--];
}

// Mostra o estado atual da fila e pilha
void mostrarEstado(Fila *f, Pilha *p) {
    printf("\nFila de peças:\n");
    if (filaVazia(f)) {
        printf("Fila vazia.\n");
    } else {
        for (int i = 0, idx = f->inicio; i < f->total; i++, idx = (idx + 1) % MAX_FILA) {
            printf("[%c %d] ", f->itens[idx].nome, f->itens[idx].id);
        }
        printf("\n");
    }

    printf("\nPilha de reserva (Topo -> Base):\n");
    if (pilhaVazia(p)) {
        printf("Nenhuma peça reservada.\n");
    } else {
        for (int i = p->topo; i >= 0; i--) {
            printf("[%c %d] ", p->itens[i].nome, p->itens[i].id);
        }
        printf("\n");
    }
}
