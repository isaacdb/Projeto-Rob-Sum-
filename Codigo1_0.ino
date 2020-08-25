#define trava 1500//CONSTANTES usadas para facilitar mudanças na programação
#define tre  500//Usadas no valor de leitura dos sensores de linha, e nas equações de tempo.
#define tgiro 300
#define tgirototal 700
#define vslin1 400
#define vslin2 400
#define vslin3 400
#define vslin4 400
//DEFINE os pinos digitais como ENTRADA dos sensores de obstáculos.
int SobsF = 11;  //FRENTE
int SobsT = 12;  //TRASEIRO
int SobsE = 9;    //ESQUERDO
int SobsD = 10;  //DIREITO

//DEFINE os pinos analógicos como ENTRADA dos sensores de linhas.
int Slin1 = A1;       //NOROESTE
int Slin2 = A0;       //NORDESTE
int Slin3 = A2;       //SUDOESTE
int Slin4 = A3;       //SUDESTE

int MdirF = 5;        //Define o SENTIDO de rotação dos motores.
int MdirT = 6;
int MesqF = 3;
int MesqT = 4;
int botao = 8;        //Define o pino digital 8 como SINAL do botão de inicio.

//VARIAVEIS de tempo necessárias para alguns pontos da programação.
//É usado mais de 1 variável para não haver conflito, por uma variável ser usada 2 vezes ao //mesmo tempo.

unsigned long Tinicio;
unsigned long Tinicio2;
unsigned long Tinicio3;
int estado;           //Variável responsável pelo inicio ou parada da movimentação do robô.

//FUNÇÕES criadas para cada tipo de ação, separadas da parte principal da programação.
void parar(void);
void start(void);
void resq(void);
void rdir(void);
void frente(void);
void tras(void);
void inimigoesquerda(void);
void inimigodireita (void);
void inimigoatras(void);

void setup() {

  pinMode(MdirF, OUTPUT);      //DEFINE os pinos dos motores como saidas
  pinMode(MdirT, OUTPUT);
  pinMode(MesqF, OUTPUT);
  pinMode(MesqT, OUTPUT);

  estado = 0;                  //Estado inicial, parado.
}

void loop() {

  // Se o estado for igual a 0, ele ira para a função parar(), seguida da função start().
  if (estado == 0) {
    parar();
    start();
  }






  do { //Executara tudo dentro deste paretese, enquanto estado for igual a 3.

    //Esta função IF aparecera muitas vezes ao longo do codigo,
    // ela quem assegurara que apos o robo estar ligado,
    // quando o botao ser pressionado novamente, ele ira parar e esperar ser reiniciado.

    if (digitalRead(botao) == HIGH) {
      estado = 0;
      parar();
    }
    else {
      //Le os valores dos sensores de linha,
      //Dependendo dos resultados(se forem positivos ou nao), eles executam uma ação
      //para que a leitura seja negativa, ou seja, esteja fora da linha branca
      if ( analogRead(Slin2) < vslin2) {
        resq();
      }
      if (analogRead(Slin1) < vslin1 ) {
        rdir();
      }
      if (analogRead(Slin3) < vslin3) {
        frente();
      }
      if (analogRead(Slin4) < vslin4) {
        frente();
      }

      //Se nenhum sensor de linha esta ativado, ele passa a ler os sensores de obstaculo.

      if (analogRead(Slin1) > vslin1 && analogRead(Slin2) > vslin2 && analogRead(Slin3) > vslin3 && analogRead(Slin4) > vslin4) {
        frente();
        //O codigo da prioridade ao sensor de obstaculo frontal, logo, esta a um IF superior aos demais.
        if (digitalRead(SobsF) == LOW) {
          frente();
        }
        else {
          //Dependendo de qual sensor for ativado, ele executa uma função diferente.
          if (digitalRead(SobsE) == LOW) {
            inimigoesquerda();
          }
          if (digitalRead(SobsD) == LOW) {
            inimigodireita();
          }
          if (digitalRead(SobsT) == LOW) {
            inimigoatras();
          }
        }
      }
    }

  } while (estado == 3);
  return (0);
}




void start() {

  //Parte do codigo para o deligamento do robo, se estado for 3(ligado) e o botao ser pressionado, estado passa a ser 0(desligado).

  if (digitalRead(botao) == HIGH  && estado == 3 ) {
    estado = 0;
  }

  do {
    //O botao precisa ser mantido pressionado por pelo menos 0.5s para iniciar o codigo de movimentação.
    Tinicio = millis();
    do {
      if ((millis() - Tinicio) > 500 ) {
        estado = 1;
      }
      else {
        delay(100);
      }
    } while (digitalRead(botao) == HIGH );
  } while (estado == 0);


  do {  //Apos pressionado por 0.5s, quando solto iniciara a contagem de 5s para o inicio do movimento.
    if (digitalRead(botao) == LOW) {
      estado = 2;
    }
  } while (estado == 1);

  do {          // inicio do funcionamento apos soltar o botao
    delay(5000);
    estado = 3;                                             //define a variavel estado = 3, ligado.

    tras();         // apos os 5s, dara um impulso com RE+FEENTE para cair a rampa.

    delay(100);

    frente();

  } while (estado == 2);
}




void parar() {

  digitalWrite(MdirT, LOW);
  digitalWrite(MdirF, LOW);

  digitalWrite(MesqT, LOW);
  digitalWrite(MesqF, LOW);


}

void frente() {

  if (digitalRead(botao) == HIGH) {
    estado = 0;
    parar();
    return (0);
  }

  digitalWrite(MdirT, HIGH);
  digitalWrite(MdirF, LOW);

  digitalWrite(MesqT, HIGH);
  digitalWrite(MesqF, LOW);


}







void tras() {

  if (digitalRead(botao) == HIGH) {
    estado = 0;

    return (0);
  }

  digitalWrite(MdirT, LOW);
  digitalWrite(MdirF, HIGH);

  digitalWrite(MesqT, LOW);
  digitalWrite(MesqF, HIGH);

}
// Todas as funções a seguir VERIFICAM todos os sensores de linha antes de qualquer movimento.

void resq() {

  Tinicio = millis();
  do {   // Inicia um movimento de RÉ por um determinado tempo

    if (analogRead(Slin3) > vslin3 && analogRead(Slin4) > vslin4) {
      if (digitalRead(botao) == HIGH) {
        estado = 0;
        return (0);
      }
      digitalWrite(MdirT, LOW);
      digitalWrite(MdirF, HIGH);

      digitalWrite(MesqT, LOW);
      digitalWrite(MesqF, HIGH);

    }
    else {
      return (0);
    }
  } while ((millis() - tre) < Tinicio);

  Tinicio2 = millis();
  //Apos o movimento de RÉ, gira para a esquerda por um determinado tempo.
  do {
    if (analogRead(Slin3) > vslin3 && analogRead(Slin4) > vslin4) {
      if (digitalRead(botao) == HIGH) {
        estado = 0;

        return (0);
      }
      digitalWrite(MdirT, LOW);
      digitalWrite(MdirF, HIGH);

      digitalWrite(MesqT, HIGH);
      digitalWrite(MesqF, LOW);

    }
    else {
      return (0);
    }
  } while ((millis() - tgirototal) < Tinicio2);
  return (0);

}








void rdir() {

  Tinicio = millis();

  do {     // Inicia um movimento de RÉ por um determinado tempo
    if (analogRead(Slin3) > vslin3 && analogRead(Slin4) > vslin4) {
      if (digitalRead(botao) == HIGH) {
        estado = 0;

        return (0);
      }
      digitalWrite(MdirT, LOW);
      digitalWrite(MdirF, HIGH);

      digitalWrite(MesqT, LOW);
      digitalWrite(MesqF, HIGH);

    }
    else {
      return (0);
    }
  } while ((millis() - tre) < Tinicio);

  Tinicio2 = millis();
  //Apos o movimento de RÉ, gira para a direita por um determinado tempo.
  do {
    if (analogRead(Slin3) > vslin3 && analogRead(Slin4) > vslin4) {
      if (digitalRead(botao) == HIGH) {
        estado = 0;

        return (0);
      }
      digitalWrite(MdirT, HIGH);
      digitalWrite(MdirF, LOW);

      digitalWrite(MesqT, LOW);
      digitalWrite(MesqF, HIGH);
    }
    else {
      return (0);
    }
  } while ((millis() - tgirototal) < Tinicio2);

  return (0);

}

void inimigoesquerda() {

  Tinicio = millis();
  do {//Gira para a esquerda por um determinado tempo.
    if (analogRead(Slin3) > vslin3 && analogRead(Slin4) > vslin4 && digitalRead(SobsF) == HIGH) {
      if (digitalRead(botao) == HIGH) {
        estado = 0;
        return (0);
      }
      digitalWrite(MdirT, HIGH);
      digitalWrite(MdirF, LOW);

      digitalWrite(MesqT, LOW);
      digitalWrite(MesqF, HIGH);

    }
    else {
      return (0);
    }
  } while ((millis() - trava ) < Tinicio);

  if (digitalRead(SobsE) == LOW) {
    //Se o tempo determinado anteriormente for atingido, e mesmo assim continuar com o sensor de obstaculo ativo, inicia um movimento de RÉ, seguido de um giro para esquerda novamente.

    Tinicio2 = millis();
    do {
      if (analogRead(Slin3) > vslin3 && analogRead(Slin4) > vslin4
          && digitalRead(SobsF) == HIGH) {
        if (digitalRead(botao) == HIGH) {
          estado = 0;

          return (0);
        }
        digitalWrite(MdirT, LOW);
        digitalWrite(MdirF, HIGH);

        digitalWrite(MesqT, LOW);
        digitalWrite(MesqF, HIGH);

      }
      else {
        return (0);
      }
    } while ((millis() - tre) < Tinicio2);

    Tinicio3 = millis();
    do {
      if (analogRead(Slin3) > vslin3 && analogRead(Slin4) > vslin4 && digitalRead(SobsF) == HIGH) {
        if (digitalRead(botao) == HIGH) {
          estado = 0;

          return (0);
        }
        digitalWrite(MdirT, HIGH);
        digitalWrite(MdirF, LOW);

        digitalWrite(MesqT, LOW);
        digitalWrite(MesqF, HIGH);

      }
      else {
        return (0);
      }
    } while ((millis() - tgiro) < Tinicio2);
    return (0);


  }
  return (0);
}

void inimigodireita() {

  Tinicio = millis();
  do {   //Gira para a direita por um determinado tempo.
    if (analogRead(Slin3) > vslin3 && analogRead(Slin4) > vslin4 && digitalRead(SobsF) == HIGH) {
      if (digitalRead(botao) == HIGH) {
        estado = 0;
        return (0);
      }
      digitalWrite(MdirT, LOW);
      digitalWrite(MdirF, HIGH);

      digitalWrite(MesqT, HIGH);
      digitalWrite(MesqF, LOW);

    }
    else {
      return (0);
    }
  } while ((millis() - trava ) < Tinicio);

  if (digitalRead(SobsE) == LOW) {
    //Se o tempo determinado anteriormente for atingido, e mesmo assim continuar com o sensor de obstaculo ativo, inicia um movimento de RÉ, seguido de um giro para direita novamente.

    Tinicio2 = millis();
    do {
      if (analogRead(Slin3) > vslin3 && analogRead(Slin4) > vslin4 && digitalRead(SobsF) == HIGH) {
        if (digitalRead(botao) == HIGH) {
          estado = 0;

          return (0);
        }
        digitalWrite(MdirT, LOW);
        digitalWrite(MdirF, HIGH);

        digitalWrite(MesqT, LOW);
        digitalWrite(MesqF, HIGH);

      }
      else {
        return (0);
      }
    } while ((millis() - tre) < Tinicio2);

    Tinicio3 = millis();
    do {
      if (analogRead(Slin3) > vslin3 && analogRead(Slin4) > vslin4
          && digitalRead(SobsF) == HIGH) {
        if (digitalRead(botao) == HIGH) {
          estado = 0;

          return (0);
        }
        digitalWrite(MdirT, LOW);
        digitalWrite(MdirF, HIGH);

        digitalWrite(MesqT, HIGH);
        digitalWrite(MesqF, LOW);

      }
      else {
        return (0);
      }
    } while ((millis() - tgiro) < Tinicio2);
    return (0);


  }
  return (0);
}

void inimigoatras() {
  //Inicia um movimento de giro para a esquerda, com objetivo de rotacionar o robo em 180graus.
  Tinicio = millis();
  do {
    if (analogRead(Slin3) > vslin3 && analogRead(Slin4) > vslin4 && digitalRead(SobsF) == HIGH) {
      if (digitalRead(botao) == HIGH) {
        estado = 0;

        return (0);
      }
      digitalWrite(MdirT, HIGH);
      digitalWrite(MdirF, LOW);

      digitalWrite(MesqT, LOW);
      digitalWrite(MesqF, HIGH);

    }
    else {
      return (0);
    }

  } while ((millis() - tgirototal) < Tinicio);
  return (0);
}
