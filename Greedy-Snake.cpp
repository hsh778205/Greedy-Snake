#include<iostream>
#include<algorithm>
#include<cstdio>
#include<cstring>
#include<cmath>
#include<windows.h>
#include<stdlib.h>
#include<fstream>
#include<set>
#include<queue>
#include<vector>
#include<ctime>
#include<conio.h> 
#define IL inline
#define re register
#define LL long long
using namespace std;

void init();
int lx, ly;
void hide();
void backxy();
void gotoxy0(int, int);
void gotoxy(int, int);
void getxy();
const int A = 10, B = 11, C = 12, D = 13, E = 14, F = 15;
void SetColor(unsigned short, unsigned short);
void SetColor(unsigned short = 0x07);
long long step, score;
int speed, Size, lenth, way, tme, init_size, start_time, clr = 1, have_eaten;
int big_apple_probability, big_apple_save_time = 10000, big_apple_time, mode;
long long his_score[10], his_step[10];
bool through_wall, wall_protection, self_protection, full_time, adj_speed;
bool have_big_apple;
int delta[4][2] = {
{-1,0},
{0,-1},
{1,0},
{0,1}
};
struct xy {
	int x, y;
	bool operator==(xy t)
	{
		return x == t.x && y == t.y;
	}
	bool inmap() {
		return x < Size&& x >= 0 && y < Size&& y >= 0;
	}
	xy edge(int w) {
		return { x + delta[w][0],y + delta[w][1] };
	}
}apple, big_apple;

bool operator<(xy a, xy b)
{
	return a.x * 100 + a.y < b.x * 100 + b.y;
}
deque<xy>snake;
set<xy>line;
int map[100][100];
void save();
void setting();
void make_apple();
void move(int);
int judge();
int calc();
void show();

int main()
{
	srand(time(0));
	hide();
	setting();
	init();
	while (true)
	{
		show();
		if (mode == 5)
		{
			if (_kbhit()) {
				//				char ch=_getch();
				if (_getch() == ' ') {
					int t = clock();
					while (_getch() != ' ');;
					start_time += clock() - t;
				}
			}

			way = calc();
		}
		else
		{
			tme = clock();
			while (clock() - tme <= speed)
			{
				gotoxy0(Size + 2 + 5, 4);
				cout << (clock() - start_time) / 1000 << "s";
				if (mode == 1) gotoxy0(Size + 2, 15), cout << "当前苹果价值=" << log2((clock() - start_time) / 1000.0) * (1000.0 / speed);
				if (_kbhit()) {
					char ch = _getch();
					if ((ch == 'a' || ch == 'A') && way != 2) {
						way = 0; break;
					}
					if ((ch == 'w' || ch == 'W') && way != 3) {
						way = 1; break;
					}
					if ((ch == 'd' || ch == 'D') && way != 0) {
						way = 2; break;
					}
					if ((ch == 's' || ch == 'S') && way != 1) {
						way = 3; break;
					}
					if (adj_speed)
						if (ch == '+' || ch == '=' || ch == '.') {
							speed = max(0, speed - 10);
							gotoxy0(Size + 2, 5);
							cout << "移动间隔：        \b\b\b\b\b\b\b\b" << speed << "ms";
							if (mode == 1) gotoxy0(Size + 2, 15), cout << "当前苹果价值=            \b\b\b\b\b\b\b\b\b\b\b\b" << log2((clock() - start_time) / 1000.0) * (1000.0 / speed);
						}
					if (adj_speed)
						if (ch == '-' || ch == ',') {
							speed += 10;
							gotoxy0(Size + 2, 5);
							cout << "移动间隔：        \b\b\b\b\b\b\b\b" << speed << "ms";
							if (mode == 1) gotoxy0(Size + 2, 15), cout << "当前苹果价值=            \b\b\b\b\b\b\b\b\b\b\b\b" << log2((clock() - start_time) / 1000.0) * (1000.0 / speed);
						}
					if (ch == 'x' || ch == 'X') save(), exit(0);
					if (ch == 'v' || ch == 'V') save();
					if (ch == ' ') {//暂停 
						long ttime = clock();
						char c;
						do {
							c = _getch();
							if (adj_speed)
								if (c == '+' || c == '=' || c == '.') {
									speed = max(0, speed - 10);
									gotoxy0(Size + 2, 5);
									cout << "移动间隔：        \b\b\b\b\b\b\b\b" << speed << "ms";
									if (mode == 1) gotoxy0(Size + 2, 15), cout << "当前苹果价值=            \b\b\b\b\b\b\b\b\b\b\b\b" << log2((clock() - start_time) / 1000.0) * (1000.0 / speed);
								}
							if (adj_speed)
								if (c == '-' || c == ',') {
									speed += 10;
									gotoxy0(Size + 2, 5);
									cout << "移动间隔：        \b\b\b\b\b\b\b\b" << speed << "ms";
									if (mode == 1) gotoxy0(Size + 2, 15), cout << "当前苹果价值=            \b\b\b\b\b\b\b\b\b\b\b\b" << log2((clock() - start_time) / 1000.0) * (1000.0 / speed);
								}
							if (c == 'x') save(), exit(0);
							if (c == 'v') save();
						} while (c != ' ');
						tme += clock() - ttime;
						big_apple_time += clock() - ttime;
					}
				}
			}
		}

		if (full_time && clock() - tme <= speed) Sleep(speed - clock() + tme);

		if (have_big_apple) {
			gotoxy0(Size + 2, 14);
			cout << "                       ";
			if (clock() - big_apple_time < big_apple_save_time) {
				gotoxy0(Size + 2, 14);
				cout << "大苹果剩余时间：" << big_apple_save_time - clock() + big_apple_time << "ms";
				gotoxy(big_apple.x, big_apple.y);
				if (rand() & 1) cout << "♀";
				else cout << "♂";
			}
			else {
				gotoxy(big_apple.x, big_apple.y);
				cout << "  ";
				have_big_apple = 0;
			}
		}

		snake.push_front({ snake.front().x + delta[way][0],snake.front().y + delta[way][1] });

		if (judge() == 1) break;
		if (judge() == 2) {
			if (mode == 1) score--;
			gotoxy0(2, Size + 2);
			cout << "蛇蛇撞墙了！扣分ing";
			cout << "\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b                   ";
			gotoxy0(Size + 2, 2);
			cout << "得分：            \b\b\b\b\b\b\b\b\b\b\b\b" << score;
			snake.pop_front();
			continue;
		}
		line.insert(snake.front());
		gotoxy(snake.front().x, snake.front().y);
		SetColor(0x0c);
		cout << "■";

		//将第二格的颜色改回来 
		gotoxy(snake[1].x, snake[1].y);
		SetColor();
		if (mode == 1) {
			clr++;
			clr %= 15;
			clr++;
			if (clr == C || clr == 7 || clr == 0) clr++;
			SetColor(0, clr);
		}
		cout << "■";
		SetColor();

		//如果吃到了苹果 
		if (have_big_apple && snake.front() == big_apple) {
			score += 5;
			for (int i = 0; i < 5 && snake.size()>2; i++) {
				line.erase(snake.back());
				gotoxy(snake.back().x, snake.back().y);
				snake.pop_back(); lenth--;
				SetColor();
				cout << "  ";

			}
			big_apple_time = -10000;
			show();
		}

		if (snake.front() == apple) {
			have_eaten++;
			if (have_big_apple) big_apple_time = -10000;
			lenth++;

			if (snake.size() == (unsigned)Size * Size) {
				show();
				break;
			}

			make_apple();

			if (mode == 1) {
				score += log2((clock() - start_time) / 1000.0) * (1000.0 / speed);

			}
			else score += 1;

			show();
		}
		else {
			line.erase(snake.back());
			gotoxy(snake.back().x, snake.back().y);
			snake.pop_back();
			SetColor();
			cout << "  ";
		}


		gotoxy0(Size + 2 + 3, -1);
		cout << "     \b\b\b\b\b" << snake.front().x << "," << snake.front().y;
		gotoxy0(Size + 2 + 5 + 4, -1);
		cout << "    \b\b\b\b\b" << apple.x << "," << apple.y;
		gotoxy0(Size + 2 + 3, 9);
		cout << ++step;
	}
	show();
	save();
	cin.get(); cin.get();
	if (snake.size() >= (unsigned int)Size * Size) {
		gotoxy0(Size / 2 - 2, Size / 2);
		SetColor(4, 7);
		cout << "You have won!!!";
		cin.get();
		return 0;
	}

	for (int i = 1; i < Size; i++)
	{
		gotoxy0(1, i);
		for (int j = 1; j * 7 < Size; j++)cout << "You are dead  ";
		cout << "\n";
	}
	cout << "\n\n\n\n\n";
	cin.get();
	main();
	return 0;
}

void save()
{
	if (his_score[mode] < score)
	{
		his_score[mode] = score;
		his_step[mode] = step;
	}
	ofstream fout("history.txt", ios::out);
	for (int i = 0; i < 10; i++) fout << his_score[i] << " " << his_step[i] << endl;
}

void setting()
{
	memset(his_step, sizeof(his_score), 0);
	memset(his_step, sizeof(his_step), 0);
	step = 0;
	if (system("@echo off\ndir /a history.txt") == 1) {
		save();
	}
	ifstream fin("history.txt", ios::in);
	if (fin.eof() || fin.peek() == EOF) {
		save();
	}
	else {
		for (int i = 0; i < 10; i++) fin >> his_score[i] >> his_step[i];
	}

	SetColor();
	system("cls");
	system("mode con cols=80 lines=40");//改变宽高
	cout << "\t序号 名称\t地图\t速度\t大苹果\t其他属性\n";
	cout << "\t【0】作弊模式\t20*20\t5m/s\t15%\t穿墙 吃自己不死亡 可调节速度\n";
	cout << "\t【1】娱乐模式\t25*25\t2m/s\t30%\t撞墙保护 特殊计分方式 彩色！\n";
	cout << "\t【2】经典模式\t20*20\t5m/s\t15%\t穿墙\n";
	cout << "\t【3】普通模式\t15*15\t10m/s\t0%\t无\n";
	cout << "\t【4】快速模式\t20*20\t1m/s\t20%\t穿墙 可调节速度 可手动移动\n";
	cout << "\t【5】电脑模式\n";
	cout << "\t【6】自定义\n";
	cout << "请输入数字选择  或按w(上)/s(下)移动 回车确定\n";

	vector<string>sentence;
	sentence.push_back("请认真阅读下面的信息！！！！！！！！！！！！！！！！！！！！");
	sentence.push_back("请把控制台窗口的字号改成14或以下！！！");
	sentence.push_back("请将输入法调成英文模式！！");
	sentence.push_back("按空格键暂停游戏");
	sentence.push_back("按v键保存(只记录最高) 按x键保存并退出");
	sentence.push_back("在可调节速度的模式下，按+加速，按-减速");
	sentence.push_back("不仅+可以加快速度，=也可以");
	sentence.push_back("其实 ,和.也可以调整速度，但是一定要是英文的符号！");
	sentence.push_back("娱乐模式下不会死亡，推荐先玩娱乐模式");
	sentence.push_back("娱乐模式下，移速越快，游戏时间越长，加分越多");
	sentence.push_back("娱乐模式下，记得先加快一点速度哦");
	sentence.push_back("快速模式下，只要你手速够快，蛇想多快有多快");
	sentence.push_back("穿墙和撞墙保护不可共存，穿墙优先");
	for (unsigned int i = 0; i < sentence.size(); i++)
	{
		gotoxy0(16 - sentence[i].size() / 4, 10 + i * 2);
		cout << sentence[i];
	}



	int ch = 0;
	char t;
	gotoxy0(1, 0);
	cout << "->";
	do {
		t = _getch();
		if (t == 's' || t == 'S' || t == 'W' || t == 'w') {
			gotoxy0(1, ch);
			cout << "  ";
			if (t == 's' || t == 'S') ch++;
			if (t == 'w' || t == 'W') ch--;
		}
		if (t >= '0' && t <= '6') {
			ch = t - '0';
			break;
		}
		ch = (ch + 7) % 7;
		gotoxy0(1, ch);
		cout << "->";
	} while (t != '\n' && t != '\r');

	system("cls");
	mode = ch;
	switch (ch + '0') {
	case '0': {
		Size = 20;
		speed = 200;
		big_apple_probability = 10;
		through_wall = 1;
		wall_protection = 0;
		self_protection = 1;
		adj_speed = 1;
		full_time = 1;
		gotoxy0(Size + 2, 6); cout << "【0】作弊模式";
		break;
	}
	case '1': {
		Size = 25;
		speed = 500;
		through_wall = 0;
		big_apple_probability = 30;
		wall_protection = 1;
		self_protection = 1;
		adj_speed = 1;
		full_time = 1;
		gotoxy0(Size + 2, 6); cout << "【1】娱乐模式";
		break;
	}
	case '2': {
		Size = 20;
		speed = 200;
		big_apple_probability = 20;
		wall_protection = 0;
		self_protection = 0;
		adj_speed = 0;
		through_wall = 1;
		full_time = 1;
		gotoxy0(Size + 2, 6); cout << "【2】经典模式";
		break;
	}
	case '3': {
		Size = 15;
		speed = 100;
		big_apple_probability = 10;
		through_wall = 0;
		wall_protection = 0;
		self_protection = 0;
		adj_speed = 0;
		full_time = 1;
		gotoxy0(Size + 2, 6); cout << "【3】普通模式";
		break;
	}
	case '4': {
		Size = 20;
		speed = 1000;
		big_apple_probability = 20;
		through_wall = 1;
		wall_protection = 1;
		self_protection = 0;
		adj_speed = 1;
		full_time = 0;
		gotoxy0(Size + 2, 6); cout << "【4】快速模式";
		break;
	}
	case '5': {
		system("mode con cols=130 lines=53");//改变宽高
		cout << "输入地图大小(10-50且只能是偶数)：";
		cin >> Size;
		Size = min(max(Size, 10), 50);
		if (Size & 1) {
			Size--;
			cout << "已自动转化为" << Size;
		}
		system("cls");
		speed = 0;
		big_apple_probability = 0;
		wall_protection = 0;
		self_protection = 0;
		through_wall = 0;
		adj_speed = 0;
		full_time = 0;
		gotoxy0(Size + 2, 6); cout << "【5】电脑模式";
		break;
	}
	case '6': {
		system("cls");
		cout << "地图大小："; cin >> Size; Size = max(min(Size, 50), 5);
		init_size = 2;
		cout << "移动间隔时间(ms)："; cin >> speed; speed = max(min(speed, 2000), 10);
		cout << "大苹果掉落概率(%)："; cin >> big_apple_probability; big_apple_probability = max(min(big_apple_probability, 100), 0);
		cout << "穿墙(1/0)：\n"; through_wall = _getch() - '0';
		if (!through_wall) cout << "撞墙不死亡(1/0)：\n", wall_protection = _getch() - '0';
		cout << "吃自己不死亡(1/0)：\n"; self_protection = _getch() - '0';
		cout << "允许调整速度(1/0)：\n"; adj_speed = _getch() - '0';
		cout << "允许手动移动(1/0)：\n"; full_time = !(_getch() - '0');
		system("cls");
		//			system("mode con cols=100 lines=45");//改变宽高
		gotoxy0(Size + 2, 6); cout << "【6】自定义模式";
		break;
	}
	}
	system("mode con cols=130 lines=53");//改变宽高
}

void init()
{
	SetColor();
	memset(map, sizeof(map), 0);
	gotoxy0(-1, -1);
	cout << "  x\ny";
	for (int i = 0; i < Size; i++) {
		gotoxy0(i + 1, -1); cout << i + 1;
		gotoxy0(-1, i + 1); cout << i + 1;
	}

	for (int i = 0; i <= Size + 1; i++)
	{
		gotoxy0(i, 0); cout << "═";
		gotoxy0(i, Size + 1); cout << "═";
		gotoxy0(0, i); cout << "║";
		gotoxy0(Size + 1, i); cout << "║";
	}
	gotoxy0(0, 0); cout << "╔";
	gotoxy0(0, Size + 1); cout << "╚";
	gotoxy0(Size + 1, Size + 1); cout << "╝";
	gotoxy0(Size + 1, 0); cout << "╗";

	snake.clear();
	line.clear();
	snake.push_back({ Size / 2 - 1,Size / 2 - 1 });
	line.insert(snake.back());
	gotoxy(Size / 2 - 1, Size / 2 - 1); SetColor(0x0c); cout << "■";

	SetColor();
	show();
	while (true) {
		char ch = _getch();
		if (ch == 'a' || ch == 'A') {
			way = 0; break;
		}
		if (ch == 'w' || ch == 'W') {
			way = 1; break;
		}
		if (ch == 'd' || ch == 'D') {
			way = 2; break;
		}
		if (ch == 's' || ch == 'S') {
			way = 3; break;
		}
	}
	for (int i = 0; i < init_size - 1; i++) {
		snake.push_back(snake.back().edge((way + 2) % 4));
		line.insert(snake.back());
		gotoxy(snake.back().x, snake.back().y); cout << "■";
	}
	start_time = clock();
	make_apple();

	lenth = snake.size();

	gotoxy0(Size + 2, -1);
	cout << "蛇头：";
	gotoxy0(Size + 2 + 6, -1);
	cout << "Apple:";
	gotoxy0(Size + 2, 9);
	cout << "step：";

	if (mode == 5) return;

	gotoxy0(Size + 2, 7); cout << "计分方式：";
	if (mode == 1) cout << "小苹果加log2(time)*(1000/speed)分并长长1m";
	else cout << "小苹果加1分并长长1m";

	gotoxy0(Size + 2, 8);
	cout << "大苹果加5分并缩短5m";

	if (mode == 1) gotoxy0(Size + 2, 12), cout << "碰到墙会持续扣分哦", gotoxy0(Size + 2, 13), cout << "吃到自己会扣得分的十分之一分(不足5分记作5分)";

	gotoxy0(Size + 2, 5);
	cout << "移动间隔：          \b\b\b\b\b\b\b\b\b\b" << speed << "ms";


}

void hide()
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO CursorInfo;
	GetConsoleCursorInfo(handle, &CursorInfo);//获取控制台光标信息
	CursorInfo.bVisible = false; //隐藏控制台光标
	SetConsoleCursorInfo(handle, &CursorInfo);//设置控制台光标状态
}

void backxy()
{
	COORD pos;
	pos.X = lx;
	pos.Y = ly;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void gotoxy0(int x, int y)
{
	COORD pos;
	pos.X = (x << 1) + 2;
	pos.Y = y + 1;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void gotoxy(int x, int y)
{
	COORD pos;
	pos.X = (x << 1) + 4;
	pos.Y = y + 2;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void getxy() {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(hConsole, &csbi);
	lx = csbi.dwCursorPosition.X, ly = csbi.dwCursorPosition.Y;
}

void SetColor(unsigned short BackGroundColor, unsigned short ForeColor)
{
	HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hCon, (ForeColor % 16) | (BackGroundColor % 16 * 16));
}

void SetColor(unsigned short x)
{
	HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hCon, x);
}

void make_apple()
{
	if (have_big_apple) {
		have_big_apple = 0;
		gotoxy(big_apple.x, big_apple.y);
		cout << "  ";
	}


	vector<xy>t;
	for (int i = 0; i < Size; i++)
		for (int j = 0; j < Size; j++)
			if (line.find({ i,j }) == line.end()) t.push_back({ i,j });
	random_shuffle(t.begin(), t.end());
	gotoxy(t[0].x, t[0].y);
	cout << "●";
	if (snake.size() > 10 && rand() % 100 + 1 <= big_apple_probability && t.size() >= 2) {
		gotoxy(t[1].x, t[1].y);
		if (rand() & 1) cout << "♀";
		else cout << "♂";
		have_big_apple = 1;
		big_apple = t[1];
		big_apple_time = clock();
	}
	apple = t.front();
}

int judge()
{
	if (snake.front().inmap() && line.find(snake.front()) == line.end()) return 0;

	if (!snake.front().inmap() && through_wall) {
		xy h = snake.front();
		h.x = (h.x + Size) % Size;
		h.y = (h.y + Size) % Size;
		snake[0] = h;
		return 0;
	}

	if (!snake.front().inmap() && wall_protection) return 2;

	if (line.find(snake.front()) != line.end() && self_protection) {
		if (mode == 1) {
			score -= max((long long)5, score / 10);
		}
		return 0;
	}
	return 1;
}



/*



*/
void show()
{
	gotoxy0(Size + 2, 1);
	cout << "长度：     \b\b\b\b\b" << snake.size();
	gotoxy0(Size + 2 + 3, 9);
	cout << step;
	gotoxy0(Size + 2, 4); cout << "游戏时间：";
	gotoxy0(Size + 2 + 5, 4);
	cout << (clock() - start_time) / 1000 << "s";
	if (mode == 5) return;


	gotoxy0(Size + 2, 0);
	cout << "已经吃了" << have_eaten << "个";
	gotoxy0(Size + 2, 2);
	cout << "得分：                    \b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b" << score;
	gotoxy0(Size + 2, 3);
	cout << "方向:     \b\b\b\b\b";
	if (way == 0) cout << "left";
	if (way == 1) cout << "up";
	if (way == 2) cout << "right";
	if (way == 3) cout << "down";


	if (mode == 1) gotoxy0(Size + 2, 15), cout << "当前苹果价值=            \b\b\b\b\b\b\b\b\b\b\b\b" << log2((clock() - start_time) / 1000.0) * (1000.0 / speed);
}

int calc()
{
	xy h = snake.front();
	if (snake.size() / Size < (Size * 0.5))
	{

		if (h.x < apple.x/*go_front*/ && h.y == Size - 1 && (snake.back().x < h.x || snake.size() < (unsigned int)Size - 1 || snake.back().x > apple.x))
		{
			if (h.x == Size - 1) {
				return 1;
			}
			if (!(apple.x & 1) && h.x + 1 == apple.x) {
				return 1;
			}
			if ((apple.x & 1) && h.x == apple.x) {
				return 1;
			}
			return 2;
		}
		if (h.x > apple.x/*go_back*/ && h.y == 1 && (snake.back().x < h.x || snake.size() < (unsigned int)Size - 1)) {
			return 1;
		}
		if (!h.y && snake.back().x > h.x) {
			if ((apple.x & 1) == 0 && apple.x == h.x) return 3;
			if ((apple.x & 1) && apple.x == h.x + 1) return 3;
			//否则就用普通规则 
			if (!h.x) return 3;
			return 0;
		}
	}

	if (!h.y) {
		if (!h.x) return 3;
		return 0;
	}
	if (!(h.x & 1)) {
		if (h.y == Size - 1) return 2;
		else return 3;
	}
	if (h.x & 1) {
		if (h.y == 1 && h.x != Size - 1) return 2;
		//else return 1;
	}

	return 1;

}