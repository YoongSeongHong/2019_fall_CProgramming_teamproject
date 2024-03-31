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

void CursorView(char show);//Ŀ�����ֱ�
void consol();//�ܼ�â ũ��
int drawmenu();//start menu
void game_info();//���Ӽ���
void gotoxy(int x, int y);
int keyDownis(int key);//Ű �ٿ�ó��
void Init();//�ʱⰪ
void createRoad(int road_rand);
void moveRoad();//�� ������
void deleteRoad();
int strike();
void moveplayer();
void printmap();
void game_over();

typedef struct
{
	int x1;//���� ��
	int x2;//������ ��
	int y;
	int act;
}Snow;
typedef struct
{
	int x;
}Player;//�÷��̾� ����ü

Snow snow[HEIGHT];//�迭���� ����ü ����
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
			} while (!strike());//���� �ʴµ��� �ݺ�
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
void CursorView(char show)//Ŀ�������
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
	//SetConsoleCursorPosition �Լ���, �ܼ��� ��� �ڵ��� �ְ�, ��ġ�� �����ָ� Ŀ�� �̵�
	//GetStdHandle: ���� �ڵ��� ��ȯ�ϴ� �Լ�
}
int keyDownis(int key)//Ű �ٿ� ó��
{   //m.blog.naver.com/power2845/50143021565
   //***GetAsyncKey(���� Ű ���¸� �˾ƿ��� �Լ�)�� Ű�� ���ȴٸ� �ֻ��� ��Ʈ�� 1�� ����, ������ Ű�� ���ȴٸ� ������ ��Ʈ�� 1�� ����
   //0x8000�� �ֻ��� ��Ʈ -> 16������ 2��Ʈ�� 4�� ���� ��Ʈ -> 0x8000 = 1000 0000 0000 0000�� �ǹ�
   // �� ������ ��Ʈ�� 0x0001 -> 0000 0000 0000 0001
   // ���� (GetAsyncKeyState(key) & 0x8000) != 0 �� ���� Ű�� �����ִٸ� ��(TRUE)
	return ((GetAsyncKeyState(key) & 0x8000) != 0);
}
void Init()//�ʱⰪ
{
	for (int i = 0; i < HEIGHT; i++)
	{
		snow[i].act = FALSE;
	}
	player.x = WIDTH / 2;//�÷��̾��� ������ġ�� �߾�
}
void createRoad(int road_rand)
{
	for (int i = 0; i < HEIGHT; i++)
	{
		if (!snow[i].act)//FALSE���
		{
			snow[i].x1 = 20 + road_rand;//���� �������� �� �� ����, ���δ� ������ġ �׻� ��ġ
			snow[i].x2 = 31 + road_rand;
			snow[i].y = HEIGHT - 1;
			snow[i].act = TRUE;//�� ���� ������ �ε���[i]�� ���¸� TRUE�� ����
			return; //break�� ��������!
		}

	}
}
void moveRoad()//���� ������
{
	for (int i = 0; i < HEIGHT; i++)
	{
		if (snow[i].act)//snow[i].act�� TRUE���(�ش� �ε���[i]�� �� ���� ������) ������ ����
		{
			snow[i].y--;
		}
	}
}
void deleteRoad()//���ϱ� ������ �� �� ����
{
	for (int i = 0; i < HEIGHT; i++)
	{
		if (snow[i].act && snow[i].y < 0)//snow[i].act�� TRUE�� ~
			snow[i].act = FALSE;
	}
}
int strike()//���� �������
{
	for (int i = 0; i < HEIGHT; i++)
	{//�� ���� ���°� TRUE�� ���� ��ġ�� y=0 �� �ٴ��̰� ���� x�� ��ġ�� �÷��̾�� ������
		if ((snow[i].act) && (snow[i].y == 3) && ((snow[i].x1 >= player.x) || (snow[i].x2 <= player.x)))
			return TRUE;
	}
	return FALSE;//���� �ʾ��� ��
}
void moveplayer()
{//�÷��̾� �̵� (��,��)
	if (keyDownis(VK_LEFT))//���� ����Ű
		player.x -= 1;
	if (keyDownis(VK_RIGHT))//������ ����Ű
		player.x += 1;
	if (player.x < 0)
		player.x = 0;
	if (player.x > WIDTH)
		player.x = WIDTH;//��ġ���� ����

}

void printmap()
{
	system("cls");
	for (int i = 0; i < HEIGHT; i++)
	{
		if (snow[i].act)//TRUE���
		{

			gotoxy(snow[i].x1, snow[i].y);
			printf("*");//�� �� ���
			gotoxy(snow[i].x2, snow[i].y);
			printf("*");

		}
	}
	gotoxy(player.x, 3);//�÷��̾� ���
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
	printf("���� ����");
	gotoxy(x, y + 1);
	printf("���� ����");
	gotoxy(x, y + 2);
	printf("  �� ��  ");

	gotoxy(x - 2, y);
	printf("��");

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
				printf("��");
			}
			break;
		case DOWN:
			if (y < 22)
			{
				gotoxy(x - 2, y);
				printf("  ");
				gotoxy(x - 2, ++y);
				printf("��");
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
	printf("                    [ ���۹� ]\n\n");
	printf("                 ����Ű�� �̿��Ͽ� \n ");
	printf("              ĳ���͸� ��, ��� �̵�\n\n\n");
	printf("                    [ �� �� ]\n\n");
	printf("                     ��   ��\n\n\n");
	printf("                    [ �� Ģ ]\n\n");
	printf("           ĳ���Ͱ� ���� ���� �� ���� ����\n\n\n\n");
	printf("     (���͸� ������ ���� ȭ������ ���ư��ϴ�.)");
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