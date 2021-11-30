/* Multiplicacao de matriz-matriz (considerando matrizes quadradas) */
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include "timer.h"

float *mat1; //primeira matriz de entrada
float *mat2; //segunda matriz de entrada
float *rconc; //matriz resultante da implementação concorrente
float *rseq; // resultado a parte para a implementação sequencial
int nthreads; //numero de threads

typedef struct{
   int id; //identificador do elemento que a thread ira processar
   int dim; //dimensao das estruturas de entrada
} tArgs;

//funcao das threads que executará a multiplicação das matrizes
void * multConc(void *arg) {
   tArgs *args = (tArgs*) arg;
   //printf("Thread %d\n", args->id);
   for(int i=args->id; i<args->dim; i+=nthreads){
      for(int j=0; j<args->dim; j++) {
         for(int k=0; k<args->dim;k++)
            rconc[i*(args->dim)+j] += mat1[i*(args->dim) + k] * mat2[k*(args->dim) +j];
      }   
   }
   pthread_exit(NULL);
}

void  multSeq(float *a, float *b,int n){
   //multiplicacao sequencial da matriz 1 pela matriz 2
   for(int i=0;i<n;i++){
      for(int j=0;j<n;j++){
         for(int k=0;k<n;k++)
         rseq[i*n+j] += a[i*n+k] * b[k*n+j];
      }
   }

}

int checaMatriz(float *a, float *b,int n){
   for(int i = 0; i<n;i++){
      for(int j = 0; j<n;j++){
         if(rconc[i*n+j] !=  rseq[i*n+j]){
            return 2;
         }   
      }
   }
   return 1;
}
//fluxo principal
int main(int argc, char* argv[]) {
   int dim; //dimensao da matriz de entrada
   pthread_t *tid; //identificadores das threads no sistema
   tArgs *args; //identificadores locais das threads e dimensao
   double inicio, fim, delta;
   
   GET_TIME(inicio);
   //leitura e avaliacao dos parametros de entrada
   if(argc<3) {
      printf("Digite: %s <dimensao da matriz> <numero de threads>\n", argv[0]);
      return 1;
   }
   dim = atoi(argv[1]);
   nthreads = atoi(argv[2]);
   if (nthreads > dim) nthreads=dim;

   //alocacao de memoria para as estruturas de dados
   mat1 = (float *) malloc(sizeof(float) * dim * dim);
   if (mat1 == NULL) {printf("ERRO--malloc\n"); return 2;}
   mat2 = (float *) malloc(sizeof(float) * dim *dim);
   if (mat2 == NULL) {printf("ERRO--malloc\n"); return 2;}
   rconc = (float *) malloc(sizeof(float) * dim * dim);
   if (rconc == NULL) {printf("ERRO--malloc\n"); return 2;}
   rseq = (float *) malloc(sizeof(float) * dim * dim);
   if(rseq == NULL) {printf("ERRO--malloc\n"); return 2;}

   //inicializacao das estruturas de dados de entrada e saida
   for(int i=0; i<dim; i++) {
      for(int j=0; j<dim; j++){
         mat1[i*dim+j] = rand();    //equivalente mat1[i][j]
         mat2[i*dim+j] = rand();    //equivalente mat2[i][j]    
         rconc[i*dim+j] = 0;
         rseq[i*dim+j] = 0;
      }
   }
   GET_TIME(fim);
   delta = fim - inicio;
   //printf("Tempo inicializacao:%lf\n", delta);
   
   GET_TIME(inicio);
   //multiplicacao sequencial da matriz 1 pela matriz 2
   multSeq(mat1,mat2,dim);
  
   GET_TIME(fim);
   delta = fim - inicio;
   printf("Tempo para a multiplicacao sequencial: %lf\n", delta);


   //multiplicacao concorrente da matriz 1 pela matriz 2
   GET_TIME(inicio);
   //alocacao das estruturas
   tid = (pthread_t*) malloc(sizeof(pthread_t)*nthreads);
   if(tid==NULL) {puts("ERRO--malloc"); return 2;}
   args = (tArgs*) malloc(sizeof(tArgs)*nthreads);
   if(args==NULL) {puts("ERRO--malloc"); return 2;}
   //criacao das threads
   for(int i=0; i<nthreads; i++) {
      (args+i)->id = i;
      (args+i)->dim = dim;
      if(pthread_create(tid+i, NULL, multConc, (void*) (args+i))){
         puts("ERRO--pthread_create"); return 3;
      }
   } 
   //espera pelo termino da threads
   for(int i=0; i<nthreads; i++) {
      pthread_join(*(tid+i), NULL);
   }
   GET_TIME(fim)   
   delta = fim - inicio;
   printf("Tempo para a multiplicacao concorrente :%lf\n", delta);

   //exibicao dos resultados
   /*
   puts("Matriz 1:");
   for(int i= 0;i<dim; i++){
      for(int j=0; j<dim; j++)
         printf("%.1f ", mat1[i*dim+j]);
      puts("");
   }
   puts("");

   puts("Matriz 2:");
   for(int i= 0;i<dim; i++){
      for(int j=0; j<dim; j++)
         printf("%.1f ", mat2[i*dim+j]);
      puts("");
   }
   puts("");

   puts("Resultante da forma sequencial:");
   for(int i= 0;i<dim; i++){
      for(int j=0; j<dim; j++)
         printf("%.1f ", rseq[i*dim+j]);
      puts("");
   }
   puts("");

   puts("Matriz de saida concorrente:");
   for(int i= 0;i<dim; i++){
      for(int j=0; j<dim; j++)
         printf("%.1f ", rconc[i*dim+j]);
      puts("");
   }
   puts("");
   
   */

   //função que compara as saidas
   checaMatriz(mat1,mat2,dim) == 1 ? puts("As matrizes são iguais.") : puts("ERRO-- matrizes diferentes");
   /*
      case 1:
         puts("As matrizes são iguais.");
         break;
      case 2:
         puts("ERRO-- matrizes diferentes");
         break;
   }
   */

   //liberacao da memoria
   GET_TIME(inicio);
   free(mat1);
   free(mat2);
   free(rconc);
   free(rseq);
   free(args);
   free(tid);
   GET_TIME(fim)   
   delta = fim - inicio;
   //printf("Tempo finalizacao:%lf\n", delta);

   return 0;
}

