CFLAGS = -g -Wall -Wpointer-arith -I. -I../../../csrc/. -I../../tga/mapgen/. `sdl2-config --cflags` 

GMSRC = $(shell ls ../../../csrc/*.c) $(shell ls ../common/*.c ) 
GMSRC += map.c  main.c item.c
#GMSRC += ../../../tools/ugl/ugl_bc.c
GMSRC += ugl_bc.c

GMOBJ = $(GMSRC:.c=.o)

MAPGENSRC = $(shell ls ../../../csrc/*.c) mapgen.c u8g2_d_tga.c
MAPGENSRC += ugl_arrays.c  ugl_error.c  ugl_parse.c

MAPGENOBJ = $(MAPGENSRC:.c=.o)

UGLSRC = ugl_arrays.c  ugl_error.c  ugl_parse.c ugl_bc.c  ugl_main.c   item.c map.c

UGLOBJ = $(UGLSRC:.c=.o)


gm: $(GMOBJ) map.c
	$(CC) $(CFLAGS) $(LDFLAGS) $(GMOBJ) `sdl2-config --libs` -o gm

mapgen: $(MAPGENOBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) $(MAPGENOBJ) -o mapgen

map.c: mapgen gm.map 
	./mapgen -o map.c gm.map
	
ugl: $(UGLOBJ) map.c
	$(CC) $(CFLAGS) $(LDFLAGS) $(UGLOBJ) -o ugl

clean:	
	-rm $(GMOBJ) $(MAPGENOBJ) $(UGLOBJ) gm mapgen ugl map.c

