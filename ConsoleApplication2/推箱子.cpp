#include <iostream>
#include <graphics.h>
#include <conio.h>
#include <Windows.h>
#include <string>
#include "database.h"

using namespace std;


#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 650
#define LINE 9
#define COLUMN 12
#define RATIO 50
#define START_X 100
#define START_Y 100
#define KEY_UP 'w' 
#define KEY_LEFT 'a'
#define KEY_RIGHT 'd'
#define KEY_DOWN 's'
#define KEY_QUIT 'q'
typedef struct _POS POS;
typedef enum _DIRECTION DIRECTION;
typedef enum _PROPS PROPS;

int map[LINE][COLUMN] = {
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0 },
	{ 0, 1, 4, 1, 0, 2, 1, 0, 2, 1, 0, 0 },
	{ 0, 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 0 },
	{ 0, 1, 0, 2, 0, 1, 1, 4, 1, 1, 1, 0 },
	{ 0, 1, 1, 1, 0, 3, 1, 1, 1, 4, 1, 0 },
	{ 0, 1, 2, 1, 1, 4, 1, 1, 1, 1, 1, 0 },
	{ 0, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};

/*0,0,0,0,0,0,0,0,0,0,0,0|0,1,0,1,1,1,1,1,1,1,0,0|0,1.4,1,0,2,1,0,2,1,0,0|0,1,0,1,0,1,0,0,1,1,1,0|0,1,0,2,0,1,1,4,1,1,1,0|0,1,1,1,0,3,1,1,1,4,1,0|0,1,2,1,1.4,1,1,1,1,1,0|0,1,0,0,1,0,1,1,0.0,1,0|0,0,0,0,0,0,0,0,0,0,0,0*/
int past = 0;      //记录GGBOND本来在的地方是什么方块

enum _PROPS {
	WALL,
	FLOOR,
	BOX_DES,
	GGBOND,
	BOX,
	HIT,
	ALL
};
enum _DIRECTION {
	UP,
	DOWN,
	LEFT,
	RIGHT
};

struct _POS {     //坐标
	int y;
	int x;
};
POS ggbond_POS;
POS next_POS, next_next_POS;//小人的位置
IMAGE images[ALL];   //定义图形数组（6个素材）
IMAGE bg_img, bg_over_img, bg_start_img;   //定义图形变量（背景）
void changeMap(POS *pos, PROPS prop)
{
	map[pos->y][pos->x] = prop;
	putimage(START_X + pos->x * RATIO, START_Y + pos->y * RATIO, &images[prop]);
}
void gameControl(DIRECTION direct)
{
	switch (direct)
	{
	case UP:
		next_POS.y = ggbond_POS.y - 1;
		next_POS.x = ggbond_POS.x;
		
		next_next_POS.y = ggbond_POS.y - 2;
		next_next_POS.x = ggbond_POS.x;
		break;

	case DOWN:
		next_POS.y = ggbond_POS.y + 1;
		next_POS.x = ggbond_POS.x;
		
		next_next_POS.y = ggbond_POS.y + 2;
		next_next_POS.x = ggbond_POS.x;
		break;

	case LEFT:
		next_POS.y = ggbond_POS.y;
		next_POS.x = ggbond_POS.x - 1;
		
		next_next_POS.y = ggbond_POS.y;
		next_next_POS.x = ggbond_POS.x - 2;
		break;

	case RIGHT:
		next_POS.y = ggbond_POS.y;
		next_POS.x = ggbond_POS.x+1;
		
		next_next_POS.y = ggbond_POS.y;
		next_next_POS.x = ggbond_POS.x + 2;
		break;
	}

	if (map[next_POS.y][next_POS.x] == FLOOR)
	{	
		changeMap(&next_POS, GGBOND);
		if (past == BOX_DES)
		{
			changeMap(&ggbond_POS, BOX_DES);
		}
		else { changeMap(&ggbond_POS, FLOOR); }
		ggbond_POS = next_POS;
		past = 0;
	}
	else if (map[next_POS.y][next_POS.x] == BOX_DES)
	{
		past = BOX_DES;
		changeMap(&next_POS, GGBOND);
		changeMap(&ggbond_POS, FLOOR);
		changeMap(&ggbond_POS, FLOOR);
		ggbond_POS = next_POS;
	}
	else if (map[next_POS.y][next_POS.x] == BOX)
	{
		if (map[next_next_POS.y][next_next_POS.x] == FLOOR)
		{
			changeMap(&next_next_POS, BOX);
			changeMap(&next_POS, GGBOND);
			changeMap(&ggbond_POS, FLOOR);
			ggbond_POS = next_POS;
		}
		else if (map[next_next_POS.y][next_next_POS.x] == BOX_DES)
		{
			changeMap(&next_next_POS, HIT);
			changeMap(&next_POS, GGBOND);
			changeMap(&ggbond_POS, FLOOR);
			ggbond_POS = next_POS;
		}
	}
}
bool isGameOver() {
	for (int i = 0; i < LINE; i++) {
		for (int j = 0; j < COLUMN; j++) {
			if (map[i][j] == BOX_DES || past == 2) return false;
		}
	}
	return true;
}


void gameStartScene(IMAGE *bg) {
	putimage(0, 0, bg);
	settextcolor(WHITE);
	RECT rec = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	settextstyle(40, 0, _T("宋体"));
	drawtext(_T("------解救GG BOND！------"),&rec, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	drawtext(_T("按下任意键以继续..."), &rec, DT_CENTER | DT_BOTTOM | DT_SINGLELINE);
}

void gameOverScene(IMAGE *bg) {
	putimage(0, 0, bg);
	settextcolor(WHITE);
	RECT rec = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	settextstyle(40, 0, _T("宋体"));
	drawtext(_T("恭喜您~ \n GG BOND向您表示感谢！"),
		&rec, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

bool login(userinfo &user)
{
	bool ret = false;
	int times = 0;

	do {
		cout << "请输入用户名：";
		cin >> user.username;
		cout << "请输入密码：";
		cin >> user.password;
		ret = fetch_user_info(user);
		//返回 bool :成功true, 失败false
		if (ret == false)
		{
			cout << "用户名或密码错误，请重新登陆！" << endl;
//			::system("pause");    //若报错system不明确，则用全局中的system(::)
			times++;
		}
		if (times == 5)
		{
			break;
		}

	} while ( !ret );
	return ret;
}

int main(void)
{
	//用户身份验证
	userinfo user;

	if (!login(user))
	{
		cout << "登陆失败， 已达最大尝试次数！" << endl;
		system("pause");
		exit(-1);
	}
	else
	{
		cout << "登陆成功， 请开始游戏！" << endl;
		system("pause");

	}

	initgraph(SCREEN_WIDTH, SCREEN_HEIGHT);   //舞台（控制台）大小

    //墙: 0，地板: 1，箱子目的地: 2, 小人: 3, 箱子: 4, 箱子命中目标: 5
	loadimage(&bg_img, "img/blackground.bmp", SCREEN_WIDTH, SCREEN_HEIGHT, true);  //将背景图加载到变量中去，“true”代表是否拉伸
	loadimage(&images[WALL], "img/wall_right.bmp", RATIO, RATIO, true);
	loadimage(&images[FLOOR], "img/floor.bmp", RATIO, RATIO, true);
	loadimage(&images[BOX_DES], "img/des.bmp", RATIO, RATIO, true);
	loadimage(&images[GGBOND], "img/ggbond.bmp", RATIO, RATIO, true);
	loadimage(&images[BOX], "img/box.bmp", RATIO, RATIO, true);
	loadimage(&images[HIT], "img/box.bmp", RATIO, RATIO, true);
	loadimage(&bg_over_img, "img/R-C.bmp", SCREEN_WIDTH, SCREEN_HEIGHT, true);
	loadimage(&bg_start_img, "img/start.bmp", SCREEN_WIDTH, SCREEN_HEIGHT, true);

	gameStartScene(&bg_start_img);

	while (1)
	{
		if (_getch())
			break;
	}

	putimage(0, 0, &bg_img);  //显示图片，传入图片左上角左边参数

	for (int i = 0; i < LINE; i++)
	{
		for (int j = 0; j < COLUMN; j++)
		{
			if (map[i][j] == GGBOND) {
				ggbond_POS.x = j;
				ggbond_POS.y = i;
			}
			putimage(START_X + j * RATIO, START_Y + i * RATIO, &images[map[i][j]]);
		}
	}
	bool quit = false;
	do {
		if (_kbhit()) {     //判断是否敲击键盘
			int ch = _getch();     //获取键值
			switch (ch) {
			    case KEY_LEFT:
					gameControl(LEFT);
					break;
				case KEY_RIGHT:
					gameControl(RIGHT);
					break;
				case KEY_DOWN:
					gameControl(DOWN);
					break;
				case KEY_UP:
					gameControl(UP);
					break;
				case KEY_QUIT:
					quit = true;
					break;

			}
		}
		Sleep(50);   //休眠（以防持续获取输入造成cpu负担）
		if (isGameOver()) {
			gameOverScene(&bg_over_img);
			quit = true;
		}
	} while (quit == false);

	system("pause");


	return 0;

}

