#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TABLE_SIZE 10 // Tamanho da tabela hash
#define EMPTY -1 // Valor que representa uma posição vazia

// Estrutura que representa uma transação
typedef struct Transacao {
    int id;
    float valor;
    char data[11]; // Data no formato dd/mm/aaaa
    char descricao[256];
} Transacao;

// Função de hash simples (Resto)
int hash(int id) {
    return id % TABLE_SIZE;
}

// Função para criar uma nova transação
Transacao criar_transacao(int id, float valor, const char* data, const char* descricao) {
    Transacao nova_transacao;
    nova_transacao.id = id;
    nova_transacao.valor = valor;
    strcpy(nova_transacao.data, data);
    strcpy(nova_transacao.descricao, descricao);
    return nova_transacao;
}

// Função para inserir uma transação na tabela hash com encadeamento aberto
void inserir_transacao_aberto(Transacao* tabela[], int id, float valor, const char* data, const char* descricao) {
    int index = hash(id);
    int original_index = index;
    Transacao nova_transacao = criar_transacao(id, valor, data, descricao);

    // procura a próxima posição livre
    while (tabela[index] != NULL && tabela[index]->id != EMPTY) {
        if (tabela[index]->id == id) {
            printf("Transacao com ID %d ja existe!\n", id);
            return; // Não insere a transação se o ID já existir
        }
        index = (index + 1) % TABLE_SIZE; // Move para a próxima posição
        if (index == original_index) {
            printf("Tabela cheia, não foi possível inserir a transação.\n");
            return; // A tabela está cheia, não há onde inserir
        }
    }

    // Insere a transação na posição encontrada
    tabela[index] = (Transacao*)malloc(sizeof(Transacao));
    *tabela[index] = nova_transacao;
    printf("Transacao com ID %d inserida com sucesso.\n", id);
}

// Função para buscar uma transação por ID
Transacao* buscar_transacao(Transacao* tabela[], int id) {
    int index = hash(id);
    int original_index = index;

    // percorre a tabela até encontrar a transação ou um índice vazio
    while (tabela[index] != NULL) {
        if (tabela[index]->id == id) {
            return tabela[index]; // Transação encontrada
        }
        index = (index + 1) % TABLE_SIZE; // Move para a próxima posição
        if (index == original_index) {
            break; // A tabela foi completamente percorrida
        }
    }

    return NULL; // Transação não encontrada
}

// Função para remover uma transação pelo ID
void remover_transacao(Transacao* tabela[], int id) {
    int index = hash(id);
    int original_index = index;

    // Sondagem linear: percorre a tabela até encontrar a transação ou um índice vazio
    while (tabela[index] != NULL) {
        if (tabela[index]->id == id) {
            free(tabela[index]); // Libera a memória da transação
            tabela[index] = NULL; // Marca a posição como vazia
            printf("Transacao %d removida com sucesso.\n", id);
            return;
        }
        index = (index + 1) % TABLE_SIZE; // Move para a próxima posição
        if (index == original_index) {
            break; // A tabela foi completamente percorrida
        }
    }

    printf("Transacao %d nao encontrada.\n", id);
}

// Função para listar todas as transações na tabela hash
void listar_transacoes(Transacao* tabela[]) {
    printf("Lista de transacoes:\n");
    for (int i = 0; i < TABLE_SIZE; i++) {
        if (tabela[i] != NULL) {
            printf("ID: %d | Valor: %.2f | Data: %s | Descricao: %s\n", tabela[i]->id, tabela[i]->valor, tabela[i]->data, tabela[i]->descricao);
        }
    }
}

// Função para medir o tempo de execução
void medir_tempo(void (*func)(Transacao*[], int, float, const char*, const char*), Transacao* tabela[], int id, float valor, const char* data, const char* descricao) {
    clock_t start_time = clock();
    func(tabela, id, valor, data, descricao);
    clock_t end_time = clock();
    double time_taken = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    printf("Tempo de execucao: %.6f segundos\n", time_taken);
}

// Função para o menu
void menu() {
    Transacao* tabela[TABLE_SIZE] = {NULL};
    int opcao, id;
    float valor;
    char data[11], descricao[256];

    do {
        printf("\nMenu de Opcoes:\n");
        printf("1. Inserir Transacao\n");
        printf("2. Buscar Transacao\n");
        printf("3. Remover Transacao\n");
        printf("4. Listar Transacoes\n");
        printf("5. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch(opcao) {
            case 1:
                printf("Digite o ID da transacao: ");
                scanf("%d", &id);
                printf("Digite o valor da transacao: ");
                scanf("%f", &valor);
                printf("Digite a data da transacao (dd/mm/aaaa): ");
                scanf("%s", data);
                printf("Digite a descricao da transacao: ");
                scanf(" %[^\n]s", descricao);

                medir_tempo(inserir_transacao_aberto, tabela, id, valor, data, descricao);
                break;

            case 2:
                printf("Digite o ID da transacao a buscar: ");
                scanf("%d", &id);
                Transacao* t = buscar_transacao(tabela, id);
                if (t != NULL) {
                    printf("Transacao encontrada: ID: %d | Valor: %.2f | Data: %s | Descricao: %s\n", t->id, t->valor, t->data, t->descricao);
                } else {
                    printf("Transacao com ID %d nao encontrada.\n", id);
                }
                break;

            case 3:
                printf("Digite o ID da transacao a remover: ");
                scanf("%d", &id);
                remover_transacao(tabela, id);
                break;

            case 4:
                listar_transacoes(tabela);
                break;

            case 5:
                printf("Saindo...\n");
                break;

            default:
                printf("Opcao invalida. Tente novamente.\n");
        }

    } while(opcao != 5);
}

int main() {
    menu();
    return 0;
}
