#ifndef CORPOS_H
#define CORPOS_H

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

#endif