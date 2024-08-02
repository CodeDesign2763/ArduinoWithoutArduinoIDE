#include <avr/io.h>
#include <avr/interrupt.h>

#define THRESHOLD 250
#define OUTPUT_PIN PD4

ISR(ADC_vect) {
	volatile unsigned int adc_val;
	volatile unsigned char low, high;

	/* При работе с 2-х байтовыми
	регистрами сначала читается
	младший байт */
	cli();
	low = ADCL;
	high = ADCH;
	adc_val = high*256+low;
		
	if (adc_val>THRESHOLD) {
		PORTD |= 1 << OUTPUT_PIN;
	} else {
		PORTD &= ~(1 << OUTPUT_PIN);
	}
	sei();
}

/* Должен быть пустой обработчик
 * прерывания, которое используется
 * для запуска АЦП */
ISR(TIMER0_COMPA_vect) {
}

int main(void) {
	/* Настройка T0 */
	/* Режим работы: CTC */
	TCCR0A |= 1 << WGM01;
	/* Предделитель: 1/1024 */
	TCCR0B |= (1 << CS02) | (1 << CS00);
	/* Регистр сравнения */
	OCR0A = 240;
	/* Прерывание при совпадении с OCR0A */
	TIMSK0 |= 1 << OCIE0A;
	
	/* Настройка ADC */
	/* Режим работы: авто */
	ADCSRA |= 1 << ADATE;
	/* Запускающее устройство: T0, совп. с OCR0A */
	ADCSRB |= (1 << ADTS1) | (1 << ADTS0);
	/* Делитель ТИ АЦП - 1/128 (fмакс < 200 КГц) */
	ADCSRA |= (1 << ADPS2) | (1<< ADPS1) | (1<<ADPS0);
	/* Источник опорного напряжения: внутренний 5В */
	ADMUX |= (0 << REFS1) | (1 << REFS0);
	/* Источник сигнала - пин ADC0 */
	/* По умолчанию */
	/* Прерывания ADC: включены */
	ADCSRA |= 1 << ADIE;
	/* Включить ADC */
	ADCSRA |= 1 << ADEN;
	
	DDRD |= 1 << OUTPUT_PIN;

	sei();
	while (1) {
	}
}
