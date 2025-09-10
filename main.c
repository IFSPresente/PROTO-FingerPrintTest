#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <libmeuprojeto.h>
#include "pgmformat.h"

#define NANO_PER_SEC 1000000000.0

int main(int argc, char** argv)
{
    //run_main(argc, argv);
    struct timespec now;
    struct PgmType pgm1, pgm2;
    int retLido =  pgmRead (argv[1], &pgm1);
    retLido = pgmRead(argv[2], &pgm2);

    graal_isolate_t *isolate = NULL;
    graal_isolatethread_t *thread = NULL;
    if (graal_create_isolate(NULL, &isolate, &thread) != 0) {
        fprintf(stderr, "initialization error\n");
        return 1;
    }

    printf("Início\n");
    zera_lista(thread);
    char serializado[30000];
    double similaridade = 0.0;
    int size = 0;

    clock_gettime(CLOCK_REALTIME, &now);
    double antes = now.tv_sec + now.tv_nsec / NANO_PER_SEC;
    double depois = 0.0;
    int sucesso = enroll(thread, pgm2.bytes, pgm2.width, pgm2.height, serializado, &size);
    for (int i =0; i < 1000; i++) {
         insere_lista(thread, serializado, size);
    }
    clock_gettime(CLOCK_REALTIME, &now);
    depois = now.tv_sec + now.tv_nsec / NANO_PER_SEC;
    printf("Tempo inserindo %lf\n", depois - antes);
    printf("Tamanho serializado = %d\n", size);
    clock_gettime(CLOCK_REALTIME, &now);
    antes = now.tv_sec + now.tv_nsec / NANO_PER_SEC;
    int posicao =  match_na_lista(thread, pgm1.bytes, pgm1.width, pgm1.height, 35.0, &similaridade);
    clock_gettime(CLOCK_REALTIME, &now);
    depois = now.tv_sec + now.tv_nsec / NANO_PER_SEC;
    printf("Tempo pesquisando %lf\n", depois - antes);
    if (posicao == -1) {
        printf("Não conseguiu match\n");
    }
    else {
        printf("Match do indice %d\n", posicao);
        printf("Similaridade %lf\n", similaridade);
    }


    for (int i=0; i < 1; i++) {
      double result = match(thread, pgm1.bytes, pgm1.width, pgm1.height, pgm2.bytes, pgm2.width, pgm2.height);
      printf("result = %lf\n", result);
    }
    //run_main(argc, argv);
    return 0;
}
