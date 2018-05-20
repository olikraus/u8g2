HEXFILE=LittleRookChess.hex
ARDUINO_PATH=/home/kraus/prg/arduino-1.8.4
stty -F /dev/ttyACM0 1200 raw ignbrk hup
sleep 3
${ARDUINO_PATH}/hardware/tools/avr/bin/avrdude -V -F -C ${ARDUINO_PATH}/hardware/tools/avr/etc/avrdude.conf -p atmega32u4 -P /dev/ttyACM0 -c avr109  -b 57600 -U flash:w:${HEXFILE}
