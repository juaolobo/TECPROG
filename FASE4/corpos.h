#ifndef CORPOS_H
#define CORPOS_H

#define TAM_TOTAL 20000000 // arbitrário, só para teste
#define WD_SPRITE 80
#define H_SPRITE 90
#define RAIO_PROJETIL 9

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

corpo * leitura(corpo *, double *, double *, int *);

corpo *inicializaCorpos(corpo *, int *);

#endif
