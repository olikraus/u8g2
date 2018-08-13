#!/bin/sh
mkdir tmp
builder=/home/kraus/prg/arduino-1.8.4/arduino-builder
ino=`ls *.ino`
tmpdir=`pwd`/tmp
echo $tmpdir

hardware="-hardware /home/kraus/prg/arduino-1.8.4/hardware -hardware /home/kraus/.arduino15/packages -hardware /home/kraus/Arduino/hardware"
tools="-tools /home/kraus/prg/arduino-1.8.4/tools-builder -tools /home/kraus/prg/arduino-1.8.4/hardware/tools/avr -tools /home/kraus/.arduino15/packages"
libs="-built-in-libraries /home/kraus/prg/arduino-1.8.4/libraries -libraries /home/kraus/Arduino/libraries"
pref1="-prefs=build.warn_data_percentage=75" 
pref2="-prefs=runtime.tools.bossac.path=/home/kraus/.arduino15/packages/arduino/tools/bossac/1.6.1-arduino" 
pref3="-prefs=runtime.tools.arm-none-eabi-gcc.path=/home/kraus/.arduino15/packages/arduino/tools/arm-none-eabi-gcc/4.8.3-2014q1"
vidpid="-vid-pid=0X2341_0X003D"

$builder -compile -logger=machine $hardware $tools $libs -fqbn=arduino:sam:arduino_due_x_dbg $vidpid -ide-version=10804 -build-path $tmpdir -warnings=none -build-cache $tmpdir $pref1 $pref2 $pref3 $pref4 -verbose $ino

# reset arduino board
stty -F /dev/ttyACM0 speed 1200 cs8 

# wait for it...
# https://stackoverflow.com/questions/43016993/how-can-i-force-a-leonardo-to-reset-with-avrdude
while :; do
  sleep 0.5
  [ -c /dev/ttyACM0 ] && break
done

# upload
/home/kraus/.arduino15/packages/arduino/tools/bossac/1.6.1-arduino/bossac -i -d --port=ttyACM0 -U false -e -w -v -b $tmpdir/$ino.bin -R 

# mkrzero
#/home/kraus/.arduino15/packages/arduino/tools/bossac/1.7.0/bossac -i -d --port=ttyACM0 -U true -i -e -w -v $tmpdir/$ino.bin -R 

