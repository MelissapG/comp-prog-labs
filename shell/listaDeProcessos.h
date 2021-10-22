#ifndef Lista_Processo_H
#define Lista_Processo_H

typedef struct Processo Processo;

struct Processo {
    pid_t pid;
    int jid;
    int estado;
    char *linhaDeComando;
    Processo *proximo;
};

typedef struct ListaProcessos {
    int tamanho;
    Processo *primeiro;
    Processo *ultimo;
} ListaProcessos;

ListaProcessos *criaListaProcessos();
Processo *criaProcesso(int pid, int estado, char *linhaDeComando, int jid);
void adicionaProcesso(ListaProcessos *lista, Processo *processo);
Processo *retornaProcessoDePid(ListaProcessos *lista, pid_t pid);
Processo *retornaProcessoDeJid(ListaProcessos *lista, int jid);
Processo *removeProcessoDaLista(ListaProcessos *lista, pid_t pid);
void removeProcesso(Processo *processo);
int proximoJID(ListaProcessos *lista);

#endif