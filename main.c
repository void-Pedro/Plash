/*
    Implementação de um Shell simplificado para a disciplina de Sistemas Operacionais
    Autores: Pedro Henrique Borges - 804071
             Luís Fernando do Carmo Lourenço - 800210

    Casos aceitos no Shell:
        a. $ prog - Processo em foreground
        b. $ prog & - Processo em background
        c. $ prog parâmetros - Processo com parâmetros (separados por espaços ou vetor de strings)
        d. $ prog parâmetros & - Processo com parâmetros em bg
        e. $ prog [parâmetros] > arquivo - Processo com parâmetros que envia o output para um arquivo
        f. $ prog1 | prog 2 - Processo que envia sua saída para outro processo através de um pipe
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>

int VerificaBackground(char comando[]) {
    if (strstr(comando, "&") != NULL) {
        return 1;
    }
    return 0;
}

int main() {
    char comando[100];

    signal(SIGCHLD, SIG_DFL);

    while (1) {
        printf("$ ");

        fgets(comando, 100, stdin);
        comando[strcspn(comando, "\n")] = '\0';

        int bg = VerificaBackground(comando);

        char *pipe_token = strchr(comando, '|');
        if (pipe_token != NULL) {
            // Comando com pipe
            *pipe_token = '\0';
            char *comando1 = comando;
            char *comando2 = pipe_token + 2;

            int pipefd[2];
            if (pipe(pipefd) == -1) {
                printf("Erro ao criar o pipe\n");
                exit(1);
            }

            pid_t pid1 = fork();
            if (pid1 < 0) {
                printf("Erro ao criar processo filho\n");
                exit(1);
            } else if (pid1 == 0) {
                // Processo filho (prog1)
                close(pipefd[0]);  // Fecha a extremidade de leitura do pipe

                // Redireciona a saída padrão para o pipe
                dup2(pipefd[1], STDOUT_FILENO);

                // Executa o primeiro comando
                char *args1[100];
                int indice1 = 0;
                char *par1 = strtok(comando1, " ");
                while (par1 != NULL) {
                    args1[indice1++] = par1;
                    par1 = strtok(NULL, " ");
                }
                args1[indice1] = NULL;

                execvp(args1[0], args1);
                printf("Erro durante a execução de %s\n", args1[0]);
                exit(1);
            } else {
                pid_t pid2 = fork();
                if (pid2 < 0) {
                    printf("Erro ao criar processo filho\n");
                    exit(1);
                } else if (pid2 == 0) {
                    // Processo filho (prog2)
                    close(pipefd[1]);  // Fecha a extremidade de escrita do pipe

                    // Redireciona a entrada padrão para o pipe
                    dup2(pipefd[0], STDIN_FILENO);

                    // Executa o segundo comando
                    char *args2[100];
                    int indice2 = 0;
                    char *par2 = strtok(comando2, " ");
                    while (par2 != NULL) {
                        args2[indice2++] = par2;
                        par2 = strtok(NULL, " ");
                    }
                    args2[indice2] = NULL;

                    execvp(args2[0], args2);
                    printf("Erro durante a execução de %s\n", args2[0]);
                    exit(1);
                } else {
                    close(pipefd[0]);  // Fecha a extremidade de leitura do pipe no processo pai
                    close(pipefd[1]);  // Fecha a extremidade de escrita do pipe no processo pai

                    int status1, status2;
                    waitpid(pid1, &status1, 0);
                    waitpid(pid2, &status2, 0);
                }
            }
        } else {
            // Comando simples
            pid_t pid = fork();
            if (pid < 0) {
                printf("Erro ao criar processo filho\n");
                exit(1);
            } else if (pid == 0) {
                // Processo filho
                char *par;
                char *args[100];
                int indice = 0;
                char *nomeArq = NULL;
                int redirecionarOutput = 0;

                par = strtok(comando, " ");
                while (par != NULL) {
                    if (strcmp(par, "&") != 0 && strcmp(par, ">") != 0) {
                        if (redirecionarOutput) {
                            nomeArq = par;
                            redirecionarOutput = 0;
                        } else {
                            args[indice++] = par;
                        }
                    } else if (strcmp(par, ">") == 0) {
                        redirecionarOutput = 1;
                    }
                    par = strtok(NULL, " ");
                }
                args[indice] = NULL;

                if (nomeArq != NULL) {
                    freopen(nomeArq, "w", stdout);
                }

                int executou = execvp(args[0], args);

                if (executou == -1) {
                    printf("Erro durante a execução do comando\n");
                    exit(1);
                }
            } else {
                if (!bg) {
                    int status;
                    wait(&status);
                }
            }
        }
    }
}
