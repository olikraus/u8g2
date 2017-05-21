# works within ubuntu and min-gw (win7) environment
# win7 exe uploaded on google drive

CC = gcc
CFLAGS = -g -Wall
#CFLAGS = -O4 -Wall

SRC = main.c bdf_font.c bdf_glyph.c bdf_parser.c bdf_map.c bdf_rle.c bdf_tga.c fd.c bdf_8x8.c bdf_kern.c

OBJ = $(SRC:.c=.o)
ASM = $(SRC:.c=.s)

.c.s:
	$(CC) $(CFLAGS) -S -o $@ $<

bdfconv: $(OBJ) $(ASM)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJ) -o bdfconv

clean:	
	-rm $(OBJ) $(ASM) bdfconv

test:	bdfconv
	./bdfconv 

greek: bdfconv
	./bdfconv -f 1 -v -m '32-127,$$370-$$3ff' ../bdf/unifont.bdf -n unifont_greek -o unifont_greek.c -d ../bdf/7x13.bdf

polish: bdfconv
	./bdfconv -f 1 -v -m '32-127,$$104-$$107,$$118-$$119,$$141-$$144,$$15a-$$15b,$$179-$$17c,$$d3,$$f3' ../bdf/unifont.bdf -n unifont_polish -o unifont_polish.c -d ../bdf/7x13.bdf

helvb18: bdfconv
	./bdfconv -f 1 -m '32-127,x32-64,x91-96,x123-191,x247,x697-879' -p 10 ../bdf/helvB18.bdf -o helvb18_tr.c -n u8g2_font_helvB18_tr -k helvb18_tr_k.c

helvb18n: bdfconv
	./bdfconv -v -f 1 -m '32,48-57' ../bdf/helvB18.bdf -o helvb18_tn.c  -n u8g2_font_helvB18_tn -d ../bdf/helvB18.bdf 

logisoso: bdfconv
	-../otf2bdf/otf2bdf -a -r 72 -p 131 -o tmp.bdf ../ttf/Logisoso.ttf
	./bdfconv -v -f 1 -m '32,42-57' tmp.bdf -o u8g2_font_logisoso_tn.c  -n u8g2_font_logisoso_tn -d ../bdf/helvB18.bdf 

tga: bdfconv
	./bdfconv -v -f 1 -m '32-255' ../bdf/helvB18.bdf -o helvb18_tn.c -n u8g2_font_helvB18_tn -d ../bdf/7x13.bdf

etl: bdfconv
	./bdfconv -v -f 1 -m '32-127,128-255>3552' ../bdf/etl16-thai.bdf -o etl16_t.c -n u8g2_font_etl16_t -d ../bdf/7x13.bdf

etl2: bdfconv
	./bdfconv -v -f 1 -m '32-127,128-255>14*256-32' ../bdf/etl16-thai.bdf -o etl16_t.c -n u8g2_font_etl16_t -d ../bdf/7x13.bdf

artos: bdfconv
	./bdfconv -b 3 -d ../bdf/7x13.bdf ../bdf/ArtosSerif-8.bdf -a -v
	
hebrew: bdfconv
	./bdfconv -v -f 1 -m '32-128,$$590-$$5ff,$$fb1d-$$fb4f' ../bdf/unifont.bdf -o u8g2_hebrew.c  -n u8g2_hebrew -d ../bdf/helvB18.bdf 

korean: bdfconv
	./bdfconv -v -f 1 -m '32-128,$$AC00-$$D7AF' ../bdf/unifont.bdf -o u8g2_korean.c  -n u8g2_korean -d ../bdf/helvB18.bdf 

bengali: bdfconv
	./bdfconv -v -f 1 -m '32-128,$$980-$$9ff' ../bdf/unifont.bdf -o u8g2_bengali.c  -n u8g2_bengali -d ../bdf/helvB12.bdf 

urdu: bdfconv
	./bdfconv -v -f 1 -m '32-128,$$600-$$6ff,$$750-$$77f,$$fb50-$$fdff,$$fe70-$$feff' ../bdf/unifont.bdf -o u8g2_urdu.c  -n u8g2_urdu -d ../bdf/helvB12.bdf 

lubr24: bdfconv
	./bdfconv -f 1 -m '32-255' -p 10 lubR24.bdf -o lubR24.c -n u8g2_font_lubR24 -d ../bdf/helvB12.bdf 

lurs24: bdfconv
	./bdfconv -f 1 -m '32-255' -p 10 luRS24.bdf -o luRS24.c -n u8g2_font_luRS24 -d ../bdf/helvB12.bdf 

	
#	http://stackoverflow.com/questions/713397/what-is-a-minimal-set-of-unicode-characters-for-reasonable-japanese-support

chinese: bdfconv
	./bdfconv ../bdf/unifont.bdf -b 0 -f 1 -M ../build/chinese1.map -d ../bdf/7x13.bdf -n u8g2_font_unifont_t_chinese1
	convert bdf.tga -trim u8g2_font_unifont_t_chinese1.png
	./bdfconv ../bdf/unifont.bdf -b 0 -f 1 -M ../build/chinese2.map -d ../bdf/7x13.bdf -n u8g2_font_unifont_t_chinese2
	convert bdf.tga -trim u8g2_font_unifont_t_chinese2.png
	./bdfconv ../bdf/unifont.bdf -b 0 -f 1 -M ../build/chinese3.map -d ../bdf/7x13.bdf -n u8g2_font_unifont_t_chinese3
	convert bdf.tga -trim u8g2_font_unifont_t_chinese3.png
	
	./bdfconv ../bdf/gb16st.bdf -b 0 -f 1 -M ../build/chinese1.map -d ../bdf/7x13.bdf -n u8g2_font_gb16st_t_1
	convert bdf.tga -trim u8g2_font_gb16st_t_1.png
	./bdfconv ../bdf/gb16st.bdf -b 0 -f 1 -M ../build/chinese2.map -d ../bdf/7x13.bdf -n u8g2_font_gb16st_t_2
	convert bdf.tga -trim u8g2_font_gb16st_t_2.png
	./bdfconv ../bdf/gb16st.bdf -b 0 -f 1 -M ../build/chinese3.map -d ../bdf/7x13.bdf -n u8g2_font_gb16st_t_3
	convert bdf.tga -trim u8g2_font_gb16st_t_3.png
	./bdfconv ../bdf/gb24st.bdf -b 0 -f 1 -M ../build/chinese1.map -d ../bdf/7x13.bdf -n u8g2_font_gb24st_t_1
	convert bdf.tga -trim u8g2_font_gb24st_t_1.png
	./bdfconv ../bdf/gb24st.bdf -b 0 -f 1 -M ../build/chinese2.map -d ../bdf/7x13.bdf -n u8g2_font_gb24st_t_2
	convert bdf.tga -trim u8g2_font_gb24st_t_2.png
	./bdfconv ../bdf/gb24st.bdf -b 0 -f 1 -M ../build/chinese3.map -d ../bdf/7x13.bdf -n u8g2_font_gb24st_t_3
	convert bdf.tga -trim u8g2_font_gb24st_t_3.png
