/*

  hbshape2u8g2
  
  converts the JSON output from hb-shape to u8g2 binary format

*/

#include "co.h"

int cpMapGetIntVal(cco o, const char *key)
{
  cco member;
  member = coMapGet(o, key);
  if ( member == NULL )
  {
    printf("cpMapGetIntVal: key %s not found\n", key);
    return 0; // key doesn't exist
  }
  if ( coIsDbl(member) == 0 )
    return 0; // not a double
  return (int)coDblGet(member);
}

                                      // NULL if key doesn't exist in the map


int main(int argc, char **argv)
{
    co jsonco;
    FILE *jsonfp;
    
    if ( argc != 2 )
    {
            printf("%s hb-shape.json\n", argv[0]);
            return 1;
    }
    jsonfp = fopen(argv[1], "rb");
    if ( jsonfp == NULL )
    {
            perror(argv[2]);
            return 2;
    }
    
    jsonco = coReadJSONByFP(jsonfp);
	
	if ( jsonco == NULL )
	{
		printf( "result is NULL\n");
	}
	else if ( !coIsVector(jsonco) )
	{
		printf( "input is not a vector\n");
	}
	else
	{
            long i;
            cco element;
            int x = 0;
            int y = 20;
            int ax;
            int ay;
            int dx;
            int dy;
            int xb;
            int yb;
            uint16_t encoding;
            for(i = 0; i < coVectorSize(jsonco); i++ )
            {
              element = coVectorGet(jsonco, i);
              if ( coIsMap(element) )
              {
                encoding = cpMapGetIntVal(element, "g")-1;
                ax = cpMapGetIntVal(element, "ax");
                ay = cpMapGetIntVal(element, "ay");
                dx = cpMapGetIntVal(element, "dx");
                dy = cpMapGetIntVal(element, "dy");
                xb = cpMapGetIntVal(element, "xb");
                yb = cpMapGetIntVal(element, "yb");
                printf("u8g2_DrawGlyph(&u8g2, %d, %d, %u); // ax=%d ay=%d dx=%d dy=%d xb=%d yb=%d\n", x+dx, y+dy, encoding, ax, ay, dx, dy, xb, yb);
                printf("u8g2_DrawGlyph(&u8g2, %ld, %d, %u); // reference \n", i*20, y+30, encoding);
                printf("u8g2_DrawPixel(&u8g2, %ld, %d);\n",  i*20, y+30);
                //printf("printf(\"%ld %u dx=%%d\\n\", u8g2_GetXOffsetGlyph(&u8g2, %u));\n", i, encoding, encoding);

                
                x += ax;
                y += ay;
              }
            }
	}
	//coPrint(jsonco);
    
    fclose(jsonfp);
    coDelete(jsonco);
    return 0;
}

