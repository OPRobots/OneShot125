// clang-format off
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/timer.h>
// clang-format on

// Velocidad máxima de reloj. En caso de la BluePill son 72MHz
#define FRECUENCIA_RELOJ_HZ 72000000
// Cantidad de microsegundos en un segundo
#define US_POR_S 1000000

/**
 * @brief 
 * Configura el TIM4 para dar una salida PWM de 4kHz, 
 * de forma que el 100% del pulso sean 250us y el 50% sean 125us
 * 
 * En este caso se usa solo el CH4 porque es el pin donde se ha conectado la señal del ESC
 * 
 */
static void setup_esc_timer(void) {
  timer_set_mode(TIM4, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);

  timer_set_prescaler(TIM4, 3);
  timer_set_repetition_counter(TIM4, 0);
  timer_enable_preload(TIM4);
  timer_continuous_mode(TIM4);
  timer_set_period(TIM4, 4500);

  timer_set_oc_mode(TIM4, TIM_OC4, TIM_OCM_PWM1);
  timer_set_oc_value(TIM4, TIM_OC4, 4500);
  timer_enable_oc_output(TIM4, TIM_OC4);

  timer_enable_counter(TIM4);
}
/**
 * @brief 
 * Realiza una pausa en el código leyendo los ticks de reloj
 * 
 * @param us Microsegundos que debe pausar la ejecución.
 */
void sleep_us(uint32_t us) {
  // Ticks actuales
  uint32_t initial_cycles = dwt_read_cycle_counter();
  // Cantidad de ticks que debe detener la ejecución en función de la frecuencia
  uint32_t sleep_cycles = (uint32_t)(FRECUENCIA_RELOJ_HZ * ((float)us / (float)US_POR_S));

  while (dwt_read_cycle_counter() - initial_cycles <= sleep_cycles) {
  };
}

int main(void) {

  // __CONFIGURACIÓN DEl MICRO__ //
  // Establece la velocidad del reloj en 72mhz
  rcc_clock_setup_in_hse_8mhz_out_72mhz();
  // Habilita el contador de ticks de reloj
  dwt_enable_cycle_counter();
  // Inicia el reloj del GPIOB
  rcc_periph_clock_enable(RCC_GPIOB);
  // Establece el pin GPIO_TIM4_CH4 (PB9) como salida PWM
  gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO_TIM4_CH4);
  // Inicia los relojes del TIM4
  rcc_periph_clock_enable(RCC_TIM4);
  // Configura el TIM4 para PWM a 4kHz. OneShot125
  setup_esc_timer();

  // __CONFIGURACIÓN DEl ESC //
  // Envía un 100% de PWM durante 5s (100% de potencia)
  timer_set_oc_value(TIM4, TIM_OC4, 4500);
  sleep_us(5000000);
  // Envía un 50% de PWM durante 5s (0% de potencia)
  timer_set_oc_value(TIM4, TIM_OC4, 2250);
  sleep_us(5000000);

  // Bucle infinito del programa principal
  while (1) {
    // Envía un 75% de PWM (50% de potencia)
    timer_set_oc_value(TIM4, TIM_OC4, 3375);
  }
  return 0;
}