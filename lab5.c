/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Laboratório: 4 */
/* Codigo: Uso de variáveis de condição e suas operações básicas para sincronização por condição */

/***** Condicao logica da aplicacao: a thread B so pode imprimir "Bye" depois que duas threads A imprimirem  "Hello"  ****/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NTHREADS  5

/* Variaveis globais, sendo X a variável de sincronização para as threads 2,3 e 4; e Y a variável de sincronização para a thread 1*/
int x = 0;
pthread_mutex_t x_mutex;
pthread_cond_t x_cond;
int y = 0;
pthread_mutex_t y_mutex;
pthread_cond_t y_cond;

void *tarefa (void *arg){
   long long int id = (long long int) arg; //identificador da thread
    switch(id){
       case 1:
       if (y < 3) { 
            printf("1: y= %d, vai se bloquear...\n", y);
            pthread_cond_wait(&y_cond, &y_mutex);
            printf("1: sinal recebido e mutex realocado, y = %d\n", y);
        }
        printf("Volte sempre!");
        break;

       case 2:
       if (x < 1) { 
            printf("2: x= %d, vai se bloquear...\n", x);
            pthread_cond_wait(&x_cond, &x_mutex);
            printf("2: sinal recebido e mutex realocado, x = %d\n", x);
        }
        printf("Fique a vontade.");
        pthread_mutex_lock(&y_mutex);
        y++;
        pthread_mutex_unlock(&y_mutex);
        if (y==3) {
            printf("2:  y = %d, vai sinalizar a condicao \n", y);
            pthread_cond_signal(&y_cond);
        }
        pthread_exit(NULL);
        break; 

       case 3:
       if (x < 1) { 
            printf("3: x= %d, vai se bloquear...\n", x);
            pthread_cond_wait(&x_cond, &x_mutex);
            printf("3: sinal recebido e mutex realocado, x = %d\n", x);
        }
        printf("Sente-se por favor.");
        pthread_mutex_lock(&y_mutex);
        y++;
        pthread_mutex_unlock(&y_mutex);
        if (y==3) {
        printf("3:  y = %d, vai sinalizar a condicao \n", y);
        pthread_cond_signal(&y_cond);
        }
        pthread_mutex_unlock(&y_mutex);
        pthread_exit(NULL);
        break; 

       case 4:
       if (x < 1) { 
            printf("4: x= %d, vai se bloquear...\n", x);
            pthread_cond_wait(&x_cond, &x_mutex);
            printf("4: sinal recebido e mutex realocado, x = %d\n", x);
        }
        printf("Aceita um copo d’agua?.");
        pthread_mutex_lock(&y_mutex);
        y++;
        pthread_mutex_unlock(&y_mutex);
        if (y==3) {
            printf("4:  y = %d, vai sinalizar a condicao \n", y);
            pthread_cond_signal(&y_cond);
        }
       pthread_exit(NULL);
       break; 

       case 5:
       printf("Seja bem-vindo!");
       pthread_mutex_lock(&x_mutex);
       x++;
       pthread_mutex_unlock(&x_mutex);
       pthread_cond_broadcast(&x_cond);
       pthread_exit(NULL);
       break; 
    }
}


/* Funcao principal */
int main(int argc, char *argv[]) {
  int i; 
  pthread_t *tid; //identificadores das threads no sistema

  /* Inicilaiza o mutex (lock de exclusao mutua) e a variavel de condicao */
  pthread_mutex_init(&x_mutex, NULL);
  pthread_cond_init (&x_cond, NULL);
  pthread_mutex_init(&y_mutex, NULL);
  pthread_cond_init (&y_cond, NULL);

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
  printf ("\nFIM\n");

  /* Desaloca variaveis e termina */
  pthread_mutex_destroy(&x_mutex);
  pthread_cond_destroy(&x_cond);
  pthread_mutex_destroy(&y_mutex);
  pthread_cond_destroy(&y_cond);

}
