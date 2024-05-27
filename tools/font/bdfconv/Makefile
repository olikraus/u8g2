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

issue_1811: bdfconv
	./bdfconv -f 1 -v -m '32-127,$$980-$$9FF,$$A00-$$A7F,$$A80-$$AFF,$$B00-$$B7f,$$B80-$$BFF,$$C00-$$C7f,$$C80-$$CFF' ../bdf/unifont.bdf -n unifont_1811 -o unifont_1811.c -d ../bdf/7x13.bdf
	convert -trim bdf.tga issue_1811.png

greek: bdfconv
	./bdfconv -f 1 -v -m '32-127,$$370-$$3ff' ../bdf/unifont.bdf -n unifont_greek -o unifont_greek.c -d ../bdf/7x13.bdf


cyrillic: bdfconv
	./bdfconv -f 1 -v -m '$$20-$$7E, $$410-$$44F' ../bdf/unifont.bdf -n unifont_cyrillic_44f -o unifont_cyrillic_44f.c -d ../bdf/7x13.bdf
	convert bdf.tga unifont_cyrillic_44f.png
	./bdfconv -f 1 -v -m '$$20-$$7E, $$400-$$4ff' ../bdf/unifont.bdf -n unifont_cyrillic_4ff -o unifont_cyrillic_4ff.c -d ../bdf/7x13.bdf
	convert bdf.tga unifont_cyrillic_4ff.png

battery19: bdfconv
	./bdfconv -f 1 -v -m '32-127' ../bdf/battery19.bdf -n battery19 -o battery19.c -d ../bdf/7x13.bdf

battery24: bdfconv
	./bdfconv -f 1 -v -m '32-127s' ../bdf/battery24.bdf -n battery24 -o battery24.c -d ../bdf/7x13.bdf
	convert -trim bdf.tga battery24.png

hornet: bdfconv
	./bdfconv -a -f 1 -v -m '32-127s' ./tmp/hornet.bdf -n hornet -o hornet.c -d ../bdf/7x13.bdf
	convert -trim bdf.tga hornet.png

10x20greek: bdfconv
	./bdfconv -f 1 -v -m '32-127,$$370-$$3ff' ../bdf/10x20.bdf -n 10x20_greek -o 10x20_greek.c -d ../bdf/7x13.bdf

5x7: bdfconv
	./bdfconv -b 4 -f 1 -a -v -m '32-255' ../bdf/5x7.bdf -n 5x7lcd -o 5x7lcd.c -d ../bdf/7x13.bdf

ti83: bdfconv
	-../otf2bdf/otf2bdf  -n -p 5 -o ../bdf/ti83.bdf ../ttf/ti83.ttf
	./bdfconv -b 4 -f 1 -a -v -m '32-65000' ../bdf/ti83.bdf -n u8g2_ti83_5x7 -o ti83.c -d ../bdf/7x13.bdf

kalpurush: bdfconv
	-../otf2bdf/otf2bdf  -g -v -p 16 -o ../bdf/kalpurush.bdf kalpurush.ttf
	./bdfconv -f 1 -m '0-65000>32' ../bdf/kalpurush.bdf -n u8g2_kalpurush -o kalpurush.c -d ../bdf/7x13.bdf
	convert bdf.tga -trim kalpurush.png


polish: bdfconv
	./bdfconv -f 1 -v -m '32-127,$$104-$$107,$$118-$$119,$$141-$$144,$$15a-$$15b,$$179-$$17c,$$d3,$$f3' ../bdf/unifont.bdf -n unifont_polish -o unifont_polish.c -d ../bdf/7x13.bdf

helvb18: bdfconv
	./bdfconv -f 1 -m '32-127,x32-64,x91-96,x123-191,x247,x697-879' -p 10 ../bdf/helvB18.bdf -o helvb18_tr.c -n u8g2_font_helvB18_tr -k helvb18_tr_k.c

helvb18n: bdfconv
	./bdfconv -v -f 1 -m '32,48-57' ../bdf/helvB18.bdf -o helvb18_tn.c  -n u8g2_font_helvB18_tn -d ../bdf/helvB18.bdf 

dseg7: bdfconv
	./bdfconv -v -f 1 -m '32-65000' dseg7_classic_regular.bdf -o u8g2_font_dseg7_classic_regular_all.c  -n u8g2_font_dseg7_classic_regular_all -d ../bdf/helvR12.bdf 
	convert bdf.tga -trim u8g2_font_dseg7_classic_regular_all.png

nmv1: bdfconv
	./bdfconv -v -f 1 -m '32-65000' NMV1.bdf -o u8g2_font_nmv1.c  -n u8g2_font_nmv1 -d ../bdf/helvR12.bdf 
	convert bdf.tga -trim u8g2_font_nmv1.png

dseg7cr: bdfconv
	./bdfconv -v -f 1 -b 2 -m '48-57,65-90,97-122' dseg7_classic_regular.bdf -o u8g2_font_dseg7_cr.c  -n u8g2_font_dseg7_cr -d ../bdf/helvR12.bdf 
	convert bdf.tga -trim u8g2_font_dseg7_cr.png

dseg14cr: bdfconv
	./bdfconv -v -f 1 -b 2 -m '32-255' dseg14_classic_regular.bdf -o u8g2_font_dseg14_cr.c  -n u8g2_font_dseg14_cr -d ../bdf/helvR12.bdf 
	convert bdf.tga -trim u8g2_font_dseg14_cr.png
        
issue1769: bdfconv
	./bdfconv -v -f 1 -b 2 -m '32-128' 3x7_Multi_V7.bdf -o u8g2_font_3x7_multi.c  -n u8g2_font_3x7_multi -d ../bdf/helvR12.bdf 
	convert bdf.tga -trim u8g2_font_3x7_multi.png

issue2406: bdfconv
	-../otf2bdf/otf2bdf -y -v -r 72 -p 16 -o tmp.bdf ../tmp/zfull/zfull-regular.ttf
	#-../otf2bdf/otf2bdf -v -r 72 -p 16 -o tmp.bdf ../ttf/Logisoso.ttf
	#-../otf2bdf/otf2bdf -v -r 72 -p 16 -o tmp.bdf ../ttf/PressStart2P.ttf
	#-../otf2bdf/otf2bdf -v -r 72 -p 16 -o tmp.bdf ../ttf/Wizzard.ttf
	./bdfconv -v -f 1 -b 2 -m '32-128' tmp.bdf -o u8g2_zfull.c  -n u8g2_zfull -d ../bdf/helvR12.bdf 
	convert bdf.tga -trim u8g2_zfull.png


emoticons21: bdfconv
	./bdfconv -v -f 1 -m '32-64' ../emoticons/emoticons21.bdf -o emoticons21.c  -n u8g2_font_emoticons21_tn -d ../bdf/helvB18.bdf 

mmmicons: bdfconv
	./bdfconv -v -f 1 -m '32-700' mmmicons.bdf -o mmmicons.c  -n u8g2_font_mmmicons -d ../bdf/helvB18.bdf 
	convert bdf.tga -trim mmmicons.png

ded: bdfconv
	./bdfconv -v -b 0 -f 1  ./F16_DED.bdf -o ded.c  -n ded -d ../bdf/helvB12.bdf 

old16x16: bdfconv
	-../otf2bdf/otf2bdf -r 72 -p 16 -o tmp.bdf ../ttf/Px437_Wyse700a.ttf
	./bdfconv -x 0 -y 8 -v -f 2 -m '32-59' tmp.bdf -o u8x8_font_px437wyse700a_ul_n.c  -n u8x8_font_px437wyse700a_ul_n
	./bdfconv -x 8 -y 8 -v -f 2 -m '32-59' tmp.bdf -o u8x8_font_px437wyse700a_ur_n.c  -n u8x8_font_px437wyse700a_ur_n
	./bdfconv -x 0 -y 0 -v -f 2 -m '32-59' tmp.bdf -o u8x8_font_px437wyse700a_ll_n.c  -n u8x8_font_px437wyse700a_ll_n
	./bdfconv -x 8 -y 0 -v -f 2 -m '32-59' tmp.bdf -o u8x8_font_px437wyse700a_lr_n.c  -n u8x8_font_px437wyse700a_lr_n	
	./bdfconv -x 0 -y 8 -v -f 2 -m '32-127' tmp.bdf -o u8x8_font_px437wyse700a_ul_r.c  -n u8x8_font_px437wyse700a_ul_r
	./bdfconv -x 8 -y 8 -v -f 2 -m '32-127' tmp.bdf -o u8x8_font_px437wyse700a_ur_r.c  -n u8x8_font_px437wyse700a_ur_r
	./bdfconv -x 0 -y 0 -v -f 2 -m '32-127' tmp.bdf -o u8x8_font_px437wyse700a_ll_r.c  -n u8x8_font_px437wyse700a_ll_r
	./bdfconv -x 8 -y 0 -v -f 2 -m '32-127' tmp.bdf -o u8x8_font_px437wyse700a_lr_r.c  -n u8x8_font_px437wyse700a_lr_r
	./bdfconv -x 0 -y 8 -v -f 2 -m '32-255' tmp.bdf -o u8x8_font_px437wyse700a_ul_f.c  -n u8x8_font_px437wyse700a_ul_f
	./bdfconv -x 8 -y 8 -v -f 2 -m '32-255' tmp.bdf -o u8x8_font_px437wyse700a_ur_f.c  -n u8x8_font_px437wyse700a_ur_f
	./bdfconv -x 0 -y 0 -v -f 2 -m '32-255' tmp.bdf -o u8x8_font_px437wyse700a_ll_f.c  -n u8x8_font_px437wyse700a_ll_f
	./bdfconv -x 8 -y 0 -v -f 2 -m '32-255' tmp.bdf -o u8x8_font_px437wyse700a_lr_f.c  -n u8x8_font_px437wyse700a_lr_f	
	-../otf2bdf/otf2bdf -r 72 -p 16 -o tmp.bdf ../ttf/Px437_Wyse700b.ttf
	./bdfconv -x 0 -y 8 -v -f 2 -m '32-59' tmp.bdf -o u8x8_font_px437wyse700b_ul_n.c  -n u8x8_font_px437wyse700b_ul_n
	./bdfconv -x 8 -y 8 -v -f 2 -m '32-59' tmp.bdf -o u8x8_font_px437wyse700b_ur_n.c  -n u8x8_font_px437wyse700b_ur_n
	./bdfconv -x 0 -y 0 -v -f 2 -m '32-59' tmp.bdf -o u8x8_font_px437wyse700b_ll_n.c  -n u8x8_font_px437wyse700b_ll_n
	./bdfconv -x 8 -y 0 -v -f 2 -m '32-59' tmp.bdf -o u8x8_font_px437wyse700b_lr_n.c  -n u8x8_font_px437wyse700b_lr_n	
	./bdfconv -x 0 -y 8 -v -f 2 -m '32-127' tmp.bdf -o u8x8_font_px437wyse700b_ul_r.c  -n u8x8_font_px437wyse700b_ul_r
	./bdfconv -x 8 -y 8 -v -f 2 -m '32-127' tmp.bdf -o u8x8_font_px437wyse700b_ur_r.c  -n u8x8_font_px437wyse700b_ur_r
	./bdfconv -x 0 -y 0 -v -f 2 -m '32-127' tmp.bdf -o u8x8_font_px437wyse700b_ll_r.c  -n u8x8_font_px437wyse700b_ll_r
	./bdfconv -x 8 -y 0 -v -f 2 -m '32-127' tmp.bdf -o u8x8_font_px437wyse700b_lr_r.c  -n u8x8_font_px437wyse700b_lr_r
	./bdfconv -x 0 -y 8 -v -f 2 -m '32-255' tmp.bdf -o u8x8_font_px437wyse700b_ul_f.c  -n u8x8_font_px437wyse700b_ul_f
	./bdfconv -x 8 -y 8 -v -f 2 -m '32-255' tmp.bdf -o u8x8_font_px437wyse700b_ur_f.c  -n u8x8_font_px437wyse700b_ur_f
	./bdfconv -x 0 -y 0 -v -f 2 -m '32-255' tmp.bdf -o u8x8_font_px437wyse700b_ll_f.c  -n u8x8_font_px437wyse700b_ll_f
	./bdfconv -x 8 -y 0 -v -f 2 -m '32-255' tmp.bdf -o u8x8_font_px437wyse700b_lr_f.c  -n u8x8_font_px437wyse700b_lr_f
	cat u8x8_font_px437*.c >16x16.c

16x16: bdfconv
	-../otf2bdf/otf2bdf -r 72 -p 16 -o tmp.bdf ../ttf/Px437_Wyse700b.ttf
	./bdfconv -y -1 -th 2 -tv 2 -v -f 2 -m '32-127' tmp.bdf -o u8x8_font_px437wyse700b_2x2_r.c  -n u8x8_font_px437wyse700b_2x2_r
	-../otf2bdf/otf2bdf -r 72 -p 16 -o tmp.bdf ../ttf/Px437_Wyse700a.ttf
	./bdfconv -y -1 -th 2 -tv 2 -v -f 2 -m '32-127' tmp.bdf -o u8x8_font_px437wyse700a_2x2_r.c  -n u8x8_font_px437wyse700a_2x2_r
	-../otf2bdf/otf2bdf -r 72 -p 32 -o tmp.bdf ../ttf/BALTT_0.ttf
	./bdfconv -y 0 -th 1 -tv 2 -v -f 2 -m '32-127' tmp.bdf -o u8x8_font_balthasar_titling_nbp_1x2_r.c  -n u8x8_font_balthasar_titling_nbp_1x2_r
	-../otf2bdf/otf2bdf -r 72 -p 32 -o tmp.bdf ../ttf/lucasarts-scumm-subtitle-roman-outline.ttf
	./bdfconv -y 1 -th 2 -tv 2 -v -f 2 -m '32-127' tmp.bdf -o lucasarts_scumm_subtitle_o_2x2_r.c  -n lucasarts_scumm_subtitle_o_2x2_r
	-../otf2bdf/otf2bdf -r 72 -p 16 -o tmp.bdf ../ttf/lucasarts-scumm-subtitle-roman.ttf
	./bdfconv -y 0 -th 2 -tv 2 -v -f 2 -m '32-127' tmp.bdf -o lucasarts_scumm_subtitle_r_2x2_r.c  -n lucasarts_scumm_subtitle_r_2x2_r  
	./bdfconv -y -1 -th 2 -tv 3 -v -f 2 -m '32-127' ../bdf/courB18.bdf -o u8x8_font_courb18_2x3_r.c  -n u8x8_font_courB18_2x3_r
	./bdfconv -y -1 -th 2 -tv 3 -v -f 2 -m '32-127' ../bdf/courR18.bdf -o u8x8_font_courr18_2x3_r.c  -n u8x8_font_courR18_2x3_r
	./bdfconv -y -2 -th 3 -tv 4 -v -f 2 -m '32-127' ../bdf/courB24.bdf -o u8x8_font_courb24_3x4_r.c  -n u8x8_font_courB24_3x4_r
	./bdfconv -y -2 -th 3 -tv 4 -v -f 2 -m '32-127' ../bdf/courR24.bdf -o u8x8_font_courr24_3x4_r.c  -n u8x8_font_courR24_3x4_r
	./bdfconv -y 0 -th 2 -tv 3 -v -f 2 -m '32-127' ../bdf/profont29.bdf -o u8x8_font_profont29_2x3_r.c  -n u8x8_font_profont29_2x3_r
	./bdfconv -y -2 -th 1 -tv 2 -v -f 2 -m '32-127' ../bdf/8x13.bdf -o u8x8_font_8x13_r.c  -n u8x8_font_8x13_1x2_r
	./bdfconv -y -2 -th 1 -tv 2 -v -f 2 -m '32-127' ../bdf/8x13B.bdf -o u8x8_font_8x13b_r.c  -n u8x8_font_8x13B_1x2_r
	./bdfconv -y -1 -x -1 -th 1 -tv 2 -v -f 2 -m '32-127' ../bdf/7x14.bdf -o u8x8_font_7x14_r.c  -n u8x8_font_7x14_1x2_r
	./bdfconv -y -1 -x -1 -th 1 -tv 2 -v -f 2 -m '32-127' ../bdf/7x14B.bdf -o u8x8_font_7x14b_r.c  -n u8x8_font_7x14B_1x2_r
	-../otf2bdf/otf2bdf -r 72 -p 16 -o tmp.bdf ../ttf/shylock_nbp.ttf
	./bdfconv -y -1 -th 1 -tv 2 -v -f 2 -m '32-127' tmp.bdf -o u8x8_font_shylock_nbp_1x2_r.c  -n u8x8_font_shylock_nbp_1x2_r

	-../otf2bdf/otf2bdf -a -r 72 -p 30 -o tmp.bdf ../ttf/inr.otf
	./bdfconv -y -1 -x 1 -th 2 -tv 4 -v -f 2 -m '32-127' tmp.bdf -o u8x8_font_inr21_2x4_r.c  -n u8x8_font_inr21_2x4_r
	-../otf2bdf/otf2bdf -a -r 72 -p 44 -o tmp.bdf ../ttf/inr.otf
	./bdfconv -y -1 -x 2 -th 3 -tv 6 -v -f 2 -m '32-127' tmp.bdf -o u8x8_font_inr33_3x6_r.c  -n u8x8_font_inr33_3x6_r
	-../otf2bdf/otf2bdf -a -r 72 -p 62 -o tmp.bdf ../ttf/inr.otf
	./bdfconv -y 0 -x 3 -th 4 -tv 8 -v -f 2 -m '32-127' tmp.bdf -o u8x8_font_inr46_4x8_r.c  -n u8x8_font_inr46_4x8_r

	-../otf2bdf/otf2bdf -a -r 72 -p 30 -o tmp.bdf ../ttf/inb.otf
	./bdfconv -y -1 -x 1 -th 2 -tv 4 -v -f 2 -m '32-127' tmp.bdf -o u8x8_font_inb21_2x4_r.c  -n u8x8_font_inb21_2x4_r
	-../otf2bdf/otf2bdf -a -r 72 -p 44 -o tmp.bdf ../ttf/inb.otf
	./bdfconv -y -1 -x 2 -th 3 -tv 6 -v -f 2 -m '32-127' tmp.bdf -o u8x8_font_inb33_3x6_r.c  -n u8x8_font_inb33_3x6_r
	-../otf2bdf/otf2bdf -a -r 72 -p 62 -o tmp.bdf ../ttf/inb.otf
	./bdfconv -y 0 -x 3 -th 4 -tv 8 -v -f 2 -m '32-127' tmp.bdf -o u8x8_font_inb46_4x8_r.c  -n u8x8_font_inb46_4x8_r

	./bdfconv -y 0 -th 2 -tv 2 -v -f 2 -m '32-127' ../bdf/open_iconic_weather_2x.bdf -o u8x8_font_open_iconic_weather_2x2.c  -n u8x8_font_open_iconic_weather_2x2
	./bdfconv -y 0 -th 4 -tv 4 -v -f 2 -m '32-127' ../bdf/open_iconic_weather_4x.bdf -o u8x8_font_open_iconic_weather_4x4.c  -n u8x8_font_open_iconic_weather_4x4
	./bdfconv -y 0 -th 8 -tv 8 -v -f 2 -m '32-127' ../bdf/open_iconic_weather_8x.bdf -o u8x8_font_open_iconic_weather_8x8.c  -n u8x8_font_open_iconic_weather_8x8


falconded: bdfconv
	-../otf2bdf/otf2bdf  -n -p 12 -o falconded.bdf FalconDED2.ttf
	./bdfconv -v -f 1 -b 2 -a -m '32-96' falconded.bdf -o u8g2_font_falconded_mr.c  -n u8g2_font_falconded_mr -d ../bdf/helvR12.bdf 
	-../otf2bdf/otf2bdf  -n -p 12 -o falconded.bdf FalconDED2.ttf
	./bdfconv -v -f 1 -b 1 -a -m '32-96' falconded.bdf -o u8g2_font_falconded_hr.c  -n u8g2_font_falconded_hr -d ../bdf/helvR12.bdf 

falconded9: bdfconv
	./bdfconv -v -f 1 -b 2 -a -m '32-99' falconded.bdf -o u8g2_font_falconded_mr.c  -n u8g2_font_falconded_mr -d ../bdf/helvR12.bdf 
	convert bdf.tga -trim u8g2_font_falconded_mr.png
	./bdfconv -v -f 1 -b 1 -a -m '32-99' falconded.bdf -o u8g2_font_falconded_hr.c  -n u8g2_font_falconded_hr -d ../bdf/helvR12.bdf 
	convert bdf.tga -trim u8g2_font_falconded_hr.png


analecta: bdfconv
	-../otf2bdf/otf2bdf -a  -p 20 -o analecta.bdf Analecta.otf
	./bdfconv -v -f 1 -m '32-65535' analecta.bdf -o u8g2_font_analecta.c  -n u8g2_font_analecta -d ../bdf/helvB18.bdf 

logisoso: bdfconv
	-../otf2bdf/otf2bdf -a -r 72 -p 131 -o tmp.bdf ../ttf/Logisoso.ttf
	./bdfconv -v -f 1 -m '32,42-57' tmp.bdf -o u8g2_font_logisoso_tn.c  -n u8g2_font_logisoso_tn -d ../bdf/helvB18.bdf 

tpss: bdfconv
	-../otf2bdf/otf2bdf -r 72 -p 8 -o tmp.bdf ../ttf/TEACPSS_.TTF
	./bdfconv -v -f 1 -m '32-255' tmp.bdf -o u8g2_font_tpss_tf.c  -n u8g2_font_tpss_tn -d ../bdf/helvR10.bdf 


q_pixia: bdfconv
	-../otf2bdf/otf2bdf -a -r 72 -p 13 -o tmp.bdf q_pixia.ttf
	./bdfconv -v -f 1 -m '32-65534' tmp.bdf -o u8g2_font_q_pixia.c  -n u8g2_font_q_pixia -d ../bdf/helvB18.bdf 

inr: bdfconv
	-../otf2bdf/otf2bdf -a -r 72 -p 86 -o tmp.bdf ../ttf/inr.otf
	./bdfconv -v -b 2 -f 1 -m '42-57' tmp.bdf -o u8g2_font_inr64_m.c  -n u8g2_font_inr64_m -d ../bdf/helvB18.bdf 
	-../otf2bdf/otf2bdf -a -r 72 -p 43 -o tmp.bdf ../ttf/inr.otf
	./bdfconv -v -b 2 -f 1 -m '42-57' tmp.bdf -o u8g2_font_inr32_m.c  -n u8g2_font_inr32_m -d ../bdf/helvB18.bdf 
#	./bdfconv -v -b 2 -f 1 -m '32-128,$$400-$$52f' tmp.bdf -o u8g2_font_inr_m.c  -n u8g2_font_inr_m -d ../bdf/helvB18.bdf 

logisoso92x: bdfconv
	-../otf2bdf/otf2bdf -a -r 72 -p 131 -o tmp.bdf ../ttf/Logisoso.ttf
	./bdfconv -v -f 1 -m '32,42-57,65-70' tmp.bdf -o u8g2_font_logisoso92_tx.c  -n u8g2_font_logisoso92_tx -d ../bdf/helvB18.bdf 

fub: bdfconv
	-../otf2bdf/otf2bdf -a -r 72 -p 40 -o tmp.bdf ../ttf/FreeUniversal-Bold.ttf
	./bdfconv -v -f 2 -m '32-255' tmp.bdf -o fub30_tf.c  -n fub30_tf -d ../bdf/helvB18.bdf 

fub20x: bdfconv
	-../otf2bdf/otf2bdf -a -r 72 -p 27 -o tmp.bdf ../ttf/FreeUniversal-Bold.ttf
	./bdfconv -v -f 1 -m '32-127,8364' tmp.bdf -o u8g2_font_fub20_tx.c  -n u8g2_font_fub20_tx -d ../bdf/helvB18.bdf 

unifont_emoticon: bdfconv
	./bdfconv -v -f 1 -m '$$01f600-$$01f64f>$$20, $$01f910-$$01f92f>$$70, $$01f970-$$01f971>$$90, $$01f973-$$01f976>$$92,$$01f97a>$$96, $$01f9b8-$$01f9b9>$$97, $$01f9d0-$$01f9d6>$$99' ../bdf/unifont_upper.bdf -o u8g2_font_unifont_emoticon.c  -n u8g2_font_unifont_emoticon_tx -d ../bdf/helvB18.bdf 

unifont_misc_sym_and_pic: bdfconv
	./bdfconv -v -f 1 -m '$$01f300-$$01f5ff>$$f300' ../bdf/unifont_upper.bdf -o u8g2_font_unifont_misc_sym_and_pic.c  -n u8g2_font_unifont_misc_sym_and_pic_tx -d ../bdf/helvB18.bdf 

unifont_animals: bdfconv
	./bdfconv -v -f 1 -m '$$01f400-$$01f43f>$$20,$$01f980-$$01f9af>$$60' ../bdf/unifont_upper.bdf -o u8g2_font_unifont_animal.c  -n u8g2_font_unifont_animal_tx -d ../bdf/helvB18.bdf 

unifont_domino: bdfconv
	./bdfconv -v -f 1 -m '$$01f030-$$01f093>$$20' ../bdf/unifont_upper.bdf -o u8g2_font_unifont_domino.c  -n u8g2_font_unifont_domino_tx -d ../bdf/helvB18.bdf 

unifont_cards: bdfconv
	./bdfconv -v -f 1 -m '$$01f0a0-$$01f0f5>$$20' ../bdf/unifont_upper.bdf -o u8g2_font_unifont_cards.c  -n u8g2_font_unifont_cards_tx -d ../bdf/helvB18.bdf 

unifont_weather: bdfconv
	./bdfconv -v -f 1 -m '$$01f310-$$01f32c>$$20' ../bdf/unifont_upper.bdf -o u8g2_font_unifont_weather.c  -n u8g2_font_unifont_weather_tx -d ../bdf/helvB18.bdf 


climacons: bdfconv
	-../otf2bdf/otf2bdf -a -r 72 -p 32 -o tmp.bdf Climacons.ttf
	./bdfconv -v -f 1 -m '32-256' tmp.bdf -o u8g2_font_climacons.c  -n u8g2_font_climacons -d ../bdf/helvB18.bdf 

logisoso20x: bdfconv
	-../otf2bdf/otf2bdf -a -r 72 -p 30 -o tmp.bdf ../ttf/Logisoso.ttf
	./bdfconv -v -f 1 -m '32-127,8364' tmp.bdf -o u8g2_font_logisoso20_tx.c  -n u8g2_font_logisoso20_tx -d ../bdf/helvB18.bdf 

xlogisoso: bdfconv
	-../otf2bdf/otf2bdf -a -r 72 -p 131 -o tmp.bdf ../ttf/Logisoso.ttf
	./bdfconv -v -f 1 -m '32,42-57,$$370-$$3ff,$$2100-$$21ff' tmp.bdf -o u8g2_font_logisoso_tn.c  -n u8g2_font_logisoso_tn -d ../bdf/helvB18.bdf 

omega: bdfconv
	-../otf2bdf/otf2bdf -a -r 72 -p 32 -o tmp.bdf ../ttf/FreeUniversal-Regular.ttf
	./bdfconv -v -f 1 -m '32,35-57,$$300-$$3ff,$$2030-$$3000' tmp.bdf -o u8g2_font_omega_tn.c  -n u8g2_font_omega_tn -d ../bdf/helvB18.bdf 

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

malayalam: bdfconv
	./bdfconv -v -f 1 -m '32-128,$$d00-$$d7f' ../bdf/unifont.bdf -o u8g2_malayalam.c  -n u8g2_font_malayalam -d ../bdf/helvB12.bdf 

lubr24: bdfconv
	./bdfconv -f 1 -m '32-65500' -p 10 lubR24.bdf -o lubR24.c -n u8g2_font_lubR24 -d ../bdf/helvB12.bdf 

lurs24: bdfconv
	./bdfconv -f 1 -m '32-255' -p 10 luRS24.bdf -o luRS24.c -n u8g2_font_luRS24 -d ../bdf/helvB12.bdf 

luBS18: bdfconv
	./bdfconv -v -f 1 -m '32-34,36-125,176>35,8364>126' -p 10 ../bdf/luBS18.bdf -o luBS18.c -n u8g2_font_luBS18 -d ../bdf/helvB12.bdf 





siji: bdfconv
#	./bdfconv -v -f 1 -m '32-$$ffff' ./siji.bdf -o test1.c  -n test -d ../bdf/helvB12.bdf 
	./bdfconv -v -f 1 -m '32-$$ffff' ../bdf/siji_with_6x10.bdf -o test2.c  -n test -d ../bdf/helvB12.bdf 

wqy: bdfconv
	./bdfconv -v -f 1 -M ../build/gb2312.map ../bdf/wenquanyi_9pt.bdf -o test.c  -n test -d ../bdf/helvB12.bdf 
	
#	./bdfconv -v -f 1 -M ../build/gb2312.map ../bdf/wenquanyi_10pt.bdf -o test.c  -n test -d ../bdf/helvB12.bdf 
#	./bdfconv -v -f 1 -M ../build/gb2312.map ../bdf/wenquanyi_13px.bdf -o test.c  -n test -d ../bdf/helvB12.bdf 
#	./bdfconv -v -f 1 -M ../build/gb2312.map ../bdf/wenquanyi_11pt.bdf -o test.c  -n test -d ../bdf/helvB12.bdf 
#	./bdfconv -v -f 1 -M ../build/gb2312.map ../bdf/wenquanyi_12pt.bdf -o test.c  -n test -d ../bdf/helvB12.bdf 


ncenR24: bdfconv
	./bdfconv  -v -f 1 -b 0 -m '32-701>32,7838' ../bdf/ncenR24.bdf -n u8g2_font_ncenR24_te -o test.c 

ncenR10: bdfconv
	./bdfconv  -v -f 1 -m '48-57,88,89' ../bdf/ncenR10.bdf -n u8g2_font_ncenR10 -o test.c -d ../bdf/helvB12.bdf 
	convert bdf.tga -trim ncenR10.png


profont12v2: bdfconv
	#./bdfconv  -v -f 1 -b 0 -m '32-701>32,7838' ../bdf/profont12v2.bdf -n u8g2_font_profont12v2_te -o test.c 
	./bdfconv  -v -f 1 -b 0 -m '32-127' ../bdf/profont12v2.bdf -n u8g2_font_profont12v2_tr -o test.c 

	
#	http://stackoverflow.com/questions/713397/what-is-a-minimal-set-of-unicode-characters-for-reasonable-japanese-support

chinese: bdfconv
	./bdfconv ../bdf/wenquanyi_9pt.bdf -g 32 -b 0 -f 1 -M ../build/chinese1.map -d ../bdf/7x13.bdf -n u8g2_font_wqy12_t_chinese1
	convert bdf.tga -trim u8g2_font_wqy12_t_chinese1.png
	./bdfconv ../bdf/wenquanyi_9pt.bdf -g 32 -b 0 -f 1 -M ../build/chinese2.map -d ../bdf/7x13.bdf -n u8g2_font_wqy12_t_chinese2
	convert bdf.tga -trim u8g2_font_wqy12_t_chinese2.png
	./bdfconv ../bdf/wenquanyi_9pt.bdf -g 32 -b 0 -f 1 -M ../build/chinese3.map -d ../bdf/7x13.bdf -n u8g2_font_wqy12_t_chinese3
	convert bdf.tga -trim u8g2_font_wqy12_t_chinese3.png
	./bdfconv ../bdf/wenquanyi_9pt.bdf -g 32 -b 0 -f 1 -M ../build/gb2312.map -d ../bdf/7x13.bdf -n u8g2_font_wqy12_t_gb2312
	convert bdf.tga -trim u8g2_font_wqy12_t_gb2312.png
	./bdfconv ../bdf/wenquanyi_9pt.bdf -g 32 -b 0 -f 1 -M ../build/gb2312a.map -d ../bdf/7x13.bdf -n u8g2_font_wqy12_t_gb2312a
	convert bdf.tga -trim u8g2_font_wqy12_t_gb2312a.png
	./bdfconv ../bdf/wenquanyi_9pt.bdf -g 32 -b 0 -f 1 -M ../build/gb2312b.map -d ../bdf/7x13.bdf -n u8g2_font_wqy12_t_gb2312b
	convert bdf.tga -trim u8g2_font_wqy12_t_gb2312b.png

	./bdfconv ../bdf/wenquanyi_10pt.bdf -g 32 -b 0 -f 1 -M ../build/chinese1.map -d ../bdf/7x13.bdf -n u8g2_font_wqy13_t_chinese1
	convert bdf.tga -trim u8g2_font_wqy13_t_chinese1.png
	./bdfconv ../bdf/wenquanyi_10pt.bdf -g 32 -b 0 -f 1 -M ../build/chinese2.map -d ../bdf/7x13.bdf -n u8g2_font_wqy13_t_chinese2
	convert bdf.tga -trim u8g2_font_wqy13_t_chinese2.png
	./bdfconv ../bdf/wenquanyi_10pt.bdf -g 32 -b 0 -f 1 -M ../build/chinese3.map -d ../bdf/7x13.bdf -n u8g2_font_wqy13_t_chinese3
	convert bdf.tga -trim u8g2_font_wqy13_t_chinese3.png
	./bdfconv ../bdf/wenquanyi_10pt.bdf -g 32 -b 0 -f 1 -M ../build/gb2312.map -d ../bdf/7x13.bdf -n u8g2_font_wqy13_t_gb2312
	convert bdf.tga -trim u8g2_font_wqy13_t_gb2312.png
	./bdfconv ../bdf/wenquanyi_10pt.bdf -g 32 -b 0 -f 1 -M ../build/gb2312a.map -d ../bdf/7x13.bdf -n u8g2_font_wqy13_t_gb2312a
	convert bdf.tga -trim u8g2_font_wqy13_t_gb2312a.png
	./bdfconv ../bdf/wenquanyi_10pt.bdf -g 32 -b 0 -f 1 -M ../build/gb2312b.map -d ../bdf/7x13.bdf -n u8g2_font_wqy13_t_gb2312b
	convert bdf.tga -trim u8g2_font_wqy13_t_gb2312b.png

	./bdfconv ../bdf/wenquanyi_13px.bdf -g 32 -b 0 -f 1 -M ../build/chinese1.map -d ../bdf/7x13.bdf -n u8g2_font_wqy14_t_chinese1
	convert bdf.tga -trim u8g2_font_wqy14_t_chinese1.png
	./bdfconv ../bdf/wenquanyi_13px.bdf -g 32 -b 0 -f 1 -M ../build/chinese2.map -d ../bdf/7x13.bdf -n u8g2_font_wqy14_t_chinese2
	convert bdf.tga -trim u8g2_font_wqy14_t_chinese2.png
	./bdfconv ../bdf/wenquanyi_13px.bdf -g 32 -b 0 -f 1 -M ../build/chinese3.map -d ../bdf/7x13.bdf -n u8g2_font_wqy14_t_chinese3
	convert bdf.tga -trim u8g2_font_wqy14_t_chinese3.png
	./bdfconv ../bdf/wenquanyi_13px.bdf -g 32 -b 0 -f 1 -M ../build/gb2312.map -d ../bdf/7x13.bdf -n u8g2_font_wqy14_t_gb2312
	convert bdf.tga -trim u8g2_font_wqy14_t_gb2312.png
	./bdfconv ../bdf/wenquanyi_13px.bdf -g 32 -b 0 -f 1 -M ../build/gb2312a.map -d ../bdf/7x13.bdf -n u8g2_font_wqy14_t_gb2312a
	convert bdf.tga -trim u8g2_font_wqy14_t_gb2312a.png
	./bdfconv ../bdf/wenquanyi_13px.bdf -g 32 -b 0 -f 1 -M ../build/gb2312b.map -d ../bdf/7x13.bdf -n u8g2_font_wqy14_t_gb2312b
	convert bdf.tga -trim u8g2_font_wqy14_t_gb2312b.png

	./bdfconv ../bdf/wenquanyi_11pt.bdf -g 32 -b 0 -f 1 -M ../build/chinese1.map -d ../bdf/7x13.bdf -n u8g2_font_wqy15_t_chinese1
	convert bdf.tga -trim u8g2_font_wqy15_t_chinese1.png
	./bdfconv ../bdf/wenquanyi_11pt.bdf -g 32 -b 0 -f 1 -M ../build/chinese2.map -d ../bdf/7x13.bdf -n u8g2_font_wqy15_t_chinese2
	convert bdf.tga -trim u8g2_font_wqy15_t_chinese2.png
	./bdfconv ../bdf/wenquanyi_11pt.bdf -g 32 -b 0 -f 1 -M ../build/chinese3.map -d ../bdf/7x13.bdf -n u8g2_font_wqy15_t_chinese3
	convert bdf.tga -trim u8g2_font_wqy15_t_chinese3.png
	./bdfconv ../bdf/wenquanyi_11pt.bdf -g 32 -b 0 -f 1 -M ../build/gb2312.map -d ../bdf/7x13.bdf -n u8g2_font_wqy15_t_gb2312
	convert bdf.tga -trim u8g2_font_wqy15_t_gb2312.png
	./bdfconv ../bdf/wenquanyi_11pt.bdf -g 32 -b 0 -f 1 -M ../build/gb2312a.map -d ../bdf/7x13.bdf -n u8g2_font_wqy15_t_gb2312a
	convert bdf.tga -trim u8g2_font_wqy15_t_gb2312a.png
	./bdfconv ../bdf/wenquanyi_11pt.bdf -g 32 -b 0 -f 1 -M ../build/gb2312b.map -d ../bdf/7x13.bdf -n u8g2_font_wqy15_t_gb2312b
	convert bdf.tga -trim u8g2_font_wqy15_t_gb2312b.png

	./bdfconv ../bdf/wenquanyi_12pt.bdf -g 32 -b 0 -f 1 -M ../build/chinese1.map -d ../bdf/7x13.bdf -n u8g2_font_wqy16_t_chinese1
	convert bdf.tga -trim u8g2_font_wqy16_t_chinese1.png
	./bdfconv ../bdf/wenquanyi_12pt.bdf -g 32 -b 0 -f 1 -M ../build/chinese2.map -d ../bdf/7x13.bdf -n u8g2_font_wqy16_t_chinese2
	convert bdf.tga -trim u8g2_font_wqy16_t_chinese2.png
	./bdfconv ../bdf/wenquanyi_12pt.bdf -g 32 -b 0 -f 1 -M ../build/chinese3.map -d ../bdf/7x13.bdf -n u8g2_font_wqy16_t_chinese3
	convert bdf.tga -trim u8g2_font_wqy16_t_chinese3.png
	./bdfconv ../bdf/wenquanyi_12pt.bdf -g 32 -b 0 -f 1 -M ../build/gb2312.map -d ../bdf/7x13.bdf -n u8g2_font_wqy16_t_gb2312
	convert bdf.tga -trim u8g2_font_wqy16_t_gb2312.png
	./bdfconv ../bdf/wenquanyi_12pt.bdf -g 32 -b 0 -f 1 -M ../build/gb2312a.map -d ../bdf/7x13.bdf -n u8g2_font_wqy16_t_gb2312a
	convert bdf.tga -trim u8g2_font_wqy16_t_gb2312a.png
	./bdfconv ../bdf/wenquanyi_12pt.bdf -g 32 -b 0 -f 1 -M ../build/gb2312b.map -d ../bdf/7x13.bdf -n u8g2_font_wqy16_t_gb2312b
	convert bdf.tga -trim u8g2_font_wqy16_t_gb2312b.png


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

gayathri: bdfconv
	-../otf2bdf/otf2bdf -a -r 72 -p 24 -o gayathri.bdf Gayatri.ttf
	./bdfconv -v -f 1 -m '$$21-$$1fd,$$d00-$$d7f,$$200c-$$20b9,$$10003-$$102bc>$$e003' gayathri.bdf -o u8g2_font_gayathri_tn.c  -n u8g2_font_gayathri_tn -d ../bdf/helvB18.bdf 
	convert bdf.tga gayathri.png

meera: bdfconv
	-../otf2bdf/otf2bdf -a -r 72 -p 24 -o meera.bdf Meera.ttf
	./bdfconv -v -f 1 -m '$$21-$$2dc,$$d02-$$d6f,$$200c-$$2215,$$10003-$$1043f>$$e003' meera.bdf -o u8g2_font_meera_tn.c  -n u8g2_font_meera_tn -d ../bdf/helvB18.bdf 
	convert bdf.tga meera.png

gess: bdfconv
	./bdfconv -v -f 1 -m '$$20-$$ffff,~1588,~65010,~65205,~65206' GESSTwoBold-Bold-75.bdf -o u8g2_font_gess.c  -n u8g2_font_gess -d ../bdf/helvB18.bdf 
	convert bdf.tga u8g2_font_gess.png

CARSYMBOL: bdfconv
	./bdfconv -v -f 1 -m '$$20-$$ffff' CARSYMBOL.bdf -o u8g2_font_carsymbol.c  -n u8g2_font_carsymbol -d ../bdf/helvB18.bdf 
	convert bdf.tga u8g2_font_carsymbol.png

Aurebesh: bdfconv
	./bdfconv -v -f 1 -m '$$20-$$ff' Aurebesh.bdf -o u8g2_font_aurebesh.c  -n u8g2_font_aurebesh -d ../bdf/helvB18.bdf 
	convert bdf.tga u8g2_font_aurebesh.png

aurebesh_small: bdfconv
	./bdfconv -v -f 1 -m '$$20-$$ff' aurebesh_small.bdf -o u8g2_font_aurebesh_small.c  -n u8g2_font_aurebesh_small -d ../bdf/helvB18.bdf 
	convert bdf.tga u8g2_font_aurebesh_small.png

issue_1521: bdfconv
	./bdfconv -v -f 1 -m '$$0000-$$3B97' ../bdf/unifont.bdf -o unifont_custom.c -n unifont_custom  -d ../bdf/helvB18.bdf 
	convert bdf.tga unifont_custom.png

out: bdfconv
	./bdfconv -v -f 1 -m '$$0000-$$3B97' out.bdf -o out.c -n out  -d ../bdf/helvR14.bdf 
	convert bdf.tga out.png

issue833: bdfconv
	./bdfconv -v -f 1 -m '19968-40959' unifont-13.0.06.bdf -o out.c -n out  -d unifont-13.0.06.bdf 
	convert bdf.tga out.png


waffle: bdfconv
	./bdfconv -v -f 1 -m '32-65535' ../bdf/waffle-10.bdf -o tmp.c  -n u8g2_font_helvB18_tn -d ../bdf/helvB18.bdf 
	convert bdf.tga out.png

CalBlk36: bdfconv
	./bdfconv -v -f 1 -m '32-65535' ../bdf/CalBlk36.bdf -o tmp.c  -n u8g2_font_helvB18_tn -d ../bdf/helvB18.bdf 
	convert bdf.tga out.png
        
CalLite24: bdfconv
	./bdfconv -v -f 1 -m '32-65535' ../bdf/CalLite24.bdf -o tmp.c  -n u8g2_font_helvB18_tn -d ../bdf/helvB18.bdf 
	convert bdf.tga out.png

7seg_33x19: bdfconv
	./bdfconv -v -f 1 -m '32-79' ./tmp/7_Seg_33x19.bdf -o tmp.c  -n u8g2_font_helvB18_tn -d ../bdf/helvB18.bdf 
	convert bdf.tga out.png

notos: bdfconv
	./bdfconv -v -f 1 -m '32-65535,~9939,~10084' NotoEmoji-SemiBold-32.bdf -o u8g2_font_noto_semi_32_t.c  -n u8g2_font_noto_semi_32_t -d ../bdf/helvB18.bdf 
	convert bdf.tga u8g2_font_noto_semi_32_t.png
	./bdfconv -v -f 1 -m '32-127,$$1f300-$$1ffff>$$f300,~127995' NotoEmoji-SemiBold-32.bdf -o u8g2_font_noto_semi_32_t2.c  -n u8g2_font_noto_semi_32_t2 -d ../bdf/helvB18.bdf 
	convert bdf.tga u8g2_font_noto_semi_32_t2.png
	./bdfconv -v -f 1 -m '32-127,$$1f300-$$1f32c>$$f300,$$2600-$$2604,$$2614,$$26c4,$$26c5,$$26c8,$$26f1' NotoEmoji-SemiBold-24.bdf -o u8g2_font_noto_semi_24_t_weather.c  -n u8g2_font_noto_semi_24_t_weather -d ../bdf/helvB18.bdf 
	convert bdf.tga u8g2_font_noto_semi_24_t_weather.png
	./bdfconv -v -f 1 -m '32-127,$$1f600-$$1f637>$$f600,$$1f641-$$1f64f>$$f641, $$1f910-$$1f917>$$f910, $$1f920-$$1f92f>$$f920, $$1f970-$$1f97a>$$f970, $$1fae0-$$1fae5>$$fae0' NotoEmoji-SemiBold-24.bdf -o u8g2_font_noto_semi_24_t_emoji.c  -n u8g2_font_noto_semi_24_t_emoji -d ../bdf/helvB18.bdf 
	convert bdf.tga u8g2_font_noto_semi_24_t_emoji.png
	./bdfconv -v -f 1 -m '32-127,$$1f300-$$1f32c>$$f300,$$2600-$$2604,$$2614,$$26c4,$$26c5,$$26c8,$$26f1' NotoEmoji-SemiBold-32.bdf -o u8g2_font_noto_semi_32_t_weather.c  -n u8g2_font_noto_semi_32_t_weather -d ../bdf/helvB18.bdf 
	convert bdf.tga u8g2_font_noto_semi_32_t_weather.png
	./bdfconv -v -f 1 -m '32-127,$$1f600-$$1f637>$$f600,$$1f641-$$1f64f>$$f641, $$1f910-$$1f917>$$f910, $$1f920-$$1f92f>$$f920, $$1f970-$$1f97a>$$f970, $$1fae0-$$1fae5>$$fae0' NotoEmoji-SemiBold-32.bdf -o u8g2_font_noto_semi_32_t_emoji.c  -n u8g2_font_noto_semi_32_t_emoji -d ../bdf/helvB18.bdf 
	convert bdf.tga u8g2_font_noto_semi_32_t_emoji.png
	./bdfconv -v -f 1 -m '32-127,$$1f300-$$1f32c>$$f300,$$2600-$$2604,$$2614,$$26c4,$$26c5,$$26c8,$$26f1,~127747,~127751' NotoEmoji-SemiBold-40.bdf -o u8g2_font_noto_semi_40_t_weather.c  -n u8g2_font_noto_semi_40_t_weather -d ../bdf/helvB18.bdf 
	convert bdf.tga u8g2_font_noto_semi_40_t_weather.png
	./bdfconv -v -f 1 -m '32-127,$$1f600-$$1f637>$$f600,$$1f641-$$1f64f>$$f641, $$1f910-$$1f917>$$f910, $$1f920-$$1f92f>$$f920, $$1f970-$$1f97a>$$f970, $$1fae0-$$1fae5>$$fae0, ~128561, ~128576' NotoEmoji-SemiBold-40.bdf -o u8g2_font_noto_semi_40_t_emoji.c  -n u8g2_font_noto_semi_40_t_emoji -d ../bdf/helvB18.bdf 
	convert bdf.tga u8g2_font_noto_semi_40_t_emoji.png

notob: bdfconv
	./bdfconv -v -f 1 -m '32-127,$$1f300-$$1f32c>$$f300,$$2600-$$2604,$$2614,$$26c4,$$26c5,$$26c8,$$26f1' NotoEmoji-Bold-24.bdf -o u8g2_font_noto_bold_24_t_weather.c  -n u8g2_font_noto_bold_24_t_weather -d ../bdf/helvB18.bdf 
	convert bdf.tga u8g2_font_noto_bold_24_t_weather.png
	./bdfconv -v -f 1 -m '32-127,$$1f600-$$1f637>$$f600,$$1f641-$$1f64f>$$f641, $$1f910-$$1f917>$$f910, $$1f920-$$1f92f>$$f920, $$1f970-$$1f97a>$$f970, $$1fae0-$$1fae5>$$fae0' NotoEmoji-Bold-24.bdf -o u8g2_font_noto_bold_24_t_emoji.c  -n u8g2_font_noto_bold_24_t_emoji -d ../bdf/helvB18.bdf 
	convert bdf.tga u8g2_font_noto_bold_24_t_emoji.png
	./bdfconv -v -f 1 -m '32-127,$$1f300-$$1f32c>$$f300,$$2600-$$2604,$$2614,$$26c4,$$26c5,$$26c8,$$26f1' NotoEmoji-Bold-32.bdf -o u8g2_font_noto_bold_32_t_weather.c  -n u8g2_font_noto_bold_32_t_weather -d ../bdf/helvB18.bdf 
	convert bdf.tga u8g2_font_noto_bold_32_t_weather.png
	./bdfconv -v -f 1 -m '32-127,$$1f600-$$1f637>$$f600,$$1f641-$$1f64f>$$f641, $$1f910-$$1f917>$$f910, $$1f920-$$1f92f>$$f920, $$1f970-$$1f97a>$$f970, $$1fae0-$$1fae5>$$fae0' NotoEmoji-Bold-32.bdf -o u8g2_font_noto_bold_32_t_emoji.c  -n u8g2_font_noto_bold_32_t_emoji -d ../bdf/helvB18.bdf 
	convert bdf.tga u8g2_font_noto_bold_32_t_emoji.png
	./bdfconv -v -f 1 -m '32-127,$$1f300-$$1f32c>$$f300,$$2600-$$2604,$$2614,$$26c4,$$26c5,$$26c8,$$26f1,~127747,~127751' NotoEmoji-Bold-40.bdf -o u8g2_font_noto_bold_40_t_weather.c  -n u8g2_font_noto_bold_40_t_weather -d ../bdf/helvB18.bdf 
	convert bdf.tga u8g2_font_noto_bold_40_t_weather.png
	./bdfconv -v -f 1 -m '32-127,$$1f600-$$1f637>$$f600,$$1f641-$$1f64f>$$f641, $$1f910-$$1f917>$$f910, $$1f920-$$1f92f>$$f920, $$1f970-$$1f97a>$$f970, $$1fae0-$$1fae5>$$fae0, ~128561, ~128576, ~128588' NotoEmoji-Bold-40.bdf -o u8g2_font_noto_bold_40_t_emoji.c  -n u8g2_font_noto_bold_40_t_emoji -d ../bdf/helvB18.bdf 
	convert bdf.tga u8g2_font_noto_bold_40_t_emoji.png


mystery_quest: bdfconv
	-../otf2bdf/otf2bdf -a -r 72 -p 24 -o mystery-quest.regular.bdf ../ttf/mystery-quest.regular.ttf
	./bdfconv -v -f 1 -m '32-127' mystery-quest.regular.bdf -o u8g2_font_mystery_quest_24_t.c  -n u8g2_font_mystery_quest_24_t -d ../bdf/helvB18.bdf 
	convert bdf.tga u8g2_font_mystery_quest_24_t.png
	-../otf2bdf/otf2bdf -a -r 72 -p 28 -o mystery-quest.regular.bdf ../ttf/mystery-quest.regular.ttf
	./bdfconv -v -f 1 -m '32-127' mystery-quest.regular.bdf -o u8g2_font_mystery_quest_28_t.c  -n u8g2_font_mystery_quest_28_t -d ../bdf/helvB18.bdf 
	convert bdf.tga u8g2_font_mystery_quest_28_t.png
	-../otf2bdf/otf2bdf -a -r 72 -p 32 -o mystery-quest.regular.bdf ../ttf/mystery-quest.regular.ttf
	./bdfconv -v -f 1 -m '32-127' mystery-quest.regular.bdf -o u8g2_font_mystery_quest_32_t.c  -n u8g2_font_mystery_quest_32_t -d ../bdf/helvB18.bdf 
	convert bdf.tga u8g2_font_mystery_quest_32_t.png
	-../otf2bdf/otf2bdf -a -r 72 -p 36 -o mystery-quest.regular.bdf ../ttf/mystery-quest.regular.ttf
	./bdfconv -v -f 1 -m '32-127' mystery-quest.regular.bdf -o u8g2_font_mystery_quest_36_t.c  -n u8g2_font_mystery_quest_36_t -d ../bdf/helvB18.bdf 
	convert bdf.tga u8g2_font_mystery_quest_36_t.png
	-../otf2bdf/otf2bdf -a -r 72 -p 42 -o mystery-quest.regular.bdf ../ttf/mystery-quest.regular.ttf
	./bdfconv -v -f 1 -m '32-127' mystery-quest.regular.bdf -o u8g2_font_mystery_quest_42_t.c  -n u8g2_font_mystery_quest_42_t -d ../bdf/helvB18.bdf 
	convert bdf.tga u8g2_font_mystery_quest_42_t.png

