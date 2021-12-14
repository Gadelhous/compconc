//Soma todos os elementos de um vetor de float
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include "timer.h"

long long int N,Li,Ls; //dimensao do vetor de entrada
int nthreads; //numero de threads, limite superior e limite inferior
float *vetor; //vetor de entrada com dimensao N 
long long int contadorConc;


//fluxo das threads
void * contaConc(void * arg) {
   long int id = (long int) arg; //identificador da thread
   float *contadorLocal; //variavel local do contador
   contadorLocal = (float*) malloc(sizeof(float));
   if(contadorLocal==NULL) {exit(1);}
   long int tamBloco = N/nthreads;  //tamanho do bloco de cada thread 
   long int ini = id * tamBloco; //elemento inicial do bloco da thread
   long int fim; //elemento final(nao processado) do bloco da thread
   if(id == nthreads-1) fim = N;
   else fim = ini + tamBloco; //trata o resto se houver
   //incrementa a lógica do contador nos elementos do bloco da thread
   for(long int i=ini; i<fim; i++){
      if(vetor[i]>Li && Ls>vetor[i]){
         *contadorLocal+=1;         // se Li<x<Ls, acrescenta no contador
      }
   }      
   //retorna o resultado do contador  local
   pthread_exit((void *) contadorLocal); 
}

//função que verifica se os números estão entre os limiares; utilizada na implementação sequencial
int contaSeq(float *vetor){
   long long int a = 0;
   for(long long int i=0; i<N; i++){
      if(vetor[i]>Li && vetor[i]<Ls){
         a++;    // se Li<x<Ls, acrescenta no contador
         printf("%lf\n", vetor[i]);
      }
   }
   return a;
}

//função para checar se os dois contadores são iguais
int checaResultado(long long int a,long long int b){
   if(a!=b)
      return 2;
   else
      return 1;
}
//fluxo principal
int main(int argc, char *argv[]) {
   long long int contadorSeq= 0; //soma sequencial
   //float somaConc= 0; //soma concorrente
   double ini, fim; //tomada de tempo
   pthread_t *tid; //identificadores das threads no sistema
   float *retorno; //valor de retorno das threads

   //recebe e valida os parametros de entrada (dimensao do vetor, numero de threads)
   if(argc < 3) {
       fprintf(stderr, "Digite: %s <dimensao do vetor> <numero threads>\n", argv[0]);
       return 1; 
   }
   N = atoll(argv[1]);
   nthreads = atoi(argv[2]);
   //aloca o vetor de entrada
   vetor = (float*) malloc(sizeof(float)*N);
   if(vetor == NULL) {
      fprintf(stderr, "ERRO--malloc\n");
      return 2;
   }
   //preenche o vetor de entrada
   for(long long int i=0; i<N; i++){
      vetor[i] = ((float)rand()/(float)(RAND_MAX)) * 100000000.0; // gerando valor float aleatório para a posição i do vetor; a lógica dá um número entre 0 e 1,que depois é multiplicado por um float, para termos uma distribuição maior
      //printf("%lf\n", vetor[i]);
   }

   //recebe os limiares pela entrada padrão
   puts("Entre o limiar inferior e superior, respectivamente:");
   scanf("%lli %lli", &Li,&Ls);
   
   //soma sequencial dos elementos
   GET_TIME(ini);
   contadorSeq = contaSeq(vetor);
   GET_TIME(fim);
   printf("Tempo sequencial:  %lf\n", fim-ini);

   //soma concorrente dos elementos
   GET_TIME(ini);
   tid = (pthread_t *) malloc(sizeof(pthread_t) * nthreads);
   if(tid==NULL) {
      fprintf(stderr, "ERRO--malloc\n");
      return 2;
   }
   //criar as threads
   for(long int i=0; i<nthreads; i++) {
      if(pthread_create(tid+i, NULL, contaConc, (void*) i)){
         fprintf(stderr, "ERRO--pthread_create\n");
         return 3;
      }
   }
   //aguardar o termino das threads
   for(long int i=0; i<nthreads; i++) {
      if(pthread_join(*(tid+i), (void**) &retorno)){
         fprintf(stderr, "ERRO--pthread_create\n");
         return 3;
      }
      //contador global
      contadorConc += *retorno;
   }
   GET_TIME(fim);
   printf("Tempo concorrente:  %lf\n", fim-ini);

   //exibir os resultados
   printf("Contador Sequencial:  %lli\n", contadorSeq);
   printf("Contador Concorrente: %lli\n", contadorConc);

   //função que checa se os resultados são iguais
   checaResultado(contadorSeq,contadorConc) == 1 ? puts("As duas implementações dão o mesmo resultado.") : puts("ERRO-- contadores diferentes");

   //libera as areas de memoria alocadas
   free(vetor);
   free(tid);

   return 0;
}
