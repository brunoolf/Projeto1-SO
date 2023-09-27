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

        if (*thread_num == 1 && from.saldo >= valor)
        {
            from.saldo -= valor;
            to.saldo += valor;
            printf("Transferência %d de c1 para c2 no valor de R$ %d concluída com sucesso!\n", *thread_num, valor);
        }
        else if (*thread_num == 2 && to.saldo >= valor)
        {
            to.saldo -= valor;
            from.saldo += valor;
            printf("Transferência %d de c2 para c1 no valor de R$ %d concluída com sucesso!\n", *thread_num, valor);
        }
        else
        {
            printf("Transferência %d falhou. Saldo insuficiente ou direção inválida.\n", *thread_num);
        }

        printf("Saldo de c1: R$ %d\n", from.saldo);
        printf("Saldo de c2: R$ %d\n", to.saldo);

        pthread_mutex_unlock(&lock);
        usleep(100000);
    }
    return NULL;
}

int main()
{
    pthread_t threads[100];
    int i, num_threads, direction;

    pthread_mutex_init(&lock, NULL);
    from.saldo = 100;
    to.saldo = 100;

    printf("Informe a quantidade de transferências (no máximo 100): ");
    scanf("%d", &num_threads);

    if (num_threads < 1 || num_threads > 100)
    {
        printf("Quantidade inválida. A quantidade de transferências deve estar entre 1 e 100.\n");
        return 1;
    }

    printf("Escolha a direção da transferência:\n");
    printf("1. De c1 para c2\n");
    printf("2. De c2 para c1\n");
    printf("Informe a direção desejada (1 ou 2): ");
    scanf("%d", &direction);

    if (direction != 1 && direction != 2)
    {
        printf("Direção inválida. Escolha 1 para c1 para c2 ou 2 para c2 para c1.\n");
        return 1;
    }

    printf("Informe o valor da transação em reais: ");
    scanf("%d", &valor);

    for (i = 0; i < num_threads; i++)
    {
        if (pthread_create(&threads[i], NULL, transferencia, &direction) != 0)
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