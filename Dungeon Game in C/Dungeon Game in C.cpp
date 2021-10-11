#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<time.h>
#include<string.h>
#include<conio.h>
typedef struct map{
	int height,width;
	char format[100][100];
}map;
typedef struct monster{
	int x, y;
	char symbol;
}monster;
typedef struct player{
	int x,y;
	char symbol;
}player;
typedef struct trap{
	int x,y;
}trap;
FILE *mapptr;
bool running ;
bool isWin;
bool checkCollide(int,int,map);
void homePage();
void startGame();
void printMap(map);
void generateEntities(map&,player&,monster[],trap[]);
void cls();
void moveLeft(int&,int&,map&,char);
void moveDown(int&,int&,map&,char);
void moveUp(int&,int&,map&,char);
void moveRight(int&,int&,map&,char);
bool checkTrap(trap[],int);
void rules();
void movePlayer(player &,trap[],char,map&,monster[]);
void moveMonster(player,monster[],trap[],map&);
void generateMap(map&);
int main(){
	int choice;
	srand(time(NULL));
	while(true){
		do{
			cls();
			homePage();
			scanf("%d",&choice);getchar();
		}while(choice < 1 || choice > 3);
			
		switch(choice){
			case 1:
				startGame();
				break;
			case 2:
				rules();
				break;
			case 3:
				return 0;
		}	
	}
}
void rules(){
	system("cls");
	printf("How to play dungeon arcade one-O-one\n");
	printf("====================================\n");
	printf("1. Player have to reach the end point ('*') of the maze to win the game\n");
	printf("2. Player can use W(up), A(left), S(down), D(right) to move.\n");
	printf("3. Beware of hidden traps around the dungeon!\n4. Player can reveal hidden traps if they are within 2 tiles from the traps.\n5. Player will be defeated if they either hit the dungeon's monster ('O') or the dungeon's traps ('X')\n6. Monsters will keep moving while you stop. So keep moving and don't get caught!\nPress enter to continue..");
	getchar();
}
void startGame(){
	cls();
	map map;
	player player;
	monster monsters[5];
	trap traps[5];
	generateMap(map);
	generateEntities(map,player,monsters,traps);
	running = true;
	isWin = false;
	while(running){
		cls();
		printMap(map);
		if(kbhit()){
			char temp = getch();
			movePlayer(player,traps,temp,map,monsters);
		}
		moveMonster(player,monsters,traps,map);
	}
	if(isWin){
		printf("Congratulation, you have reached the end point safely!\n\n");
	}
	else{
		printf("You died!\n\n");	
	}
	printf("Returning to main menu\n");
	printf("Press enter to continue...");
	getchar();
}
void moveMonster(player player,monster monster[],trap trap[],map &map){
	for(int i = 0; i < 2; i ++){
		int a = rand() % (RAND_MAX);
		int c = a % 5000;
		if(c == 0){ // move
			int move;
			int x,y;
			bool moveable = true;
			move = rand() % 3;
			if(move == 0){
				if(map.format[monster[i].y][monster[i].x + 1] == '*' || map.format[monster[i].y][monster[i].x + 1] == 'O'){
					continue;
				}
				else{
					for(int j = 0 ; j < 2 ; j ++){
						if(trap[j].x == monster[i].x+1 && trap[j].y == monster[i].y)
						{
							moveable = false;
						}
					}
					if(moveable)
						moveRight(monster[i].x,monster[i].y,map,monster[i].symbol);
				}
			}
			else if(move == 1){
				if(map.format[monster[i].y][monster[i].x - 1] == '*' || map.format[monster[i].y][monster[i].x - 1] == 'O'){
					continue;
				}
				else{
					for(int j = 0 ; j < 2 ; j ++){
						if(trap[j].x == monster[i].x-1 && trap[j].y == monster[i].y)
						{
							moveable = false;
						}
					}
					if(moveable)
						moveLeft(monster[i].x,monster[i].y,map,monster[i].symbol);
				}
			}
			else if(move == 2){
				if(map.format[monster[i].y + 1][monster[i].x] == '*' || map.format[monster[i].y + 1][monster[i].x] == 'O'){
					continue;
				}
				else{
					for(int j = 0 ; j < 2 ; j ++){
						if(trap[j].x == monster[i].x && trap[j].y == monster[i].y + 1)
						{
							moveable = false;
						}
					}
					if(moveable)
						moveDown(monster[i].x,monster[i].y,map,monster[i].symbol);
				}
			}
			else if(move == 3){
				if(map.format[monster[i].y - 1][monster[i].x ] == '*' || map.format[monster[i].y - 1][monster[i].x ] == 'O'){
					continue;
				}
				else{
					for(int j = 0 ; j < 2 ; j ++){
						if(trap[j].x == monster[i].x && trap[j].y == monster[i].y - 1)
						{
							moveable = false;
						}
					}
					if(moveable)
						moveUp(monster[i].x,monster[i].y,map,monster[i].symbol);
				}
			}
			if(player.x == monster[i].x && player.y == monster[i].y)
			{
				running = false;
				return;
			}
		}	
	}
}
void moveLeft(int &x,int &y,map& map,char symbol){
	if(map.format[y][x - 1] == '#')return;
	map.format[y][x] = ' ';
	map.format[y][x - 1] = symbol;
	x --;
}
void moveDown(int &x,int &y,map& map,char symbol){
	if(map.format[y + 1][x] == '#')return;
	map.format[y][x] = ' ';
	map.format[y + 1][x ] = symbol;
	y ++;
}
void moveRight(int &x,int &y,map& map,char symbol){
	if(map.format[y][x + 1] == '#')return;
	map.format[y][x] = ' ';
	map.format[y][x + 1] = symbol;
	x ++;
}
void moveUp(int &x,int &y,map& map,char symbol){
	if(map.format[y - 1][x] == '#')return;
	map.format[y][x] = ' ';
	map.format[y - 1][x] = symbol;
	y--;
}
void movePlayer(player &player,trap traps[],char temp,map &map,monster monsters[]){
	if(temp == 'a')
	{
		moveLeft(player.x,player.y,map,player.symbol);
	}
	else if(temp == 'd'){
		moveRight(player.x,player.y,map,player.symbol);
	}
	else if(temp == 's'){
		moveDown(player.x,player.y,map,player.symbol);
	}
	else if(temp == 'w'){
		moveUp(player.x,player.y,map,player.symbol);
	}
	for(int i = 0 ; i < 2; i ++){
		if(traps[i].x == player.x && traps[i].y == player.y){
			running = false; return;
		}
	}
	for(int i = 0 ; i < 2; i ++){
		if(monsters[i].x == player.x && monsters[i].y == player.y){
			running = false; return;
		}
	}
	if(player.x == 14 && player.y == 8) // WIN
	{
		cls();
		printMap(map);
		isWin = true;
		running = false; return;
	}
}
void printMap(map map){
	for(int i = 0 ; i < map.height ; i ++){
		for(int j = 0 ; j < map.width ; j ++){
			printf("%c",map.format[i][j]);
		}
		printf("\n");
	}
}
void generateEntities(map &map,player &player, monster monsters[],trap trap[]){
	//PLAYER
	player.x = 1;
	player.y = 1;
	player.symbol = '@';
	map.format[player.y][player.x] = player.symbol;
	//MONSTER
	int x,y;
	for(int i = 0 ; i < 2 ; i ++){
		do{
			x = rand()%16;
			y = rand()%8;
		}while(!checkCollide(x,y,map));	
		monsters[i].x = x;
		monsters[i].y = y;
		monsters[i].symbol = 'O';
		map.format[monsters[i].y][monsters[i].x] = monsters[i].symbol; 
	}
	//TRAP
	for(int i = 0 ; i < 2 ; i ++){
		do{
			x = rand()%16;
			y = rand()%9;
		}while(!checkCollide(x,y,map) && !checkTrap(trap,i));	
		trap[i].x = x;
		trap[i].y = y;
	}
	
}
bool checkTrap(trap trap[],int pos){
	for(int i = 0 ; i < pos; i ++){
		if(trap[pos].x == trap[i].x && trap[pos].y == trap[i].y)return false;
	}
	return true;
}
bool checkCollide(int x,int y,map map){
	if(map.format[y][x] == '#' || map.format[y][x] == '@' || map.format[y][x] == '*'|| map.format[y][x] == 'O')return false;
	return true;
}
void generateMap(map &map){
	mapptr = fopen("map.txt","r");
	map.height = 0;
	if(!mapptr){
		printf("map.txt not found !");
	}
	
	while(!feof(mapptr)){
		fscanf(mapptr,"%[^\n]\n",map.format[map.height]);
		map.height ++;
	}
	map.width = strlen(map.format[0]);
}
void cls(){
	system("cls");
}
void homePage(){
	printf("Dungeon Master\n");
	printf("1. Play\n");
	printf("2. Rules\n");
	printf("3. Exit\n");
	printf(">>");
}
