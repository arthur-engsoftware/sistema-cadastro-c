#include "cadastrolib.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int cadastrarProduto(Produto **inicio, int id, const char *nome, int quant,
                     int tipo)
{
    Produto *novo = malloc(sizeof(*novo));
    if (novo == NULL) {
        return 0;
    }

    novo->id = id;
    snprintf(novo->nome_prod, sizeof(novo->nome_prod), "%s", nome);
    novo->quant = quant;
    novo->tipo = tipo;
    novo->prox = NULL;

    while (*inicio != NULL) {
        inicio = &(*inicio)->prox;
    }
    *inicio = novo;
    return 1;
}

Produto *buscarProduto(Produto *inicio, int id)
{
    while (inicio != NULL) {
        if (inicio->id == id) {
            return inicio;
        }
        inicio = inicio->prox;
    }
    return NULL;
}

void listarProdutos(const Produto *inicio)
{
    if (inicio == NULL) {
        printf("Nenhum produto cadastrado.\n");
        return;
    }

    printf("\n============= PRODUTOS CADASTRADOS =============\n");
    while (inicio != NULL) {
        printf("ID: %d | Nome: %s | Quantidade: %d | Tipo: %d\n",
               inicio->id, inicio->nome_prod, inicio->quant, inicio->tipo);
        inicio = inicio->prox;
    }
}

int atualizarProduto(Produto *produto, const char *nome, int quant, int tipo)
{
    if (produto == NULL) {
        return 0;
    }

    snprintf(produto->nome_prod, sizeof(produto->nome_prod), "%s", nome);
    produto->quant = quant;
    produto->tipo = tipo;
    return 1;
}

int removerProduto(Produto **inicio, int id)
{
    while (*inicio != NULL && (*inicio)->id != id) {
        inicio = &(*inicio)->prox;
    }

    if (*inicio == NULL) {
        return 0;
    }

    Produto *removido = *inicio;
    *inicio = removido->prox;
    free(removido);
    return 1;
}

void liberarProdutos(Produto **inicio)
{
    while (*inicio != NULL) {
        Produto *removido = *inicio;
        *inicio = removido->prox;
        free(removido);
    }
}

void limparLinha(void)
{
    int caractere;
    while ((caractere = getchar()) != '\n' && caractere != EOF) {
    }
}

int lerInteiro(const char *mensagem, int *valor)
{
    while (1) {
        printf("%s", mensagem);
        if (scanf("%d", valor) == 1) {
            limparLinha();
            return 1;
        }

        if (feof(stdin)) {
            return 0;
        }

        limparLinha();
        printf("Digite um numero valido.\n");
    }
}

int lerNome(char nome[50])
{
    printf("Nome do produto: ");
    if (scanf(" %49[^\n]", nome) != 1) {
        return 0;
    }
    limparLinha();
    return 1;
}

int cadastrar(Produto **produtos, int *proximoId)
{
    char nome[50];
    int quantidade;
    int tipo;

    if (!lerNome(nome) ||
        !lerInteiro("Quantidade: ", &quantidade) ||
        !lerInteiro("Tipo: ", &tipo)) {
        return 0;
    }

    if (!cadastrarProduto(produtos, *proximoId, nome, quantidade, tipo)) {
        printf("Erro ao cadastrar produto.\n");
        return 1;
    }

    printf("Produto cadastrado com o ID %d.\n", *proximoId);
    (*proximoId)++;
    return 1;
}

int consultar(Produto *produtos)
{
    int id;
    if (!lerInteiro("ID do produto: ", &id)) {
        return 0;
    }

    Produto *produto = buscarProduto(produtos, id);
    if (produto == NULL) {
        printf("Produto nao encontrado.\n");
    } else {
        printf("ID: %d | Nome: %s | Quantidade: %d | Tipo: %d\n",
               produto->id, produto->nome_prod, produto->quant, produto->tipo);
    }
    return 1;
}

int atualizar(Produto *produtos)
{
    int id;
    if (!lerInteiro("ID do produto a atualizar: ", &id)) {
        return 0;
    }

    Produto *produto = buscarProduto(produtos, id);
    if (produto == NULL) {
        printf("Produto nao encontrado.\n");
        return 1;
    }

    char nome[50];
    int quantidade;
    int tipo;
    if (!lerNome(nome) ||
        !lerInteiro("Nova quantidade: ", &quantidade) ||
        !lerInteiro("Novo tipo: ", &tipo)) {
        return 0;
    }

    atualizarProduto(produto, nome, quantidade, tipo);
    printf("Produto atualizado.\n");
    return 1;
}

int remover(Produto **produtos)
{
    int id;
    if (!lerInteiro("ID do produto a remover: ", &id)) {
        return 0;
    }

    if (removerProduto(produtos, id)) {
        printf("Produto removido.\n");
    } else {
        printf("Produto nao encontrado.\n");
    }
    return 1;
}

void exibirMenu(void)
{
    printf("\n===== SISTEMA DE CADASTRO =====\n");
    printf("1. Cadastrar produto\n");
    printf("2. Listar produtos\n");
    printf("3. Buscar produto\n");
    printf("4. Atualizar produto\n");
    printf("5. Remover produto\n");
    printf("0. Sair\n");
}

int main(void)
{
    Produto *produtos = NULL;
    int proximoId = 1;
    int opcao;
    int continuar = 1;

    while (continuar) {
        exibirMenu();
        if (!lerInteiro("Escolha uma opcao: ", &opcao)) {
            break;
        }

        switch (opcao) {
        case 1:
            continuar = cadastrar(&produtos, &proximoId);
            break;
        case 2:
            listarProdutos(produtos);
            break;
        case 3:
            continuar = consultar(produtos);
            break;
        case 4:
            continuar = atualizar(produtos);
            break;
        case 5:
            continuar = remover(&produtos);
            break;
        case 0:
            continuar = 0;
            break;
        default:
            printf("Opcao invalida.\n");
        }
    }

    liberarProdutos(&produtos);
    printf("Sistema encerrado.\n");
    return 0;
}
