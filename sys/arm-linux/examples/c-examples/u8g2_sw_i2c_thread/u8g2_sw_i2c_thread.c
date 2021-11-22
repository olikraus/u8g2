/*
 * Run three displays drawing random lines. This proves you can use U8g2 to run
 * multiple displays in a thread safe way.
 */

#include "u8g2port.h"
#include <pthread.h>
#include <string.h>
#include <errno.h>

// Encapsulate each thread's info
struct display {
	uint8_t gpio_chip;
	uint8_t scl;
	uint8_t sda;
	uint8_t res;
	unsigned long delay;
};

typedef struct display display_t;

/*
 * Draw random lines with 50 ms delay.
 */
void* do_display(void *arg) {
	display_t disp = *((display_t*) arg);
	pthread_t id = pthread_self();
	uint8_t h, w, y1, y2;

	printf("Thread %lu start\n", id);
	u8g2_t u8g2;
	// CHange setup function call as needed to support your display
	u8g2_Setup_ssd1306_i2c_128x32_univision_f(&u8g2, U8G2_R0, u8x8_byte_sw_i2c,
			u8x8_arm_linux_gpio_and_delay);
	init_i2c_sw(&u8g2, disp.gpio_chip, disp.scl, disp.sda, disp.res,
			disp.delay);
	u8g2_InitDisplay(&u8g2);
	h = u8g2_GetDisplayHeight(&u8g2);
	w = u8g2_GetDisplayWidth(&u8g2);
	u8g2_SetPowerSave(&u8g2, 0);
	for (int i = 1; i < 200; ++i) {
		u8g2_ClearBuffer(&u8g2);
		y1 = rand() % h;
		y2 = rand() % h;
		u8g2_DrawLine(&u8g2, 0, y1, w - 1, y2);
		u8g2_SendBuffer(&u8g2);
		sleep_ms(50);
	}
	u8g2_SetPowerSave(&u8g2, 1);
	// Close and deallocate GPIO resources
	done_user_data(&u8g2);
	printf("Thread %lu end\n", id);
	return NULL;
}

int main(void) {

	pthread_t id_1;
	pthread_t id_2;
	pthread_t id_3;

	void *retval_1;
	void *retval_2;
	void *retval_3;

	display_t display_1 = { 0, 14, 13, U8X8_PIN_NONE, 0 };
	display_t display_2 = { 0, 15, 16, U8X8_PIN_NONE, 0 };
	display_t display_3 = { 0, 198, 199, U8X8_PIN_NONE, 0 };

	pthread_create(&id_1, NULL, &do_display, &display_1);
	pthread_create(&id_2, NULL, &do_display, &display_2);
	pthread_create(&id_3, NULL, &do_display, &display_3);

	pthread_join(id_1, (void**) &retval_1);
	pthread_join(id_2, (void**) &retval_2);
	pthread_join(id_3, (void**) &retval_3);
	printf("Done\n");

	return 0;
}
