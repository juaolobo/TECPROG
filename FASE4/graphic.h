#ifndef GRAPHIC_H
#define GRAPHIC_H

#include "xwc.h"
#include "corpos.h"

#define JUMP1 24
#define SHOOT1 50
#define LEFT1 38
#define RIGHT1 40
#define ACEL1 25
#define JUMP2 32
#define SHOOT2 65
#define LEFT2 44
#define RIGHT2 46
#define ACEL2 31
#define WIDTH 600
#define HEIGHT 600

int calculaOrientacao(corpo corpo);

void interacaoTeclado(WINDOW *W,corpo *corpos, int nCorpos, int *warp);

void carregarMenu(WINDOW *W, PIC *menuI, PIC *menuS, PIC *textoVidas, PIC *numeros, PIC *corsa, PIC *saveiro, PIC *empate);

void renderMenu(WINDOW *W, PIC menuAtual);

void carregarSprites(WINDOW *W, PIC *planetaPIC, PIC *projPIC, PIC *fundo, PIC *numeros, PIC saveiro[], PIC corsinha[], MASK mask1[], MASK mask2[], MASK *mask3, MASK *projMASK);

void carregarEfeitos(WINDOW *W, PIC explosion[], MASK maskExp[], PIC warp[], MASK maskWarp[]);

int posicaoGrafica(double x, int eixo);

void atualizarJanela(int init, int nCorpos, int vidasSav, int vidasCorsa, corpo *corpos, WINDOW *W, PIC fundo, PIC planetaPIC, PIC projPIC, PIC numeros, PIC saveiro[], PIC corsinha[], PIC warp[], MASK mask1[], MASK mask2[], MASK mask3, MASK projMASK, MASK maskWarp[]);

void explosao(WINDOW *W, corpo *corpos, int nave, PIC explosion[], MASK mask[]);

void warp(WINDOW *W, corpo *corpos, PIC warp[], MASK mask[]);

int menuVidas (PIC fundo, WINDOW *W, PIC textoVidas, PIC numeros);

void fimDeJogo(PIC fundo, WINDOW *W, PIC vencedor);

void resetLoops();

#endif
