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

    int gameRunning = 1;
    int escolha = -1, itemAtual = 0;  /*escolha: Variavel para manter o jogo no main menu ate que uma opcao seja escolhida.*/
                                          /*itemAtual: Variavel para escolher qual imagem do fundo usar*/
    int keyCode;
    
    colisao = 0;
    G = 6.67 *pow(10, -11);
    duracaoProjs = 0.25;

    WINDOW *W;
    PIC fundo;
    PIC menuI, menuS, textoVidas, numeros, numeros_pequenos;
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
    /* inicializar janela */

    W = InitGraph(WIDTH, HEIGHT, "Jogo do Peixe");

    InitKBD(W);

    carregarSprites(W, &planetaPIC, &projPIC, &fundo, &numeros_pequenos, saveiro, corsinha, mask1, mask2, &mask3, &projMASK);
    carregarMenu(W, &menuI, &menuS, &textoVidas, &numeros);

    while(gameRunning){

        /*Aqui terei que inicializar a tela do main menu*/

        renderMenu(W, menuI);
        escolha = -1;
        itemAtual = 0;

        while(escolha == -1){
            keyCode = 0;
            if(WCheckKBD(W)){
                keyCode = WGetKey(W);
            }
            if(keyCode == 111 || keyCode == 116){
                if(itemAtual == 0){
                    itemAtual = 1;
                    renderMenu(W, menuS);
                }
                else{ 
                    itemAtual = 0;
                    renderMenu(W, menuI);
                }
            }
            else if(keyCode == 36){
                if(itemAtual == 0) escolha = 1;
                else {
                    escolha = 0;
                    gameRunning = 0;
                }
            }
        }


        if(escolha == 1){

            int vidasSav = menuVidas(fundo, W, textoVidas, numeros);
            int vidasCorsa = vidasSav;

            while(vidasSav > 0 && vidasCorsa > 0){
                atualizarJanela(1, nCorpos, vidasSav, vidasCorsa, corpos, W, fundo, planetaPIC, projPIC, numeros_pequenos, saveiro, corsinha, mask1, mask2, mask3, projMASK);
                if(input == 1)  corpos = leitura(&planeta, &tempoSim, &duracaoProjs, &nCorpos);
                else            corpos = inicializaCorpos(&planeta, &nCorpos);

                while(1){	

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
                        }
                    }


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
             
                    
                    if ((colisao = verifica(corpos, planeta, nCorpos, freq, tempoSim)) != -1) break;
                /* 
                    saídas : atualização da posição, velocidade e força dos corpos
                        que não são projéteis
                */

                /*
                    atualizar passo a passo a posição dos corpos na janela
                */

                    atualizarJanela(0, nCorpos, vidasSav, vidasCorsa, corpos, W, fundo, planetaPIC, projPIC, numeros_pequenos, saveiro, corsinha, mask1, mask2, mask3, projMASK);

                    usleep(10000);
                }
            if (colisao == 0) vidasCorsa--;
            else if(colisao == 1) vidasSav--;
            else vidasCorsa--, vidasSav--;
            explosao(W, corpos, colisao, nCorpos, fundo, planetaPIC, projPIC, saveiro, corsinha, mask1, mask2, mask3, projMASK);

            free(corpos);

            /*Renderizacao da tela de morte*/




            }
        }

    }
        printf("Game Over\n");
        WDestroy(W);
}