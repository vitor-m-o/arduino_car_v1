//=====VITOR-M-O=====//
//=====TINF1M=====//
//=====PROGRAMA-EM-ARDUINO-PARA-CONTROLAR-CARRO-ARDUINO-COM-BLUETOOTH-E-MOSTRAR-INFORMACOES-NA-TELA-OLED=====//

//=====CHAMAR-BIBLIOTECA-UTILIZADA-PARA-USAR-ULTRASSONICO=====//
#include <Ultrasonic.h>

//=====CHAMAR-BIBLIOTECA-UTILIZADA-PARA-USAR-TELA-OLED=====//
#include "U8glib.h"

//=====DEFINIR-NOME-PARA-CLASSE-DE-FUNCOES-DA-TELA-OLED=====//
U8GLIB_SSD1306_128X64 oled(U8G_I2C_OPT_NO_ACK);

//=====DEFINIR-VARIAVEL-UTILIZADA-PARA-ESCREVER-NA-TELA-OLED=====//
char stringVar[10]; 

//=====DEFINIR-VARIAVEL-DE-CONTROLE-PARA-VALOR-LIDO-POR-BLUETOOTH=====//
char var = '\0';

//=====DEFINIR-VARIAVEL-UTILIZADA-PARA-DETERMINAR-SE-A-TELA-OLED-RESETARA=====//
bool displayReboot;

//=====CHAMAR-BIBLIOTECA-PARA-USAR-BLUETOOTH=====//
#include <SoftwareSerial.h>

//=====DEFINIR-NOME-PARA-CLASSE-DE-FUNCOES-DO-BLUETOOTH=====//
SoftwareSerial hc06(12,13);

//=====DEFINIR-VARIAVEL-PARA-LER-VALOR-RECEBIDO-PELO-BLUETOOTH=====//
char readBlue;

//=====DEFINIR-PINOS-UTILIZADOS-PARA-ULTRASSONICO=====//
#define pino_trigger1 2
#define pino_echo1 3

#define pino_trigger2 2
#define pino_echo2 7

#define pino_trigger3 2
#define pino_echo3 10

//=====DEFINIR-NOME-PARA-CLASSE-DE-FUNCOES-DO-ULTRASSONICO=====//
Ultrasonic ultrassonic1(pino_trigger1, pino_echo1);  
Ultrasonic ultrassonic2(pino_trigger2, pino_echo2);  
Ultrasonic ultrassonic3(pino_trigger3, pino_echo3);  

//=====DEFINIR-VARIAVEL-PARA-LER-DISTANCIA-DO-ULTRASSONICO=====//
unsigned int distance1;
unsigned int distance2;
unsigned int distance3;

//=====PORTAS-PARA-PONTE-H=====//
  #define trasEsq 4    //N1
  #define frenteEsq 5 //N2
  #define trasDir 11   //N3
  #define frentedir 6  //N4

//=====PORTAS-SENSORES-DE-REFLEXAO-BINARIOS=====//
  #define sen_1_pin A1
  #define sen_2_pin A2
  #define sen_3_pin A6
  #define sen_4_pin A7
  
  int sen_1;
  int sen_2;
  int sen_3;
  int sen_4;

  int ult_1;
  int ult_2;
  int ult_3;

//=====VARIAVEL-PARA-CONTROLAR-VELOCIDADE=====//
  int VEL = 150;
//=====VARIAVEL-PARA-CONTROLAR-VELOCIDADE-EM-CURVAS=====//
  int curvaVel = 150;
//=====VELOCIDADE-PARA-CONTROLVAR-VELOCIDADE-DA-RODA-DE-SUPORTE=====//
  int suporteVel = 150;

  int velocidade = 0;
  
  int velocidadeNeg = 0;

//=====VARIAVEL-PARA-CONTROLAR-MOVIMENTO-DO-CARRO=====//
  int carState;

  int buttonState;
  int buttonAction;
  #define button A0

//=====VARIAVEIS-UTILIZADAS-PARA-CONTROLAR-O-CURSOR-DA-TELA=====//
  int y = 30;
  int x = 2;

//=====VARIAVEIS-UTILIZADAS-PARA-MEDIR-O-TEMPO=====//
  int tempoAtual;
  int tempoAnterior;
  int tempoAnterior1;
  int tempoAnterior2;

//=====PROTOCOLO-INICIAL=====//
void setup() {
  //=====INICIALIZAR-O-MILLIS-CONTADOR-DE-MILISEGUNDOS=====//
  millis();
  
  //=====INICIALIZAR-SERIAL=====//
  Serial.begin(9600);
  
  //=====INICIARLIZAR-COMUNICACAO-SERIAL=====//
  hc06.begin(9600);

  //=====COMUNICAR-NO-SERIAL-MONITOR-FUNCIONAMENTO=====//
  Serial.println("Pronto para funcionamento!");

  //=====DEFINIR-PORTAS-E-SUAS-FUNCOES-PONTE-H=====//
  pinMode(trasEsq, OUTPUT);
  pinMode(frenteEsq, OUTPUT);
  pinMode(trasDir, OUTPUT);
  pinMode(frentedir, OUTPUT);
  
  //=====DEFINIR-PORTAS-E-SUAS-FUNCOES-SENSORES-REFLEXAO=====//
  pinMode(sen_1_pin, INPUT);
  pinMode(sen_2_pin, INPUT);
  pinMode(sen_3_pin, INPUT);
  pinMode(sen_4_pin, INPUT);

  //=====DESENHAR-TELA-INICIAL-COM-LOGO-DO-IFSUL=====//

  //=====PARTE-INICIALIZADORA-PARA-DESENHAR-NA-TELA=====//
  oled.firstPage();
  do {
  //=====PARTE-INICIALIZADORA-PARA-DESENHAR-NA-TELA=====//

    //=====DEFINIR-FONTE-UTILIZADA=====//
    oled.setFont(u8g_font_10x20);
    
    //=====DEFINIR-AS-COORDENADAS-E-O-TEXTO-PARA-MOSTRAR-NA-TELA-OLED=====//
    oled.drawStr(15, 35, "IF");

    oled.setFont(u8g_font_5x7);
    
    oled.drawStr(40, 25, "Instituto Federal");

    oled.drawStr(40, 35, "SUL-RIO-GRANDENSE");
    
    oled.drawStr(40, 45, "Campus Camaqua");
    
  //=====PARTE-FINALIZADORA-PARA-DESENHAR-NA-TELA=====//
  } while (oled.nextPage());
  //=====PARTE-FINALIZADORA-PARA-DESENHAR-NA-TELA=====//

  //=====ESPERAR-3-SEGUNDOS=====//
  delay(3000);

  //=====DESENHAR-UMA-TELA-COM-A-INSTRUÇÃO-DA-UTILIZAÇÃO-DOS-BOTOÕES-DO-CARRO-ATÉ-QUE-O-USUÁRIO-APERTE-ALGUM-BOTAO=====//
  while (buttonState < 400)
  {
    buttonState = analogRead(button);

    oled.firstPage();
    do {
      oled.setFont(u8g_font_5x7);

      oled.drawStr(25, 10, "BOTOES DO PAINEL");
      
      oled.drawStr(5, 25, "o");

      oled.drawStr(10, 25, "SELECIONAR");

      
      oled.drawStr(5, 45, "o");

      oled.drawStr(10, 45, "VOLTAR");


      oled.drawStr(90, 25, "o");

      oled.drawStr(95, 25, "CIMA");


      oled.drawStr(90, 45, "o");

      oled.drawStr(95, 45, "BAIXO");


      oled.drawStr(10, 60, "APERTE QUALQUER BOTAO");

    } while (oled.nextPage());

  }

}

void loop() 
{
  //=====UTILIZAR-FUNCAO-PARA-DESENHAR-NA-TELA-DA-TELA-OLED=====//
  draw();

  //=====LER-O-VALOR-DO-PINO-DO-BOTAO=====//
  buttonState = analogRead(button);

  //=====IMPRIMIR-NO-SERIAL-MONITOR-O-VALOR-RECEBIDO-DO-BOTAO-E-O-SEU-ESTADO=====//
  Serial.println(buttonState);
  Serial.println(buttonAction);

  //=====VERIFICAÇÃO-DA-TENSAO-DO-PINO-PARA-PODER-DETERMINAR-QUAL-BOTAO-FOI-CLICADO=====//
  if (buttonState < 600)
  {
    buttonAction = 0;
  }
  else if (buttonState < 800)
  {
    buttonAction = 1;
  }
  else if (buttonState < 900)
  {
    buttonAction = 2;
  }
  else if (buttonState < 1000)
  {
    buttonAction = 3;
  }
  else
  {
    buttonAction = 4;
  }

  //=====DETERMINAR-QUAL-FUNÇÃO-O-BOTÃO-REALIZARÁ-DE-ACORDO-COM-O-BOTAO-PRESSIONADO=====//

  //=====SE-O-BOTAO-2-FOR-PRESSIONADO=====//
  if (buttonAction == 2)
  {
    //=====ESPERAR-200-MILISEGUNDOS=====//
    delay(200);

    //=====SE-A-COORDENADA-DO-Y-FOR-30=====//
    if (y == 30)
    {
      //=====SE-A-COORDENADA-DO-X-FOR-2=====//
      if (x == 2)
      {
        //=====DEFINIR-NOVAS-CORDENADAS=====//
        y = 55;
        x = 60;
      }
      //=====SENÃO=====//
      else
      {
        y = 55;
        x = 2;
      }
    }
    else
    {
      y = y - 25;
    }
  }
  else if (buttonAction == 3)
  {
    delay(200);

    if (y == 55)
    {
      if (x == 2)
      {
        y = 30;
        x = 60;
      }
      else
      {
        y = 30;
        x = 2;
      }
    }
    else
    {
      y = y + 25;
    }
  }

}

void draw()
{
  if (buttonAction == 1)
  {
    if ((y == 30) && (x == 2))
    {
      delay(300);
      buttonState = 0;
      oled.firstPage();
      
      do {

        oled.setFont(u8g_font_5x8);
        
        oled.drawStr(30, 10, "MODO BLUETOOTH");
        
        oled.drawStr(0, 30, "CONECTE-SE PARA CONTROLAR");

        oled.drawStr(10, 50, "APERTE QUALQUER BOTAO");
        oled.drawStr(40, 60, "PARA SAIR");

      } while (oled.nextPage());
      while (buttonState < 600)
      {
        buttonState = analogRead(button);
        
          //=====VERIFICAR-SE-O-BLUETOOTH-ESTA-DISPONIVEL=====//
          if (hc06.available()) 
          {
            //=====LER-VALOR-RECEBIDO-PELO-BLUETOOTH=====//
            readBlue = hc06.read(); 
            //=====DESCARTAR-VALORES-NULOS-E-DESNECESSARIOS-COM-FUNCAO-DE-PULAR-LINHA=====//
            if (readBlue != '\r' && readBlue != '\n')
            {
              //=====IMPRIMIR-VALOR-RECEBIDO-PELO-BLUETOOTH=====//
              //Serial.print("Recebido: ");
              //Serial.println(readBlue);    
            
              //=====COMPARAR-SE-O-VALOR-E-IGUAL-AO-CARACTERE-F=====//
              if (readBlue == 'F')
              {
                //=====MOVER-O-CARRO-PARA-FRENTE=====//
                moverCarro(0, VEL, 0, VEL);
              } 
              //=====SENAO-COMPARAR-SE-O-VALOR-E-IGUAL-AO-CARACTERE-R=====//
              else if (readBlue == 'R')
              {
                //=====MOVER-O-CARRO-PARA-DIREITA=====//
                moverCarro(0, 0, 0, curvaVel);
              }
              //=====SENAO-COMPARAR-SE-O-VALOR-E-IGUAL-AO-CARACTERE-L=====//
              else if (readBlue == 'L')
              {
                //=====MOVER-O-CARRO-PARA-ESQUERDA=====//
                moverCarro(0, curvaVel, 0, 0);
              }
              //=====SENAO-COMPARAR-SE-O-VALOR-E-IGUAL-AO-CARACTERE-B=====//
              else if (readBlue == 'B')
              {
                //=====MOVER-O-CARRO-PARA-TRAS=====//
                moverCarro(140, 0, 140, 0);
              }
              //=====SENAO-COMPARAR-SE-O-VALOR-E-IGUAL-AO-CARACTERE-S=====//
              else if (readBlue == 'S')
              {
                //=====PARAR-O-CARRO=====//
                moverCarro(0, 0, 0, 0);
              }
              //=====SENAO-COMPARAR-SE-O-VALOR-E-IGUAL-AO-CARACTERE-Q=====//
              else if (readBlue == 'Q')
              {
                //=====MOVER-O-CARRO-PARA-FRENTE-E-ESQUERDA=====//
                moverCarro(0, VEL, 0, suporteVel);
              }
              //=====SENAO-COMPARAR-SE-O-VALOR-E-IGUAL-AO-CARACTERE-Z=====//
              else if (readBlue == 'Z')
              {
                //=====MOVER-O-CARRO-PARA-TRAS-E-ESQUERDA=====//
                moverCarro(VEL, 0, suporteVel, 0);
              }
              //=====SENAO-COMPARAR-SE-O-VALOR-E-IGUAL-AO-CARACTERE-E=====//
              else if (readBlue == 'E')
              {
                //=====MOVER-O-CARRO-PARA-FRENTE-E-DIREITA=====//
                moverCarro(0, suporteVel, 0, VEL);
              }
              //=====SENAO-COMPARAR-SE-O-VALOR-E-IGUAL-AO-CARACTERE-C=====//
              else if (readBlue == 'C')
              {
                //=====MOVER-O-CARRO-PARA-TRAS-E-DIREITA=====//
                moverCarro(suporteVel, 0, VEL, 0);
              }
              //=====SENAO-COMPARAR-SE-O-VALOR-E-IGUAL-AO-CARACTERE-Z=====//
              else if (readBlue == 'X')
              {
                //=====GIRAR-O-CARRO-PARA-ESQUERDA=====//
                moverCarro(0, VEL, 140, 0);
              }
              //=====SENAO-COMPARAR-SE-O-VALOR-E-IGUAL-AO-CARACTERE-Y=====//
              else if (readBlue == 'Y')
              {
                //=====GIRAR-O-CARRO-PARA-DIREITA=====//
                moverCarro(VEL, 0, 0, VEL);
              }
            }
          }
      }

      moverCarro(0, 0, 0, 0);
      delay(300);
    }
    else if ((y == 55) && (x == 2))
    {
      delay(300);
      buttonState = 0;

      oled.firstPage();
      do {

        oled.setFont(u8g_font_5x8);
        
        oled.drawStr(35, 10, "MODO AUTONOMO");
        
        oled.drawStr(30, 25, "CARRO AUTONOMO" );
        oled.drawStr(22, 35, "SEGUIDOR DE LINHA" );

        oled.drawStr(10, 50, "APERTE QUALQUER BOTAO");
        oled.drawStr(40, 60, "PARA SAIR");

      } while (oled.nextPage());

      while (buttonState < 600)
      {
        buttonState = analogRead(button);

        //=====LER-VALORES-DOS-SENSORES-DE-REFLEXAO=====//
        sen_1 = analogRead(sen_1_pin);
        sen_2 = analogRead(sen_2_pin);
        sen_3 = analogRead(sen_3_pin);
        sen_4 = analogRead(sen_4_pin);

        //distance1 = ultrassonic1.read();
        //Serial.println(distance1);

        //=====IDENTIFICAR-VALOR-DO-SENSOR=====//
        
        //=====SE-O-SENSOR-1-E-4-NAO-RECEBEREM-REFLEXAO-OU-O-SENSOR-1-E-3-OU-O-SENSOR-4-E-2=====//
        //=====DEFINIR-VARIAVEL-PARA-CONTROLAR-MOVIMENTO-DO-CARRO-COMO-0=====//
        if (sen_1 < 10 && sen_4 < 10) carState = 0;
        else if (sen_1 < 10 && sen_3 < 10) carState = 0;
        else if (sen_4 < 10 && sen_2 < 10) carState = 0;
        else if (sen_4 < 10 && sen_3 < 10) carState = 4;
        //else if (sen_1 < 10 && sen_2 < 10) carState = 1;
        //else if (sen_2 < 10 && sen_3 < 10) carState = 5;
        //=====SE-A-CONDICAO-ANTERIOR-NAO-FOR-VERDADEIRA-VERIFIQUE-SE-O-SENSOR-1-NAO=RECEBER-REFLEXAO=====//
        //=====DEFINIR-VARIAVEL-PARA-CONTROLAR-MOVIMENTO-DO-CARRO-COMO-1=====//
        else if (sen_1 < 10) carState = 1;
        //=====SE-AS-CONDICOES-ANTERIORES-NAO-FOREM-VERDADEIRAS-VERIFIQUE-SE-O-SENSOR-2-NAO=RECEBER-REFLEXAO=====//
        //=====DEFINIR-VARIAVEL-PARA-CONTROLAR-MOVIMENTO-DO-CARRO-COMO-2=====//
        else  if (sen_2 < 10) carState = 2;
        //=====SE-AS-CONDICOES-ANTERIORES-NAO-FOREM-VERDADEIRAS-VERIFIQUE-SE-O-SENSOR-3-NAO=RECEBER-REFLEXAO=====//
        //=====DEFINIR-VARIAVEL-PARA-CONTROLAR-MOVIMENTO-DO-CARRO-COMO-3=====//
        else if (sen_3 < 10) carState = 3;
        //=====SE-AS-CONDICOES-ANTERIORES-NAO-FOREM-VERDADEIRAS-VERIFIQUE-SE-O-SENSOR-4-NAO=RECEBER-REFLEXAO=====//
        //=====DEFINIR-VARIAVEL-PARA-CONTROLAR-MOVIMENTO-DO-CARRO-COMO-4=====//
        else if (sen_4 < 10) carState = 4;
        //=====SE-AS-CONDICOES-ANTERIORES-NAO-FOREM-VERDADEIRAS=====//
        else carState = 0;
        
        //=====VERIFICACAO-DE-CASOS-ESPECIFICOS-DA-VARIAVEL-QUE-DEFINE-O-MOVIMENTO-DO-CARRO=====//
        switch (carState) 
        {
          //=====SE-A-VARIAVEL-FOR-1=====//
          case 1: 
            //=====MOVER-O-MOTOR-ESQUERDO-PARA-TRAS=====//
            moverCarro(0, 0, VEL, 0);
            //=====SAIR-DESTA-CONDICAO=====//
            break;
          //=====SE-A-VARIAVEL-FOR-2=====//
          case 2:
            //=====MOVER-O-MOTOR-DIREITO-PARA-FRENTE=====//
            moverCarro(0, suporteVel, 0, 0);
            //=====SAIR-DESTA-CONDICAO=====//
            break;
          //=====SE-A-VARIAVEL-FOR-3=====//
          case 3:
            //=====MOVER-O-MOTOR-ESQUERDO-PARA-FRENTE=====//
            moverCarro(0, 0, 0, 150);
            //=====SAIR-DESTA-CONDICAO=====//
            break;
          //=====SE-A-VARIAVEL-FOR-4=====//
          case 4:
            //=====MOVER-O-MOTOR-DIREITO-PARA-TRAS=====//
            moverCarro(VEL, 0, 0, 0);
            //=====SAIR-DESTA-CONDICAO=====//
            break;
          //=====SE-A-VARIAVEL-FOR-0=====//
          case 0:
            //=====MOVER-AMBOS-OS-MOTORES-PARA-FRENTE=====//
            moverCarro(0, suporteVel, 0, suporteVel);
            //=====SAIR-DESTA-CONDICAO=====//
            break;
          case 5:
            moverCarro(suporteVel, 0, suporteVel, 0);
            break;
        }
      }

      moverCarro(0, 0, 0, 0);
      delay(300);
    }
    else if ((y == 30) && (x == 60))
    {

      delay(300);
      buttonState = 0;

      oled.firstPage();
      do {

        oled.setFont(u8g_font_5x8);
        
        oled.drawStr(30, 10, "MODO LABIRINTO");
        
        oled.drawStr(30, 25, "CARRO AUTONOMO" );
        oled.drawStr(18, 35, "PERCORRER LABIRINTO" );

        oled.drawStr(10, 50, "APERTE QUALQUER BOTAO");
        oled.drawStr(40, 60, "PARA SAIR");

      } while (oled.nextPage());

      tempoAtual = millis();
      tempoAnterior = tempoAtual;
      while (buttonState < 600)
      {
        buttonState = analogRead(button);

        //=====ult_1 -> ultrassonico esquerda=====//
        //=====ult_2 -> ultrassonico central=====//
        //=====ult_3 -> ultrassonico direita=====//

        tempoAtual = millis();

        if (tempoAtual - tempoAnterior1 > 50)
        {
          ult_1 = ultrassonic1.read();
          ult_2 = ultrassonic2.read();
          ult_3 = ultrassonic3.read();

          tempoAnterior1 = tempoAtual;
        }

        /*
        if (ult_2 > 10)
        {
        moverCarro(0, 120, 0, 120);
        }
        else
        {
          if (ult_1 > ult_3)
          {
            while (ult_2 < 20)
            {
              ult_2 = ultrassonic2.read();
              moverCarro(0,120,0,0);
            }
          }
          else if (ult_3 > ult_1)
          {
            while(ult_2 < 20)
            {
              ult_2 = ultrassonic2.read();
              moverCarro(0,0,0,120);
            }
          }
        }
        */

        int velocidade = 60 + ult_2 * 1.2;

        if (ult_2 > 5)
        {
          moverCarro(0,velocidade,0,velocidade);
          Serial.println("CENTRO!!!");
        }
        //   /*
        else
        {
          moverCarro(0,0,0,0);
          while (tempoAtual - tempoAnterior2 < 300)
          {
            tempoAtual = millis();
            if (ult_1 > ult_3)
            {
              tempoAtual = millis();
              tempoAnterior = tempoAtual;
              while(tempoAtual - tempoAnterior < 170)
              {
                tempoAtual = millis();
                moverCarro(0,130,130,0);
                Serial.println("ESQUERDA!!!");
              }
              moverCarro(0,0,0,0);
              tempoAnterior = tempoAtual;
              while(tempoAtual - tempoAnterior < 300)
              {
                tempoAtual = millis();
                ult_2 = ultrassonic2.read();
                Serial.println("LENDO!!!");
              }
              tempoAnterior = tempoAtual;
            }
            else
            {
              tempoAtual = millis();
              tempoAnterior = tempoAtual;
              while(tempoAtual - tempoAnterior < 170)
              {
                tempoAtual = millis();
                moverCarro(130,0,0,130);
                Serial.println("DIREITA!!!");
              }
              moverCarro(0,0,0,0);
              tempoAnterior = tempoAtual;
              while(tempoAtual - tempoAnterior < 300)
              {
                tempoAtual = millis();
                ult_2 = ultrassonic2.read();
                Serial.println("LENDO!!!");
              }
              tempoAnterior = tempoAtual;
            }

          }
          tempoAnterior2 = tempoAtual;
        }
          

        
      }
      moverCarro(0,0,0,0);

      delay(300);
    }
    else if ((y == 55) && (x == 60))
    {
      
      delay(300);
      buttonAction = 0;

      x = 25;
      y = 30;

      while (buttonAction != 4)
      {
        buttonState = analogRead(button);

        if (buttonState < 600)
        {
          buttonAction = 0;
        }
        else if (buttonState < 800)
        {
          buttonAction = 1;
        }
        else if (buttonState < 900)
        {
          buttonAction = 2;
        }
        else if (buttonState < 1000)
        {
          buttonAction = 3;
        }
        else
        {
          buttonAction = 4;
        }

        if (buttonAction == 2)
        {
          delay(200);

          if (y == 30)
          {
            y = 60;
          }
          else
          {
            y = y - 15;
          }
        }
        else if (buttonAction == 3)
        {
          delay(200);

          if (y == 60)
          {
            y = 30;
          }
          else
          {
            y = y + 15;
          }
        }

        if (buttonAction == 1)
        {
          if (y == 30)
          {
            delay(300);
            buttonState = 0;

            while (buttonState < 600)
            {
              buttonState = analogRead(button);

              sen_1 = analogRead(sen_1_pin);
              sen_2 = analogRead(sen_2_pin);
              sen_3 = analogRead(sen_3_pin);
              sen_4 = analogRead(sen_4_pin);

              oled.firstPage();
              do {

                oled.setFont(u8g_font_5x8);
                
                oled.drawStr(20, 10, "TESTANDO SENSORES");
                oled.drawStr(35, 20, "DE REFLEXAO" );

                if (sen_1 < 10)
                {
                  oled.drawStr(25, 35, "0" );
                }
                else
                {
                  oled.drawStr(25, 35, "1" );
                }

                if (sen_2 < 10)
                {
                  oled.drawStr(50, 35, "0" );
                }
                else
                {
                  oled.drawStr(50, 35, "1" );
                }

                if (sen_3 < 10)
                {
                  oled.drawStr(70, 35, "0" );
                }
                else
                {
                  oled.drawStr(70, 35, "1" );
                }

                if (sen_4 < 10)
                {
                  oled.drawStr(95, 35, "0" );
                }
                else
                {
                  oled.drawStr(95, 35, "1" );
                }

                oled.drawStr(10, 50, "APERTE QUALQUER BOTAO");
                oled.drawStr(40, 60, "PARA SAIR");

              } while (oled.nextPage());

            }

            buttonAction = 0;
            delay(300);
            
          }
          else if (y == 45)
          {
            delay(300);

            buttonState = 0;

            while (buttonState < 600)
            {
              buttonState = analogRead(button);

              ult_1 = ultrassonic1.read();
              ult_2 = ultrassonic2.read();
              ult_3 = ultrassonic3.read();

              Serial.println(ult_1);
              Serial.println(ult_2);
              Serial.println(ult_3);

              char ult1s[10]; 
              sprintf(ult1s, "%d", ult_1); 

              char ult2s[10]; 
              sprintf(ult2s, "%d", ult_2); 

              char ult3s[10]; 
              sprintf(ult3s, "%d", ult_3); 

              oled.firstPage();
              do {

                oled.setFont(u8g_font_5x8);
                
                oled.drawStr(20, 10, "TESTANDO SENSORES");
                oled.drawStr(35, 20, "ULTRASSONICOS" );

                oled.drawStr(40, 35, ult1s );
                oled.drawStr(60, 35, ult2s );
                oled.drawStr(80, 35, ult3s );
                
                oled.drawStr(10, 50, "APERTE QUALQUER BOTAO");
                oled.drawStr(40, 60, "PARA SAIR");

              } while (oled.nextPage());

            }

            buttonAction = 0;
            delay(300);
          }

          else
          {
            buttonAction = 0;
            buttonState = 0;
            velocidade = 0;

            delay(300);

            while (buttonState < 400)
            {

              buttonState = analogRead(button);

              oled.firstPage();
              do {
                oled.setFont(u8g_font_5x7);

                oled.drawStr(25, 10, "BOTOES DO PAINEL");
                
                oled.drawStr(5, 25, "o");

                oled.drawStr(10, 25, "ZERAR");

                
                oled.drawStr(5, 45, "o");

                oled.drawStr(10, 45, "VOLTAR");


                oled.drawStr(75, 25, "o");

                oled.drawStr(80, 25, "AUMENTAR");


                oled.drawStr(75, 45, "o");

                oled.drawStr(80, 45, "DIMINUIR");


                oled.drawStr(10, 60, "APERTE QUALQUER BOTAO");

              } while (oled.nextPage());

            }

            delay(500);

            while (buttonAction != 4)
            {

              buttonState = analogRead(button);

              if (buttonState < 600)
              {
                buttonAction = 0;
              }
              else if (buttonState < 830)
              {
                buttonAction = 1;

                velocidade = 0;
              }
              else if (buttonState < 900)
              {
                buttonAction = 2;

                if (velocidade < 250)
                {
                  velocidade = velocidade + 10;
                }
                                      

                delay(200);
              }
              else if (buttonState < 1000)
              {
                buttonAction = 3;

                if (velocidade > -250)
                {
                  velocidade = velocidade - 10;
                }
                delay(200);
              }
              else
              {
                buttonAction = 4;
              }

              oled.firstPage();
              do {

                oled.setFont(u8g_font_5x8);
                
                oled.drawStr(25, 10, "TESTANDO MOTORES");

                char velocidadeStr[10]; 
                sprintf(velocidadeStr, "%d", velocidade); 
                oled.drawStr(25, 20, "VELOCIDADE :" );
                oled.drawStr(95, 20,  velocidadeStr );

                if (velocidade > 0)
                {
                  oled.drawStr(60, 30, "^");
                  oled.drawStr(60, 35, "|");

                  moverCarro(0,velocidade,0,velocidade);
                  
                }
                else if (velocidade < 0)
                {
                  oled.drawStr(60, 30, "|");
                  oled.drawStr(60, 35, "v");

                  velocidadeNeg = velocidade * -1;

                  moverCarro(velocidadeNeg, 0, velocidadeNeg, 0);
                }
                else
                {
                  oled.drawStr(60, 35, "o");

                  moverCarro(0,0,0,0);
                }
                
                oled.drawStr(10, 50, "APERTE QUALQUER BOTAO");
                oled.drawStr(40, 60, "PARA SAIR");

              } while (oled.nextPage());

            }

            buttonAction = 0;
            moverCarro(0,0,0,0);
            delay(500);
          }
        }
        else
        {
          oled.firstPage();
          do 
          {

            oled.setFont(u8g_font_5x8);
            
            oled.drawStr(30, 10, "MENU DE TESTES");

            oled.setFont(u8g_font_5x7);

            oled.drawStr(45, 30, "REFLEXAO");
            
            oled.drawStr(35, 45, "ULTRASSONICOS");

            oled.drawStr(48, 60, "MOTORES");

            oled.drawStr(x, y, ">");
            
          } while (oled.nextPage());
        }
        
      }
      x = 60;
      y = 55;

    }

  }
  else
  {
    oled.firstPage();
    do {

      oled.setFont(u8g_font_5x8);
      
      oled.drawStr(50, 10, "MENU");

      oled.setFont(u8g_font_5x7);

      oled.drawStr(10, 30, "BLUETOOTH");
      
      oled.drawStr(12, 55, "AUTONOMO");

      oled.drawStr(70, 30, "LABIRINTO");

      oled.drawStr(75, 55, "TESTES");

      oled.drawStr(x, y, ">");

      
      
    } while (oled.nextPage());
  }
}

//=====FUNCAO-UTILIZADA-PARA-MOVER-OS-MOTORES=====//
void moverCarro(int valor1, int valor2, int valor3, int valor4)
{
  //=====FUNCAO-PARA-ENVIAR-ENERGIA-PARA-A-PORTA-N1-E-MOVER-MOTOR-DIREITO-PARA-TRAS=====//
  analogWrite(trasDir, valor1);
  //=====FUNCAO-PARA-ENVIAR-ENERGIA-PARA-A-PORTA-N2-E-MOVER-MOTOR-DIREITO-PARA-FRENTE=====//
  analogWrite(frentedir, valor2);
  //=====FUNCAO-PARA-ENVIAR-ENERGIA-PARA-A-PORTA-N3-E-MOVER-MOTOR-ESQUERDO-PARA-TRAS=====//
  analogWrite(trasEsq, valor3);
  //=====FUNCAO-PARA-ENVIAR-ENERGIA-PARA-A-PORTA-N4-E-MOVER-MOTOR-ESQUERDO-PARA-FRENTE=====//
  analogWrite(frenteEsq, valor4);

}
