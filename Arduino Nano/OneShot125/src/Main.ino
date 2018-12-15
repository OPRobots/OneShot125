// Inclusión de librería del Timer1
#include <TimerOne.h>

// Pin de la placa donde se conecta la señal del ESC
// Usando el Timer1, solo se pueden usar los pines 9 y 10 del Arduino Nano
#define PIN_ESC 10

void setup() {
  // __CONFIGURACIÓN DEl TIMER__ //
  // Configura el Timer1 para dar una salida PWM de 4kHz,
  // de forma que el 100% del pulso sean 250us y el 50% sean 125us
  // El timer admite valores de PWM entre 0 y 1023
  Timer1.initialize(250);

  // __CONFIGURACIÓN DEl ESC //
  // Envía un 100% de PWM durante 5s (100% de potencia)
  Timer1.pwm(PIN_ESC, 1023);
  delay(5000);
  // Envía un 50% de PWM durante 5s (0% de potencia)
  Timer1.pwm(PIN_ESC, 512);
  delay(5000);
}
void loop() {
  // Envía un 75% de PWM (50% de potencia)
  Timer1.pwm(PIN_ESC, 768);
}


