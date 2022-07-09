#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

#define L 3
#define E 3
#define NTHREADS L+E

sem_t em_e, em_l, escr, leit;
int e=0, l=0;

void *leitores (void *args){
   int id = *(int *)args;
   while(1) {
      printf("Leitor %d vai tentar ler\n", id);
      sem_wait(&leit);
      sem_wait(&em_l);
      l++;
      if(l==1){
         printf("Leitor %d vai decrementar escr\n", id);
         sem_wait(&escr);
         printf("Leitor %d conseguiu decrementar escr=%d agora\n", id, escr);
      }
      sem_post(&em_l);
      sem_post(&leit);
      printf("Leitor %d está lendo agora\n", id);
      /* Faz a leitura de algo */
      sleep(1);
      printf("Leitor %d terminou de ler\n", id);
      sem_wait(&em_l);
      l--;
      if(l==0){
         printf("Leitor %d vai incrementar escr\n", id);
         sem_post(&escr);
         printf("Leitor %d consegiu incrementar escr=%d\n", id, escr);
      }
      sem_post(&em_l);
   }
   pthread_exit(NULL);
}

void *escritores (void *args){
   int id = *(int *)args;
   while(1){
      printf("Escritor %d vai tentar escrever -\n", id);
      sem_wait(&em_e);
      e++;
      if(e==1){
         printf("Escritor %d vai decrementar leit\n", id);
         sem_wait(&leit);
         printf("Escritor %d consegiu decrementar leit=%d agora\n", id, leit);
      }
      sem_post(&em_e);
      sem_wait(&escr);
      printf("Escritor %d está escrevendo agora -\n", id);
      /* Faz a escrita de algo */
      sleep(1);
      printf("Escritor %d terminou de escrever -\n", id);
      sem_post(&escr);
      sem_wait(&em_e);
      e--;
      if(e==0){
         printf("Escritor %d vai incrementar leit\n", id);
         sem_post(&leit);
         printf("Escritor %d conseguiu incrementar leit=%d agora\n", id,leit);
      }
      sem_post(&em_e);
   }
   pthread_exit(NULL);
}

int main(void){
   pthread_t tid[NTHREADS];
   int id[NTHREADS];
   
   //inicia os semaforos
   sem_init(&em_e, 0, 1);
   sem_init(&em_l, 0, 1);
   sem_init(&leit, 0, 1);
   sem_init(&escr, 0, 1);

   //cria as threads
  for (int t=0; t<E; t++) {
     id[t]=t;
     if (pthread_create(&tid[t], NULL, escritores, (void *)&id[t])){ printf("--ERRO: pthread_create()\n"); exit(-1); }
  }
  sleep(1);
  for (int t=L; t<NTHREADS; t++) {
     id[t]=t-L;
     if (pthread_create(&tid[t], NULL, leitores, (void *)&id[t])) { printf("--ERRO: pthread_create()\n"); exit(-1); }
  }  
  //--espera todas as threads terminarem
  for (int t=0; t<NTHREADS; t++) {
    if (pthread_join(tid[t], NULL)) {
         printf("--ERRO: pthread_join() \n"); exit(-1); 
    }
  }
  
   return 0;
}
