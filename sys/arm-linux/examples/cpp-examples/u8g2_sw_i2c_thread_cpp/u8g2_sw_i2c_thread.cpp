#include <U8g2lib.h>
#include <pthread.h>

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
void * doDisplay(void *arg) {
	display_t disp = *((display_t*) arg);
	pthread_t id = pthread_self();
	uint8_t h, w, y1, y2;

	printf("Thread %lu start\n", id);
	U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C u8g2(U8G2_R0, disp.scl, disp.sda,
			U8X8_PIN_NONE);
	u8g2.initI2cSw(disp.gpio_chip, disp.scl, disp.sda, disp.res, disp.delay);
	u8g2.begin();
	h = u8g2.getDisplayHeight();
	w = u8g2.getWidth();
	for (int i = 1; i < 200; ++i) {
		u8g2.clearBuffer();
		y1 = rand() % h;
		y2 = rand() % h;
		u8g2.drawLine(0, y1, w - 1, y2);
		u8g2.sendBuffer();
		u8g2.sleepMs(50);
	}
	u8g2.sleepOn();
	u8g2.doneUserData();
	printf("Thread %lu end\n", id);
	return NULL;
}

int main(void) {
	pthread_t id1;
	pthread_t id2;
	pthread_t id3;

	void *retVal1;
	void *retVal2;
	void *retVal3;

	display_t display1 = { 0, 14, 13, U8X8_PIN_NONE, 0 };
	display_t display2 = { 0, 15, 16, U8X8_PIN_NONE, 0 };
	display_t display3 = { 0, 198, 199, U8X8_PIN_NONE, 0 };

	pthread_create(&id1, NULL, &doDisplay, &display1);
	pthread_create(&id2, NULL, &doDisplay, &display2);
	pthread_create(&id3, NULL, &doDisplay, &display3);

	pthread_join(id1, (void**) &retVal1);
	pthread_join(id2, (void**) &retVal2);
	pthread_join(id3, (void**) &retVal3);
	printf("Done\n");

	return 0;
}
