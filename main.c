/*
    Implementação de um Shell simplificado para a disciplina de Sistemas Operacionais
    Autores: Pedro Henrique Borges - 804071
             Luís Fernando do Carmo - 

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
#include <sys/wait.h>

int main() {
    // Foi definido como 100 o tamanho máximo do comando
    char comando[100];
    signal(SIGCHLD, SIG_IGN);
    
    while(1) {
        printf("$ ");

        // Recebe o comando escrito no terminal
        fgets(comando, 100, stdin);

        comando[strcspn(comando, "\n")] = '\0';
        
        int bg = VerificaBackground(comando);
        pid_t pid = fork();

        // Erro ao criar processo filho
        if (pid < 0) {
            printf("Erro\n");
            exit(1);

            // Processo filho
        } else if (pid == 0) {
            
            char *par;
            char *args[100];
            int indice = 0;

            // Separar os parâmetros do comando
            par = strtok(comando, " ");
            while (par != NULL) {
                args[indice++] = par;
                par = strtok(NULL, " ");
            }
            args[indice] = NULL;
            // No fim, o vetor args vai armazenar os parâmetros em cada índice

            // Executar comando com execvp
            int executou = execvp(args[0], args);

            if (executou == -1) {
                printf("Erro durante a execução do comando\n");
                exit(1);
            }
        } else {

            // Se o processo não for em background, o pai deve esperar o processo filho terminar
            if(!bg) {
                int status;
                wait(&status);
            }
        }
    }
}

int VerificaBackground(char comando[]) {
    for(i = 0; i < 100; i++) {
        if(strcmp(comando[i], "&")) {
            return 1;
        }
    }
    return 0;
}