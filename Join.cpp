#include <graphics.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAP_LENGTH 6
#define MAX_CAT 36
#define INIT_STEP 150
#define INIT_STONE 4
#define INIT_CAT 3

#define EMPTY 0
#define PLATE 1
#define CAT 2
#define BOMB 3
#define RAINBOW 4
#define STONE 5
#define LEAF 6
#define BUSH 7
#define TREE 8
#define HOUSE 9
#define CAMP 10
#define PALACE_1 11
#define PALACE_2 12
#define PALACE_3 13
#define GRAVE 14

#define left 0
#define up 1
#define right 2
#define down 3

typedef int Type;
typedef struct{//位置结构体 
	int x;
	int y;
}Position;
typedef struct{//物品结构体 
	Type type;
	Position pos;
}Item, Map[MAP_LENGTH][MAP_LENGTH];
typedef struct{//玩家结构体 
	int score;
	int step;
	int money;
}User;
typedef Position ElemType;
#include "SqStack.h"

IMAGE scoreBlack;
IMAGE welcomeSur, gameSur;
IMAGE empty_pic, nempty_pic;
IMAGE newItem_pic, plate_pic;
IMAGE cat_pic, catCover_pic;
IMAGE bomb_pic, bombCover_pic;
IMAGE rainbow_pic, rainbowCover_pic;
IMAGE stone_pic, stoneCover_pic;
IMAGE leaf_pic, leafCover_pic;
IMAGE bush_pic, bushCover_pic;
IMAGE tree_pic, treeCover_pic;
IMAGE house_pic, houseCover_pic;
IMAGE camp_pic, campCover_pic;
IMAGE palace_1_pic, palace_1Cover_pic;
IMAGE palace_2_pic, palace_2Cover_pic;
IMAGE palace_3_pic, palace_3Cover_pic;
IMAGE grave_pic, graveCover_pic;

Map gameMap;
User user;
Item item;
Item clickItem;
int plate;
int probArray[100];
char s[MAX_CAT];
SqStack tmpCat;

MOUSEMSG m;

void loadImg();
void welcomeSurface();
void gameSurface();
void map();
void nextItem();
void showClick();
void join();
void catMove();
void putItem(int x, int y, Type type);
void toJoin();
void toCat();
void toPlate();
void stepDown();
void probability();
void move(Position catPos);
bool checkPosDirec(Position pos, int direc);
void toPos(Position pos_1, Position pos_2);

int main()
{	
	initgraph(400, 600);
	loadImg();
	welcomeSurface();//欢迎界面 
	_getch();
	closegraph();
}

void loadImg(){
	loadimage(&welcomeSur, "./pics/welcomeSur.jpg", 400, 600);
	loadimage(&gameSur, "./pics/gameSur.jpg", 400, 600);
	loadimage(&scoreBlack, "./pics/scoreBlack.jpg", 60, 20);
	loadimage(&nempty_pic, "./pics/nempty.jpg", 50, 50);
	loadimage(&empty_pic, "./pics/empty.jpg", 50, 50);
	loadimage(&newItem_pic, "./pics/newItem.jpg", 50, 50);
	loadimage(&plate_pic, "./pics/plate.jpg", 80, 50);
	loadimage(&leaf_pic, "./pics/leaf.jpg", 50, 50);
	loadimage(&leafCover_pic, "./pics/leaf_cover.jpg", 50, 50);
	loadimage(&bush_pic, "./pics/bush.jpg", 50, 50);
	loadimage(&bushCover_pic, "./pics/bush_cover.jpg", 50, 50);
	loadimage(&tree_pic, "./pics/tree.jpg", 50, 50);
	loadimage(&treeCover_pic, "./pics/tree_cover.jpg", 50, 50);
	loadimage(&house_pic, "./pics/house.jpg", 50, 50);
	loadimage(&houseCover_pic, "./pics/house_cover.jpg", 50, 50);
	loadimage(&camp_pic, "./pics/camp.jpg", 50, 50);
	loadimage(&campCover_pic, "./pics/camp_cover.jpg", 50, 50);
	loadimage(&palace_1_pic, "./pics/palace_1.jpg", 50, 50);
	loadimage(&palace_1Cover_pic, "./pics/palace_1_cover.jpg", 50, 50);
	loadimage(&palace_2_pic, "./pics/palace_2.jpg", 50, 50);
	loadimage(&palace_2Cover_pic, "./pics/palace_2_cover.jpg", 50, 50);
	loadimage(&palace_3_pic, "./pics/palace_3.jpg", 50, 50);
	loadimage(&palace_3Cover_pic, "./pics/palace_3_cover.jpg", 50, 50);
	loadimage(&cat_pic, "./pics/cat.jpg", 50, 50);
	loadimage(&catCover_pic, "./pics/cat_cover.jpg", 50, 50);
	loadimage(&bomb_pic, "./pics/bomb.jpg", 50, 50);
	loadimage(&bombCover_pic, "./pics/bomb_cover.jpg", 50, 50);
	loadimage(&rainbow_pic, "./pics/rainbow.jpg", 50, 50);
	loadimage(&rainbowCover_pic, "./pics/rainbow_cover.jpg", 50, 50);
	loadimage(&stone_pic, "./pics/stone.jpg", 50, 50);
	loadimage(&stoneCover_pic, "./pics/stone_cover.jpg", 50, 50);
	loadimage(&grave_pic, "./pics/grave.jpg", 50, 50);
	loadimage(&graveCover_pic, "./pics/grave_cover.jpg", 50, 50);
}

void welcomeSurface(){
	//欢迎显示初始化界面
	putimage(0, 0, &welcomeSur);
	MOUSEMSG m;
	while(true){
		m = GetMouseMsg();
		if(m.uMsg == WM_LBUTTONDOWN && m.x >= 158 && m.x <= 242 && m.y >= 317 && m.y <= 388){//点击开始按钮 
			gameSurface();//进入游戏界面 
		} 
	}
}

void gameSurface(){
	//显示初始化界面
	putimage(0, 0, &gameSur);
	map();//生成随机地图
	nextItem();//生成下一物品 
	while(true){
		m = GetMouseMsg();
		if(m.uMsg == WM_LBUTTONDOWN){
			//点击重新开始 
			if(257 <= m.x && m.x <= 383 && 117 <= m.y && m.y <= 149){
				gameSurface();
			}
			//点击返回键
			else if(16 <= m.x && m.x <= 240 && 117 <= m.y && m.y <= 149){
				//返回欢迎界面 
				welcomeSurface();
			}
			//点击地图位置 
			else if(12 <= m.x && m.x <= 384 && 184 <= m.y && m.y <= 530){
				clickItem.pos.x = m.x;
				clickItem.pos.y = m.y; 
				//根据点击位置生成clickItem,并在地图中标注 
				showClick();
				//根据点击的位置进行操作
				join(); 
			}
			//点击其他位置
			else{
				//无反应 
			}
		} 
	} 
}

void map(){
	Position randomPos;
	//生成比率数组 
	probability();
	//初始化地图 
	gameMap[0][0].pos.x = 18;
	gameMap[0][0].pos.y = 186;
	for(int i = 0; i < MAP_LENGTH; i++){
		for(int j = 0; j < MAP_LENGTH; j++){
			if(j > 0){
				gameMap[i][j].pos.x = gameMap[i][j - 1].pos.x + 62;
				gameMap[i][j].pos.y = gameMap[i][j - 1].pos.y;
			}
			if(i > 0) {
				gameMap[i][j].pos.x = gameMap[i - 1][j].pos.x;
				gameMap[i][j].pos.y = gameMap[i - 1][j].pos.y + 58;
			}
			gameMap[i][j].type = EMPTY;
			if(!(i == 0 && j == 0)) putimage(gameMap[i][j].pos.x, gameMap[i][j].pos.y, &nempty_pic);
		}
	}
	//初始化盘子 
	gameMap[0][0].type = PLATE;
	plate = EMPTY;
	putItem(0, 0, PLATE);
	/*///随机放置石头
	srand(time(NULL));
	for(i = 0; i < INIT_STONE; i++){
		//生成随机位置
		while(true){
			randomPos.x = rand() % MAP_LENGTH;
			randomPos.y = rand() % MAP_LENGTH;
			if(gameMap[randomPos.x][randomPos.y].type == EMPTY) break;
		}
		//在地图上放置石头
		putItem(randomPos.x, randomPos.y, STONE);
	}
	//随机放置猫
	for(i = 0; i < INIT_CAT; i++){
		//生成随机位置
		while(true){
			randomPos.x = rand() % MAP_LENGTH;
			randomPos.y = rand() % MAP_LENGTH;
			if(gameMap[randomPos.x][randomPos.y].type == EMPTY) break;
		}
		//在地图上放置猫 
		putItem(randomPos.x, randomPos.y, CAT);
	}
	//...随机叶子、灌木 */
	//for test
	putItem(5, 0, STONE);
	putItem(4, 1, STONE);
	putItem(4, 2, STONE);
	putItem(5, 1, CAT);
	putItem(5, 2, CAT);
	putItem(5, 3, CAT);
	//end test
	//设置初始化数据 
	user.score = 0;
	user.step = INIT_STEP;
	user.money = 0;
	//在地图上标注数据 
	putimage(18, 27, &scoreBlack);
	putimage(18, 68, &scoreBlack);
	putimage(288, 27, &scoreBlack);
	setbkmode(TRANSPARENT);
	outtextxy(25, 30, _T(itoa(user.score, s, 10)));
	outtextxy(25, 75, _T(itoa(user.step, s, 10)));
	outtextxy(289, 32, _T(itoa(user.money, s, 10)));
}

void nextItem(){
	/*item.type = probArray[rand() % 100];//生成随机物品，并记录进item 
	//在地图上显示 
	putimage(174, 35, &newItem_pic);
	switch(item.type){
		case LEAF:{
			putimage(174, 35, &leafCover_pic, SRCAND);
			putimage(174, 35, &leaf_pic, SRCPAINT);
			break;
		}
		case BUSH:{
			putimage(174, 35, &bushCover_pic, SRCAND);
			putimage(174, 35, &bush_pic, SRCPAINT);
			break;
		}
		case TREE:{
			putimage(174, 35, &treeCover_pic, SRCAND);
			putimage(174, 35, &tree_pic, SRCPAINT);
			break;
		}
		case HOUSE:{
			putimage(174, 35, &houseCover_pic, SRCAND);
			putimage(174, 35, &house_pic, SRCPAINT);
			break;
		}
		case CAT:{
			putimage(174, 35, &catCover_pic, SRCAND);
			putimage(174, 35, &cat_pic, SRCPAINT);
			break;
		}
		case BOMB:{
			putimage(174, 35, &bombCover_pic, SRCAND);
			putimage(174, 35, &bomb_pic, SRCPAINT);
			break;
		}
		case RAINBOW:{
			putimage(174, 35, &rainbowCover_pic, SRCAND);
			putimage(174, 35, &rainbow_pic, SRCPAINT);
			break;
		}
		default:;//item类型错误 
	}*/
	//for test
	item.type = LEAF;
	putimage(174, 35, &newItem_pic);
	putimage(174, 35, &leafCover_pic, SRCAND);
	putimage(174, 35, &leaf_pic, SRCPAINT);
	//end test
}

void probability(){
	for(int i = 0; i < 74; i++) probArray[i] = LEAF;
	for(int j = i; j < i + 15; j++) probArray[j] = BUSH;
	for(i = j; i < j + 5; i++) probArray[i] = TREE;
	for(j = i; j < i + 2; j++) probArray[j] = HOUSE;
	for(i = j; i < j + 2; i++) probArray[i] = CAT;
	for(j = i; j < i + 1; j++) probArray[j] = BOMB;
	for(i = j; i < j + 1; i++) probArray[i] = RAINBOW;
}

void showClick(){ 
	int tmp = (clickItem.pos.x - 12) / 62;
	//clickItem.pos记录对应map的坐标
	clickItem.pos.x = (clickItem.pos.y - 182) / 58;
	clickItem.pos.y = tmp;
	//从gameMap中找到对应的方格，将类型赋给clickItem
	clickItem.type = gameMap[clickItem.pos.x][clickItem.pos.y].type; //注意对应的i，j 
	//...在地图中标注点击位置 
}

void join(){
	//若下一物品为普通物品 
	if(LEAF <= item.type && item.type <= PALACE_3){
		//若放置位置为空 
		if(clickItem.type == EMPTY){
			putItem(clickItem.pos.x, clickItem.pos.y, item.type);//放下物品 
			toCat();//...将猫变为墓碑
			toJoin();//...合体 
			stepDown();//步数减1 
			nextItem();//生成下一个物品
			catMove();//猫咪移动   
		}
		//若放置位置为盘子
		else if(clickItem.type == PLATE){
			toPlate();//判断盘子信息 
		}
		//若放置位置为其他物品 
		else if(CAT <= clickItem.type && clickItem.type <= PALACE_3){
			//禁止放置 
		}
		else{
			//...clickItem位置类型错误 
		}
	}
	//若下一物品为猫 
	else if(item.type == CAT){
		//若放置位置为空 
		if(clickItem.type == EMPTY){
			//...放置猫 (判断放下后是否被困住)
			user.step--;//步数减1 
			nextItem();//生成下一个物品 
			catMove();//猫咪移动  
		}
		//若放置位置为盘子
		else if(clickItem.type == PLATE){
			toPlate();//判断盘子信息 
		}
		//若放置位置为其他物品 
		else if(CAT <= clickItem.type && clickItem.type <= PALACE_3){
			//禁止放置 
		}
		else{
			//...clickItem位置类型错误 
		}
	}
	//若下一物品为炸弹 
	else if(item.type == BOMB){
		//若放置位置为空 
		if(clickItem.type == EMPTY){
			//禁止放置 
		}
		//若放置位置为盘子
		else if(clickItem.type == PLATE){
			toPlate();//判断盘子信息 
		}
		//若放置位置为猫
		else if(clickItem.type == CAT){
			//...猫变为墓碑 
			user.step--;//步数减1 
			nextItem();//生成下一个物品 
			catMove();//猫咪移动 
		}
		//若放置位置为一般物品
		else if(LEAF <= clickItem.type && clickItem.type <= PALACE_3){ 
			//...清除一般物品
			catMove();//猫咪移动 
		}
		else{
			//...clickItem位置类型错误 
		}
	}
	//若下一物品为彩虹 
	else if(item.type == RAINBOW){
		//若放置位置为空 
		if(clickItem.type == EMPTY){
			toCat();//...将猫变为墓碑 
			toJoin();//...合体 （彩虹能合体） 
			user.step--;//步数减1 
			nextItem();//生成下一个物品 
			catMove();//猫咪移动 
		}
		//若放置位置为盘子
		else if(clickItem.type == PLATE){
			toPlate();//...判断盘子信息 
		}
		//若放置位置为其他物品 
		else if(CAT <= clickItem.type && clickItem.type <= PALACE_3){
			//禁止放置 
		}
		else{
			//...clickItem位置类型错误 
		}
	}
	else{
		//...item类型错误 
	}
}

void putItem(int x, int y, Type type){
	if(!(x == 0 && y == 0) && x < 6 && y < 6){
		gameMap[x][y].type = type;
		putimage(gameMap[x][y].pos.x, gameMap[x][y].pos.y, &empty_pic);
	}
	switch(type){
		case EMPTY:{
			putimage(gameMap[x][y].pos.x, gameMap[x][y].pos.y, &nempty_pic);
			break;
		}
		case PLATE:{
			putimage(0, gameMap[0][0].pos.y, &plate_pic);
			break;
		}
		case LEAF:{
			if(x < 6 && y < 6){
				putimage(gameMap[x][y].pos.x, gameMap[x][y].pos.y, &leafCover_pic, SRCAND);
				putimage(gameMap[x][y].pos.x, gameMap[x][y].pos.y, &leaf_pic, SRCPAINT);
			}
			else{
				putimage(x, y, &leafCover_pic, SRCAND);
				putimage(x, y, &leaf_pic, SRCPAINT);
			}
			break;
		}
		case BUSH:{
			if(x < 6 && y < 6){
				putimage(gameMap[x][y].pos.x, gameMap[x][y].pos.y, &bushCover_pic, SRCAND);
				putimage(gameMap[x][y].pos.x, gameMap[x][y].pos.y, &bush_pic, SRCPAINT);
			}
			else{
				putimage(x, y, &bushCover_pic, SRCAND);
				putimage(x, y, &bush_pic, SRCPAINT);
			}
			break;
		}
		case TREE:{
			if(x < 6 && y < 6){
				putimage(gameMap[x][y].pos.x, gameMap[x][y].pos.y, &treeCover_pic, SRCAND);
				putimage(gameMap[x][y].pos.x, gameMap[x][y].pos.y, &tree_pic, SRCPAINT);
			}
			else{
				putimage(x, y, &treeCover_pic, SRCAND);
				putimage(x, y, &tree_pic, SRCPAINT);
			}
			break;
		}
		case HOUSE:{
			if(x < 6 && y < 6){
				putimage(gameMap[x][y].pos.x, gameMap[x][y].pos.y, &houseCover_pic, SRCAND);
				putimage(gameMap[x][y].pos.x, gameMap[x][y].pos.y, &house_pic, SRCPAINT);
			}
			else{
				putimage(x, y, &houseCover_pic, SRCAND);
				putimage(x, y, &house_pic, SRCPAINT);
			}
			break;
		}
		case CAMP:{
			if(x < 6 && y < 6){
				putimage(gameMap[x][y].pos.x, gameMap[x][y].pos.y, &campCover_pic, SRCAND);
				putimage(gameMap[x][y].pos.x, gameMap[x][y].pos.y, &camp_pic, SRCPAINT);
			}
			else{
				putimage(x, y, &campCover_pic, SRCAND);
				putimage(x, y, &camp_pic, SRCPAINT);
			}
			break;
		}
		case PALACE_1:{
			if(x < 6 && y < 6){
				putimage(gameMap[x][y].pos.x, gameMap[x][y].pos.y, &palace_1Cover_pic, SRCAND);
				putimage(gameMap[x][y].pos.x, gameMap[x][y].pos.y, &palace_1_pic, SRCPAINT);
			}
			else{
				putimage(x, y, &palace_1Cover_pic, SRCAND);
				putimage(x, y, &palace_1_pic, SRCPAINT);
			}
			break;
		}
		case PALACE_2:{
			if(x < 6 && y < 6){
				putimage(gameMap[x][y].pos.x, gameMap[x][y].pos.y, &palace_2Cover_pic, SRCAND);
				putimage(gameMap[x][y].pos.x, gameMap[x][y].pos.y, &palace_2_pic, SRCPAINT);
			}
			else{
				putimage(x, y, &palace_2Cover_pic, SRCAND);
				putimage(x, y, &palace_2_pic, SRCPAINT);
			}
			break;
		}
		case PALACE_3:{
			if(x < 6 && y < 6){
				putimage(gameMap[x][y].pos.x, gameMap[x][y].pos.y, &palace_3Cover_pic, SRCAND);
				putimage(gameMap[x][y].pos.x, gameMap[x][y].pos.y, &palace_3_pic, SRCPAINT);
			}
			else{
				putimage(x, y, &palace_3Cover_pic, SRCAND);
				putimage(x, y, &palace_3_pic, SRCPAINT);
			}
			break;
		}
		case CAT:{
			if(x < 6 && y < 6){
				putimage(gameMap[x][y].pos.x, gameMap[x][y].pos.y, &catCover_pic, SRCAND);
				putimage(gameMap[x][y].pos.x, gameMap[x][y].pos.y, &cat_pic, SRCPAINT);
			}
			else{
				putimage(x, y, &catCover_pic, SRCAND);
				putimage(x, y, &cat_pic, SRCPAINT);
			}
			break;
		}
		case BOMB:{
			if(x < 6 && y < 6){
				putimage(gameMap[x][y].pos.x, gameMap[x][y].pos.y, &bombCover_pic, SRCAND);
				putimage(gameMap[x][y].pos.x, gameMap[x][y].pos.y, &bomb_pic, SRCPAINT);
			}
			else{
				putimage(x, y, &bombCover_pic, SRCAND);
				putimage(x, y, &bomb_pic, SRCPAINT);
			}
			break;
		}
		case RAINBOW:{
			if(x < 6 && y < 6){
				putimage(gameMap[x][y].pos.x, gameMap[x][y].pos.y, &rainbowCover_pic, SRCAND);
				putimage(gameMap[x][y].pos.x, gameMap[x][y].pos.y, &rainbow_pic, SRCPAINT);
			}
			else{
				putimage(x, y, &rainbowCover_pic, SRCAND);
				putimage(x, y, &rainbow_pic, SRCPAINT);
			}
			break;
		}
		case STONE:{
			if(x < 6 && y < 6){
				putimage(gameMap[x][y].pos.x, gameMap[x][y].pos.y, &stoneCover_pic, SRCAND);
				putimage(gameMap[x][y].pos.x, gameMap[x][y].pos.y, &stone_pic, SRCPAINT);
			}
			else{
				putimage(x, y, &stoneCover_pic, SRCAND);
				putimage(x, y, &stone_pic, SRCPAINT);
			}
			break;
		}
		case GRAVE:{
			if(x < 6 && y < 6){
				putimage(gameMap[x][y].pos.x, gameMap[x][y].pos.y, &graveCover_pic, SRCAND);
				putimage(gameMap[x][y].pos.x, gameMap[x][y].pos.y, &grave_pic, SRCPAINT);
			}
			else{
				putimage(x, y, &graveCover_pic, SRCAND);
				putimage(x, y, &grave_pic, SRCPAINT);
			}
			break;
		}
		default:;//item类型错误 
	}
}

void toCat(){
	int i, j;
	Position catPos;
	int graveFlag, tmpFlag;
	//检测周围是否有猫
	for(i = 0; i < 4; i++){
		//若有猫则判断是否被全包围 
		if(checkPosDirec(clickItem.pos, i)){
			if(gameMap[clickItem.pos.x + (i - 1) % 2][clickItem.pos.y + (i - 2) % 2].type == CAT){
				graveFlag = 1;
				tmpFlag = 0;
				catPos.x = clickItem.pos.x + (i - 1) % 2;
				catPos.y = clickItem.pos.y + (i - 2) % 2;
				for(j = 0; j < 4; j++){
					if(checkPosDirec(catPos, j)){
						if(gameMap[catPos.x + (j - 1) % 2][catPos.y + (j - 2) % 2].type == EMPTY){
				   			graveFlag = 0;
						}
						else if(gameMap[catPos.x + (j - 1) % 2][catPos.y + (j - 2) % 2].type == CAT){
							tmpFlag = 1;
						}
					}
				}
				//若全包围则将猫变为墓碑
				if(graveFlag == 1 && tmpFlag == 0){
					//改变地图标志 
					putItem(catPos.x, catPos.y, GRAVE);
				}
				//若四周除了猫咪以外，其他方向均包围 ，则加入暂存数组 
				else if(graveFlag == 1 && tmpFlag == 1){
					Push(tmpCat, catPos);
				}
			}
		}
	} 
}

bool checkPosDirec(Position pos, int direc){
	if(0 <= pos.x + (direc - 1) % 2 && pos.x + (direc - 1) % 2 < MAP_LENGTH
	&& 0 <= pos.y + (direc - 2) % 2 && pos.y + (direc - 2) % 2 < MAP_LENGTH){
		return true;
	}
	return false;
}

void toPos(Position pos_1, Position pos_2){
	pos_1.x = pos_2.x;
	pos_1.y = pos_2.y;
}

void stepDown(){
	user.step--;
	putimage(18, 68, &scoreBlack);
	setbkmode(TRANSPARENT);
	outtextxy(25, 75, _T(itoa(user.step, s, 10)));
}

void toJoin(){
	//从clickItem开始在gameMap中寻找相邻的值,放在栈中 
	//将栈中对应坐标的值都变为空地
	//标注地图 
	//将点击的位置坐标升级 
	if(gameMap[clickItem.pos.x][clickItem.pos.y].type != PALACE_3){
		gameMap[clickItem.pos.x][clickItem.pos.y].type++;
		//标注地图 
	}
	//...加分 
}

void toPlate(){
	//若盘子为空
	if(plate == EMPTY){
		plate = item.type;
		putItem(0, 0, plate);
		nextItem();
	}
	//若盘子不为空 
	else{
		item.type = plate;
		putimage(174, 35, &newItem_pic);
		putItem(174, 35, plate);
		putItem(0, 0, PLATE);
		plate = EMPTY;
	}
}

void catMove(){
	//遍历gameMap，找到猫，并移动
	Position catPos[MAX_CAT];
	int k = 0;
	for(int i = 0; i < MAP_LENGTH; i++){
		for(int j = 0; j < MAP_LENGTH; j++){
			if(gameMap[i][j].type == CAT){
				catPos[k].x = i;
				catPos[k].y = j;
				k++;
			}
		}
	}
	for(i = 0; i < k; i++){
		move(catPos[i]);
	}
	for(i = 0; i < k; i++){
		putItem(catPos[i].x, catPos[i].y, EMPTY);
	}
}

//猫的转向不予考虑 
void move(Position catPos){
	Position changePos;
	int direction = rand() % 4;
	int posFlag = 0;
	int i;
	for(i = 0; i < 4; i++){
		if(checkPosDirec(catPos, direction)){
			if(gameMap[catPos.x + (direction - 1) % 2][catPos.y + (direction - 2) % 2].type == EMPTY){
				 posFlag = 1;
			}
		}
		if(posFlag == 1) break;
		direction = (direction + 1) % 4;
	}
	if(i >= 4){//若四个方向均不能移动 
		//改变地图标志 
		putItem(catPos.x, catPos.y, GRAVE);
	}
	else{
		changePos.x = catPos.x + (direction - 1) % 2;
		changePos.y = catPos.y + (direction - 2) % 2;
		//改变地图标志 
		putItem(changePos.x, changePos.y, CAT);
	}
}

