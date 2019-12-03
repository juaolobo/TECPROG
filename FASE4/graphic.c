#include <stdio.h>
#include "graphic.h"
#include "corpos.h"
#include <math.h>
#define PI           3.14159265358979323846
int ang1, ang2;
int loopsWarp1 = -1, loopsWarp2 = -1;
int loopAtual1 = -1, loopAtual2 = -1;
int warpou1 = 0, warpou2 = 0;


/*
    calculaOrientacao - graphic.c :: determina a direção para a qual as naves apontam.
*/
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

/*
    interacaoTeclado - graphic.c :: gerencia o teclado.
*/
void interacaoTeclado(WINDOW *W, corpo *corpos, int nCorpos, int *warp){

    int keycode, i, nave, rot;
    double hip;
    *warp = -1;

    if(WCheckKBD(W)){

        keycode = WGetKey(W);

        if (keycode == ACEL1 || keycode == LEFT1 || keycode == RIGHT1 
            || keycode == SHOOT1 || keycode == JUMP1) {
            nave = 0;
            rot = ang1;
        }

        else {
            nave = 1;
            rot = ang2;
        }

        if(keycode == ACEL1 || keycode == ACEL2){
            corpos[nave].fr_x += cos(PI*(90-rot*22.5)/180)*100000000000000;
            corpos[nave].fr_y -= sin(PI*(90-rot*22.5)/180)*100000000000000;
        }

        else if (keycode == JUMP1 || keycode == JUMP2) {
            if(keycode == JUMP1) *warp = 0;
            if(keycode == JUMP2) *warp = 1;

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


            hip = sqrt(pow(corpos[nave].vel_y, 2) + pow(corpos[nave].vel_x, 2)) + 3000000;

            corpos[i].vel_x = hip*cos(PI*(90-rot*22.5)/180); 
            corpos[i].vel_y = -hip*sin(PI*(90-rot*22.5)/180);
            

        }

    }

}   

/*
    carregarMenu - graphic.c :: carrega o menu de interação do usuário.
*/
void carregarMenu(WINDOW *W, PIC *menuI, PIC *menuS, PIC *textoVidas, PIC *numeros, PIC *corsa, PIC *saveiro, PIC *empate){

    *menuI = ReadPic(W, "res/MENU_INICIAR.xpm", NULL);
    *menuS = ReadPic(W, "res/MENU_SAIR.xpm", NULL);
    *textoVidas = ReadPic(W, "res/vidas.xpm", NULL);
    *numeros = ReadPic(W, "res/numeros.xpm", NULL);
    *corsa =  ReadPic(W, "res/vitoria_corsinha.xpm", NULL);
    *saveiro = ReadPic(W, "res/vitoria_saveiro.xpm", NULL);
    *empate = ReadPic(W, "res/empate.xpm", NULL);
}

/*
    renderMenu - graphic.c :: renderiza o menu de interação do usuário.
*/
void renderMenu(WINDOW *W, PIC menuAtual){

    WClear(W);
    PutPic(W, menuAtual, 0, 0, WIDTH, HEIGHT, 0, 0);
}

/*
    carregarSprites - graphic.c :: carrega sprites para naves do jogo.
*/
void carregarSprites(WINDOW *W, PIC *planetaPIC, PIC *projPIC, PIC *fundo, PIC *numeros, PIC saveiro[], PIC corsinha[], MASK mask1[], MASK mask2[], MASK *mask3, MASK *projMASK){

    int i;
    PIC aux; 
    PIC sprite_saveiro, sprite_corsinha;
    char nome_arquivo[28] = "res/naves/saveiromask00.xpm";
    char nome_arquivo2[29] = "res/naves/corsinhamask00.xpm";

    *fundo = ReadPic(W, "res/campodefut.xpm", NULL);
    *numeros = ReadPic(W, "res/numeros_pequenos.xpm", NULL); 


    /* inicialização da masks */

    for(i = 0; i < 16; i = i + 1)
    {
        mask1[i] = NewMask(W, WD_SPRITE, H_SPRITE);
        nome_arquivo[21] = ((i) / 10) + 48;
        nome_arquivo[22] = ((i) % 10) + 48;
        aux = ReadPic(W, nome_arquivo, mask1[i]);
        mask2[i] = NewMask(W, WD_SPRITE, H_SPRITE);
        nome_arquivo2[22] = ((i) / 10) + 48;
        nome_arquivo2[23] = ((i) % 10) + 48;
        aux = ReadPic(W, nome_arquivo2, mask2[i]);
    }

    *mask3 = NewMask(W, 200, 200);
    aux = ReadPic(W, "res/ronaldinho_mask.xpm", *mask3);
    *projMASK = NewMask(W, 18, 18);
    aux = ReadPic(W, "res/balamask.xpm", *projMASK);

    /* criação dos sprites e renderização nas PICs*/

    sprite_saveiro = ReadPic(W, "res/naves/saveiro_full.xpm", NULL);
    sprite_corsinha = ReadPic(W, "res/naves/corsinha_full.xpm", NULL);

    for(i = 0; i < 16; i = i + 1)
    {
        saveiro[i] = NewPic(W, WD_SPRITE, H_SPRITE);
        PutPic(saveiro[i], sprite_saveiro, WD_SPRITE * i, 0, WD_SPRITE, H_SPRITE, 0, 0);
        corsinha[i] = NewPic(W, WD_SPRITE, H_SPRITE);
        PutPic(corsinha[i], sprite_corsinha, WD_SPRITE * i, 0, WD_SPRITE, H_SPRITE, 0, 0);
    }

    *planetaPIC = ReadPic(W, "res/ronaldinho.xpm", NULL);
    *projPIC = ReadPic(W, "res/bala.xpm", NULL);
}

void carregarEfeitos(WINDOW *W, PIC explosion[], MASK maskExp[], PIC warp[], MASK maskWarp[]){

    PIC aux;
    char nome[28] = "res/xplosao/explosion00.xpm";
    char nome1[20] = "res/warp/warp00.xpm";
    PIC sprite = ReadPic(W, "res/xplosao/explosionBig.xpm", NULL);
    int i;

    for (i = 0; i < 9; i++) {

        maskExp[i] = NewMask(W, 80, 79);
        nome[21] = ((i) / 10) + 48;
        nome[22] = ((i) % 10) + 48;
        aux = ReadPic(W, nome, maskExp[i]);
    }

    for (i = 0; i < 9; i++) {
        explosion[i] = NewPic(W, 80, 79);
        PutPic(explosion[i], sprite, 80*i, 0, 80, 79, 0, 0);
    }

    sprite = ReadPic(W, "res/warp/warp.xpm", NULL);

    for (i = 0; i < 8; i++) {

        maskWarp[i] = NewMask(W, 120, 120);
        nome1[13] = ((i) / 10) + 48;
        nome1[14] = ((i) % 10) + 48;
        aux = ReadPic(W, nome1, maskWarp[i]);
    }

    for (i = 0; i < 8; i++) {
        warp[i] = NewPic(W, 120, 120);
        PutPic(warp[i], sprite, 120*i, 0, 120, 120, 0, 0);
    }
}

/*
    posicaoGrafica - graphic.c :: receberá uma posicao do objeto a ser calculado e retornará o equivalente na tela renderizada em um dos dois eixos.
*/

int posicaoGrafica(double x, int eixo){

    int posicao;
    x += TAM_TOTAL/2;
    posicao = (eixo*x)/TAM_TOTAL;
    return(posicao);

}

/*
    atualizarJanela - graphic.c :: atualiza a situação do jogo na janela.
*/
void atualizarJanela(int init, int nCorpos, int vidasSav, int vidasCorsa, corpo *corpos, WINDOW *W, PIC fundo, PIC planetaPIC, PIC projPIC, PIC numeros, PIC saveiro[], PIC corsinha[], PIC warpPIC[], MASK mask1[], MASK mask2[], MASK mask3, MASK projMASK, MASK maskWarp[])
{

    int x, y, i, isWarp = -1;

    /*pinta as naves calculando primeiro a orientacao
    *
    *As naves, projeteis e o planeta sao renderizados de maneira centralizada subtraindo das coordenadas x e y
    *metade de suas larguras e alturas.
    *
    */

    WClear(W);
    PutPic(W, fundo, 0, 0, WIDTH, HEIGHT, 0, 0);
    PutPic(W, numeros, vidasCorsa*40, 0, 40, 40, WIDTH/2 - 50, 5);
    PutPic(W, numeros, 2, 6, 11, 5, WIDTH/2 - 5, 22);
    PutPic(W, numeros, vidasSav*40, 0, 40, 40, WIDTH/2 + 10, 5);

    SetMask(W, mask3);
    PutPic(W, planetaPIC, 0, 0, 200, 200, (WIDTH/2)-100, (HEIGHT/2)-100);
    UnSetMask(W);


    if (init == 0) {
        interacaoTeclado(W, corpos, nCorpos, &isWarp);
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

    if(isWarp == 0 && loopsWarp1 <= 0) loopsWarp1 = 128; /*Esse numero pois a cada 8 loops do jogo uma animacao de warp sera executada e o loop precisa ser executado 2 vezes 8*8*2 = 128*/
    else if (isWarp == 1 && loopsWarp2 <= 0) loopsWarp2 = 128;

    if(loopsWarp1 >= 0 || loopsWarp2 >= 0){
        warp(W, corpos, warpPIC, maskWarp);
        loopsWarp1--;
        loopsWarp2--;
    }


}

/*
    explosao - graphic.c :: gerencia a explosão da nave.
*/
void explosao(WINDOW *W, corpo *corpos, int nave, PIC explosion[], MASK mask[]){

    int i, xCorsa, yCorsa, xSav, ySav;

    xCorsa = posicaoGrafica(corpos[0].pos_x, WIDTH);
    yCorsa = posicaoGrafica(corpos[0].pos_y, HEIGHT);

    xSav = posicaoGrafica(corpos[1].pos_x, WIDTH);
    ySav = posicaoGrafica(corpos[1].pos_y, HEIGHT);

    if(nave == 0){
        for (i = 0; i < 9; i++) {
            SetMask(W, mask[i % 9]);
            PutPic(W, explosion[i], 0, 0, 80, 79, xCorsa-40, yCorsa-39);
            UnSetMask(W);
            usleep(80000);
        }
    }else if(nave == 1){
        for (i = 0; i < 9; i++) {
            SetMask(W, mask[i % 9]);
            PutPic(W, explosion[i], 0, 0, 80, 79, xSav-40, ySav-39);
            UnSetMask(W);
            usleep(80000);
        }
    }else{
        for (i = 0; i < 9; i++) {
            SetMask(W, mask[i % 9]);
            PutPic(W, explosion[i], 0, 0, 80, 79, xCorsa-40, yCorsa-39);
            PutPic(W, explosion[i], 0, 0, 80, 79, xSav-40, ySav-39);
            UnSetMask(W);
            usleep(80000);
        }
    }

}

/*
    warp - graphic.c :: gerencia a a execucao do salto das naves (ambas renderizacao e mecanica do salto).
*/

void warp(WINDOW *W, corpo *corpos, PIC warp[], MASK mask[]) {

    int xCorsa, yCorsa, xSav, ySav;
    
   
    xCorsa = posicaoGrafica(corpos[0].pos_x, WIDTH);
    yCorsa = posicaoGrafica(corpos[0].pos_y, HEIGHT);

    xSav = posicaoGrafica(corpos[1].pos_x, WIDTH);
    ySav = posicaoGrafica(corpos[1].pos_y, HEIGHT);

    if(loopsWarp1 > 0 && loopsWarp1 % 8 == 0){
        if(loopsWarp1/8 > 8) loopAtual1 = 16 - loopsWarp1/8;
        else loopAtual1 = loopsWarp1/8 - 1;
    }
    if(loopsWarp2 > 0 && loopsWarp2 % 8 == 0){
        if(loopsWarp2/8 > 8) loopAtual2 = 16 - loopsWarp2/8;
        else loopAtual2 = loopsWarp2/8 - 1;
    }

    if(loopsWarp1 <= 0) loopAtual1 = -1;
    if(loopsWarp2 <= 0) loopAtual2 = -1;

    if(loopAtual1 != -1){
        SetMask(W, mask[loopAtual1 % 8]);
        PutPic(W, warp[loopAtual1], 0, 0, 120, 120, xCorsa-60, yCorsa-60);
        UnSetMask(W);
    }else warpou1 = 0;
    if(loopAtual2 != -1){
        SetMask(W, mask[loopAtual2 % 8]);
        PutPic(W, warp[loopAtual2], 0, 0, 120, 120, xSav-60, ySav-60);
        UnSetMask(W);
    }else warpou2 = 0;

    if(warpou1 == 0 && loopsWarp1 < 64 && loopsWarp1 > 0){
        corpos[0].pos_x += cos(PI*(90-ang1*22.5)/180)*8000000;
        corpos[0].pos_y -= sin(PI*(90-ang1*22.5)/180)*8000000;
        warpou1 = 1;
    }
    if(warpou2 == 0 && loopsWarp2 < 64 && loopsWarp2 > 0){
        corpos[1].pos_x += cos(PI*(90-ang2*22.5)/180)*8000000;
        corpos[1].pos_y -= sin(PI*(90-ang2*22.5)/180)*8000000;
        warpou2 = 1;
    }
}

/* 
    menuVidas - graphic.c :: gerencia o menu de vidas na janela.
*/
int menuVidas (PIC fundo, WINDOW *W, PIC textoVidas, PIC numeros){

    int itemAtual = 1, keyCode;
    WClear(W);
    PutPic(W, fundo, 0, 0, WIDTH, HEIGHT, 0, 0);
    PutPic(W, textoVidas, 0, 0, 500, 55, 50, 10);
    PutPic(W, numeros, itemAtual*100, 0, 100, 100, 250, 250);

    
    while(1){
        keyCode = 0;
        if(WCheckKBD(W)){
            keyCode = WGetKey(W);
        }
        if(keyCode == 114){   /*Direita = 114, esquerda = 113*/
            if(itemAtual < 9) itemAtual++;
            else itemAtual = 1;
            PutPic(W, numeros, itemAtual*100, 0, 100, 100, 250, 250);
        }
        else if(keyCode == 113){   /*Direita = 114, esquerda = 113*/
            if(itemAtual > 1) itemAtual--;
            else itemAtual = 9;
            PutPic(W, numeros, itemAtual*100, 0, 100, 100, 250, 250);
        }

        else if(keyCode == 36) return (itemAtual);
    }
}

/* 
    fimDeJogo - graphic.c :: finaliza o jogo.
*/
void fimDeJogo(PIC fundo, WINDOW *W, PIC vencedor){

    WClear(W);
    PutPic(W, fundo, 0, 0, WIDTH, HEIGHT, 0, 0);
    PutPic(W, vencedor, 0, 0, WIDTH, HEIGHT, 0, 0);
}

void resetLoops(){
    loopAtual1 = -1, loopAtual2 = -1;
    loopsWarp1 = -1, loopsWarp2 = -1;
}