
#include <Wire.h>
#include <LiquidCrystal.h>
#include <Adafruit_MotorShield.h>
  
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

// Define valores usados pelos botões
int lcd_key     = 0;
int adc_key_in  = 0;
#define btnRIGHT  5
#define btnUP     4
#define btnDOWN   3
#define btnLEFT   2
#define btnSELECT 1
#define btnNONE   0

// Define as variaveis para multitarefa
int redColor = 0;
int greenColor = 0;
int blueColor = 0;
int yellowColor = 0;

// Definição de pinos usados para os leds e ldr
#define ldr A15
#define led_r 23
#define led_g 31
#define led_b 27

// Parêmetros do sensor
#define toleraciaDeteccao 10
#define mediasSensor 10

//Função para ler os botões
int read_LCD_buttons()
{
	adc_key_in = analogRead(0);      // read the value from the sensor 
	// my buttons when read are centered at these valies: 0, 144, 329, 504, 741
	// we add approx 50 to those values and check to see if we are close
	if (adc_key_in > 1000) return btnNONE; // We make this the 1st option for speed reasons since it will be the most likely result
	// For V1.1 us this threshold
	if (adc_key_in < 50)   return btnRIGHT;  
	if (adc_key_in < 250)  return btnUP; 
	if (adc_key_in < 450)  return btnDOWN; 
	if (adc_key_in < 650)  return btnLEFT; 
	if (adc_key_in < 850)  return btnSELECT;  

 	return btnNONE;  // when all others fail, return this...
}

//Criando o objeto motor shield com o endereco default I2C
Adafruit_MotorShield AFMS = Adafruit_MotorShield();

//Selecionando a porta de cada motor
Adafruit_DCMotor *myMotor = AFMS.getMotor(3);
Adafruit_DCMotor *myMotor2 = AFMS.getMotor(4);

//Intervalo de parada do Multitarefa
const long intervalo_parada = 10000;
//Intervalo de mudança do Display
const long intervalo_display = 400;
//Variáveis para armazenar tempo
unsigned long tempo_anterior = 0, tempo_atual = 0;

// Variaveis Leds e LDR
int leitura_ldr[3];             // Variavel para armazenar a leitura atual do sensor
int color_cal[3], black_cal[3]; // Variáveis para armazenar o valor máximo e mínimo de leitura do sensor
int ambiente_cal, objeto_cal;   // Variáveis para guardar a intensidade luminosa do ambiente e do sensor tampado

void setup(){

	//Display com 16 colunas e 2 linhas
	lcd.begin(16, 2);

	//Inicia o objeto de motor com a frequencia default = 1.6KHz
	AFMS.begin();

  // Define os pinos do LED RGB como saída
  pinMode(led_r, OUTPUT);
  pinMode(led_g, OUTPUT);
  pinMode(led_b, OUTPUT);
  
  // Certifica-se que o LED se inicie apagado
  digitalWrite(led_r, HIGH);
  digitalWrite(led_g, HIGH);
  digitalWrite(led_b, HIGH);

}
 

void loop() 
{

	int i;

//Tela Inicial
	lcd.clear();
	lcd.setCursor(0,0);
	lcd.print("Mandela TP2");
	lcd.setCursor(0,1);
	lcd.print("Iniciar (SELECT)");           
	while(lcd_key != 1){
		lcd_key = read_LCD_buttons();  //Lê os botões
	}
	delay(300);
	lcd_key = 0;

//Selecionar tarefas
	lcd.clear();
	lcd.setCursor(0,0);
	lcd.print("Escolha a tarefa");
	lcd.setCursor(0,1);
	lcd.print("1-Locomocao(LEFT) 2-Id Cor(DOWN) 3-Multitarefa(UP)");
	i = 0;
	tempo_anterior = 0;
	while((lcd_key != 2)&&(lcd_key != 3)&&(lcd_key != 4)){		
		lcd_key = read_LCD_buttons();
		tempo_atual = millis();
		if(tempo_atual - tempo_anterior >= intervalo_display){
			tempo_anterior = tempo_atual;
			lcd.scrollDisplayLeft();
			i++;
			lcd.setCursor(i,0);
			lcd.print("Escolha a tarefa");
		}
	}
	
	//Locomoção
	if(lcd_key == 2){
		delay(300);
		lcd_key = 0;
		lcd.clear();
		while(lcd_key != 1){
			lcd.setCursor(0,0);
			lcd.print("Caminhos (LEFT)");
			lcd.setCursor(0,1);
			lcd.print("Sair (SELECT)");
			lcd_key = read_LCD_buttons();
			if(lcd_key == 2){
				delay(300);				
				lcd_key = 0;
				lcd.clear();
				lcd.setCursor(0,0);
				lcd.print("Caminhos:");
				lcd.setCursor(0,1);
				lcd.print("1-Linha(LEFT) 2-Triangulo(DOWN) 3-Quadrado(UP)");
				i = 0;
				tempo_anterior = 0;
				while((lcd_key != 2)&&(lcd_key != 3)&&(lcd_key != 4)){
					lcd_key = read_LCD_buttons();
					tempo_atual = millis();
					if(tempo_atual - tempo_anterior >= intervalo_display){
						tempo_anterior = tempo_atual;
						lcd.scrollDisplayLeft();
						i++;
						lcd.setCursor(i,0);
						lcd.print("Caminhos:");
    				}
				}
				//Caminho Linha
				if(lcd_key == 2){
					delay(300);
					lcd_key = 0;
					lcd.clear();
					lcd.setCursor(0,0);
					lcd.print("Executando...");
					lcd.setCursor(0,1);
					lcd.print("Linha");
					//Frente 30cm
					myMotor->run(FORWARD);
					myMotor->setSpeed(120);
					myMotor2->run(FORWARD);
					myMotor2->setSpeed(142);
					delay(4018);
					myMotor->run(RELEASE);
					myMotor2->run(RELEASE);
					//Pausa
					delay(500);
					//Trás 30cm
					myMotor->run(BACKWARD);
					myMotor->setSpeed(126);
					myMotor2->run(BACKWARD);
					myMotor2->setSpeed(142);
					delay(4018);
					myMotor->run(RELEASE);
					myMotor2->run(RELEASE);
					lcd.clear();
				}
				//Caminho Triângulo
				if(lcd_key == 3){
					delay(300);
					lcd_key = 0;
					lcd.clear();
					lcd.setCursor(0,0);
					lcd.print("Executando...");
					lcd.setCursor(0,1);
					lcd.print("Triangulo");
					//Frente 30cm
					myMotor->run(FORWARD);
					myMotor->setSpeed(120);
					myMotor2->run(FORWARD);
					myMotor2->setSpeed(142);
					delay(4018);
					myMotor->run(RELEASE);
					myMotor2->run(RELEASE);
					//Pausa
					delay(300);
					//Gira 90º para direita
					myMotor->run(FORWARD);
					myMotor->setSpeed(120);
					myMotor2->run(BACKWARD);
					myMotor2->setSpeed(142);
					delay(1480);
					myMotor->run(RELEASE);
					myMotor2->run(RELEASE);
					//Pausa
					delay(300);
					//Frente 30cm
					myMotor->run(FORWARD);
					myMotor->setSpeed(120);
					myMotor2->run(FORWARD);
					myMotor2->setSpeed(142);
					delay(4018);
					myMotor->run(RELEASE);
					myMotor2->run(RELEASE);
					//Pausa
					delay(300);
					//Gira 145º para direita
					myMotor->run(FORWARD);
					myMotor->setSpeed(120);
					myMotor2->run(BACKWARD);
					myMotor2->setSpeed(142);
					delay(2304);
					myMotor->run(RELEASE);
					myMotor2->run(RELEASE);
					//Pausa
					delay(300);
					//Frente 42,43cm
					myMotor->run(FORWARD);
					myMotor->setSpeed(120);
					myMotor2->run(FORWARD);
					myMotor2->setSpeed(142);
					delay(5683);
					myMotor->run(RELEASE);
					myMotor2->run(RELEASE);
          delay(300);
          //Gira 145º para direita
          myMotor->run(FORWARD);
          myMotor->setSpeed(120);
          myMotor2->run(BACKWARD);
          myMotor2->setSpeed(142);
          delay(2304);
          myMotor->run(RELEASE);
          myMotor2->run(RELEASE);
					lcd.clear();
				}
				//Caminho Quadrado
				if(lcd_key == 4){
					delay(300);
					lcd_key = 0;
					lcd.clear();
					lcd.setCursor(0,0);
					lcd.print("Executando...");
					lcd.setCursor(0,1);
					lcd.print("Quadrado");
					//Frente 30cm
					myMotor->run(FORWARD);
					myMotor->setSpeed(120);
					myMotor2->run(FORWARD);
					myMotor2->setSpeed(142);
					delay(4018);
					myMotor->run(RELEASE);
					myMotor2->run(RELEASE);
					//Pausa
					delay(300);
					//Gira 90º para direita
					myMotor->run(FORWARD);
					myMotor->setSpeed(120);
					myMotor2->run(BACKWARD);
					myMotor2->setSpeed(142);
					delay(1480);
					myMotor->run(RELEASE);
					myMotor2->run(RELEASE);
					//Pausa
					delay(300);
					//Frente 30cm
					myMotor->run(FORWARD);
					myMotor->setSpeed(120);
					myMotor2->run(FORWARD);
					myMotor2->setSpeed(142);
					delay(4018);
					myMotor->run(RELEASE);
					myMotor2->run(RELEASE);
					//Pausa
					delay(300);
					//Gira 90º para direita
					myMotor->run(FORWARD);
					myMotor->setSpeed(120);
					myMotor2->run(BACKWARD);
					myMotor2->setSpeed(142);
					delay(1480);
					myMotor->run(RELEASE);
					myMotor2->run(RELEASE);
					//Pausa
					delay(300);
					//Frente 30cm
					myMotor->run(FORWARD);
					myMotor->setSpeed(120);
					myMotor2->run(FORWARD);
					myMotor2->setSpeed(142);
					delay(4018);
					myMotor->run(RELEASE);
					myMotor2->run(RELEASE);
					//Pausa
					delay(300);
					//Gira 90º para direita
					myMotor->run(FORWARD);
					myMotor->setSpeed(120);
					myMotor2->run(BACKWARD);
					myMotor2->setSpeed(142);
					delay(1480);
					myMotor->run(RELEASE);
					myMotor2->run(RELEASE);
					//Pausa
					delay(300);
					//Frente 30cm
					myMotor->run(FORWARD);
					myMotor->setSpeed(120);
					myMotor2->run(FORWARD);
					myMotor2->setSpeed(142);
					delay(4018);
					myMotor->run(RELEASE);
					myMotor2->run(RELEASE);
          delay(300);
          //Gira 90º para direita
          myMotor->run(FORWARD);
          myMotor->setSpeed(120);
          myMotor2->run(BACKWARD);
          myMotor2->setSpeed(142);
          delay(1480);
          myMotor->run(RELEASE);
          myMotor2->run(RELEASE);
					lcd.clear();
				}
			}
		}
	}

	//Identificação de cor
	if(lcd_key == 3){
  
		delay(300);
		lcd_key = 0;
		
		//Para calibracao: LEFT
		while(lcd_key != 1){
      
      lcd_key = read_LCD_buttons();

      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Calibracao:");
      lcd.setCursor(0,1);
      lcd.print("1-Calibracao(LEFT) 2-Ident(DOWN) 3-Sair(SELECT)");
  
      int aux = 0;
      //Rolando o display para a esquerda 29 vezes
      for (int i = 0; i < 29; i++) {
        aux++;
        lcd.setCursor(aux,0);
        lcd.print("Calibracao:");
        lcd.scrollDisplayLeft();
        delay(500);
        lcd_key = read_LCD_buttons();
        if ((lcd_key == 1)||(lcd_key == 2)||(lcd_key == 3))
          break;
      }
      
			if(lcd_key == 2){
      
        lcd_key = 0;
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Calibracao");
        lcd.setCursor(0,1);
        lcd.print("Aguarde...");

        //aguarda 2 segundos para o início da calibração
        delay(2000);
        calibrar_sensor(); // Chama a rotina de calibração

			}
      else if(lcd_key == 3){
      
        lcd_key = 0;
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Identificar");
        lcd.setCursor(0,1);
        lcd.print("Aguarde...");

        //aguarda 2 segundos para o início da identificacao
        delay(2000);

        lcd.setCursor(0,1);
        lcd.print("Insira um obj");

        while(lcd_key != 1){
          
          while(analogRead(ldr) > objeto_cal){ // Aguarda a aproximação de um objeto
            lcd_key = read_LCD_buttons();
            if (lcd_key == 1)
              break;
          }
          
          if (lcd_key == 1)
            break;
          
          delay(500); // Delay para estabilização do objeto na área do sensor
          
          leSensor();   // Chama a rotina de leitura do sensor
          enviaCores(); // Envia as cores lidas para a porta serial

          while(analogRead(ldr) < ambiente_cal); // Aguarda a remoção do objeto
        }

      }
		}
	}

	//Multitarefa e tomada de decisão
	if(lcd_key == 4){

    int sair = 0;
		delay(300);
		lcd_key = 0;
		lcd.clear();
		lcd.setCursor(0,0);
		lcd.print("Executando...");

    tempo_anterior = millis();

		while(sair != 1){

      while(analogRead(ldr) > objeto_cal){ // Aguarda a aproximação de um objeto
        
        if (sair == 1){
          //Para o robô
          myMotor->run(RELEASE);
          myMotor2->run(RELEASE);
          break;
        }

        //Conta 10s
        tempo_atual = millis();
        if(tempo_atual - tempo_anterior >= intervalo_parada){
          sair = 1;
        }
        
  			//Frente até identificar bloco ou intervalo limite
  			myMotor->run(FORWARD);
  			myMotor->setSpeed(120);
  			myMotor2->run(FORWARD);
  			myMotor2->setSpeed(142);
      }

      if (sair == 1)
        break;

			//Saiu do loop - bloco à frente identificado
			//Para o robô
			myMotor->run(RELEASE);
      myMotor2->run(RELEASE);
      
      leSensor();   // Chama a rotina de leitura do sensor
      enviaCores(); // Envia as cores lidas para a porta serial
				
			if(blueColor == 1){ // Cor azul
				lcd_key = 0;
				//Move para trás
				myMotor->run(BACKWARD);
				myMotor->setSpeed(126);
				myMotor2->run(BACKWARD);
				myMotor2->setSpeed(142);
				delay(1300);
				myMotor->run(RELEASE);
				myMotor2->run(RELEASE);
				//Pausa
				delay(300);
				//Gira 90º para direita
				myMotor->run(FORWARD);
				myMotor->setSpeed(120);
				myMotor2->run(BACKWARD);
				myMotor2->setSpeed(142);
				delay(1480);
				myMotor->run(RELEASE);
				myMotor2->run(RELEASE);
				//Pausa
				delay(300);
				//Segue em frente
				myMotor->run(FORWARD);
				myMotor->setSpeed(120);
				myMotor2->run(FORWARD);
				myMotor2->setSpeed(142);
        tempo_anterior = millis();
			}
			else if(yellowColor == 1){ //Cor amarela
				//Move para trás
				myMotor->run(BACKWARD);
				myMotor->setSpeed(126);
				myMotor2->run(BACKWARD);
				myMotor2->setSpeed(142);
				delay(1300);
				myMotor->run(RELEASE);
				myMotor2->run(RELEASE);
				//Pausa
				delay(300);
				//Gira 90º para esquerda
				myMotor->run(BACKWARD);
				myMotor->setSpeed(120);
				myMotor2->run(FORWARD);
				myMotor2->setSpeed(142);
				delay(1480);
				myMotor->run(RELEASE);
				myMotor2->run(RELEASE);
				//Pausa
				delay(300);
				//Segue em frente
				myMotor->run(FORWARD);
				myMotor->setSpeed(120);
				myMotor2->run(FORWARD);
				myMotor2->setSpeed(142);
				lcd.setCursor(0,1);
        tempo_anterior = millis();
			}
			else if(redColor == 1){ // Cor vermelha
				//Move para trás
				myMotor->run(BACKWARD);
				myMotor->setSpeed(126);
				myMotor2->run(BACKWARD);
				myMotor2->setSpeed(142);
				delay(1300);
				myMotor->run(RELEASE);
				myMotor2->run(RELEASE);
				//Pausa
				delay(300);
				//Gira 180º pela direita
				myMotor->run(FORWARD);
				myMotor->setSpeed(120);
				myMotor2->run(BACKWARD);
				myMotor2->setSpeed(142);
				delay(2960);
				myMotor->run(RELEASE);
				myMotor2->run(RELEASE);
				//Pausa
				delay(300);
				//Segue em frente
				myMotor->run(FORWARD);
				myMotor->setSpeed(120);
				myMotor2->run(FORWARD);
				myMotor2->setSpeed(142);
				lcd.setCursor(0,1);
        tempo_anterior = millis();
			}
			else if(greenColor == 1){ // Cor verde
        //Move para trás
        myMotor->run(BACKWARD);
        myMotor->setSpeed(126);
        myMotor2->run(BACKWARD);
        myMotor2->setSpeed(142);
        delay(1300);
        myMotor->run(RELEASE);
        myMotor2->run(RELEASE);
        //Pausa
        delay(300);
				//Gira 360º pela direita
				myMotor->run(FORWARD);
				myMotor->setSpeed(120);
				myMotor2->run(BACKWARD);
				myMotor2->setSpeed(142);
				delay(5920);
				myMotor->run(RELEASE);
				myMotor2->run(RELEASE);
        tempo_anterior = millis();
			}
			//Caso não identifique o bloco, bloco preto por exemplo
			else{
				lcd.setCursor(0,1);
				lcd.print("Cor: Erro       ");					
			}
		}	
	}

	delay(300);
	lcd_key = 0;

}

// Rotina de calibração do sensor
void calibrar_sensor() {
  
  // Analiza a luminosidade do ambiente
  ambiente_cal = mediaLeituras(mediasSensor);
  ambiente_cal -= ambiente_cal/toleraciaDeteccao; // Tolerância para facilitar detecção

  delay(300);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Aproxime:");
  lcd.setCursor(0,1);
  lcd.print("Um obj qualquer");
  Serial.println("Aproxime um objeto qualquer ao sensor");
  delay(5000); // Aguarda 5 segundos para aproximação do objeto

  delay(300);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Calibracao:");
  lcd.setCursor(0,1);
  lcd.print("Calibrando...");
  Serial.println("Calibrando...");

  // Analiza a intesidade de luz com o objeto colocado em frente ao sensor
  objeto_cal = mediaLeituras(mediasSensor);
  objeto_cal += objeto_cal/toleraciaDeteccao; // Tolerância para facilitar detecção
  
  delay(300);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Calibracao:");
  lcd.setCursor(0,1);
  lcd.print("Remova o objeto");
  Serial.println("Remova o objeto");
  while(analogRead(ldr) < ambiente_cal); // Aguarda a remoção do objeto

  // Início de calibração para valor máximo de leitura do sensor
  color_cal[0] = 0;
  color_cal[1] = 0;
  color_cal[2] = 0;

  delay(300);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Aproxime:");
  lcd.setCursor(0,1);
  lcd.print("Um objeto branco");
  Serial.println("Aproxime um objeto branco");
  
  while(analogRead(ldr) > objeto_cal); // Aguarda a aproximação de um objeto
  delay(500); // Delay para estabilização do objeto na área do sensor

  delay(300);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Calibracao:");
  lcd.setCursor(0,1);
  lcd.print("Calibrando...");
  Serial.println("Calibrando...");

  // São realizadas 10 medidas para encontrar o valor máximo registrado pelo sensor
  for(int i = 0; i < 10; i++)
  {
    
    digitalWrite(led_r, LOW); // Acende o LED vermelho
    delay(100); // Aguarda 100ms para estabilizar o LDR

    // Realiza leitura do sensor e avalia se é o maior valor encontrado para o vermelho
    leitura_ldr[0] = mediaLeituras(mediasSensor);
    if (leitura_ldr[0] > color_cal[0]) {
      color_cal[0] = leitura_ldr[0];
    }
    
    digitalWrite(led_r, HIGH); // Apaga o LED vermelho
    delay(100); // Aguarda 100ms para estabilizar o LDR
    
    digitalWrite(led_g, LOW); // Acende o LED verde
    delay(100); // Aguarda 100ms para estabilizar o LDR
    
    // Realiza leitura do sensor e avalia se é o maior valor encontrado para o verde
    leitura_ldr[1] = mediaLeituras(mediasSensor);
    if (leitura_ldr[1] > color_cal[1]) {
      color_cal[1] = leitura_ldr[1];
    }
    
    digitalWrite(led_g, HIGH); // Apaga o LED verde
    delay(100); // Aguarda 100ms para estabilizar o LDR
    
    digitalWrite(led_b, LOW); // Acende o LED azul
    delay(100); // Aguarda 100ms para estabilizar o LDR
    
    // Realiza leitura do sensor e avalia se é o maior valor encontrado para o azul
    leitura_ldr[2] = mediaLeituras(mediasSensor);
    if (leitura_ldr[2] > color_cal[2]) {
      color_cal[2] = leitura_ldr[2];
    }
    
    digitalWrite(led_b, HIGH); // Apaga o LED azul
    delay(100); // Aguarda 100ms para estabilizar o LDR
    
  }

  for(int i = 0; i < 3; i++)
  {
    Serial.println(color_cal[i]);
  }

  delay(300);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Calibracao:");
  lcd.setCursor(0,1);
  lcd.print("Remova o objeto");
  Serial.println("Remova o objeto");
  while(analogRead(ldr) < ambiente_cal); // Aguarda a remoção do objeto

  // Início de calibração para valor mínimo de leitura do sensor
  black_cal[0] = 1023;
  black_cal[1] = 1023;
  black_cal[2] = 1023;

  delay(300);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Aproxime:");
  lcd.setCursor(0,1);
  lcd.print("Um objeto preto");
  Serial.println("Aproxime um objeto preto");
  
  while(analogRead(ldr) > objeto_cal); // Aguarda a aproximação de um objeto
  delay(500); // Delay para estabilização do objeto na área do sensor

  delay(300);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Calibracao:");
  lcd.setCursor(0,1);
  lcd.print("Calibrando...");
  Serial.println("Calibrando...");

  // São realizadas 10 medidas para encontrar o valor mínimo registrado pelo sensor
  for(int i = 0; i < 10; i++)
  {
    
    digitalWrite(led_r, LOW);
    delay(100);
    
    leitura_ldr[0] = mediaLeituras(mediasSensor);
    if (leitura_ldr[0] < black_cal[0]) {
      black_cal[0] = leitura_ldr[0];
    }
    
    digitalWrite(led_r, HIGH);
    delay(100);
    
    digitalWrite(led_g, LOW);
    delay(100);
    
    leitura_ldr[1] = mediaLeituras(mediasSensor);
    if (leitura_ldr[1] < black_cal[1]) {
      black_cal[1] = leitura_ldr[1];
    }
    
    digitalWrite(led_g, HIGH);
    delay(100);
    
    digitalWrite(led_b, LOW);
    delay(100);
    
    leitura_ldr[2] = mediaLeituras(mediasSensor);
    if (leitura_ldr[2] < black_cal[2]) {
      black_cal[2] = leitura_ldr[2];
    }
    
    digitalWrite(led_b, HIGH);
    delay(100);
    
  }

  for(int i = 0; i < 3; i++)
  {
    Serial.println(black_cal[i]);
  }

  delay(300);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Calibracao:");
  lcd.setCursor(0,1);
  lcd.print("Remova o objeto");
  Serial.println("Remova o objeto");
  while(analogRead(ldr) < ambiente_cal); // Aguarda a remoção do objeto

  // Fim da calibração
  delay(300);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Calibracao:");
  lcd.setCursor(0,1);
  lcd.print("Finalizada");
  Serial.println("Calibracao finalizada");
  delay(1000);
  
  Serial.println('#'); // Envia caractere para informar fim da calibração
}

// Rotina para realizar média nas leituras do sensor
int mediaLeituras(int vezes) {
  
  int leitura;
  int acumulador = 0;
  
  // Soma a leitura do sensor à variável acumulador pelo número de vezes
  for(int i = 0; i < vezes; i++){
    leitura = analogRead(ldr);
    acumulador += leitura;
    delay(10);
  }
  
  // Calcula a média
  return((acumulador)/vezes);
  
}

// Rotina de leitura do sensor
void leSensor() {
  
  digitalWrite(led_r, LOW); // Acende o LED vermelho
  delay(100); // Aguarda 100ms para o LDR estabilizar
  
  leitura_ldr[0] = mediaLeituras(mediasSensor); // Realiza a leitura do sensor
  
  digitalWrite(led_r, HIGH); // Apaga o LED vermelho
  delay(100); // Aguarda 100ms para o LDR estabilizar
  
  digitalWrite(led_g, LOW); // Acende o LED verde
  delay(100); // Aguarda 100ms para o LDR estabilizar
  
  leitura_ldr[1] = mediaLeituras(mediasSensor); // Realiza a leitura do sensor
  
  digitalWrite(led_g, HIGH); // // Apaga o LED verde
  delay(100); // Aguarda 100ms para o LDR estabilizar
  
  digitalWrite(led_b, LOW); // Acende o LED azul
  delay(100); // Aguarda 100ms para o LDR estabilizar
  
  leitura_ldr[2] = mediaLeituras(mediasSensor); // Realiza a leitura do sensor
  
  digitalWrite(led_b, HIGH); // Apaga o LED azul
  
}

// Rotina para enviar o valor da cor lida, convertida para a escala RGB
void enviaCores() {

  // Converte o valor lido do sensor para a escala RGB (0 - 255)
  unsigned char red = constrain(map(leitura_ldr[0], black_cal[0], color_cal[0], 0, 255),0,255);
  unsigned char green = constrain(map(leitura_ldr[1], black_cal[1], color_cal[1], 0, 255),0,255);
  unsigned char blue = constrain(map(leitura_ldr[2], black_cal[2], color_cal[2], 0, 255),0,255);

  delay(300);       
  lcd_key = 0;
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Identificar");

  if (((int)red > (int)green)&&((int)red > (int)blue)){
    if((int)green < (int)blue){
      lcd.setCursor(0,1);
      lcd.print("AMARELO");
      redColor = 0;
      greenColor = 0;
      blueColor = 0;
      yellowColor = 1;
    } else {
      lcd.setCursor(0,1);
      lcd.print("VERMELHO");
      redColor = 1;
      greenColor = 0;
      blueColor = 0;
      yellowColor = 0;
    }
  } else if (((int)green > (int)red)&&((int)green > (int)blue)) {
      lcd.setCursor(0,1);
      lcd.print("AZUL");
      redColor = 0;
      greenColor = 0;
      blueColor = 1;
      yellowColor = 0;
  } else if (((int)blue > (int)red)&&((int)blue > (int)green)) {
    lcd.setCursor(0,1);
    lcd.print("VERDE");
    redColor = 0;
    greenColor = 1;
    blueColor = 0;
    yellowColor = 0;
  } else {
    redColor = 0;
    greenColor = 0;
    blueColor = 0;
    yellowColor = 0;
    lcd.setCursor(0,1);
    lcd.print("PRETO");
  }
  
  // Envia os valores convertidos para a porta serial
  Serial.print("R = ");
  Serial.println(red, DEC); 
  Serial.print("G = ");
  Serial.println(green, DEC);
  Serial.print("B = ");
  Serial.println(blue, DEC);
  
}
