#include <stdio.h>	
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include "xwc.h"
#include <string.h>

/*
    GRUPO:

    Patrick Silva Souza         NUSP : 11221821
    Murilo Andre Gomes Felipe   NUSP : 11221863
    João Felipe Lobo Pevidor NUSP : 11221992
    Mohamad Hussein Rkein NUSP : 10740130

*/                
#define TAM_TOTAL 20000000 // arbitrário, só para teste
#define WIDTH 600
#define HEIGHT 600
#define WD_SPRITE 80
#define H_SPRITE 90
#define PI 3.14159265
#define SHOOT1 50
#define LEFT1 38
#define RIGHT1 40
#define ACEL1 25
#define SHOOT2 65
#define LEFT2 44
#define RIGHT2 46
#define ACEL2 32

int ang1, ang2;

/*

    corpos tem raio pois com a implementação do planeta como um corpo 
    fica mais fácil de iterar sobre todos os corpos com as mesmas
    funções

*/

typedef struct corpo {

	char nome[20];
	double massa;
    double raio;
	double pos_x; 	
	double pos_y;
	double vel_x;
	double vel_y;
    double fr_x;
    double fr_y;  
    double tempoVida; 
    int vida;

} corpo;


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

void forcaResult(corpo *corpo1, corpo corpo2, double G){


    double  distx, disty, dist2x, dist2y, 
            resx, resy, 
            coA, coB, 
            Cx, Cy, Dx, Dy;
    int     inverte = 0;

    /* 
        considerando a superfície como toroidal:
        se os corpos estão mais próximos entre si através das bordas, então eles se atraem pelas bordas; caso contrário, atraem-se pela superfície.

        As duas naves estão localizados em pontos num sistema de coordenadas cartesianas xy. São dois pontos: consigo determinar a reta r: y = ax + b usando as suas posições. 
        
        coA = (y2 - y1) / (x2 - x1);
        coB = y1 - coA * x1 (ou = y2 - coA * x2).

        Preciso encontrar os pontos de interseção da reta com as bordas. Sejam C e D esses pontos:
        
        Eu preciso calcular C = (Cx, Cy) e D = (Dx, Dy): 
        Cx = TAM_TOTAL / 2.
        Cy = coA * Cx + coB
        Se Cy < -TAM_TOTAL / 2 ou Cy > TAM_TOTAL / 2, então
            Cy = TAM_TOTAL / 2
            Cx = (Cy - coB) / coA
        Dx = -TAM_TOTAL / 2.
        Dy = coA * Cx + coB
        Se Dy < -TAM_TOTAL / 2 ou Dy > TAM_TOTAL / 2, então
            Dy = -TAM_TOTAL / 2
            Dx = (Dy - coB) / coA 
    */
    if(corpo2.massa != -1 && corpo1->massa != -1)
    {
        /* determinando coeficientes da reta */
        
        /* se a reta não é vertical */
        if((corpo1->pos_x - corpo2.pos_x) != 0)
        {
            coA = (corpo2.pos_y - corpo1->pos_y) / (corpo2.pos_x - corpo1->pos_x);
            coB = corpo1->pos_y - coA * corpo1->pos_x;
            
            /* determinando C */
            Cx = TAM_TOTAL / 2;
            Cy = coA * Cx + coB;
            if(Cy < -TAM_TOTAL / 2 || Cy > TAM_TOTAL / 2)
            {
                Cy = TAM_TOTAL / 2;
                Cx = (Cy - coB) / coA;
            }

            /* determinando D */
            Dx = -TAM_TOTAL / 2;
            Dy = coA * Cx + coB;
            if(Dy < -TAM_TOTAL / 2 || Dy > TAM_TOTAL / 2)
            {
                Dy = -TAM_TOTAL / 2;
                Dx = (Dy - coB) / coA;
            } 
        }
        else /* se é vertical */
        {
            Dx = Cx = 0;
            Cy = -TAM_TOTAL / 2;
            Dy = -Cy;
        }
        
        distx = fabs(corpo1->pos_x - corpo2.pos_x);
        disty = fabs(corpo1->pos_y - corpo2.pos_y);

        /* 
            tenho que verificar que distância vou determinar: 
            se o corpo1 estiver mais próximo do ponto C, há que as distância a seguir é a soma das distâncias entre os pares (corpo1, C) e (corpo2, D). Caso o contrário, (corpo1, D) e (corpo2, D).     
        */

        dist2x = (sqrt(pow(corpo1->pos_x - Cx, 2) + pow(corpo1->pos_y - Cy, 2)) < sqrt(pow(corpo1->pos_x - Dx, 2) + pow(corpo1->pos_y - Dy, 2)) ? fabs(corpo1->pos_x - Cx) + fabs(corpo2.pos_x - Dx) : fabs(corpo1->pos_x - Dx) + fabs(corpo2.pos_x - Cx));
        dist2y = (sqrt(pow(corpo1->pos_x - Cx, 2) + pow(corpo1->pos_y - Cy, 2)) < sqrt(pow(corpo1->pos_x - Dx, 2) + pow(corpo1->pos_y - Dy, 2)) ? fabs(corpo1->pos_y - Cy) + fabs(corpo2.pos_y - Dy) : fabs(corpo1->pos_y - Dy) + fabs(corpo2.pos_y - Cy));

        /* a menor das distâncias é a distância que define a força resultante */
        if(sqrt(pow(distx, 2) + pow(disty, 2)) > sqrt(pow(dist2x, 2) + pow(dist2y, 2)))
        {
            distx = dist2x;
            disty = dist2y;
            inverte = 1;
        }

        if (distx != 0)
            resx = (corpo1->massa * corpo2.massa * G)/(pow(distx,2));
        else
            resx = 0;

        if (disty != 0)
            resy = (corpo1->massa * corpo2.massa * G)/(pow(disty,2));
        else
            resy = 0;
        
        // a posição do corpo influencia o sinal do vetor resultante
        if(inverte)
        {
            resx = -resx;
            resy = -resy;
        }

        if (corpo1->pos_x > corpo2.pos_x)
            corpo1->fr_x -= resx;

        else if(corpo1->pos_x < corpo2.pos_x)
            corpo1->fr_x += resx;

        if (corpo1->pos_y > corpo2.pos_y)
            corpo1->fr_y -= resy;

        else if (corpo1->pos_y < corpo2.pos_y)
            corpo1->fr_y += resy;
    }
}

void atualiza(corpo *corpo, double tempo){

	double acelx, acely;
    tempo = tempo/1000;

    // série de casos caso o corpo tenha saído da tela

	if (corpo->pos_x < -(TAM_TOTAL))
		corpo->pos_x += 2*TAM_TOTAL;

	if(corpo->pos_x > TAM_TOTAL)
		corpo->pos_x -= 2*TAM_TOTAL;

	if(corpo->pos_y < -(TAM_TOTAL))
		corpo->pos_y += 2*TAM_TOTAL;

	if(corpo->pos_y > TAM_TOTAL)
		corpo->pos_y -= 2*TAM_TOTAL;
  
    // a = F/m

    acelx = (corpo->fr_x)/(corpo->massa);
    acely = (corpo->fr_y)/(corpo->massa);

    // v = vo + at

	corpo->vel_x = corpo->vel_x + acelx*tempo;
	corpo->vel_y = corpo->vel_y + acely*tempo;

    // S = So +vt + (at^2)/2

	corpo->pos_x = corpo->pos_x + (corpo->vel_x)*tempo + acelx*tempo*tempo/2;
	corpo->pos_y = corpo->pos_y + (corpo->vel_y)*tempo + acely*tempo*tempo/2;


}

int verifica (corpo *corpos, corpo planeta, int nCorpos, double freq, double tempoSim) {

    /*

    essa função serve para tratar os casos em que talvez há "colisão"
    ou seja em que os corpos estão na mesma posição

    */

    int i;

    /* ve se as naves tem vida ainda para continuar o jogo */

    for (i = 0; i < 2; i++){

        if (corpos[i].vida == 0){

            printf("A nave %s morreu\n\n", corpos[i].nome);
            return 1;

        }  
    }

    /* calcula se as naves colidiram com elas mesmas  */

    if (sqrt(pow(corpos[0].pos_x, 2) + pow(corpos[0].pos_y, 2)) <= corpos[1].raio &&
        sqrt(pow(corpos[1].pos_x, 2) + pow(corpos[1].pos_y, 2)) <= corpos[0].raio) {
        printf("As duas naves colidiram\n\n");
        return (1);
    }

    /* calcula se as naves não colidiram com o planeta */

    for (i = 0; i < 2; i++)
        if (sqrt(pow(corpos[i].pos_x, 2) + pow(corpos[i].pos_y, 2)) <= planeta.raio) {
            printf("A nave %s colidiu com o planeta\n\n", corpos[i].nome);
            return (1);
        }

    /* calcula as colisões dos projetei */

    for (i = 2; i < nCorpos+2; i++) {

        if (corpos[i].massa != -1){

            corpos[i].tempoVida += freq;

            if (sqrt(pow(corpos[i].pos_x, 2) + pow(corpos[i].pos_y, 2)) <= corpos[0].raio) {

                corpos[0].vida -= 10;
                corpos[i].massa = -1;
                corpos[i].tempoVida = 0;

            } 

            if (sqrt(pow(corpos[i].pos_x, 2) + pow(corpos[i].pos_y, 2)) <= corpos[1].raio) {

                corpos[1].vida -= 10;
                corpos[i].massa = -1;
                corpos[i].tempoVida = 0;

            } 

            if (corpos[i].tempoVida == tempoSim || 
                sqrt(pow(corpos[i].pos_x, 2) + pow(corpos[i].pos_y, 2)) <= planeta.raio){
                corpos[i].massa = -1;
                corpos[i].tempoVida = 0;
            }

        }
    }




    return 0;


}


/*
    FUNCOES DA PARTE GRAFICA ABAIXO
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

int interacaoTeclado(WINDOW *W, corpo *nave, int naveNum ,corpo *corpos, int nCorpos, int rot){

    int keycode, i;
    double hip;

    if(WCheckKBD(W)){
        keycode = WGetKey(W); 

        if((keycode == ACEL1 && naveNum == 0) || (keycode == ACEL2 && naveNum == 1)){

            switch(rot){
                case 0:
                    nave->fr_y -= 1000000;
                    break;

                case 1:
                    nave->fr_x += cos(PI*67.5/180)*1000000;
                    nave->fr_y -= sin(PI*67.5/180)*1000000;
                    break;

                case 2:
                    nave->fr_x += cos(PI*45/180)*1000000;
                    nave->fr_y -= sin(PI*45/180)*1000000;
                    break;

                case 3:
                    nave->fr_x += cos(PI*22.5/180)*1000000;
                    nave->fr_y -= sin(PI*22.5/180)*1000000;
                    break;

                case 4:
                    nave->fr_x += 1000000;
                    break;

                case 5:
                    nave->fr_x += cos(PI*-22.5/180)*1000000;
                    nave->fr_y += sin(PI*-22.5/180)*1000000;
                    break;

                case 6:
                    nave->fr_x += cos(PI*-45/180)*1000000;
                    nave->fr_y += sin(PI*-45/180)*1000000;
                    break;

                case 7:
                    nave->fr_x += cos(PI*-67.5/180)*1000000;
                    nave->fr_y += sin(PI*-67.5/180)*1000000;
                    break;

                case 8:
                    nave->fr_y += 1000000;
                    break;

                case 9:
                    nave->fr_x -= cos(PI*-112.5/180)*1000000;
                    nave->fr_y += sin(PI*-112.5/180)*1000000;
                    break;

                case 10:
                    nave->fr_x -= cos(PI*-135/180)*1000000;
                    nave->fr_y += sin(PI*-135/180)*1000000;
                    break;

                case 11:
                    nave->fr_x -= cos(PI*-157.5/180)*1000000;
                    nave->fr_y += sin(PI*-157.5/180)*1000000;
                    break;

                case 12:
                    nave->fr_x -= 1000000;
                    break;

                case 13:
                    nave->fr_x -= cos(PI*-202.5/180)*1000000;
                    nave->fr_y -= sin(PI*-202.5/180)*1000000;
                    break;

                case 14:
                    nave->fr_x -= cos(PI*-225/180)*1000000;
                    nave->fr_y -= sin(PI*-225/180)*1000000;
                    break;

                case 15:
                    nave->fr_x -= cos(PI*-247.5/180)*1000000;
                    nave->fr_y -= sin(PI*-247.5/180)*1000000;
                    break;

            }

        }

        else if(keycode == RIGHT1){

            if(rot != 15)
                rot++;

            else 
                rot = 0;

            return rot;

        }

        else if(keycode == LEFT1){

            if(rot != 0)
                rot--;

            else 
                rot = 15;

            return rot;

        }

        else if((keycode == SHOOT1 && naveNum == 0) || (keycode == SHOOT2 && naveNum == 1)){

            i = 2;

            while (i < nCorpos+2 && corpos[i].massa != -1)
                i++;

            corpos[i].massa = 100;

            corpos[i].pos_x = nave->pos_x + WD_SPRITE;
            corpos[i].pos_y = nave->pos_y + H_SPRITE;            
            corpos[i].fr_x = 0;
            corpos[i].fr_y = 0;


            hip = sqrt(pow(nave->vel_y, 2) + pow(nave->vel_x, 2)) + 300000000;
            switch(rot){

                case 0:
                    corpos[i].vel_x = 0; 
                    corpos[i].vel_y = -hip; 
                    break;

                case 1:
                    corpos[i].vel_x = hip*cos(PI*67.5/180); 
                    corpos[i].vel_y = -hip*sin(PI*67.5/180);
                    break;

                case 2:
                    corpos[i].vel_x = hip*cos(PI*45/180); 
                    corpos[i].vel_y = -hip*sin(PI*45/180);
                    break;

                case 3:
                    corpos[i].vel_x = hip*cos(PI*22.5/180); 
                    corpos[i].vel_y = -hip*sin(PI*22.5/180);
                    break;

                case 4: 
                    corpos[i].vel_x = hip; 
                    corpos[i].vel_y = 0;
                    break;

                case 5:
                    corpos[i].vel_x = hip*cos(PI*-22.5/180); 
                    corpos[i].vel_y = -hip*sin(PI*-22.5/180);
                    break;

                case 6:
                    corpos[i].vel_x = hip*cos(PI*-45/180); 
                    corpos[i].vel_y = -hip*sin(PI*-45/180);
                    break;

                case 7:
                    corpos[i].vel_x = hip*cos(PI*-67.5/180); 
                    corpos[i].vel_y = -hip*sin(PI*-67.5/180);
                    break;

                case 8:
                    corpos[i].vel_x = 0; 
                    corpos[i].vel_y = hip;
                    break;

                case 9:
                    corpos[i].vel_x = hip*cos(PI*-112.5/180); 
                    corpos[i].vel_y = -hip*sin(PI*-112.5/180);
                    break;

                case 10:
                    corpos[i].vel_x = hip*cos(PI*-135/180); 
                    corpos[i].vel_y = -hip*sin(PI*-135/180);
                    break;

                case 11:
                    corpos[i].vel_x = hip*cos(PI*-157.5/180); 
                    corpos[i].vel_y = -hip*sin(PI*-157.5/180);
                    break;

                case 12:
                    corpos[i].vel_x = -hip; 
                    corpos[i].vel_y = 0;
                    break;

                case 13:
                    corpos[i].vel_x = hip*cos(PI*-202.5/180); 
                    corpos[i].vel_y = -hip*sin(PI*-202.5/180);
                    break;

                case 14:
                    corpos[i].vel_x = hip*cos(PI*-225/180); 
                    corpos[i].vel_y = -hip*sin(PI*-225/180);
                    break;

                case 15:
                    corpos[i].vel_x = hip*cos(PI*-247.5/180); 
                    corpos[i].vel_y = -hip*sin(PI*-247.5/180);
                    break;

            }

        }

    }

    return rot;

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
        ang1 = interacaoTeclado(W, &corpos[0], 0, corpos, nCorpos, ang1);
        // ang2 = interacaoTeclado(W, &corpos[1], corpos, nCorpos, ang2);
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