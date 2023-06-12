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
    
    while(1) {
        printf("$ ");

        // Recebe o comando escrito no terminal
        fgets(comando, 100,  stdin);
        
        pid_t pid = fork();

        if (pid < 0) {
            printf("Erro\n");
            exit(1);
        } else if (pid == 0) {
            
            // Executar comando com execvp
            int executou = execvp(comando, NULL);

            if (executou == -1) {
                printf("Erro durante a execução do comando\n");
                exit(1);
            }
        } else {
            int status;
            wait(&status);
        }
    }
}