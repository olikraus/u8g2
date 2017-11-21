/*

  golem master
  
  
  
  event language
  if cond then action
  if obj in [equip, inventory, map] move to [hand, shoe, body, head, inventory, map]

  cond:
    1. part:
      enter tile					$01		(!BLOCKABLE, !MOVEABLE)
      leave tile					$02		(!BLOCKABLE, !MOVEABLE)
      hit tile					$03		(BLOCKABLE, MOVEABLE)
      use tile (only for inventory)	$04		
    2. AND part
      flag is clear				$01	$xx
      flag is set					$02	$xx
      item part of inventory		$03  $xx
      item not part of inventory		$04	$xx
  action
      set flag to zero				$01	$xx
      set flag to one				$02	$xx
      add item to inventory			$03	$xx
      delete item from inventory		$04	$xx
      create obj					$06	<objnum>
      remove obj					$07 <objnum>
      add curr obj to Y			$1y			y=0: gold, y=1:health, y=2: attack, y=3:defense
      equip curr obj to z			$2z			z=0:right hand, z=1:left hand z=2:shoe, 4: body 5:head
      
      message					$0f	<len> 
      
  object
      obj base info			$01 <objnum> <tile> <mode>
      default position			$02 <x> <y>
      when equiped add X to Y	$1y	X			y=2: attack, y=3:defense
      
      
    for each action of the player,there is a procedure
      enter tile on map
      leave tile on map
      hit tile on map
      use tile from inventory to tile on map
      equip tile from inventory
      
      if  cond cond cond ... then action action ... else action action action... endif
      
      if $f0 
      then $f1
      else $f2
      endif $f3
      
      condition
	flag is set
	flag is clear
	target map obj is equal to <num>
	
      action
         
      
      
*/



#include "u8g2.h"
#include <stdio.h>
#include <unistd.h>


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


/*
maze_Redraw()
maze_SetData()
maze->px		upper left corner (pixel pos)
maze->py
maze_SetPixelPos()
maze_GetTile(tx, ty)

gm_SetTilePos(tx, ty)
gm_GetCurrTile()
gm_GetNeighborTile(dir)		dir=0,1,2,3
gm->state
gm->tx, ty
gm->walk_dir
gm->px_delta, py_delta
*/

struct v16_struct
{
  uint16_t v[2];
};
typedef struct v16_struct v16_t;


/*=================================================*/
struct map_struct
{
  /* input: upper left corner of the visible area in the map */
  /* visible window position in pixel coordinates (vwpp)  */
  v16_t vwpp;

  /* input: dimensions of the visible window (pixel) */
  uint8_t pdw;
  uint8_t pdh;	
  
  /* input: offset for the visible window on the display from the upper left display corner */
  v16_t vis_win_disp_pos_pix;
  
  
  /* upper left visible tile of the visible window, derived from vwpp.v[0], vwpp.v[1] */
  /* visible window position in tile coordinats (vwpt)  */
  v16_t vwpt;
  
  /* visible area in tiles, derived from pdw/pdh */
  uint8_t tmw;
  uint8_t tmh;

  
  /* offset from the upper left tile corner to vwpp.v[0]/vwpp.v[1] */
  v16_t dtwp;

};
typedef struct map_struct map_t;

/*=================================================*/

struct gm_struct
{
  /* tile position of golem master (upper left corner) */
  v16_t pt;
  
  
  uint8_t state;
  uint8_t dir;		/* for GM_STATE_READY_FOR_WALK */
  
  /* target offset of the visible window in map pixel */
  v16_t twop;
  
  /* this is the current offset, which follows the target offset */
  v16_t cwop; 	/* current window offset in pixel */
  
  /* golem master offset pixel */
  v16_t gmop;
  
};
typedef struct gm_struct gm_t;

#define GM_STATE_CENTER 0
#define GM_STATE_READY_FOR_WALK 1

#define GM_OFFSET (0x040)

/*=================================================*/

struct obj_struct
{
  v16_t pos_t;		/* tile position of the object */
  v16_t offset_p;	/* pixel offset of the object, used for animation, this will decrease to 0 */

  uint8_t tile;		/* glyph index, shape of the tile, 0 means empty */
  
  uint8_t health;	/* current health of the object */
  uint8_t attack;	/* attack value of the object, 0 if it does not attack */
  uint8_t defense;	
  uint8_t mode;
};
typedef struct obj_struct obj_t;


#define OBJ_MODE_INIT_AT_MAPSTARTUP 1
#define OBJ_MODE_ATTACK_PLAYER 2
#define OBJ_MODE_ATTACK_MONSTER 4
#define OBJ_MODE_MOVABLE 8
#define OBJ_MODE_BLOCKABLE 16

/*=================================================*/

#define OBJ_LIST_MAX 16

obj_t obj_list[OBJ_LIST_MAX];
uint8_t obj_cnt = 0;

void obj_list_Init(void)
{
  uint8_t i;
  for( i = 0; i < OBJ_LIST_MAX; i++ )
    obj_list[i].tile = 0;			/* mark as empty */
}

/* 255 means "not found" */
uint8_t obj_list_GetEmpty(void)
{
  uint8_t i;
  for( i = 0; i < OBJ_LIST_MAX; i++ )
    if ( obj_list[i].tile == 0 )
      return i;
  return 255;
}

/*=================================================*/

void v16_SetByConstant(v16_t *v, uint16_t x, uint16_t y )
{
  v->v[0] = x;
  v->v[1] = y;
}

void v16_SetByV16(v16_t *v, v16_t *w )
{
  v->v[0] = w->v[0];
  v->v[1] = w->v[1];
}

void v16_RightShift(v16_t *v, uint8_t s )
{
  v->v[0]>>=s;
  v->v[1]>>=s;
}

void v16_LeftShift(v16_t *v, uint8_t s )
{
  v->v[0]<<=s;
  v->v[1]<<=s;
}

void v16_Add(v16_t *v, v16_t *w )
{
  v->v[0]+=w->v[0];
  v->v[1]+=w->v[1];
}

void v16_AddConstant(v16_t *v, uint16_t c )
{
  v->v[0]+=c;
  v->v[1]+=c;
}

void v16_SubConstant(v16_t *v, uint16_t c )
{
  v->v[0]-=c;
  v->v[1]-=c;
}

void v16_Sub(v16_t *v, v16_t *w )
{
  v->v[0]-=w->v[0];
  v->v[1]-=w->v[1];
}

void v16_SetByConstantByOneConstant(v16_t *v, uint16_t w)
{
  v->v[0] = w;
  v->v[1] = w;
}

void v16_AverageByConstant(v16_t *v, uint16_t x, uint16_t y)
{
  v->v[0] = (v->v[0] + x + 1)/2;
  v->v[1] = (v->v[1] + y +1)/2;
}

void v16_AverageByV16(v16_t *v, v16_t *vv)
{
  v->v[0] = (v->v[0] + vv->v[0] + 1)/2;
  v->v[1] = (v->v[1] + vv->v[1] +1)/2;
  
}

void v16_AddDir(v16_t *v, uint8_t dir, uint16_t w)
{
  switch(dir)
  {
    case 0:
      v->v[0] += w;
      break;
    case 1:
      v->v[1] += w;
      break;
    case 2:
      v->v[0] -= w;
      break;
    case 3:
      v->v[1] -= w;
      break;
  }
}

/*=================================================*/

void map_Init(map_t *m)
{
  /* constants */
  m->pdw = 128;
  m->pdh = 64;
  m->vis_win_disp_pos_pix.v[0] = 0;
  m->vis_win_disp_pos_pix.v[1] = 0;
  
  /* the visible area in tiles is a little bit bigger than the pixel size of the area in pixel */
  m->tmw = (m->pdw + 15)/16 + 1;
  m->tmh = (m->pdh + 15)/16 + 1;
}

/* set the position of the visible window in the map */
void map_SetWindowPos(map_t *m, v16_t *newpos)
{
  
  v16_t v;

  v16_SetByV16(&(m->vwpp), newpos );
  v16_SetByV16(&(m->vwpt), newpos );
  v16_RightShift(&(m->vwpt), 4);
  v16_SetByV16(&v, &(m->vwpt));
  v16_LeftShift(&v, 4);
  v16_SetByV16(&(m->dtwp), &(m->vwpp));
  v16_Sub(&(m->dtwp), &v);
  
  //printf("vwpp.v[0]=%u vwpt.v[0]=%u delta-x=%u tmw=%u\n", m->vwpp.v[0], m->vwpt.v[0], m->dtwp.v[0], m->tmw);
  //printf("vwpp.v[1]=%u vwpt.v[1]=%u delta-y=%u tmh=%u\n", m->vwpp.v[1], m->vwpt.v[1], m->dtwp.v[1], m->tmh);
}

uint8_t map_GetTile(map_t *m, uint16_t tx, uint16_t ty)
{
  /*
  static uint8_t map[12][12+1] =
     {
       "\x80\x7d\x7d\x7d\x7d\x7d\x7d\x7d\x7d\x7d\x7d\x81",
       "\x7e\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x7e",
       "\x7e\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x7e",
       "\x7e\x20\x43\x20\x20\x20\x20\x20\x20\x4e\x20\x7e",
       "\x7e\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x7e",
       "\x7e\x20\x20\x20\x20\x20\x45\x20\x20\x20\x20\x7e",
       "\x7e\x20\x20\x20\x20\x20\x20\x20\x20\x20\x93\x7e",
       "\x7e\x20\x20\x20\x20\x20\x20\xa0\x20\x20\x20\x7e",
       "\x7e\x20\x50\x20\x20\x20\x20\x20\x20\x20\x20\x7e",
       "\x7e\x20\x20\x20\x91\x20\x20\x20\x20\x92\x20\x7e",
       "\x7e\x54\x20\x20\x20\x20\x20\x20\x20\x20\x20\x7e",
       "\x82\x7d\x7d\x7d\x7d\x7d\x7d\x7d\x7d\x7d\x7d\x83"
     };
  */
     
  static uint8_t map[12][12+1] =
     {
    "\x80\x7d\x7d\x7d\x81\x20\x20\x80\x7d\x7d\x7d\x81",
    "\x7e\x20\x20\x20\x7e\x20\x20\x7e\x20\x20\x20\x7e",
    "\x82\x7d\x81\x20\x82\x7d\x7d\x85\x20\x7e\x20\x7e",
    "\x20\x20\x7e\x20\x20\x20\x20\x84\x7d\x83\x20\x7e",
    "\x20\x20\x7e\x20\x20\x20\x20\x7f\x20\x20\x20\x7e",
    "\x20\x20\x7e\x20\x20\x20\x20\x20\x20\x20\x20\x7e",
    "\x20\x20\x82\x7d\x81\x20\x7d\x7d\x7d\x7d\x7d\x85",
    "\x20\x20\x20\x20\x7e\x20\x20\x20\x20\x54\xa0\x7e",
    "\x20\x20\x20\x20\x82\x7d\x7d\x7d\x7d\x7d\x7d\x83",
    "\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20",
    "\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20",
    "\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20"
     };

     
     /* pixel dimensions are 2^16 * 2^16, this means tile dimension is 4096x4096 */
     /* as a consequence, wrap around at 4096 */
     /* am not sure to do this here, maybe it should be done outside this proc */
     tx &= 0x0fff;
     ty &= 0x0fff;
     if ( tx > 12 )
       return 32;
     if ( ty > 12 )
       return 32;
     return map[ty][tx];
}

/* check whether the target object is occupied */
int map_IsOccupied(map_t *m, v16_t *pos_t)
{
  uint8_t tile;
  tile = map_GetTile(m, pos_t->v[0], pos_t->v[1]);
  if ( tile == 32 )
    return 0;
  return 1;
}

/* check whether tile in direction dir is occpied */
int map_isOccupiedInDir(map_t *m, v16_t *pos_t, uint8_t dir)
{
  v16_t tmp;
  v16_SetByV16(&tmp, pos_t);
  v16_AddDir(&tmp, dir, 1);
  return map_IsOccupied(m, &tmp);
}

/* for a position on the map (tile coordinates) return pixel pos on display */
void map_GetDisplayPosByTileMapPos(map_t *m, v16_t *dest_pos_pix, v16_t *src_tile_pos)
{
  v16_SetByV16(dest_pos_pix, src_tile_pos );
  v16_Sub(dest_pos_pix, &(m->vwpt));  /* upper left tile corner of the visible area */
  v16_LeftShift(dest_pos_pix, 4);			/* convert to pixel */
  v16_Sub(dest_pos_pix, &(m->dtwp));  /* add the offset of the upper left tile corner */
  v16_Add(dest_pos_pix, &(m->vis_win_disp_pos_pix));  /* add display offset */
}

/*x,y: tile position */
uint8_t map_IsTileVisible(map_t *m, uint16_t x, uint16_t y)
{
  uint16_t ux, uy;
  
  /* calculate the lower left corner (ux/uy) of the visible window in tiles */
  ux = m->vwpt.v[0]+m->tmw;
  ux &=0x0fff;		/* wrap around mask for tiles */
  uy = m->vwpt.v[1]+m->tmh;
  uy &=0x0fff;		/* wrap around mask for tiles */
  
  if (  m->vwpt.v[0] < ux )
  {
    if ( x < m->vwpt.v[0] )
      return 0;
    if ( x >= ux )
      return 0;
  }
  else
  {
    if ( x < ux && x >= m->vwpt.v[0] )
      return 0;
  }
  
  if (  m->vwpt.v[1] < uy )
  {
    if ( y < m->vwpt.v[1] )
      return 0;
    if ( y >= uy )
      return 0;
  }
  else
  {
    if ( y < uy && y >= m->vwpt.v[1] )
      return 0;
  }
  
  return 1;
}

void map_Draw(map_t *m, u8g2_t *u8g2)
{
  uint16_t tx;
  uint16_t ty;
  u8g2_uint_t px, ppx;
  u8g2_uint_t py;

  /* offset for the visible window on the display from the upper left display corner */
  px = m->vis_win_disp_pos_pix.v[0];
  px -= m->dtwp.v[0];
  
  py = m->vis_win_disp_pos_pix.v[1];
  py -= m->dtwp.v[1];
  
  for( ty = 0; ty < m->tmh; ty++ )
  {
    ppx = px;
    for( tx = 0; tx < m->tmw; tx++ )
    {
      /* py+16 is there because reference point for the tiles is lower left (baseline) */ 
      u8g2_DrawGlyph(u8g2, ppx, py+16, map_GetTile(m, tx+m->vwpt.v[0], ty+m->vwpt.v[1]));
      ppx += 16;
    }
    py += 16;
  }
}

/*=================================================*/

void gm_Init(gm_t *gm)
{
  gm->pt.v[0] = 1;
  gm->pt.v[1] = 1;
  gm->state = GM_STATE_CENTER;
  gm->dir = 0;
  v16_SetByConstantByOneConstant(&(gm->twop), GM_OFFSET);
  v16_SetByConstantByOneConstant(&(gm->cwop), GM_OFFSET);
  v16_SetByConstantByOneConstant(&(gm->gmop), GM_OFFSET);
}

void gm_SetWindowPosByGolemMasterPos(gm_t *gm, map_t *map)
{
  v16_t v, w;
  
  v16_SetByV16(&v, &(gm->pt));
  v16_LeftShift(&v, 4);
  //v16_SetByV16(&w, &(map->pdw));
  w.v[0] = map->pdw;
  w.v[1] = map->pdh;
  v16_RightShift(&w, 1);  
  v16_Sub(&v, &w);
  v16_AddConstant(&v, 8);	/* adjust half tile to center exaktly */
  v16_Add(&v, &(gm->cwop));
  v16_SubConstant(&v, GM_OFFSET);
    
  map_SetWindowPos(map, &v);
}

void gm_Draw(gm_t *gm, map_t *map, u8g2_t *u8g2)
{
  v16_t dest_pos_pix;

  map_GetDisplayPosByTileMapPos(map, &dest_pos_pix, &(gm->pt));  
  v16_Add(&dest_pos_pix, &(gm->gmop) );
  v16_SubConstant(&dest_pos_pix, GM_OFFSET);
    
  if ( map_IsTileVisible(map, gm->pt.v[0], gm->pt.v[1]) )
  {
    // puts("visible!");
    u8g2_DrawGlyph(u8g2, 
      dest_pos_pix.v[0],
      dest_pos_pix.v[1]+16,
      0x4e);
  }
  else
  {
    // puts("not visible");
  }
}

/* returns 1 if gm made a step */
int8_t gm_Walk(gm_t *gm, map_t *map, uint8_t dir)
{
  uint8_t gm_made_step;
  
  gm_made_step = 0; /* so far gm did not walk */
  
  printf("input dir=%d, curr state = %d\n", dir, gm->state);
  if ( gm->state == GM_STATE_CENTER )
  {
    gm->state = GM_STATE_READY_FOR_WALK;
    gm->dir = dir;
    v16_SetByConstantByOneConstant(&(gm->twop), GM_OFFSET);
    v16_AddDir(&(gm->twop), dir, (uint16_t)16);
  }
  else if ( gm->state == GM_STATE_READY_FOR_WALK )
  {
    if ( gm->dir != dir )
    {
      gm->state = GM_STATE_CENTER;
      gm->dir = 0;
      v16_SetByConstantByOneConstant(&(gm->twop), GM_OFFSET);
      printf("reset state=%d dir=%d\n", gm->state, gm->dir);
    }
    else
    {
      uint8_t dest_tile;
      
      /* try to walk into the requested direction */
      v16_AddDir(&(gm->pt), dir, 1);
      
      /* check whether the dest tile is valid. if not, go back */
      dest_tile = map_GetTile(map, gm->pt.v[0], gm->pt.v[1]);
      if ( dest_tile == 32 )
      {
	/* destination tile is valid, gm did a step */
	gm_made_step = 1;
	
	v16_SetByConstantByOneConstant(&(gm->gmop), GM_OFFSET);
	v16_AddDir(&(gm->gmop), dir, (uint16_t)-16);
	v16_SetByV16(&(gm->cwop), &(gm->twop));
	v16_AddDir(&(gm->cwop), dir, (uint16_t)-16);

	printf("walk state=%d dir=%d\n", gm->state, gm->dir);
      }
      else
      {
	v16_AddDir(&(gm->pt), (dir+2)&3, 1);
	printf("blocked\n");
      }
    }
  }
  return gm_made_step;
}

void gm_Step(gm_t *gm, map_t *map)
{
  v16_AverageByV16(&(gm->cwop), &(gm->twop));
  v16_AverageByConstant(&(gm->gmop), GM_OFFSET, GM_OFFSET);
  
  gm_SetWindowPosByGolemMasterPos(gm, map);
}

/*=================================================*/


void obj_Init(obj_t *o)
{
  o->tile = 0xe0;
  v16_SetByConstant(&(o->pos_t), 4, 3);
  v16_SetByConstant(&(o->offset_p), 0, 0);
  o->health = 10;
  o->attack = 2;
  o->defense = 2;
  o->mode = OBJ_MODE_ATTACK_PLAYER;
}


/* try to walk towards destination */
void obj_WalkTo(obj_t *o, map_t *map, v16_t *dest_t)
{
  uint8_t dir;
  
  /* which direction should the object move? */
  if ( dest_t->v[0] < o->pos_t.v[0] && !map_isOccupiedInDir(map, &(o->pos_t), 2) )
    dir = 2;
  else if ( dest_t->v[0] > o->pos_t.v[0] && !map_isOccupiedInDir(map, &(o->pos_t), 0) )
    dir = 0;
  else if ( dest_t->v[1] > o->pos_t.v[1] && !map_isOccupiedInDir(map, &(o->pos_t), 1) )
    dir = 1;
  else if ( dest_t->v[1] < o->pos_t.v[1] && !map_isOccupiedInDir(map, &(o->pos_t), 3) )
    dir = 3;
  else
    /* no move */
    return;
  
  v16_AddDir(&(o->pos_t), dir, 1); 
}

void obj_Draw(obj_t *o, map_t *map, u8g2_t *u8g2)
{
  v16_t dest_pos_pix;


  map_GetDisplayPosByTileMapPos(map, &dest_pos_pix, &(o->pos_t));  
  
  
  if ( map_IsTileVisible(map, o->pos_t.v[0], o->pos_t.v[1]) )
  {
    // puts("visible!");
    u8g2_DrawGlyph(u8g2, 
      dest_pos_pix.v[0],
      dest_pos_pix.v[1]+16,
      o->tile);
  }
  else
  {
    puts("Spinne nicht sichtbar!!!");
  }
}

void obj_list_Draw(map_t *map, u8g2_t *u8g2)
{
  uint8_t i;
  for( i = 0; i < OBJ_LIST_MAX; i++ )
    obj_Draw(obj_list+i, map, u8g2);
}



/*=================================================*/

u8g2_t u8g2;
map_t map;
gm_t gm;

obj_t spider;



int main(void)
{
  int k;
  
  int8_t walk_direction;
  
  u8g2_SetupBuffer_SDL_128x64_4(&u8g2, &u8g2_cb_r0);
  u8x8_InitDisplay(u8g2_GetU8x8(&u8g2));
  u8x8_SetPowerSave(u8g2_GetU8x8(&u8g2), 0);  
  
  u8g2_SetFont(&u8g2, scrollosprites);
  
  obj_Init(&spider);
  spider.tile = 0x54;

  obj_list_Init();
  map_Init(&map);
  gm_Init(&gm);

  
  for(;;)
  {
    
    gm_SetWindowPosByGolemMasterPos(&gm, &map);
    
    do
    {
      u8g2_FirstPage(&u8g2);
      do
      {
	u8g2_SetFontDirection(&u8g2, 0);
	map_Draw(&map, &u8g2);
	gm_Draw(&gm, &map, &u8g2);
	obj_Draw(&spider, &map,&u8g2);
      } while( u8g2_NextPage(&u8g2) );
    
      gm_Step(&gm, &map);
      
      usleep(100000);
      k = u8g_sdl_get_key();
    } while( k < 0 );

    if ( k == 'q' ) break;    
    
    switch( k )
    {
      case 273: walk_direction = 3; break;
      case 274: walk_direction = 1; break;
      case 276: walk_direction = 2; break;
      case 275: walk_direction = 0; break;      
      default: walk_direction = -1; break;
    }
    
    if ( walk_direction >= 0 )
    {
      if ( gm_Walk(&gm,&map, walk_direction) != 0 )
      {
	obj_WalkTo(&spider, &map, &(gm.pt));
      }
    }
    
        
  }
  return 0;
}

