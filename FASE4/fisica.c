#include "fisica.h"
#include "corpos.h"
#include <stdio.h>
#include <math.h>
#include <string.h>
#define EPS 0.00001
/*
    forcaResult - fisica.c :: Recalcula a força resultante do corpo 1 em relação ao corpo 2. 
*/
void forcaResult(corpo *corpo1, corpo corpo2, double G){


    double  distx, disty, dist2x, dist2y, dist, 
            resx = 0, resy = 0, res = 0, 
            coA = 0, coB = 0, 
            Cx, Cy, Dx, Dy;
    int     inverte = 0;
    FILE * log;

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
        if(fabs(corpo1->pos_x - corpo2.pos_x) > EPS)
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
            se o corpo1 estiver mais próximo do ponto C, há que a distância a seguir é a soma das distâncias entre os pares (corpo1, C) e (corpo2, D). Caso o contrário, (corpo1, D) e (corpo2, D).     
        */

        dist2x = (sqrt(pow(corpo1->pos_x - Cx, 2) + pow(corpo1->pos_y - Cy, 2)) < sqrt(pow(corpo1->pos_x - Dx, 2) + pow(corpo1->pos_y - Dy, 2)) ? fabs(corpo1->pos_x - Cx) + fabs(corpo2.pos_x - Dx) : fabs(corpo1->pos_x - Dx) + fabs(corpo2.pos_x - Cx));
        dist2y = (sqrt(pow(corpo1->pos_x - Cx, 2) + pow(corpo1->pos_y - Cy, 2)) < sqrt(pow(corpo1->pos_x - Dx, 2) + pow(corpo1->pos_y - Dy, 2)) ? fabs(corpo1->pos_y - Cy) + fabs(corpo2.pos_y - Dy) : fabs(corpo1->pos_y - Dy) + fabs(corpo2.pos_y - Cy));

        /* a menor das distâncias é a distância que define a força resultante */
        if(sqrt(pow(distx, 2) + pow(disty, 2)) - sqrt(pow(dist2x, 2) + pow(dist2y, 2)) > EPS)
        {
            distx = dist2x;
            disty = dist2y;
            inverte = 1;
        }
        
        dist = sqrt(pow(distx,2) + pow(disty,2));
        if(dist > EPS)
        {
            res = (corpo1->massa * corpo2.massa * G) / pow(dist, 2);
            if(fabs(corpo1->pos_x - corpo2.pos_x) > EPS)
            {
                /* coA é o coeficiente de inclinação da reta que passa pelas posições dos corpos, a sua tangente */
                resx = cos(atan(coA)) * res; 
                resy = sin(atan(coA)) * res;
            }
            else
                resy = res;  
        }
        else
            resx = resy = 0;

        // a posição do corpo influencia o sinal do vetor resultante
        if(inverte)
        {
            resx = -resx;
            resy = -resy;
        }

        if (corpo1->pos_x - corpo2.pos_x > EPS)
            corpo1->fr_x -= resx;

        else if(corpo1->pos_x - corpo2.pos_x < -EPS)
            corpo1->fr_x += resx;

        if (corpo1->pos_y - corpo2.pos_y > EPS)
            corpo1->fr_y -= resy;

        else if (corpo1->pos_y - corpo2.pos_y < -EPS)
            corpo1->fr_y += resy;
    }
}

/* 
    atualiza - fisica.c :: Recalcula acelaração, velocidade, posição de um corpo.    
*/
void atualiza(corpo *corpo, double tempo){

	double acelx, acely;
    tempo = tempo/1000;

    // série de casos caso o corpo tenha saído da tela

	if (corpo->pos_x < -(TAM_TOTAL/2))
		corpo->pos_x = TAM_TOTAL/2;

	if(corpo->pos_x > TAM_TOTAL/2)
		corpo->pos_x = -TAM_TOTAL/2;

	if(corpo->pos_y < -(TAM_TOTAL/2))
		corpo->pos_y = TAM_TOTAL/2;

	if(corpo->pos_y > TAM_TOTAL/2)
		corpo->pos_y = -TAM_TOTAL/2;
  
    // a = F/m

    acelx = (corpo->fr_x)/(corpo->massa);
    acely = (corpo->fr_y)/(corpo->massa);

    // S = So +vot + (at^2)/2

	corpo->pos_x = corpo->pos_x + (corpo->vel_x)*tempo + acelx*tempo*tempo/2;
	corpo->pos_y = corpo->pos_y + (corpo->vel_y)*tempo + acely*tempo*tempo/2;


    // v = vo + at

	corpo->vel_x = corpo->vel_x + acelx*tempo;
	corpo->vel_y = corpo->vel_y + acely*tempo;    
}

/*
    verifica - fisica.c :: Verifica se houve colisão entre corpos.
*/
int verifica (corpo *corpos, corpo planeta, int nCorpos, double freq, double tempoSim) {

    int i, nave;

    /* ve se as naves tem vida ainda para continuar o jogo */

    for (i = 0; i < 2; i++){

        if (corpos[i].vida == 0){

            printf("\nA nave %s morreu\n\n", corpos[i].nome);
            if (i == 0) return 0;
            return 1;

        }  
    }

    /* calcula se as naves colidiram com elas mesmas  */

    if (sqrt(pow((corpos[0].pos_x - corpos[1].pos_x), 2) + pow((corpos[0].pos_y - corpos[1].pos_y), 2)) <= corpos[0].raio + corpos[1].raio){
        printf("\nAs duas naves colidiram\n\n");
        return 2;
    }

    /* calcula se as naves não colidiram com o planeta */

    for (i = 0; i < 2; i++)
        if (sqrt(pow(corpos[i].pos_x, 2) + pow(corpos[i].pos_y, 2)) <= planeta.raio + corpos[i].raio) {
            printf("\nA nave %s colidiu com o planeta\n\n", corpos[i].nome);
            if (i == 0) return 0;
            return 1;
        }

    /* calcula as colisões dos projeteis */

    for (i = 2; i < nCorpos+2; i++) {

        if (corpos[i].massa != -1){

            corpos[i].tempoVida += freq;

            for(nave = 0; nave < 2; nave++){    

                if (sqrt(pow((corpos[nave].pos_x - corpos[i].pos_x), 2) + pow((corpos[nave].pos_y - corpos[i].pos_y), 2)) <= corpos[nave].raio + (RAIO_PROJETIL*TAM_TOTAL/600)) {

                    corpos[nave].vida -= 10;
                    corpos[i].massa = -1;
                    corpos[i].tempoVida = 0;
                    printf("\nVida %s : %d\n", corpos[nave].nome, corpos[nave].vida);

                } 
            }

            if (corpos[i].tempoVida == tempoSim || 
                sqrt(pow(corpos[i].pos_x, 2) + pow(corpos[i].pos_y, 2)) <= planeta.raio){
                corpos[i].massa = -1;
                corpos[i].tempoVida = 0;
            }

        }
    }

    return -1;


}

