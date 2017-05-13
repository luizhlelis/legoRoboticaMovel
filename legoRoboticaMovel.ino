
#include <Wire.h>
#include <LiquidCrystal.h>
#include <Adafruit_MotorShield.h>
  
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

//Define valores usados pelos botões
int lcd_key     = 0;
int adc_key_in  = 0;
#define btnRIGHT  5
#define btnUP     4
#define btnDOWN   3
#define btnLEFT   2
#define btnSELECT 1
#define btnNONE   0

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

//Pinos conectados aos LEDs
const int redPin = 49;
const int greenPin = 51;
const int bluePin = 53;
//Pino conectado ao sensor
const int analogInPin = A15;
//Valor de leitura do sensor
int sensorValue = 0;

void setup(){

	//Display com 16 colunas e 2 linhas
	lcd.begin(16, 2);

	//Inicia o objeto de motor com a frequencia default = 1.6KHz
	AFMS.begin();

	//Pinos dos LEDs terão comportamento de output
	pinMode(redPin, OUTPUT);
  	pinMode(greenPin, OUTPUT);
  	pinMode(bluePin, OUTPUT);
	//LEDs inicialmente desligados
	digitalWrite(redPin, LOW);
	digitalWrite(greenPin, LOW);
	digitalWrite(bluePin, LOW);

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
	while((lcd_key != 2)&&(lcd_key != 3)&&(lcd_key != 4)){
		lcd_key = read_LCD_buttons();
		lcd.scrollDisplayLeft();
		i++;
		lcd.setCursor(i,0);
		lcd.print("Escolha a tarefa");
    	delay(300);
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
				while((lcd_key != 2)&&(lcd_key != 3)&&(lcd_key != 4)){
					lcd_key = read_LCD_buttons();
					lcd.scrollDisplayLeft();
					i++;
					lcd.setCursor(i,0);
					lcd.print("Caminhos:");
    				delay(300);
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
#if 0
	//Identificação de cor
	if(lcd_key == 3){
		delay(300);
		lcd_key = 0;
		lcd.clear();
		lcd.setCursor(0,1);
		lcd.print("Sair (SELECT)");
		
		//Para teste: Azul=LEFT; Amarelo=DOWN; Vermelho=UP; Verde=RIGHT.
		while(lcd_key != 1){
			lcd.setCursor(0,0);
			lcd.print("Identificando...");
			lcd_key = read_LCD_buttons();
			sensorValue = analogRead(analogInPin);
			//Estou editando aqui:if(sensorValue == ){
				delay(300);
				lcd_key = 0;
				lcd.setCursor(0,0);
				lcd.print("Cor: Azul       ");
				delay(3000);
			}
			if(lcd_key == 3){
				delay(300);
				lcd_key = 0;
				lcd.setCursor(0,0);
				lcd.print("Cor: Amarelo    ");
				delay(3000);
			}
			if(lcd_key == 4){
				delay(300);
				lcd_key = 0;
				lcd.setCursor(0,0);
				lcd.print("Cor: Vermelho   ");
				delay(3000);
			}
			if(lcd_key == 5){
				delay(300);
				lcd_key = 0;
				lcd.setCursor(0,0);
				lcd.print("Cor: Verde      ");
				delay(3000);
			}
		}
	}

	//Multitarefa e tomada de decisão
	if(lcd_key == 4){
		delay(300);
		lcd_key = 0;
		lcd.clear();
		lcd.setCursor(0,0);
		lcd.print("Executando...");
		lcd.setCursor(0,1);
		lcd.print("Sair (SELECT)");
		while(lcd_key != 1){
			lcd_key = read_LCD_buttons();
		}
	}
#endif

	delay(300);
	lcd_key = 0;

} 
