/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Laboratório: 10 */
/* Codigo:Implementação de produtor consumidor com semáforos */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

#define NTHREADS  5
#define TAM       5  // tamanho do buffer

/* Variaveis globais, sendo X a variável de sincronização para as threads 2,3 e 4; */
int numeroConsumidores;
int numeroProdutores;
int nThreads; // vai ser retirado a partir do número de consumidores + n Produtores
int buffer[TAM];
int x = 0;

sem_t condC, condP; //semaforos para sincronizar a ordem de execucao das threads



void *consumidor(void *arg){
    int id = (long long int) arg;
    printf("Consumidor %d\n" , id);
    if(x>0){
        printf("Tamanho de x = %d\n",buffer[x-1]);
        x--;
    }else{
        printf("Consumidor %d entrando no wait.\n ", id);
        sem_wait(&condC);
    }
    if(x == 0){
        sem_post(&condP);
    }
    sem_wait(&condC);
}

void *produtor(void *arg){
    int id = (long long int) arg;
    printf("Produtor %d\n", id);
    sem_wait(&condP);
    for(int i = 0; i < TAM;i++){
        //buffer[i] = i;
        x++;
    }
    printf("%d", x);
    for(int i = 0; i<numeroConsumidores;i++){
        sem_post(&condC);
    }
    //sem_wait(&condP);
}

/* Funcao principal */
int main(int argc, char *argv[]) {
    int i; 
    pthread_t *tid; //identificadores das threads no sistema

    //leitura e avaliacao dos parametros de entrada
    if(argc<3) {
      printf("Digite: %s <número de consumidores> <numero de produtores>\n", argv[0]);
      return 1;
    }

    numeroConsumidores = atoll(argv[1]);
    numeroProdutores = atoi(argv[2]);
    nThreads = numeroProdutores + numeroConsumidores;

    //inicia os semaforos
    sem_init(&condC,0,0);
    sem_init(&condP,0,1);
    


    //alocacao das estruturas
    tid = (pthread_t*) malloc(sizeof(pthread_t)*(nThreads));
    if(tid==NULL) {puts("ERRO--malloc"); return 2;}

    /* Cria as threads */
    for(long int i=0; i<numeroProdutores; i++) {
        if(pthread_create(tid+i, NULL, produtor, (void*) i)){
            fprintf(stderr, "ERRO--pthread_create\n");
            return 3;
        }
    }

    for(long int i=0; i<numeroConsumidores; i++) {
        if(pthread_create(tid+i, NULL, consumidor, (void*) i)){
            fprintf(stderr, "ERRO--pthread_create\n");
            return 3;
        }
    }

    

    /* Espera todas as threads completarem */
    for (i = 0; i < nThreads; i++) {
        pthread_join(*(tid+i), NULL);
    }
    
    /* Desaloca variaveis e termina */
    sem_destroy(&condC);
    sem_destroy(&condP);

}
