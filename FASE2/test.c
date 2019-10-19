#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "xwc.h"

int main()
{
    int i;
    int x0; // qual rotação da nave queremos
    PIC aux;
    WINDOW *W;
    PIC p1, p2, p3;

    /* janela de fundo */
    W = InitGraph(600,600, "Jogo do Peixe");
    ReadPic(W, "../imagens/estrelas.xpm", NULL); /* pôr o nome do arquivo de fundo aqui */
    
    /*planeta->m = NewMask(planeta->P, wd, h); // completar o tamanho do planeta
    aux = ReadPic(W, "_mask.xpm", planeta->m);
    planeta->P = ReadPic(W, ".xpm");
    PutPic(W, planeta->P, 0, 0, wd, h, planeta.pos_x, planeta.pos_y); // completar o tamnanho  
    SetMask(planeta->P, planeta->m);*/

    /* dispor todos os corpos na janela */
    p1 = NewMask(p1, 135, 151);
    aux = ReadPic(W, "../imagens/corsinha/masks/corsinha_tunado_mask.xpm", p1);
    p1 = ReadPic(W, "../imagens/corsinha/corsinha_tunado.xpm", NULL);  /* imagem da nave 1 */
    PutPic(W, p1, x0, 0, 135, 151, p1, p1);
    SetMask(p1, p1);

    p2 = NewMask(p2, 135, 151);
    aux = ReadPic(W, "../imagens/saveiro/masks/saveiro_rebaixado180_mask.xpm", p2);
    p2 = ReadPic(W, "../imagens/saveiro/saveiro_rebaixado180.xpm", NULL); /* imagem da nave 2 */
    PutPic(W, p2, x0, 0, 135, 151, p2, p2);
    SetMask(p2, p2);
}