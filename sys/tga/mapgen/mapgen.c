/*
  mapgen.c
  
  tile <ascii> <mapto> <top> <right> <bottom> <left>
  ":"<mapline>


    num := <hexnum> | <decnum> | <asciinum>
    asciinum := "'" <char>
    hexnum := "$" <hexdigit> { <hexdigit> }
    decnum := <decdigit> { <decdigit> }
    decdigit := "0" | "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9"
    hexdigit := "a" | "b" | "c" | "d" | "e" | "f" | "A" | "B" | "C" | "D" | "E" | "F" | <decdigit>


  The value 0 for "top", "right", "bottom" or "left" means match any.
  
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <assert.h>

#include "u8g2.h"
#include "ugl.h"

extern void u8g2_SetupBuffer_TGA(u8g2_t *u8g2, const u8g2_cb_t *u8g2_cb);
extern void tga_save(const char *name);
extern int tga_init(uint16_t w, uint16_t h);
extern u8x8_display_info_t u8x8_tga_info;


const uint8_t scrollosprites[6642] U8G2_FONT_SECTION("scrollosprites") = 
  "\323\0\3\3\5\5\5\6\6\20\20\0\0\17\0\17\1\4\11U\31\331 \6\0@\30\6!$\316"
  "\305\20>\60\16\211\243\320A\326\220,\16\220H\342\0ID<\31\217cs\330\60:\207\315\201r("
  "\0\42$\316\305\20>,\16\212\243\320A\223\304\220(\16\212\305\1\21:$\34\245\203f\341\71l\16"
  "\233\3\345P\0#\37\255\305\20>\240\16\70\213\344\0\221\34B\11\207\250\223\324\71h\16\232\203\346\240"
  "\71L\16\4$\35\214I\21\236\65\34\213\320\42\21\233\304&\261\211(!\361D\226\24\11\306\222\243\66"
  "\0%\32\314\311\20&\35`\16\206Eb;\200xx\30\22'\224!\35BG&\2&\27kI\21"
  "\26E\26y\70XB\262\231p$,YD\304\362$\12'$\316\305\20\346A\30:\304\42\7Y\34"
  "|\220\306\1\321\203T\365 \215\3\242\7\221D*\12\35d\207\31\0((\316\305\20N\344\20\211L"
  "\16\221P\344\20I\307!\16\210\36\244q@\364 G\214\34\42\221\311!\22\212\34\42\351P\0)\35"
  "\313\315\20>@\16\240\332H\242\241H\70\22U\16\223C\314J\7H.\222K\344\60*\34\316\305\20"
  "\306C\34*\71X\16\226\203\345`\71<\324\21\336QBJ\241C\345p+%\356E\20\246Y-\16"
  "\213\304DA\71($\225F\342\260HD,\221\331\346\60\363\341@\71TB\207PX\14,'\316\305"
  "\20F\70\32\211\303\302uH\64\34\7\4c\304H(\30\15e\11\305d\301\70$\32\216\3\254qX"
  "$\32\16-&\316\305\20>\60\35\22:\330\342\220`$\30\11\206\62\246\5c\301\244\214\221`$\30"
  "\207\304\16\246\70$\30\7\2.*\316\305\20F\34\32\11Fb\321H:(\222\16\212d\7D\262\211"
  "\42Y\204\307\220L\32\221D\304\63\71\340\30+E\342\320\0/&\316\305\20.\35\42\233\6c\211\321"
  "P\60\32\211F\307\261\210\34\60\222\311\1\221\220\70\24\207\245C\242B\71\204\12\60\36\314I\20\236\365"
  "\20<\4#\224\340!\250x\210Z\303\261\303Db\241Xd\42q\60\6\61)\317A\20\256\35r\210"
  "\3\16q@\204\222z\10\5\225d\207\220\320$\11\205)\221\303Pb\13IlQ\221,%\30\212\0"
  "\62*\317E\20\246\35r\10\5\17\241`\204\22\12\36BA\245\340!\24.\235e\222\310A\42\211\324"
  "\42\222HY\42\222\203\202Q\0\63+\356I\20>\64h\32\35B\261C(\26\241\204b\207PL("
  "\212\35BA\223,L:T$\23\251D\226\70\221\305\16\241\10\0\64(\356I\20\226\35d\7\34\302"
  "\251\221\210$\61b\11\206H\321p\34`\7\204c\221\303\204bI\221X\244\42\71$\30\5\65*\16"
  "J\20>L\16\225\32C\207\360!\24\213P\62\35B\61\241(v\10\5M\262\60\211d\221X%\42"
  "\71D$\7\10\205\0\66%\315I\20\236\371\20=\204R(\241\320!\24\22\212B\207P\314$\12S"
  "H\207\210Q\16\10\305l\241C(\0\67%\354I\20\316D\62;\304\221\16\61\11Et\30\261D\16"
  "\221\230mBY\221L\224$\24\221D&\222\12E\0\70\25hQ\20\26\315\24\221\204Lj\224\304\203"
  "$B$\206B\0\71\42\255I\20V\34*\36\35\206\207h\204\22=\204B\63Q$\62\21\316a\207"
  "\350!*J\215\205\202\0:\34\314I\20\236\365\20\264\4#\306\71H\221\22\244X\224e\207\250\365\70"
  "\223\303\202\0;#\316E\20\246\35p\10\37\302\22\345Cx\64\215Xb\343\311\341 \71DD\61Y"
  "\360\60<L\205\62\0<,\356E\20\246\35\20\216\306!\301\70$\30\11F\202qH\60\16\11\306!"
  "A\261\60b\211\305a\221P\70$\12\207J\42Z\204\22\3=$\316E\20\246\35p\210\36\206\22\212"
  "\360\60\212\220(\21\211EL\207\10\245\207\231\304\42\265CDrP*\0>)\317A\20\256\35r\10"
  "\37\304\22J\342A\24\211\220F\22\211MJ\223\304\202\244\320!\24:\310B\22[T$KJ\213\0"
  "?+\317E\20\246\35r\10\305\16\223\230\204\42\211\35&)$JHbI\246]g\222\310A\42\211"
  "\324\42\222HY\42\222\303Ba\0@+\357E\20>\70j\232\35B\261\303$&\241Hb\207I\12"
  "\211\22\222X\222iBa\355\60\224LdQY\352D\26<\204\42\0A'\356I\20\226\35d\7\34"
  "\302\251\221\210$\61b\11\206H\321p\34`\7\204c\221\303\204bI\264HErP\62\0B)\17"
  "F\20>P\16\26\33c\207\360a\22\223P\62\35&)$JHbI\246\11\205\65\22Qb\7\251"
  "\203\324!B!\0C%\316E\20\246\35p\210\36&!\11E\22:L$$JDbI\245)\226"
  "\330$\266\70,\24\264\305\16\241\0D'\356E\20V$\42I=\304Q\17C\11Ev(\65I,"
  "\42\21I\22\22\212B\303\231\304\42\265C\354\200C\14\0E\30\212M\20\216P\22\42\205\214\21I\320"
  "(\222Rb\301X\204\314j\2F \253I\20^\70&\224\35D\207H\12%\205\62\211L\42*\246"
  "\71\344\20;\304DiI!\0G#\316E\20\246\35p\210^\204\222ci\42\241\221$\42\71\210$"
  "\24\16\17B\211\35b\207\210\344\240T\0H%\316E\20\246\35p\210\36F\221\211d\22\71\24G\323"
  "\210%\66\236\34\16\222CD\24\223\5\17\303\303T(\3I.\360A\20\256\35\24\216\3\342\220h\34"
  "\26\14\5C\241\71\214\22\207E\222\302\241\30\221\32\42E\343\260\244p(D\246\206DqPD\22\5"
  "J\42\255E\20\246\371\20=D\17\321\220(z\10FD\222\220\304\205R\211\260E$\221\231$-$"
  "\24\1K\36\314I\20\236\65T\34I#\243`)(\33\36\242F\261\344p\261P,\62\221\70\30\3"
  "L\64\17F\20>\70\16\220FM\263C\34p\10\5C\242\304p(\70\32\205C\243\10%\244\42\11"
  "GT\42\24\231$R\213HB\322\210$I\26\222\4c\21\0M&\354I\20\246\325\22;\4\303\261"
  "\70$\24\42\245\210$\261\10%\32\14\256\204\224(\241\10%)\222\30\12\13E\0N\63\16F\20\206"
  "\34*\264\305\16\241\220)\22:\204D\341X$\16\11EB\244\224\210H\22\22\31)\224\30\205\42\216"
  "H\42\222\230$E\22\233\10C\207\30\0O+\356I\20\246\35`\7\34\302\251qH\60D\12FD"
  "\222h\204\22\7\4\343\200\10%\26\71L(\226\24\211E*\222C\202Q\0P%\317E\20\246\35r"
  "\10\37\246\22\212\360@,\21%\226\240\210\22\14\11\305\207PLR\223\216B\303\203\34`\4Q!\216"
  "E\20\226P,\224\3\312\221\210\362A\26\7\214\242\227\340!\22\213\134\242\227\350%\30\241\210\0R\36"
  "N\305\21V\34\22\13\5C\21\221\22I\251\42\222\34\36\42\222C\304B\11e\221C\3S \314\311"
  "\20\16)j\12\36C\226X\250|\210\34D\7I\304\134\212EL\301cH\222\32\251\0T,\356E"
  "\20V\210\24\214Xb!S\222)\26\42\205$!Qdr\230\304$\212\23\312$\24\22\205$\241,"
  "\241\34\343\220h\70\33\0U'\356E\20\336\34V\7E\346\240h\34\22\222\243\304AtH\70\16\260"
  "C\254\24\11\227\212X\42\12\207\42\311\221\10\0V\37\216E\20\356\371 =\10G\61\212\312\212J>"
  "I\262EF\242\231\251R\212\215#I\26\0W%\356E\20> \16\252\3\346\260\203\345p:\324!"
  "\346C\360 \214P\302\207p$\24\11\247\3\202q\20\25\0X)\356E\20\246X\30\11F\202\207\351"
  "!\24\223h;D\244\64\311$\24\211\34E\207\322\241\64\222Md\341H\60\16\11\306\1Y-\356E"
  "\20\246X\30\261\4\17\223X\204\222x\210\204\210\222\310D$\212\220D\241\12%d\24\35\42#\311l"
  "$\21\216B\62RH\26IZ&\356E\20\226\304\42\212\34\42\261\303\354P\232PF\207Jd\70Y"
  "\232\224H\347\203\360\360P\241,\221$\243H\2[(\357E\20\266$\16\272\3&\325\303\324$\225\314"
  "A\242Ix\224\26\42\311b\345\330\241h\207D\330&\223\234\242\221\24\0\134,\357E\20Fd\42\11"
  "\305$\7\351\304xI\264C\326\202#J\220\24\13EJ\243\220\71t\60\35\342\240\12MB\311\42\211"
  "FR\0]*\15J\20f(\16\241C&r\300!x\20N\306Ca\35r\20\36\42\241\203D\22"
  "\271Hb!\321,\65\24\213CbA\0^+\356E\20>,\216\42\207\312\16Q[\354\20\13\35b"
  "\241H\61\24\7\204\222\304\241\210\34\20\212\206\42\241\70,T\216\320$\241\0_*\356E\20N\34\22"
  "\15\247\312\42qXDH\212\310L\243\342\210B\31I\316\244\10K\344\62\221\204(\321Hd$\212h"
  "\231\0`*\356E\20\246\371\60;\224\206\225\221\350\20\11E\16\221P\344\20\11E\16!\321Ah\71"
  "\224H\262\230\34\64\213\210$*\23\2a \216E\20\256\35`\211\36\302Bq$\24\11\247F\202\221"
  "\330\204\62\42R$\207\210L(N\3b-\356E\20n\60\16\20\305A!\71\220\16\261\3&\21i"
  "$e\32\7\10#\261\210\60\16\220\206\203\222`D\222R\212\10kqX\14\0c(\316E\20\346$"
  "\62\215\3\344PY\60&\207\226&\222\310!\42\211\34\42\21\311!B\71D(\207\10\35\61\34\25\213"
  "\0d(\356E\20> \216\315\16\70Dd\21JDv\10\305\302\241\210\34\42\11\35b\241CLr"
  "\210I\16q\14\321\260p\4e!\356E\20\246\35b\207\330a\342\211\213H\342K\311\64\12\207&\207"
  "\10\345\20\221\35\302B\351p\4f\22\20B\20\206\344\377RG\262\374\377%\216$\371Rg\32\20B"
  "\20\206\344\377RG\222L.+\227\225\313\34<\331\377\22G\222|)h\33\20B\20\206\344\377RG"
  "\262\203N\226\223\345d\71YN\226\223%\216$\371Ri-\20B\20\206($\241\210E\242\70\262H"
  "\34\32\251\312\1r\320\70Z\34E*\241a\35:\7\211e\21\222DJ\222\203\304\62\71\204\4j-"
  "\20B\20\206($\241\210E\242\330\34Z\224Ej\304\32u*\13VG\307\311!\16\71D\244\207\310"
  "$t\10\11\357\260\222L\16!\1k-\20B\20\206\244D\221\134(\221\203\355 \21\36dB\243d"
  "B\232\215+\223\226\320\205v\220\10\17\63\311a\64:\210\204\27\231\34B\2l'\20B\20\306\303\34"
  "Ar\250H\16\25u\210v\210\246\212\246\212\246\212\246\212\246\212v\210\312\241\42\71T\344\10\17\3m"
  ":\20B\20\306\303\34Ar\250H\342\260\210$\16\213Hb\221\244\210$\24\311E\22\213$E$\241"
  "H.\222X$)\42\11Er\221\304\42I\21I\34\26\221\34*r\204\207\1n'\20B\20\306\303"
  "\34Ar\250\210\16#\331!&\64J\251b]CQa\242,\34\23\305!!I\34\26\221#<\14"
  "o\61\20B\20\306\303\34Ar\250H\306\23\311\322D\62I\232H&I\23\311$i\42\231$M$"
  "\223\244\211di\42\31O$\343\211\344P\221#<\14p!\316\305\20\306C\34*\207\312\241rP\35"
  "T\7\25)EJ\221r\210P\16\21\312!B\71\34\10q#\356\305\20>\214\16\243\303\250\24*\205"
  "J\71D(\207\10\345\20!V\210uP\35$\207\312\241\207C\0r%\316\305\20^\204\22\234P&"
  ",\207\10\345\20\241\34\42\224C\204R:P\344\300P\204r\210P\16\21v\204\7s\15\304\355\20\216"
  "\344P\341\213$)\30t&\354I\20V\60\16\11\311\1\242\60\35b\225D\206\223Lr@H,\211"
  "\34\42r\330\301r\230\304a\221\303\4\0u)\320\301\20\346!\16\210C\242Aa,\30\12F\204\254"
  "R:\264\16\243\310!\222\311a\242r\210\310\344\20\351a\16\70\4\1v\24\212M\21\226\355\20\71\30"
  "\17\207\360\341\17\22IE$\11w\31\316\305\20\266\370\360A\16;\34\352\220\303\37\346\310r\250\34*"
  "\207\212\1x\17\350P\22\216\345p\220\330a\42\211P\0y\22\216E\21\306\377\357\10\17\221IJ\313"
  "\245r\207\6z$\356E\20\306?\304\241\222t\210$Ez\70\310\241\22I\312D%er\70\310\241"
  "\22J\26\11%\313\341\20{\32\316\305\20\316\241r\370\377r\250\304\241\221C\35I\16\21\311!\42\71"
  "D\2|\32\314\311\20\236Mb\241X\346\260\310ar\370\203\35z\250C\345\20:D\0}\26\220A"
  "\21\306\377\37\352h\221|\221\344\247H\276H\362S$?~\12\10R\20\306\377\377\203\0\23\310Q"
  "\21\306\377!\16\212\244H\62ER$\231\42\31\200\32\314Q\20\306\377\207\341!\22\71\210\42\207H\344"
  " \212\34\42\221\203\360\20\4\201\33\314A\20\306\377\203\360\20\11\35$\221C$t\220D\16\221\320!"
  "x\10\36\2\202\31\314Q\21\306!x\10\36\376\17rp$\27I\236\42\271H\362\24\311\3\203\27\314"
  "A\21\346!x\370\230\203#\271H\362\24\311E\222\247H\36\204\37\14R\20\306!x\10\36\376\37"
  "\206\207H\344 \212\34\42\221\203(r\210D\16\302C\20\0\205\36\14B\20\346!x\370\30\36\42"
  "\241\203$r\210\204\16\222\310!\22:\4\17\301C\0\206\42\320A\20\306\377\37\212\207h\350\20\211H"
  "\42\207P\322!\22\221D\16\241\244C$\361\20\207\34\202\0\207\33\320A\21\346!\16\71\4\17\377?"
  "\324\321\42\371\42\311O\221|\221\344\247H~\210'\20B\20\346!\16\71\4\17\377?\24\17\321\320!"
  "\22\221D\16\241\244C$\42\211\34BI\207H\342!\16\71\4\1\211\13g\320\23V\70\22\221\205\0"
  "\212\16\307\320\22\246,\62\235CD!\31\0\213\16\246T\23\226(\42\31IB\61\21\0\214 \316\305"
  "\20.\35r\210\36f\207\322\241r\370P\254LB\221\241D\35$\207\312\241r \25\0\215\32\314\311"
  "\20\256\34D\7\330\21\255\207\330a\216r\210\35&\207:\232\34&\5\216\42\256\305\20^$\24\211\3"
  "\202\341\341\70\30\7\245RB\21J\310\24\246\3\345P\71T\16\225\3\251\0\217\24\214I\21\226\22I"
  "\35\257\42\21I\35\257\42\21I\11\0\220!\316\305\20\246\35p\10\37\204\207!\35\66\61J\16\341C"
  "Lr\220P\16\227\203\205t\231H\214\4\221\23\212\315\20\36\325v\10\35\42\207\323\321\213dF\65\1"
  "\222\36\214\311\20\256(\34\212D#\212\21J,bI\71D\42\207Q\376\64\221L\351\0\33\0\223\35"
  "\214\311\20FD\313\341\201\16>\214\42\226PD$\11\15G\303\321D\62\245\3l\0\224\34\216\305\20"
  "\306;\70\26\7\305\342\240\70\370p\210\261M\326\230#\351\240X\34\66\4\225#\256E\21n$\16\225"
  "\3\347\60:@B\11\212H\261\330\60\24\23\317\302\22alb)\35\210\7!\0\226 \314\305\20>"
  "$\16\224\4Ct\10\35`\12\215\204\243a\214\32\241Z\345 \71H\16\222\303\0\227!\256E\21."
  "\35\26\212\203\42\222\70@(\214\34\42Q;\242\310$\264C\254\221\10%Q\35`\4\230$\316\305\20"
  ">L\16\215\304\201\241\70\250\16\231\303\346\200\320\34\60\221\3\350\240;\340\20\276\3\354\220:\10\0\231"
  "#\316\305\20^\35J\221\203\352\260\71t\16\21\311\1\63\351P\70\216\315\1\241\71$\62\207\315\201r"
  "(\0\232#\314\305\20>\200\16\10J\203\221`J\64\224J\212\12c\21R:$\224\227`H\24\14"
  "\7\343\0\12\0\233\37\254I\21>D\16J\226\305!\241X-D\212\235b\246Xd\226\16\210\305\1"
  "\301h\270\12\234\30-\305\21\316!\42\71DN\23Z\34D\231\234&\207\312\241r\250\20\235#n\305"
  "\21\346$\62\215\3\204q\200\60\16\20\306\1\245\11\345\20\241\34\42\224C\204r\210P\16\21\12\0\236"
  "#\316\305\20\246\35\20N\22E\305\62\211EB>\34(\207J\344\60\221\331\42\311\221\20EB;\214"
  "\215\0\237 \216\305\20\356\34\32\31\206B\264\71H(\26E$\61:\350\30:\204(\242QUr\7"
  "\24\1\240\27\14I\22\316\34R\7\304\342\200\330Av\220\5\17\301Hd\16\1\241\30\15E\22\246\34"
  "\26\212\3F\341\320\201t\230\214b\263P,\22\224\3\242\27k\311\20^U&\214M\253i\222\10e"
  "\22\71\204\16\17\246\33\0\243\37\256\305\20^\35r\7\330A\224Z\360\66\271C\352\220h\34R\252D"
  "\17\221\312\35R\207\0\244\26J\315\21\226\315\22\71D,\22\312h\351&\21\245\204c&\0\245\27J"
  "\315\21\246-R\212PRH\22\232d(\21F\302\241h\314\10\246\27J\315\21\226-\62\212DJ\222"
  "\222\244$)\211frHr\314\4\247\30\214I\21n\371,\22\213\244\21\362XQ$\207\310!\42\331"
  "\270\16\21\250\17\314\310\22>\214\16\71\230\16v\10\35\6\251\27k\311\21V\365v\20\225D\224\210\210"
  "\62\211\20\17\17\224\303\354\2\252\30K\311\21\216QV\21\306h\242\310\201\26I\241C(Y\16\224\303"
  "\0\253$\316\305\20>L\16\245\303\350 \71P\16\242C(\322\211\34\20!EMQ\211(L\213\3"
  "dq\20\35\2\254#\316\305\20\16\35\24\224\3\302\322\70D\26\207\205b\323H,\24\25N\345\320\71"
  "\354\360Ar\250\35F\0\255%\316\305\20>\70\16\231\304!u\350x&-\311!\223\340LZ\22\306"
  "&\321\240\34\24\222CbqHH\16\2\256\37\216\305\20>L,\222\10%\25Y\304\32\261\36\244\7"
  "aez\220\306(\62\321DJ\7\1\257\37\254\311\20&\371\20\223\225\42\42\212\322uB\233D\16\25"
  "\313\350\24;D\202\222X\34\26\1\260&\316\305\20.\35&\221C\354\200\203\220b\212X\16\222\203\344"
  "\20\23\31#!Ih\26\22\305\1\301\70(\35(\7\261\27\212M\21\36\325v\10Z&\241\313Ar"
  "\230\34$\207\313!f\2\262\31L\311\21\336\34\62\221\36D\207\213\354 ;\310&\241C$t\70\214"
  "\4\263\26\212M\21\226\34\62\207\210%\25\222\371\360\201r\10\35b&\0\264\31\256E\21>@\16\224"
  "C\345 \241pBY\361\277L$\26\251\35D\5\265\42\314\311\20fl$\221\214F\21I\204|\70"
  "D*\7\213\344\60\233\220\351 \71H\22\207d\7E\1\266\33n\305\21>\70\16\230\210)b\312\224"
  "B\235P\17\322C\364\20\266\3\352 \0\267\37\314\311\20\356\34R\224T$\323\221TZ\207\314\61\204"
  "e\341I\34\42\211\303\342\300\70\4\0\270\27kI\21\336\351\360\60\246\211$\222\211t\16\231\3\350\0"
  ":D\12\271\42\315\305\20>H\16\234\3$T\312\320|\10^\202\21R\60$\213\3c\323 -<"
  "\221C\344 \0\272$\314\311\20> \16\222\203\310\207\330D\222\242t\270H&\222I\344\20I\207\204"
  "&I\241IRL\222\325\6\273(\316\305\20F\34\355|\20\36&\241\303\360@<DD\61Y$\64"
  "\13EB\223\244Hh\222$\233$\205$\322\240\304\10\274\32\214\311\20\236U\250\355\60\221X\312\207\7"
  "\313a\22\71D\322!\261C\10\0\275#\316\305\20.\35\244\16\261N(\223\242\351\20\22\35B\223C"
  "Dr\250\35\246\207\70\204\16\12\306AT\0\276\22h\321\20\26M$\262\321\202\221!\307H\60F\2"
  "\277\35\254\311\20&\35 \22\333\1t@\60:\214\215#t\0\35B\207\314A\21\261\320\6\300\32\314"
  "\311\20&\35 \22\33%\24I\34F\207\230\257_#\65\241H\16\221\2\301\27J\315\21>D\246\66"
  "\211\3\304\22!E\64\211H&QIx\16\302&\316\305\20>\214\16\252C(q\0)L\12\223\302"
  "\244\240d\24\225\210\342\0\311\34$\7F\346\0\231\34 \207\2\303 \216E\21>,\16\215\304a\351"
  "\220\240\34\20\233FBSId*\242\312\246B\251\34*\207\2\304$\316\305\20\366!l\211\3Hq"
  "\210,\16\212\305\1\241\344Q$:\24\316as\330\34\66\207\315\201r(\0\305 \316\305\20>T\16"
  "\234\303\346\260\71P\16\213C\347\260\71P\16\213C\347\260\71l\16\224C\1\306\27\214I\21v\371z"
  ",\5i\61\232l\64\273\226\351\200\71H\16\3\307 \316\305\20>@\16\14E\304\301i\70u\34+"
  "G\354\0\253\210:\223\316as\330\34(\207\2\310\35\214I\21\326!v\230\34D\7\231\271\42\254\10"
  "\353\0:d\16\212\304!\261C\10\0\311\27\314\311\20\206\34b<\230\256\257\25aEh\276\276Fj"
  "B\33\0\312&\316\305\20\206\34v\230FI\261\340$\32\214\304\1\301\210\70\24\241\3b\343\250\64\24"
  "\314\26\207\205\342\300\210\34*\313(\316\305\20>H\42\207\205\342\220P\34\224\42\16\305\242\241\344P\252"
  "(U\42\212J&q\310$\16\213\310\201rh\34\4\314\23J\315\21\226\355\20\71|\20\12c\352\220"
  "\261D(\1\315\27L\311\21\326!v\30\35F\207\321a\24\207\204\342\220\20/D\2\316\31N\305\21"
  ">L\16\233\304!\224\70\240\16\261\3\316\207\70\200L$\23\5\317&\316\305\20>(\22\7F\322A"
  "\221\244H(bI:D\322\202\226\20\345\61d\241\5o\24;$\30\7\321A\0\320%\216E\21>"
  "(\42\7M\344\20\311$\42\233TD\222\312hR\21\5'\243 EF\11\306!\301\70\210\16\2\321"
  "\27N\305\21N\251;\32\211H\42\213\244\223Pdb:\224\16\304\2\322\32LI\22\6EB\207F"
  "\16\21\311!\22\71L\210\207\340!x\10\36\202\4\323\30\356D\22\316\241\22\207V(\207H(r\210"
  "H\42\224I(\62\246\2\324\33nE\21\236P*\241\310\306\223\342\201t(\331Fs\200\35\20\216\222"
  "\210K\0\325\31L\311\21\6\361\20\221\34\202\224\341l\64\213\311B\207\321a\216r\10\1\326\32l\311"
  "\21&\35\20\214\21/\241\310Er\20\35\16\23\22Y|\230\35B\0\327\37\314\311\20\246\325z\10\206"
  "cqH(\16\11\305!\261p\64\30\234H&\304C\360\134\207\10\330\30kM\21\316\32\205D\251L"
  "d\252e:\200\16\210\210c\302h\15\0\331\37\254\311\20\216X\22\62\311ar\330\34\242B\21\216\242"
  "\245`)X\12Fd\321`\34@\4\332\20\214H\23FP\70\42M\222\42)\207\11\0\333\21\256\304"
  "\22\316\341\60\233\211b\263,\261\340l\2\334\33\216E\21\246YM\16Q\233\204HJ$%\222\22I"
  "\211\66!\311\301\312F\0\335\32\214I\21\256\34D\7X\17\261\303\344P\244C\350\20:\204\16\241C"
  "\210\0\336\26kI\21\326Av\10^\215\267C\250\62\251]#sP\34\2\337\15J\315\21\36\231\355"
  "\360C\215\314\6\340\22J\315\21\206\270\70\31\315\254d\252m\64\31\226\5\341\23L\311\21\16\211r\370"
  "\203\345\60;D\355\0:H\12\342\35\256E\21\266\34*\7\322a\324\303\345P;L\17\321\303\360\60"
  "+\225\306\23\71L\0\343%\316\305\20\266\34 \207\210B\246\340!z\30\36F\221\303Dr\230\204\16"
  "\303\303\364\20\14\231Br\210\34 \6\344\21\212M\21N\361\20\265\226\253\35\213\226C\254\10\345 \316"
  "\305\20.\35r\210\36fD\322x\62\223\231H&\222M\66\31\217\210\264\303\364\20\207P\1\346\35\256"
  "E\21\266\34H\207\321Av\210H\16\30\215GS\22\221D;\224ZL\207\203\1\347\35\253I\21>"
  "H\16\240\332\16\261Rl\30\13\307b\222X\314\22;\4%t\210\34\2\350%\316\305\20v\34\24\23"
  "\5E\24\231\350\20\12\35\204\7\312\341@\222\234b\22J\34`\7I\344\240\230Xl\4\351#\256\305"
  "\20VT\30I\223\316a\221\344`\70\222\16\233\303\42\351\300\70$\16\214\244\303\346\260H:\60\16\352"
  "\35\312\315\20\246\34\20\12\207\242\243\340(V\12\225\42\266\233d(\207\310!\311\61\23\0\353\23\252\315"
  "\20\336\315\261\365p\240\222\327\1r\200\34\22\7\354!\316\305\20.\35r\210\36fD\322x\62\223\231"
  "\222L\21\221IF\221\235f\305\71t\16\236\4\1\355$\316\305\20>\240\16\71\37\244\7i\244\22\215"
  "\3\42\243h\244T\232\304A\261\70l&\7\322a\241\70P\12\356\36\316\305\20\366!|\210\303\346\260"
  "Qm\34\33\307\16\221\340\301\216\3\35\32\207\306\241T\0\357\33\312\315\20\226\355\20\71<\204D\63m"
  "\221\321\220\30\221$\205C\22\65+\15\0\360 \316\305\20\306\34r>\10+\245\332\250\32\252C\352\220"
  ":\244\24\64\221\332\206\263\210P\222\16\15\361$\314\311\20\316a\22\207I\250\22\252\204*\241J\250\22"
  "\252\204\32\211\14c\21Y\64\22\212\3Bq\220\24\0\362\35\312\315\20\306\203%\34\12\207\302\261`\64"
  "\24\16E\203\261\220(\205\22:D\16\7\3\0\0\0";



struct tile_struct
{
  int ascii;
  int map_to;
  int condition[4];  
  int item_index;		/* reference into item_list */  
};
#define TILE_MAX 4096
struct tile_struct tile_list[TILE_MAX];

int tile_cnt = 0;

#define ITEM_IDENTIFIER_MAX 256

/* global item templates */
struct item_struct
{
  char name[ITEM_IDENTIFIER_MAX];
  uint8_t fg_tile;	/* for tga output and inital tile, bg-tile is set via tile_list */
  uint16_t init_proc;
  uint16_t hit_proc;
  uint16_t step_proc;
};

struct item_struct item_list[256];
int item_cnt;

#define MAP_SIZE_X 1024
#define MAP_SIZE_Y 1024
#define MAP_LINE_MAX 4096


#define PHASE_MAPDATA 0
#define PHASE_MAPSTRUCT 1

int map_phase;
uint8_t map[MAP_SIZE_Y][MAP_SIZE_X];
uint8_t map2[MAP_SIZE_Y][MAP_SIZE_X];
int map_curr_line = 0;
char map_name[MAP_LINE_MAX];
long map_width = 0;
long map_height = 0;
uint16_t map_init_code_pos;


FILE *map_fp;
char map_line[MAP_LINE_MAX];

FILE *out_fp;


/*============================================*/
int item_get_idx_by_name(const char *name)
{
  int i;
  for( i = 0; i < item_cnt; i++ )
  {
    if ( strcmp(item_list[i].name, name) == 0 )
      return i;
  }
  return -1;
}

int item_add(const char *name)
{
  if ( item_cnt < 256 )
  {
    strcpy(item_list[item_cnt].name, name);
    item_cnt++;
    return item_cnt-1;
  }
  return -1;
}

void item_write(FILE *fp)
{
  int i;
  fprintf(fp, "item_template_t item_template_list[] = {\n");
  for( i = 0; i < item_cnt;i++ )
  {
    fprintf(fp, "  { /* init= */ %u, /* hit= */%u, /* step= */ %u, /* fg= */ 0x%02x}", item_list[i].init_proc, item_list[i].hit_proc, item_list[i].step_proc, item_list[i].fg_tile);    
    if ( i != item_cnt-1 )
	fprintf(fp, ",\n");
    else
 	fprintf(fp, "\n");
      
  }
  
  fprintf(fp, "};\n\n");
}



/*============================================*/

static void skip_space(const char **s)
{
  for(;;)
  {
    if ( **s == '#' )
    {
      while( **s != '\0' )
	(*s)++;
      break;
    }
    if ( **s == '\0' )
      break;
    if ( **s > ' ' )
      break;
    (*s)++;
  }
}

static long get_dec(const char **s)
{
  long v = 0;
  for(;;)
  {
    if ( (**s) >= '0' && (**s)  <= '9' )
    {
      v*=10;
      v+= (**s)-'0';
      (*s)++;
    }
    else
    {
      break;
    }
  }  
  skip_space(s);
  return v;
}

static long get_hex(const char **s)
{
  long v = 0;
  for(;;)
  {
    if ( (**s) >= '0' && (**s)  <= '9' )
    {
      v*=16;
      v+= (**s)-'0';
      (*s)++;
    }
    else if ( (**s) >= 'a' && (**s)  <= 'f' )
    {
      v*=16;
      v+= (**s)-'a'+10;
      (*s)++;
    }
    else if ( (**s) >= 'A' && (**s)  <= 'F' )
    {
      v*=16;
      v+= (**s)-'A'+10;
      (*s)++;
    }
    else
    {
      break;
    }
  }
  skip_space(s);
  return v;
}

static long get_ascii(const char **s)
{
  long v = 0;
  v = **s;
  (*s)++;
  skip_space(s);
  return v;  
}


static long get_num(const char **s)
{
  if ( (**s) == '$' )
  {
    (*s)++;
    return get_hex(s);
  }
  if ( (**s) == '\'' )
  {
    (*s)++;
    return get_ascii(s);
  }
  
  return get_dec(s);
}

static const char *get_identifier(const char **s)
{
  static char buf[MAP_LINE_MAX];
  int c;
  int i = 0;
  buf[0] = '\0';
  for(;;)
  {
    c = **s;
    if ( c < 'A' )
      break;
    if ( i >= MAP_LINE_MAX-2 )
      break;
    buf[i++] = c;
    buf[i] = '\0';
    (*s)++;
  }
  skip_space(s);
  return buf;
}

/*============================================*/



int get_tile_idx_by_ascii(int ascii)
{
  int i;
  for( i = 0; i < tile_cnt; i++ )
  {
    if ( tile_list[i].ascii == ascii )
      return i;
  }
  return -1;
}

/*============================================*/
/* map a tile from map[][] to map2[][] */
/* called by map_all_tile */
int map_tile(int x, int y)
{
  int ascii, i, j;
  int cond[4];
  int is_condition_match;
  //int is_simple_match;
  int condition_match_cnt;
  int condition_match_max;
  int i_best;
  
  /* get the ascii version */
  ascii = map[y][x];
  cond[0] = 32;
  cond[1] = 32;
  cond[2] = 32;
  cond[3] = 32;
  
  if ( y > 0 ) cond[0] = map[y-1][x];
  if ( x+1 < map_width ) cond[1] = map[y][x+1];
  if ( y+1 < map_height ) cond[2] = map[y+1][x];
  if ( x > 0 ) cond[3] = map[y][x-1];
  
  /* find matching tile */
  condition_match_max = -1;
  i_best = -1;
  for( i = 0; i < tile_cnt; i++ )
  {
    if ( tile_list[i].ascii == ascii )
    {
      is_condition_match = 1;
      //is_simple_match = 1;
      condition_match_cnt = 0;
      for( j = 0; j < 4; j++ )
      {
	if ( tile_list[i].condition[j] != 0 )
	{
	  //is_simple_match = 0;
	  if ( tile_list[i].condition[j] != cond[j] )
	  {
	    is_condition_match = 0;
	  }
	  else
	  {
	    condition_match_cnt++;
	  }
	}
      }
      if ( is_condition_match )
      {
	if ( condition_match_max < condition_match_cnt )
	{
	  condition_match_max = condition_match_cnt;
	  i_best = i;
	}
      }
    }
  }
  if ( i_best < 0 )
  {
    printf("no tile mapping found for '%c' (x=%d, y=%d)\n", ascii, x, y);
    return 0;
  }
  //printf("tile mapping '%c' --> $%02x (x=%d, y=%d)\n", ascii, tile_list[i_best].map_to, x, y);
  map2[y][x] = tile_list[i_best].map_to;
  return 1;
}

int map_all_tiles(void)
{
  int x, y;
  for( y = 0; y < map_height; y++ )
    for( x = 0; x < map_width; x++ )
      if ( map_tile(x,y) == 0 )
	return 0;
  return 1;
}

/*============================================*/

void clear_map(void)
{
  int x, y;
  for( y = 0; y < MAP_SIZE_Y; y++ )
    for( x = 0; x < MAP_SIZE_X; x++ )
      map[y][x] =32;
  map_curr_line = 0;
}

void write_map(void)
{
  int x, y;
  
  if ( map_phase == PHASE_MAPDATA )
  {
    if ( out_fp != NULL )
    {      
      fprintf(out_fp, "unsigned char map_%s[%ld] = \n", map_name, map_height*map_width);
      for( y = 0; y < map_height; y++ )
      {
	fprintf(out_fp, "    \"");
	for( x = 0; x < map_width; x++ )
	{
	  fprintf(out_fp, "\\x%02x", map2[y][x]);
	}
	fprintf(out_fp, "\"");
	if ( y+1 < map_height )
	  fprintf(out_fp, "\n");
	else
	  fprintf(out_fp, ";\n");
      }
      fprintf(out_fp, "\n");
    }
  }
}

void write_map_struct(void)
{
  if ( map_phase == PHASE_MAPSTRUCT )
  {
    if ( out_fp != NULL )
    {
      fprintf(out_fp, "  { ");
      fprintf(out_fp, "map_%s, ", map_name);
      fprintf(out_fp, "item_onmap_%s, ", map_name);
      fprintf(out_fp, "/* map init code */ %u, ", map_init_code_pos);
      fprintf(out_fp, "ITEM_ONMAP_%s_CNT, ", map_name);      
      fprintf(out_fp, "/* width= */ %ld, ", map_width);
      fprintf(out_fp, "/* height= */ %ld ", map_height);
      fprintf(out_fp, " },");
      fprintf(out_fp, "\n");
    }
  }
}

void write_tga_map(const char *filename)
{
  static u8g2_t u8g2;
  int x, y, i;
  unsigned tile;
  if ( map_phase != PHASE_MAPDATA )
    return;
  
  u8x8_tga_info.tile_width = map_width*2;
  u8x8_tga_info.tile_height  = map_height*2;
  u8x8_tga_info.pixel_width = map_width*16;
  u8x8_tga_info.pixel_height  = map_height*16;
  
  u8g2_SetupBuffer_TGA(&u8g2, &u8g2_cb_r0);
  u8x8_InitDisplay(u8g2_GetU8x8(&u8g2));
  u8x8_SetPowerSave(u8g2_GetU8x8(&u8g2), 0);  
  u8g2_SetFont(&u8g2, scrollosprites);
  
  
  u8g2_FirstPage(&u8g2);
  do
  {
    for( y = 0; y < map_height; y++ )
    {
      for( x = 0; x < map_width; x++ )
      {
	tile = map2[y][x];
	/* check if there is a fg_tile, if so, use that tile instead */
	for( i = 0; i < tile_cnt; i++ )
	{
	  if ( tile_list[i].item_index >= 0 )
	  {
	    if ( tile_list[i].ascii == map[y][x] )
	    {
	      tile = item_list[tile_list[i].item_index].fg_tile;
	    }
	  }
	}
	
	u8g2_DrawGlyph(&u8g2, x*16, y*16+16, tile);
      }
    }
    
  } while( u8g2_NextPage(&u8g2) );
    
  tga_save(filename);
  
}

static void write_item_onmap(void)
{
  int x, y, i;
  int is_first = 1;
  int cnt = 0;
  /* 
    pseudo code:
    with all tiles on the map
      if the tile is connected to an item
	output an item onmap entry
  */
  if ( map_phase == PHASE_MAPDATA )
  {
    if ( out_fp != NULL )
    {
      fprintf(out_fp, "item_onmap_t item_onmap_%s[] = {\n", map_name);
      for( y = 0; y < map_height; y++ )
      {
	for( x = 0; x < map_width; x++ )
	{
	  for( i = 0; i < tile_cnt; i++ )
	  {
	    if ( tile_list[i].item_index >= 0 )
	    {
	      if ( tile_list[i].ascii == map[y][x] )
	      {
		if ( is_first == 0 )
		{
		  fprintf(out_fp, ",\n");
		}
		is_first = 0;
		fprintf(out_fp, "  ");
		fprintf(out_fp, "{ /*x=*/ %d, /*y=*/ %d, /*idx=*/ %d, %d}", x, y, tile_list[i].item_index, 0);
		cnt++;
	      }
	    }
	  }
	}
      }
      fprintf(out_fp, "\n};\n");
      fprintf(out_fp, "#define ITEM_ONMAP_%s_CNT %d\n\n", map_name, cnt);
      
    }
  }
}

int map_read_tile(const char **s, int is_overwrite, int item_index)
{
  long ascii;
  int idx, i;
  
  ascii = get_num(s);
  
  if ( is_overwrite )
  {
    for( idx = 0; idx < tile_cnt; idx++ )
    {
      if ( tile_list[idx].ascii == ascii )
	break;
    }
  }
  else
  {
    idx = tile_cnt;
  }
  
  if ( idx < tile_cnt )
  {
    for( i = 0; i < 4; i++ )
    {
      tile_list[idx].condition[i] = 0;
    }
  }
  else
  {
    
    if ( tile_cnt >= TILE_MAX )
    {
      printf("max number of tiles reached\n");
      return 0;
    }
    idx = tile_cnt;
    tile_cnt++;
  }
  
  tile_list[idx].ascii = ascii;
  tile_list[idx].item_index = item_index;
    
  tile_list[idx].map_to = get_num(s);
  for( i = 0; i < 4; i++ )
  {
    tile_list[idx].condition[i] = get_num(s);
  }
  
  //printf("[%d] tile %c: ", idx, (int)ascii);
  //printf("map to $%02x\n", tile_list[idx].map_to);
    
  return 1;
}


/*============================================*/
/* read one row of the game map */


int map_read_row(const char **s)
{
  int x = 0;
  //printf("line %d\n", map_curr_line);
  while ( **s >= ' ' )
  {
    if ( x > map_width )
    {
      printf("map '%s': Row '%d' too long\n", map_name, map_curr_line);
      return 0;
    }
    //printf("%d ", **s);
    map[map_curr_line][x] = **s;
    (*s)++;
    x++;
    
  }
  map_curr_line++;
  return 1;
}


/*============================================*/
/* read a command of the map file */


int map_read_map_cmd(const char **s)
{
  
  /* get new map */
  strcpy(map_name, get_identifier(s));
  map_width = get_num(s);
  map_height = get_num(s);

  printf("map '%s' (%ld x %ld)\n", map_name, map_width, map_height);
  
  map_init_code_pos = 0;
  clear_map();
  return 1;
}


int is_inside_proc;
int is_inside_map;

int map_read_line(const char **s)
{
  const char *id;
  
  if ( is_inside_proc != 0 )
  {
    if ( uglReadLine(s) == 0 )
      is_inside_proc = 0;
    return 1;
  }
  skip_space(s);
  
  if ( **s == '#' )		/* comment (hmm handled by skip_space) */
    return 1;

  if ( **s == '\0' )		/* empty line */
    return 1;
  
  if ( **s == ':' )
  {
    (*s)++;
    return map_read_row(s);
  }
  
  id = get_identifier(s);
  if ( strcmp(id, "tile") == 0 )
  {
     return map_read_tile(s, 0, -1);
  }
  else if ( strcmp(id, "thing") == 0 )
  {
     return map_read_tile(s, 1, -1);
  }
  else if ( strcmp(id, "itemkey") == 0 )
  {
    const char *id;
    int idx;
    id = get_identifier(s);
    idx = item_get_idx_by_name(id);
    if ( idx < 0 )
    {
      printf("code line %d, item '%s' unknown.\n", ugl_current_input_line, id);
    }
    else
    {
     return map_read_tile(s, 1, idx);
    }
  }
  else if ( strcmp(id, "item") == 0 )
  {
    const char *id;
    int idx;
    id = get_identifier(s);
    idx = item_get_idx_by_name(id);
    if ( idx >= 0 )
    {
      printf("code line %d, item '%s' already exists.\n", ugl_current_input_line, id);
    }
    else
    {
      if ( item_add(id) < 0 )
      {
	printf("code line %d, item '%s': Too many items.\n", ugl_current_input_line, id);
      }
      else
      {
	idx = item_get_idx_by_name(id);
	assert( idx >= 0 );
	item_list[idx].fg_tile = get_num(s);
      }
    }  
    
    return 1;
  }
  else if ( strcmp(id, "mapinit") == 0 )
  {
    map_init_code_pos = uglStartNamelessProc(0);
    is_inside_proc = 1;
    return 1;
  }  
  else if ( strcmp(id, "iteminit") == 0 )
  {
    const char *id;
    int idx;
    uint16_t code_pos;
    
    id = get_identifier(s);
    idx = item_get_idx_by_name(id);
    code_pos = uglStartNamelessProc(0);
    if ( idx < 0 )
    {
	printf("code line %d, item '%s' not found.\n", ugl_current_input_line, id);
    }
    else
    {
      item_list[idx].init_proc= code_pos;
    }    
    is_inside_proc = 1;
    return 1;
  }
  else if ( strcmp(id, "itemhit") == 0 )
  {
    const char *id;
    int idx;
    uint16_t code_pos;
    
    id = get_identifier(s);
    idx = item_get_idx_by_name(id);
    code_pos = uglStartNamelessProc(0);
    if ( idx < 0 )
    {
	printf("code line %d, item '%s' not found.\n", ugl_current_input_line, id);
    }
    else
    {
      item_list[idx].hit_proc= code_pos;
    }    
    is_inside_proc = 1;
    return 1;
  }
  else if ( strcmp(id, "itemstep") == 0 )
  {
    const char *id;
    int idx;
    uint16_t code_pos;
    
    id = get_identifier(s);
    idx = item_get_idx_by_name(id);
    code_pos = uglStartNamelessProc(0);
    if ( idx < 0 )
    {
	printf("code line %d, item '%s' not found.\n", ugl_current_input_line, id);
    }
    else
    {
      item_list[idx].step_proc= code_pos;
    }    
    is_inside_proc = 1;
    return 1;
  }
  else if ( strcmp(id, "map") == 0 )
  {
    is_inside_map = 1;

    return map_read_map_cmd(s);
  }
  else if ( strcmp(id, "endmap") == 0 )
  {
    /* write existing map: once a map has been read, transform it to the target */
    if ( map_width > 0 && map_height > 0 )
    {
      if ( map_all_tiles() )
      {
	char buf[128];
	write_map();
	write_item_onmap();
	sprintf(buf, "%s.tga", map_name);
	write_tga_map(buf);
	write_map_struct();
      }
    }
    is_inside_map = 0;
    return 1;
  }
  else
  {
    printf("code line %d, map line %d: unkown command '%s'\n", ugl_current_input_line, map_curr_line, id);
  }
  
  return 1;
}

int map_read_fp(void)
{
  const char *s;
  ugl_InitBytecode();
  if ( map_phase == PHASE_MAPDATA )
    ugl_is_suppress_log = 0;
  if ( map_phase == PHASE_MAPSTRUCT )
    ugl_is_suppress_log = 1;
  
  ugl_current_input_line = 1;
  tile_cnt = 0;
  item_cnt = 0;
  
  for(;;)
  {
    if ( fgets(map_line, MAP_LINE_MAX, map_fp) == NULL )
      break;
    s = &(map_line[0]);
    if ( map_read_line(&s) == 0 )
    {
      if ( is_inside_proc )
	printf("endproc missing\n");
      if ( is_inside_map )
	printf("endmap missing\n");
      return 0;
    }
    ugl_current_input_line++;
  }
  ugl_ResolveSymbols();
  return 1;
}

int map_read_filename(const char *name, int phase)
{
  map_phase = phase;  
  map_fp = fopen(name, "r");
  if ( map_fp == NULL )
    return 0;
  printf("file '%s'\n", name);
  if ( map_read_fp() == 0 )
    return fclose(map_fp), 0;
  fclose(map_fp);
  return 1;
}

int main(int argc, char **argv)
{
  const char *filename = NULL;
  const char *outfile = NULL;
  if ( argc <= 1 )
  {
    printf("%s [options] mapfile\n", argv[0]);
    printf("  -o c-file-name\n");
    exit(1);
  }
  argv++;
  argc--;
  
  while( argc > 0 )
  {
    if ( strcmp(*argv, "-o" ) == 0 )
    {
      argv++;
      argc--;
      if ( argc <= 0 )
      {
	printf("output file missing\n");
	exit(1);
      }
      outfile = argv[0];
    }
    else
    {
      filename = argv[0];
    }
    argv++;
    argc--;
  }
  
  clear_map();
  
  if ( filename != NULL  )
  {
    out_fp= NULL;
    if ( outfile != NULL )
    {
      out_fp = fopen(outfile, "w");
      printf("output file %s\n", outfile);
      fprintf(out_fp, "/* %s generated by mapgen, olikraus@gmail.com */\n", outfile);
      fprintf(out_fp, "\n");
      fprintf(out_fp, "#include \"map.h\"\n");
      fprintf(out_fp, "\n");
    }
    map_read_filename(filename, PHASE_MAPDATA);

    ugl_WriteBytecodeCArray(out_fp, "map_code");
    
    //write_item_onmap();
    
    item_write(out_fp);
    
    if ( out_fp != NULL )
    {
      fprintf(out_fp, "map_t map_list[] = {\n");
      map_read_filename(filename, PHASE_MAPSTRUCT);
      fprintf(out_fp, "};\n");
      fprintf(out_fp, "\n");
      
    }
    if ( out_fp != NULL )
      fclose(out_fp);
  }
}
