

//adiciona a biblioteca de controle de motores ao codigo
#include <RoboCore_Vespa.h>
#include <NewPing.h>

//declaracao do objeto "motores"
VespaMotors motores;

//declaracao dos pinos conectados aos sensores de linha
const int SENSOR_LINHA_ESQUERDO = 36;
const int SENSOR_LINHA_DIREITO = 39;

//declaracao das variaveis que armazenam as leituras dos sensores
int leitura_esquerdo = 0;
int leitura_direito = 0;

//declaracao da variavel que armazena o valor de corte para as leituras dos sensores
const int LEITURA_LINHA = 3000;
//declaracao da variavel que armazena a velocidade em linha reta do robo
const int VELOCIDADE = 80;
//declaracao da variavel que armazena o valor que sera somado a velocidade de rotacao dos motores
const int VELOCIDADE_SOMA = 20;
//declaracao da variavel que armazena o valor que sera subtraido da valocidade de rotacao dos motores
const int VELOCIDADE_SUBTRACAO = 50;
//declaracao da variavel que armazena o valor maximo de contagem de parada
const int CONTAGEM_MAXIMA = 100;
//declaracao da variavel do contador para parar o robo caso ele fuja da pista
int contador_parada = 0;

//declaracao dos pinos do sensor
const int PINO_TRIGGER = 25;
const int PINO_ECHO = 26;
//declaracao da variavel que armazena a distancia do obstaculo
const int DISTANCIA_OBSTACULO = 7;
//declaracao da variavel que armazena a distancia maxima de leitura do sensor (300 cm = 3 m)
const int DISTANCIA_MAXIMA = 300;
//declaracao da variavel que armazena o tempo de espera entre leituras do sensor
const int ESPERA = 30;
//declaracao da variavel que armazena o tempo que os movimentos de desvio irao durar
const int ESPERA_MOVIMENTO = 400;
//declaracao da variavel que armazena a distancia medida pelo sensor
int distancia;


NewPing sensor_ultrassonico(PINO_TRIGGER, PINO_ECHO, DISTANCIA_MAXIMA);

void setup() {

  //configuracao dos pinos conectados aos sensores como entrada
  pinMode(SENSOR_LINHA_ESQUERDO, INPUT);
  pinMode(SENSOR_LINHA_DIREITO, INPUT);

}

void loop() {

  //executa a funcao de controle de seguidor de linha
  //seguidor_linha();
  //executa a funcao de controle de anticolisao
  anticolisao();

}

void seguidor_linha() {

  //realiza a leitura dos sensores
  leitura_esquerdo = analogRead(SENSOR_LINHA_ESQUERDO);
  leitura_direito = analogRead(SENSOR_LINHA_DIREITO);

  //verifica se os dois sensores estao sobre a parte branca da pista
  if ((leitura_esquerdo > LEITURA_LINHA) && (leitura_direito > LEITURA_LINHA)) { //se for verdadeiro
    //movimenta o robo para frente
    motores.forward(VELOCIDADE);
    contador_parada = 0; //zera o contador de parada
  }

  //verifica se ambas as leituras dos sensores sao menores que o valor de leitura de corte
  //ou seja se os dois sensores estao fora da linha da pista
  else if ((leitura_esquerdo < LEITURA_LINHA) && (leitura_direito < LEITURA_LINHA)) { //se for verdadeiro
    contador_parada++; //incrementa o contador de parada
  }

  //verifica se o sensor da direta esta sobre a parte branca da pista
  else if (leitura_direito > LEITURA_LINHA) { //se for verdadeiro
    //gira o robo para a esquerda ajustando a velocidade dos motores
    motores.turn(VELOCIDADE + VELOCIDADE_SOMA, VELOCIDADE - VELOCIDADE_SUBTRACAO);
    contador_parada = 0; //zera o contador de parada
  }


  //verifica se apenas o sensor da esquerda esta sobre a parte branca da pista
  else if (leitura_esquerdo > LEITURA_LINHA) {
    //gira o robo para a direita ajustando a velocidade dos motores
    motores.turn(VELOCIDADE - VELOCIDADE_SUBTRACAO, VELOCIDADE + VELOCIDADE_SOMA);
    contador_parada = 0; //zera o contador de parada
  }

  
  //DESATIVA CONTAGEM MÁXIMA
  //if (contador_parada >= CONTAGEM_MAXIMA) { //se for verdadeiro
  //  motores.stop(); //para o robo
  //}

}

void anticolisao() {
  delay(ESPERA); //aguarda o tempo de espera para leitura do sensor
  distancia = (sensor_ultrassonico.ping()) / 58;

  //verifica se a distancia lida pelo sensor e menor ou igual ao valor configurado na variavel "DISTANCIA_OBSTACULO"
  if (distancia <= DISTANCIA_OBSTACULO && distancia != 0) {
    motores.stop();
    
    delay(ESPERA); //aguarda o tempo de espera para leitura do sensor
    distancia = (sensor_ultrassonico.ping()) / 58; //atualiza a leitura do sensor

    //confirma se a distancia e menor ou igual ao valor configurado na variavel "DISTANCIA_OBSTACULO"
    if (distancia <= DISTANCIA_OBSTACULO && distancia != 0) {
      motores.stop();
    }
  } else {
    seguidor_linha();
  }
}

