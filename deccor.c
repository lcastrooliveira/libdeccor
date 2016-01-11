#include "deccor.h"

  
// Valor da leitura


void gravarValor(void);
int *cog;
int *cog_beep;
int *cog_beep2;

int DO = 22, CLK = 23, DI = 24, CS = 25;      // SD card pins on Propeller BOE

int lerValorSensor() {
  high(3);
  pause(1);
  return rc_time(3, 1);
}  

int getCorAtual() {
  return cor_atual;
}

int getLeituraAtual() {
  return leitura_atual;
}

int getCorAnterior() {
  return cor_anterior;
}  

int getEstadoAtual() {
  return estado_atual;
} 

void beep() {
  freqout(4, 500, 3000);
  cog_end(cog_beep);
}

void beep2() {
  freqout(4, 500, 2000);
  freqout(4, 500, 2000);
  cog_end(cog_beep2);
} 

int avancar1Quadrado() {
  if (ping_cm(11) <= 15) {
    drive_speed(0,0);
    return FALSE;
  }     
  while(estado_atual != LINHA) {
    drive_speed(16,16);
    //pause(50);
  }
  drive_speed(0,0);
  drive_speed(16,16);
  pause(5000);
  drive_speed(0,0);
  return TRUE;    
}  

void startLeitura() {
  estado_atual = CAPTURA;
  cor_atual = PRETO;
  cor_anterior = PRETO;
  leitura_atual = 0;
  while(1)
  {
    leitura_atual = lerValorSensor();
    cor_anterior = cor_atual;
    if (cor_atual == BRANCO && leitura_atual >= 150000) {
      cor_atual = PRETO;
    } 
    else if (cor_atual == PRETO && leitura_atual < 150000) {
      cor_atual = BRANCO;
    }
    //leitura_atual = nova_leitura;
    if(estado_atual == CAPTURA && cor_atual == BRANCO) {
      estado_atual = REG_B;
      //Se estiver mapeamento chamar funcao pra gravar texto (em outra thread)
      if(tarefa == MAPEAMENTO) cog = cog_run(gravarValor,128);
    } else if (estado_atual == CAPTURA && cor_atual == PRETO) {
      estado_atual = REG_P;
      //Se estiver mapeando chamar funcao pra gravar texto (em outra thread)
      if(tarefa == MAPEAMENTO) cog = cog_run(gravarValor,128);
    } else if (estado_atual == REG_B && cor_atual == PRETO && cor_anterior != cor_atual) {
      estado_atual = LINHA;
    } else if (estado_atual == REG_P && cor_atual == BRANCO && cor_anterior != cor_atual) {
      estado_atual = FIM_P;
    } else if (estado_atual == FIM_P && cor_atual == PRETO && cor_anterior != cor_atual) {
      estado_atual = LINHA;
    } else if (estado_atual == LINHA && cor_atual == BRANCO && cor_anterior != cor_atual) {
      estado_atual = FIM_LINHA;
    } else if(estado_atual == FIM_LINHA) {
      pause(750);
      estado_atual = CAPTURA;
    }
    pause(200);
  }
}

inline void gravarValor() {
  int estado_int = estado_atual;
  unsigned char* estado;
  switch(estado_int) {
    case REG_B:
      estado = "B";
      cog_beep = cog_run(beep,128);
      break;
    case REG_P:
      estado = "P";
      cog_beep2 = cog_run(beep2,128);
      break;
  }
  num_vertices++;
  //sd_mount(DO, CLK, DI, CS);                  // Mount SD card
  FILE* fp = fopen("quadrado.txt", "a");          // Open a file for writing
  fwrite(estado, 1, 1, fp);      // Add contents to the file
  fclose(fp);                                 // Close the file
  cog_end(cog);
}  