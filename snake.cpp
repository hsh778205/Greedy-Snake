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
int lx,ly;
void hide();
void backxy();
void gotoxy0(int,int);
void gotoxy(int,int);
void getxy(); 
const int A=10,B=11,C=12,D=13,E=14,F=15;
void SetColor(unsigned short,unsigned short);
void SetColor(unsigned short=0x07);
unsigned long long step;
int speed,size,lenth,way,tme,score,init_size,start_time,clr=1,have_eaten;
int big_apple_probability,big_apple_save_time=10000,big_apple_time,mode;
int his[10];
bool through_wall,wall_protection,self_protection,full_time,adj_speed;
bool have_big_apple;
int delta[4][2]={
{-1,0},
{0,-1},
{1,0},
{0,1}
};
struct xy{
	int x,y;
	bool operator==(xy t)
	{
		return x==t.x&&y==t.y;
	}
	bool inmap(){
		return x<size&&x>=0&&y<size&&y>=0;
	}
	xy edge(int w){
		return {x+delta[w][0],y+delta[w][1]};
	}
}apple,big_apple;

bool operator<(xy a,xy b)
{
	return a.x*100+a.y<b.x*100+b.y;
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
	while(true)
	{
		show();
		if(mode==5)
		{
			if(_kbhit()){
//				char ch=getch();
				if(getch()==' '){
					int t=clock();
					while(getch()!=' ');;
					start_time+=clock()-t;
				}
			}
			
			way=calc();
		}
		else
		{
			tme=clock();
			while(clock()-tme<=speed)
			{
				gotoxy0(size+2+5,4);
				cout<<(clock()-start_time)/1000<<"s";
				if(mode==1) gotoxy0(size+2,15),cout<<"��ǰƻ����ֵ="<<log2((clock()-start_time)/1000.0)*(1000.0/speed);
				if(_kbhit()){
					char ch=getch();
					if((ch=='a'||ch=='A')&&way!=2){
						way=0;break;
					}
					if((ch=='w'||ch=='W')&&way!=3){
						way=1;break;
					}
					if((ch=='d'||ch=='D')&&way!=0){
						way=2;break;
					}
					if((ch=='s'||ch=='S')&&way!=1){
						way=3;break;
					}
					if(adj_speed)
					if(ch=='+'||ch=='='||ch=='.'){
						speed=max(0,speed-10);
						gotoxy0(size+2,5);
						cout<<"�ƶ������        \b\b\b\b\b\b\b\b"<<speed<<"ms";
						if(mode==1) gotoxy0(size+2,15),cout<<"��ǰƻ����ֵ=            \b\b\b\b\b\b\b\b\b\b\b\b"<<log2((clock()-start_time)/1000.0)*(1000.0/speed);
					}
					if(adj_speed)
					if(ch=='-'||ch==','){
						speed+=10;
						gotoxy0(size+2,5);
						cout<<"�ƶ������        \b\b\b\b\b\b\b\b"<<speed<<"ms";
						if(mode==1) gotoxy0(size+2,15),cout<<"��ǰƻ����ֵ=            \b\b\b\b\b\b\b\b\b\b\b\b"<<log2((clock()-start_time)/1000.0)*(1000.0/speed);
					}
					if(ch=='x'||ch=='X') save(),exit(0);
					if(ch=='v'||ch=='V') save();
					if(ch==' '){//��ͣ 
						long ttime=clock();
						char c;
						do{
							c=getch();
							if(adj_speed)
							if(c=='+'||c=='='||c=='.'){
								speed=max(0,speed-10);
								gotoxy0(size+2,5);
								cout<<"�ƶ������        \b\b\b\b\b\b\b\b"<<speed<<"ms";
								if(mode==1) gotoxy0(size+2,15),cout<<"��ǰƻ����ֵ=            \b\b\b\b\b\b\b\b\b\b\b\b"<<log2((clock()-start_time)/1000.0)*(1000.0/speed);
							}
							if(adj_speed)
							if(c=='-'||c==','){
								speed+=10;
								gotoxy0(size+2,5);
								cout<<"�ƶ������        \b\b\b\b\b\b\b\b"<<speed<<"ms";
								if(mode==1) gotoxy0(size+2,15),cout<<"��ǰƻ����ֵ=            \b\b\b\b\b\b\b\b\b\b\b\b"<<log2((clock()-start_time)/1000.0)*(1000.0/speed);
							}
							if(c=='x') save(),exit(0);
							if(c=='v') save();
						}while(c!=' ');
						tme+=clock()-ttime;
						big_apple_time+=clock()-ttime;
					}
				}
			}
		}
		
		if(full_time&&clock()-tme<=speed) Sleep(speed-clock()+tme);
		
		if(have_big_apple){
			gotoxy0(size+2,14);
			cout<<"                       ";
			if(clock()-big_apple_time<big_apple_save_time){
				gotoxy0(size+2,14);
				cout<<"��ƻ��ʣ��ʱ�䣺"<<big_apple_save_time-clock()+big_apple_time<<"ms";
				gotoxy(big_apple.x,big_apple.y);
				if(rand()&1) cout<<"��";
				else cout<<"��";
			}
			else{
				gotoxy(big_apple.x,big_apple.y);
				cout<<"  ";
				have_big_apple=0;
			}
		}
		
		snake.push_front({snake.front().x+delta[way][0],snake.front().y+delta[way][1]});
		
		if(judge()==1) break;
		if(judge()==2){
			if(mode==1) score--;
			gotoxy0(size+2,2);
			cout<<"�÷֣�            \b\b\b\b\b\b\b\b\b\b\b\b"<<score;
			snake.pop_front();
			continue;
		}
		line.insert(snake.front());
		gotoxy(snake.front().x,snake.front().y);
		SetColor(0x0c);
		cout<<"��"; 
		
		//���ڶ������ɫ�Ļ��� 
		gotoxy(snake[1].x,snake[1].y);
		SetColor();
		if(mode==1){
			clr++;
			clr%=15;
			clr++;
			if(clr==C||clr==7||clr==0) clr++;
			SetColor(0,clr);
		}
		cout<<"��";
		SetColor();
		
		//����Ե���ƻ�� 
		if(have_big_apple&&snake.front()==big_apple){
			score+=5;
			for(int i=0;i<5&&snake.size()>2;i++){
				line.erase(snake.back());
				gotoxy(snake.back().x,snake.back().y);
				snake.pop_back();lenth--;
				SetColor();
				cout<<"  ";
				
			}
			big_apple_time=-10000;
			show();
		}
		
		if(snake.front()==apple){
			have_eaten++;
			if(have_big_apple) big_apple_time=-10000;
			lenth++;
			
			if(snake.size()==(unsigned)size*size){
				show();
				break;
			}
			
			make_apple();
			
			if(mode==1){
				score+=log2((clock()-start_time)/1000.0)*(1000.0/speed);
				
			}
			else score+=1;
			
			show();
		} 
		else{
			line.erase(snake.back());
			gotoxy(snake.back().x,snake.back().y);
			snake.pop_back();
			SetColor();
			cout<<"  ";
		}
		
		
		gotoxy0(size+2+3,-1);
		cout<<"     \b\b\b\b\b"<<snake.front().x<<","<<snake.front().y;
		gotoxy0(size+2+5+4,-1);
		cout<<"    \b\b\b\b\b"<<apple.x<<","<<apple.y;
		gotoxy0(size+2+3,9);
		cout<<++step;
	}
	show();
	save();
	cin.get();cin.get();
	if(snake.size()>=(unsigned int)size*size){
		gotoxy0(size/2-2,size/2); 
		SetColor(4,7);
		cout<<"You have won!!!";
		cin.get();
		return 0;
	}
	
	for(int i=1;i<size;i++)
	{
		gotoxy0(1,i);
		for(int j=1;j*7<size;j++)cout<<"You are dead  ";
		cout<<"\n"; 
	}
	cout<<"\n\n\n\n\n";
	return 0;
}

void save()
{
	his[mode]=max(his[mode],score);
	ofstream fout("history.txt",ios::out);
	for(int i=0;i<10;i++) fout<<his[i]<<endl;
}

void setting()
{
	memset(his,sizeof(his),0x3f);
	step=0;
	if(system("@echo off\ndir /a history.txt")==1){
		save();
	}
	ifstream fin("history.txt",ios::in);
	if(fin.eof()||fin.peek()==EOF){
		save();
	}
	else{
		for(int i=0;i<10;i++) fin>>his[i];
	}
	
	SetColor();
	system("cls");
	system("mode con cols=80 lines=40");//�ı���
	cout<<"\t��� ����\t��ͼ\t�ٶ�\t��ƻ��\t��������\n";
	cout<<"\t��0������ģʽ\t20*20\t5m/s\t15%\t��ǽ ���Լ������� �ɵ����ٶ�\n";
	cout<<"\t��1������ģʽ\t25*25\t2m/s\t30%\tײǽ���� ����Ʒַ�ʽ ��ɫ��\n";
	cout<<"\t��2������ģʽ\t20*20\t5m/s\t15%\t��ǽ\n";
	cout<<"\t��3����ͨģʽ\t15*15\t10m/s\t0%\t��\n";
	cout<<"\t��4������ģʽ\t20*20\t1m/s\t20%\t��ǽ �ɵ����ٶ� ���ֶ��ƶ�\n";
	cout<<"\t��5������ģʽ\n";
	cout<<"\t��6���Զ���\n";
	cout<<"����������ѡ��  ��w(��)/s(��)�ƶ� �س�ȷ��\n";
	
	vector<string>sentence;
	sentence.push_back("�������Ķ��������Ϣ����������������������������������������");
	sentence.push_back("��ѿ���̨���ڵ��ֺŸĳ�14�����£�����");
	sentence.push_back("�뽫���뷨����Ӣ��ģʽ����");
	sentence.push_back("���ո����ͣ��Ϸ");
	sentence.push_back("��v������(ֻ��¼���) ��x�����沢�˳�");
	sentence.push_back("�ڿɵ����ٶȵ�ģʽ�£���+���٣���-����"); 
	sentence.push_back("����+���Լӿ��ٶȣ�=Ҳ����");
	sentence.push_back("��ʵ ,��.Ҳ���Ե����ٶȣ�����һ��Ҫ��Ӣ�ĵķ��ţ�");
	sentence.push_back("����ģʽ�²����������Ƽ���������ģʽ"); 
	sentence.push_back("����ģʽ�£�����Խ�죬��Ϸʱ��Խ�����ӷ�Խ��"); 
	sentence.push_back("����ģʽ�£��ǵ��ȼӿ�һ���ٶ�Ŷ"); 
	sentence.push_back("����ģʽ�£�ֻҪ�����ٹ��죬�������ж��"); 
	sentence.push_back("��ǽ��ײǽ�������ɹ��棬��ǽ����"); 
	for(unsigned int i=0;i<sentence.size();i++)
	{
		gotoxy0(16-sentence[i].size()/4,10+i*2);
		cout<<sentence[i];
	}
	
	
	
	int ch=0;
	char t;
	gotoxy0(1,0);
	cout<<"->"; 
	do{
		t=getch();
		if(t=='s'||t=='S'||t=='W'||t=='w'){
			gotoxy0(1,ch);
			cout<<"  ";
			if(t=='s'||t=='S') ch++;
			if(t=='w'||t=='W') ch--;
		}
		if(t>='0'&&t<='6'){
			ch=t-'0';
			break;
		}
		ch=(ch+7)%7;
		gotoxy0(1,ch);
		cout<<"->";
	}while(t!='\n'&&t!='\r');
	
	system("cls");
	mode=ch;
	switch(ch+'0'){
		case '0':{
			size=20;
			speed=200;
			big_apple_probability=10;
			through_wall=1;
			wall_protection=0;
			self_protection=1; 
			adj_speed=1;
			full_time=1;
			gotoxy0(size+2,6);cout<<"��0������ģʽ";
			break;
		}
		case '1':{
			size=25;
			speed=500;
			through_wall=0; 
			big_apple_probability=30;
			wall_protection=1;
			self_protection=1;
			adj_speed=1;
			full_time=1;
			gotoxy0(size+2,6);cout<<"��1������ģʽ";
			break;
		}
		case '2':{
			size=20;
			speed=200;
			big_apple_probability=20;
			wall_protection=0;
			self_protection=0;
			adj_speed=0;
			through_wall=1; 
			full_time=1;
			gotoxy0(size+2,6);cout<<"��2������ģʽ";
			break;
		}
		case '3':{
			size=15;
			speed=100;
			big_apple_probability=10;
			through_wall=0; 
			wall_protection=0;
			self_protection=0;
			adj_speed=0;
			full_time=1;
			gotoxy0(size+2,6);cout<<"��3����ͨģʽ";
			break;
		}
		case '4':{
			size=20;
			speed=1000;
			big_apple_probability=20;
			through_wall=1;
			wall_protection=1;
			self_protection=0;
			adj_speed=1;
			full_time=0;
			gotoxy0(size+2,6);cout<<"��4������ģʽ";
			break;
		}
		case '5':{
			system("mode con cols=130 lines=53");//�ı���
			cout<<"�����ͼ��С(10-50��ֻ����ż��)��";
			cin>>size;
			size=min(max(size,10),50);
			if(size&1){
				size--; 
				cout<<"���Զ�ת��Ϊ"<<size;
			} 
			system("cls");
			speed=0;
			big_apple_probability=0;
			wall_protection=0;
			self_protection=0;
			through_wall=0; 
			adj_speed=0;
			full_time=0;
			gotoxy0(size+2,6);cout<<"��5������ģʽ";
			break;
		}
		case '6':{
			system("cls");
			cout<<"��ͼ��С��";cin>>size;size=max(min(size,50),5);
			init_size=2;
			cout<<"�ƶ����ʱ��(ms)��";cin>>speed;speed=max(min(speed,2000),10);
			cout<<"��ƻ���������(%)��";cin>>big_apple_probability;big_apple_probability=max(min(big_apple_probability,100),0);
			cout<<"��ǽ(1/0)��\n";through_wall=getch()-'0';
			if(!through_wall) cout<<"ײǽ������(1/0)��\n",wall_protection=getch()-'0';
			cout<<"���Լ�������(1/0)��\n";self_protection=getch()-'0';
			cout<<"��������ٶ�(1/0)��\n";adj_speed=getch()-'0';
			cout<<"�����ֶ��ƶ�(1/0)��\n";full_time=!(getch()-'0');
			system("cls");
			system("mode con cols=100 lines=45");//�ı���
			gotoxy0(size+2,6);cout<<"��6���Զ���ģʽ"; 
			break;
		}
	}
}

void init()
{
	SetColor();
	memset(map,sizeof(map),0);
	gotoxy0(-1,-1);
	cout<<"  x\ny";
	for(int i=0;i<size;i++){
		gotoxy0(i+1,-1);cout<<i+1;
		gotoxy0(-1,i+1);cout<<i+1;
	}
	
	for(int i=0;i<=size+1;i++)
	{
		gotoxy0(i,0);cout<<"�T";
		gotoxy0(i,size+1);cout<<"�T";
		gotoxy0(0,i);cout<<"�U";
		gotoxy0(size+1,i);cout<<"�U";
	}
	gotoxy0(0,0);cout<<"�X";
	gotoxy0(0,size+1);cout<<"�^";
	gotoxy0(size+1,size+1);cout<<"�a";
	gotoxy0(size+1,0);cout<<"�[";
	
	snake.clear();
	line.clear();
	snake.push_back({size/2-1,size/2-1});
	line.insert(snake.back());
	gotoxy(size/2-1,size/2-1);SetColor(0x0c);cout<<"��";
	
	SetColor();
	show(); 
	while(true){
		char ch=getch();
		if(ch=='a'||ch=='A'){
			way=0;break;
		}
		if(ch=='w'||ch=='W'){
			way=1;break;
		}
		if(ch=='d'||ch=='D'){
			way=2;break;
		}
		if(ch=='s'||ch=='S'){
			way=3;break;
		}
	}
	for(int i=0;i<init_size-1;i++){
		snake.push_back(snake.back().edge((way+2)%4));
		line.insert(snake.back());
		gotoxy(snake.back().x,snake.back().y);cout<<"��";
	}
	start_time=clock();
	make_apple();
	
	lenth=snake.size();
	
	gotoxy0(size+2,-1);
	cout<<"��ͷ��";
	gotoxy0(size+2+6,-1);
	cout<<"Apple:";
	gotoxy0(size+2,9);
	cout<<"step��"; 
	
	if(mode==5) return;
	
	gotoxy0(size+2,7); cout<<"�Ʒַ�ʽ��";
	if(mode==1) cout<<"Сƻ����log2(time)*(1000/speed)�ֲ�����1m";
	else cout<<"Сƻ����1�ֲ�����1m";
	
	gotoxy0(size+2,8);
	cout<<"��ƻ����5�ֲ�����5m";
	
	if(mode==1) gotoxy0(size+2,12),cout<<"����ǽ������۷�Ŷ",gotoxy0(size+2,13),cout<<"�Ե��Լ���۵÷ֵ�ʮ��֮һ��(����5�ּ���5��)";
	
	gotoxy0(size+2,5);
	cout<<"�ƶ������          \b\b\b\b\b\b\b\b\b\b"<<speed<<"ms";
	
	
}

void hide()
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO CursorInfo;
	GetConsoleCursorInfo(handle, &CursorInfo);//��ȡ����̨�����Ϣ
	CursorInfo.bVisible = false; //���ؿ���̨���
	SetConsoleCursorInfo(handle, &CursorInfo);//���ÿ���̨���״̬
}

void backxy()
{
	COORD pos;
	pos.X = lx;
	pos.Y = ly;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),pos);
} 

void gotoxy0(int x,int y)
{
	COORD pos;
	pos.X =(x<<1)+2;
	pos.Y =y+1;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),pos);
}

void gotoxy(int x, int y)
{
	COORD pos;
	pos.X =(x<<1)+4;
	pos.Y =y+2;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),pos);
}

void getxy(){
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
	lx=csbi.dwCursorPosition.X,ly=csbi.dwCursorPosition.Y;
} 

void SetColor(unsigned short BackGroundColor,unsigned short ForeColor)
{
    HANDLE hCon=GetStdHandle(STD_OUTPUT_HANDLE);  
    SetConsoleTextAttribute(hCon,(ForeColor%16)|(BackGroundColor%16*16));  
}

void SetColor(unsigned short x)
{
	HANDLE hCon=GetStdHandle(STD_OUTPUT_HANDLE);  
    SetConsoleTextAttribute(hCon,x);  
}

void make_apple()
{
	if(have_big_apple){
		have_big_apple=0;
		gotoxy(big_apple.x,big_apple.y);
		cout<<"  "; 
	}
	
	
	vector<xy>t;
	for(int i=0;i<size;i++)
	for(int j=0;j<size;j++)
	if(line.find({i,j})==line.end()) t.push_back({i,j});
	random_shuffle(t.begin(),t.end());
	gotoxy(t[0].x,t[0].y);
	cout<<"��";
	if(snake.size()>10&&rand()%100+1<=big_apple_probability&&t.size()>=2){
		gotoxy(t[1].x,t[1].y);
		if(rand()&1) cout<<"��";
		else cout<<"��";
		have_big_apple=1;
		big_apple=t[1];
		big_apple_time=clock();
	}
	apple=t.front();
}

int judge()
{
	if(snake.front().inmap()&&line.find(snake.front())==line.end()) return 0;
	
	if(!snake.front().inmap()&&through_wall){
		xy h=snake.front();
		h.x=(h.x+size)%size;
		h.y=(h.y+size)%size;
		snake[0]=h;
		return 0;
	}
	
	if(!snake.front().inmap()&&wall_protection) return 2;
	
	if(line.find(snake.front())!=line.end()&&self_protection){
		if(mode==1){
			score-=max(5,score/10); 
		} 
		return 0;
	}
	return 1;
}



/*



*/ 
void show() 
{
	gotoxy0(size+2,1);
	cout<<"���ȣ�     \b\b\b\b\b"<<snake.size();
	gotoxy0(size+2+3,9);
	cout<<step;
	gotoxy0(size+2,4);cout<<"��Ϸʱ�䣺";
	gotoxy0(size+2+5,4);
	cout<<(clock()-start_time)/1000<<"s";
	if(mode==5) return;
	
	
	gotoxy0(size+2,0);
	cout<<"�Ѿ�����"<<have_eaten<<"��"; 
	gotoxy0(size+2,2);
	cout<<"�÷֣�            \b\b\b\b\b\b\b\b\b\b\b\b"<<score;
	gotoxy0(size+2,3);
	cout<<"����:     \b\b\b\b\b";
	if(way==0) cout<<"left";
	if(way==1) cout<<"up";
	if(way==2) cout<<"right";
	if(way==3) cout<<"down";
	
	
	if(mode==1) gotoxy0(size+2,15),cout<<"��ǰƻ����ֵ=            \b\b\b\b\b\b\b\b\b\b\b\b"<<log2((clock()-start_time)/1000.0)*(1000.0/speed);
}

int calc()
{
	xy h=snake.front();
	if(snake.size()/size<((unsigned int)size>>1))
	{
		 
		if(h.x<apple.x/*go_front*/&&h.y==size-1&&(snake.back().x<h.x||snake.size()<(unsigned int)size-1||snake.back().x>apple.x))
		{
			if(h.x==size-1){
				return 1;
			}
			if(!(apple.x&1)&&h.x+1==apple.x){
				return 1;
			}
			if((apple.x&1)&&h.x==apple.x){
				return 1;
			}
			return 2;
		}
		if(h.x>apple.x/*go_back*/&&h.y==1&&(snake.back().x<h.x||snake.size()<(unsigned int)size-1)){
			return 1;
		}
		if(!h.y&&snake.back().x>h.x){
			if((apple.x&1)==0&&apple.x==h.x) return 3;
			if((apple.x&1)&&apple.x==h.x+1) return 3;
			//���������ͨ���� 
			if(!h.x) return 3;
			return 0; 
		}
	}
	
	if(!h.y){
		if(!h.x) return 3;
		return 0;
	}
	if(!(h.x&1)){
		if(h.y==size-1) return 2;
		else return 3;
	}
	if(h.x&1){
		if(h.y==1&&h.x!=size-1) return 2;
		//else return 1;
	}
	
	return 1;
	
}
