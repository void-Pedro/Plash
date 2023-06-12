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
