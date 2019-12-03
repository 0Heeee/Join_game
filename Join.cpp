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

#define newCAT 15

#define left 0
#define up 1
#define right 2
#define down 3

typedef int Type;
//λ�ýṹ�� 
typedef struct{
	int x;
	int y;
}Position;
//��Ʒ�ṹ�� 
typedef struct{
	Type type;
	Position pos;
}Item, Map[MAP_LENGTH][MAP_LENGTH];
//��ҽṹ�� 
typedef struct{
	int score;
	int step;
	int money;
}User;
typedef Position SElemType;
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
SqStack tmpCatS, catS, newCatS;

MOUSEMSG m;

void loadImg();
void welcomeSurface();
void gameSurface();
void map();
void nextItem();
void showClick();
void join();
void catMove();
void uniteSame();
void putItem(int x, int y, Type type);
void toPlate();
void stepDown();
void probability();
bool checkPosDirec(int x, int y, int direc);
void toPos(Position pos_1, Position pos_2);
void delTmpCat(Position pos);

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
	putItem(1, 0, STONE);
	putItem(1, 1, STONE);
	putItem(1, 2, STONE);
	putItem(1, 3, STONE);
	putItem(2, 0, STONE);
	putItem(2, 3, STONE);
	putItem(3, 0, STONE);
	putItem(3, 3, STONE);
	putItem(4, 0, STONE);
	putItem(4, 3, STONE);
	putItem(5, 0, STONE);
	putItem(5, 1, STONE);
	putItem(5, 2, STONE);
	putItem(2, 1, CAT);
	putItem(3, 1, CAT);
	putItem(4, 2, CAT);
	//end test
	//���ó�ʼ������ 
	user.score = 0;
	user.step = INIT_STEP;
	user.money = 0;
	putimage(18, 27, &scoreBlack);
	putimage(18, 68, &scoreBlack);
	putimage(288, 27, &scoreBlack);
	setbkmode(TRANSPARENT);
	outtextxy(25, 30, _T(itoa(user.score, s, 10)));
	outtextxy(25, 75, _T(itoa(user.step, s, 10)));
	outtextxy(289, 32, _T(itoa(user.money, s, 10)));
}

void nextItem(){
	//���������Ʒ������¼��item 
	/*item.type = probArray[rand() % 100];
	//�ڵ�ͼ����ʾ 
	putItem(174, 35, item.type);*/
	//for test
	item.type = CAT;
	putimage(174, 35, &newItem_pic);
	putimage(174, 35, &catCover_pic, SRCAND);
	putimage(174, 35, &cat_pic, SRCPAINT);
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
			catMove();//�ƶ�è
			uniteSame();//�ϲ���ͬ��Ʒ 
			stepDown();//������1 
			nextItem();//������һ����Ʒ  
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
			putItem(clickItem.pos.x, clickItem.pos.y, item.type);//������Ʒ 
			catMove();//�ƶ�è
			stepDown();//������1 
			nextItem();//������һ����Ʒ
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
	/*else if(item.type == BOMB){
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
	}*/
}

void catMove(){
	int graveFlag, tmpFlag;
	Position tmpPos, changePos;
	//��ʼ���洢ջ 
	InitStack(tmpCatS);
	InitStack(catS);
	InitStack(newCatS);
	//������ͼ 
	for(int i = 0; i < MAP_LENGTH; i++){
		for(int j = 0; j < MAP_LENGTH; j++){
			//������è 
			if(gameMap[i][j].type == CAT){
				//���è����Χ�Ƿ�ȫ��Χ 
				graveFlag = 1;
				tmpFlag = 0;
				tmpPos.x = i;
				tmpPos.y = j;
				for(int k = 0; k < 4; k++){
					if(checkPosDirec(i, j, k)){
						if(gameMap[i + (k - 1) % 2][j + (k - 2) % 2].type == EMPTY){
				   			graveFlag = 0;
				   			//��¼Ϊ��ʱ�ķ��� 
				   			changePos.x = i + (k - 1) % 2;
				   			changePos.y = j + (k - 2) % 2;
						}
						else if(gameMap[i + (k - 1) % 2][j + (k - 2) % 2].type == CAT
						|| gameMap[i + (k - 1) % 2][j + (k - 2) % 2].type == newCAT){
							tmpFlag = 1;
						}
					}
				}
				//��ȫ��Χ
				if(graveFlag == 1 && tmpFlag == 0){
					//��è��ΪĹ��
					putItem(i, j, GRAVE);
				}
				//�����ܳ���è�����⣬�����������Χ
				else if(graveFlag == 1 && tmpFlag == 1){
					//�����ݴ�ջ
					Push(tmpCatS, tmpPos);
				}
				//��è���ƶ� 
				else{
					if(!(i == clickItem.pos.x && j == clickItem.pos.y)){
						//��¼���ƶ�è  
						Push(catS, tmpPos);
						//�ƶ� 
						putItem(changePos.x, changePos.y, CAT);
						putItem(i, j, EMPTY);
						//��ע���ƶ�����λ�� 
						gameMap[changePos.x][changePos.y].type = newCAT; 
						Push(newCatS, changePos);
					}
				}
			}
		}
	}
	//������ߵ�è��ԭλ�������Ƿ���è
	while(!StackEmpty(catS)){
		Pop(catS, tmpPos);
		if(gameMap[tmpPos.x][tmpPos.y].type != newCAT){
			for(int k = 0; k < 4; k++){
				//�����ߵ�è��ԭλ��������è
				changePos.x = tmpPos.x + (k - 1) % 2;
				changePos.y = tmpPos.y + (k - 2) % 2;
				if(checkPosDirec(tmpPos.x, tmpPos.y, k)){
					if(gameMap[changePos.x][changePos.y].type == CAT && !(changePos.x == clickItem.pos.x && changePos.y == clickItem.pos.y)){
						//��¼���ƶ�è  
						Push(catS, changePos);
						//��è�ƶ������ߵ�è��ԭλ�� 
						putItem(changePos.x, changePos.y, EMPTY);
						putItem(tmpPos.x, tmpPos.y, CAT);
						//��ע���ƶ�����λ�� 
						gameMap[tmpPos.x][tmpPos.y].type = newCAT;
						Push(newCatS, tmpPos);
						//���ݴ�ջ��ɾ����è 
						delTmpCat(changePos);
					}
				}
			}
		}
	}
	//���ݴ�è���Ĺ 
	while(!StackEmpty(tmpCatS)){
		Pop(tmpCatS, tmpPos);
		graveFlag = 1;
		for(int k = 0; k < 4; k++){
			if(checkPosDirec(tmpPos.x, tmpPos.y, k)){
				if(gameMap[tmpPos.x + (k - 1) % 2][tmpPos.y + (k - 2) % 2].type == newCAT){ 
					graveFlag = 0;
				}
			}
		}
		//���ݴ�è����û�����ƶ�����è 
		if(graveFlag == 1){
			//��è���Ĺ
			putItem(tmpPos.x, tmpPos.y, GRAVE);
		}
		else{
			putItem(tmpPos.x, tmpPos.y, CAT);
		}
	} 
	//����ע����è��־���è
	while(!StackEmpty(newCatS)){
		Pop(newCatS, tmpPos);
		gameMap[tmpPos.x][tmpPos.y].type = CAT;
	} 
}

void delTmpCat(Position pos){
	SqStack tmpS;
	Position tmpPos;
	InitStack(tmpS);
	while(!StackEmpty(tmpCatS)){
		Pop(tmpCatS, tmpPos);
		if(!(tmpPos.x == pos.x && tmpPos.y == pos.y)) Push(tmpS, tmpPos);
	}
	while(!StackEmpty(tmpS)){
		Pop(tmpS, tmpPos);
		Push(tmpCatS, tmpPos);
	}
}

void uniteSame(){
	
}

void putItem(int x, int y, Type type){
	if(!(x == 0 && y == 0) && x < 6 && y < 6){
		gameMap[x][y].type = type;
		putimage(gameMap[x][y].pos.x, gameMap[x][y].pos.y, &empty_pic);
	}
	if(x == 174 && y == 35){
		putimage(x, y, &newItem_pic);
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

bool checkPosDirec(int x, int y, int direc){
	if(0 <= x + (direc - 1) % 2 && x + (direc - 1) % 2 < MAP_LENGTH
	&& 0 <= y + (direc - 2) % 2 && y + (direc - 2) % 2 < MAP_LENGTH){
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


