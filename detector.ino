/* DEFINIENDO PINES DE DETECTOR DE GAS*/
const int pinGas = A0; //pin analogico para el sensor de gas
const int pinLed = 13; //pin digital para indicar escape de gas(HIGH=escape de gas y LOW=sin escape)
const int pinBuzzer = 10; //pin pwm del buzzer
// VARIABLES : sensor de gas
int lecturaGas = 0; // guarda la lectura del sensor
// VARIABLES : alarma
float sinVal; // guarda valor de sen
int toneVal;

/* MODULO BLUETOOH*/ 
 const int pinLedB = 9; // indicara el incio de la configuracion
 const int pinBT = 8; //pin vcc de modulo bluetooh
 const int pinEA = 7; // encendido y apagado del sensor
 // VARIABLES: bluetooh
 const char nombreBT[11] = "HC-05";
 const char pin[5] = "0000"; // pin para asignar al bluetooh
 const char velocidad = '4'; // constante 4 = 9600
 const char mode = '1'; // 0 esclavo y 1 maestro

/* FUNCIONES */
void on_off_led(int pin){
  digitalWrite(pin,HIGH);
  delay(100);
  digitalWrite(pin,LOW);
  delay(100);
  return ;
}
void alarma(){
  for(int i = 0;  i < 180 ; i++){
    sinVal = (sin(i*(3.1412/180)));
    toneVal = 2000 + int((sinVal*1000));
    tone(pinBuzzer,toneVal);
    delay(2);
  }
}

void conf_BT(){
  
  pinMode(pinLedB,OUTPUT); // azul
  pinMode(pinBT,OUTPUT);
  Serial.begin(38400);
  digitalWrite(pinLedB,HIGH);
  delay(4000);
  digitalWrite(pinLedB,LOW);
  digitalWrite(pinBT,HIGH);
  delay(3000);
  Serial.print("AT\r\n");
  Serial.print("AT+NAME:");
  Serial.print(nombreBT);
  Serial.print("\r\n");
  Serial.print("AT+PIN:");
  Serial.print(pin);
  Serial.print("\r\n");
  Serial.print("AT+BAUD:");
  Serial.print(velocidad);
  Serial.print("\r\n");
  Serial.print("AT+MODE:");
  Serial.print(mode);
  Serial.print("\r\n");
  digitalWrite(pinLedB,HIGH);
  digitalWrite(pinLedB,LOW);
}

bool Conect_android(){
  if(Serial.available()){
    char data=Serial.read();
    if(data=='1'){ //conectado
      digitalWrite(pinEA,HIGH);
      return true;
    }else if(data == '0'){//desconectado
      return false;
    }
  }  
}

void iniciar_sensor(){
    digitalWrite(pinEA, HIGH);
    lecturaGas = analogRead(pinGas);
    Serial.println(lecturaGas,DEC);
    if(lecturaGas > 300 && lecturaGas < 400){
      alarma();
      on_off_led(pinLed);
      Serial.println("!Cuidado, escape de gas!");
    }else if(lecturaGas > 400){
      alarma();
      on_off_led(pinLedB); //pin azul
      on_off_led(pinLed);// pin rojo
      Serial.println("!Peligro, salga de casa!");
    }else{
      noTone(pinBuzzer);
      digitalWrite(pinLed, LOW);
      Serial.println("Todo tranquilo :D");
    }
    delay(1000);
}
void setup() {
  pinMode(pinGas, INPUT );
  pinMode(pinLed, OUTPUT );
  pinMode(pinBuzzer, OUTPUT);
  pinMode(pinLedB, OUTPUT );
  pinMode(pinEA, OUTPUT );
  conf_BT();
  Serial.begin(9600); 

}

void loop() {
  Serial.println("Realizando la configuracion ...");
  if(Conect_android() == true){
      Serial.println("Sensor prendido");
      iniciar_sensor();
  }else{
    Serial.println("Sensor apagado");
    digitalWrite(pinEA,LOW);
  }
  delay(1000);
  // Obtenemos la lecttura del sensor de gas
  

}