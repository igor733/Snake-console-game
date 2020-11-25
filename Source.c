#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include "curses.h"

#pragma comment (lib, "pdcurses") 

#define WIDTH 120
#define HEIGHT 17

int main(void)
{
	initscr(); // enter in curses mode
	curs_set(0); // remove the cursor

	/* Set background of the console*/
	start_color();
	init_pair(1, COLOR_WHITE, COLOR_BLUE);
	bkgd(COLOR_PAIR(1));
	refresh();

	WINDOW *margin = newwin(HEIGHT, WIDTH, 0, 0); // create game margin
	wbkgd(margin, COLOR_PAIR(1)); // set backrgound of the maring
	box(margin, 0, 0); // create margin borders
	wrefresh(margin); // output

	WINDOW *score = newwin(1, 15, HEIGHT, 0);
	wbkgd(score, COLOR_PAIR(1));
	wprintw(score, "Score: %d", 5);
	wrefresh(score);

	/*The massives with the snake coordinates */
	int x[100] = { 10, 9, 8 };
	int y[100] = { 5, 5, 5 };

	/* Game loop*/
	for (int c, eat_x = -1, eat_y = -1, size = 2, eat = 0, sw = 0, last = 100, head = 62; ;)
	{
		nodelay(margin, TRUE);

		/* Draw snake body */
		mvwprintw(margin, y[0], x[0], "%c", head);
		for (int i = 1; i <= size; ++i)
			mvwprintw(margin, y[i], x[i], "$");

		/* Snake ate food*/
		if (x[0] == eat_x && y[0] == eat_y)
		{
			eat = 0;
			++size;
			x[size] = x[size - 1];
			y[size] = y[size - 1];
		}

		/* Spawn food */
		if (eat == 0)
		{
			eat_x = rand() % (WIDTH -3) + 1;
			eat_y = rand() % (HEIGHT -3) + 1;
			for (; mvwinch(margin, eat_y, eat_x) != 16777248;)
			{
				eat_x = rand() % (WIDTH - 3) + 1;
				eat_y = rand() % (HEIGHT - 3) + 1;
			}
			eat = 1;
		}
		mvwprintw(margin, eat_y, eat_x, "@");

		/* Cheking lose */
		if (x[0] >= WIDTH - 1 || x[0] <= 0 || y[0] >= HEIGHT - 1 || y[0] <= 0 || mvwinch(margin, y[0], x[0]) == 16777252)
		{
			mvwprintw(margin, HEIGHT / 2, WIDTH / 2 - 4, "You lose!");
			wrefresh(margin);
			break;
		}

		/* Snake moving */
		for (int i = size; i > 0; --i)
		{
			x[i] = x[i - 1];
			y[i] = y[i - 1];
		}

		wrefresh(margin);
		Sleep(150); // delay

		c = wgetch(margin); // get inputr from keyboard
		
		/* Cheking input */
		switch (c) {
		case 'w':
			if (head != 118)
			{
				y[0]--;
				last = c;
				head = 94;
				break;
			}
			else
				y[0]++;
			break;
		case 'a':
			if (head != 62)
			{
				x[0]--;
				last = c;
				head = 60;
				break;
			}
			else
				x[0]++;
			break;
		case 's':
			if (head != 94)
			{
				y[0]++;
				last = c;
				head = 118;
				break;
			}
			else
				y[0]--;
			break;
		case 'd':
			if (head != 60)
			{
				x[0]++;
				last = c;
				head = 62;
				break;
			}
			else
				x[0]--;
			break;
		default:
			switch (last) {
			case 'w':
				y[0]--;
				break;
			case 'a':
				x[0]--;
				break;
			case 's':
				y[0]++;
				break;
			case 'd':
				x[0]++;
				break;
			}
		}

		wclear(score);
		wprintw(score, "Score: %d", size - 2);
		wrefresh(score);
		wclear(margin); // margin clear
		box(margin, 0, 0); // borders remaking
		wrefresh(margin); // updating output
	}
	getch(); //press any key to stop the programm
	endwin(); // leave the curses mode
	return 0;
}