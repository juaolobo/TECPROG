#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "xwc.h"

int main()
{
    int i;
    WINDOW *W;
    PIC p1, p2, p3;
    MASK msk1, msk2;
    W = InitGraph(600, 600, "Jogo do Peixe");
    /*p1 = NewPic(W, 135, 151);
    p2 = NewPic(W, 135, 151);*/
    p3 = NewPic(W, 600, 600);


    p3 = ReadPic(W, "samp.xpm", NULL);
    PutPic(W, p3, 0, 0, 600, 600, 0, 0);
/*    p1 = ReadPic(W, "Nose.xpm", NULL);
    PutPic(W, p1, 0, 0, 135, 151, 100, 100);
    p2 = ReadPic(W, "mascara.xpm", NULL);
    PutPic(W, p2, 0, 0, 135, 151, 500, 500);
    
    /*planeta->m = NewMask(planeta->P, wd, h); // completar o tamanho do planeta
    aux = ReadPic(W, "_mask.xpm", planeta->m);
    planeta->P = ReadPic(W, ".xpm");
    PutPic(W, planeta->P, 0, 0, wd, h, planeta.pos_x, planeta.pos_y); // completar o tamnanho  
    SetMask(planeta->P, planeta->m);*/

    
    //msk1 = NewMask(p1, 135, 151);
    //aux = ReadPic(W, "../imagens/corsinha/masks/corsinha_tunado_mask.xpm", msk1);
    //SetMask(p1, msk1);

    //msk2 = NewMask(p2, 135, 151);
    //aux = ReadPic(W, "../imagens/saveiro/masks/saveiro_rebaixado180_mask.xpm", msk2);
    //SetMask(p2, msk2);

    CloseGraph();
    return 0;
}