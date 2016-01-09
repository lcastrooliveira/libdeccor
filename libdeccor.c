#include "simpletools.h"
#include "abdrive.h"                          // Include abdrive header
#include "deccor.h"
#include "ping.h"

// Valor da leitura
int leitura_atual;
int cor_atual;
int cor_anterior;
int estado_atual;
  
int nova_leitura;

int main()                    
{  
  //leitura_atual = lerValorSensor();  
  //cor_atual = BRANCO;
  pause(5000);
  //drive_speed(8,8);
  
  cog_run(startLeitura,128);
  //pause(1000);
  while(ping_cm(11) >= 5)
  {
    leitura_atual = getLeituraAtual();
    cor_atual = getCorAtual(); 
    cor_anterior = getCorAnterior();
    estado_atual = getEstadoAtual();
    print("%cESTADO_ATUAL: = %d\n",HOME, estado_atual);
    print("COR_ATUAL: = %d\n", cor_atual);
    print("COR_ANTERIOR: = %d\n", cor_anterior);
    print("LEITURA SENSOR: = %d%c", leitura_atual,CLREOL);
    pause(10);
   
  }
  drive_speed(0,0);
  
  //avancar1Quadrado();
}
