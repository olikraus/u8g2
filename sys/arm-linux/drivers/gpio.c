#include "gpio.h"

#define GPIO_FILENAME_DEFINE(pin,field) char fileName[255] = {0}; \
        sprintf(fileName, "/sys/class/gpio/gpio%d/%s", pin, field);

static int writeValueToFile(char* fileName, char* buff) 
{
    FILE *fp = fopen(fileName, "w");
    if (fp != NULL) 
    {
        fwrite(buff, strlen(buff), 1, fp);
        return 0;
    }
    return -1;
}

static int readValueFromFile(char* fileName, char* buff, int len) 
{
    int ret = -1;
    FILE *fp = fopen(fileName,"r");
    if (fp == NULL) 
    {
        return -1;
    } 
    else 
    {
        if (fread(buff, sizeof(char), len, fp) > 0) 
        {
            ret = 0;
        }
    }
    fclose(fp);
    return ret;
}

static int readIntValueFromFile(char* fileName) 
{
    char buff[255];
    memset(buff, 0, sizeof(buff));
    int ret = readValueFromFile(fileName, buff, sizeof(buff) - 1);
    if (ret == 0) {
        return atoi(buff);
    }
    return ret;
}

int exportGPIOPin(int pin) 
{
    FILE* fp = fopen("/sys/class/gpio/export", "w+");
    if (fp != NULL) 
    {
        fprintf(fp, "%d", pin);
        fclose(fp);
        return 0;
    }
    else
    {
        printf("Failed to export pin %d\n", pin);
        return -1;
    }
}

int unexportGPIOPin(int pin) 
{
    FILE* fp = fopen("/sys/class/gpio/unexport", "w+");
    if (fp != NULL) 
    {
        fprintf(fp, "%d", pin);
        fclose(fp);
        return 0;
    }
    else
    {
        printf("Failed to unexport pin %d\n", pin);
        return -1;
    }
}

int getGPIOValue(int pin) 
{
    GPIO_FILENAME_DEFINE(pin, "value")
    return readIntValueFromFile(fileName);
}

int setGPIOValue(int pin, int value) 
{
    static FILE* fp_gpio[255];
    if(fp_gpio[pin] != NULL)
    {
        fprintf(fp_gpio[pin], "%d", value);
        fflush(fp_gpio[pin]);
        return 0;
    }
    else
    {
        GPIO_FILENAME_DEFINE(pin, "value")
        fp_gpio[pin] = fopen(fileName, "w+");
        if (fp_gpio[pin] != NULL) 
        {
            fprintf(fp_gpio[pin], "%d", value);
            fflush(fp_gpio[pin]);
            return 0;
        }
        else
        {
            return -1;
        }
    }
}

int setGPIODirection(int pin, int direction) 
{
    char directionStr[10];
    GPIO_FILENAME_DEFINE(pin, "direction")
    
    if (direction == GPIO_IN) 
    {
        strcpy(directionStr, "in");
    }
    else if (direction == GPIO_OUT) 
    {
        strcpy(directionStr, "out");
    }
    else
    {
        return -1;
    }
    return writeValueToFile(fileName, directionStr);
}

int getGPIODirection(int pin) 
{
    char buff[255] = {0};
    int direction;
    int ret;

    GPIO_FILENAME_DEFINE(pin, "direction")
    ret = readValueFromFile(fileName, buff, sizeof(buff)-1);
    if (ret >= 0) 
    {
        if (strncasecmp(buff, "out", 3)==0)
        {
            direction = GPIO_OUT;
        }
        else if (strncasecmp(buff, "in", 2)==0)
        {
            direction = GPIO_IN;
        }
        else
        {
            return -1;
        }
        return direction;
    }
    return ret;
}
