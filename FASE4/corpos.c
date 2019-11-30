#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "corpos.h"

corpo * leitura(corpo *planeta, double *tempoSim, double *duracaoProjs, int *nCorpos){

    int i;
    FILE * file;
    char arquivoEntrada[20];
    corpo nave1, nave2;
    corpo *corpos;  

    printf("Digite o nome do arquivo que você deseja carregar : ");
    i = scanf("%s", arquivoEntrada);
    file = fopen(arquivoEntrada, "r"); 

    if (fscanf(file, "%lf %lf %lf", &(planeta->raio), &(planeta->massa), tempoSim) < 3)
        fprintf(stdout, "O arquivo esta defeituoso.\n");


    planeta->pos_x = 0;
    planeta->pos_y = 0;
    planeta->vel_x = 0;
    planeta->vel_y = 0;
    planeta->fr_y = 0;
    planeta->fr_x = 0;
    // utilizamos a nave1 e nave2 para podermos le-las antes da leitura do nCorpos e então fazer o malloc

    if(fscanf(file, "%s %lf %lf %lf %lf %lf", nave1.nome, &(nave1.massa), &(nave1.pos_x), &(nave1.pos_y), &(nave1.vel_x), &(nave1.vel_y)) < 6)
        fprintf(stdout, "O arquivo esta defeituoso.\n");
    if(fscanf(file, "%s %lf %lf %lf %lf %lf", nave2.nome, &(nave2.massa), &(nave2.pos_x), &(nave2.pos_y), &(nave2.vel_x), &(nave2.vel_y)) < 6)
        fprintf(stdout, "O arquivo esta defeituoso.\n");

    /* definindo como NULL o ponteiro para a imagem que representa o corpo */

    if(fscanf(file, "%d %lf", nCorpos, duracaoProjs) < 2)
        fprintf(stdout, "O arquivo esta defeituoso.\n");

    fclose(file);

    corpos = malloc((*nCorpos+2)*sizeof(corpo));

    // agora podemos atribuir as naves às posições no vetor

    corpos[0] = nave1;
    corpos[1] = nave2;

    /*for (i = 2; i < (*nCorpos + 2); i ++){
        if(fscanf(file, "%lf %lf %lf %lf %lf", &(corpos[i].massa), &(corpos[i].pos_x), &(corpos[i].pos_y), &(corpos[i].vel_x), &(corpos[i].vel_y)) < 5)
            fprintf(stdout, "O arquivo esta defeituoso.\n");
    }*/

    /*
    
    para a criação de projeteis a massa deles no vetor será -1 se a posição no vetor
    não é um projetil ativo

    */


    for (i = 2; i < *nCorpos + 2; i++){
        corpos[i].massa = -1;
        corpos[i].tempoVida = 0;

    }



    return corpos;
}

corpo *inicializaCorpos(corpo *planeta, int *nCorpos){

    corpo *corpos;
    int i;
 
    /* inicializa o planeta */

    planeta->massa = 6e27;
    planeta->pos_x = 0;
    planeta->pos_y = 0;
    planeta->vel_x = 0;
    planeta->vel_y = 0;
    planeta->fr_x = 0;
    planeta->fr_y = 0;
    planeta->raio = 88*TAM_TOTAL/600;    /*Esse raio foi calculado a partir da metade de pixels do comprimento do planeta, 'traduzindo' esse tamanho para a medida do jogo*/

    /* inicializa o vetor de corpos e determina 
        o número de projetei possiveis
    */

    *nCorpos = 20;
    corpos = malloc((*nCorpos+2)*sizeof(corpo));

    /* inicializa a nave 1 */

    strncpy(corpos[0].nome,"Corsinha Tunado", 15);
    corpos[0].massa = 10.29e+4;
    corpos[0].pos_x = -TAM_TOTAL/2 + WD_SPRITE*TAM_TOTAL/2/600;
    corpos[0].pos_y = -TAM_TOTAL/2 + H_SPRITE*TAM_TOTAL/2/600;
    corpos[0].fr_x = 0;
    corpos[0].fr_x = 0;
    corpos[0].vel_x = 0;
    corpos[0].vel_y = 0;
    corpos[0].vida = 100;
    corpos[0].raio = TAM_TOTAL*30/600;      /*Esse raio foi calculado a partir da metade de pixels do comprimento da nave, 'traduzindo' esse tamanho para a medida do jogo*/

    /* inicializa a nave 2 */

    strncpy(corpos[1].nome,"Saveiro Rebaixado", 17);
    corpos[1].massa = 11.3e+4;
    corpos[1].pos_x = TAM_TOTAL/2 - WD_SPRITE*TAM_TOTAL/2/600;
    corpos[1].pos_y = TAM_TOTAL/2 - H_SPRITE*TAM_TOTAL/2/600;
    corpos[1].fr_x = 0;
    corpos[1].fr_x = 0;
    corpos[1].vel_x = 0;
    corpos[1].vel_y = 0;
    corpos[1].vida = 100;
    corpos[1].raio = TAM_TOTAL*30/600;

    /* inicializa a massa dos projeiteis = -1 para
        indicar que eles não estão ativos */

    for (i = 2; i < *nCorpos + 2; i++){
        corpos[i].massa = -1;
        corpos[i].tempoVida = 0;
    }

    return corpos;

}