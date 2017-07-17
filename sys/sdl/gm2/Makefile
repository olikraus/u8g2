CFLAGS = -g -Wall -Wpointer-arith -I. -I../../../csrc/. -I../../tga/mapgen/. -I../../../tools/ugl/ `sdl-config --cflags` 

GMSRC = $(shell ls ../../../csrc/*.c) $(shell ls ../common/*.c ) 
GMSRC += map.c  main.c item.c
GMSRC += ../../../tools/ugl/ugl_bc.c

GMOBJ = $(GMSRC:.c=.o)

MAPGENSRC = $(shell ls ../../../csrc/*.c) mapgen.c u8g2_d_tga.c
MAPGENSRC += ../../../tools/ugl/ugl_arrays.c  
MAPGENSRC += ../../../tools/ugl/ugl_error.c  
MAPGENSRC += ../../../tools/ugl/ugl_parse.c
#MAPGENSRC += ../../../tools/ugl/ugl_bc.c

MAPGENOBJ = $(MAPGENSRC:.c=.o)



gm: $(GMOBJ) map.c
	$(CC) $(CFLAGS) $(LDFLAGS) $(GMOBJ) `sdl-config --libs` -o gm

mapgen: $(MAPGENOBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) $(MAPGENOBJ) -o mapgen

map.c: mapgen gm.map
	./mapgen -o map.c gm.map

clean:	
	-rm $(GMOBJ) $(MAPGENOBJ) gm mapgen map.c

