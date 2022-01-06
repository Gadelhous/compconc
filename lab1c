#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

#define NTHREADS 2
#define tam_vet 10000

int vetor[tam_vet];

//funcao que a thread ira executar
void * tarefa (void * arg) {
   int ident = * (int *) arg;
   switch(ident){
       case 1:
       for(int i = 0; i < tam_vet/2; i++){
           vetor[i] *= vetor[i];
       }
       break;

       case 2:
       for(int i = tam_vet/2 ; i < tam_vet; i++){
           vetor[i] *= vetor[i];
       }
       break;
   }
  
  
   pthread_exit(NULL);
}

//funcao principal
int main(void) {
    pthread_t tid[NTHREADS]; //identificador da thread no sistema
    int ident[NTHREADS]; //identificador local da thread
    //inicializando o vetor
    for(int i = 0;i<tam_vet;i++){
        vetor[i] = i;
    }

    //cria as threads 
    for(int i=1; i<=NTHREADS; i++) {
       ident[i-1] = i;
       if (pthread_create(&tid[i-1], NULL, tarefa, (void *)&ident[i-1])) 
          printf("ERRO -- pthread_create\n");
    }
    //espera as threads terminarem 
    for(int i=0; i<NTHREADS; i++) {
       if (pthread_join(tid[i], NULL)) 
          printf("ERRO -- pthread_join\n");
    }
    //imprimir o vetor 
    for(int i=0; i<tam_vet; i++) 
       printf("%d ", vetor[i]);
    printf("\n");

    //imprime a mensagem de boas vindas
    printf("Ola, sou a thread principal!\n");

    /*faz o vetor cópia 
    int vet2[tam_vetor];
    for(int i = 0; i < tam_vetor){
        vet2[i] = i*i;
    }
    */

    //desvincula o termino da main do termino programa
    //pthread_exit(NULL);
    return 0;
}
