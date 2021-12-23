/* Disciplina: Computacao Concorrente */
/***** Condicao logica da aplicacao: a thread 2 so podera imprimir a sentenca X=... quando x for multiplo de 100 *****/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NTHREADS  2
#define N  500

/* Variaveis globais */
int contador = 0;
pthread_mutex_t x_mutex;
pthread_cond_t x_cond;
pthread_mutex_t y_mutex;
pthread_cond_t y_cond;

/* Thread que incrementa x */
void *Thread1 (void *arg) {
  
  for (int i=0; i < N; i++) {
    pthread_mutex_lock(&x_mutex);
    if ((contador%100) == 0) {
      //printf("cont = %d\n", contador);
      pthread_cond_signal(&x_cond);
      //pthread_cond_wait(&y_cond, &y_mutex);
    }
    contador++;
    pthread_mutex_unlock(&x_mutex);

  }
   
  pthread_exit(NULL);
}

/* Thread que imprime x */
void *Thread2 (void *arg) {
  pthread_mutex_lock(&x_mutex);
  while ((contador % 100) != 0) {
  
    pthread_cond_wait(&x_cond, &x_mutex);
     
  }
  printf("Contador=%d\n", contador);
  //pthread_cond_signal(&y_cond);
  pthread_mutex_unlock(&x_mutex); 
  pthread_exit(NULL);
}


/* Funcao principal */
int main(int argc, char *argv[]) {
  int i; 
  int *t1, *t2, *t3;
  pthread_t threads[NTHREADS];
  //aloca espaco para os identificadores das threads
  t1=malloc(sizeof(int)); 
  t2=malloc(sizeof(int)); 
  *t1=1, *t2=2;

  /* Inicilaiza o mutex (lock de exclusao mutua) e a variavel de condicao */
  pthread_mutex_init(&x_mutex, NULL);
  pthread_cond_init (&x_cond, NULL);
   pthread_mutex_init(&y_mutex, NULL);
  pthread_cond_init (&y_cond, NULL);
  

  /* Cria as threads */
  pthread_create(&threads[0], NULL, Thread1, (void *)t1);
  pthread_create(&threads[1], NULL, Thread2, (void *)t2);

  /* Espera todas as threads completarem */
  for (i = 0; i < NTHREADS; i++) {
    pthread_join(threads[i], NULL);
  }
  printf ("FIM.\n");

  /* Desaloca variaveis e termina */
  pthread_mutex_destroy(&x_mutex);
  pthread_cond_destroy(&x_cond);
  pthread_mutex_destroy(&y_mutex);
  pthread_cond_destroy(&y_cond);
}

