 #include <stdio.h>	
#include <stdlib.h>
#include <math.h>

/*
    GRUPO:

    Patrick Silva Souza NUSP : 11221821
    Murilo Andre Gomes Felipe NUSP : 11221863
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
    planeta->fr_x = 0;
    planeta->fr_y = 0;


    // utilizamos a nave1 e nave2 para podermos le-las antes da leitura do nCorpos e então fazer o malloc

    fscanf(file, "%s %lf %lf %lf %lf %lf", nave1.nome, &(nave1.massa), &(nave1.pos_x), &(nave1.pos_y), &(nave1.vel_x), &(nave1.vel_y));
	fscanf(file, "%s %lf %lf %lf %lf %lf", nave2.nome, &(nave2.massa), &(nave2.pos_x), &(nave2.pos_y), &(nave2.vel_x), &(nave2.vel_y));

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


int main(int argc, char*argv[]){

	corpo *corpos, planeta;
	double freq, tempo, duracaoProjs, tempoSim, G;
    int nCorpos, i, j, colisao;
    colisao = 0;
	tempo = 0;
    G = 6.67 *pow(10, -11);

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

    while(colisao == 0){		

    /*
        esse loop calcula a resultante do planeta
        em função de todos os corpos que existem
        ao seu redor e verifica para ver se 
        nenhum deles está na mesma posição que
        o planeta

    */

        for (i = 0; i < nCorpos+2; i++){
            planeta.fr_y = 0;
            planeta.fr_x = 0;
            forcaResult(&planeta, corpos[i], G);  
        }   

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
        atualiza as variáveis de todos os corpos
         depois dos cálculos
    */
        for(i = 0; i < nCorpos+2; i++){
            atualiza(&corpos[i] , freq);

    /* 
        saídas : atualização da posição, velocidade e força dos corpos
            que não são projéteis
    */

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
    free(corpos);

}