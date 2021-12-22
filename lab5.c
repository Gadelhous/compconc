/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Laboratório: 5 */
/* Codigo: Uso de variáveis de condição e suas operações básicas para sincronização por condição */

/***** Condicao logica da aplicacao: a thread 1 so pode imprimir após as threads 2,3 e 4, que só podem imprimir após a thread 5  ****/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NTHREADS  6

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
            pthread_mutex_lock(&y_mutex);
            if (y < 3) { 
                
                pthread_cond_wait(&y_cond, &y_mutex);
                
            }
            printf("Volte sempre!\n");
            pthread_mutex_unlock(&y_mutex);
            break;

        case 2:
            pthread_mutex_lock(&x_mutex);
            if (x < 1) { 
                
                pthread_cond_wait(&x_cond, &x_mutex);
               
            }
            printf("Fique a vontade.\n");
            pthread_mutex_lock(&y_mutex);
            y++;
            pthread_mutex_unlock(&y_mutex);
            if (y==3) {
                
                pthread_cond_signal(&y_cond);
            }
            pthread_mutex_unlock(&x_mutex);
            break; 

        case 3:
            pthread_mutex_lock(&x_mutex);
            if (x < 1) { 
               
                pthread_cond_wait(&x_cond, &x_mutex);
                
            }
            printf("Sente-se por favor.\n");
            pthread_mutex_lock(&y_mutex);
            y++;
            pthread_mutex_unlock(&y_mutex);
            if (y==3) {
                
                pthread_cond_signal(&y_cond);
            }
            pthread_mutex_unlock(&x_mutex);
            break; 

        case 4:
            pthread_mutex_lock(&x_mutex);
            if (x < 1) { 
                
                pthread_cond_wait(&x_cond, &x_mutex);
                
            }
            printf("Aceita um copo d’agua?.\n");
            pthread_mutex_lock(&y_mutex);
            y++;
            pthread_mutex_unlock(&y_mutex);
            if (y==3) {
                
                pthread_cond_signal(&y_cond);
            };
            pthread_mutex_unlock(&x_mutex);    
             break; 

        case 5:
            printf("Seja bem-vindo!\n");
            pthread_mutex_lock(&x_mutex);
            x++;
            pthread_mutex_unlock(&x_mutex);
            pthread_cond_broadcast(&x_cond);
            break; 

        case 6:
            printf("TESTE TESTE");
    pthread_exit(NULL);
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
 
  /* Desaloca variaveis e termina */
  pthread_mutex_destroy(&x_mutex);
  pthread_cond_destroy(&x_cond);
  pthread_mutex_destroy(&y_mutex);
  pthread_cond_destroy(&y_cond);

}
