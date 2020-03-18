#include "uart.h"
#include "accelerometer.h"
#include "ubit_led_matrix.h"
#include "utility.h"

int main() {
	uart_init();
	accelerometer_init();
	ubit_led_matrix_init();

	while(1) {

		int16_t accelerometer_data[3] = {0, 0, 0};
		accelerometer_read(&accelerometer_data[0], &accelerometer_data[1], &accelerometer_data[2]);

		int x_accel = accelerometer_data[0];
		int y_accel = accelerometer_data[1];
		int x_dot = x_accel / 50;
		int y_dot = y_accel / 50;

		ubit_led_matrix_light_only_at(x_dot, y_dot);	

		utility_print(&uart_send, "X: %6d Y: %6d Z: %6d\n\r", accelerometer_data[0], accelerometer_data[1], accelerometer_data[2]);
	}

	return 0;
}
