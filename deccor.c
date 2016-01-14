#include "deccor.h"

  
// Valor da leitura

int leitura_anterior = 250000;

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
    drive_speed(MSPEED,MSPEED);
    //pause(50);
  }
  drive_speed(0,0);
  //drive_speed(MSPEED,MSPEED);
  drive_goto(40,40);
  return TRUE;    
}  

int avancar1QuadradoVolta() {
  if (ping_cm(11) <= 7) {
    drive_speed(0,0);
    return FALSE;
  }
  freqout(4, 500, 3000);     
  while(cor_atual != PRETO) {
    drive_speed(MSPEED,MSPEED);
    pause(50);
  }
  drive_speed(0,0);
  //drive_speed(MSPEED,MSPEED);
  drive_goto(40,40);
  return TRUE;    
}

void startLeitura() {
  estado_atual = CAPTURA;
  cor_atual = PRETO;
  cor_anterior = PRETO;
  leitura_atual = 250000;
  while(1)
  {
    high(26);
    leitura_anterior = leitura_atual;
    leitura_atual = lerValorSensor();
    cor_anterior = cor_atual;
    if (cor_atual == BRANCO && leitura_atual - leitura_anterior > 25000) {
      cor_atual = PRETO;
      high(27);
    } 
    else if (cor_atual == PRETO && leitura_anterior - leitura_atual > 25000) {
      cor_atual = BRANCO;
      low(27);
    }
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
      //cog_beep = cog_run(beep,128);
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
    low(26);
    pause(50);
  }
}

inline void gravarValor() {
  int estado_int = estado_atual;
  unsigned char* estado;
  switch(estado_int) {
    case REG_B:
      estado = "B";
      //cog_beep = cog_run(beep,128);
      break;
    case REG_P:
      estado = "P";
      //cog_beep2 = cog_run(beep2,128);
      break;
  }
  num_vertices++;
  //sd_mount(DO, CLK, DI, CS);                  // Mount SD card
  FILE* fp = fopen("quadrado.txt", "a");          // Open a file for writing
  fwrite(estado, 1, 1, fp);      // Add contents to the file
  fclose(fp);                                 // Close the file
  cog_end(cog);
}  