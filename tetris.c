#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_FILA 5   // Tamanho máximo da fila
#define MAX_PILHA 3  // Tamanho máximo da pilha

// Estrutura que representa uma peça do jogo
typedef struct {
    char nome; // Tipo da peça 
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

// Declaração das funções
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
void trocarPeca(Fila *f, Pilha *p);
void trocaMultipla(Fila *f, Pilha *p);

int main() {
    Fila fila;
    Pilha pilha;
    int opcao, idAtual = MAX_FILA;

    srand(time(NULL)); // Gera números aleatórios para as peças
    inicializarFila(&fila);
    inicializarPilha(&pilha);

    // Preenche a fila inicial com peças aleatórias
    for (int i = 0; i < MAX_FILA; i++) {
        inserirFila(&fila, gerarPeca(i));
    }

    // Menu principal
    do {
        printf("\n=== Estado Atual ===\n");
        mostrarEstado(&fila, &pilha);

        printf("\nOpções:\n");
        printf("1 - Jogar peça da frente da fila\n");
        printf("2 - Enviar peça da fila para reserva (pilha)\n");
        printf("3 - Usar peça da reserva (pilha)\n");
        printf("4 - Trocar peça da frente da fila com o topo da pilha\n");
        printf("5 - Trocar as 3 primeiras da fila com as 3 da pilha\n");
        printf("0 - Sair\n");
        printf("Opção: ");
        scanf("%d", &opcao);

        printf("\n");

        switch (opcao) {
            case 1: { // Jogar peça da frente da fila
                Peca jogada = removerFila(&fila);
                if (jogada.id != -1) {
                    printf("Peça jogada: [%c %d]\n", jogada.nome, jogada.id);
                    inserirFila(&fila, gerarPeca(idAtual++));
                }
                break;
            }
            case 2: { // Enviar peça da fila para pilha de reserva
                if (pilhaCheia(&pilha)) {
                    printf("A pilha de reserva está cheia!\n");
                } else {
                    Peca reservada = removerFila(&fila);
                    if (reservada.id != -1) {
                        push(&pilha, reservada);
                        printf("Peça [%c %d] reservada!\n", reservada.nome, reservada.id);
                        inserirFila(&fila, gerarPeca(idAtual++));
                    }
                }
                break;
            }
            case 3: { // Usar peça do topo da pilha
                if (pilhaVazia(&pilha)) {
                    printf("Nenhuma peça reservada.\n");
                } else {
                    Peca usada = pop(&pilha);
                    printf("Peça usada: [%c %d]\n", usada.nome, usada.id);
                }
                break;
            }
            case 4: { // Trocar a primeira da fila com o topo da pilha
                trocarPeca(&fila, &pilha);
                break;
            }
            case 5: { // Trocar as 3 primeiras da fila com as 3 da pilha
                trocaMultipla(&fila, &pilha);
                break;
            }
            case 0:
                printf("Encerrando...\n");
                break;
            default:
                printf("Opção inválida.\n");
        }

    } while (opcao != 0);

    return 0;
}

// Inicializa a fila vazia
void inicializarFila(Fila *f) {
    f->inicio = 0;
    f->fim = 0;
    f->total = 0;
}

// Inicializa a pilha vazia
void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

// Verificações de estado
int filaCheia(Fila *f) { return f->total == MAX_FILA; }
int filaVazia(Fila *f) { return f->total == 0; }
int pilhaCheia(Pilha *p) { return p->topo == MAX_PILHA - 1; }
int pilhaVazia(Pilha *p) { return p->topo == -1; }

// Gera uma nova peça com tipo e id aleatórios
Peca gerarPeca(int id) {
    Peca nova;
    char tipos[] = {'I', 'O', 'T', 'L'};
    nova.nome = tipos[rand() % 4];
    nova.id = id;
    return nova;
}

// Insere uma peça no final da fila
void inserirFila(Fila *f, Peca p) {
    if (filaCheia(f)) return;
    f->itens[f->fim] = p;
    f->fim = (f->fim + 1) % MAX_FILA;
    f->total++;
}

// Remove a peça da frente da fila
Peca removerFila(Fila *f) {
    Peca vazio = {'-', -1};
    if (filaVazia(f)) return vazio;
    Peca removida = f->itens[f->inicio];
    f->inicio = (f->inicio + 1) % MAX_FILA;
    f->total--;
    return removida;
}

// Insere uma peça no topo da pilha
void push(Pilha *p, Peca e) {
    if (!pilhaCheia(p)) p->itens[++p->topo] = e;
}

// Remove e retorna a peça do topo da pilha
Peca pop(Pilha *p) {
    Peca vazio = {'-', -1};
    if (pilhaVazia(p)) return vazio;
    return p->itens[p->topo--];
}

// Mostra o estado atual da fila e da pilha
void mostrarEstado(Fila *f, Pilha *p) {
    printf("Fila de Peças: ");
    if (filaVazia(f))
        printf("(vazia)");
    else {
        int idx = f->inicio;
        for (int i = 0; i < f->total; i++) {
            printf("[%c %d] ", f->itens[idx].nome, f->itens[idx].id);
            idx = (idx + 1) % MAX_FILA;
        }
    }

    printf("\nPilha de Reserva (Topo -> Base): ");
    if (pilhaVazia(p))
        printf("(vazia)");
    else {
        for (int i = p->topo; i >= 0; i--) {
            printf("[%c %d] ", p->itens[i].nome, p->itens[i].id);
        }
    }
    printf("\n");
}

// Troca a peça da frente da fila com o topo da pilha
void trocarPeca(Fila *f, Pilha *p) {
    if (filaVazia(f) || pilhaVazia(p)) {
        printf("Não há peças suficientes para trocar.\n");
        return;
    }

    printf("\nTroca realizada entre a peça da frente da fila e o topo da pilha.\n");
    printf("\n=== Antes ===\n");
    mostrarEstado(f, p);

    int idxFila = f->inicio;
    Peca temp = f->itens[idxFila];
    f->itens[idxFila] = p->itens[p->topo];
    p->itens[p->topo] = temp;

    printf("\n=== Depois ===\n");
    mostrarEstado(f, p);
}

// Troca múltipla entre as 3 primeiras da fila e as 3 da pilha
void trocaMultipla(Fila *f, Pilha *p) {
    if (f->total < 3 || p->topo < 2) {
        printf("Não há peças suficientes para troca múltipla.\n");
        return;
    }

    printf("Troca realizada entre as 3 primeiras da fila e as 3 da pilha.\n");
    printf("\n=== Antes ===\n");
    mostrarEstado(f, p);

    for (int i = 0; i < 3; i++) {
        int idxFila = (f->inicio + i) % MAX_FILA;
        Peca temp = f->itens[idxFila];
        f->itens[idxFila] = p->itens[p->topo - i];
        p->itens[p->topo - i] = temp;
    }

    printf("\n=== Depois ===\n");
    mostrarEstado(f, p);
}
