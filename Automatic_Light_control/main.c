#define F_CPU 12000000UL
#include <avr/io.h>
#include <util/delay.h>

#define lcd PORTC
#define rs PD2
#define e PD4
#define relay PB0
#define s1 PINA0
#define s2 PINA1

void delay(int);
void cmd(char);
void display(char);
void init(void);
void string(char *);
void view(int);
void controlRelay(int);

int count = 0;
char no[10] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

void delay(int d) {
	while (d--) {
		_delay_ms(1);
	}
}

void cmd(char c) {
	lcd = c;
	PORTD &= ~(1 << rs);
	PORTD |= (1 << e);
	delay(5);
	PORTD &= ~(1 << e);
}

void display(char c) {
	lcd = c;
	PORTD |= (1 << rs);
	PORTD |= (1 << e);
	delay(5);
	PORTD &= ~(1 << e);
}

void string(char *p) {
	while (*p) {
		display(*p++);
	}
}

void view(int n) {
	cmd(0xc0);
	display(no[(n / 10) % 10]);
	display(no[n % 10]);
}

void init(void) {
	cmd(0x38);
	cmd(0x0c);
	cmd(0x01);
	cmd(0x80);
}

void controlRelay(int state) {
	if (state) {
		PORTB |= (1 << relay); // Turn on relay
		} else {
		PORTB &= ~(1 << relay); // Turn off relay
	}
}

int main() {
	DDRD = 0xFF; // Port D as output
	DDRC = 0xFF; // Port C as output
	DDRB = (1 << relay); // Relay pin as output
	DDRA &= ~((1 << s1) | (1 << s2)); // Set s1 and s2 pins as inputs (IR sensors)
	PORTA |= (1 << s1) | (1 << s2); // Enable internal pull-up resistors for s1 and s2

	init();
	string("No of Person");
	cmd(0xc0);
	view(count);

	while (1) {
		if (PINA & (1 << s1)) {
			while (!(PINA & (1 << s2)));
			if (count != 99)
			count = count + 1;
			while (PINA & (1 << s2));
			view(count);
			} else if (PINA & (1 << s2)) {
			while (!(PINA & (1 << s1)));
			if (count != 0)
			count = count - 1;
			while (PINA & (1 << s1));
			view(count);
		}
		
		// Turn on relay when count is greater than 0, otherwise turn it off
		if (count > 0) {
			controlRelay(1); // Turn on relay when count is greater than 0
			} else {
			controlRelay(0); // Turn off relay when count is 0
		}

	}

	return 0;
}
