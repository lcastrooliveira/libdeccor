#define BRANCO 42
#define PRETO 43

//Automato de Estados
#define CAPTURA 1
#define REG_B 2
#define REG_P 3
#define FIM_P 4
#define LINHA 5
#define FIM_LINHA 6
#define DELAY 7

#define TRUE 1
#define FALSE 0;

#include "simpletools.h"

int lerValorSensor(void);
int getCorAtual(void);
int getCorAnterior(void);
int getLeituraAtual(void);
int getEstadoAtual(void);
void startLeitura(void);
int avancar1Quadrado(void);

