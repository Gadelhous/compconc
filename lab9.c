/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Laboratório: 9 */
/* Codigo: Comunicação entre threads usando variável compartilhada e sincronização com semáforos */

/***** Condicao logica da aplicacao: a thread 1 so pode imprimir após as threads 2,3 e 4, que só podem imprimir após a thread 5  ****/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

#define NTHREADS  5

/* Variaveis globais, sendo X a variável de sincronização para as threads 2,3 e 4; */
int x = 0;
sem_t condt2, condt3; //semaforos para sincronizar a ordem de execucao das threads

//função que opera a lógica desejada (nome inspirado em outros exemplos e por falta de criatividade)
void *tarefa (void *arg){                    
    int id = (long long int) arg; //identificador da thread
    switch(id){
        //thread 1
        case 0:
            sem_wait(&condt3);
            printf("Volte sempre!\n");
            break;
            
        //thread 2
        case 1:
            sem_wait(&condt2); //espera T5 executar
            printf("Fique a vontade.\n");
            x++;
            sem_post(&condt2);
            if(x==3){
                sem_post(&condt3);
            }
            break; 

        //thread 3
        case 2:
            sem_wait(&condt2); //espera T5 executar
            printf("Sente-se por favor.\n");
            x++;
            sem_post(&condt2);
            if(x==3){
                sem_post(&condt3);
            }
            break; 

        //thread 4
        case 3:
            sem_wait(&condt2); //espera T5 executar
            printf("Aceita um copo d’agua?\n");
            x++;
            sem_post(&condt2);
            if(x==3){
                sem_post(&condt3);
            }
            break; 
        //thread 5
        case 4:
            printf("Seja bem-vindo!\n");
            sem_post(&condt2);
            break;
    }
}


/* Funcao principal */
int main(int argc, char *argv[]) {
    int i; 
    pthread_t *tid; //identificadores das threads no sistema

    //inicia os semaforos
    sem_init(&condt2, 0, 0);
    sem_init(&condt3, 0, 0);

    //alocacao das estruturas
    tid = (pthread_t*) malloc(sizeof(pthread_t)*NTHREADS);
    if(tid==NULL) {puts("ERRO--malloc"); return 2;}

    /* Cria as threads */
    for(long int i=0; i<NTHREADS; i++) {
        if(pthread_create(tid+i, NULL, tarefa, (void*) i)){
            fprintf(stderr, "ERRO--pthread_create\n");
            return 3;
        }
    }

    /* Espera todas as threads completarem */
    for (i = 0; i < NTHREADS; i++) {
        pthread_join(*(tid+i), NULL);
    }
    
    /* Desaloca variaveis e termina */
    sem_destroy(&condt2);
    sem_destroy(&condt3);

}
