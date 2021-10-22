#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <wait.h>

#include "listaDeProcessos.h"

#define tamanho_linhaDeComando 1024
#define tamanho_argumentos 128
#define tamanho_diretorio 4096
#define delimitador_argumentos " \t\r\n\a"

//Lista de processos em execução
ListaProcessos *listaProcessos;

//pid do processo que está em foreground
pid_t pid_fg = 0;

/*
* Recebe o sinal e faz o tratamento.
*/
void handler(int sinal) {
    pid_t pidFilho;
    Processo *filho;

    switch(sinal) {
        //modifica o handler para quando o usuário digita CTRL + C.
        case SIGINT:
            //confere se há processo em execução
            if(pid_fg < 1){
                break;
            }
            kill(pid_fg, SIGQUIT);
            removeProcessoDaLista(listaProcessos, pid_fg);
            pid_fg = 0;
            break;

        //modifica o handler para quando o usuário digita CTRL + Z.
        case SIGTSTP:
            if(pid_fg < 1){
                break;
            }
            kill(pid_fg, SIGSTOP);
            retornaProcessoDePid(listaProcessos, pid_fg) -> estado = 0;
            pid_fg = 0;
            break;

        //sinal para quando um processo em background for concluído. 
        case SIGCHLD:
            pidFilho = waitpid(-1, NULL, WNOHANG);
            if(pidFilho > 0) {
                filho = retornaProcessoDePid(listaProcessos, pidFilho);
                filho -> estado = -1;
            }
            break;
    }
    return;
}

/*
* Recebe a linha de comando atual, a separa em argumentos e os salva em uma lista.
* A função retorna 1 se o comando será executado em background e 0 caso contrário.
*/
int separador(char *linhaDeComando, char **args) {
    char *token;
    int posicao = 0;

    //separa a string pelo delimitador
    token = strtok(linhaDeComando, delimitador_argumentos);
    while (token != NULL) {
    args[posicao] = token;
    posicao++;
    token = strtok(NULL, delimitador_argumentos);
    }

    args[posicao] = NULL;

    if (posicao == 0) return 1;

    if(*args[posicao-1]=='&') {
        args[posicao-1] = NULL;
        return 1;
    }
    return 0;
}

/*
* Lista os atuais jobs da shell.
*/
void jobs(char **args) {
    Processo *processo = listaProcessos -> primeiro;
    Processo *aux = NULL;

    /*
    * Define se printa e como printa o pid dos processos;
    * Caso 0: não printa pid;
    * Caso 1: printa pid;
    * Caso 2: apenas printa pid;
    */
    int printaPid = 0;

    /*
    * Filtra quais jobs serão printados
    * Caso -1 (default): printa todos os jobs;
    * Caso 0: printa todos os jobs pausados;
    * Caso 1: printa todos os jobs em execução;
    */
    int filtroEstado = -1;

    for(int i=1; args[i]; i++) {
        if(args[i][0] == '-'){
            switch(args[i][1]) {
                case 'l':
                    printaPid = 1;
                    break;
                case 'p':
                    printaPid = 2;
                    break;
                case 'r':
                    filtroEstado = 1;
                    break;
                case 's':
                    filtroEstado = 0;
                    break;
                default:
                    printf("Opção inválida.\n");
                    return;
            }
        } else {
            printf("Opção inválida.\n");
            return;
        }
    }
    if (!processo) return;
    while(processo != NULL) {

        //verifica qual é o estado do processo
        char estado[11] = "";
        switch (processo -> estado) {
            case 1:
                strcpy(estado, "Executando");
                break;
            case -1:
                strcpy(estado, "Concluído");
                aux = removeProcessoDaLista(listaProcessos, processo -> pid);
                break;
            default:
                strcpy(estado, "Pausado");
                break;
        }

        //verifica se o processo será printado
        if(filtroEstado != -1 && processo -> estado != filtroEstado){
            processo = processo -> proximo;
            continue;
        }

        //printa as informações do processo
        if(!printaPid) {
            printf("[%d] %s\t%s\n", processo->jid, estado, processo->linhaDeComando);
        } else if(printaPid==1) {
            printf("[%d] %d %s \t%s\n", processo -> jid, processo -> pid, estado, processo->linhaDeComando);
        } else {
            printf("%d\n", processo -> pid);
        }

        //se o processo tiver sido concluído, remove ele.
        if(aux) {
            removeProcesso(aux);
            aux = NULL;
        }
        processo = processo -> proximo;
    }
    return;
}

/*
* Recebe a lista de comandos do usuário, já separada, e se o comando é foreground (1) ou background (0),
* e retoma a execução do processo indicado pelo usuário.
*/
void retomaProcesso(char **args, int eh_fg) {
    Processo *processo;

    if(!listaProcessos -> primeiro) {
        printf("Não existe processo em execução.\n");
        return;
    }

    //verifica se existe um argumento pro comando
    if(args[1]) {
        int parametro;

        //verifica se é jid
        if(args[1][0] == "%"){
            //verifica se é um jid válido
            if(!sscanf(args[1],"%%%d", &parametro)) {
                printf("jid inválido.\n");
                return;
            }

            processo = retornaProcessoDeJid(listaProcessos, parametro);
            //verifica se o  processo existe
            if(!processo) {
                printf("Processo com este jid não existe.\n");
                return;
            }
        } else {
            //verifica se é um pid válido
            if(!sscanf(args[1],"%d", &parametro)) {
                printf("pid inválido.\n");
                return;
            }

            processo = retornaProcessoDePid(listaProcessos, parametro);
            //verifica se o  processo existe
            if(!processo) {
                printf("Processo com este pid não existe.\n");
                return;
            }
        }
    } else {
        processo = listaProcessos -> ultimo;
    }

    // printa para o usuário o jid e o comando que o processo irá executar
    printf("[%d] %s", processo -> jid, processo -> linhaDeComando);
    processo -> estado = 1;
    kill(processo -> pid, SIGCONT);

    //se for background
    if(!eh_fg){
        return;
    }
    //senão, ele vai executar em foreground e o processo pai vai esperar pelo filho
    int estado;
    pid_fg = processo -> pid;
    if(waitpid(pid_fg, &estado, WUNTRACED) < 0){
        printf("Erro ao esperar pelo término do processo.\n");
        exit(1);
    }
    //se o estado for 5247 é porque o processo foi parado e não terminado, 
    //e não deve ser removido da lista de processos.
    if(estado != 5247) {
        removeProcesso(removeProcessoDaLista(listaProcessos, pid_fg));
    }
    pid_fg = 0;
}

/*
* Recebe um comando do usuário, que já foi separado, e se este for built-in, 
* a função executa o comando e retorna 1, senão retorna 0.
*/
int builtin(char **args) {
    char *comando = args[0];

    //comando para sair da shell
    if(!strcmp(comando, "quit")) 
        exit(0);

    //comando para executar em background
    if(!strcmp(comando, "&"))
        return 1;

    //comando para mudar de diretório    
    if(!strcmp(comando, "cd")) {
        if(!args[1])
            return 1;
        if(chdir(args[1]) < 0)
            printf("Erro ao mudar de diretório.\n");
        return 1;
    }

    //comando para os trabalhos da shell
    if(!strcmp(comando, "jobs")) {
        jobs(args);
        return 1;
    }

    //comando para retomar processos em foreground 
    if(!strcmp(comando, "fg")) {        
        retomaProcesso(args, 1);
        return 1;
    }

    //comando para retomar processos em background
    if(!strcmp(comando, "bg")) {
        retomaProcesso(args, 0);
        return 1;
    }

    return 0;
}

/*
* Recebe do usuário o comando pela linha de comando e o executa.
*/
void trataComando(char *comando) {
    char *args[tamanho_argumentos];
    char linhaDeComando[tamanho_linhaDeComando];
    int bg;
    pid_t pid;
 
    strcpy(linhaDeComando, comando);
    //separa o comando em arguumentos e retorna se é para executar em background
    bg = separador(linhaDeComando, args);
    //caso não tenha comandos
    if(args[0] == NULL) {
        return;
    }

    Processo *processo;
    //verifica se o comando é built-in e executa
    if(!builtin(args)) {

        //cria processo filho para executar comandos não-built-in
        if((pid = fork()) == 0) {
            //os processos filhos não podem ser parados pelo input do usuário
            if(signal(SIGINT, SIG_IGN) == SIG_ERR || signal(SIGTSTP, SIG_IGN) == SIG_ERR) {
                printf("Erro de sinal.\n");
                exit(1);
            }
            
            //espera o pai adicionar o filho na lista de processos 
            waitpid(getppid(), NULL, 0);

            //processos filhos não têm lista de processo
            listaProcessos = NULL;

            //executa os comandos não built-in
            if(execve(args[0], args, __environ) < 0) {
                printf("O comando não foi encontrado.\n");
                exit(0);
            }
        }
        // adiciona o processo filho criado anteriormente na lista de processos.
        else {
            char *buffer = malloc(sizeof(char[tamanho_linhaDeComando]));
            strcpy(buffer, linhaDeComando);
            processo = criaProcesso(pid, 1, buffer, proximoJID(listaProcessos));
            adicionaProcesso(listaProcessos, processo);
        }

        //se o comando for em foreground, pai espera o filho terminar a execução
        if(bg) {
            pid_fg = pid;
            int estado;
            if(waitpid(pid, &estado, WUNTRACED) < 0) {
                printf("Erro ao esperar pelo término do processo.\n");
                exit(1);
            }

            //se o estado for 5247 é porque o processo foi parado e não terminado, 
            //e não deve ser removido da lista de processos.
            if(estado != 5247) {
                removeProcesso(removeProcessoDaLista(listaProcessos, pid_fg));
            }
            pid_fg = 0;

        //printa para o usuário o jid e o comando que o processo irá executar
        } else {
            printf("[%d] %s", processo -> jid, linhaDeComando);
        }
    } 
}

/*
* Fluxo principal de execução da shell
*/
int shell() {
    //buffer para entrada do usuário
    char linhaDeComando[tamanho_linhaDeComando];

    //buffer do diretório atual
    char diretorioAtual[tamanho_diretorio];

    //handler
    if(signal(SIGINT, handler) == SIG_ERR || signal(SIGTSTP, handler) == SIG_ERR || signal(SIGCHLD, handler) == SIG_ERR) {
        printf("Erro de sinal.\n");
        exit(1);
    }

    //inicia a lista de processos
    listaProcessos = criaListaProcessos();

    //loop para shell
    while(1) {

        //lê o diretório atual e salva no buffer
        if(getcwd(diretorioAtual, tamanho_diretorio) == NULL) {
            printf("Erro ao tentar acessar o diretório atual\n");
            exit(1);
        }

        //lê a entrada do usuário
        printf("mpshell:%s> ", diretorioAtual);
        fgets(linhaDeComando, tamanho_linhaDeComando, stdin);

        //finaliza a shell
        if(feof(stdin)) {
            printf("Saindo.\n");
            exit(0);
        }

        //executa o comando do usuário
        trataComando(linhaDeComando);

    }
    return 0;
}

/*
* Executa a shell.
*/
int main(int argc, char const *argv[], char *envp[]) {
    printf("Bem vindo a MPShell - Computadores e Programação 2021.2 - Melissa e Pedro\n\n");
    shell();
    return 0;
}