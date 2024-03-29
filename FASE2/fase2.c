 #include <stdio.h>	
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include "xwc.h"

/*
    GRUPO:

    Patrick Silva Souza         NUSP : 11221821
    Murilo Andre Gomes Felipe   NUSP : 11221863
    João Felipe Lobo Pevidor NUSP : 11221992
    Mohamad Hussein Rkein NUSP : 10740130

*/                
#define TAM_TOTAL 40000000 // arbitrário, só para teste
#define WIDTH 600
#define HEIGHT 600
#define WD_SPRITE 80
#define H_SPRITE 90


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
    PIC P;

} corpo;


double modulo(double x){
    if (x < 0)
        return -x;
    return x;
}


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
    planeta->P = NULL;
    // utilizamos a nave1 e nave2 para podermos le-las antes da leitura do nCorpos e então fazer o malloc

    if(fscanf(file, "%s %lf %lf %lf %lf %lf", nave1.nome, &(nave1.massa), &(nave1.pos_x), &(nave1.pos_y), &(nave1.vel_x), &(nave1.vel_y)) < 6)
        fprintf(stdout, "O arquivo esta defeituoso.\n");
	if(fscanf(file, "%s %lf %lf %lf %lf %lf", nave2.nome, &(nave2.massa), &(nave2.pos_x), &(nave2.pos_y), &(nave2.vel_x), &(nave2.vel_y)) < 6)
        fprintf(stdout, "O arquivo esta defeituoso.\n");
    /* definindo como NULL o ponteiro para a imagem que representa o corpo */
    nave1.P = NULL;
    nave2.P = NULL;

	i = fscanf(file, "%d %lf", nCorpos, duracaoProjs);

    corpos = malloc((*nCorpos+2)*sizeof(corpo));

    // agora podemos atribuir as naves às posições no vetor

    corpos[0] = nave1;
    corpos[1] = nave2;

	for (i = 2; i < (*nCorpos + 2); i ++){
		if(fscanf(file, "%lf %lf %lf %lf %lf", &(corpos[i].massa), &(corpos[i].pos_x), &(corpos[i].pos_y), &(corpos[i].vel_x), &(corpos[i].vel_y)) < 5)
            fprintf(stdout, "O arquivo esta defeituoso.\n");
	}
    return corpos;
}

void forcaResult(corpo *corpo1, corpo corpo2, double G){

	// Fr = (MmG)/d²
	// planeta está no 0,0

    double disty, distx;
    double resx, resy;

    distx  = corpo1->pos_x - corpo2.pos_x;
    disty  = corpo1->pos_y - corpo2.pos_y;

    if (distx != 0)
        resx = (corpo1->massa * corpo2.massa * G)/(pow(distx,2));

    else
        resx = 0;

    if (disty != 0)
        resy = (corpo1->massa * corpo2.massa * G)/(pow(disty,2));
    
    else
        resy = 0;

    // a posição do corpo influencia o sinal do vetor resultante

    if (corpo1->pos_x > corpo2.pos_x)
        corpo1->fr_x -= resx;

    else if(corpo1->pos_x < corpo2.pos_x)
        corpo1->fr_x += resx;

    if (corpo1->pos_y > corpo2.pos_y)
        corpo1->fr_y -= resy;

    else if (corpo1->pos_y < corpo2.pos_y)
        corpo1->fr_y += resy;

} 

void atualiza(corpo *corpos, double tempo){

	int acelx, acely;
    tempo = tempo/1000;

    // série de casos caso o corpo tenha saído da tela

	if (corpos->pos_x < -(TAM_TOTAL))
		corpos->pos_x += 2*TAM_TOTAL;

	if(corpos->pos_x > TAM_TOTAL)
		corpos->pos_x -= 2*TAM_TOTAL;

	if(corpos->pos_y < -(TAM_TOTAL))
		corpos->pos_y += 2*TAM_TOTAL;

	if(corpos->pos_y > TAM_TOTAL)
		corpos->pos_y -= 2*TAM_TOTAL;
  
    // a = F/m

    acelx = (corpos->fr_x)/(corpos->massa);
    acely = (corpos->fr_y)/(corpos->massa);

    // v = vo + at

	corpos->vel_x = corpos->vel_x + acelx*tempo;
	corpos->vel_y = corpos->vel_y + acely*tempo;

    // S = So +vt + (at^2)/2

	corpos->pos_x = corpos->pos_x + (corpos->vel_x)*tempo + acelx*tempo*tempo/2;
	corpos->pos_y = corpos->pos_y + (corpos->vel_y)*tempo + acely*tempo*tempo/2;


}

int verifica(corpo corpo1, corpo corpo2, double planeta, double raioPlaneta){

    /*

    essa função serve para tratar os casos em que talvez há "colisão"
    ou seja em que os corpos estão na mesma posição

    */

    if(planeta == 0){
        if(corpo1.pos_x == corpo2.pos_x && corpo1.pos_y == corpo2.pos_y){
            printf("As duas nave colidiram\n\n");
            return (1);
        }
        
    }
    else{
        if(modulo(corpo1.pos_x - corpo2.pos_x) <= raioPlaneta && modulo(corpo1.pos_y - corpo2.pos_y) <= raioPlaneta){
            printf("A nave %s colidiu com o planeta\n\n", corpo2.nome);
            return (1);
        }
    }

    return 0;

}

/*
    FUNCOES DA PARTE GRAFICA ABAIXO
*/

int calculaOrientacao(corpo* corpos, int nave){

    //corpos[0] e [1]
    double arc, rad;
    int n;
    if(corpos[nave].vel_y != 0){

        /*rad equivale a 22,5 graus em radianos*/

        rad = 2*acos(-1)*(22.5/360);

<<<<<<< HEAD
        arc = atan(corpos[nave].vel_y/corpos[nave].vel_x);
        n = arc/rad;
        if(corpos[nave].vel_y < 0) n += 8;
=======
        arc = atan(corpos[nave].vel_x/(corpos[nave].vel_y));

        if(arc < 0) arc += acos(-1);

        if((corpos[nave].vel_y) < 0) n += 8;
>>>>>>> 57af7f08fe8274f20bba3d3f969e5d04a8389b3a
        if(n == 0){
            if(corpos[nave].vel_x < 0)
                n = 8;
        }

        /*n = 4 eh a primeira sprite*/

        if(n-4 >= 0) n -= 4;
        else n = 16 + (n-4);
        return(n);
    }
return 0;

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
void atualizarJanela(int nCorpos, corpo *corpos, corpo planeta, WINDOW *W, PIC fundo, PIC planetaPIC, PIC projPIC, PIC saveiro[], PIC corsinha[], MASK mask1[], MASK mask2[], MASK mask3, MASK projMASK)
{

    int ang1, ang2, x, y, i;

    /*pinta as naves calculando primeiro a orientacao
    *
    *As naves, projeteis e o planeta sao renderizados de maneira centralizada subtraindo das coordenadas x e y
    *metade de suas larguras e alturas.
    *
    */

    WClear(W);
    PutPic(W, fundo, 0, 0, WIDTH, HEIGHT, 0, 0);

<<<<<<< HEAD
=======
    SetMask(W, mask3);
    PutPic(W, planetaPIC, 0, 0, 200, 200, (WIDTH/2)-100, (HEIGHT/2)-100);
    UnSetMask(W);
>>>>>>> 57af7f08fe8274f20bba3d3f969e5d04a8389b3a

    ang1 = calculaOrientacao(corpos, 0);
    ang2 = calculaOrientacao(corpos, 1);

    x = posicaoGrafica(corpos[0].pos_x, WIDTH);
    y = posicaoGrafica(corpos[0].pos_y, HEIGHT);

<<<<<<< HEAD
    PutPic(W, corsinha[ang2%16], 0, 0, 80, 90, x, y);
=======
    SetMask(W, mask2[ang2]);
    PutPic(W, corsinha[ang2], 0, 0, WD_SPRITE, H_SPRITE, x-(WD_SPRITE/2), y-(H_SPRITE)/2);
>>>>>>> 57af7f08fe8274f20bba3d3f969e5d04a8389b3a
    UnSetMask(W);

    x = posicaoGrafica(corpos[1].pos_x, WIDTH);
    y = posicaoGrafica(corpos[1].pos_y, HEIGHT);

    SetMask(W, mask1[ang1]);
    PutPic(W, saveiro[ang1], 0, 0, WD_SPRITE, H_SPRITE, x-(WD_SPRITE/2), y-(H_SPRITE)/2);
    UnSetMask(W);

<<<<<<< HEAD
    UnSetMask(W);    

    SetMask(W, *mask3);

    x = posicaoGrafica(planeta.pos_x, 600);
    y = posicaoGrafica(planeta.pos_y, 600);

    PutPic(W, *planetaPIC, 0, 0, 200, 200, x, y);
    
=======
    SetMask(W, projMASK);
    for (i = 2; i < nCorpos+2; i++){
        x = posicaoGrafica(corpos[i].pos_x, WIDTH);
        y = posicaoGrafica(corpos[i].pos_y, HEIGHT);
        PutPic(W, projPIC, 0, 0, 18, 18, x-18, y-18);
    }
>>>>>>> 57af7f08fe8274f20bba3d3f969e5d04a8389b3a
    UnSetMask(W);

}

int main(int argc, char*argv[]){

	corpo *corpos, planeta;
	double freq, tempo, duracaoProjs, tempoSim, G;
    int nCorpos, i, j, colisao;
    colisao = 0;
	tempo = 0;
    G = 6.67 *pow(10, -11);

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

    corpos = leitura(&planeta, &tempoSim, &duracaoProjs, &nCorpos);
    
    /*

     posição, velocidade e força resultante inicial de todos os corpos
     que não são projéteis

    */

    printf("pos inicial de 1: %lf %lf\n", corpos[0].pos_x, corpos[0].pos_y);
    printf("pos inicial de 2 : %lf %lf\n", corpos[1].pos_x, corpos[1].pos_y);
    printf("pos inicial do planeta : %lf %lf\n\n", planeta.pos_x, planeta.pos_y);

    printf("vel inicial de 1 : %lf %lf\n", corpos[0].vel_x, corpos[0].vel_y);
    printf("vel inicial de 2 : %lf %lf\n", corpos[1].vel_x, corpos[1].vel_y);
    printf("vel inicial do planeta : %lf %lf\n\n", planeta.vel_x, planeta.vel_y);

    printf("força resultante inicial de 1 : %lf %lf\n", corpos[0].fr_x, corpos[0].fr_y);
    printf("força resultante inicial de 2 : %lf %lf\n", corpos[1].fr_x, corpos[1].fr_y);
    printf("força resultante inicial do planeta : %lf %lf\n\n", planeta.fr_x, planeta.fr_y); 

    /* inicializar janela */
    W = InitGraph(WIDTH, HEIGHT, "Jogo do Peixe");

    carregarSprites(W, &planetaPIC, &projPIC, &fundo, saveiro, corsinha, mask1, mask2, &mask3, &projMASK);
    
    atualizarJanela(nCorpos, corpos, planeta, W, fundo, planetaPIC, projPIC, saveiro, corsinha, mask1, mask2, mask3, projMASK);
    
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
            corpos[i].fr_x = 0;
            corpos[i].fr_y = 0;
            forcaResult(&corpos[i], planeta, G);       

            for(j = 0; j < nCorpos+2; j++ /*(1)*/ ){

                if(i != j /*(2)*/ )
                    forcaResult(&corpos[i], corpos[j], G);

            }
        }

        colisao += verifica(planeta, corpos[0], 1, planeta.raio);
        colisao += verifica(planeta, corpos[1], 1, planeta.raio);
        colisao += verifica(corpos[0], corpos[1], 0, 0);

    /*  
        atualiza as variáveis do planeta
        depois dos cálculos  
    */
        atualiza(&planeta, freq);
    /* 
        atualiza     as variáveis de todos os corpos
         depois dos cálculos
    */
        for(i = 0; i < nCorpos+2; i++){
            atualiza(&corpos[i] , freq);

    /* 
        saídas : atualização da posição, velocidade e força dos corpos
            que não são projéteis
    */

    /*
        atualizar passo a passo a posição dos corpos na janela
    */

        atualizarJanela(nCorpos, corpos, planeta, W, fundo, planetaPIC, projPIC, saveiro, corsinha, mask1, mask2, mask3, projMASK);
    


        printf("pos de 1: %lf %lf\n", corpos[0].pos_x, corpos[0].pos_y);
        printf("pos de 2 : %lf %lf\n", corpos[1].pos_x, corpos[1].pos_y);
        printf("pos do planeta : %lf %lf\n\n", planeta.pos_x, planeta.pos_y);

        printf("vel de 1 : %lf %lf\n", corpos[0].vel_x, corpos[0].vel_y);
        printf("vel de 2 : %lf %lf\n", corpos[1].vel_x, corpos[1].vel_y);

        printf("força resultante de 1 : %lf %lf\n", corpos[0].fr_x, corpos[0].fr_y);
        printf("força resultante de 2 : %lf %lf\n", corpos[1].fr_x, corpos[1].fr_y);

        }
		tempo += freq;
        usleep(10000);
    }

    printf("Game Over\n");
    WDestroy(W);
    free(corpos);

}