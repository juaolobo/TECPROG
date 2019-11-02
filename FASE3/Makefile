CC     = gcc
CFLAGS = -Wall -O2 -std=c99 -lm -g -lX11 -lXpm -pedantic -Wno-unused-result -Wno-deprecated-declarations
RM     = rm

#--------------------------------------

fase3 : fisica.o graphic.o main.o xwc.o corpos.o
	$(CC) fisica.o graphic.o main.o xwc.o corpos.o -o fase3 $(CFLAGS)

main.o : main.c fisica.h graphic.h xwc.h
	$(CC) $(CFLAGS) -c main.c

fisica.o : fisica.c fisica.h
	$(CC) $(CFLAGS) -c fisica.c

corpos.o : corpos.c corpos.h
	$(CC) $(CFLAGS) -c corpos.c

xwc.o : xwc.c xwc.h
	$(CC) $(CFLAGS) -c xwc.c

graphic.o : graphic.c graphic.h xwc.h 
	$(CC) $(CFLAGS) -c graphic.c

clean:
	$(RM) *.o *~ fase3
