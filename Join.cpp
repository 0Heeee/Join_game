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
typedef struct{//λ�ýṹ�� 
	int x;
	int y;
}Position;
typedef struct{//��Ʒ�ṹ�� 
	Type type;
	Position pos;
}Item, Map[MAP_LENGTH][MAP_LENGTH];
typedef struct{//��ҽṹ�� 
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
	welcomeSurface();//��ӭ���� 
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
	//��ӭ��ʾ��ʼ������
	putimage(0, 0, &welcomeSur);
	MOUSEMSG m;
	while(true){
		m = GetMouseMsg();
		if(m.uMsg == WM_LBUTTONDOWN && m.x >= 158 && m.x <= 242 && m.y >= 317 && m.y <= 388){//�����ʼ��ť 
			gameSurface();//������Ϸ���� 
		} 
	}
}

void gameSurface(){
	//��ʾ��ʼ������
	putimage(0, 0, &gameSur);
	map();//���������ͼ
	nextItem();//������һ��Ʒ 
	while(true){
		m = GetMouseMsg();
		if(m.uMsg == WM_LBUTTONDOWN){
			//������¿�ʼ 
			if(257 <= m.x && m.x <= 383 && 117 <= m.y && m.y <= 149){
				gameSurface();
			}
			//������ؼ�
			else if(16 <= m.x && m.x <= 240 && 117 <= m.y && m.y <= 149){
				//���ػ�ӭ���� 
				welcomeSurface();
			}
			//�����ͼλ�� 
			else if(12 <= m.x && m.x <= 384 && 184 <= m.y && m.y <= 530){
				clickItem.pos.x = m.x;
				clickItem.pos.y = m.y; 
				//���ݵ��λ������clickItem,���ڵ�ͼ�б�ע 
				showClick();
				//���ݵ����λ�ý��в���
				join(); 
			}
			//�������λ��
			else{
				//�޷�Ӧ 
			}
		} 
	} 
}

void map(){
	Position randomPos;
	//���ɱ������� 
	probability();
	//��ʼ����ͼ 
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
	//��ʼ������ 
	gameMap[0][0].type = PLATE;
	plate = EMPTY;
	putItem(0, 0, PLATE);
	/*///�������ʯͷ
	srand(time(NULL));
	for(i = 0; i < INIT_STONE; i++){
		//�������λ��
		while(true){
			randomPos.x = rand() % MAP_LENGTH;
			randomPos.y = rand() % MAP_LENGTH;
			if(gameMap[randomPos.x][randomPos.y].type == EMPTY) break;
		}
		//�ڵ�ͼ�Ϸ���ʯͷ
		putItem(randomPos.x, randomPos.y, STONE);
	}
	//�������è
	for(i = 0; i < INIT_CAT; i++){
		//�������λ��
		while(true){
			randomPos.x = rand() % MAP_LENGTH;
			randomPos.y = rand() % MAP_LENGTH;
			if(gameMap[randomPos.x][randomPos.y].type == EMPTY) break;
		}
		//�ڵ�ͼ�Ϸ���è 
		putItem(randomPos.x, randomPos.y, CAT);
	}
	//...���Ҷ�ӡ���ľ */
	//for test
	putItem(5, 0, STONE);
	putItem(4, 1, STONE);
	putItem(4, 2, STONE);
	putItem(5, 1, CAT);
	putItem(5, 2, CAT);
	putItem(5, 3, CAT);
	//end test
	//���ó�ʼ������ 
	user.score = 0;
	user.step = INIT_STEP;
	user.money = 0;
	//�ڵ�ͼ�ϱ�ע���� 
	putimage(18, 27, &scoreBlack);
	putimage(18, 68, &scoreBlack);
	putimage(288, 27, &scoreBlack);
	setbkmode(TRANSPARENT);
	outtextxy(25, 30, _T(itoa(user.score, s, 10)));
	outtextxy(25, 75, _T(itoa(user.step, s, 10)));
	outtextxy(289, 32, _T(itoa(user.money, s, 10)));
}

void nextItem(){
	/*item.type = probArray[rand() % 100];//���������Ʒ������¼��item 
	//�ڵ�ͼ����ʾ 
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
		default:;//item���ʹ��� 
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
	//clickItem.pos��¼��Ӧmap������
	clickItem.pos.x = (clickItem.pos.y - 182) / 58;
	clickItem.pos.y = tmp;
	//��gameMap���ҵ���Ӧ�ķ��񣬽����͸���clickItem
	clickItem.type = gameMap[clickItem.pos.x][clickItem.pos.y].type; //ע���Ӧ��i��j 
	//...�ڵ�ͼ�б�ע���λ�� 
}

void join(){
	//����һ��ƷΪ��ͨ��Ʒ 
	if(LEAF <= item.type && item.type <= PALACE_3){
		//������λ��Ϊ�� 
		if(clickItem.type == EMPTY){
			putItem(clickItem.pos.x, clickItem.pos.y, item.type);//������Ʒ 
			toCat();//...��è��ΪĹ��
			toJoin();//...���� 
			stepDown();//������1 
			nextItem();//������һ����Ʒ
			catMove();//è���ƶ�   
		}
		//������λ��Ϊ����
		else if(clickItem.type == PLATE){
			toPlate();//�ж�������Ϣ 
		}
		//������λ��Ϊ������Ʒ 
		else if(CAT <= clickItem.type && clickItem.type <= PALACE_3){
			//��ֹ���� 
		}
		else{
			//...clickItemλ�����ʹ��� 
		}
	}
	//����һ��ƷΪè 
	else if(item.type == CAT){
		//������λ��Ϊ�� 
		if(clickItem.type == EMPTY){
			//...����è (�жϷ��º��Ƿ���ס)
			user.step--;//������1 
			nextItem();//������һ����Ʒ 
			catMove();//è���ƶ�  
		}
		//������λ��Ϊ����
		else if(clickItem.type == PLATE){
			toPlate();//�ж�������Ϣ 
		}
		//������λ��Ϊ������Ʒ 
		else if(CAT <= clickItem.type && clickItem.type <= PALACE_3){
			//��ֹ���� 
		}
		else{
			//...clickItemλ�����ʹ��� 
		}
	}
	//����һ��ƷΪը�� 
	else if(item.type == BOMB){
		//������λ��Ϊ�� 
		if(clickItem.type == EMPTY){
			//��ֹ���� 
		}
		//������λ��Ϊ����
		else if(clickItem.type == PLATE){
			toPlate();//�ж�������Ϣ 
		}
		//������λ��Ϊè
		else if(clickItem.type == CAT){
			//...è��ΪĹ�� 
			user.step--;//������1 
			nextItem();//������һ����Ʒ 
			catMove();//è���ƶ� 
		}
		//������λ��Ϊһ����Ʒ
		else if(LEAF <= clickItem.type && clickItem.type <= PALACE_3){ 
			//...���һ����Ʒ
			catMove();//è���ƶ� 
		}
		else{
			//...clickItemλ�����ʹ��� 
		}
	}
	//����һ��ƷΪ�ʺ� 
	else if(item.type == RAINBOW){
		//������λ��Ϊ�� 
		if(clickItem.type == EMPTY){
			toCat();//...��è��ΪĹ�� 
			toJoin();//...���� ���ʺ��ܺ��壩 
			user.step--;//������1 
			nextItem();//������һ����Ʒ 
			catMove();//è���ƶ� 
		}
		//������λ��Ϊ����
		else if(clickItem.type == PLATE){
			toPlate();//...�ж�������Ϣ 
		}
		//������λ��Ϊ������Ʒ 
		else if(CAT <= clickItem.type && clickItem.type <= PALACE_3){
			//��ֹ���� 
		}
		else{
			//...clickItemλ�����ʹ��� 
		}
	}
	else{
		//...item���ʹ��� 
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
		default:;//item���ʹ��� 
	}
}

void toCat(){
	int i, j;
	Position catPos;
	int graveFlag, tmpFlag;
	//�����Χ�Ƿ���è
	for(i = 0; i < 4; i++){
		//����è���ж��Ƿ�ȫ��Χ 
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
				//��ȫ��Χ��è��ΪĹ��
				if(graveFlag == 1 && tmpFlag == 0){
					//�ı��ͼ��־ 
					putItem(catPos.x, catPos.y, GRAVE);
				}
				//�����ܳ���è�����⣬�����������Χ ��������ݴ����� 
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
	//��clickItem��ʼ��gameMap��Ѱ�����ڵ�ֵ,����ջ�� 
	//��ջ�ж�Ӧ�����ֵ����Ϊ�յ�
	//��ע��ͼ 
	//�������λ���������� 
	if(gameMap[clickItem.pos.x][clickItem.pos.y].type != PALACE_3){
		gameMap[clickItem.pos.x][clickItem.pos.y].type++;
		//��ע��ͼ 
	}
	//...�ӷ� 
}

void toPlate(){
	//������Ϊ��
	if(plate == EMPTY){
		plate = item.type;
		putItem(0, 0, plate);
		nextItem();
	}
	//�����Ӳ�Ϊ�� 
	else{
		item.type = plate;
		putimage(174, 35, &newItem_pic);
		putItem(174, 35, plate);
		putItem(0, 0, PLATE);
		plate = EMPTY;
	}
}

void catMove(){
	//����gameMap���ҵ�è�����ƶ�
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

//è��ת���迼�� 
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
	if(i >= 4){//���ĸ�����������ƶ� 
		//�ı��ͼ��־ 
		putItem(catPos.x, catPos.y, GRAVE);
	}
	else{
		changePos.x = catPos.x + (direction - 1) % 2;
		changePos.y = catPos.y + (direction - 2) % 2;
		//�ı��ͼ��־ 
		putItem(changePos.x, changePos.y, CAT);
	}
}

