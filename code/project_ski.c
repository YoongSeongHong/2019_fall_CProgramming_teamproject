#include <stdio.h>
#include <windows.h>
#include<time.h>

#define WIDTH 48
#define HEIGHT 38
#define UP 72
#define LEFT 75
#define RIGHT 77
#define DOWN 80
#define ENTER 13
#define ESC 27

void CursorView(char show);//커서없애기
void consol();//콘솔창 크기
int drawmenu();//start menu
void game_info();//게임설명
void gotoxy(int x, int y);
int keyDownis(int key);//키 다운처리
void Init();//초기값
void createRoad(int road_rand);
void moveRoad();//길 움직임
void deleteRoad();
int strike();
void moveplayer();
void printmap();
void game_over();

typedef struct
{
	int x1;//왼쪽 길
	int x2;//오른쪽 길
	int y;
	int act;
}Snow;
typedef struct
{
	int x;
}Player;//플레이어 구조체

Snow snow[HEIGHT];//배열변수 구조체 선언
Player player;



int main()
{
	int road_rand = 0;
	int i = 0;
	int score = 0;
	while (1)
	{
		consol();
		int menu_select = drawmenu();
		if (menu_select == 0)
		{
			Init();
			do
			{
				createRoad(road_rand);
				if (rand() % 2 == 0)
				{
					if (road_rand == 9)
						road_rand--;
					else
						road_rand++;
				}
				else
				{
					if (road_rand == -9)
						road_rand++;
					else
						road_rand--;
				}
				moveRoad();
				deleteRoad();
				moveplayer();
				printmap();
				if (i == 34)
				{
					for (int j = 3; j >= 1; j--) 
					{
						gotoxy(5, 3);
						printf("%d", j);
						Sleep(1000);
					}
					gotoxy(5, 3);
					printf("START!");
					Sleep(1000);

				}
				else if (i > 34)
				{
					Sleep(20);
					gotoxy(1, 1);
					printf("<SCORE>");
					gotoxy(3, 2);
					printf("%d", score - 33);
				}
				i++;
				score++;
			} while (!strike());//닿지 않는동안 반복
			game_over();
			gotoxy(22, 17);
			printf("SCORE: %d", score - 34);
			while (1)
			{
				if (_getch() == ENTER)
					break;
				else if (_getch() == ESC)
					return 0;

			}
		}
		else if (menu_select == 1)
		{
			game_info();
		}
		else if (menu_select == 2)
			return 0;
		road_rand = 0;
		i = 0;


		score = 0;
	}


	return 0;
}
void CursorView(char show)//커서숨기기
{
	HANDLE hConsole;
	CONSOLE_CURSOR_INFO ConsoleCursor;

	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	ConsoleCursor.bVisible = show;
	ConsoleCursor.dwSize = 1;

	SetConsoleCursorInfo(hConsole, &ConsoleCursor);
}
void gotoxy(int x, int y)
{
	COORD pos = { x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
	//SetConsoleCursorPosition 함수에, 콘솔의 출력 핸들을 주고, 위치를 정해주면 커서 이동
	//GetStdHandle: 실제 핸들을 반환하는 함수
}
int keyDownis(int key)//키 다운 처리
{   //m.blog.naver.com/power2845/50143021565
   //***GetAsyncKey(현재 키 상태를 알아오는 함수)는 키가 눌렸다면 최상위 비트를 1로 설정, 이전에 키가 눌렸다면 최하위 비트를 1로 설정
   //0x8000은 최상위 비트 -> 16진수는 2비트가 4개 모인 비트 -> 0x8000 = 1000 0000 0000 0000를 의미
   // 즉 최하위 비트는 0x0001 -> 0000 0000 0000 0001
   // 따라서 (GetAsyncKeyState(key) & 0x8000) != 0 은 현재 키가 눌려있다면 참(TRUE)
	return ((GetAsyncKeyState(key) & 0x8000) != 0);
}
void Init()//초기값
{
	for (int i = 0; i < HEIGHT; i++)
	{
		snow[i].act = FALSE;
	}
	player.x = WIDTH / 2;//플레이어의 시작위치는 중앙
}
void createRoad(int road_rand)
{
	for (int i = 0; i < HEIGHT; i++)
	{
		if (!snow[i].act)//FALSE라면
		{
			snow[i].x1 = 20 + road_rand;//가로 무작위로 눈 벽 생성, 세로는 출현위치 항상 일치
			snow[i].x2 = 31 + road_rand;
			snow[i].y = HEIGHT - 1;
			snow[i].act = TRUE;//눈 벽이 출현한 인덱스[i]의 상태를 TRUE로 변경
			return; //break랑 같은역할!
		}

	}
}
void moveRoad()//길의 움직임
{
	for (int i = 0; i < HEIGHT; i++)
	{
		if (snow[i].act)//snow[i].act가 TRUE라면(해당 인덱스[i]에 눈 벽이 있으면) 움직임 실행
		{
			snow[i].y--;
		}
	}
}
void deleteRoad()//피하기 성공한 눈 벽 삭제
{
	for (int i = 0; i < HEIGHT; i++)
	{
		if (snow[i].act && snow[i].y < 0)//snow[i].act가 TRUE고 ~
			snow[i].act = FALSE;
	}
}
int strike()//눈에 닿을경우
{
	for (int i = 0; i < HEIGHT; i++)
	{//눈 벽의 상태가 TRUE고 눈의 위치가 y=0 즉 바닥이고 눈의 x축 위치가 플레이어와 같을때
		if ((snow[i].act) && (snow[i].y == 3) && ((snow[i].x1 >= player.x) || (snow[i].x2 <= player.x)))
			return TRUE;
	}
	return FALSE;//닿지 않았을 때
}
void moveplayer()
{//플레이어 이동 (좌,우)
	if (keyDownis(VK_LEFT))//왼쪽 방향키
		player.x -= 1;
	if (keyDownis(VK_RIGHT))//오른쪽 방향키
		player.x += 1;
	if (player.x < 0)
		player.x = 0;
	if (player.x > WIDTH)
		player.x = WIDTH;//위치범위 제한

}

void printmap()
{
	system("cls");
	for (int i = 0; i < HEIGHT; i++)
	{
		if (snow[i].act)//TRUE라면
		{

			gotoxy(snow[i].x1, snow[i].y);
			printf("*");//눈 벽 출력
			gotoxy(snow[i].x2, snow[i].y);
			printf("*");

		}
	}
	gotoxy(player.x, 3);//플레이어 출력
	printf("l0l");

}
void consol()
{
	system("mode con cols=50 lines=40");
	CursorView(0);
	printf("\n\n\n\n");
	printf("          00000000      0   00     00000 \n");
	printf("         0              0 00         0   \n");
	printf("         00000000       00           0   \n");
	printf("                 0      0 00         0   \n");
	printf("         00000000       0   00     00000 \n\n\n\n\n");
	printf("      000000        0        0   0      00000   \n");
	printf("     0             0 0      0 0 0 0     0       \n");
	printf("     0   0000     00000    0   0   0    0000    \n");
	printf("     0      0     0   0    0       0    0       \n");
	printf("      000000      0   0    0       0    00000   \n");
}
int drawmenu()
{
	int x = 20; int y = 20;

	gotoxy(x, y);
	printf("게임 시작");
	gotoxy(x, y + 1);
	printf("게임 설명");
	gotoxy(x, y + 2);
	printf("  종 료  ");

	gotoxy(x - 2, y);
	printf("▶");

	while (1)
	{
		switch (_getch())
		{
		case UP:
			if (y > 20)
			{
				gotoxy(x - 2, y);
				printf("  ");
				gotoxy(x - 2, --y);
				printf("▶");
			}
			break;
		case DOWN:
			if (y < 22)
			{
				gotoxy(x - 2, y);
				printf("  ");
				gotoxy(x - 2, ++y);
				printf("▶");
			}
			break;
		case ENTER:
			return y - 20;
		}
	}	
}

void game_info()
{
	system("cls");
	printf("\n\n\n\n\n\n\n\n");
	printf("                    [ 조작법 ]\n\n");
	printf("                 방향키를 이용하여 \n ");
	printf("              캐릭터를 좌, 우로 이동\n\n\n");
	printf("                    [ 선 택 ]\n\n");
	printf("                     엔   터\n\n\n");
	printf("                    [ 규 칙 ]\n\n");
	printf("           캐릭터가 벽에 닿을 시 게임 오버\n\n\n\n");
	printf("     (엔터를 누르면 이전 화면으로 돌아갑니다.)");
	while (1)
	{
		if (_getch() == ENTER)
			break;
	}
}
void game_over()
{
	gotoxy(15, 13);
	printf("+---------------------+");
	gotoxy(15, 14);
	printf("|      GAME OVER!     |");
	gotoxy(15, 15);
	printf("+---------------------+");
	gotoxy(15, 17);
	printf("                   ");
	gotoxy(12, 19);
	printf("PRESS ENTER KEY TO GO TO MENU");
	gotoxy(12, 21);
	printf("  PRESS ESC KEY TO END GAME");
}