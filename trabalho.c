#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include "timer.h"

int   *vet1; //vetor a ser ordenado pelo método concorrente
int   *vet2; //vetor a ser ordenado pelo método sequencial
int nthreads; //numero de threads
long long int dim;    //dimensao do vetor de entrada

typedef struct{
   int id; //identificador do elemento que a thread ira processar
   long long int tam; //dimensão das threads
} tArgs;

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

//lógica básica que troca a primeira posição com a posterior caso ela seja maior
/*
void troca(int a){
   if( (a+1) == dim){
      puts("fon");
   }
   int b,aux;
   b = a+1;
   aux = vet1[b];
   vet1[b] = vet1[a];
   vet1[a] = aux;
}
*/

/*
int checaVetor(vet *a, vet *b, int n){
   for(int i = 0; i<n;i++){
         if(vet1[i] !=  vet2[i]){
            return 2;
         }   
      }
   }
   return 1;
}
*/

void sortFinal (int *v,int inicio,int meio,int fim){
    int *aux,p1,p2,tam, i,j,k;
    int fim1 =0 , fim2=0;
    tam = fim - inicio   ;
    p1=inicio;
    p2=meio;
    aux=(int *) malloc (tam*sizeof(int));
    if(aux!= NULL){
        for(i=0; i<tam;i++){
            if(!fim1 && !fim2){
                if(v[p1]<v[p2]){
                    aux[i]=v[p1++];
                    //printf("temp recebe %d na posição %d %d",aux[i],i,v[p1]);
                    //printf("\n");
                }
                else{
                    aux[i]=v[p2++];
                    //printf("temp recebe %d na posição %d %d",aux[i],i,v[p2]);
                    //printf("\n");
                }
                if(p1>=meio)
                    fim1=1;
                if(p2>=fim)
                    fim2=1;
            }else{
                if(!fim1){
                    aux[i]=v[p1++];
                    //printf("temp recebe %d na posição %d %d",aux[i],i,v[p1]);
                    //printf("\n");
                }
                else{
                    aux[i]=v[p2++];
                    //printf("temp recebe %d na posição %d %d",aux[i],i,v[p2]);
                    //printf("\n");
                }
            }
        }
        for(j=0,k=inicio;j<tam;j++,k++){
            v[k]=aux[j];
            }

    }
    free(aux);
}


/*"merge" serguindo a lógica (início,meio,fim)     /////// (inicio1, final1, inicio2, final2)  ?como passar parâmetros de threads diferentes? -> fixando nthreads e dim em potências de 2
void sortFinal(int a,int n1,int n2){
   int b = n1+1;
   int k = 0;
   int tamaux = n2+1; 
   int teste[tamaux];
   while(a <= n1 ||  b <= n2){
      if(a == n1){
         teste[k] = vet1[b];
         b++;
         k++;
      }
      else{
         if(b == n2){
            teste[k] = vet1[a];
            a++;
            k++;
         }
         if(vet1[a]<=vet1[b]){
            teste[k] = vet1[a];
            a++;
            k++;

         }else{
            teste[k] = vet1[b];
            b++;
            k++;
         }
         
      }

   }
}
*/

//lógica que aplica um bubble sort fragmentado para cada thread
void* sortConc(void *arg){
   tArgs *args = (tArgs*) arg;
   /* printf("Thread %d\n", args->id); DEBUG */
   int tamBloco = dim/nthreads;  //tamanho do bloco de cada threadd
   int ini = args->id * tamBloco; //elemento inicial do bloco da thread
   int fim; //elemento final(nao processado) do bloco da thread
   int z;
   /* printf("Thread %d, tambloco %d, ini %d\n", args->id,tamBloco,ini); DEBUG*/
   //int tamMerge 
   int aux;
   if(args->tam == nthreads-1) fim = dim;
   else fim = ini + tamBloco; //trata o resto se houver
   /*printf("Fim: %d\n", fim); DEBUG */
   for(int i = ini; i<fim ; i++){
      for(int j = ini; j<fim-1;j++){
         if(vet1[j]>vet1[j+1]){
            aux = vet1[j];
            vet1[j] = vet1[j+1];
            vet1[j+1] = aux;
         }   
      }
   }
   pthread_exit(NULL);
}



//Lógica do Bubble sort sequencial
void sortSeq(int v[]){
   int aux = 0;
   for(int i = 0; i<dim-1;i++){
      for(int j = 0; j < dim-1; j++){
         if(v[j]>v[j+1]){
            //troca(j);
            aux = v[j];
            v[j] = v[j+1];
            v[j+1] = aux;
         }
      }  
   }
}



//fluxo principal
int main(int argc, char* argv[]) {
   pthread_t *tid; //identificadores das threads no sistema
   tArgs *args; //identificadores locais das threads e dimensao
   double inicio, fim, delta,tConc,tSeq; //variáveis para a checagem do tempo
   int ajuda;
   
   //GET_TIME(inicio);
   //leitura e avaliacao dos parametros de entrada
   if(argc<3) {
      printf("Digite: %s <dimensao do vetor> <numero de threads>\n", argv[0]);
      return 1;
   }
   dim = atoll(argv[1]);
   nthreads = atoi(argv[2]);
  
   //alocacao de memoria para as estruturas de dados
   vet1 = (int *) malloc(sizeof(int) * dim);
   if (vet1 == NULL) {printf("ERRO--malloc\n"); return 2;}
   vet2 = (int *) malloc(sizeof(int) * dim);
   if (vet2 == NULL) {printf("ERRO--malloc\n"); return 2;}


   //inicializa o vetor com números aleatórios
   for(int i = 0; i<dim;i++){
      ajuda = 10 - i;
      vet1[i] = ajuda;
      vet2[i] = ajuda;
   }


   puts("Vetor 1 antes do sort:");
   for(int i= 0;i<dim; i++){
         printf("%d ", vet1[i]);
   }
   puts("");

   puts("Vetor 2 antes do sort:");
   for(int i= 0;i<dim; i++){
         printf("%d ", vet2[i]);
   }
   puts("");

  
   //GET_TIME(fim);
   //delta = fim - inicio;
   //printf("Tempo inicializacao:%lf\n", delta);
   
   GET_TIME(inicio);
   //implementação sequencial do sort
   
   sortSeq(vet2);
  
   GET_TIME(fim);
   delta = fim - inicio;
   tSeq = delta;
   printf("Tempo para o sort sequencial: %lf\n", delta);

   puts("Vetor 2 depois do sort:");
   for(int i= 0;i<dim; i++){
         printf("%d ", vet2[i]);
   }
   puts("");

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
      (args+i)->tam = dim;
      if(pthread_create(tid+i, NULL, sortConc, (void*) (args+i))){
         puts("ERRO--pthread_create"); return 3;
      }
   } 

   //espera pelo termino da threads
   for(int i=0; i<nthreads; i++) {
      pthread_join(*(tid+i), NULL);
   }

   
  // sortFinal(vet1);
   
   GET_TIME(fim)   
   delta = fim - inicio;
   tConc = delta;
   printf("Tempo para o sort concorrente :%lf\n", delta);

   puts("Vetor 1 depois do sort:");
   for(int i= 0;i<dim; i++){
      printf("%d ", vet1[i]);
   }
   puts("");

   //exibicao dos resultados
   
   

   //função que compara as saidas
   //checaMatriz(mat1,mat2,dim) == 1 ? puts("As matrizes são iguais.") : puts("ERRO-- matrizes diferentes");
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
   //GET_TIME(inicio);
   
   //free(vet1);
   free(args);
   free(tid);
   
   //GET_TIME(fim)   
   //delta = fim - inicio;
   //printf("Tempo finalizacao:%lf\n", delta);
   printf("O desempenho da solucao foi de %lf\n", tSeq/tConc);

   return 0;
}
