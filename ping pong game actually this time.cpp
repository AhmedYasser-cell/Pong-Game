#include <iostream>
#include <conio.h>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <locale>

using namespace std;


int ballx, bally;
bool game_over;
bool round_over;
const int width = 81;
const int length = 21;
int score_p1 , score_p2;
int P1_head , P1_tail, P2_head , P2_tail;
enum pdir { STOP, UP, DOWN , STALL };
pdir P1;
pdir P2;
enum bdir { Stop , UP_LEFT = 0, UP_RIGHT = 1, DOWN_LEFT = 2, DOWN_RIGHT = 3};
bdir ball ; 
string result;

void improve_visuals() {
	CONSOLE_CURSOR_INFO cursor;
	cursor.bVisible = FALSE;
	cursor.dwSize = 1;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor);
}
void setup() {
	improve_visuals();
	SetConsoleOutputCP(CP_UTF8);
	srand(time(NULL));
	game_over = false; 
	round_over = false;
	score_p1 = 0; score_p2 = 0 ;
	P1_head = 9 , P1_tail = 13;
	P2_head = 9, P2_tail = 13;
	P1 = STOP; P2 = STOP;
	ball = Stop;
	ballx = width / 2;
	bally = length / 2;
}
void draw() {
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 0,0 });
	
	for (int i = 0; i < width; i++) cout << u8"▄";
	cout << endl;
	for (int i = 0; i < length; i++) {
		for (int j = 0; j < width; j++) {
			 if (j == 0 || j == width - 1 || j == (width / 2)) {
				cout << u8"█";
			}
			else if (j == 1  && i <= P1_tail && i >= P1_head) {
				cout << u8"▐";
			}
			else if (i == bally && j == ballx) { cout << u8"●"; }
			else if ( j == width - 2 && i <= P2_tail && i >= P2_head) {
				cout << u8"▌";
			}
			else cout << ' ';
		}
		cout << endl;
	}
	for (int i = 0; i < width; i++) cout << u8"▀";
	result = (score_p1 == 9 || score_p2 == 9) ? "Match Point" : " ";
	cout << endl << "Score: " << score_p1 << " - " << score_p2 << endl << result << endl;

}
void input() {
	if (GetAsyncKeyState('W') & 0x8000) {
		P1 = UP;
	}
	else if (GetAsyncKeyState('S') & 0x8000) {
		P1 = DOWN;
	}

	if (GetAsyncKeyState('I') & 0x8000) {
		P2 = UP;
	}
	else if (GetAsyncKeyState('K') & 0x8000) {
		P2 = DOWN;
	}

	if (GetAsyncKeyState('X') & 0x8000) {
		game_over = true;
		round_over = true;
	}
}

void __move_ball() {
	switch (ball) {
	case UP_LEFT:
		bally--;
		ballx-= 2;
		break;
	case UP_RIGHT:
		bally--;
		ballx+= 2 ;
		break;
	case DOWN_LEFT:
		bally++;
		ballx-= 2;
		break;
	case DOWN_RIGHT:
		bally++;
		ballx+= 2;
		break;
	}
}

void _start_ball() {
	round_over = false;
	for (int i = 3; i > 0; i--) {
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { width / 2, length / 2 });
		cout << i;
		Sleep(350);
	}
	
	switch (int y = rand() % 4) {
	case 0:
		ball = UP_LEFT;
		break;
	case 1:
		ball = UP_RIGHT;
		break;
	case 2:
		ball = DOWN_LEFT;
		break;
	case 3:
		ball = DOWN_RIGHT;
		break;
	}
	ballx = width / 2;
	bally = length / 2;
}
void player_logic() {

	switch (P1)
	{
	case DOWN:
		if (P1_tail < length - 1) {
			P1_tail++;
			P1_head++;
			P1 = STALL;
		}
		break;
	case UP:
		if (P1_head > 0) {
			P1_tail--;
			P1_head--;
			P1 = STALL;
		}
		break;
	}
	switch (P2) {
	case DOWN:
		if (P2_tail < length - 1) {
			P2_tail++;
			P2_head++;
			P2 = STALL;
		}
		break;
	case UP:
		if (P2_head > 0) {
			P2_tail--;
			P2_head--;
			P2 = STALL;
		}
		break;
	}
}


void ball_logic() {
	if (ballx == 0) {
		score_p2++;
		round_over = true;
	}
	else if (ballx == width - 1) { 
		score_p1++; 
		round_over = true;
	}
	if (ballx <=2 && (bally >= P1_head && bally <= P1_tail)) {
		if (ball == UP_LEFT) { ball = UP_RIGHT; Beep(1000, 50); }
		else if (ball == DOWN_LEFT) { ball = DOWN_RIGHT; Beep(1000, 50); }
	}
	else if (ballx >= width - 3 && (bally >= P2_head && bally <= P2_tail)) {
		if (ball == UP_RIGHT) {ball = UP_LEFT; Beep(1000, 50);
	}
		else if (ball == DOWN_RIGHT) {ball = DOWN_LEFT; Beep(1000, 50);}
	}
	else if (bally == 0) {
		if (ball == UP_LEFT) ball = DOWN_LEFT;
		else if (ball == UP_RIGHT) ball = DOWN_RIGHT;
	}
	else if (bally == length - 1 ) {
		if (ball == DOWN_LEFT) ball = UP_LEFT;
		else if (ball == DOWN_RIGHT) ball = UP_RIGHT;
	}
}

int main()
{
	
	setup();
	while (!game_over) {
		_start_ball();
		while (!round_over) {
			draw();
			input();
			player_logic();
			ball_logic();
			__move_ball();
			Sleep(10);
		}
		system("cls");
		if (score_p1 == 10 || score_p2 == 10) {
			game_over = true;
		}
	}
	cout << endl << "Score: " << score_p1 << " - " << score_p2 << endl <<endl;
	result = (score_p2 <= score_p1) ? ((score_p1 > score_p2) ? "PLAYER 1 WON!!!" : "DRAW") : "PLAYER 2 WON!!!";
	cout << result ;

	return 0;
}
