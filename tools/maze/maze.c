/*

Source: http://rosettacode.org/wiki/Maze_generation#C
License: http://www.gnu.org/licenses/fdl.html

another option could be "recursive division"
http://weblog.jamisbuck.org/2011/1/12/maze-generation-recursive-division-algorithm


roguelike:
http://www.roguebasin.com/index.php?title=Roguelike_Dev_FAQ
http://www.roguebasin.com/index.php?title=How_to_Write_a_Roguelike_in_15_Steps
http://www.roguebasin.com/index.php?title=Finding_graphical_tiles
	http://opengameart.org/content/2d-rpg-tiles
	http://opengameart.org/content/tileset-1bit-color
	http://opengameart.org/content/tileset-1bit-color-extention
	http://dwarffortresswiki.org/index.php/Tileset_repository
	http://game-icons.net/	<< could be nice
https://www.reddit.com/r/gamedev/comments/444zbl/roguelike_tilesets/
	http://cc.retinaleclipse.com/minirogue-c64-all.png (https://forums.tigsource.com/index.php?topic=14166.0)
	http://quale-art.blogspot.de/p/scroll-o-sprites.html  oder http://imgur.com/a/uHx4k


fonts:
http://de.fonts2u.com/pixelcharas.schriftart
http://fontstruct.com/fontstructions/show/475298/monstapix (http://de.fonts2u.com/monstapix-regular.schriftart)
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
 
#define DOUBLE_SPACE 0
 
#if DOUBLE_SPACE
#	define SPC "　"
#else
#	define SPC " "
#endif
 
wchar_t glyph[] = L""SPC"│││─┘┐┤─└┌├─┴┬┼"SPC"┆┆┆┄╯╮ ┄╰╭ ┄";
 
typedef unsigned char byte;
enum { N = 1, S = 2, W = 4, E = 8, V = 16 };
 
byte **cell;
int w, h, avail;
#define each(i, x, y) for (i = x; i <= y; i++)
 
int irand(int n)
{
	int r, rmax = n * (RAND_MAX / n);
	while ((r = rand()) >= rmax);
	return r / (RAND_MAX/n);
}
 
void show()
{
	int i, j, c;
	each(i, 0, 2 * h) {
		each(j, 0, 2 * w) {
			c = cell[i][j];
			if (c > V) printf("\033[31m");
			printf("%lc", glyph[c]);
			if (c > V) printf("\033[m");
		}
		putchar('\n');
	}
}
 
inline int max(int a, int b) { return a >= b ? a : b; }
inline int min(int a, int b) { return b >= a ? a : b; }
 
static int dirs[4][2] = {{-2, 0}, {0, 2}, {2, 0}, {0, -2}};
void walk(int x, int y)
{
	int i, t, x1, y1, d[4] = { 0, 1, 2, 3 };
 
	cell[y][x] |= V;
	avail--;
 
	for (x1 = 3; x1; x1--)
		if (x1 != (y1 = irand(x1 + 1)))
			i = d[x1], d[x1] = d[y1], d[y1] = i;
 
	for (i = 0; avail && i < 4; i++) {
		x1 = x + dirs[ d[i] ][0], y1 = y + dirs[ d[i] ][1];
 
		if (cell[y1][x1] & V) continue;
 
		/* break walls */
		if (x1 == x) {
			t = (y + y1) / 2;
			cell[t][x+1] &= ~W, cell[t][x] &= ~(E|W), cell[t][x-1] &= ~E;
		} else if (y1 == y) {
			t = (x + x1)/2;
			cell[y-1][t] &= ~S, cell[y][t] &= ~(N|S), cell[y+1][t] &= ~N;
		}
		walk(x1, y1);
	}
}
 
int solve(int x, int y, int tox, int toy)
{
	int i, t, x1, y1;
 
	cell[y][x] |= V;
	if (x == tox && y == toy) return 1;
 
	each(i, 0, 3) {
		x1 = x + dirs[i][0], y1 = y + dirs[i][1];
		if (cell[y1][x1]) continue;
 
		/* mark path */
		if (x1 == x) {
			t = (y + y1)/2;
			if (cell[t][x] || !solve(x1, y1, tox, toy)) continue;
 
			cell[t-1][x] |= S, cell[t][x] |= V|N|S, cell[t+1][x] |= N;
		} else if (y1 == y) {
			t = (x + x1)/2;
			if (cell[y][t] || !solve(x1, y1, tox, toy)) continue;
 
			cell[y][t-1] |= E, cell[y][t] |= V|E|W, cell[y][t+1] |= W;
		}
		return 1;
	}
 
	/* backtrack */
	cell[y][x] &= ~V;
	return 0;
}
 
void make_maze()
{
	int i, j;
	int h2 = 2 * h + 2, w2 = 2 * w + 2;
	byte **p;
 
	p = calloc(sizeof(byte*) * (h2 + 2) + w2 * h2 + 1, 1);
 
	p[1] = (byte*)(p + h2 + 2) + 1;
	each(i, 2, h2) p[i] = p[i-1] + w2;
	p[0] = p[h2];
	cell = &p[1];
 
	each(i, -1, 2 * h + 1) cell[i][-1] = cell[i][w2 - 1] = V;
	each(j, 0, 2 * w) cell[-1][j] = cell[h2 - 1][j] = V;
	each(i, 0, h) each(j, 0, 2 * w) cell[2*i][j] |= E|W;
	each(i, 0, 2 * h) each(j, 0, w) cell[i][2*j] |= N|S;
	each(j, 0, 2 * w) cell[0][j] &= ~N, cell[2*h][j] &= ~S;
	each(i, 0, 2 * h) cell[i][0] &= ~W, cell[i][2*w] &= ~E;
 
	avail = w * h;
	walk(irand(2) * 2 + 1, irand(h) * 2 + 1);
 
	/* reset visited marker (it's also used by path finder) */
	each(i, 0, 2 * h) each(j, 0, 2 * w) cell[i][j] &= ~V;
	solve(1, 1, 2 * w - 1, 2 * h - 1);
 
	show();
}
 
int main(int c, char **v)
{
	setlocale(LC_ALL, "");
	if (c < 2 || (w = atoi(v[1])) <= 0) w = 16;
	if (c < 3 || (h = atoi(v[2])) <= 0) h = 8;
 
	make_maze();
 
	return 0;
}

