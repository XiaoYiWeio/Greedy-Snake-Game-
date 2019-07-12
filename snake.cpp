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
#define MAX  200			//�ߵ���󳤶�
#define SIZE 40				//�ߵĿ��,�ƶ��ٶ�
#define UP 72
#define DOWN 80
#define LEFT 75
#define RIGHT 77
IMAGE head_up, head_down, head_left, head_right, s_body, s_food, s_wall,s_food_0;

struct POS					//����
{
	int x;
	int y;
	bool eat;				//food��
};

struct FOOD
{
	int num;			//��¼�ִ�ʳ������
	POS f_pos[5];		//ʳ������
};
//ǽ�ڵ�����
vector<int>wall_x;
vector<int>wall_y;
//��ʬ�������ʳ������
vector<int>food_x;
vector<int>food_y;

//���Ű�
class SNAKE_1
{
	//private:
public:
	int length;			//�ߵĽ���
	POS s_pos[MAX];		//��ÿ�ڵ�����
	char ch;			//�ߵķ���
	FOOD food;
	int score = 0;			//�÷�
	int final[2];		//��¼���һ���ߵ�λ��
public:
	//��ʼ����Ϸ����
	void initGame()
	{
		initgraph(IMAGE_LENGTH + SIZE * 2, IMAGE_WIDTH + SIZE * 2);							//���ڿ�ȣ��߶�

		setbkcolor(WHITE);								//������ɫ������ɫ
		cleardevice();									//����

		//����ͼƬ
		loadimage(&head_up, L"��ͷ��.jpg", SIZE, SIZE);
		loadimage(&head_down, L"��ͷ��.jpg", SIZE, SIZE);
		loadimage(&head_left, L"��ͷ��.jpg", SIZE, SIZE);
		loadimage(&head_right, L"��ͷ��.jpg", SIZE, SIZE);
		loadimage(&s_body, L"����.jpg", SIZE, SIZE);
		loadimage(&s_food, L"����.jpg", SIZE, SIZE);
		loadimage(&s_wall, L"ǽ.jpg", SIZE, SIZE);
		loadimage(&s_food_0, L"�ǹ�.jpg", SIZE, SIZE);
		length = 1;										//�ߵĽ�����ʼ��
		srand((unsigned)time(NULL));
		s_pos[0].x = rand() % 10 * SIZE + SIZE;									//�ߵ�����
		s_pos[0].y = rand() % 10 * SIZE + SIZE;
		int direction = rand() % 4 + 1;
		switch (direction)
		{
		case 1:ch = UP; break;
		case 2:ch = DOWN; break;
		case 3:ch = LEFT; break;
		case 4:ch = RIGHT; break;
		default:break;
		}												//�ߵķ���

		food.num = 0;									//δ����ʳ��
		for (int i = 0; i < 5; i++)
			food.f_pos[i].eat = 1;						//ʳ��δ���֣��ѳԵ���
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

	//������ͷ����������
	void SnakeDisplay()
	{
		//���ݷ�������ͷ
		switch (ch)
		{
		case UP:
			putimage(s_pos[0].x, s_pos[0].y, &head_up);			//��Ԫ��դȱʡ
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
		//��������
		for (int i = length - 1; i > 0; i--)
		{
			putimage(s_pos[i].x, s_pos[i].y, &s_body);
		}
	}

	//�����ߵ��ƶ�
	void SnakeMove()
	{
		//�ƶ�ʱ����ͷ�⣬����ÿ�������ǰ��һλ��ֱ�Ӷ��������
		final[0] = s_pos[length - 1].x;
		final[1] = s_pos[length - 1].y;
		for (int i = length - 1; i > 0; i--)
		{
			s_pos[i].x = s_pos[i - 1].x;
			s_pos[i].y = s_pos[i - 1].y;
		}
		switch (ch)				//���ݷ����趨��ͷ�˶�
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

	//��������ʱ�˻���֮ǰλ��
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

	//����������ߵķ���ı�
	void SnakeDirect()
	{
		switch (_getch())
		{
		case 72:
			if (ch != DOWN)			//���¼���
				ch = UP;
			break;
		case 80:					//����
			if (ch != UP)
				ch = DOWN;
			break;
		case 75:					//����
			if (ch != RIGHT)
				ch = LEFT;
			break;
		case 77:					//����
			if (ch != LEFT)
				ch = RIGHT;
			break;
		default:
			break;
		}
	}

	//�����Ƿ������������غ�
	bool find_in_snake(int x, int y)
	{
		for (int i = 0; i < length; i++)
			if (s_pos[i].x == x && s_pos[i].y == y)
				return TRUE;
		return FALSE;
	}
	//����ʳ������
	virtual void CreateFOOD()
	{
		//���������
		int num = 0, k = 0;
		unsigned int t = 0;
		srand((unsigned)time(NULL));
		while (k == num)
		{
			t++;
			k = 0;
			//����ע�����ʱ��Ϊ���ӻ����OVERFLOW������ѭ��
			num = rand() % 5 + 1;								//�������n��ʳ��
			food.num = num;										//��¼ʳ�������Ϊ0ʱ����ʳ��
			for (int i = 0; i < num; i++)
			{
				food.f_pos[i].x = rand() % 10 * SIZE + SIZE;			//���ݵ�ͼ��С���������Χ
				food.f_pos[i].y = rand() % 10 * SIZE + SIZE;
				food.f_pos[i].eat = 0;							//δ����
			}
			for (int i = 0; i < num; i++)
				if (food.f_pos[i].eat == 1)
					k++;
		}
		return;
	}

	//�ж��Ե�ʳ��
	virtual void EatFOOD()
	{
		//int num = food.num;
		for (int i = 0; i < 5; i++)			//5���ж�	
		{
			if (s_pos[0].x == food.f_pos[i].x
				&&s_pos[0].y == food.f_pos[i].y
				&&food.f_pos[i].eat == 0)			//�����غ���ʳ�����
			{
				length++;						//���ȼ�1
				food.num--;						//ʳ��������һ
				food.f_pos[i].eat = 1;			//ʳ�ﱻ��
				score += 2;
			}
		}
	}

	//��Ϸ�������
	virtual void EndGame(int flag)
	{
		if (flag == -1)
			return;
		cleardevice();
		settextcolor(LIGHTRED);
		settextstyle(50, 0, L"����");
		int X = IMAGE_LENGTH / 4;
		int Y = IMAGE_WIDTH / 4;
		if (flag == 0)
		{
			outtextxy(X - SIZE, Y, L"��ҧ�����Լ���");

		}
		else if (flag == 1)
		{
			outtextxy(X - SIZE, Y, L"��ҧ����ǽ�ڣ�");

		}
		else if (flag == 2)
		{
			outtextxy(X - SIZE, Y, L"�����������û�ռ�����");
		}
		outtextxy(X - SIZE, Y + 50, L"���յ÷�Ϊ:");
		char temp[50];
		_itoa(score, temp, 10);
		int n = MultiByteToWideChar(0, 0, temp, -1, NULL, 0);
		wchar_t *str = new wchar_t[n];
		MultiByteToWideChar(0, 0, temp, -1, str, n);
		outtextxy(X + 300, Y + 50, str);				//��������

		int k;
		ReadFile(k);
		if (score <= k)
			outtextxy(X - SIZE, Y + 100, L"���ٽ�������");
		else
		{
			outtextxy(X - SIZE, Y + 100, L"��ϲ����Ƽ�¼��");
			WriteFile(score);
		}
		char ch;
		ch = _getch();


		PrintFile();
		exit(0);
	}

	//�ж���Ϸ����
	virtual	int GameOver()
	{
		//��ͷײ��ǽ��
		if (s_pos[0].x < SIZE || s_pos[0].x >= IMAGE_LENGTH + SIZE ||
			s_pos[0].y < SIZE || s_pos[0].y >= IMAGE_WIDTH + SIZE)
		{
			return(1);
		}
		//��ͷײ��������
		for (int i = length - 1; i > 0; i--)
		{
			if (s_pos[0].x == s_pos[i].x
				&&s_pos[0].y == s_pos[i].y)
			{
				return(0);
			}
		}
		return -1;			//δʧ��
	}

	//��ӡʳ��
	virtual void DisplayFOOD()
	{
		for (int i = 0; i < 5; i++)
		{
			if (food.f_pos[i].eat == 0)
				putimage(food.f_pos[i].x, food.f_pos[i].y, &s_food);
		}
		settextcolor(BLACK);
		settextstyle(15, 0, L"����");
		outtextxy(SIZE, IMAGE_WIDTH + SIZE, L"��ǰ�÷�Ϊ��");
		char temp[50];
		_itoa(score, temp, 10);
		int n = MultiByteToWideChar(0, 0, temp, -1, NULL, 0);
		wchar_t *str = new wchar_t[n];
		MultiByteToWideChar(0, 0, temp, -1, str, n);
		outtextxy(SIZE * 3 + 15, IMAGE_WIDTH + SIZE, str);				//��������


		outtextxy(SIZE, IMAGE_WIDTH + SIZE + 15, L"�ߵĳ���Ϊ��");
		//char temp[50];
		_itoa(length, temp, 10);
		n = MultiByteToWideChar(0, 0, temp, -1, NULL, 0);
		str = new wchar_t[n];
		MultiByteToWideChar(0, 0, temp, -1, str, n);
		outtextxy(SIZE * 3 + 15, IMAGE_WIDTH + SIZE + 15, str);				//��������


		outtextxy(SIZE * 4, IMAGE_WIDTH + SIZE, L"��ǰ�汾��߷���Ϊ��");
		//char temp[50];
		//dai gao
		int k;
		ReadFile(k);
		_itoa(k, temp, 10);
		n = MultiByteToWideChar(0, 0, temp, -1, NULL, 0);
		str = new wchar_t[n];
		MultiByteToWideChar(0, 0, temp, -1, str, n);
		outtextxy(SIZE * 8, IMAGE_WIDTH + SIZE, str);				//��������
	}

	//��ӡ�ļ�����
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

	//ȡ���汾��ʷ�������
	virtual void ReadFile(int &k)
	{

		//����snake_1 
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
		/* �ú������ڶ�originȫ�Ĳ��ҳ��ֵ�old_value ����new_value�滻 */
		for (string::size_type pos(0); pos != string::npos; pos += new_value.length()) {
			if (((pos = origin.find(old_value, pos)) != string::npos))
				origin.replace(pos, old_value.length(), new_value);
			else break;
		}
	}
	//������ʷ��߼�¼
	void WriteFile(int score)
	{
		ifstream out("record.txt", ios::binary);
		//string str;

		istreambuf_iterator<char> beg(out), end;              // ���������ļ�ָ�룬ָ��ʼ�ͽ�������char(һ�ֽ�)Ϊ����
		string content(beg, end);                             // ���ļ�ȫ������string�ַ���			
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
	//��ʾǽ
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

	//��Ϸѭ��
	virtual	void loop()
	{
		CreateFOOD();
		DisplayFOOD();			//��ʾʳ��
		SnakeDisplay();
		_getch();
		while (1)
		{
			//_getch();
			while (!_kbhit())			//�����°����˳�ѭ��
			{
				BeginBatchDraw();
				if (food.num == 0)
					CreateFOOD();
				cleardevice();			//ˢ����Ļ
				DisplayWall();
				DisplayFOOD();
				SnakeMove();			//�ߵ��ƶ�
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
	//�ȴ�����
	void wait_for_enter()
	{
		cout << endl << "���س�������";
		while (_getch() != '\r')
			;
		cout << endl << endl;
	}


};


//���װ�
class SNAKE_2 :public SNAKE_1
{
public:
	void ReadFile(int &k)
	{

		//����snake_2 
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
		//�����Ƿ������ɵ�������ǽ���غ�
		for (unsigned int i = 0; i < wall_x.size(); i++)
			if (wall_x[i] == x && wall_y[i] == y)
				return TRUE;
		return FALSE;
	}

	void ReInit(int flag) //flag= 0 1 ʱִ�в���
	{
		//���߹ҵ�����������ǽ��,��������ӽ���wall����
		if (flag == 0 || flag == 1)
		{
			//��¼ǽ������
			for (int i = 0; i < length; i++)
			{
				wall_x.push_back(s_pos[i].x);
				wall_y.push_back(s_pos[i].y);
			}


			//��������������¹�ʵ���ܿ�ǽ�ڣ�
			unsigned int t = 0;

			while (find_in_wall(s_pos[0].x, s_pos[0].y))		//��������
			{
				t++;
				srand((unsigned)time(NULL) + t);
				s_pos[0].x = rand() % 10 * SIZE + SIZE;
				s_pos[0].y = rand() % 10 * SIZE + SIZE;
			}
			//�漴�����ߵķ���
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
	//�ж���Ϸ����  ��Ҫ�޸�
	//��Ϸ�������
	void EndGame(int flag)			//-2ʱִ�в���
	{
		if (flag != -2)
		{
			return;
		}
		BeginBatchDraw();
		cleardevice();			//ˢ����Ļ
		DisplayFOOD();
		SnakeDisplay();
		DisplayWall();
		EndBatchDraw();
		Sleep(100);
		cleardevice();
		settextcolor(LIGHTRED);
		settextstyle(SIZE, 0, L"����");
		int X = 1 * SIZE;
		int Y = 4 * SIZE;
		outtextxy(X - SIZE, Y + 50, L"���յ÷�Ϊ:");
		char temp[50];
		_itoa(score, temp, 10);
		int n = MultiByteToWideChar(0, 0, temp, -1, NULL, 0);
		wchar_t *str = new wchar_t[n];
		MultiByteToWideChar(0, 0, temp, -1, str, n);
		outtextxy(X + 300, Y + 50, str);				//��������

		int k;
		ReadFile(k);
		if (score <= k)
			outtextxy(X - SIZE, Y + 100, L"���ٽ�������");
		else
		{
			outtextxy(X - SIZE, Y + 100, L"��ϲ����Ƽ�¼��");
			WriteFile(score);
		}
		char ch;
		ch = _getch();


		PrintFile();
		exit(0);

	}

	int GameOver()		//ײǽ 1 ײ�Լ� 0 ��ײ -1 ������Ϸ-2
	{
		int flag = -1;
		//��ͷײ��ǽ��
		if (s_pos[0].x < SIZE || s_pos[0].x >= IMAGE_LENGTH + SIZE ||
			s_pos[0].y < SIZE || s_pos[0].y >= IMAGE_WIDTH + SIZE ||
			find_in_wall(s_pos[0].x, s_pos[0].y))
		{
			flag = 1;
		}
		//��ͷײ��������
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
		if (temp == wall_x.size())												//��û�пռ������ʵ����ͷʱ������Ϸ
			flag = -2;
		return flag;
	}

	void CreateFOOD()
	{
		//���������
		int num = 0, k = 0, t = 0;
		while (k == num)
		{
			t++;
			k = 0;
			srand((unsigned)time(NULL) + t);						//����ע�����ʱ��Ϊ���ӻ����OVERFLOW������ѭ��
			num = rand() % 5 + 1;								//�������n��ʳ��
			food.num = num;										//��¼ʳ�������Ϊ0ʱ����ʳ��
			for (int i = 0; i < num; i++)
			{
				food.f_pos[i].x = rand() % 10 * SIZE + SIZE;			//���ݵ�ͼ��С���������Χ
				food.f_pos[i].y = rand() % 10 * SIZE + SIZE;
				food.f_pos[i].eat = 0;							//δ����
				if (find_in_wall(food.f_pos[i].x, food.f_pos[i].y))		//��¼����ǽ�ص��Ĺ�ʵ�������ظ��Ĺ�ʵ��
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
		//��¼ÿһ�����������������������ʾǽ
		for (unsigned int i = 0; i < wall_x.size(); i++)
		{
			putimage(wall_x[i], wall_y[i], &s_wall);				//����ǽ��ͼƬ
		}


		settextcolor(BLACK);
		settextstyle(15, 0, L"����");
		outtextxy(SIZE, IMAGE_WIDTH + SIZE, L"��ǰ�÷�Ϊ��");
		char temp[50];
		_itoa(score, temp, 10);
		int n = MultiByteToWideChar(0, 0, temp, -1, NULL, 0);
		wchar_t *str = new wchar_t[n];
		MultiByteToWideChar(0, 0, temp, -1, str, n);
		outtextxy(SIZE * 3 + 15, IMAGE_WIDTH + SIZE, str);				//��������


		outtextxy(SIZE, IMAGE_WIDTH + SIZE + 15, L"�ߵĳ���Ϊ��");
		//char temp[50];
		_itoa(length, temp, 10);
		n = MultiByteToWideChar(0, 0, temp, -1, NULL, 0);
		str = new wchar_t[n];
		MultiByteToWideChar(0, 0, temp, -1, str, n);
		outtextxy(SIZE * 3 + 15, IMAGE_WIDTH + SIZE + 15, str);				//��������

		outtextxy(SIZE * 4, IMAGE_WIDTH + SIZE, L"��ǰ�汾��߷���Ϊ��");
		//char temp[50];
		//dai gao
		int k;
		ReadFile(k);
		_itoa(k, temp, 10);
		n = MultiByteToWideChar(0, 0, temp, -1, NULL, 0);
		str = new wchar_t[n];
		MultiByteToWideChar(0, 0, temp, -1, str, n);
		outtextxy(SIZE * 8, IMAGE_WIDTH + SIZE, str);				//��������
	}

	void loop()
	{
		CreateFOOD();
		DisplayFOOD();			//��ʾʳ��
		SnakeDisplay();
		_getch();
		//int k = 0;
		while (1)
		{
			//_getch();
			while (!_kbhit())			//�����°����˳�ѭ��
			{
				BeginBatchDraw();
				if (food.num == 0)
					CreateFOOD();
				cleardevice();			//ˢ����Ļ
				DisplayFOOD();
				SnakeMove();			//�ߵ��ƶ�

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





//�߼���
//˼·���ӽ��װ�����������ڹҵ���ʱ���¼����������Ȼ������ʳ�ﵱʳ���ٴα������ʱ���ٴ��漴����ʳ����ϼ��ײǽ�������������ʱ���������EndGame
class SNAKE_3 :public SNAKE_1
{
public:
	void ReadFile(int &k)
	{

		//����snake_1 
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
		//�����Ƿ������ɵ�������ǽ���غ�
		for (unsigned int i = 0; i < food_x.size(); i++)
			if (food_x[i] == x && food_y[i] == y)
				return TRUE;
		return FALSE;
	}
	void ReInit(int flag)
	{
		//���߹ҵ�����������ǽ��,��������ӽ���wall����
		if (flag == 0 || flag == 1)
		{
			//��¼�����ɹ�������
			for (int i = 0; i < length; i++)
			{
				food_x.push_back(s_pos[i].x);
				food_y.push_back(s_pos[i].y);
			}
			//��������������¹�ʵ���ܿ�ǽ�ڣ�
			//unsigned int t = 0;
			srand((unsigned)time(NULL));
			while (find_in_food(s_pos[0].x, s_pos[0].y))		//��������
			{
				
				
				s_pos[0].x = rand() % 10 * SIZE + SIZE;
				s_pos[0].y = rand() % 10 * SIZE + SIZE;
			}
			//�漴�����ߵķ���

			//�漴�����ߵķ���
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
		//���������
		int num = 0, k = 1, t = 0;
		srand((unsigned)time(NULL) );
		while (k != 0)
		{
			//t++;
			k = 0;
									//����ע�����ʱ��Ϊ���ӻ����OVERFLOW������ѭ��
			num = rand() % 5 + 1;								//�������n��ʳ��
			food.num = num;										//��¼ʳ�������Ϊ0ʱ����ʳ��
			for (int i = 0; i < num; i++)
			{
				food.f_pos[i].x = rand() % 10 * SIZE + SIZE;			//���ݵ�ͼ��С���������Χ
				food.f_pos[i].y = rand() % 10 * SIZE + SIZE;
				food.f_pos[i].eat = 0;							//δ����
				if (find_in_food(food.f_pos[i].x, food.f_pos[i].y))		//��¼����ʬ���ص��Ĺ�ʵ�������ظ��Ĺ�ʵ��
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
		for (int i = 0; i < 5; i++)			//5���ж�	
		{
			if (s_pos[0].x == food.f_pos[i].x
				&&s_pos[0].y == food.f_pos[i].y
				&&food.f_pos[i].eat == 0)			//�����غ���ʳ�����
			{
				length++;						//���ȼ�1
				food.num--;						//ʳ��������һ
				food.f_pos[i].eat = 1;			//ʳ�ﱻ��
				score += 2;
			}
		}
		/*
		for (unsigned int i = 0; i < food_x.size(); i++)
		{
			if (s_pos[0].x == food_x[i]
				&& s_pos[0].y == food_y[i])
			{
				length++;						//���ȼ�1	
				score += 1;
				food_x.erase(food_x.begin()+i);
				food_y.erase(food_y.begin()+i);
			}
		}*/
		vector<int>::iterator iter_x = food_x.begin();
		vector<int>::iterator iter_y = food_y.begin();
		for ( ;iter_x != food_x.end(); )
		{
			if (*iter_x == s_pos[0].x && (*iter_y) == s_pos[0].y)		//����غ�
			{
				iter_x = food_x.erase(iter_x);							//��ȥͬʱ���¸ô�
				iter_y = food_y.erase(iter_y);							//�������ֵ��ͬʱ�����Ե�����
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
	//��ӡʳ�� 
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
		settextstyle(15, 0, L"����");
		outtextxy(SIZE, IMAGE_WIDTH + SIZE, L"��ǰ�÷�Ϊ��");
		char temp[50];
		_itoa(score, temp, 10);
		int n = MultiByteToWideChar(0, 0, temp, -1, NULL, 0);
		wchar_t *str = new wchar_t[n];
		MultiByteToWideChar(0, 0, temp, -1, str, n);
		outtextxy(SIZE * 3 + 15, IMAGE_WIDTH + SIZE, str);				//��������


		outtextxy(SIZE, IMAGE_WIDTH + SIZE + 15, L"�ߵĳ���Ϊ��");
		//char temp[50];
		_itoa(length, temp, 10);
		n = MultiByteToWideChar(0, 0, temp, -1, NULL, 0);
		str = new wchar_t[n];
		MultiByteToWideChar(0, 0, temp, -1, str, n);
		outtextxy(SIZE * 3 + 15, IMAGE_WIDTH + SIZE + 15, str);				//��������


		outtextxy(SIZE * 4, IMAGE_WIDTH + SIZE, L"��ǰ�汾��߷���Ϊ��");
		//char temp[50];
		//dai gao
		int k;
		ReadFile(k);
		_itoa(k, temp, 10);
		n = MultiByteToWideChar(0, 0, temp, -1, NULL, 0);
		str = new wchar_t[n];
		MultiByteToWideChar(0, 0, temp, -1, str, n);
		outtextxy(SIZE * 8, IMAGE_WIDTH + SIZE, str);				//��������
	}
	int Space()
	{
		unsigned int temp = (IMAGE_LENGTH / SIZE) * (IMAGE_WIDTH / SIZE) - 2;
		if (temp == food_x.size())												//��û�пռ������ʵ����ͷʱ������Ϸ
			return 0;
		return 1;
	}
	void EndGame(int Death)
	{
		if (Death != 5 && Space())			//δ�ﵽʧ�ܴ��������пռ�
		{
			return;
		}
		cleardevice();
		settextcolor(LIGHTRED);
		settextstyle(SIZE, 0, L"����");
		int X = 1 * SIZE;
		int Y = 4 * SIZE;


		if (!Space())			//�޶���ռ�
		{
			outtextxy(X, Y, L"��Ҳ߯���ˣ�û�ռ��ˣ�");
		}
		else
		{
			outtextxy(X, Y, L"You Suck��ײ�����ǽ��");
		}
		outtextxy(X, Y + SIZE, L"���յ÷�Ϊ:");
		char temp[50];
		_itoa(score, temp, 10);
		int n = MultiByteToWideChar(0, 0, temp, -1, NULL, 0);
		wchar_t *str = new wchar_t[n];
		MultiByteToWideChar(0, 0, temp, -1, str, n);
		outtextxy(X + 6 * SIZE, Y + SIZE, str);				//��������

		int k;
		ReadFile(k);
		if (score <= k)
			outtextxy(X - SIZE, Y + 100, L"���ٽ�������");
		else
		{
			outtextxy(X - SIZE, Y + 100, L"��ϲ����Ƽ�¼��");
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
		DisplayFOOD();			//��ʾʳ��
		SnakeDisplay();
		_getch();
		int Death = 0;
		while (1)
		{
			//_getch();
			while (!_kbhit())			//�����°����˳�ѭ��
			{
				BeginBatchDraw();
				if (food.num == 0)
					CreateFOOD();
				cleardevice();			//ˢ����Ļ
				DisplayWall();
				SnakeMove();			//�ߵ��ƶ�
				//k++;
				EatFOOD();
				int flag = GameOver();
				if (flag != -1)			//ײ��ǽ�����Լ�
				{
					Death++;
					ReSnakeMove();
				}
				ReInit(flag);
				SnakeDisplay();
				DisplayFOOD();			//������	
				EndBatchDraw();
				Sleep(200);
				EndGame(Death);
			}
			SnakeDirect();
		}

	}

};
//�˵�����
void Menu()
{
	SetConsoleTitleA("��Ϸ��2048");                           //���ÿ���̨����
	HANDLE handle_out = GetStdHandle(STD_OUTPUT_HANDLE);      //��ȡ��׼�����豸���
	SetConsoleTextAttribute(handle_out, FOREGROUND_GREEN);         //���ô�ӡ��ɫΪ��ɫ
	cout << "            --------------------------------------------\n";
	cout << "            ********************************************\n";
	SetConsoleTextAttribute(handle_out, FOREGROUND_RED | FOREGROUND_GREEN);       //���ô�ӡ��ɫΪ��ɫ,����ԭɫ���䣨���в��ģ�
	cout << "                              �˵�\n";
	cout << "                          a.���Ű�\n";
	cout << "                          b.���װ�\n";
	cout << "                          c.�߼���\n";
	cout << "                          d.��Ϸ����\n";
	cout << "                          e.�˳���Ϸ\n";
	SetConsoleTextAttribute(handle_out, FOREGROUND_GREEN);         //���ô�ӡ��ɫΪ��ɫ
	cout << "            ********************************************\n";
	cout << "            --------------------------------------------\n";
	cout << "\n���������ѡ��(a-e):";
}

void wait_for_enter()
{
	cout << endl << "���س�������";
	while (_getch() != '\r')
		;
	cout << endl << endl;
}

void Help()
{
	system("cls");
	HANDLE handle_out = GetStdHandle(STD_OUTPUT_HANDLE);      //��ȡ��׼�����豸���
	CONSOLE_SCREEN_BUFFER_INFO csbi;                          //���崰�ڻ�������Ϣ�ṹ��
	GetConsoleScreenBufferInfo(handle_out, &csbi);            //��ô��ڻ�������Ϣ
	SetConsoleTextAttribute(handle_out, FOREGROUND_RED | FOREGROUND_GREEN);            //���ô�ӡ��ɫΪ��ɫ
	cout << "            ----------------------------------------------------------\n";
	cout << "            **********************************************************\n\n";
	cout << "            ����˵����\n            ������   ������  ������  ������  ESC�����˳�\n\n";
	cout << "            ��Ϸ���ܣ�\n            ÿ��ѡ��һ��������Ϊ�ߵ��ƶ�����\n";
	cout << "            ���װ�Ϊ����������ǽ�ڣ�����������µ��ߺ�ʳ�ֱ���ռ䲻��Ϊֹ\n";
	cout << "            �߼���Ϊ����������ʳ�����������µ��ߺ�ʳ�ֱ��ײǽ��λ�� \n";
	cout << "            �䲻��Ϊֹ\n\n";
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
		Menu(); //���ò˵���ʾ����
		cin >> choice;
		if (choice == 'e') //ѡ���˳�
		{
			cout << "\nȷ���˳���?" << endl;
			cin >> ch;
			if (ch == 'y' || ch == 'Y')
			{
				cout << "�Ǻð�..886";
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
			cout << "\n�����������������" << endl;
			wait_for_enter();
		}
	}
	return 0;
}
