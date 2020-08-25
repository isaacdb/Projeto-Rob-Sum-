#include <TimerOne.h>
unsigned long T;
unsigned long E;

#define trava 150                   //CONSTANTES usadas para facilitar mudanças na programação
#define tre  30                      //Usadas no valor de leitura dos sensores de linha, e nas equações de tempo.
#define tgiro 60
#define tgirototal 70
#define vslin1 400
#define vslin2 400
#define vslin3 400
#define vslin4 400

int SobsF = 12; //define o pino digital como entrada do sensor de obstaculo da frente                  //fio preto sinal, gnd azul, marrom vcc.
int SobsT = 11; //define o pino digital como entrada do sensor de obstaculo de traz
int SobsE = 10; //define o pino digital como entrada do sensor de obstaculo da esquerda
int SobsD = 9; //define o pino digital como entrada do sensor de obstaculo da direita
int Slin1 = A2; //define o pino analogico como entrada do sensor de linha frente esq
int Slin2 = A1; //define o pino analogico como entrada do sensor de linha frente dir
int Slin3 = A3; //define o pino analogico como entrada do sensor de linha traseiro esq
int Slin4 = A0; //define o pino analogico como entrada do sensor de linha traseiro dir

int Linha[4];
int Visao[4];

int MdirF = 6;        //Define o SENTIDO de rotação dos motores.
int MdirT = 5;        //sinal HIGH faz a açao
int MesqF = 4;
int MesqT = 3;

int  DirecaoEsq;
int  DirecaoDir;

int R;

int Acao;

void Leitura(void);

void ConfigTimer(void) {
  Timer1.initialize(10000);
  Timer1.attachInterrupt(Tempo);
}
void Tempo(void) {
  noInterrupts();
  T++;
  E++;
  interrupts();
}
void Rampa (void) {
  digitalWrite(MdirF, 0);
  digitalWrite(MdirT, digitalRead(MdirF) ^ 1);

  digitalWrite(MesqF, 0);
  digitalWrite(MesqT, digitalRead(MesqF) ^ 1);

  delay(100);

  digitalWrite(MdirF, 1);
  digitalWrite(MdirT, digitalRead(MdirF) ^ 1);

  digitalWrite(MesqF, 1);
  digitalWrite(MesqT, digitalRead(MesqF) ^ 1);
}

void setup() {

  ConfigTimer();

  pinMode(MdirF, OUTPUT);      //DEFINE os pinos dos motores como saidas
  pinMode(MdirT, OUTPUT);
  pinMode(MesqF, OUTPUT);
  pinMode(MesqT, OUTPUT);

  delay(5000);
  Rampa();

}
void loop() {
  Leitura();
  Condicao();
  Action();
  Motores();

}

void Motores() {
  digitalWrite(MdirF, DirecaoDir);
  digitalWrite(MdirT, digitalRead(MdirF) ^ 1);

  digitalWrite(MesqF, DirecaoEsq);
  digitalWrite(MesqT, digitalRead(MesqF) ^ 1);
}

void Leitura(void) {
  Linha[0] = analogRead(Slin1);
  Linha[1] = analogRead(Slin2);
  Linha[2] = analogRead(Slin3);
  Linha[3] = analogRead(Slin4);
  int i;
  for (i = 0; i < 4; i++) {
    if (Linha[i] > 400) {
      Linha[i] = 0;
    }
    else {
      Linha[i] = 1;
    }
  }

  Visao[0] = digitalRead(SobsF);
  Visao[1] = digitalRead(SobsD);
  Visao[2] = digitalRead(SobsT);
  Visao[3] = digitalRead(SobsE);


}

void Condicao() {
  if ( Linha[0] == 0 &&  Linha[1] == 0 &&  Linha[2] == 0 &&  Linha[3] == 0) {

    if (Visao[0] == 0 && R == 0) {
      Acao = 1; // frente
    }
    else if (Visao[1] == 0) {
      if (Acao != 4) {
        E = 0;
        Acao = 4; //Girar p Direita
      }
      T = 0;
    }
    else if (Visao[3] == 0) {
      if (Acao != 5) {
        E = 0;
        Acao = 5; //Girar p Esquerda
      }
      T = 0;
    }
    else if (Visao[2] == 0) {
      if (Acao != 6) {
        E = 0;
        Acao = 6; //Girar 360 p direita
      }
      T = 0;
    }
    else {
      Acao = Acao;
    }
  }
  else {
    if ( Linha[3] == 1 ||  Linha[4] == 1) {
      Acao = 1; //frente
    }
    else if ( Linha[0] == 1) {
      Acao = 2; //ReGirDir
      T = 0;
    }
    else if ( Linha[1] == 1) {
      Acao = 3; // ReGirEsq
      T = 0;
    }
    else if ( Linha[1] == 1 &&  Linha[0] == 1)
    {
      Acao = 2;  //ReGirDir
      T = 0;
    }
    else {
      Acao = Acao;
    }
  }
}

void Action() {

  switch (Acao) {
    case 1 :
      DirecaoEsq = 1;
      DirecaoDir = 1;
      break;
    case 2 :
      if (T <= tre) {
        DirecaoEsq = 0;
        DirecaoDir = 0;
        R = 1;
      }
      else if (T <= tgiro) {
        DirecaoEsq = 0;
        DirecaoDir = 1;
        R = 0;
      }
      else {

        Acao = 1;
      }
      break;
    case 3 :
      if (T <= tre) {
        DirecaoEsq = 0;
        DirecaoDir = 0;
        R = 1;
      }
      else if (T <= tgiro) {
        DirecaoEsq = 1;
        DirecaoDir = 0;
        R = 0;
      }
      else {

        Acao = 1;
      }
      break;
    case 4 :
      if (E >= trava) {
        DirecaoEsq = 1;
        DirecaoDir = 1;
      }
      else if (T <= tgiro) {
        DirecaoEsq = 0;
        DirecaoDir = 1;
      }
      else {
        Acao = 1;
      }
      break;
    case 5 :
      if (E >= trava) {
        DirecaoEsq = 1;
        DirecaoDir = 1;
      }
      else if (T <= tgiro) {
        DirecaoEsq = 1;
        DirecaoDir = 0;
      }
      else {
        Acao = 1;
      }
      break;

    case 6 :

      if (E >= trava) {
        DirecaoEsq = 1;
        DirecaoDir = 1;
      }
      else if (T <= tgirototal) {
        DirecaoEsq = 0;
        DirecaoDir = 1;
      }
      else {
        Acao = 1;
      }
      break;

  }
}
