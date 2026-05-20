#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define TAM_MAILBOX 12

int canal[8];

// 0 -> A
// 1 -> B
// 2 -> C
// 3 -> D
// 4 -> E
// 5 -> F
// 6 -> G
// 7 -> H

sem_t sem_send[8];
sem_t sem_rec[8];


int mailbox[TAM_MAILBOX];
int indice_mailbox = 0;


int converte_valor(unsigned char bytex2, unsigned char bytex1)
{
    return (bytex2 << 8) | bytex1;
}


void encontra_bytes(int valor,
                    unsigned char *byte2,
                    unsigned char *byte1)
{
    *byte2 = (valor >> 8) & 0xFF;
    *byte1 = valor & 0xFF;
}

void send_sync(int *buf, int c)
{
    canal[c] = *buf;

    sem_post(&sem_send[c]);

    sem_wait(&sem_rec[c]);
}

void receive_sync(int *buf, int c)
{
    sem_wait(&sem_send[c]);

    *buf = canal[c];

    sem_post(&sem_rec[c]);
}


void *thread_A(void *arg)
{
    while(1)
    {
        int msg;

        msg = converte_valor('E', 10);

        printf("A enviando mensagem\n");

        send_sync(&msg, 0);

        sleep(1);
    }

    return NULL;
}

void *thread_B(void *arg)
{
    while(1)
    {
        int msg;

        msg = converte_valor('F', 20);

        printf("B enviando mensagem\n");

        send_sync(&msg, 1);

        sleep(1);
    }

    return NULL;
}

void *thread_C(void *arg)
{
    while(1)
    {
        int msg;

        msg = converte_valor('G', 30);

        printf("C enviando mensagem\n");

        send_sync(&msg, 2);

        sleep(1);
    }

    return NULL;
}

void *thread_D(void *arg)
{
    while(1)
    {
        int msg;

        msg = converte_valor('H', 40);

        printf("D enviando mensagem\n");

        send_sync(&msg, 3);

        sleep(1);
    }

    return NULL;
}





void encaminha_mensagens()
{
    unsigned char destino;
    unsigned char valor;

    printf("\nMAILBOX CHEIA\n");
    printf("Encaminhando mensagens...\n\n");

    for(int i = 0; i < TAM_MAILBOX; i++)
    {
        encontra_bytes(mailbox[i],
                        &destino,
                        &valor);

        printf("Destino: %c | Valor: %d\n",
               destino,
               valor);

        int v = valor;

        if(destino == 'E')
        {
            send_sync(&v, 4);
        }

        else if(destino == 'F')
        {
            send_sync(&v, 5);
        }

        else if(destino == 'G')
        {
            send_sync(&v, 6);
        }

        else if(destino == 'H')
        {
            send_sync(&v, 7);
        }
    }

    printf("\nMensagens entregues\n\n");
}

void *thread_mailbox(void *arg)
{
    int msg;

    while(1)
    {
        for(int h = 0; h < 4; h++)
        {
            receive_sync(&msg, h);

            mailbox[indice_mailbox] = msg;

            printf("Mailbox armazenou mensagem[%d] = %d\n",
                   indice_mailbox,
                   msg);

            indice_mailbox++;

            
            if(indice_mailbox == TAM_MAILBOX)
            {
                encaminha_mensagens();

                indice_mailbox = 0;
            }
        }
    }

    return NULL;
}

void *thread_E(void *arg)
{
    while(1)
    {
        int valor;

        receive_sync(&valor, 4);

        printf("Thread E recebeu: %d\n", valor);
    }

    return NULL;
}

void *thread_F(void *arg)
{
    while(1)
    {
        int valor;

        receive_sync(&valor, 5);

        printf("Thread F recebeu: %d\n", valor);
    }

    return NULL;
}

void *thread_G(void *arg)
{
    while(1)
    {
        int valor;

        receive_sync(&valor, 6);

        printf("Thread G recebeu: %d\n", valor);
    }

    return NULL;
}

void *thread_H(void *arg)
{
    while(1)
    {
        int valor;

        receive_sync(&valor, 7);

        printf("Thread H recebeu: %d\n", valor);
    }

    return NULL;
}


int main()
{
    pthread_t A, B, C, D;
    pthread_t E, F, G, H;
    pthread_t mailbox_thread;

    
    for(int i = 0; i < 8; i++)
    {
        sem_init(&sem_send[i], 0, 0);
        sem_init(&sem_rec[i], 0, 0);
    }

    pthread_create(&E, NULL, thread_E, NULL);
    pthread_create(&F, NULL, thread_F, NULL);
    pthread_create(&G, NULL, thread_G, NULL);
    pthread_create(&H, NULL, thread_H, NULL);

   
    pthread_create(&mailbox_thread,
                   NULL,
                   thread_mailbox,
                   NULL);


    pthread_create(&A, NULL, thread_A, NULL);
    pthread_create(&B, NULL, thread_B, NULL);
    pthread_create(&C, NULL, thread_C, NULL);
    pthread_create(&D, NULL, thread_D, NULL);


    // pthread_join(A, NULL);
    // pthread_join(B, NULL);
    // pthread_join(C, NULL);
    // pthread_join(D, NULL);

    pthread_join(mailbox_thread, NULL);

    // pthread_join(E, NULL);
    // pthread_join(F, NULL);
    // pthread_join(G, NULL);
    // pthread_join(H, NULL);

    // for(int i = 0; i < 8; i++)
    // {
    //     sem_destroy(&sem_send[i]);
    //     sem_destroy(&sem_rec[i]);
    // }

    return 0;
}
