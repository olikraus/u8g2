gcc -g bdf*.c fd.c main.c -o bdfconv.exe
dir bdfconv.exe


./bdfconv.exe -v -f 1 -m '32-127' ../bdf/helvB18.bdf -o helvb18_tf.c  -n u8g2_font_helvB18_tf -d ../bdf/helvB18.bdf