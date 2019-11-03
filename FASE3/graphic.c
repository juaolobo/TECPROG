#include <stdio.h>
#include "graphic.h"
#include "corpos.h"
#include <math.h>
#define PI           3.14159265358979323846
int ang1, ang2;

int calculaOrientacao(corpo corpo){

    //corpos[0] e [1]
    double arc, rad;
    int n;
    if(corpo.pos_x != 0){

        /*rad equivale a 22,5 graus em radianos*/

        rad = 2*acos(-1)*(22.5/360);
        arc = atan(fabs(corpo.pos_y/corpo.pos_x));
        n = arc/rad;

        if(arc < 0) arc += acos(-1);

        if((corpo.pos_y) < 0) n += 8;
        if(n == 0){
            if(corpo.pos_x < 0)
                n = 8;
        }

        /*n = 4 eh a primeira sprite*/

        if(n-4 >= 0) n -= 4;
        else n = 16 + (n-4);
        return(n);
    }

    return 0;

}

void interacaoTeclado(WINDOW *W, corpo *corpos, int nCorpos){

    int keycode, i, nave, rot;
    double hip;

    if(WCheckKBD(W)){

        keycode = WGetKey(W);

        if(keycode == ACEL1 || keycode == LEFT1 || keycode == RIGHT1 || keycode == SHOOT1){
            nave = 0;
            rot = ang1;
        }
        
        else{
            nave = 1;
            rot = ang2;
        }

        if(keycode == ACEL1 || keycode == ACEL2){
            corpos[nave].fr_x += cos(PI*(90-rot*22.5)/180)*100000000000000;
            corpos[nave].fr_y -= sin(PI*(90-rot*22.5)/180)*100000000000000;
        }

        else if(keycode == RIGHT1 || keycode == RIGHT2){
            if(rot != 15)
                rot++;

            else 
                rot = 0;

            if(nave == 0)
                ang1 = rot;
            else
                ang2 = rot;

        }

        else if(keycode == LEFT1 || keycode == LEFT2){

            if(rot != 0)
                rot--;

            else 
                rot = 15;

            if(nave == 0)
                ang1 = rot;
            else
                ang2 = rot;

        }

        else if(keycode == SHOOT1 || keycode == SHOOT2){

            i = 2;

            while (i < nCorpos+2 && corpos[i].massa != -1)
                i++;

            corpos[i].massa = 100;
            corpos[i].pos_x = corpos[nave].pos_x + corpos[nave].raio*cos(PI*(90-(rot*22.5))/180);
            corpos[i].pos_y = corpos[nave].pos_y - corpos[nave].raio*sin(PI*(90-(rot*22.5))/180); 


            corpos[i].fr_x = 0;
            corpos[i].fr_y = 0;


            hip = sqrt(pow(corpos[nave].vel_y, 2) + pow(corpos[nave].vel_x, 2)) + 300000000;

            corpos[i].vel_x = hip*cos(PI*(90-rot*22.5)/180); 
            corpos[i].vel_y = -hip*sin(PI*(90-rot*22.5)/180);

        }

    }

}   

void carregarSprites(WINDOW *W, PIC *planetaPIC, PIC *projPIC, PIC *fundo, PIC saveiro[], PIC corsinha[], MASK mask1[], MASK mask2[], MASK *mask3, MASK *projMASK){

    int i;
    PIC aux; 
    PIC sprite_saveiro, sprite_corsinha;
    char nome_arquivo[23] = "naves/saveiromask00.xpm";
    char nome_arquivo2[24] = "naves/corsinhamask00.xpm";

    *fundo = ReadPic(W, "campodefut.xpm", NULL);


    /* inicialização da masks */

    for(i = 0; i < 16; i = i + 1)
    {
        mask1[i] = NewMask(W, WD_SPRITE, H_SPRITE);
        nome_arquivo[17] = ((i) / 10) + 48;
        nome_arquivo[18] = ((i) % 10) + 48;
        aux = ReadPic(W, nome_arquivo, mask1[i]);
        mask2[i] = NewMask(W, WD_SPRITE, H_SPRITE);
        nome_arquivo2[18] = ((i) / 10) + 48;
        nome_arquivo2[19] = ((i) % 10) + 48;
        aux = ReadPic(W, nome_arquivo2, mask2[i]);
    }

    *mask3 = NewMask(W, 200, 200);
    aux = ReadPic(W, "ronaldinho_mask.xpm", *mask3);
    *projMASK = NewMask(W, 18, 18);
    aux = ReadPic(W, "balamask.xpm", *projMASK);

    /* criação dos sprites e renderização nas PICs*/

    sprite_saveiro = ReadPic(W, "naves/saveiro_full.xpm", NULL);
    sprite_corsinha = ReadPic(W, "naves/corsinha_full.xpm", NULL);

    for(i = 0; i < 16; i = i + 1)
    {
        saveiro[i] = NewPic(W, WD_SPRITE, H_SPRITE);
        PutPic(saveiro[i], sprite_saveiro, WD_SPRITE * i, 0, WD_SPRITE, H_SPRITE, 0, 0);
        corsinha[i] = NewPic(W, WD_SPRITE, H_SPRITE);
        PutPic(corsinha[i], sprite_corsinha, WD_SPRITE * i, 0, WD_SPRITE, H_SPRITE, 0, 0);
    }

    *planetaPIC = ReadPic(W, "ronaldinho.xpm", NULL);
    *projPIC = ReadPic(W, "bala.xpm", NULL);
}


/*Essa funcao recebera uma posicao do objeto a ser calculado e retornara o equivalente na tela renderizada em um dos dois eixos*/

int posicaoGrafica(double x, int eixo){

    int posicao;
    x += TAM_TOTAL/2;
    posicao = (eixo*x)/TAM_TOTAL;
    return(posicao);

}


void atualizarJanela(int init, int nCorpos, corpo *corpos, WINDOW *W, PIC fundo, PIC planetaPIC, PIC projPIC, PIC saveiro[], PIC corsinha[], MASK mask1[], MASK mask2[], MASK mask3, MASK projMASK)
{

    int x, y, i;

    /*pinta as naves calculando primeiro a orientacao
    *
    *As naves, projeteis e o planeta sao renderizados de maneira centralizada subtraindo das coordenadas x e y
    *metade de suas larguras e alturas.
    *
    */

    WClear(W);
    PutPic(W, fundo, 0, 0, WIDTH, HEIGHT, 0, 0);

    SetMask(W, mask3);
    PutPic(W, planetaPIC, 0, 0, 200, 200, (WIDTH/2)-100, (HEIGHT/2)-100);
    UnSetMask(W);


    if (init == 0) {
        interacaoTeclado(W, corpos, nCorpos);
    }
    else {
        ang1 = calculaOrientacao(corpos[0]);
        ang2 = calculaOrientacao(corpos[1]);
    }

    x = posicaoGrafica(corpos[0].pos_x, WIDTH);
    y = posicaoGrafica(corpos[0].pos_y, HEIGHT);

    SetMask(W, mask2[ang1]);
    PutPic(W, corsinha[ang1], 0, 0, WD_SPRITE, H_SPRITE, x-(WD_SPRITE/2), y-(H_SPRITE)/2);
    UnSetMask(W);

    x = posicaoGrafica(corpos[1].pos_x, WIDTH);
    y = posicaoGrafica(corpos[1].pos_y, HEIGHT);

    SetMask(W, mask1[ang2]);
    PutPic(W, saveiro[ang2], 0, 0, WD_SPRITE, H_SPRITE, x-(WD_SPRITE/2), y-(H_SPRITE)/2);
    UnSetMask(W);

    SetMask(W, projMASK);
    for (i = 2; i < nCorpos+2; i++){
        if(corpos[i].massa != -1){
            x = posicaoGrafica(corpos[i].pos_x, WIDTH);
            y = posicaoGrafica(corpos[i].pos_y, HEIGHT);
            PutPic(W, projPIC, 0, 0, 18, 18, x-18, y-18);
        }
    }
    UnSetMask(W);

}