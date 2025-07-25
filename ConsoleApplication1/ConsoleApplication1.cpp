#define _CRT_SECURE_NO_WARNINGS
#define WIN_WIDTH 1012
#define	 WIN_HEIGHT 396
#define OBSTACLE_COUNT 10  
#define WIN_SCORE 20   
#include<stdio.h>
#include<graphics.h>
#include"tools.h"
#include<conio.h>
#include<vector>
#include<stdbool.h>
using namespace std;  
//滑铲跳跃逻辑
bool flag;
bool flag2;
//技能冷却
int timer1=0;
int timer2 = 0;
//分数
LOGFONT text;
//背景图像、速度、位置
IMAGE imaBgs[3];
IMAGE SUN,ground;
int groundX;
int bgX[3];
int bgspeed[3] = {1,2,4};
//人物类型
typedef enum {
	hero1,
	hero2
}bgHeros;
bgHeros hc;
//人物图像
IMAGE imahero[2][9];
//人物位置
int HeroX[2], HeroY[2],HeroIndex[2],renjumpindex[2];
//跳跃滑铲判定
bool heroJump;
bool herodown;
bool renjump2; 
//跳跃高度
int jumpHeightMax[2];
//下降速度
int jumpHeightOff;
int rendownspeed2;
//二段跳图像
IMAGE imgerduantiao1[7];
IMAGE imgerduantiao2[10];
//滑铲图像
IMAGE imadown1;
IMAGE imadown2;
vector<vector <IMAGE>> imgHeroDown;
//更新逻辑
int update;
//血量分数
int heroBlood;
int score;
//存储得分图像
IMAGE imgSZ[10];
//障碍物枚举
typedef enum {
	KULOU,
	DOG,
	GREAT,
	SISHEN,
	OBSTACLE_TYPE_COUNT
}obstacle_type;
obstacle_type type;
//障碍物属性
typedef struct obstacle
{
	obstacle_type type;
	int imaIndex;
	int x, y;
	int speed;
	int power;
	bool exist;
	bool hited;
	bool passed;
} obstacle_t;
obstacle_t obstacles[OBSTACLE_COUNT];
//障碍物图像
vector<vector<IMAGE>> obstacleImags;
//火焰
IMAGE fire;
int firespeed = 2;
int fX,fY;
bool fe;
//冰盾
IMAGE defence;
int dY, dX;
bool de;
//游戏基本内容初始化
void init1()
{
	initgraph(WIN_WIDTH, WIN_HEIGHT);
	char name[64] = {0};
	//背景图像
	loadimage(&SUN, "res/SUN.png", 1012, 396, true);
	loadimage(&imaBgs[0], "res/BK1.png", 2024, 350);
	loadimage(&imaBgs[1], "res/BK2.png", 2024, 300);
	loadimage(&imaBgs[2], "res/BK3.png");
	bgX[0] = 0;
	bgX[1] = 0;
	bgX[2] = 0;
	loadimage(&ground, "res/ground.png", 2024, 100, true);
	groundX = 0;
	
	update = true;
	//骷髅图像
	IMAGE imgkl;
	vector<IMAGE> imaklArray;
	for (int i = 0; i < 4; i++) {
		sprintf(name, "res/kl%d.png", i + 1);
		loadimage(&imgkl, name);
		imaklArray.push_back(imgkl);
	}
	obstacleImags.push_back(imaklArray);
	//狗子图像
	IMAGE imadog;
	vector<IMAGE> imadogArray;
	for (int i = 0; i < 8; i++) {
		sprintf(name, "res/dog%d.png", i + 1);
		loadimage(&imadog, name);
		imadogArray.push_back(imadog);
	}
	obstacleImags.push_back(imadogArray);
	//巨人图像
	IMAGE imggreat;
	vector<IMAGE> imggreatArray;
	for (int i = 0; i < 11; i++) {
		
		sprintf(name, "res/great%02d.png", i + 1);
		loadimage(&imggreat, name);
		imggreatArray.push_back(imggreat);
		
	}
	obstacleImags.push_back(imggreatArray);
	IMAGE imgsishen;
	vector<IMAGE> imgsishenArray;
	loadimage(&imgsishen, "res/sishen.png");
	imgsishenArray.push_back(imgsishen);
	obstacleImags.push_back(imgsishenArray);
	//初始化障碍物为不存在
	for (int i = 0; i < OBSTACLE_COUNT; i++)
	{
		obstacles[i].exist = false;
	}
	//人物血量
	heroBlood = 100;
	//受击音效
	preLoadSound("res/hit.mp3");

	//得分及图像
	score = 0;
	for (int i = 0; i < 10; i++) {
		sprintf(name, "res/sz/%d.png", i);
		loadimage(&imgSZ[i], name);
	}
	gettextstyle(&text); // 获取当前字体设置
	text.lfHeight = 50;   // 设置字体高度为 24
	settextstyle(&text); // 应用新的字体设置
}
//游戏人物初始化
void init2() {
	char name1[64] = { 0 };
	if (hc == hero1) {
		for (int a = 0; a < 9; a++) {
			sprintf(name1, "res/gn%d.png", a);
			loadimage(&imahero[hero1][a], name1);
			}
			HeroX[hero1] = WIN_WIDTH * 0.5 - imahero[hero1][0].getwidth() * 0.5;
			HeroY[hero1] = 345 - imahero[hero1][0].getheight();
			HeroIndex[hero1] = 0;
	}
	else if (hc == hero2) {
		for (int a = 0; a < 9; a++) {
			sprintf(name1, "res/bsr%02d.png", a);
			loadimage(&imahero[hero2][a], name1);
			}
			HeroX[hero2] = WIN_WIDTH * 0.5 - imahero[hero2][0].getwidth() * 0.5;
			HeroY[hero2] = 345 - imahero[hero2][0].getheight();
			HeroIndex[hero2] = 0;
	}
		

		for (int i = 0; i < 7; i++) {
			sprintf(name1, "res/1gnj%02d.png", i + 2);
			loadimage(&imgerduantiao1[i], name1);
			}
		for (int i = 0; i < 10; i++) {
			sprintf(name1, "res/bsd%02d.png", i + 11);
			loadimage(&imgerduantiao2[i], name1);
		}
	heroJump = false;
	renjump2 = false;
	flag = true;
	flag2 = false;

	jumpHeightMax[hero1] = 345 - imahero[hero1][0].getheight() - 120;
	jumpHeightMax[hero2] = 345 - imahero[hero2][0].getheight() - 120;
	jumpHeightOff = -6;
	rendownspeed2 = -8;

	vector<IMAGE> imagnd;
	for (int i = 0; i < 18; i++) {
		sprintf(name1, "res/gnd%02d.png", i);
		loadimage(&imadown1, name1,100,85,true);
		imagnd.push_back(imadown1);
	}
	imgHeroDown.push_back(imagnd);
	vector<IMAGE> imabsd;
	for (int i = 0; i < 11; i++) {
		sprintf(name1, "res/bsd%02d.png", i);
		loadimage(&imadown2, name1);
		imabsd.push_back(imadown2);
	}
	imgHeroDown.push_back(imabsd);

	herodown = false;
	//护盾和加血
	loadimage(&defence, "res/defence.png");
	de = false;
	dX = WIN_WIDTH * 0.5 + imahero[hero1][0].getwidth() * 0.5;
	dY = 345 -imahero[hero1][0].getheight();
	//火焰图像及位置
	loadimage(&fire, "res/fire.png");
	fe = false;
	fX = WIN_WIDTH * 0.5 + imahero[hero2][0].getwidth() * 0.5;
	fY = 345 - imahero[hero2][0].getheight();
	
}
//生成障碍物
void creatObstacle()
{
	int i;
	//循环检测，直到检测到未存在的障碍物
	for (i = 0; i < OBSTACLE_COUNT; i++)
	{
		if (obstacles[i].exist == false)
		{
			break;
		}
	}
	//大于则直接停止，不生成障碍物
	if (i >= OBSTACLE_COUNT) {
		return;
	}
	//对找到的i，进行设置
	obstacles[i].exist = true;//存在
	obstacles[i].hited = false;//未碰撞
	obstacles[i].imaIndex = 0;//
	obstacles[i].type = (obstacle_type)(rand() % 4);//强制转换，随机生成type的前三种


	
	//生成位置
	obstacles[i].x = WIN_WIDTH;
	obstacles[i].y = 350 - obstacleImags[obstacles[i].type][0].getheight();

	if (obstacles[i].type == KULOU) {
		obstacles[i].speed = 4;
		obstacles[i].power = 5;
	}

	else if (obstacles[i].type == DOG) {
		obstacles[i].speed = 8;
		obstacles[i].power = 10;
	}

	else if (obstacles[i].type ==GREAT) {
		obstacles[i].speed = 10;
		obstacles[i].power = 20;
		obstacles[i].y = 50;
	}
	else if (obstacles[i].type == SISHEN) {
		obstacles[i].speed = 6;
		obstacles[i].power = 10;
		obstacles[i].y = 200;
	}

	obstacles[i].passed = false;
}
//检测火球与障碍物的碰撞
void checkhit() {
	if (hc == hero2) {
		for (int i = 0; i < OBSTACLE_COUNT; i++) {
		if (fe&&obstacles[i].exist && obstacles[i].hited == false) {
			int off = 30;

			int firex1 = fX;
			int firex2 = fX + off;
			int firey1 = fY;
			int firey2 = fY + off;

			IMAGE img = obstacleImags[obstacles[i].type][obstacles[i].imaIndex];
			int b1x = obstacles[i].x + off;
			int b1y = obstacles[i].y + off;
			int b2x = obstacles[i].x + img.getwidth() - off;
			int b2y = obstacles[i].y + img.getheight() - off;

			if (rectIntersect(firex1, firey1, firex2, firey2, b1x, b1y, b2x, b2y)) {
				
				playSound("res/hit.mp3");
				fe = false;
				fX = WIN_WIDTH * 0.5 + imahero[hc][0].getwidth() * 0.5;
				fY = 345 - imahero[hc][0].getheight();
				obstacles[i].exist = false;
			}
		}
	}
	}
}
//检测人物与障碍物的碰撞
void checkHit()
{
	for (int i = 0; i < OBSTACLE_COUNT; i++) {
		if (obstacles[i].exist && obstacles[i].hited == false) {
			int a1x, a1y, a2x, a2y;
			int off = 30;
			if (!herodown) {
				a1x = HeroX[hc] + off;
				a1y = HeroY[hc] - off;
				a2x = HeroX[hc] + imahero[hc][HeroIndex[hc]].getwidth() - off;
				a2y = HeroY[hc] + imahero[hc][HeroIndex[hc]].getheight();
			}
			else
			{
				a1x = HeroX[hc] + off;
				a1y = 344 ;
				a2x = HeroX[hc] + imgHeroDown[hc][HeroIndex[hc]].getwidth() - off;
				a2y = 345;
			}

			IMAGE img = obstacleImags[obstacles[i].type][obstacles[i].imaIndex];
			int b1x = obstacles[i].x + off;
			int b1y = obstacles[i].y + off;
			int b2x = obstacles[i].x + img.getwidth() - off;
			int b2y = obstacles[i].y + img.getheight() -50;
			if (rectIntersect(a1x, a1y, a2x, a2y, b1x, b1y, b2x, b2y)) {
				if (de == false) {
						heroBlood -= obstacles[i].power;
				}
				playSound("res/hit.mp3");
				obstacles[i].hited = true;
				de = false;
			}	  
		}
	}
}
//背景、怪物、火球、人物的移动
void fly()
{
	for (int i = 0; i < 3; i++)
	{
		bgX[i] -= bgspeed[i];
		if (bgX[i] < -WIN_WIDTH)
		{
			bgX[i] = 0;
		}
	}
	groundX = groundX - bgspeed[2];
	if (groundX < -WIN_WIDTH / 2) {
		groundX = 0;
	}

	if (fe) {
		static int a = 0;
		fX += firespeed;
		a++;
		if (a > 100)
		{
			fe = false;
			a = 0;
			fX = WIN_WIDTH * 0.5 + imahero[hero2][0].getwidth() * 0.5;
			fY = 345 - imahero[hero2][0].getheight();
		}
	}

	if (heroJump) {
		flag2 = true;
		if (HeroY[hc] < jumpHeightMax[hc]) {
			jumpHeightOff = 6;
		}
		HeroY[hc] += jumpHeightOff;

		if (HeroY[hc] > 345 - imahero[hc][0].getheight()) {
			heroJump = false;
			flag2 = false;
			jumpHeightOff = -6;
		}
	}
	else if (renjump2) {
		flag2 = false;

			if (HeroY[hc] < jumpHeightMax[hc] -80 ){//第一次是起跳所以开始负的，接下来一定是下降所以为正即可
				rendownspeed2 = 8;
			}
			HeroY[hc] += rendownspeed2;
			if (HeroY[hc] > 345 - imahero[hc][0].getheight()) {//已经降到地面了
				renjump2 = false;
				rendownspeed2 = -8;
				flag = true;
			}
		

		//翻滚画面加载
		static int count = 0;
		count++;
		if (hc == hero1) {
			int delays[7] = { 1,2,2,2,2,2,1};   //想要分别控制12张图片的站场时间
			if (count >= delays[renjumpindex[hc]]) {    //站场时间不同
				count = 0;
				renjumpindex[hc]++;
				if (renjumpindex[hc] >= 7) {
					renjumpindex[hc] = 0;
			}
		}
		}
		else if (hc == hero2) {
			int delays[10] = { 1,1,1,2,2,2,2,1,1,1,};   //想要分别控制12张图片的站场时间
			if (count >= delays[renjumpindex[hc]]) {    //站场时间不同
				count = 0;
				renjumpindex[hc]++;
				if (renjumpindex[hc] >= 10) {
					renjumpindex[hc] = 0;
				}
			}
		}
		
	}
	else if (herodown) {
		static int count = 0;
		count++;
		flag = true;
		if (hc==hero1) {
			int delays[18] = { 1,1,1,2,2,2,2,2,2,2,2,2,2,2,2,1,1};
			if (count >= delays[HeroIndex[hero1]]) {
				count = 0;
				HeroIndex[hero1]++;
				if (HeroIndex[hero1] == 18) {
					HeroIndex[hero1] = 0;
					herodown = false;
				}
			}
		}
		else if (hc==hero2) {
			int delays[11] = { 3,3,3,3,3,3,3,3,3,3,3 };
			if (count >= delays[HeroIndex[hero2]]) {
				count = 0;
				HeroIndex[hero2]++;
				if (HeroIndex[hero2] == 11) {
					HeroIndex[hero2] = 0;
					herodown = false;
				}
			}
		}
	}
	else {
			HeroIndex[hc] = (HeroIndex[hc] + 1) % 9;
		}
		static int frameCount = 0;
		static int torToiseFre = 60;
		frameCount++;
		if (frameCount > torToiseFre)
		{
			frameCount = 0;
			torToiseFre = 50 + rand() % 60;
			creatObstacle();
		}
		for (int i = 0; i < OBSTACLE_COUNT; i++) {
			if (obstacles[i].exist) {
				obstacles[i].x -= obstacles[i].speed + bgspeed[i];
				if (obstacles[i].x < -obstacleImags[obstacles[i].type][0].getwidth() * 2) {
					obstacles[i].exist = false;
				}

				int len = obstacleImags[obstacles[i].type].size();
				obstacles[i].imaIndex = (obstacles[i].imaIndex + 1) % len;
			}
		}
		checkhit();
		checkHit();  
	}
//生成背景
void updateBg()
{
	putimage(0, -25, &SUN);
	putimagePNG(bgX[0], 20, &imaBgs[0]);
	putimagePNG(bgX[1], 60, &imaBgs[1]);
	putimagePNG(bgX[2], 119, &imaBgs[2]);
	putimagePNG2(groundX, WIN_HEIGHT - 50, &ground);
}
//跳跃
void jump()
{
	heroJump = true;
	renjump2 = false;
	update = true;
	herodown = false;
	flag2 = true;
	rendownspeed2 = -6;
}
//滑铲
void down()
{
	herodown = true;
	update = true;
	HeroY[hc] = 345 - imahero[hc][0].getheight();
	heroJump = false;
	HeroIndex[hc] = 0;
	renjump2 = false;
	flag2 = false;
	jumpHeightOff = -6;
	rendownspeed2 = -6;
}
//二段跳
void jump2()
{
	renjump2 = true;
	heroJump = false;
	update = true;
	herodown = false;
	jumpHeightOff = -6;//上升速度
	flag = false;

}
//火球
void shoot() {
	if (timer2 > 100) {
		timer2 = 0;
		if (hc == hero2) {
		update = true;
		fe = true;
	}
	}
	
}
//冰盾
void defend() {
	if (timer1 > 100) {
		timer1 = 0;
		if (hc==hero1) {
			update = true;
			de = true;
	}
	}
	
}
//按键输入
void keyEvent()
{
	char ch;
	if (_kbhit())
	{
		ch = _getch();
		if ((ch == 'w' || ch == 'W') && flag == true) {
			jump();
		}
		else if (ch == 's' || ch == 'S') {
			down();
		}
		else if (ch == 'f'||ch=='F') {
			shoot();
			defend();
		}
		else if (ch == ' ' && flag2 == true) {
			jump2();
		}
	}
}
//生成怪物
void updateEnemy()
{


	for (int i = 0; i < OBSTACLE_COUNT; i++)
	{
		if (obstacles[i].exist)
		{
			putimagePNG2(obstacles[i].x, obstacles[i].y, WIN_WIDTH, &obstacleImags[obstacles[i].type][obstacles[i].imaIndex]);
		}
	}
}
//生成人物
void updateHero()
{
	if (hc == hero1) {
		 if (!herodown && !renjump2 ) {
			 if(HeroIndex[hero1]>8)
				putimagePNG2(HeroX[hero1], HeroY[hero1], &imahero[0][HeroIndex[hero1]-9]);
			 else 
				 putimagePNG2(HeroX[hero1], HeroY[hero1], &imahero[0][HeroIndex[hero1]]);
		}
		else if (renjump2) {
			putimagePNG2(HeroX[hero1], HeroY[hero1], &imgerduantiao1[renjumpindex[hero1]]);
			
		}
		else  {
			int y = 345 - imgHeroDown[hero1][HeroIndex[hero1]].getheight();
			putimagePNG2(HeroX[hero1], y, &imgHeroDown[hero1][HeroIndex[hero1]]);
		}
	}
	if (hc == hero2) {
		if (!herodown && !renjump2 ) {
			if (HeroIndex[hero2] > 8)
				putimagePNG2(HeroX[hero2], HeroY[hero2], &imahero[1][HeroIndex[hero2] - 2]);
			else
				putimagePNG2(HeroX[hero2], HeroY[hero2], &imahero[1][HeroIndex[hero2]]);
		}
		else if (renjump2) {
				putimagePNG2(HeroX[hero2], HeroY[hero2], &imgerduantiao2[renjumpindex[hero2]]);
		}
		else {
			int y = 345 - imgHeroDown[hero2][HeroIndex[hero2]].getheight();
			putimagePNG2(HeroX[hero2], y, &imgHeroDown[hero2][HeroIndex[hero2]]);
		}
	}
	
}	
//更新血量
void updateBloodBar()
{
	drawBloodBar(10, 10, 200, 10, 2, BLUE, DARKGRAY, RED, heroBlood / 100.0);
}
//生成冰盾
void updatedefence()
{
	if (de) {
		putimagePNG(dX, dY, &defence);
		}
}
//生成火球
void updatefire() {
	if (fe&&!herodown) {
		putimagePNG(fX, fY, &fire);
	}
	if (fe && herodown) {
		int fy = fY + 100;
		putimagePNG(fX, fy, &fire);
	}
}
//检测人物是否经过了障碍物
void checkOver()
{
	if (heroBlood <= 0)
	{
		char ScoreArray[8];
		sprintf(ScoreArray, "%d", score);
		loadimage(0, "res/death1.png",1012,396,true);
		FlushBatchDraw();
		outtextxy(600,135, ScoreArray);
		mciSendString("stop res/Maclk.mp3", 0, 0, 0);
		system("pause");
		heroBlood = 100;
		score = 0;
		mciSendString("play res/Maclk.mp3 repeat", 0, 0, 0);
	}
}
//检测分数
void checkScore()
{

	for (int i = 0; i < OBSTACLE_COUNT; i++) {
		if (obstacles[i].exist && obstacles[i].passed == false && obstacles[i].hited == false && obstacles[i].x + obstacleImags[obstacles[i].type][0].getwidth() < HeroX[hc])
		{
			score++;
			obstacles[i].passed = true;

		}
	}
}
//更新分数
void updateScore()
{
	char str[8];
	sprintf(str, "%d", score);

	int x = 20;
	int y = 25;

	for (int i = 0; str[i]; i++) {
		int sz = str[i] - '0';
		putimagePNG(x, y, &imgSZ[sz]);
		x += imgSZ[sz].getwidth() + 5;
	}
}
//开始界面
void startUI() {
	/*mciSendString("play res/BGM.mp3 repeat", 0, 0, 0);*/
	IMAGE imgMenu, begin, exit;
	int	flag1 = 0, flag2 = 0;
	loadimage(&imgMenu, "res/beginbg.jpg", 1012, 396, true);
	loadimage(&begin, "res/begin.png", 100, 50, true);
	loadimage(&exit, "res/exit.png", 100, 50, true);

	while (1) {
		BeginBatchDraw();
		putimage(0, 0, &imgMenu);
		putimage(200, 250, &begin);
		putimage(800, 250, &exit);
		ExMessage	msg;
		if (peekmessage(&msg)) {

			if (msg.message == WM_LBUTTONDOWN && msg.x > 200 && msg.x < 300 && msg.y>250 && msg.y < 300) {
				flag1 = 1;
			}
			else if (msg.message == WM_LBUTTONDOWN && msg.x > 800 && msg.x < 900 && msg.y>250 && msg.y < 300)
			{
				flag2 = 1;
			}
		}

		else if (msg.message == WM_LBUTTONUP && flag1 == 1) {
			/*mciSendString("close res/BGM.mp3", 0, 0, 0);*/
			EndBatchDraw();
			return;
		}
		else if (msg.message == WM_LBUTTONUP && flag2 == 1)
		{
			closegraph();
			return;
		}
		EndBatchDraw();
	}
}
//人物选择
void HeroChoose()
{
	cleardevice();
	IMAGE HERO1, HERO2, chooseBg;
	loadimage(&HERO1, "res/hhero1.png");
	loadimage(&HERO2, "res/hhero2.png");
	loadimage(&chooseBg, "res/cbg.jpg", 1012, 396, true);
	while (1)
	{
		BeginBatchDraw();
		putimagePNG2(0, 0, &chooseBg);
		putimagePNG2(300, 200, &HERO1);
		putimagePNG2(600, 170, &HERO2);
		ExMessage	msg;
		if (peekmessage(&msg))
		{
			if (msg.message == WM_LBUTTONDOWN && msg.x > 350 && msg.x < 350 + HERO1.getwidth()
				&& msg.y>200 && msg.y < 200 + HERO1.getheight())
			{
				hc = hero1;
				return;
			}
			if (msg.message == WM_LBUTTONDOWN && msg.x > 600 && msg.x < 600 + HERO1.getwidth()
				&& msg.y>200 && msg.y < 200 + HERO1.getheight())
			{
				hc=hero2;
				return;
			}
		}
		EndBatchDraw();
	}
}

int main(void)
{
	mciSendString("play res/BGM.mp3 repeat", 0, 0, 0);
	init1();
	startUI();
	HeroChoose();
	init2();
	
	int timer = 0;
	while (1) {
		keyEvent();
		timer += getDelay();
		if (timer > 15) {
			timer = 0;
			update = true;
		}

		if (update) {
			timer1++;
			timer2++;
			update = false;
			BeginBatchDraw();
			updateBg();
			updateHero();
			updateEnemy();
			updateBloodBar();	
			updateScore();
			updatedefence();
			updatefire();
			fly();
			EndBatchDraw();
			checkOver();
			checkScore();
		}
	}

	system("pause");

	return 0;
}

