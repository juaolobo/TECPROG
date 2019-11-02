#include <stdio.h>
#include "xwc.h"

int main(){

	WINDOW *w;

	w = InitGraph(600, 600, "teste");
	WCor(w, 0xffd700);
	//WFillRect(w, 0, 0, 600, 600, 0);

	InitKBD(w);

	if(WCheckKBD(w)){
		printf("AA");
	}

	return 0;
}