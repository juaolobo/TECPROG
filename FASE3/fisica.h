#ifndef FISICA_H
#define FISICA_H

#include <math.h>
#include "corpos.h"

#define TAM_TOTAL 20000000 // arbitrário, só para teste

void forcaResult(corpo *, corpo, double);

void atualiza(corpo *, double);

int verifica (corpo *, corpo , int , double , double );

#endif