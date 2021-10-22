#include <stdlib.h>
#include <string.h>
#include "listaDeProcessos.h"

/*
* Cria uma lista de processos e retorna o ponteiro para essa lista.
*/
ListaProcessos *criaListaProcessos() {
    ListaProcessos *lista = malloc(sizeof(ListaProcessos));

    lista -> tamanho = 0;
    lista -> primeiro = NULL;
    lista -> ultimo = NULL;

    return lista;
}

/*
* Cria um processo e retorna o ponteiro para esse processo.
*/
Processo *criaProcesso(int pid, int estado, char *linhaDeComando, int jid) {
    Processo *processo = malloc(sizeof(Processo));

    processo -> proximo = NULL;
    processo -> pid = pid;
    processo -> jid = jid;
    processo -> estado = estado;
    processo -> linhaDeComando = linhaDeComando;

    return processo;
}

/*
* Recebe uma lista de processos e um processo e 
* adiciona esse processo a lista de processos.
*/
void adicionaProcesso(ListaProcessos *lista, Processo *processo) {
    //verifica se o primeiro item da lista é vazio, se for adiciona então o processo
    if(!lista->primeiro)
        lista -> primeiro = processo;
    //senão, adiciona ele como proximo item do último item da lista.
    else 
        lista -> ultimo -> proximo = processo;
    
    lista -> ultimo = processo;
    lista -> tamanho++;
}

/*
* Recebe uma lista de processos e um pid de processo, 
* Retorna um ponteiro pro processo desse pid (se existir),
* do contrário retorna nulo.
*/
Processo *retornaProcessoDePid(ListaProcessos *lista, pid_t pid) {
    Processo *processoAtual = lista -> primeiro;

    while(processoAtual != NULL) {
        if(processoAtual -> pid == pid)
            return processoAtual;
        processoAtual = processoAtual -> proximo;
    }
    return NULL;
}

/*
* Recebe uma lista de processos e um jid de processo, 
* Retorna um ponteiro pro processo desse jid (se existir),
* do contrário retorna nulo.
*/
Processo *retornaProcessoDeJid(ListaProcessos *lista, int jid) {
    Processo *processoAtual = lista -> primeiro;

    while(processoAtual != NULL) {
        if(processoAtual -> jid == jid)
            return processoAtual;
        processoAtual = processoAtual -> proximo;
    }
    return NULL;
}

/*
* Recebe uma lista de processos e um pid de processo, 
* e remove o processo referente ao pid (se existir), e retorna ele,
* caso contrário, retorna NULL.
*/
Processo *removeProcessoDaLista(ListaProcessos *lista, pid_t pid) {
    Processo *processoAtual = lista -> primeiro;
    Processo *aux;

    //verifica se o processo atual é nulo
    if(!processoAtual)
        return NULL;
    
    //verifica se o pid do processo atual é igual ao recebido no argumento
    if(processoAtual -> pid == pid) {
        lista -> primeiro = processoAtual -> proximo;
        //confere se esse é o único elemento da lista
        if(lista -> ultimo == processoAtual) {
            lista -> ultimo = NULL;
        }
        return processoAtual;
    }

    //procura o pid na lista de processos
    while(processoAtual -> proximo != NULL) {
        if(processoAtual -> proximo -> pid == pid) {
            aux = processoAtual -> proximo;
            if(aux == lista -> ultimo) {
                lista -> ultimo = processoAtual;
            }
            processoAtual -> proximo = processoAtual -> proximo -> proximo;
            return aux;
        }
        processoAtual = processoAtual -> proximo;
    }
    return NULL;
}

/*
* Recebe um processo e o remove.
*/
void removeProcesso(Processo *processo) {
    //verifica se o processo existe
    if(!processo)
        return;
    free(processo);
    return;
}

/*
* Recebe uma lista de processos e retorna qual o jid do próximo processo a ser adicionado
* nesta lista.
*/
int proximoJID(ListaProcessos *lista) {
    //verifica se a lista possui um ultimo elemento
    if(!lista -> ultimo)
        return 1;
    return lista -> ultimo -> jid+1;
}