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

10x20greek: bdfconv
	./bdfconv -f 1 -v -m '32-127,$$370-$$3ff' ../bdf/10x20.bdf -n 10x20_greek -o 10x20_greek.c -d ../bdf/7x13.bdf

polish: bdfconv
	./bdfconv -f 1 -v -m '32-127,$$104-$$107,$$118-$$119,$$141-$$144,$$15a-$$15b,$$179-$$17c,$$d3,$$f3' ../bdf/unifont.bdf -n unifont_polish -o unifont_polish.c -d ../bdf/7x13.bdf

helvb18: bdfconv
	./bdfconv -f 1 -m '32-127,x32-64,x91-96,x123-191,x247,x697-879' -p 10 ../bdf/helvB18.bdf -o helvb18_tr.c -n u8g2_font_helvB18_tr -k helvb18_tr_k.c

helvb18n: bdfconv
	./bdfconv -v -f 1 -m '32,48-57' ../bdf/helvB18.bdf -o helvb18_tn.c  -n u8g2_font_helvB18_tn -d ../bdf/helvB18.bdf 

emoticons21: bdfconv
	./bdfconv -v -f 1 -m '32-64' ../emoticons/emoticons21.bdf -o emoticons21.c  -n u8g2_font_emoticons21_tn -d ../bdf/helvB18.bdf 

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
	-../otf2bdf/otf2bdf -a -r 72 -p 33 -o tmp.bdf ../ttf/inr.otf
	./bdfconv -v -b 2 -f 1 -m '32,42-57' tmp.bdf -o u8g2_font_inr_m.c  -n u8g2_font_inr_m -d ../bdf/helvB18.bdf 

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

lubr24: bdfconv
	./bdfconv -f 1 -m '32-255' -p 10 lubR24.bdf -o lubR24.c -n u8g2_font_lubR24 -d ../bdf/helvB12.bdf 

lurs24: bdfconv
	./bdfconv -f 1 -m '32-255' -p 10 luRS24.bdf -o luRS24.c -n u8g2_font_luRS24 -d ../bdf/helvB12.bdf 



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
