#include<stdio.h>
#include<stdlib.h>
#include<Windows.h>
#include<conio.h>
#include<time.h>
#define SNAKESIZE 100
#define MAPWIDTH 78
#define MAPHEIGHT 24 
#ifdef _cplusplus
typedef char bool;
#define false 0
#define true 1
#endif // _cplusplus
//食物的属性
struct 
{
	int x; int y;
}food;
struct 
{
	int speed;
	int len;
	int x[SNAKESIZE];
	int y[SNAKESIZE];
}snake;
//蛇的属性如上
//将光标移动到控制台的(x,y)坐标处
void gotoxy(int x, int y) {
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
//绘制游戏边框
void drawMap();
//随机生成食物
void createFood();
//按键操作
void keyDown();
//蛇的状态
bool snakeStatus();
//从控制台移动光标
void gotoxy(int x, int y);
int key = 72;//表示蛇移动的方向, 72为"⬆"代表的数字
//判断蛇是否吃掉食物
int changeFlag = 0;
//记录得分
int score = 0;
int i;
void drawMap() {
	for (i = 0; i < MAPWIDTH; i += 2) {
		//将光标依次移动到(i,0)处
		gotoxy(i, 0);
		printf("⬜");
		//再去填满(i,MAXXHEIGHT)
		gotoxy(i, MAPHEIGHT);
		printf("⬜");
	}
	//打印左右边框
	for (i = 1; i < MAPHEIGHT; i++) {
		gotoxy(0, i);
		printf("⬜");
		gotoxy(MAPWIDTH,i);
		printf("⬜");
	}
	//随机生成初始食物(答辩)
	while (true) {
		srand((unsigned int)time(NULL));
		food.x = rand() % (MAPWIDTH - 4) + 2;
		food.y = rand() % (MAPHEIGHT - 2) + 1;
		//这里有个细节，生成的食物横坐标的奇偶必须和初始的蛇头的坐标的奇偶一致，因为一个字占两位
		if (food.x % 2 == 0) {
			break;
		}
	}
	//移动光标到食物处
	gotoxy(food.x, food.y);
	printf("💩");
	//初始化蛇
	snake.len = 3;
	snake.speed = 200;
	//在屏幕中间生成蛇头
	snake.x[0] = MAPWIDTH / 2 + 1;
	snake.y[0] = MAPHEIGHT / 2;
	//打印蛇头
	gotoxy(snake.x[0], snake.y[0]);
	printf("⚕️");
	//生成蛇身
	for (i = 1; i < snake.len; i++) {
		//打印蛇身，纵坐标不变，横坐标为上一节舍身的坐标值+2
		snake.x[i] = snake.x[i - 1] + 2;
		snake.y[i] = snake.y[i - 1];
		gotoxy(snake.x[i], snake.y[i]);
		printf("💩");
	}
	//打印玩蛇身后将光标移动到屏幕最上方
	gotoxy(MAPWIDTH - 2, 0);
	return;
}
void keyDown() {
	int pre_key = key;
	if (_kbhit()) {
		fflush(stdin);//清空缓冲区
		key = _getch();
		key = _getch();//
	}
	/*蛇移动的时候先擦去蛇尾一节
	* changeFlag相当于bool用于判断蛇是否吃到食物
	* 每走一步就擦去蛇尾，借此营造移动的状态
	* 若吃到食物，则不需要擦除，营造一个舍身增长的效果
	*/
	if (changeFlag == 0) {
		gotoxy(snake.x[snake.len - 1], snake.y[snake.len - 1]);
		printf(" ");//actually, the body of the snake is still there.
	}
	for (i = snake.len - 1; i > 0; i--) {//移动蛇身
		snake.x[i] = snake.x[i - 1];
		snake.y[i] = snake.y[i - 1];
	}
	//舍身移动方向不能和前一次相反，如果键入相反方向，则改变方向为上一次的方向
	if (pre_key == 72 && key == 80)
		key == 72;
	if (pre_key == 80 && key == 72)
		key = 80;
	if (pre_key == 75 && key == 77)
		key == 75;
	if (pre_key == 77 && key == 75)
		key == 77;
	switch (key)
	{
	case 75:
		snake.x[0] -= 2;
		break;
	case 77:
		snake.x[0] += 2;
		break;
	case 72:
		snake.y[0] --;
		break;
	case 80:
		snake.y[0] ++;
		break;
	default:
		break;
	}
	//打印蛇头
	gotoxy(snake.x[0], snake.y[0]);
}


void createFood() {
	if (snake.x[0] == food.x && snake.y[0] == food.y) {
		//判断蛇头是否接触食物
		while (1) {
			int flag = 1;
			srand((unsigned int)time(NULL));
			food.x = rand() % (MAPWIDTH - 4) - 2;
			food.y = rand() % (MAPHEIGHT - 2) - 1;
			for (i = 0; i < snake.len; i++) {
				if (snake.x[i] == food.x && snake.y[i] == food.y) {
					flag = 0;
					break;
				}
			}
			if (flag && food.x % 2 == 0)
				break;
		}
		gotoxy(food.x, food.y);
		printf("💩");
		snake.len++;
		score++;
		snake.speed -= 5;
		changeFlag = 1;//吃了变为true
	}
	return;
}
bool snakeStatus() {
	if (snake.x[0] == 0 || snake.x[0] == MAPWIDTH) {
		return false;
	}
	if (snake.y[0] == 0 || snake.y[0] == MAPHEIGHT)
		return false;
	for (i = 1; i < snake.len; i++) {
		if (snake.x[i] == snake.x[0] && snake.y[0] == snake.y[i])
			return false;
	}
	return true;
}
int main() {
	drawMap();
	while (true) {
		keyDown();
		if (!snakeStatus())
			break;
		createFood();
		Sleep(snake.speed);
	}
	gotoxy(MAPWIDTH / 2, MAPHEIGHT / 2);
	printf("game over\n");
	gotoxy(MAPWIDTH / 2, MAPHEIGHT / 2 + 1);
	printf("本次游戏得分: %d\n", score);
}



































