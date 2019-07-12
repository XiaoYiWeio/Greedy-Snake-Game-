//1750562 zhangbo
#define _CRT_SECURE_NO_WARNINGS
#include<graphics.h>	
#include<stdio.h>
#include<conio.h>			
#include<time.h>
#include<stdlib.h>
#include<windows.h>
#include<vector>
#include<iostream>
#include<fstream>
#include<string>
//#include<algorithm>
using namespace std;
#define IMAGE_LENGTH 400
#define IMAGE_WIDTH  400
#define MAX  200			//蛇的最大长度
#define SIZE 40				//蛇的宽度,移动速度
#define UP 72
#define DOWN 80
#define LEFT 75
#define RIGHT 77
IMAGE head_up, head_down, head_left, head_right, s_body, s_food, s_wall,s_food_0;

struct POS					//坐标
{
	int x;
	int y;
	bool eat;				//food用
};

struct FOOD
{
	int num;			//记录现存食物数量
	POS f_pos[5];		//食物坐标
};
//墙壁的坐标
vector<int>wall_x;
vector<int>wall_y;
//由尸体产生的食物坐标
vector<int>food_x;
vector<int>food_y;

//入门版
class SNAKE_1
{
	//private:
public:
	int length;			//蛇的节数
	POS s_pos[MAX];		//蛇每节的坐标
	char ch;			//蛇的方向
	FOOD food;
	int score = 0;			//得分
	int final[2];		//记录最后一节蛇的位置
public:
	//初始化游戏界面
	void initGame()
	{
		initgraph(IMAGE_LENGTH + SIZE * 2, IMAGE_WIDTH + SIZE * 2);							//窗口宽度，高度

		setbkcolor(WHITE);								//背景颜色调至白色
		cleardevice();									//清屏

		//加载图片
		loadimage(&head_up, L"蛇头上.jpg", SIZE, SIZE);
		loadimage(&head_down, L"蛇头下.jpg", SIZE, SIZE);
		loadimage(&head_left, L"蛇头左.jpg", SIZE, SIZE);
		loadimage(&head_right, L"蛇头右.jpg", SIZE, SIZE);
		loadimage(&s_body, L"蛇身.jpg", SIZE, SIZE);
		loadimage(&s_food, L"西瓜.jpg", SIZE, SIZE);
		loadimage(&s_wall, L"墙.jpg", SIZE, SIZE);
		loadimage(&s_food_0, L"糖果.jpg", SIZE, SIZE);
		length = 1;										//蛇的节数初始化
		srand((unsigned)time(NULL));
		s_pos[0].x = rand() % 10 * SIZE + SIZE;									//蛇的坐标
		s_pos[0].y = rand() % 10 * SIZE + SIZE;
		int direction = rand() % 4 + 1;
		switch (direction)
		{
		case 1:ch = UP; break;
		case 2:ch = DOWN; break;
		case 3:ch = LEFT; break;
		case 4:ch = RIGHT; break;
		default:break;
		}												//蛇的方向

		food.num = 0;									//未生成食物
		for (int i = 0; i < 5; i++)
			food.f_pos[i].eat = 1;						//食物未出现（已吃掉）
		for (int j = 0; j < IMAGE_WIDTH + SIZE * 2; j += SIZE)
		{
			putimage(0, j, &s_wall);
			putimage(IMAGE_LENGTH + SIZE, j, &s_wall);
		}

		for (int i = SIZE; i <= IMAGE_LENGTH; i += SIZE)
		{
			putimage(i, 0, &s_wall);
			putimage(i, IMAGE_WIDTH + SIZE, &s_wall);
		}
	}

	//加载蛇头、蛇身绘出蛇
	void SnakeDisplay()
	{
		//根据方向绘出蛇头
		switch (ch)
		{
		case UP:
			putimage(s_pos[0].x, s_pos[0].y, &head_up);			//三元光栅缺省
			break;
		case DOWN:
			putimage(s_pos[0].x, s_pos[0].y, &head_down);
			break;
		case LEFT:
			putimage(s_pos[0].x, s_pos[0].y, &head_left);
			break;
		case RIGHT:
			putimage(s_pos[0].x, s_pos[0].y, &head_right);
			break;
		default:
			break;
		}
		//绘制蛇身
		for (int i = length - 1; i > 0; i--)
		{
			putimage(s_pos[i].x, s_pos[i].y, &s_body);
		}
	}

	//控制蛇的移动
	void SnakeMove()
	{
		//移动时除蛇头外，后面每节蛇身均前移一位，直接对数组操作
		final[0] = s_pos[length - 1].x;
		final[1] = s_pos[length - 1].y;
		for (int i = length - 1; i > 0; i--)
		{
			s_pos[i].x = s_pos[i - 1].x;
			s_pos[i].y = s_pos[i - 1].y;
		}
		switch (ch)				//根据方向设定蛇头运动
		{
		case UP:
			s_pos[0].y -= SIZE;
			break;
		case DOWN:
			s_pos[0].y += SIZE;
			break;
		case LEFT:
			s_pos[0].x -= SIZE;
			break;
		case RIGHT:
			s_pos[0].x += SIZE;
			break;
		default:
			break;
		}
	}

	//在蛇死亡时退回死之前位置
	void ReSnakeMove()
	{
		for (int i = 0; i < length - 1; i++)
		{
			s_pos[i].x = s_pos[i + 1].x;
			s_pos[i].y = s_pos[i + 1].y;
		}
		s_pos[length - 1].y = final[1];
		s_pos[length - 1].x = final[0];
	}

	//方向键控制蛇的方向改变
	void SnakeDirect()
	{
		switch (_getch())
		{
		case 72:
			if (ch != DOWN)			//非下即可
				ch = UP;
			break;
		case 80:					//非上
			if (ch != UP)
				ch = DOWN;
			break;
		case 75:					//非右
			if (ch != RIGHT)
				ch = LEFT;
			break;
		case 77:					//非左
			if (ch != LEFT)
				ch = RIGHT;
			break;
		default:
			break;
		}
	}

	//查找是否坐标与蛇身重合
	bool find_in_snake(int x, int y)
	{
		for (int i = 0; i < length; i++)
			if (s_pos[i].x == x && s_pos[i].y == y)
				return TRUE;
		return FALSE;
	}
	//生成食物坐标
	virtual void CreateFOOD()
	{
		//随机数种子
		int num = 0, k = 0;
		unsigned int t = 0;
		srand((unsigned)time(NULL));
		while (k == num)
		{
			t++;
			k = 0;
			//由其注意这个时间为种子会出现OVERFLOW出现死循环
			num = rand() % 5 + 1;								//随机产生n个食物
			food.num = num;										//记录食物个数，为0时补充食物
			for (int i = 0; i < num; i++)
			{
				food.f_pos[i].x = rand() % 10 * SIZE + SIZE;			//根据地图大小定随机数范围
				food.f_pos[i].y = rand() % 10 * SIZE + SIZE;
				food.f_pos[i].eat = 0;							//未被吃
			}
			for (int i = 0; i < num; i++)
				if (food.f_pos[i].eat == 1)
					k++;
		}
		return;
	}

	//判定吃到食物
	virtual void EatFOOD()
	{
		//int num = food.num;
		for (int i = 0; i < 5; i++)			//5次判定	
		{
			if (s_pos[0].x == food.f_pos[i].x
				&&s_pos[0].y == food.f_pos[i].y
				&&food.f_pos[i].eat == 0)			//坐标重合且食物存在
			{
				length++;						//长度加1
				food.num--;						//食物数量减一
				food.f_pos[i].eat = 1;			//食物被吃
				score += 2;
			}
		}
	}

	//游戏结束输出
	virtual void EndGame(int flag)
	{
		if (flag == -1)
			return;
		cleardevice();
		settextcolor(LIGHTRED);
		settextstyle(50, 0, L"宋体");
		int X = IMAGE_LENGTH / 4;
		int Y = IMAGE_WIDTH / 4;
		if (flag == 0)
		{
			outtextxy(X - SIZE, Y, L"蛇咬到了自己！");

		}
		else if (flag == 1)
		{
			outtextxy(X - SIZE, Y, L"蛇咬到了墙壁！");

		}
		else if (flag == 2)
		{
			outtextxy(X - SIZE, Y, L"你可真厉害！没空间啦！");
		}
		outtextxy(X - SIZE, Y + 50, L"最终得分为:");
		char temp[50];
		_itoa(score, temp, 10);
		int n = MultiByteToWideChar(0, 0, temp, -1, NULL, 0);
		wchar_t *str = new wchar_t[n];
		MultiByteToWideChar(0, 0, temp, -1, str, n);
		outtextxy(X + 300, Y + 50, str);				//出现问题

		int k;
		ReadFile(k);
		if (score <= k)
			outtextxy(X - SIZE, Y + 100, L"请再接再厉！");
		else
		{
			outtextxy(X - SIZE, Y + 100, L"恭喜你打破纪录！");
			WriteFile(score);
		}
		char ch;
		ch = _getch();


		PrintFile();
		exit(0);
	}

	//判断游戏结束
	virtual	int GameOver()
	{
		//蛇头撞到墙壁
		if (s_pos[0].x < SIZE || s_pos[0].x >= IMAGE_LENGTH + SIZE ||
			s_pos[0].y < SIZE || s_pos[0].y >= IMAGE_WIDTH + SIZE)
		{
			return(1);
		}
		//蛇头撞到了蛇身
		for (int i = length - 1; i > 0; i--)
		{
			if (s_pos[0].x == s_pos[i].x
				&&s_pos[0].y == s_pos[i].y)
			{
				return(0);
			}
		}
		return -1;			//未失败
	}

	//打印食物
	virtual void DisplayFOOD()
	{
		for (int i = 0; i < 5; i++)
		{
			if (food.f_pos[i].eat == 0)
				putimage(food.f_pos[i].x, food.f_pos[i].y, &s_food);
		}
		settextcolor(BLACK);
		settextstyle(15, 0, L"宋体");
		outtextxy(SIZE, IMAGE_WIDTH + SIZE, L"当前得分为：");
		char temp[50];
		_itoa(score, temp, 10);
		int n = MultiByteToWideChar(0, 0, temp, -1, NULL, 0);
		wchar_t *str = new wchar_t[n];
		MultiByteToWideChar(0, 0, temp, -1, str, n);
		outtextxy(SIZE * 3 + 15, IMAGE_WIDTH + SIZE, str);				//出现问题


		outtextxy(SIZE, IMAGE_WIDTH + SIZE + 15, L"蛇的长度为：");
		//char temp[50];
		_itoa(length, temp, 10);
		n = MultiByteToWideChar(0, 0, temp, -1, NULL, 0);
		str = new wchar_t[n];
		MultiByteToWideChar(0, 0, temp, -1, str, n);
		outtextxy(SIZE * 3 + 15, IMAGE_WIDTH + SIZE + 15, str);				//出现问题


		outtextxy(SIZE * 4, IMAGE_WIDTH + SIZE, L"当前版本最高分数为：");
		//char temp[50];
		//dai gao
		int k;
		ReadFile(k);
		_itoa(k, temp, 10);
		n = MultiByteToWideChar(0, 0, temp, -1, NULL, 0);
		str = new wchar_t[n];
		MultiByteToWideChar(0, 0, temp, -1, str, n);
		outtextxy(SIZE * 8, IMAGE_WIDTH + SIZE, str);				//出现问题
	}

	//打印文件内容
	void PrintFile()
	{
		ifstream in("record.txt", ios::binary);
		string x[12];
		for (int i = 0; i < 12; i += 3)
		{
			in >> x[i] >> x[i + 1] >> x[i + 2];
			cout << x[i] << " " << x[i + 1] << " " << x[i + 2] << endl;
		}
		_getch();
		in.close();
	}

	//取出版本历史最高数据
	virtual void ReadFile(int &k)
	{

		//对于snake_1 
		ifstream in("record.txt", ios::binary);
		string x[3];
		char temp;
		int i;
		in >> x[0] >> x[1];
		for (i = 0; i < 6; i++)
			in >> temp;
		in >> k;
	}

	void replaceString(string &origin, string old_value, string new_value) {
		//int k = 0;
		/* 该函数用于对origin全文查找出现的old_value 并用new_value替换 */
		for (string::size_type pos(0); pos != string::npos; pos += new_value.length()) {
			if (((pos = origin.find(old_value, pos)) != string::npos))
				origin.replace(pos, old_value.length(), new_value);
			else break;
		}
	}
	//更新历史最高记录
	void WriteFile(int score)
	{
		ifstream out("record.txt", ios::binary);
		//string str;

		istreambuf_iterator<char> beg(out), end;              // 设置两个文件指针，指向开始和结束，以char(一字节)为步长
		string content(beg, end);                             // 将文件全部读入string字符串			
		out.close();
		int k;
		char temp_k[10], temp_score[10];
		ReadFile(k);
		_itoa(k, temp_k, 10);
		_itoa(score, temp_score, 10);
		replaceString(content, temp_k, temp_score);
		ofstream fout("record.txt", ios::binary);
		fout << content;
		fout.close();
	}
	//显示墙
	virtual void DisplayWall()
	{
		for (int j = 0; j < IMAGE_WIDTH + SIZE * 2; j += SIZE)
		{
			putimage(0, j, &s_wall);
			putimage(IMAGE_LENGTH + SIZE, j, &s_wall);
		}

		for (int i = SIZE; i <= IMAGE_LENGTH; i += SIZE)
		{
			putimage(i, 0, &s_wall);
			putimage(i, IMAGE_WIDTH + SIZE, &s_wall);
		}
	}

	//游戏循环
	virtual	void loop()
	{
		CreateFOOD();
		DisplayFOOD();			//显示食物
		SnakeDisplay();
		_getch();
		while (1)
		{
			//_getch();
			while (!_kbhit())			//当按下按键退出循环
			{
				BeginBatchDraw();
				if (food.num == 0)
					CreateFOOD();
				cleardevice();			//刷新屏幕
				DisplayWall();
				DisplayFOOD();
				SnakeMove();			//蛇的移动
				EatFOOD();
				int flag = GameOver();
				if (flag != -1)
					ReSnakeMove();
				SnakeDisplay();
				EndBatchDraw();
				Sleep(200);
				EndGame(flag);
			}
			SnakeDirect();
		}
	}
	//等待按键
	void wait_for_enter()
	{
		cout << endl << "按回车键继续";
		while (_getch() != '\r')
			;
		cout << endl << endl;
	}


};


//进阶版
class SNAKE_2 :public SNAKE_1
{
public:
	void ReadFile(int &k)
	{

		//对于snake_2 
		ifstream in("record.txt", ios::binary);
		string x[3];
		char temp;
		int i;
		in >> x[0] >> x[1] >> x[2] >> x[0] >> x[1];
		for (i = 0; i < 6; i++)
			in >> temp;
		in >> k;
	}
	bool find_in_wall(int x, int y)
	{
		//查找是否新生成的坐标与墙壁重合
		for (unsigned int i = 0; i < wall_x.size(); i++)
			if (wall_x[i] == x && wall_y[i] == y)
				return TRUE;
		return FALSE;
	}

	void ReInit(int flag) //flag= 0 1 时执行操作
	{
		//若蛇挂掉，将蛇身变成墙壁,将蛇身添加进入wall数组
		if (flag == 0 || flag == 1)
		{
			//记录墙壁坐标
			for (int i = 0; i < length; i++)
			{
				wall_x.push_back(s_pos[i].x);
				wall_y.push_back(s_pos[i].y);
			}


			//随机生成新蛇与新果实（避开墙壁）
			unsigned int t = 0;

			while (find_in_wall(s_pos[0].x, s_pos[0].y))		//重新生成
			{
				t++;
				srand((unsigned)time(NULL) + t);
				s_pos[0].x = rand() % 10 * SIZE + SIZE;
				s_pos[0].y = rand() % 10 * SIZE + SIZE;
			}
			//随即生成蛇的方向
			int direction = rand() % 4 + 1;
			switch (direction)
			{
			case 1:ch = UP; break;
			case 2:ch = DOWN; break;
			case 3:ch = LEFT; break;
			case 4:ch = RIGHT; break;
			default:break;
			}
			food.num = 0;
			length = 1;
		}
	}
	//判断游戏结束  需要修改
	//游戏结束输出
	void EndGame(int flag)			//-2时执行操作
	{
		if (flag != -2)
		{
			return;
		}
		BeginBatchDraw();
		cleardevice();			//刷新屏幕
		DisplayFOOD();
		SnakeDisplay();
		DisplayWall();
		EndBatchDraw();
		Sleep(100);
		cleardevice();
		settextcolor(LIGHTRED);
		settextstyle(SIZE, 0, L"宋体");
		int X = 1 * SIZE;
		int Y = 4 * SIZE;
		outtextxy(X - SIZE, Y + 50, L"最终得分为:");
		char temp[50];
		_itoa(score, temp, 10);
		int n = MultiByteToWideChar(0, 0, temp, -1, NULL, 0);
		wchar_t *str = new wchar_t[n];
		MultiByteToWideChar(0, 0, temp, -1, str, n);
		outtextxy(X + 300, Y + 50, str);				//出现问题

		int k;
		ReadFile(k);
		if (score <= k)
			outtextxy(X - SIZE, Y + 100, L"请再接再厉！");
		else
		{
			outtextxy(X - SIZE, Y + 100, L"恭喜你打破纪录！");
			WriteFile(score);
		}
		char ch;
		ch = _getch();


		PrintFile();
		exit(0);

	}

	int GameOver()		//撞墙 1 撞自己 0 不撞 -1 结束游戏-2
	{
		int flag = -1;
		//蛇头撞到墙壁
		if (s_pos[0].x < SIZE || s_pos[0].x >= IMAGE_LENGTH + SIZE ||
			s_pos[0].y < SIZE || s_pos[0].y >= IMAGE_WIDTH + SIZE ||
			find_in_wall(s_pos[0].x, s_pos[0].y))
		{
			flag = 1;
		}
		//蛇头撞到了蛇身
		else
		{
			for (int i = length - 1; i > 0; i--)
			{
				if (s_pos[0].x == s_pos[i].x
					&&s_pos[0].y == s_pos[i].y)
				{
					flag = 0;
					break;
				}
			}
		}
		unsigned int temp = (IMAGE_LENGTH / SIZE) * (IMAGE_WIDTH / SIZE);
		if (temp == wall_x.size())												//当没有空间产生果实和蛇头时结束游戏
			flag = -2;
		return flag;
	}

	void CreateFOOD()
	{
		//随机数种子
		int num = 0, k = 0, t = 0;
		while (k == num)
		{
			t++;
			k = 0;
			srand((unsigned)time(NULL) + t);						//由其注意这个时间为种子会出现OVERFLOW出现死循环
			num = rand() % 5 + 1;								//随机产生n个食物
			food.num = num;										//记录食物个数，为0时补充食物
			for (int i = 0; i < num; i++)
			{
				food.f_pos[i].x = rand() % 10 * SIZE + SIZE;			//根据地图大小定随机数范围
				food.f_pos[i].y = rand() % 10 * SIZE + SIZE;
				food.f_pos[i].eat = 0;							//未被吃
				if (find_in_wall(food.f_pos[i].x, food.f_pos[i].y))		//记录下与墙重叠的果实（包括重复的果实）
				{
					food.f_pos[i].eat = 1;
				}
			}
			for (int i = 0; i < num; i++)
				if (food.f_pos[i].eat == 1)
					k++;
		}
		return;
	}

	void DisplayWall()
	{
		for (int j = 0; j < IMAGE_WIDTH + SIZE * 2; j += SIZE)
		{
			putimage(0, j, &s_wall);
			putimage(IMAGE_LENGTH + SIZE, j, &s_wall);
		}

		for (int i = SIZE; i <= IMAGE_LENGTH; i += SIZE)
		{
			putimage(i, 0, &s_wall);
			putimage(i, IMAGE_WIDTH + SIZE, &s_wall);
		}
		//记录每一次死亡后蛇身的坐标用于显示墙
		for (unsigned int i = 0; i < wall_x.size(); i++)
		{
			putimage(wall_x[i], wall_y[i], &s_wall);				//加载墙的图片
		}


		settextcolor(BLACK);
		settextstyle(15, 0, L"宋体");
		outtextxy(SIZE, IMAGE_WIDTH + SIZE, L"当前得分为：");
		char temp[50];
		_itoa(score, temp, 10);
		int n = MultiByteToWideChar(0, 0, temp, -1, NULL, 0);
		wchar_t *str = new wchar_t[n];
		MultiByteToWideChar(0, 0, temp, -1, str, n);
		outtextxy(SIZE * 3 + 15, IMAGE_WIDTH + SIZE, str);				//出现问题


		outtextxy(SIZE, IMAGE_WIDTH + SIZE + 15, L"蛇的长度为：");
		//char temp[50];
		_itoa(length, temp, 10);
		n = MultiByteToWideChar(0, 0, temp, -1, NULL, 0);
		str = new wchar_t[n];
		MultiByteToWideChar(0, 0, temp, -1, str, n);
		outtextxy(SIZE * 3 + 15, IMAGE_WIDTH + SIZE + 15, str);				//出现问题

		outtextxy(SIZE * 4, IMAGE_WIDTH + SIZE, L"当前版本最高分数为：");
		//char temp[50];
		//dai gao
		int k;
		ReadFile(k);
		_itoa(k, temp, 10);
		n = MultiByteToWideChar(0, 0, temp, -1, NULL, 0);
		str = new wchar_t[n];
		MultiByteToWideChar(0, 0, temp, -1, str, n);
		outtextxy(SIZE * 8, IMAGE_WIDTH + SIZE, str);				//出现问题
	}

	void loop()
	{
		CreateFOOD();
		DisplayFOOD();			//显示食物
		SnakeDisplay();
		_getch();
		//int k = 0;
		while (1)
		{
			//_getch();
			while (!_kbhit())			//当按下按键退出循环
			{
				BeginBatchDraw();
				if (food.num == 0)
					CreateFOOD();
				cleardevice();			//刷新屏幕
				DisplayFOOD();
				SnakeMove();			//蛇的移动

				//k++;
				EatFOOD();
				int flag = GameOver();
				if (flag != -1)
					ReSnakeMove();
				ReInit(flag);
				SnakeDisplay();
				DisplayWall();
				EndBatchDraw();

				Sleep(200);
				EndGame(flag);
			}
			SnakeDirect();
		}

	}

};





//高级版
//思路：从进阶版进化而来，在挂掉的时候记录下蛇身坐标然后生成食物当食物再次被吃完的时候再次随即生成食物。不断检测撞墙次数，到达五的时候结束调用EndGame
class SNAKE_3 :public SNAKE_1
{
public:
	void ReadFile(int &k)
	{

		//对于snake_1 
		ifstream in("record.txt", ios::binary);
		string x[3];
		char temp;
		int i;
		in >> x[0] >> x[1] >> x[2]
			>> x[0] >> x[1] >> x[2]
			>> x[0] >> x[1];
		for (i = 0; i < 6; i++)
			in >> temp;
		in >> k;
	}
	bool find_in_food(int x, int y)
	{
		//查找是否新生成的坐标与墙壁重合
		for (unsigned int i = 0; i < food_x.size(); i++)
			if (food_x[i] == x && food_y[i] == y)
				return TRUE;
		return FALSE;
	}
	void ReInit(int flag)
	{
		//若蛇挂掉，将蛇身变成墙壁,将蛇身添加进入wall数组
		if (flag == 0 || flag == 1)
		{
			//记录新生成果子坐标
			for (int i = 0; i < length; i++)
			{
				food_x.push_back(s_pos[i].x);
				food_y.push_back(s_pos[i].y);
			}
			//随机生成新蛇与新果实（避开墙壁）
			//unsigned int t = 0;
			srand((unsigned)time(NULL));
			while (find_in_food(s_pos[0].x, s_pos[0].y))		//重新生成
			{
				
				
				s_pos[0].x = rand() % 10 * SIZE + SIZE;
				s_pos[0].y = rand() % 10 * SIZE + SIZE;
			}
			//随即生成蛇的方向

			//随即生成蛇的方向
			int direction = rand() % 4 + 1;
			switch (direction)
			{
			case 1:ch = UP; break;
			case 2:ch = DOWN; break;
			case 3:ch = LEFT; break;
			case 4:ch = RIGHT; break;
			default:break;
			}
			food.num = 0;
			length = 1;
		}
	}

	void CreateFOOD()
	{
		//随机数种子
		int num = 0, k = 1, t = 0;
		srand((unsigned)time(NULL) );
		while (k != 0)
		{
			//t++;
			k = 0;
									//由其注意这个时间为种子会出现OVERFLOW出现死循环
			num = rand() % 5 + 1;								//随机产生n个食物
			food.num = num;										//记录食物个数，为0时补充食物
			for (int i = 0; i < num; i++)
			{
				food.f_pos[i].x = rand() % 10 * SIZE + SIZE;			//根据地图大小定随机数范围
				food.f_pos[i].y = rand() % 10 * SIZE + SIZE;
				food.f_pos[i].eat = 0;							//未被吃
				if (find_in_food(food.f_pos[i].x, food.f_pos[i].y))		//记录下与尸体重叠的果实（包括重复的果实）
				{
					food.f_pos[i].eat = 1;
				}
			}
			for (int i = 0; i < num; i++)
				if (food.f_pos[i].eat == 1)
					k++;
		}
		return;
	}
	void EatFOOD()
	{
		for (int i = 0; i < 5; i++)			//5次判定	
		{
			if (s_pos[0].x == food.f_pos[i].x
				&&s_pos[0].y == food.f_pos[i].y
				&&food.f_pos[i].eat == 0)			//坐标重合且食物存在
			{
				length++;						//长度加1
				food.num--;						//食物数量减一
				food.f_pos[i].eat = 1;			//食物被吃
				score += 2;
			}
		}
		/*
		for (unsigned int i = 0; i < food_x.size(); i++)
		{
			if (s_pos[0].x == food_x[i]
				&& s_pos[0].y == food_y[i])
			{
				length++;						//长度加1	
				score += 1;
				food_x.erase(food_x.begin()+i);
				food_y.erase(food_y.begin()+i);
			}
		}*/
		vector<int>::iterator iter_x = food_x.begin();
		vector<int>::iterator iter_y = food_y.begin();
		for ( ;iter_x != food_x.end(); )
		{
			if (*iter_x == s_pos[0].x && (*iter_y) == s_pos[0].y)		//如果重合
			{
				iter_x = food_x.erase(iter_x);							//擦去同时记下该处
				iter_y = food_y.erase(iter_y);							//避免出现值相同时被忽略的问题
				length++;
				score++;
			}
			else
			{
				iter_x++;
				iter_y++;
			}
		}
	}
	//打印食物 
	void DisplayFOOD()
	{
		for (int i = 0; i < 5; i++)
		{
			if (food.f_pos[i].eat == 0)
				putimage(food.f_pos[i].x, food.f_pos[i].y, &s_food);
		}
		for (unsigned int i = 0; i < food_x.size(); i++)
		{
			putimage(food_x[i], food_y[i], &s_food_0);
		}
		settextcolor(BLACK);
		settextstyle(15, 0, L"宋体");
		outtextxy(SIZE, IMAGE_WIDTH + SIZE, L"当前得分为：");
		char temp[50];
		_itoa(score, temp, 10);
		int n = MultiByteToWideChar(0, 0, temp, -1, NULL, 0);
		wchar_t *str = new wchar_t[n];
		MultiByteToWideChar(0, 0, temp, -1, str, n);
		outtextxy(SIZE * 3 + 15, IMAGE_WIDTH + SIZE, str);				//出现问题


		outtextxy(SIZE, IMAGE_WIDTH + SIZE + 15, L"蛇的长度为：");
		//char temp[50];
		_itoa(length, temp, 10);
		n = MultiByteToWideChar(0, 0, temp, -1, NULL, 0);
		str = new wchar_t[n];
		MultiByteToWideChar(0, 0, temp, -1, str, n);
		outtextxy(SIZE * 3 + 15, IMAGE_WIDTH + SIZE + 15, str);				//出现问题


		outtextxy(SIZE * 4, IMAGE_WIDTH + SIZE, L"当前版本最高分数为：");
		//char temp[50];
		//dai gao
		int k;
		ReadFile(k);
		_itoa(k, temp, 10);
		n = MultiByteToWideChar(0, 0, temp, -1, NULL, 0);
		str = new wchar_t[n];
		MultiByteToWideChar(0, 0, temp, -1, str, n);
		outtextxy(SIZE * 8, IMAGE_WIDTH + SIZE, str);				//出现问题
	}
	int Space()
	{
		unsigned int temp = (IMAGE_LENGTH / SIZE) * (IMAGE_WIDTH / SIZE) - 2;
		if (temp == food_x.size())												//当没有空间产生果实和蛇头时结束游戏
			return 0;
		return 1;
	}
	void EndGame(int Death)
	{
		if (Death != 5 && Space())			//未达到失败次数且仍有空间
		{
			return;
		}
		cleardevice();
		settextcolor(LIGHTRED);
		settextstyle(SIZE, 0, L"宋体");
		int X = 1 * SIZE;
		int Y = 4 * SIZE;


		if (!Space())			//无多余空间
		{
			outtextxy(X, Y, L"你也忒猛了！没空间了！");
		}
		else
		{
			outtextxy(X, Y, L"You Suck！撞了五次墙！");
		}
		outtextxy(X, Y + SIZE, L"最终得分为:");
		char temp[50];
		_itoa(score, temp, 10);
		int n = MultiByteToWideChar(0, 0, temp, -1, NULL, 0);
		wchar_t *str = new wchar_t[n];
		MultiByteToWideChar(0, 0, temp, -1, str, n);
		outtextxy(X + 6 * SIZE, Y + SIZE, str);				//出现问题

		int k;
		ReadFile(k);
		if (score <= k)
			outtextxy(X - SIZE, Y + 100, L"请再接再厉！");
		else
		{
			outtextxy(X - SIZE, Y + 100, L"恭喜你打破纪录！");
			WriteFile(score);
		}
		char ch;
		ch = _getch();
		PrintFile();
		exit(0);

	}
	void loop()
	{
		CreateFOOD();
		DisplayFOOD();			//显示食物
		SnakeDisplay();
		_getch();
		int Death = 0;
		while (1)
		{
			//_getch();
			while (!_kbhit())			//当按下按键退出循环
			{
				BeginBatchDraw();
				if (food.num == 0)
					CreateFOOD();
				cleardevice();			//刷新屏幕
				DisplayWall();
				SnakeMove();			//蛇的移动
				//k++;
				EatFOOD();
				int flag = GameOver();
				if (flag != -1)			//撞到墙或者自己
				{
					Death++;
					ReSnakeMove();
				}
				ReInit(flag);
				SnakeDisplay();
				DisplayFOOD();			//有问题	
				EndBatchDraw();
				Sleep(200);
				EndGame(Death);
			}
			SnakeDirect();
		}

	}

};
//菜单函数
void Menu()
{
	SetConsoleTitleA("游戏：2048");                           //设置控制台标题
	HANDLE handle_out = GetStdHandle(STD_OUTPUT_HANDLE);      //获取标准输入设备句柄
	SetConsoleTextAttribute(handle_out, FOREGROUND_GREEN);         //设置打印颜色为绿色
	cout << "            --------------------------------------------\n";
	cout << "            ********************************************\n";
	SetConsoleTextAttribute(handle_out, FOREGROUND_RED | FOREGROUND_GREEN);       //设置打印颜色为黄色,用三原色搭配（自行查阅）
	cout << "                              菜单\n";
	cout << "                          a.入门版\n";
	cout << "                          b.进阶版\n";
	cout << "                          c.高级版\n";
	cout << "                          d.游戏规则\n";
	cout << "                          e.退出游戏\n";
	SetConsoleTextAttribute(handle_out, FOREGROUND_GREEN);         //设置打印颜色为绿色
	cout << "            ********************************************\n";
	cout << "            --------------------------------------------\n";
	cout << "\n请输入你的选择(a-e):";
}

void wait_for_enter()
{
	cout << endl << "按回车键继续";
	while (_getch() != '\r')
		;
	cout << endl << endl;
}

void Help()
{
	system("cls");
	HANDLE handle_out = GetStdHandle(STD_OUTPUT_HANDLE);      //获取标准输入设备句柄
	CONSOLE_SCREEN_BUFFER_INFO csbi;                          //定义窗口缓冲区信息结构体
	GetConsoleScreenBufferInfo(handle_out, &csbi);            //获得窗口缓冲区信息
	SetConsoleTextAttribute(handle_out, FOREGROUND_RED | FOREGROUND_GREEN);            //设置打印颜色为黄色
	cout << "            ----------------------------------------------------------\n";
	cout << "            **********************************************************\n\n";
	cout << "            操作说明：\n            ↓：下   ←：左  ↑：上  →：右  ESC键：退出\n\n";
	cout << "            游戏介绍：\n            每次选择一个方向作为蛇的移动方向\n";
	cout << "            进阶版为：蛇死后变成墙壁，再随机产生新的蛇和食物，直到空间不足为止\n";
	cout << "            高级版为：蛇死后变成食物，再随机产生新的蛇和食物，直到撞墙五次或空 \n";
	cout << "            间不足为止\n\n";
	cout << "            **********************************************************\n";
	cout << "            ----------------------------------------------------------\n\n";
	wait_for_enter();
}
int main()
{

	char choice, ch;

	while (1)  
	{
		system("CLS");  
		Menu(); //调用菜单显示函数
		cin >> choice;
		if (choice == 'e') //选择退出
		{
			cout << "\n确定退出吗?" << endl;
			cin >> ch;
			if (ch == 'y' || ch == 'Y')
			{
				cout << "那好吧..886";
				break;
			}
			else
				continue;
		}
		SNAKE_1 snake1;
		SNAKE_2 snake2;
		SNAKE_3 snake3;
		switch (choice)
		{
		case 'a': snake1.initGame(); snake1.loop(); break;
		case 'b': snake2.initGame(); snake2.loop(); break;
		case 'c':snake3.initGame(); snake3.loop(); break;
		case 'd':Help();
		default:
			cout << "\n输入错误，请重新输入" << endl;
			wait_for_enter();
		}
	}
	return 0;
}
