#include "fisica.h"
#include "graphic.h"
#include "corpos.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define WD_SPRITE 80
#define H_SPRITE 90
#define WIDTH 600
#define HEIGHT 600


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

    planeta->massa = 6e+30;
    planeta->pos_x = 0;
    planeta->pos_y = 0;
    planeta->vel_x = 0;
    planeta->vel_y = 0;
    planeta->fr_x = 0;
    planeta->fr_y = 0;
    planeta->raio = 100*TAM_TOTAL/600;

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
    corpos[0].vel_x = 0;
    corpos[0].vida = 100;

    /* inicializa a nave 2 */

    strncpy(corpos[1].nome,"Saveiro Rebaixado", 17);
    corpos[1].massa = 11.3e+4;
    corpos[1].pos_x = TAM_TOTAL/2 - WD_SPRITE*TAM_TOTAL/2/600;
    corpos[1].pos_y = TAM_TOTAL/2 - H_SPRITE*TAM_TOTAL/2/600;
    corpos[1].fr_x = 0;
    corpos[1].fr_x = 0;
    corpos[1].vel_x = 0;
    corpos[1].vel_x = 0;
    corpos[1].vida = 100;

    /* inicializa a massa dos projeiteis = -1 para
        indicar que eles não estão ativos */

    for (i = 2; i < *nCorpos + 2; i++){
        corpos[i].massa = -1;
        corpos[i].tempoVida = 0;
    }

    return corpos;

}



int main(int argc, char*argv[]){

	corpo *corpos, planeta;
	double duracaoProjs, tempoSim, freq, G;
    int nCorpos, i, j, colisao, input;
    colisao = 0;
    G = 6.67 *pow(10, -11);
    duracaoProjs = 0.25;

    WINDOW *W;
    PIC fundo;
    PIC saveiro[16], corsinha[16], projPIC, planetaPIC;
    MASK mask1[16], mask2[16], mask3, projMASK;


	if(argc > 1)
		freq = atof(argv[1]);

	else{
		printf("Digite a frequência de atualização : ");
		int x = scanf("%lf", &freq);
	}

    printf("Deseja ler a config de um arquivo ? (1 = sim, 0 = não) ");
    scanf("%d", &input);

    if(input == 1)
        corpos = leitura(&planeta, &tempoSim, &duracaoProjs, &nCorpos);

    else
        corpos = inicializaCorpos(&planeta, &nCorpos);

       
    /*

     posição, velocidade e força resultante inicial de todos os corpos
     que não são projéteis

    */

    // printf("pos inicial de 1: %lf %lf\n", corpos[0].pos_x, corpos[0].pos_y);
    // printf("pos inicial de 2 : %lf %lf\n", corpos[1].pos_x, corpos[1].pos_y);
    // printf("pos inicial do planeta : %lf %lf\n\n", planeta.pos_x, planeta.pos_y);

    // printf("vel inicial de 1 : %lf %lf\n", corpos[0].vel_x, corpos[0].vel_y);
    // printf("vel inicial de 2 : %lf %lf\n", corpos[1].vel_x, corpos[1].vel_y);
    // printf("vel inicial do planeta : %lf %lf\n\n", planeta.vel_x, planeta.vel_y);

    // printf("força resultante inicial de 1 : %lf %lf\n", corpos[0].fr_x, corpos[0].fr_y);
    // printf("força resultante inicial de 2 : %lf %lf\n", corpos[1].fr_x, corpos[1].fr_y);
    // printf("força resultante inicial do planeta : %lf %lf\n\n", planeta.fr_x, planeta.fr_y); 

    /* inicializar janela */

    W = InitGraph(WIDTH, HEIGHT, "Jogo do Peixe");

    InitKBD(W);

    carregarSprites(W, &planetaPIC, &projPIC, &fundo, saveiro, corsinha, mask1, mask2, &mask3, &projMASK);
    
    atualizarJanela(1, nCorpos, corpos, W, fundo, planetaPIC, projPIC, saveiro, corsinha, mask1, mask2, mask3, projMASK);
    
    while(!colisao){	

    /*
        esse loop calcula a resultante do planeta
        em função de todos os corpos que existem
        ao seu redor e verifica para ver se 
        nenhum deles está na mesma posição que
        o planeta

    */

        for (i = 0; i < nCorpos+2; i++)
            forcaResult(&planeta, corpos[i], G);  


    /*
    
        esse loop calcula a resultante de todos 
        os corpos em função do planeta.

        Depois no loop de dentro (1) calcula-se a força 
        resultante de todos os corpos em função
        dos outros corpos no espaço que não são
        o planeta

        A condição (2) existe para evitar que o cálculo
        da força resultante de um corpo seja feito com
        ele mesmo

    */

        for(i = 0; i < nCorpos+2; i++){
            if(corpos[i].massa != -1){

                corpos[i].fr_x = 0;
                corpos[i].fr_y = 0;
                forcaResult(&corpos[i], planeta, G);       

                for(j = 0; j < nCorpos+2; j++ /*(1)*/ ){

                    if(i != j /*(2)*/ )
                        forcaResult(&corpos[i], corpos[j], G);

                }
            }
        }
    /*  
        atualiza as variáveis do planeta
        depois dos cálculos  
    */
        atualiza(&planeta, freq);
    /* 
        atualiza     as variáveis de todos os corpos
         depois dos cálculos
    */


        for (i = 0; i < nCorpos+2; i++) {
            if (corpos[i].massa != -1) {
                atualiza (&corpos[i] , freq);
                // colisao += verifica(corpos[i], planeta, 1, 0);
                // colisao += verifica(corpos[i], corpos[0], 0, 0);
                // colisao += verifica(corpos[i], corpos[1], 0, 0);
            }
        }
    
        colisao += verifica(corpos, planeta, nCorpos, freq, tempoSim);

    /* 
        saídas : atualização da posição, velocidade e força dos corpos
            que não são projéteis
    */

    /*
        atualizar passo a passo a posição dos corpos na janela
    */

        atualizarJanela(0, nCorpos, corpos, W, fundo, planetaPIC, projPIC, saveiro, corsinha, mask1, mask2, mask3, projMASK);

        // printf("pos de 1: %lf %lf\n", corpos[0].pos_x, corpos[0].pos_y);
        // printf("pos de 2 : %lf %lf\n", corpos[1].pos_x, corpos[1].pos_y);
        // printf("pos do planeta : %lf %lf\n\n", planeta.pos_x, planeta.pos_y);

        // printf("vel de 1 : %lf %lf\n", corpos[0].vel_x, corpos[0].vel_y);
        // printf("vel de 2 : %lf %lf\n", corpos[1].vel_x, corpos[1].vel_y);

        // printf("força resultante de 1 : %lf %lf\n", corpos[0].fr_x, corpos[0].fr_y);
        // printf("força resultante de 2 : %lf %lf\n", corpos[1].fr_x, corpos[1].fr_y);

        
        usleep(10000);
    }

    printf("Game Over\n");
    WDestroy(W);
    free(corpos);

}