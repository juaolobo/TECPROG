#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "fisica.h"
#include "graphic.h"
#include "corpos.h"
    

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