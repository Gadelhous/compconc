/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Codigo: Laboratório 6 */

/***** Condicao logica da aplicacao: a cada iteracao, as threads somam os valores do vetor, reescrevem um valor aleatório no v[id da thread], esperam todas as threads realizarem essa operação e passam pela barreira ****/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NTHREADS  5  // 

/* Variaveis globais */
int bloqueadas = 0;
int vetor[NTHREADS];
int testeResultado[NTHREADS];
pthread_mutex_t x_mutex;
pthread_cond_t x_cond;

/*funcao para a soma local
int soma(){
  int a;
  for(int i = 0;i<NTHREADS;i++){
      a += vetor[i];
    }
  return a;
}
*/

/*funcao para printar o vetor da iteração N
void pvet(){
  for(int i=0;i<NTHREADS;i++){
    printf("%d", vetor[i]);
    puts("");
  }
}
*/

//funcao barreira
void barreira(int nthreads,int b) {
  pthread_mutex_lock(&x_mutex); //inicio secao critica
  if (bloqueadas == (nthreads-1)) { 
    //ultima thread a chegar na barreira
    pthread_cond_broadcast(&x_cond);
    //pvet();
    //printf("PASSOU!\n");
    bloqueadas=0;
  }else{
    bloqueadas++;
    pthread_cond_wait(&x_cond, &x_mutex);
  }
  pthread_mutex_unlock(&x_mutex); //fim secao critica
}

//funcao das threads
void *tarefa (void *arg) {
  int id = *(int*)arg;
  int sl; //variável local para o somatório dos valores do vetor
  int contador = NTHREADS;
  int *retorno;

  //aloca o retorno
  retorno = (int*) malloc(sizeof(int));
  if(retorno==NULL) {exit(1);}

  //somando os elementos do vetor
  for(int j = 0; j<NTHREADS;j++){
    for(int i = 0;i<NTHREADS;i++){
      sl += vetor[i];
    }
    //sl = soma();
    printf("A thread %d encontrou %d como soma total na iteração %d\n",id, sl,j+1);
    //sincronizacao condicional
    barreira(NTHREADS,id);
    /*for(int i = 0;i<NTHREADS;i++){
      printf("%d\n", vetor[i]);
    }*/

    vetor[id] = rand() % 10;
    //printf("%d\n", vetor[id]);
    barreira(NTHREADS,id);
  }
  *retorno = sl;
  pthread_exit((void *) retorno);
}

/* Funcao principal */
int main(int argc, char *argv[]) {
  pthread_t threads[NTHREADS];
  int id[NTHREADS];
  int *retorno;
  //inicializando o vetor
  for(int i = 0;i<NTHREADS;i++){
    vetor[i] = rand() % 10;
    //printf("%d\n", vetor[i]);
  }

  /* Inicilaiza o mutex (lock de exclusao mutua) e a variavel de condicao */
  pthread_mutex_init(&x_mutex, NULL);
  pthread_cond_init (&x_cond, NULL);

  /* Cria as threads */
  for(int i=0;i<NTHREADS;i++) {
     id[i]=i;
     pthread_create(&threads[i], NULL, tarefa, (void *) &id[i]);
  }

  /* Espera todas as threads completarem */
  for (int i = 0; i < NTHREADS; i++) {
    if(pthread_join(threads[i], (void**) &retorno)){
      fprintf(stderr, "ERRO--pthread_create\n");
      return 3;
    }
    //contador global
    testeResultado[i] += *retorno;
  }
  //printf("%d", testeResultado/NTHREADS);
  for(int i = 0; i<NTHREADS-1;i++){
    if(testeResultado[i] != testeResultado[i+1]){
      printf("ERRO -- as threads encontratam valores diferentes");
    }else{
      printf("As threads %d e %d encontraram %d como resultado.\n",i,i+1,testeResultado[i]);
    }
    
  }
  printf ("FIM.\n");

  /* Desaloca variaveis e termina */
  pthread_mutex_destroy(&x_mutex);
  pthread_cond_destroy(&x_cond);
  return 0;
}
