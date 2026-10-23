#ifndef CADASTROLIB_H
#define CADASTROLIB_H

typedef struct Produto {
    int id;
    char nome_prod[50];
    int quant;
    int tipo;
    struct Produto *prox;
} Produto;

int cadastrarProduto(Produto **inicio, int id, const char *nome, int quant,
                     int tipo);
Produto *buscarProduto(Produto *inicio, int id);
void listarProdutos(const Produto *inicio);
int atualizarProduto(Produto *produto, const char *nome, int quant, int tipo);
int removerProduto(Produto **inicio, int id);
void liberarProdutos(Produto **inicio);

#endif
