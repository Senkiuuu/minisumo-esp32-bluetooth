#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

// ========= CONFIGURACIÓN =========
String nombreBT = "SENKIUUU";  // 🔹 

// ================= PINES =================
#define pwmi  19
#define izq1  5
#define izq2  18
#define stby  13
#define der1  27
#define der2  26
#define pwmd  25
#define led   2
#define buzzer 23

// ============== PWM ======================
#define FREQ_PWM  1000
#define RES_PWM   8
#define CH_IZQ 0
#define CH_DER 1
#define CH_BUZZ 2

int vel = 150;

// Estado BT
bool btConectado = false;

// ================= SETUP ==================
void setup() {
  Serial.begin(115200);
  Serial.println("Iniciando sistema...");

  // -------- BLUETOOTH ----------
  SerialBT.begin(nombreBT);
  Serial.println("Bluetooth listo");

  // Pines
  pinMode(izq1, OUTPUT);
  pinMode(izq2, OUTPUT);
  pinMode(der1, OUTPUT);
  pinMode(der2, OUTPUT);
  pinMode(stby, OUTPUT);
  pinMode(led, OUTPUT);

  digitalWrite(stby, LOW);

  // PWM motores
  ledcAttach(pwmi, FREQ_PWM, RES_PWM);
  ledcAttach(pwmd, FREQ_PWM, RES_PWM);

  // PWM buzzer
  ledcAttach(buzzer, 2000, 8);

  // 🔊 Sonido inicio
  melodiaInicio();

  Serial.println("Sistema listo");
  Serial.println("F B L R S | Vel 1-9");
}

// ================= LOOP ===================
void loop() {
  if (SerialBT.available()) {
    char cmd = SerialBT.read();
    procesarComando(cmd);
  }

  if (Serial.available()) {
    char cmd = Serial.read();
    procesarComando(cmd);
  }

  // 🔊 Detectar conexión BT
  if (SerialBT.hasClient() && !btConectado) {
    btConectado = true;
    melodiaConexion();
  }

  if (!SerialBT.hasClient()) {
    btConectado = false;
  }
}

// ============ COMANDOS ====================
void procesarComando(char cmd) {
  switch (cmd) {
    // 🔹 Velocidad
    case '1': vel = 80;  blinkLED(); break;
    case '2': vel = 110; blinkLED(); break;
    case '3': vel = 130; blinkLED(); break;
    case '4': vel = 150; blinkLED(); break;
    case '5': vel = 170; blinkLED(); break;
    case '6': vel = 190; blinkLED(); break;
    case '7': vel = 210; blinkLED(); break;
    case '8': vel = 230; blinkLED(); break;
    case '9': vel = 255; blinkLED(); break;

    // Movimiento
    case 'F': motores( vel,  vel); break;
    case 'B': motores(-vel, -vel); break;
    
    // 🔄 Giros Corregidos
    case 'R': motores(-vel,  vel); break; 
    case 'L': motores( vel, -vel); break; 

    case 'S':
      motores(0, 0);
      digitalWrite(stby, LOW);
      break;
  }
}

// ============== LED =======================
void blinkLED() {
  digitalWrite(led, HIGH);
  delay(100);
  digitalWrite(led, LOW);
}

// ============ MOTORES =====================
void motores(int izq, int der) {
  digitalWrite(stby, HIGH);

  izq = constrain(izq, -255, 255);
  der = constrain(der, -255, 255);

  // Izquierdo
  if (izq >= 0) {
    digitalWrite(izq1, HIGH);
    digitalWrite(izq2, LOW);
  } else {
    digitalWrite(izq1, LOW);
    digitalWrite(izq2, HIGH);
    izq = -izq;
  }
  ledcWrite(pwmi, izq);

  // Derecho
  if (der >= 0) {
    digitalWrite(der1, HIGH);
    digitalWrite(der2, LOW);
  } else {
    digitalWrite(der1, LOW);
    digitalWrite(der2, HIGH);
    der = -der;
  }
  ledcWrite(pwmd, der);
}

// ============ BUZZER ======================
void tono(int freq, int dur) {
  ledcAttach(buzzer, freq, 8);
  ledcWrite(buzzer, 128); 
  delay(dur);
  ledcWrite(buzzer, 0);
}

void melodiaInicio() {
  tono(1000, 120);
  tono(1500, 120);
  tono(2000, 150);
}

void melodiaConexion() {
  tono(2000, 100);
  tono(1500, 100);
  tono(2500, 180);
}
