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
#define TAM_TOTAL 10000000000000 // arbitrário, só para teste

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
	scanf("%s", arquivoEntrada);
	file = fopen(arquivoEntrada, "r"); 

	fscanf(file, "%lf %lf %lf", &(planeta->raio), &(planeta->massa), tempoSim);

    planeta->pos_x = 0;
    planeta->pos_y = 0;
    planeta->vel_x = 0;
    planeta->vel_y = 0;
    planeta->fr_y = 0;
    planeta->fr_x = 0;
    planeta->P = NULL;
    // utilizamos a nave1 e nave2 para podermos le-las antes da leitura do nCorpos e então fazer o malloc

    fscanf(file, "%s %lf %lf %lf %lf %lf", nave1.nome, &(nave1.massa), &(nave1.pos_x), &(nave1.pos_y), &(nave1.vel_x), &(nave1.vel_y));
	fscanf(file, "%s %lf %lf %lf %lf %lf", nave2.nome, &(nave2.massa), &(nave2.pos_x), &(nave2.pos_y), &(nave2.vel_x), &(nave2.vel_y));
    /* definindo como NULL o ponteiro para a imagem que representa o corpo */
    nave1.P = NULL;
    nave2.P = NULL;

	fscanf(file, "%d %lf", nCorpos, duracaoProjs);

    corpos = malloc((*nCorpos+2)*sizeof(corpo));

    // agora podemos atribuir as naves às posições no vetor

    corpos[0] = nave1;
    corpos[1] = nave2;

	for (i = 2; i < (*nCorpos + 2); i ++){
		fscanf(file, "%lf %lf %lf %lf %lf", &(corpos[i].massa), &(corpos[i].pos_x), &(corpos[i].pos_y), &(corpos[i].vel_x), &(corpos[i].vel_y));
	}
    return corpos;
}

void forcaResult(corpo *corpo1, corpo corpo2, double G){

	// Fr = (MmG)/d²
	// planeta está no 0,0

    double disty, distx, resx, resy;

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

	double acelx, acely;
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

int verifica(corpo corpo1, corpo corpo2, int planeta, double raioPlaneta){

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

void inicializarGrafico(WINDOW *W, corpo *corpos, PIC sprite1[], PIC sprite2[], MASK mask1[], MASK mask2[]){

    int i;
    char arquivo1[15] = "corsinha_00.xpm";
    char arquivo2[14] = "saveiro_00.xpm";
    for (i = 0; i < 1; i++){
        //sprite1[i] = NewPic(W, 135, 151);
        //sprite2[i] = NewPic(W, 135, 151);

        mask1[i] = NewMask(sprite1[i], 135, 151);
        mask2[i] = NewMask(sprite2[i], 135, 151);

        arquivo1[10] = 48 + (i % 10);
        arquivo1[9] = 48 + i/10;
        sprite1[i] = ReadPic(W, arquivo1, mask1[i]);
        arquivo2[9] = 48 + (i % 10);
        arquivo2[8] = 48 + i/10;
        sprite2[i] = ReadPic(W, arquivo2, mask2[i]);
        SetMask(W,mask1[i]);
        SetMask(W,mask2[i]);
    }

}

void inicializarJanela(corpo *corpos, WINDOW *W, int nCorpos, PIC fundo, PIC sprite1[], PIC sprite2[])
{
    int i;

    /* janela de fundo */
     /* pôr o nome do arquivo de fundo aqui */
    PutPic(W, fundo, 0, 0, 600, 600, 0, 0);

    
    /* dispor todos os corpos na janela */
    PutPic(W, sprite1[0], 0, 0, 135, 151, 0, 0);
    //PutPic(W, sprite2[0], 0, 0, 135, 151, 400, 400);

    /*ESSA PASSAGEM EH DESNECESSARIA*/
    /*
    for(i = 2; i < nCorpos + 2; i = i + 1)
    {
        corpos[i].P = NewPic(W, 18, 18);
        corpos[i].P = ReadPic(W, "bala1.xpm", NULL);
        PutPic(W, corpos[i].P, 0, 0, 18, 18, corpos[i].pos_x, corpos[i].pos_y);
    }
    */
}

void atualizarJanela(corpo *corpos, WINDOW *W, int nCorpos, PIC fundo, PIC sprite1[], PIC sprite2[], MASK mask1[], MASK mask2[])
{

    int i, n;

    //WLine(W,0,0,100, 100*tan(15), 0x000495);

    /* limpa imagem pintando o fundo por cima */
    PutPic(W, fundo, 0, 0, 600, 600, 0, 0);

    /* pinta as naves calculando primeiro a orientacao */
    /*
    for(i = 0; i < 2; i++){
        n = calculaOrientacao(corpos, i);
        PutPic(W, corpos[i].P, n*135, 0, 135, 151, 0, 0);
    }*/


    PutPic(W, sprite1[0], 0, 0, 135, 151, 0, 0);
    //PutPic(W, sprite2[0], 0, 0, 135, 151, 400, 400);
    

    /* pinta as balas */

    for (i = 2; i< nCorpos; i++){
        //PutPic(W, corpos[i].P, 0, 0, 18, 18, 0, 0);
    }
}

int calculaOrientacao(corpo* corpos, int nave){

    //corpos[0] e [1]
    double arc, rad;
    int n;

    rad = 2*acos(-1)*(22.5/360);

    arc = atan(corpos[nave].vel_x/corpos[nave].vel_y);
    n = arc/rad;
    if(corpos[nave].vel_y < 0) n += 8;
    if(n == 0){
        if(corpos[nave].vel_x < 0)
            n = 8;
    }

    /*n = 4 eh a primeira sprite*/

    if(n-4 >= 0) n -= 4;
    else n = 16 + (n-4);

    return(n);
}


int main(int argc, char*argv[]){

	corpo *corpos, planeta;
	double freq, tempo, duracaoProjs, tempoSim, G;
    int nCorpos, i, j, colisao;
    colisao = 0;
	tempo = 0;
    WINDOW *W, *TELA;
    PIC fundo;
    G = 6.67 *pow(10, -11);

    PIC sprite1[16], sprite2[16];
    MASK mask1[16], mask2[16];


	if(argc > 1)
		freq = atof(argv[1]);

	else{
		printf("Digite a frequência de atualização : ");
		scanf("%lf", &freq);
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
    W = InitGraph(600,600, "Jogo do Peixe");
    fundo = NewPic(W, 600, 600);
    fundo = ReadPic(W, "campodefut.xpm", NULL);

    inicializarGrafico(W, corpos, sprite1, sprite2, mask1, mask2);

    inicializarJanela(corpos, W, nCorpos, fundo, sprite1, sprite2);
    for(i = 0; i > 16; i++){
        SetMask(sprite1[i], mask1[i]);
        SetMask(sprite2[i], mask2[i]);
    }
    atualizarJanela(corpos, W, nCorpos, fundo, sprite1, sprite2, mask1, mask2);
    while(1){}
    /* inicializar objetos */
    colisao = 1;
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

        atualizarJanela(corpos, W, nCorpos, fundo, sprite1, sprite2, mask1, mask2); 

        printf("pos de 1: %lf %lf\n", corpos[0].pos_x, corpos[0].pos_y);
        printf("pos de 2 : %lf %lf\n", corpos[1].pos_x, corpos[1].pos_y);
        printf("pos do planeta : %lf %lf\n\n", planeta.pos_x, planeta.pos_y);

        printf("vel de 1 : %lf %lf\n", corpos[0].vel_x, corpos[0].vel_y);
        printf("vel de 2 : %lf %lf\n", corpos[1].vel_x, corpos[1].vel_y);
        printf("vel do planeta : %lf %lf\n\n", planeta.vel_x, planeta.vel_y);

        printf("força resultante de 1 : %lf %lf\n", corpos[0].fr_x, corpos[0].fr_y);
        printf("força resultante de 2 : %lf %lf\n", corpos[1].fr_x, corpos[1].fr_y);
        printf("força resultante do planeta : %lf %lf\n\n", planeta.fr_x, planeta.fr_y);

        }

		tempo += freq;
    }

    printf("Game Over\n");
    WDestroy(W);
    free(corpos);

}