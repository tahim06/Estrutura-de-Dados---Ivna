#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TABLE_SIZE 10 // Tamanho da tabela hash

// Estrutura que representa uma transação
typedef struct Transacao {
    int id;
    float valor;
    char data[11]; // Data no formato dd/mm/aaaa
    char descricao[256];
    struct Transacao* prox; // Ponteiro para a próxima transação (para resolver colisões)
} Transacao;

// Função de hash simples (Resto)
int hash(int id) {
    return id % TABLE_SIZE;
}

// Função para criar uma nova transação
Transacao* criar_transacao(int id, float valor, const char* data, const char* descricao) {
    Transacao* nova_transacao = (Transacao*)malloc(sizeof(Transacao));
    nova_transacao->id = id;
    nova_transacao->valor = valor;
    strcpy(nova_transacao->data, data);
    strcpy(nova_transacao->descricao, descricao);
    nova_transacao->prox = NULL;
    return nova_transacao;
}

// Função para inserir uma transação na tabela hash com encadeamento
void inserir_transacao_encadeado(Transacao* tabela[], int id, float valor, const char* data, const char* descricao) {
    int index = hash(id);
    Transacao* nova_transacao = criar_transacao(id, valor, data, descricao);
    
    // Se não houver colisão, insere diretamente
    if (tabela[index] == NULL) {
        tabela[index] = nova_transacao;
    } else {
        // Se houver colisão, insere no início da lista encadeada
        nova_transacao->prox = tabela[index];
        tabela[index] = nova_transacao;
    }
}

// Função para buscar uma transação por ID
Transacao* buscar_transacao(Transacao* tabela[], int id) {
    int index = hash(id);
    Transacao* temp = tabela[index];
    
    // Percorre a lista encadeada na posição do índice
    while (temp != NULL) {
        if (temp->id == id) {
            return temp; // Transação encontrada
        }
        temp = temp->prox;
    }
    
    return NULL; // Transação não encontrada
}

// Função para remover uma transação pelo ID
void remover_transacao(Transacao* tabela[], int id) {
    int index = hash(id);
    Transacao* temp = tabela[index];
    Transacao* ant = NULL;
    
    // Percorre a lista encadeada procurando a transação a ser removida
    while (temp != NULL) {
        if (temp->id == id) {
            if (ant == NULL) {
                // Remove o primeiro elemento
                tabela[index] = temp->prox;
            } else {
                // Remove um elemento intermediário ou final
                ant->prox = temp->prox;
            }
            free(temp);
            printf("Transacao %d removida com sucesso.\n", id);
            return;
        }
        ant = temp;
        temp = temp->prox;
    }
    
    printf("Transacao %d nao encontrada.\n", id);
}

// Função para listar todas as transações na tabela hash
void listar_transacoes(Transacao* tabela[]) {
    printf("Lista de transacoes:\n");
    for (int i = 0; i < TABLE_SIZE; i++) {
        Transacao* temp = tabela[i];
        while (temp != NULL) {
            printf("ID: %d | Valor: %.2f | Data: %s | Descricao: %s\n", temp->id, temp->valor, temp->data, temp->descricao);
            temp = temp->prox;
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

// Função para o menu interativo
void menu() {
    Transacao* tabela[TABLE_SIZE] = {0};
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
                scanf(" %[^\n]s", descricao);  // Para ler até a nova linha

                medir_tempo(inserir_transacao_encadeado, tabela, id, valor, data, descricao);
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
