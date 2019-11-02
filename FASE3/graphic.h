#ifndef GRAPHIC_H
#define GRAPHIC_H

#include "xwc.h"
#include "corpos.h"

#define SHOOT1 50
#define LEFT1 38
#define RIGHT1 40
#define ACEL1 25
#define SHOOT2 65
#define LEFT2 44
#define RIGHT2 46
#define ACEL2 31
#define WIDTH 600
#define HEIGHT 600

int calculaOrientacao(corpo corpo);

int interacaoTeclado(WINDOW *W, corpo *nave, int naveNum ,corpo *corpos, int nCorpos, int rot);

void carregarSprites(WINDOW *W, PIC *planetaPIC, PIC *projPIC, PIC *fundo, PIC saveiro[], PIC corsinha[], MASK mask1[], MASK mask2[], MASK *mask3, MASK *projMASK);

int posicaoGrafica(double x, int eixo);

void atualizarJanela(int init, int nCorpos, corpo *corpos, WINDOW *W, PIC fundo, PIC planetaPIC, PIC projPIC, PIC saveiro[], PIC corsinha[], MASK mask1[], MASK mask2[], MASK mask3, MASK projMASK);

#endif