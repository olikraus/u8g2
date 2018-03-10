rm track_font.png
../png2bdf -f track -o track.bdf -e 65 *.png
../../bdfconv/bdfconv -f 1 -b 0 -m '65-97' -v track.bdf -n track_font -o track_font.c -d ../../bdf/helvB24.bdf -g 4
convert bdf.tga track_font.png
rm bdf.tga

