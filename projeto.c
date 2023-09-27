// Raul Vilhora Cardoso Matias, 32267274
// Bruno Lauand Ferrão, 32217994

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define FIBER_STACK 1024 * 64

struct c {
    int saldo;
};
typedef struct c conta;

conta from, to;
int valor;
pthread_mutex_t lock;

void *transferencia(void *arg)
{
    int *thread_num = (int *)arg;
    for (int i = 0; i < 10; i++)
    {
        pthread_mutex_lock(&lock);
        if (from.saldo >= valor)
        {
            from.saldo -= valor;
            to.saldo += valor;
            printf("Transferência %d concluída com sucesso!\n", *thread_num);
            printf("Saldo de c1: %d\n", from.saldo);
            printf("Saldo de c2: %d\n", to.saldo);
        }
        else
        {
            printf("Transferência %d falhou. Saldo insuficiente em c1.\n", *thread_num);
        }
        pthread_mutex_unlock(&lock);
        usleep(100000);
    }
    return NULL;
}

int main()
{
    pthread_t threads[100];
    int i, num_threads;

    pthread_mutex_init(&lock, NULL);
    from.saldo = 100;
    to.saldo = 100;

    printf("Informe a quantidade de transferências (no máximo 100): ");
    scanf("%d", &num_threads);

    if (num_threads < 1)
    {
        printf("Quantidade inválida. Pelo menos uma transferência é necessária.\n");
        return 1;
    }

    if (num_threads > 100)
    {
        printf("A quantidade de transferências foi limitada a 100.\n");
        num_threads = 100;
    }

    valor = 10;

    for (i = 0; i < num_threads; i++)
    {
        if (pthread_create(&threads[i], NULL, transferencia, &i) != 0)
        {
            perror("pthread_create");
            exit(2);
        }
    }

    for (i = 0; i < num_threads; i++)
    {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&lock);
    printf("Transferências concluídas.\n");

    return 0;
}
