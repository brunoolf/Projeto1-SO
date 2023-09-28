// Raul Vilhora Cardoso Matias, 32267274
// Bruno Lauand Ferrão, 32217994

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

// Esta parte das funções foi feita pelo aluno Raul Vilhora

#define MAX_THREADS 100
#define MIN_THREADS 1

typedef struct {
  int saldo;  
} Conta;

Conta conta1, conta2;
int valorTransacao;
int direcaoTransferencia;

pthread_mutex_t lock;

void* transferencia(void* arg) {

  int dir = (int)arg;

  pthread_mutex_lock(&lock);

  if (dir == 1) {
    if (conta1.saldo >= valorTransacao) {
      conta1.saldo -= valorTransacao;
      conta2.saldo += valorTransacao;
      printf("Transferência Conta 1 -> Conta 2\n");
    } else {
      printf("Saldo insuficiente Conta 1\n");
    }
  } else {
    if (conta2.saldo >= valorTransacao) {
      conta2.saldo -= valorTransacao;
      conta1.saldo += valorTransacao;
      printf("Transferência Conta 2 -> Conta 1\n");
    } else {
      printf("Saldo insuficiente Conta 2\n");
    }
  }

  printf("Saldo Conta 1: %d\n", conta1.saldo);
  printf("Saldo Conta 2: %d\n", conta2.saldo);
  
  pthread_mutex_unlock(&lock);

  return NULL;
}

// A main ficou responsável pelo Bruno Lauand

int main() {

  int numThreads;

  conta1.saldo = 100;
  conta2.saldo = 100;

  printf("Informe o número de transações: ");
  scanf("%d", &numThreads);

  if (numThreads > MAX_THREADS) {
    printf("Número máximo de transações é %d\n", MAX_THREADS);
    return 1;
  }
  if (numThreads < MIN_THREADS) {
    printf("Número mínimo de transações é %d\n", MIN_THREADS);
    return 1;
  }
  
  printf("Informe o valor da transação: ");
  scanf("%d", &valorTransacao);
  

  printf("Informe a direção da transferência:\n");
  printf("1. Conta 1 -> Conta 2\n");
  printf("2. Conta 2 -> Conta 1\n");
  scanf("%d", &direcaoTransferencia);

  pthread_mutex_init(&lock, NULL);

  pthread_t threads[numThreads];

  for (int i = 0; i < numThreads; i++) {
    if (pthread_create(&threads[i], NULL, transferencia, &direcaoTransferencia) != 0) {
      return 2;
    }
  }

  for (int i = 0; i < numThreads; i++) {
    pthread_join(threads[i], NULL);  
  }

  pthread_mutex_destroy(&lock);

  return 0;
}