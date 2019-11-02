#ifndef GRAPHIC_H
#define GRAPHIC_H

#include "xwc.h"
#include "corpos.h"

int calculaOrientacao(corpo corpo);

int interacaoTeclado(WINDOW *W, corpo *nave, int naveNum ,corpo *corpos, int nCorpos, int rot);

void carregarSprites(WINDOW *W, PIC *planetaPIC, PIC *projPIC, PIC *fundo, PIC saveiro[], PIC corsinha[], MASK mask1[], MASK mask2[], MASK *mask3, MASK *projMASK);

int posicaoGrafica(double x, int eixo);

void atualizarJanela(int init, int nCorpos, corpo *corpos, WINDOW *W, PIC fundo, PIC planetaPIC, PIC projPIC, PIC saveiro[], PIC corsinha[], MASK mask1[], MASK mask2[], MASK mask3, MASK projMASK);

#endif