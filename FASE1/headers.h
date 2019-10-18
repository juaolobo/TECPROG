/*

headers.h

definição da strcut de corpos e protótipo das
funções

*/

#define CENTRO_X 0
#define CENTRO_Y 0
#define TAM_TOTAL

typedef struct corpo {

	char nome[20];
	double massa;
	double pos_x; 	
	double pos_y;
	double vel_x;
	double vel_y;

} corpo;

void leitura(corpo, corpo, corpo, double, double, int, int, int);

double forcaResult(corpo, double);

void atualiza(double, corpo, double, int);