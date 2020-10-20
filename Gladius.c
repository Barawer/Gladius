#include "windows.h"
#include <cvintwrk.h>
#include "toolbox.h"
#include <utility.h>
#include <cvirte.h>		
#include <userint.h>
#include "Gladius.h"
#include "bass.h"


typedef struct{
	int x,y,isfired,vx,vy;
} bullet;

typedef struct{
	int x,y,vx,vy,dead,width,fired,firex,firey;
} enemies;

typedef struct{
	int x,y,vx,vy,dead,lives,fired;
}bosses;
static int panelHandle,panelHandle2,panelHandle3,panelHandle4,panelHandle5,panelHandle6;
int player,playerlaser,enemy1,stage1back,platform,enemy3,enemy3laser,stage4laser,rogue,boss5midbullets,stage2back,stage3back,stage4back,stage5back,heart;//images
int boss1img,boss1laser,boss3img,boss3regim,boss3top,boss3bot,boss3both,boss3bullets,boss5img,boss5bot,boss5top,boss5both,boss5bullets;//bossimages
int x=0,y=40,fire=0,starty,startwidth,lives=3,maxlives=5,j=2,can_fire=1;//Play Stats
int Callback_Data,Posting_Handle,Keys_Down[256]={0};//Keys check	
int stage=3;//Stage Cases
int canvas_width,canvas_height,backx=0;//Canvas Size
int cheat[10],code=0,konami[10]={1536,1536,1792,1792,2048,2304,2048,2304,98,97},konami2[10]={1536,1536,1792,1792,2048,2304,2048,2304,240,249},konami3[10]={1536,1536,1792,1792,2048,2304,2048,2304,66,65};//enter cheat menu
bullet bullets[10],boss3topbullets[4],boss3botbullets[4],boss5topbullets[4],boss5botbullets[4],boss5middle1[4],boss5middle2[4];//bullets array
int bulletnum=0;//the bullet in the array
int timeCounter=0; //used to count time passed
int point=0,StageTimeMin,StageTimeSec,CurrentStageTime,textDisplay=100; //hud ints
char hudPoint[1000],hudTime[5],stageNumb[10]; //Hud Strings				  
enemies enemies1[4],enemies2[6],enemies3[3],enemies4[8];
bosses boss1,boss3,boss5;
HSTREAM titlesong,stage1,stage1boss,stage2,stage3,stage3boss,stage4,stage5,playerfire,Boss1Firing,Enemy4Explode,Enemy1Explode,Enemy3Explode,Cheat;
HSTREAM BossExplode,Boss3Hatch,PlayerExplode,Boss5VentExplode,Boss5EnergyBall,Boss5Missiles,Victory,Boss3Firing,Enemy3Firing;

int rouge_state=0;
int currentscore,leader[10];
int PowerLivesDisplay=50,PowerPointsDisplay=50,LivesFlag=0,PointFlag=0,PowerUpEnable;
FILE *fp;

void initEnemies();//prototype
void CreateLeader();//prototype


void init(){
	srand(time(0));
	CreateMetaFont ("STAGEFONT", "Fixedsys", 36, 0, 0, 0, 0);
	CreateMetaFont ("TIMEFONT", "Fixedsys", 14, 0, 0, 0, 0);
	SetCtrlAttribute (panelHandle, GAME_CANVAS, ATTR_PEN_COLOR, VAL_WHITE);
	GetCtrlAttribute(panelHandle, GAME_CANVAS, ATTR_WIDTH, &canvas_width);
	GetCtrlAttribute(panelHandle, GAME_CANVAS, ATTR_HEIGHT, &canvas_height);
	GetBitmapFromFile("Data\\Sprites\\Player.png", &player);
	GetBitmapFromFile("Data\\Sprites\\PlayerLaser.png", &playerlaser);
	GetBitmapFromFile("Data\\Sprites\\enemy1.png", &enemy1);
	GetBitmapFromFile("Data\\Backgrounds\\Stage1.png", &stage1back);
	GetBitmapFromFile("Data\\Backgrounds\\Stage2.png", &stage2back);
	GetBitmapFromFile("Data\\Backgrounds\\Stage3.jpg", &stage3back);
	GetBitmapFromFile("Data\\Backgrounds\\Stage4.png", &stage4back);
	GetBitmapFromFile("Data\\Backgrounds\\Stage5.png", &stage5back);
	GetBitmapFromFile("Data\\Sprites\\boss1.png", &boss1img);
	GetBitmapFromFile("Data\\Sprites\\boss1Laser.png",&boss1laser);
	GetBitmapFromFile("Data\\Sprites\\platform.png",&platform);
	GetBitmapFromFile("Data\\Sprites\\enemy3.png",&enemy3);
	GetBitmapFromFile("Data\\Sprites\\enemy3laser.png",&enemy3laser);
	GetBitmapFromFile("Data\\Sprites\\stage4laser.png",&stage4laser);
	GetBitmapFromFile("Data\\Sprites\\boss3.png",&boss3regim);
	GetBitmapFromFile("Data\\Sprites\\boss3top.png",&boss3top); 
	GetBitmapFromFile("Data\\Sprites\\boss3bot.png",&boss3bot); 
	GetBitmapFromFile("Data\\Sprites\\boss3both.png",&boss3both);
	GetBitmapFromFile("Data\\Sprites\\boss3bullets.png",&boss3bullets);
	GetBitmapFromFile("Data\\Sprites\\boss5.png",&boss5img);
	GetBitmapFromFile("Data\\Sprites\\boss5bullets.png",&boss5midbullets);
	GetBitmapFromFile("Data\\Sprites\\fire.png",&fire);
	GetBitmapFromFile("Data\\rogue.png",&rogue);
	GetBitmapFromFile("Data\\heart.png",&heart);
	initEnemies();
	BASS_Init( -1,44100, 0,0,NULL);
	titlesong = BASS_StreamCreateFile(FALSE,"Data\\Music\\Title.mp3",0,0,0);
	stage1 = BASS_StreamCreateFile(FALSE,"Data\\Music\\stage1.mp3",0,0,0);
	stage1boss = BASS_StreamCreateFile(FALSE,"Data\\Music\\stage1boss.mp3",0,0,0);
	stage2 = BASS_StreamCreateFile(FALSE,"Data\\Music\\stage2.mp3",0,0,0);
	stage3 = BASS_StreamCreateFile(FALSE,"Data\\Music\\stage3.mp3",0,0,0);
	stage3boss = BASS_StreamCreateFile(FALSE,"Data\\Music\\stage3boss.mp3",0,0,0);
	stage4 = BASS_StreamCreateFile(FALSE,"Data\\Music\\stage4.mp3",0,0,0);
	stage5 = BASS_StreamCreateFile(FALSE,"Data\\Music\\stage5.mp3",0,0,0);
	playerfire=BASS_StreamCreateFile(FALSE,"Data\\Music\\sfx\\playerfire.wav",0,0,0);
	Boss1Firing=BASS_StreamCreateFile(FALSE,"Data\\Music\\sfx\\Boss1Firing.wav",0,0,0);
	Enemy1Explode=BASS_StreamCreateFile(FALSE,"Data\\Music\\\sfx\\Enemy1Explode.mp3",0,0,0);
	Enemy4Explode=BASS_StreamCreateFile(FALSE,"Data\\Music\\sfx\\Enemy4Explode.wav",0,0,0);
	BossExplode=BASS_StreamCreateFile(FALSE,"Data\\Music\\SFX\\BossExplode.mp3",0,0,0);
	Boss3Hatch=BASS_StreamCreateFile(FALSE,"Data\\Music\\SFX\\Boss3Hatch.mp3",0,0,0);
	Cheat=BASS_StreamCreateFile(FALSE,"Data\\Music\\SFX\\Cheat.mp3",0,0,0);
	PlayerExplode=BASS_StreamCreateFile(FALSE,"Data\\Music\\SFX\\PlayerExplode.mp3",0,0,0);
	Enemy3Explode=BASS_StreamCreateFile(FALSE,"Data\\Music\\sfx\\Enemy3Explode.wav",0,0,0);
	Boss5VentExplode=BASS_StreamCreateFile(FALSE,"Data\\Music\\SFX\\VentExplode.wav",0,0,0);
	Boss3Firing=BASS_StreamCreateFile(FALSE,"Data\\Music\\SFX\\Boss3Firing.mp3",0,0,0);
	Boss5EnergyBall=BASS_StreamCreateFile(FALSE,"Data\\Music\\SFX\\Boss5EnergyBall.wav",0,0,0);
	Boss5Missiles=BASS_StreamCreateFile(FALSE,"Data\\Music\\SFX\\Boss5Missiles.wav",0,0,0);
	Victory=BASS_StreamCreateFile(FALSE,"Data\\Music\\Victory.mp3",0,0,0);
	Enemy3Firing=BASS_StreamCreateFile(FALSE,"Data\\Music\\SFX\\Enemy3Firing.wav",0,0,0);
	CreateLeader();
	

}

void initEnemies(){
	starty=rand()%(540)+40;
	startwidth=rand()%400;
	
	for (int i=0;i<4;i++){//init enemies1
		enemies1[i].x=canvas_width+40*i;
		enemies1[i].y=starty+i*10;
		enemies1[i].vy=0;
		enemies1[i].vx=-1;
		enemies1[i].dead=0;
	}
	for (int i=0;i<6;i++){//init enemies2 
		startwidth=rand()%400;
		enemies2[i].vx=-1;
		enemies2[i].x=canvas_width+rand()%canvas_width;
		enemies2[i].y=starty+i*rand()%canvas_height;
		enemies2[i].width=50+startwidth;
	}
	for (int i=0;i<3;i++){//init enemies3 
		startwidth=rand()%400;
		enemies3[i].vx=-1;
		enemies3[i].x=canvas_width+rand()%canvas_width;
		enemies3[i].y=starty+i*rand()%canvas_height;
		enemies3[i].vy=0;
		enemies3[i].dead=0;
		enemies3[i].fired=1;
	}
	for (int i=0;i<8;i++){//init enemies4 
		startwidth=rand()%400;
		enemies4[i].vx=-rand()%5;
		enemies4[i].x=canvas_width+rand()%canvas_width;
		enemies4[i].y=starty+i*rand()%canvas_height;
		enemies4[i].width=100;
		enemies4[i].dead=0;
	}
	for (int j=0;j<10;j++){//init bullets 
		bullets[j].isfired=0;
	}
	for (int j=0;j<4;j++){//init bullets 
		boss3topbullets[j].isfired=0;
	}
	for (int j=0;j<4;j++){//init bullets 
		boss3botbullets[j].isfired=0;
	}
	//boss1
	boss1.x=canvas_width;
	boss1.vx=1;
	boss1.y=80;
	boss1.vy=1;
	boss1.lives=10;
	boss1.dead=0;
	//boss3
	boss3.x=canvas_width;
	boss3.vx=1;
	boss3.y=80;
	boss3.lives=10;
	boss3.dead=0;
	boss3img=boss3regim;
	//boss5
	boss5.x=canvas_width;
	boss5.vx=1;
	boss5.y=40;
	boss5.lives=10;
	boss5.dead=0;
	boss5bullets=stage4laser;
}

void freeMem(){
	DiscardBitmap(player);
	DiscardBitmap(playerlaser);
	DiscardBitmap(enemy1);
	DiscardBitmap(stage1back);
	DiscardBitmap(stage2back);
	DiscardBitmap(stage3back);
	DiscardBitmap(stage4back);
	DiscardBitmap(stage5back);
	DiscardBitmap(heart);
	DiscardBitmap(boss1img);
	DiscardBitmap(boss1laser);
	DiscardBitmap(platform);
	DiscardBitmap(enemy3);
	DiscardBitmap(enemy3laser);
	DiscardBitmap(stage4laser);
	DiscardBitmap(boss3regim);
	DiscardBitmap(boss5img);
	DiscardBitmap(boss3top);
	DiscardBitmap(boss3bot);
	DiscardBitmap(boss3both);
	DiscardBitmap(boss3bullets);
	DiscardBitmap(boss5midbullets);
	DiscardBitmap(fire);
	DiscardBitmap(rogue);
	BASS_StreamFree(titlesong);
	BASS_StreamFree(stage1);
	BASS_StreamFree(stage1boss);
	BASS_StreamFree(stage2);
	BASS_StreamFree(stage3);
	BASS_StreamFree(stage3boss);
	BASS_StreamFree(stage4);
	BASS_StreamFree(stage5);
	BASS_StreamFree(playerfire);
	BASS_StreamFree(Boss1Firing);
	BASS_StreamFree(Enemy1Explode);
	BASS_StreamFree(Enemy4Explode);
	BASS_StreamFree(BossExplode);
	BASS_StreamFree(Boss3Hatch);
	BASS_StreamFree(PlayerExplode);
	BASS_StreamFree(Enemy3Explode);
	BASS_StreamFree(Boss5VentExplode);
	BASS_StreamFree(Boss3Firing);
	BASS_StreamFree(Boss5EnergyBall);
	BASS_StreamFree(Boss5Missiles);
	BASS_StreamFree(Victory);
	BASS_StreamFree(Enemy3Firing);
	BASS_StreamFree(Cheat);
}

void CreateLeader(){
	char data[50],names[21];
	int namescore,i=0;
	fp=fopen("Data\\score.scores","r");
	while (fgets(data,49,fp)!=NULL){ 
		sscanf(data,"%[^,],%d",names,&namescore);
		SetTableCellVal (panelHandle5, LEADER_TABLE, MakePoint (1,1+i), names);
		SetTableCellVal (panelHandle5, LEADER_TABLE, MakePoint (2,1+i), namescore);
		i++;
	}
	fclose(fp);
	SortTableCells (panelHandle5, LEADER_TABLE, MakeRect(1,1,10,2), VAL_COLUMN_MAJOR, 2, 1, 0, 0);
	 
}
void PlayerDied(){
	char playername[21],name[21];
	int scores;
	if(lives==0) BASS_ChannelPlay(PlayerExplode,TRUE);
	GetTableCellVal (panelHandle5, LEADER_TABLE, MakePoint(2,10),&currentscore);
	if(point>currentscore){
		GetCtrlVal (panelHandle2, MENU_STRING, playername);
		SetTableCellVal (panelHandle5, LEADER_TABLE, MakePoint(1,10),playername);
		SetTableCellVal (panelHandle5, LEADER_TABLE, MakePoint (2,10), point);
		SortTableCells (panelHandle5, LEADER_TABLE, MakeRect(1,1,10,2), VAL_COLUMN_MAJOR, 2, 1, 0, 0);
		fp=fopen("Data\\score.scores","w");
		for(int i=1;i<11;i++){
		GetTableCellVal (panelHandle5, LEADER_TABLE, MakePoint(1,i),name);
		GetTableCellVal (panelHandle5, LEADER_TABLE, MakePoint(2,i),&scores);
		fprintf(fp,"%s,%d\n",name,scores);
		}
		fclose(fp);
	}
	stage=-1;
	textDisplay=100;
}
//Receives information from windows regarding key presses
int CVICALLBACK Key_DownFunc(int panel, int message, unsigned int* wParam, unsigned int* lParam, void* callbackData)
{
	unsigned int Key = *wParam;
	Keys_Down[Key] = 1;

	return 0;
	
}

//Receives information from windows regarding key releases
int CVICALLBACK Key_UpFunc(int panel, int message, unsigned int* wParam, unsigned int* lParam, void* callbackData)
{
	unsigned int Key = *wParam;
	Keys_Down[Key] = 0;
	return 0;
}

//asking windows to send keyboard press and release events to our software 
//Specifing 'Key_DownFunc' and 'Key_UpFunc' as the functions to call
void Connect_To_Windows_Events(void)
{
	InstallWinMsgCallback(panelHandle,WM_KEYDOWN,Key_DownFunc,VAL_MODE_IN_QUEUE,&Callback_Data,&Posting_Handle);
	InstallWinMsgCallback(panelHandle,WM_KEYUP,Key_UpFunc,VAL_MODE_IN_QUEUE,&Callback_Data,&Posting_Handle);
}

int main (int argc, char *argv[])
{
	if (InitCVIRTE (0, argv, 0) == 0)
		return -1;	/* out of memory */
	if ((panelHandle = LoadPanel (0, "Gladius.uir", GAME)) < 0)
		return -1;
	if ((panelHandle2 = LoadPanel (0, "Gladius.uir", MENU)) < 0)
		return -1;
	if ((panelHandle3 = LoadPanel (0, "Gladius.uir", ABOUT)) < 0)
		return -1;
	if ((panelHandle4 = LoadPanel (0, "Gladius.uir", CHEATMENU)) < 0)
		return -1;
	if ((panelHandle5 = LoadPanel (0, "Gladius.uir", LEADER)) < 0)
		return -1;
	if ((panelHandle6 = LoadPanel (0, "Gladius.uir", SETTINGS)) < 0)
		return -1;
	init();
	Connect_To_Windows_Events();
	DisplayPanel (panelHandle2);
	BASS_ChannelPlay(titlesong,TRUE);
	RunUserInterface ();
	DiscardPanel (panelHandle2);
	freeMem();
	return 0;
}

									   
int CVICALLBACK exitGame (int panel, int event, void *callbackData,
					   int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_GOT_FOCUS:

			break;
		case EVENT_LOST_FOCUS:

			break;
		case EVENT_CLOSE:
			HidePanel(panelHandle);
			SetCtrlAttribute (panelHandle, GAME_TIMER, ATTR_ENABLED, 0);
			DisplayPanel(panelHandle2);
			PlayerDied();
			BASS_ChannelStop(stage1);
			BASS_ChannelStop(stage1boss);
			BASS_ChannelStop(stage2);
			BASS_ChannelStop(stage3);
			BASS_ChannelStop(stage3boss);
			BASS_ChannelStop(stage4);
			BASS_ChannelStop(stage5);
			BASS_ChannelStop(Victory);
			BASS_ChannelPlay(titlesong,TRUE);
			
			break;
	}
	return 0;
}

int CVICALLBACK exitMenu (int panel, int event, void *callbackData,
					   int eventData1, int eventData2)
{

	switch (event)
	{
		case EVENT_KEYPRESS:
		cheat[code]=eventData1;
		if (cheat[code]==konami[code]||cheat[code]==konami2[code]||cheat[code]==konami3[code]){
			code++;
			if (code>=10){
				SetCtrlAttribute (panelHandle2, MENU_CHEATS, ATTR_VISIBLE, 1);
				BASS_ChannelPlay(Cheat,TRUE);
				MessagePopup("Cheat Menu Unlocked!","Cheat Menu Unlocked!");
				code=0;
			}
		}
		else{
			code=0;
		}
		
			break;
		case EVENT_GOT_FOCUS:

			break;
		case EVENT_LOST_FOCUS:

			break;
		case EVENT_CLOSE:
			QuitUserInterface (0);
			break;
	}

	return 0;
}

int CVICALLBACK exitAbout (int panel, int event, void *callbackData,
					   int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_GOT_FOCUS:

			break;
		case EVENT_LOST_FOCUS:

			break;
		case EVENT_CLOSE:
			HidePanel(panelHandle3);
			break;
	}
	return 0;
}

int CVICALLBACK startGame (int panel, int control, int event,
						   void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			HidePanel (panelHandle2);
			DisplayPanel(panelHandle);
			initEnemies();
			textDisplay=100;
			point=0;
			stage=1;
			StageTimeMin=1;
			StageTimeSec=30;
			CurrentStageTime=30;
			BASS_ChannelStop(titlesong);
			GetCtrlVal (panelHandle4, CHEATMENU_LEVELSELECT, &stage);
			GetCtrlVal (panelHandle4, CHEATMENU_LIVES, &lives);
			GetCtrlVal (panelHandle4, CHEATMENU_LIVES, &maxlives);
			GetCtrlVal (panelHandle4, CHEATMENU_POWERUPS, &PowerUpEnable);
			SetCtrlAttribute (panelHandle, GAME_TIMER, ATTR_ENABLED, 1);
			break;
	}
	return 0;
}

int CVICALLBACK cheatMenu (int panel, int control, int event,
						   void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			DisplayPanel(panelHandle4);
			SetCtrlAttribute (panelHandle4, CHEATMENU_ROUGETIMER, ATTR_ENABLED, 1);
			break;
	}
	return 0;
}

int CVICALLBACK exitCheat (int panel, int event, void *callbackData,
						   int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_GOT_FOCUS:

			break;
		case EVENT_LOST_FOCUS:

			break;
		case EVENT_CLOSE:
			HidePanel(panelHandle4);
			SetCtrlAttribute (panelHandle4, CHEATMENU_ROUGETIMER, ATTR_ENABLED, 0);
			break;
	}
	return 0;
}

int CVICALLBACK showLeaderBoards (int panel, int control, int event,
								  void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			DisplayPanel(panelHandle5);
			break;
	}
	return 0;
}

int CVICALLBACK quitGameButton (int panel, int control, int event,
								void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			QuitUserInterface(0);
			break;
	}
	return 0;
}

int CVICALLBACK exitLeader (int panel, int event, void *callbackData,
							int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_GOT_FOCUS:

			break;
		case EVENT_LOST_FOCUS:

			break;
		case EVENT_CLOSE:
			HidePanel(panelHandle5);
			break;
	}
	return 0;
}

int CVICALLBACK ShowRouge (int panel, int control, int event,
						   void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_TIMER_TICK:
		rouge_state++;
		if (rouge_state>=110) rouge_state=0;
		CanvasStartBatchDraw (panelHandle4, CHEATMENU_CHEATCANVAS);
		CanvasClear (panelHandle4, CHEATMENU_CHEATCANVAS, VAL_ENTIRE_OBJECT);
		CanvasDrawBitmap (panelHandle4, CHEATMENU_CHEATCANVAS, rogue, MakeRect(32*(rouge_state/10),32*(rouge_state%10),32,32), VAL_ENTIRE_OBJECT);
		CanvasEndBatchDraw (panelHandle4, CHEATMENU_CHEATCANVAS); 


			break;
	}
	return 0;
}

void Powerups(){
	if (PowerUpEnable==1){
		if (rand()%15==7&&LivesFlag==0&&lives<maxlives){
			lives++;
			LivesFlag=1;
		}
		if (rand()%15==10&&PointFlag==0 ){
			point+=500;
			PointFlag=1;
		}
	}
}

void player_movment(){
	if(Keys_Down[40]==1){//down arrow
		y+=5;
	}
	if(Keys_Down[38]==1){ //up arrow
		y-=5;
	}
	if(Keys_Down[37]==1){//left arrow
		x-=5;
	}
	if(Keys_Down[39]==1){//right arrow
		x+=5;
	}
	if (y<=40){
		y=40;
	}
	if (y>=canvas_height-70){
		y=canvas_height-70;
	}
	if (x<=0){
		x=0;
	}
	if (x>=canvas_width-70){
		x=canvas_width-70;
	}
}

void Fire(){
	
	if (Keys_Down[32]){//space
		if(bullets[bulletnum].isfired==0){
			bullets[bulletnum].x=x+70;
			bullets[bulletnum].y=y+60/2;
			BASS_ChannelPlay(playerfire,TRUE);
		} 
		bullets[bulletnum].isfired=1;
		bulletnum=(bulletnum+1)%4;
	}
}

void Background(){
	switch (stage){
		case 1:	
			CanvasDrawBitmap (panelHandle, GAME_CANVAS, stage1back, VAL_ENTIRE_OBJECT, MakeRect(40,backx,560,canvas_width));
			CanvasDrawBitmap (panelHandle, GAME_CANVAS, stage1back, VAL_ENTIRE_OBJECT, MakeRect(40,canvas_width+backx,560,canvas_width));
			if(backx<-canvas_width){
				backx=0;
			}
			else{
				backx-=2;
			}
			break;
		case 2:
			CanvasDrawBitmap (panelHandle, GAME_CANVAS, stage2back, VAL_ENTIRE_OBJECT, MakeRect(40,backx,560,canvas_width));
			CanvasDrawBitmap (panelHandle, GAME_CANVAS, stage2back, VAL_ENTIRE_OBJECT, MakeRect(40,canvas_width+backx,560,canvas_width));
			if(backx<-canvas_width){
				backx=0;
			}
			else{
				backx-=2;
			}
			break;
		case 3:
			CanvasDrawBitmap (panelHandle, GAME_CANVAS, stage3back, VAL_ENTIRE_OBJECT, MakeRect(40,backx,560,canvas_width));
			CanvasDrawBitmap (panelHandle, GAME_CANVAS, stage3back, VAL_ENTIRE_OBJECT, MakeRect(40,canvas_width+backx,560,canvas_width));
			if(backx<-canvas_width){
				backx=0;
			}
			else{
				backx-=2;
			}
			break;
		case 4:
			CanvasDrawBitmap (panelHandle, GAME_CANVAS, stage4back, VAL_ENTIRE_OBJECT, MakeRect(40,backx,560,canvas_width));
			CanvasDrawBitmap (panelHandle, GAME_CANVAS, stage4back, VAL_ENTIRE_OBJECT, MakeRect(40,canvas_width+backx,560,canvas_width));
			if(backx<-canvas_width){
				backx=0;
			}
			else{
				backx-=2;
			}
			break;
		case 5:
			CanvasDrawBitmap (panelHandle, GAME_CANVAS, stage5back, VAL_ENTIRE_OBJECT, MakeRect(40,backx,560,canvas_width));
			CanvasDrawBitmap (panelHandle, GAME_CANVAS, stage5back, VAL_ENTIRE_OBJECT, MakeRect(40,canvas_width+backx,560,canvas_width));
			if(backx<-canvas_width){
				backx=0;
			}
			else{
				backx-=2;
			}
			break;
		case 0:
			CanvasDrawBitmap (panelHandle, GAME_CANVAS, stage5back, VAL_ENTIRE_OBJECT, MakeRect(40,backx,560,canvas_width));
			CanvasDrawBitmap (panelHandle, GAME_CANVAS, stage5back, VAL_ENTIRE_OBJECT, MakeRect(40,canvas_width+backx,560,canvas_width));
			if(backx<-canvas_width){
				backx=0;
			}
			else{
				backx-=2;
			}
			break;
	}
}

void drawHud(){
	SetCtrlAttribute (panelHandle, GAME_CANVAS, ATTR_PEN_FILL_COLOR, VAL_DK_GREEN); //hud background color
	CanvasDrawRect (panelHandle, GAME_CANVAS,MakeRect(0,0,40,canvas_width) , VAL_DRAW_FRAME_AND_INTERIOR);//draw hud background
	sprintf(hudPoint,"%d",point);//create points string
	CanvasDrawText (panelHandle, GAME_CANVAS, hudPoint, "TIMEFONT",MakeRect(10,(9*canvas_width/10)-100,20,100) , VAL_CENTER);//draw point string
	sprintf(hudTime,"%d:%02d",StageTimeMin,StageTimeSec);	//create timer string
	CanvasDrawText(panelHandle,GAME_CANVAS,hudTime,"TIMEFONT",MakeRect(10,(5*canvas_width/10)-100,20,100) , VAL_CENTER);//draw timer
	SetCtrlAttribute (panelHandle, GAME_CANVAS, ATTR_PEN_FILL_COLOR, VAL_BLACK);//change background color to black
	CanvasDrawRect (panelHandle, GAME_CANVAS,MakeRect(15,10,10,150) , VAL_DRAW_FRAME_AND_INTERIOR);//create empty lives meter
	SetCtrlAttribute (panelHandle, GAME_CANVAS, ATTR_PEN_FILL_COLOR, VAL_RED);//change background color to red
	CanvasDrawRect (panelHandle, GAME_CANVAS,MakeRect(15,10,10,150*(double)lives/maxlives), VAL_DRAW_FRAME_AND_INTERIOR); //create lives meter
}

void Time(){
	 StageTimeSec=CurrentStageTime-(int)(timeCounter*0.02);
	 if (StageTimeSec<0){
		 CurrentStageTime=59; 
		 StageTimeMin--;
		 timeCounter=0;
	 }
	 timeCounter++;
}

void Stage1(){
	for (int j=0;j<4;j++){//draw bullets
		if (bullets[j].isfired==1){
			bullets[j].x+=10;
			CanvasDrawBitmap (panelHandle, GAME_CANVAS, playerlaser, VAL_ENTIRE_OBJECT, MakeRect(bullets[j].y,bullets[j].x,10,10));
		}
		for(int i=0;i<4;i++){;
			if(enemies1[i].dead==0){//check if enemy is alive
				if (bullets[j].isfired==1&&RectIntersection(MakeRect(bullets[j].y,bullets[j].x,10,10),MakeRect(enemies1[i].y,enemies1[i].x,20,20),NULL)){ //check if the bullet killed the enemy
					enemies1[i].dead=1;
					BASS_ChannelPlay(Enemy1Explode,TRUE);
					Powerups();
					bullets[j].isfired=0;
					point+=100;
				}
				else if(RectIntersection(MakeRect(y,x,70,70),MakeRect(enemies1[i].y,enemies1[i].x,20,20),NULL)==1){ //check if the player crashed into the enemy
					lives--;
					if (lives<=0){
						PlayerDied();
					}
					enemies1[i].dead=1; 
				}
				else{
					CanvasDrawBitmap (panelHandle, GAME_CANVAS, enemy1, VAL_ENTIRE_OBJECT, MakeRect(enemies1[i].y,enemies1[i].x,20,20));
					if((enemies1[i].y-(y+30))!=0&&enemies1[i].x>x){//close in on the player 
						enemies1[i].vy=-(enemies1[i].y-(y+30))/abs(enemies1[i].y-(y+30));
					}
					else{ 
						enemies1[i].vy=0;
					}
					enemies1[i].x+=enemies1[i].vx;
					enemies1[i].y+=enemies1[i].vy;
					if(enemies1[i].x+20<0){ //check if the enemy went past the screen
						enemies1[i].x+=canvas_width+500;
						enemies1[i].y=starty;
					}
					if(bullets[j].isfired==1&&bullets[j].x>canvas_width){ //check if the bullet went past the screen  
						bullets[j].x=0;
						bullets[j].isfired=0;	
					}
				}
			}
			else{ //restart the enemy
				enemies1[i].dead=0;
				enemies1[i].x=canvas_width+40*i;
				enemies1[i].y=starty;
				enemies1[i].vy=0;
				enemies1[i].vx=-2;
			}
		}
	}
}

void Stage1Boss(){
	static int move=0;//enemy laser movement
	if (boss1.x>(canvas_width-300)){//boss entry
		if (boss1.x==canvas_width){
			BASS_ChannelStop(stage1);
			BASS_ChannelPlay(stage1boss,TRUE);
		}
		can_fire=0;
		boss1.x-=boss1.vx;
		CanvasDrawBitmap (panelHandle, GAME_CANVAS, boss1img, VAL_ENTIRE_OBJECT,MakeRect(boss1.y,boss1.x,canvas_height/2,300));
		SetCtrlAttribute (panelHandle, GAME_CANVAS, ATTR_PEN_FILL_COLOR, VAL_TRANSPARENT);
		CanvasDrawText (panelHandle, GAME_CANVAS, "BOSS APPROCHING", "STAGEFONT",MakeRect(canvas_height/2-50,canvas_width/2-150,VAL_KEEP_SAME_SIZE,VAL_KEEP_SAME_SIZE) , VAL_CENTER);
	}
	else{
		can_fire=1;
		for (int i=0;i<4;i++){
			if (bullets[i].isfired==1){//draw bullets
				bullets[i].x+=10;
				CanvasDrawBitmap (panelHandle, GAME_CANVAS, playerlaser, VAL_ENTIRE_OBJECT, MakeRect(bullets[i].y,bullets[i].x,10,10));
			}
			if(boss1.dead==0){
				if (bullets[i].isfired==1&&RectIntersection(MakeRect(bullets[i].y,bullets[i].x,10,10),MakeRect(boss1.y+120,boss1.x,70,50),NULL)){
					boss1.lives--;
					bullets[i].isfired=0;
					point+=200;
					if (boss1.lives==0){
						BASS_ChannelPlay(BossExplode,TRUE);
						boss1.dead=1;
						move=0;
					}
					else{
						CanvasDrawBitmap (panelHandle, GAME_CANVAS, boss1img, VAL_ENTIRE_OBJECT,MakeRect(boss1.y,boss1.x,canvas_height/2,300));
					}
				}
				else if(RectIntersection(MakeRect(y,x,70,70),MakeRect(boss1.y,boss1.x,canvas_height-40,300),NULL)==1){
					lives--;
					if (lives<=0){
						PlayerDied();
					}
					CanvasDrawBitmap (panelHandle, GAME_CANVAS, boss1img, VAL_ENTIRE_OBJECT,MakeRect(boss1.y,boss1.x,canvas_height/2,300));
				}
				else if(RectIntersection(MakeRect(y,x,70,70),MakeRect(boss1.y+121,boss1.x+move*2,60,-move*2),NULL)==1){
					lives--;
					boss1.fired=0;
					move=0;
					if (lives<=0){
						PlayerDied();
					}
					CanvasDrawBitmap (panelHandle, GAME_CANVAS, boss1img, VAL_ENTIRE_OBJECT,MakeRect(boss1.y,boss1.x,canvas_height/2,300));
				}
				else{
					CanvasDrawBitmap (panelHandle, GAME_CANVAS, boss1img, VAL_ENTIRE_OBJECT,MakeRect(boss1.y,boss1.x,canvas_height/2,300));
					boss1.y+=boss1.vy;
					if (boss1.y>canvas_height/2){
						boss1.vy=-1;
					}
					else if (boss1.y-40<0){
						boss1.vy=1;
					}
					if(bullets[i].isfired==1&&bullets[i].x>canvas_width){  
						bullets[i].x=0;
						bullets[i].isfired=0;	
					}
				}
			}
			else{
				point+=10000;
				stage=2;
				textDisplay=100;
				CurrentStageTime=59;
				StageTimeMin=0;
				StageTimeSec=59;
				}
				
			}
			if (((rand()%100==1||boss1.fired==1)&&boss1.x==canvas_width-300)&&boss1.dead==0){//boss firing
				if (boss1.fired==0) BASS_ChannelPlay(Boss1Firing,TRUE);
				boss1.fired=1;
				CanvasDrawBitmap (panelHandle, GAME_CANVAS, boss1laser, VAL_ENTIRE_OBJECT,MakeRect(boss1.y+121,boss1.x+move*2,60,-move*2) );
				move=move-2;
				if (abs(move*2)>canvas_width){
					move=0;
					boss1.fired=0;
				}
				
			}
		}
}

void Stage2(){
	for (int j=0;j<4;j++){
		if (bullets[j].isfired==1){
			bullets[j].x+=10;
			CanvasDrawBitmap (panelHandle, GAME_CANVAS, playerlaser, VAL_ENTIRE_OBJECT, MakeRect(bullets[j].y,bullets[j].x,10,10));
		}
		for(int i=0;i<6;i++){
			if(enemies2[i].dead==0){
				if (bullets[j].isfired==1&&RectIntersection(MakeRect(bullets[j].y,bullets[j].x,10,10),MakeRect(enemies2[i].y,enemies2[i].x,20,enemies2[i].width),NULL)){
					bullets[j].isfired=0;
				}
				else if(RectIntersection(MakeRect(y,x,70,70),MakeRect(enemies2[i].y,enemies2[i].x,20,enemies2[i].width),NULL)==1){
					lives--;
					if (lives<=0){
						PlayerDied();
					}
					CanvasDrawBitmap (panelHandle, GAME_CANVAS, platform, VAL_ENTIRE_OBJECT, MakeRect(enemies2[i].y,enemies2[i].x,20,enemies2[i].width));
				}
				else{
					CanvasDrawBitmap (panelHandle, GAME_CANVAS, platform, VAL_ENTIRE_OBJECT, MakeRect(enemies2[i].y,enemies2[i].x,20,enemies2[i].width));
					if(enemies2[i].x+enemies2[i].width<0){
						enemies2[i].x+=canvas_width+rand()%canvas_width+100;
						enemies2[i].y=starty;
					}
					if(bullets[j].isfired==1&&bullets[j].x>canvas_width){  
						bullets[j].x=0;
						bullets[j].isfired=0;	
					}
				}
				enemies2[i].x+=enemies2[i].vx;
			}
			else{
				enemies2[i].dead=0;
				enemies2[i].x=canvas_width+rand()%canvas_width+100;
				enemies2[i].y=starty;
				enemies2[i].vx=-1;
				enemies2[i].width=50+startwidth;
			}
		}
	}
}

void Stage3(){
	for (int j=0;j<4;j++){
		if (bullets[j].isfired==1){
			bullets[j].x+=10;
			CanvasDrawBitmap (panelHandle, GAME_CANVAS, playerlaser, VAL_ENTIRE_OBJECT, MakeRect(bullets[j].y,bullets[j].x,10,10));
		}
		for(int i=0;i<3;i++){;//first enemy
			if(enemies1[i].dead==0){
				if (bullets[j].isfired==1&&RectIntersection(MakeRect(bullets[j].y,bullets[j].x,10,10),MakeRect(enemies1[i].y,enemies1[i].x,20,20),NULL)){
					enemies1[i].dead=1;
					BASS_ChannelPlay(Enemy1Explode,TRUE);
					Powerups();
					bullets[j].isfired=0;
					point+=100;
				}
				else if(RectIntersection(MakeRect(y,x,70,70),MakeRect(enemies1[i].y,enemies1[i].x,20,20),NULL)==1){
					lives--;
					if (lives<=0){
						PlayerDied();
					}
					enemies1[i].dead=1; 
				}
				else{
					CanvasDrawBitmap (panelHandle, GAME_CANVAS, enemy1, VAL_ENTIRE_OBJECT, MakeRect(enemies1[i].y,enemies1[i].x,20,20));
					if((enemies1[i].y-(y+30))!=0&&enemies1[i].x>x){ 
						enemies1[i].vy=-(enemies1[i].y-(y+30))/abs(enemies1[i].y-(y+30));
					}
					else{ 
						enemies1[i].vy=0;
					}
					enemies1[i].x+=enemies1[i].vx;
					enemies1[i].y+=enemies1[i].vy;
					if(enemies1[i].x+20<0){
						enemies1[i].x+=canvas_width+500;
						enemies1[i].y=starty;
					}
					if(bullets[j].isfired==1&&bullets[j].x>canvas_width){  
						bullets[j].x=0;
						bullets[j].isfired=0;	
					}
				}
			}
			else{
				enemies1[i].dead=0;
				enemies1[i].x=canvas_width+40*i;
				enemies1[i].y=starty;
				enemies1[i].vy=0;
				enemies1[i].vx=-1;
			}
			if(enemies3[i].dead==0){ //second enemy
				if (rand()%2==1&&enemies3[i].fired==0&&enemies3[i].x<=canvas_width-40){//randomise enemy firing
				 	enemies3[i].firex=enemies3[i].x;
					enemies3[i].firey=enemies3[i].y;
					enemies3[i].fired=1;
					BASS_ChannelPlay(Enemy3Firing,TRUE);
				}
				else{
					CanvasDrawBitmap (panelHandle, GAME_CANVAS, enemy3laser, VAL_ENTIRE_OBJECT, MakeRect(enemies3[i].firey+20,enemies3[i].firex,10,10));
					enemies3[i].firex-=2;
				}
				if (bullets[j].isfired==1&&RectIntersection(MakeRect(bullets[j].y,bullets[j].x,10,10),MakeRect(enemies3[i].y,enemies3[i].x,40,40),NULL)){
					enemies3[i].dead=1;
					BASS_ChannelPlay(Enemy3Explode,TRUE);
					Powerups();
					bullets[j].isfired=0;
					point+=150;
				}
				else if(RectIntersection(MakeRect(y,x,70,70),MakeRect(enemies3[i].y,enemies3[i].x,40,40),NULL)==1){
					lives--;
					if (lives<=0){
						PlayerDied();
					}
					enemies3[i].dead=1; 
				}
				else if(RectIntersection(MakeRect(y,x,70,70),MakeRect(enemies3[i].firey+20,enemies3[i].firex,10,10),NULL)==1&&enemies3[i].fired==1){
					lives--;
					if (lives<=0){
						PlayerDied();
					}
					enemies3[i].fired=0;
				}
				else{
					CanvasDrawBitmap (panelHandle, GAME_CANVAS, enemy3, VAL_ENTIRE_OBJECT, MakeRect(enemies3[i].y,enemies3[i].x,40,40));
					enemies3[i].x+=enemies3[i].vx;
					enemies3[i].y+=enemies3[i].vy;
					if(enemies3[i].x+20<0){
						enemies3[i].x+=canvas_width+500;
						enemies3[i].y=starty;
						enemies3[i].fired=0;
					}
					if(bullets[j].isfired==1&&bullets[j].x>canvas_width){  
						bullets[j].x=0;
						bullets[j].isfired=0;	
					}
				}
			}
			else{
				enemies3[i].dead=0;
				enemies3[i].x=canvas_width+40*i;
				enemies3[i].y=starty;
				enemies3[i].vx=-1;
			}
		}
	}
}

void Stage3Boss(){
	static int topattack=3,topdestroyed=0,botattack=3,botdestroyed=0;
	if (boss3.x>(canvas_width-180)){//boss entry
		if (boss3.x==canvas_width){
			BASS_ChannelStop(stage3);
			BASS_ChannelPlay(stage3boss,TRUE);
		}
		can_fire=0;
		boss3.x-=boss3.vx;
		CanvasDrawBitmap (panelHandle, GAME_CANVAS, boss3img, VAL_ENTIRE_OBJECT,MakeRect(boss3.y,boss3.x,368,180));
		SetCtrlAttribute (panelHandle, GAME_CANVAS, ATTR_PEN_FILL_COLOR, VAL_TRANSPARENT);
		CanvasDrawText (panelHandle, GAME_CANVAS, "BOSS APPROCHING", "STAGEFONT",MakeRect(canvas_height/2-50,canvas_width/2-150,VAL_KEEP_SAME_SIZE,VAL_KEEP_SAME_SIZE) , VAL_CENTER);
	}
	else{
		can_fire=1;
		for (int i=0;i<4;i++){
			if(boss3.dead==0){
				if (bullets[i].isfired==1&&RectIntersection(MakeRect(bullets[i].y,bullets[i].x,10,10),MakeRect(boss3.y+252,boss3.x+127,30,30),NULL)&&botdestroyed==0){//bottom port
					botattack--;
					bullets[i].isfired=0;
					point+=200;
					if (botattack==0){
						botdestroyed=1;
						BASS_ChannelPlay(Boss3Hatch,TRUE);
						if (topdestroyed==1){
							boss3img=boss3both;
						}
						else{
							boss3img=boss3bot;
						}
					}
					else{
						CanvasDrawBitmap (panelHandle, GAME_CANVAS, boss3img, VAL_ENTIRE_OBJECT,MakeRect(boss3.y,boss3.x,368,180));
					}
				}
				else if(RectIntersection(MakeRect(bullets[i].y,bullets[i].x,10,10),MakeRect(boss3.y+90,boss3.x+127,30,30),NULL)&&topdestroyed==0){//top port
					topattack--;
					bullets[i].isfired=0;
					point+=200;
					if (topattack==0){
						topdestroyed=1;
						BASS_ChannelPlay(Boss3Hatch,TRUE);
						if (botdestroyed==1){
							boss3img=boss3both;
						}
						else{
							boss3img=boss3top;
						}	
					}
					else{
						CanvasDrawBitmap (panelHandle, GAME_CANVAS, boss3img, VAL_ENTIRE_OBJECT,MakeRect(boss3.y,boss3.x,368,180));
					}
				}
				else if(RectIntersection(MakeRect(bullets[i].y,bullets[i].x,10,10),MakeRect(boss3.y+160,boss3.x+27,47,47),NULL)&&topdestroyed==1&&botdestroyed==1){//cockpit
					boss3.lives--;
					bullets[i].isfired=0;
					point+=200;
					if (boss3.lives==0){
						BASS_ChannelPlay(BossExplode,TRUE);
						boss3.dead=1;
					}
					else{
						CanvasDrawBitmap (panelHandle, GAME_CANVAS, boss3img, VAL_ENTIRE_OBJECT,MakeRect(boss3.y,boss3.x,368,180));
					}
				}
				else if(RectIntersection(MakeRect(bullets[i].y,bullets[i].x,10,10),MakeRect(boss3.y,boss3.x+160,canvas_height-40,300),NULL)==1){//kill bullets
 					bullets[i].isfired=0;
					CanvasDrawBitmap (panelHandle, GAME_CANVAS, boss3img, VAL_ENTIRE_OBJECT,MakeRect(boss3.y,boss3.x,368,180));
				}
				else if(RectIntersection(MakeRect(y,x,70,70),MakeRect(boss3.y,boss3.x,canvas_height-40,300),NULL)==1){//crash check
					lives--;
					if (lives<=0){
						PlayerDied();
					}
					CanvasDrawBitmap (panelHandle, GAME_CANVAS, boss3img, VAL_ENTIRE_OBJECT,MakeRect(boss3.y,boss3.x,368,180));
				}
				else if(RectIntersection(MakeRect(y,x,70,70),MakeRect(boss3topbullets[i].y,boss3topbullets[i].x,20,20),NULL)==1&&boss3topbullets[i].isfired==1){//top bullets check
					lives--;
					boss3topbullets[i].isfired=0;
					if (lives<=0){
						PlayerDied();
					}
					CanvasDrawBitmap (panelHandle, GAME_CANVAS, boss3img, VAL_ENTIRE_OBJECT,MakeRect(boss3.y,boss3.x,368,180));
				}
				else if(RectIntersection(MakeRect(y,x,70,70),MakeRect(boss3botbullets[i].y,boss3botbullets[i].x,20,20),NULL)==1&&boss3botbullets[i].isfired==1){//bot bullets check
					lives--;
					boss3botbullets[i].isfired=0;
					if (lives<=0){
						PlayerDied();
					}
					CanvasDrawBitmap (panelHandle, GAME_CANVAS, boss3img, VAL_ENTIRE_OBJECT,MakeRect(boss3.y,boss3.x,368,180));
				}
				else{
					CanvasDrawBitmap (panelHandle, GAME_CANVAS, boss3img, VAL_ENTIRE_OBJECT,MakeRect(boss3.y,boss3.x,368,180));
					if(bullets[i].isfired==1&&bullets[i].x>canvas_width){  
						bullets[i].x=0;
						bullets[i].isfired=0;	
					}
				}
				if (boss3topbullets[i].isfired==1){ //top cannon firing
					CanvasDrawBitmap (panelHandle, GAME_CANVAS, boss3bullets, VAL_ENTIRE_OBJECT, MakeRect(boss3topbullets[i].y,boss3topbullets[i].x,20,20));
					if((boss3topbullets[i].y-(y+30))!=0&&boss3topbullets[i].x>x){//close in on the player 
						boss3topbullets[i].vy=-(boss3topbullets[i].y-(y+30))/abs(boss3topbullets[i].y-(y+30));
					}
					else{ 
						boss3topbullets[i].vy=0;
					}
					boss3topbullets[i].x+=boss3topbullets[i].vx;
					boss3topbullets[i].y+=boss3topbullets[i].vy;
					if(boss3topbullets[i].x+20<0){ //check if the bullet went past the screen
						boss3topbullets[i].isfired=0;
					}
				}
				else{
					boss3topbullets[i].x=boss3.x+9;
					boss3topbullets[i].y=boss3.y+125;
					boss3topbullets[i].vy=0;
					boss3topbullets[i].vx=-4;
					if(rand()%50==1){
						boss3topbullets[i].isfired=1;
						BASS_ChannelPlay(Boss3Firing,TRUE);
					}
				}
				if (boss3botbullets[i].isfired==1){//bottom cannon firing
					CanvasDrawBitmap (panelHandle, GAME_CANVAS, boss3bullets, VAL_ENTIRE_OBJECT, MakeRect(boss3botbullets[i].y,boss3botbullets[i].x,20,20));
					if((boss3botbullets[i].y-(y+30))!=0&&boss3botbullets[i].x>x){//close in on the player 
						boss3botbullets[i].vy=-(boss3botbullets[i].y-(y+30))/abs(boss3botbullets[i].y-(y+30));
					}
					else{ 
						boss3botbullets[i].vy=0;
					}
					boss3botbullets[i].x+=boss3botbullets[i].vx;
					boss3botbullets[i].y+=boss3botbullets[i].vy;
					if(boss3botbullets[i].x+20<0){ //check if the bullet went past the screen
						boss3botbullets[i].isfired=0;
					}
				}
				else{
					boss3botbullets[i].x=boss3.x+9;
					boss3botbullets[i].y=boss3.y+231;
					boss3botbullets[i].vy=0;
					boss3botbullets[i].vx=-4;
					if(rand()%50==1){
						boss3botbullets[i].isfired=1;
						BASS_ChannelPlay(Boss3Firing,TRUE);
					}
				}
				if (bullets[i].isfired==1){//draw bullets
					bullets[i].x+=10;
					CanvasDrawBitmap (panelHandle, GAME_CANVAS, playerlaser, VAL_ENTIRE_OBJECT, MakeRect(bullets[i].y,bullets[i].x,10,10));
				}
			}
			else{
				point+=10000;
				stage=4;
				textDisplay=100;
				textDisplay=100;
				CurrentStageTime=59;
				StageTimeMin=0;
				StageTimeSec=59;
				}
				
			}
		}
}

void Stage4(){		
	for (int j=0;j<4;j++){
		if (bullets[j].isfired==1){
			bullets[j].x+=10;
			CanvasDrawBitmap (panelHandle, GAME_CANVAS, playerlaser, VAL_ENTIRE_OBJECT, MakeRect(bullets[j].y,bullets[j].x,10,10));
		}
		for(int i=0;i<8;i++){
			if(enemies4[i].dead==0){
				if (bullets[j].isfired==1&&RectIntersection(MakeRect(bullets[j].y,bullets[j].x,10,10),MakeRect(enemies4[i].y,enemies4[i].x,20,enemies4[i].width),NULL)){
					bullets[j].isfired=0;
					enemies4[i].dead=1;
					BASS_ChannelPlay(Enemy4Explode,TRUE);
					Powerups();
					point+=100;
				}
				else if(RectIntersection(MakeRect(y,x,70,70),MakeRect(enemies4[i].y,enemies4[i].x,20,enemies4[i].width),NULL)==1){
					lives--;
					if (lives<=0){
						PlayerDied();
					}
					enemies4[i].dead=1;
				}
				else{
					CanvasDrawBitmap (panelHandle, GAME_CANVAS, stage4laser, VAL_ENTIRE_OBJECT, MakeRect(enemies4[i].y,enemies4[i].x,20,enemies4[i].width));
					if(enemies4[i].x+enemies4[i].width<0){
						enemies4[i].x+=canvas_width+rand()%canvas_width+100;
						enemies4[i].y=40+starty;
					}
					if(bullets[j].isfired==1&&bullets[j].x>canvas_width){  
						bullets[j].x=0;
						bullets[j].isfired=0;	
					}
				}
				enemies4[i].x+=enemies4[i].vx;
			}
			else{
				enemies4[i].dead=0;
				enemies4[i].x=canvas_width+rand()%canvas_width+100;
				enemies4[i].y=starty;
				enemies4[i].vx=-1;
				enemies4[i].width=100;
			}
		}
	}
}

void Stage5Boss(){
	static int topattack=3,topdestroyed=0,botattack=3,botdestroyed=0,midtopattack=3,midtopdestroyed=0,midbotattack=3,midbotdestroyed=0,fire_state=0;
	if (boss5.x>(canvas_width-240)){//boss entry
		boss5.x-=boss5.vx;
		can_fire=0;
		CanvasDrawBitmap (panelHandle, GAME_CANVAS, boss5img, VAL_ENTIRE_OBJECT,MakeRect(boss5.y,boss5.x,560,240));
		SetCtrlAttribute (panelHandle, GAME_CANVAS, ATTR_PEN_FILL_COLOR, VAL_TRANSPARENT);
		CanvasDrawText (panelHandle, GAME_CANVAS, "BOSS APPROCHING", "STAGEFONT",MakeRect(canvas_height/2-50,canvas_width/2-150,VAL_KEEP_SAME_SIZE,VAL_KEEP_SAME_SIZE) , VAL_CENTER);
	}
	else{
		can_fire=1;
		for (int i=0;i<4;i++){
			if(boss5.dead==0){
				if (boss5topbullets[i].isfired==1){ //top cannon firing
						CanvasDrawBitmap (panelHandle, GAME_CANVAS, boss5bullets, VAL_ENTIRE_OBJECT, MakeRect(boss5topbullets[i].y,boss5topbullets[i].x,20,50));
					if((boss5topbullets[i].y-(y+30))!=0&&boss5topbullets[i].x>x){//close in on the player 
						boss5topbullets[i].vy=-(boss5topbullets[i].y-(y+30))/abs(boss5topbullets[i].y-(y+30));
					}
					else{ 
						boss5topbullets[i].vy=0;
					}
					boss5topbullets[i].x+=boss5topbullets[i].vx;
					boss5topbullets[i].y+=boss5topbullets[i].vy;
					if(boss5topbullets[i].x+50<0){ //check if the bullet went past the screen
						boss5topbullets[i].isfired=0;
					}
				}
				else{
					boss5topbullets[i].x=boss5.x;
					boss5topbullets[i].y=boss5.y+95;
					boss5topbullets[i].vy=0;
					boss5topbullets[i].vx=-4;
					if(rand()%50==1){
						boss5topbullets[i].isfired=1;
						BASS_ChannelPlay(Boss5Missiles,TRUE);
					}
				}
				if (boss5botbullets[i].isfired==1){//bottom cannon firing
					CanvasDrawBitmap (panelHandle, GAME_CANVAS, boss5bullets, VAL_ENTIRE_OBJECT, MakeRect(boss5botbullets[i].y,boss5botbullets[i].x,20,50));
					if((boss5botbullets[i].y-(y+30))!=0&&boss5botbullets[i].x>x){//close in on the player 
						boss5botbullets[i].vy=-(boss5botbullets[i].y-(y+30))/abs(boss5botbullets[i].y-(y+30));
					}
					else{ 
						boss5botbullets[i].vy=0;
					}
					boss5botbullets[i].x+=boss5botbullets[i].vx;
					boss5botbullets[i].y+=boss5botbullets[i].vy;
					if(boss5botbullets[i].x+50<0){ //check if the bullet went past the screen
						boss5botbullets[i].isfired=0;
					}
				}
				else{
					boss5botbullets[i].x=boss5.x;
					boss5botbullets[i].y=boss5.y+435;
					boss5botbullets[i].vy=0;
					boss5botbullets[i].vx=-4;
					if(rand()%50==1){
						boss5botbullets[i].isfired=1;
						BASS_ChannelPlay(Boss5Missiles,TRUE);
					}
				}
				if (boss5middle1[i].isfired==1){//top middle cannon firing
					CanvasDrawBitmap (panelHandle, GAME_CANVAS, boss5midbullets, VAL_ENTIRE_OBJECT, MakeRect(boss5middle1[i].y,boss5middle1[i].x,18,18));
					if (boss5middle1[i].y<=40){
						boss5middle1[i].isfired=0;
					}
					boss5middle1[i].vx=-rand()%15;
					boss5middle1[i].x+=boss5middle1[i].vx;
					boss5middle1[i].y+=boss5middle1[i].vy;
					if(boss5middle1[i].x+50<0){ //check if the bullet went past the screen
						boss5middle1[i].isfired=0;
					}
				}
				else{
					boss5middle1[i].x=boss5.x;
					boss5middle1[i].y=boss5.y+264;
					if((boss5middle1[i].y-(y+30))!=0&&boss5middle1[i].x>x){//close in on the player 
						boss5middle1[i].vy=-(boss5middle1[i].y-(y+30))/abs(boss5middle1[i].y-(y+30));
					}
					else{ 
						boss5middle1[i].vy=1;
					}
					boss5middle1[i].vx=-rand()%15;
					if(rand()%50==1){
						BASS_ChannelPlay(Boss5EnergyBall,TRUE);
						boss5middle1[i].isfired=1;
					}
				}
				if (boss5middle2[i].isfired==1){//bot middle cannon firing
					CanvasDrawBitmap (panelHandle, GAME_CANVAS, boss5midbullets, VAL_ENTIRE_OBJECT, MakeRect(boss5middle2[i].y,boss5middle2[i].x,18,18));
					if (boss5middle2[i].y<=40){
						boss5middle2[i].isfired=0;
					}
					boss5middle2[i].vx=-rand()%7;
					boss5middle2[i].x+=boss5middle2[i].vx;
					boss5middle2[i].y+=boss5middle2[i].vy;
					if(boss5middle2[i].x+50<0){ //check if the bullet went past the screen
						boss5middle2[i].isfired=0;
					}
				}
				else{
					boss5middle2[i].x=boss5.x;
					boss5middle2[i].y=boss5.y+280;
					if((boss5middle2[i].y-(y+30))!=0&&boss5middle2[i].x>x){//close in on the player 
						boss5middle2[i].vy=-(boss5middle2[i].y-(y+30))/abs(boss5middle2[i].y-(y+30));
					}
					else{ 
						boss5middle2[i].vy=-1;
					}
					boss5middle2[i].vx=-rand()%15;
					if(rand()%50==1){
						BASS_ChannelPlay(Boss5EnergyBall,TRUE);
						boss5middle2[i].isfired=1;
					}
				}
				if (bullets[i].isfired&&RectIntersection(MakeRect(bullets[i].y,bullets[i].x,10,10),MakeRect(boss5.y+517,boss5.x+20,24,24),NULL)&&botdestroyed==0){//bottom vent
					botattack--;
					bullets[i].isfired=0;
					point+=200;
					if (botattack==0){
						botdestroyed=1;
						BASS_ChannelPlay(Boss5VentExplode,TRUE);
					}
					else{
						CanvasDrawBitmap (panelHandle, GAME_CANVAS, boss5img, VAL_ENTIRE_OBJECT,MakeRect(boss5.y,boss5.x,560,240));
					}
				}
				else if(bullets[i].isfired&&RectIntersection(MakeRect(bullets[i].y,bullets[i].x,10,10),MakeRect(boss5.y+18,boss5.x+20,24,24),NULL)&&topdestroyed==0){//top vent
					topattack--;
					bullets[i].isfired=0;
					point+=200;
					if (topattack==0){
						topdestroyed=1;
						BASS_ChannelPlay(Boss5VentExplode,TRUE);
					}
					else{
						CanvasDrawBitmap (panelHandle, GAME_CANVAS, boss5img, VAL_ENTIRE_OBJECT,MakeRect(boss5.y,boss5.x,560,240));
					}
				}
				else if(bullets[i].isfired&&RectIntersection(MakeRect(bullets[i].y,bullets[i].x,10,10),MakeRect(boss5.y+202,boss5.x+20,24,24),NULL)&&midtopdestroyed==0){//top mid vent
					midtopattack--;
					bullets[i].isfired=0;
					point+=200;
					if (midtopattack==0){
						midtopdestroyed=1;
						BASS_ChannelPlay(Boss5VentExplode,TRUE);
					}
					else{
						CanvasDrawBitmap (panelHandle, GAME_CANVAS, boss5img, VAL_ENTIRE_OBJECT,MakeRect(boss5.y,boss5.x,560,240));
					}
				}
				else if(bullets[i].isfired&&RectIntersection(MakeRect(bullets[i].y,bullets[i].x,10,10),MakeRect(boss5.y+306,boss5.x+20,24,24),NULL)&&midbotdestroyed==0){//bot mid vent
					midbotattack--;
					bullets[i].isfired=0;
					point+=200;
					if (midbotattack==0){
						midbotdestroyed=1;
						BASS_ChannelPlay(Boss5VentExplode,TRUE);
					}
					else{
						CanvasDrawBitmap (panelHandle, GAME_CANVAS, boss5img, VAL_ENTIRE_OBJECT,MakeRect(boss5.y,boss5.x,560,240));
					}
				}
				else if(bullets[i].isfired&&RectIntersection(MakeRect(bullets[i].y,bullets[i].x,10,10),MakeRect(boss5.y+258,boss5.x+50,44,44),NULL)&&topdestroyed&&botdestroyed&&midbotdestroyed&&midtopdestroyed){//cockpit
					boss5.lives--;
					bullets[i].isfired=0;
					point+=200;
					if (boss5.lives<=0){
						BASS_ChannelPlay(BossExplode,TRUE);
						boss5.dead=1;
					}
					else{
						CanvasDrawBitmap (panelHandle, GAME_CANVAS, boss5img, VAL_ENTIRE_OBJECT,MakeRect(boss5.y,boss5.x,560,240));
					}
				}
				else if(RectIntersection(MakeRect(bullets[i].y,bullets[i].x,10,10),MakeRect(boss5.y,boss5.x+150,canvas_height-40,300),NULL)==1){//kill bullets
 					bullets[i].isfired=0;
					CanvasDrawBitmap (panelHandle, GAME_CANVAS, boss5img, VAL_ENTIRE_OBJECT,MakeRect(boss5.y,boss5.x,560,240));
				}
				else if(RectIntersection(MakeRect(y,x,70,70),MakeRect(boss5.y,boss5.x,canvas_height-40,300),NULL)==1){//crash check
					lives--;
					if (lives<=0){
						PlayerDied();
					}
					CanvasDrawBitmap (panelHandle, GAME_CANVAS, boss5img, VAL_ENTIRE_OBJECT,MakeRect(boss5.y,boss5.x,560,240));
				}
				else if(RectIntersection(MakeRect(y,x,70,70),MakeRect(boss5topbullets[i].y,boss5topbullets[i].x,20,50),NULL)==1&&boss5topbullets[i].isfired==1){//top bullets check
					lives--;
					boss5topbullets[i].isfired=0;
					if (lives<=0){
						PlayerDied();
					}
					CanvasDrawBitmap (panelHandle, GAME_CANVAS, boss5img, VAL_ENTIRE_OBJECT,MakeRect(boss5.y,boss5.x,560,240));
				}
				else if(RectIntersection(MakeRect(y,x,70,70),MakeRect(boss5botbullets[i].y,boss5botbullets[i].x,20,50),NULL)==1&&boss5botbullets[i].isfired==1){//bot bullets check
					lives--;
					boss5botbullets[i].isfired=0;
					if (lives<=0){
						PlayerDied();
					}
					CanvasDrawBitmap (panelHandle, GAME_CANVAS, boss5img, VAL_ENTIRE_OBJECT,MakeRect(boss5.y,boss5.x,560,240));
				}																												
				else if (RectIntersection(MakeRect(y,x,70,70),MakeRect(boss5middle1[i].y,boss5middle1[i].x,18,18),NULL)==1&&boss5middle1[i].isfired==1){//mid bot bullets check
					lives--;
					boss5middle1[i].isfired=0;
					if (lives<=0){
						PlayerDied();
					}
					CanvasDrawBitmap (panelHandle, GAME_CANVAS, boss5img, VAL_ENTIRE_OBJECT,MakeRect(boss5.y,boss5.x,560,240));
				}
				else if(RectIntersection(MakeRect(y,x,70,70),MakeRect(boss5middle2[i].y,boss5middle2[i].x,18,18),NULL)==1&&boss5middle2[i].isfired==1){//mid top bullets check
					lives--;
					boss5middle2[i].isfired=0;
					if (lives<=0){
						PlayerDied();
					}
					CanvasDrawBitmap (panelHandle, GAME_CANVAS, boss5img, VAL_ENTIRE_OBJECT,MakeRect(boss5.y,boss5.x,560,240));
				}
				else{
					CanvasDrawBitmap (panelHandle, GAME_CANVAS, boss5img, VAL_ENTIRE_OBJECT,MakeRect(boss5.y,boss5.x,560,240));
					if(bullets[i].isfired==1&&bullets[i].x>canvas_width){  
						bullets[i].x=0;
						bullets[i].isfired=0;	
					}
				}
			if (bullets[i].isfired==1){//draw bullets
				bullets[i].x+=10;
				CanvasDrawBitmap (panelHandle, GAME_CANVAS, playerlaser, VAL_ENTIRE_OBJECT, MakeRect(bullets[i].y,bullets[i].x,10,10));
			}
			if (botdestroyed==1){//draw fire
				fire_state++;
				if (fire_state>=60) fire_state=0;
				CanvasDrawBitmap (panelHandle, GAME_CANVAS, fire, MakeRect(64*(fire_state%10),64*(fire_state/10),64,64), MakeRect(boss5.y+499,boss5.x+10,64,64));
			}
			if (topdestroyed==1){
				fire_state++;
				if (fire_state>=60) fire_state=0;
				CanvasDrawBitmap (panelHandle, GAME_CANVAS, fire, MakeRect(64*(fire_state%10),64*(fire_state/10),64,64), MakeRect(boss5.y,boss5.x+10,64,64));
			}
			if (midtopdestroyed==1){
				fire_state++;
				if (fire_state>=60) fire_state=0;
				CanvasDrawBitmap (panelHandle, GAME_CANVAS, fire, MakeRect(64*(fire_state%10),64*(fire_state/10),64,64), MakeRect(boss5.y+190,boss5.x+18,64,64));
			}
			if (midbotdestroyed==1){
				fire_state++;
				if (fire_state>=60) fire_state=0;
				CanvasDrawBitmap (panelHandle, GAME_CANVAS, fire, MakeRect(64*(fire_state%10),64*(fire_state/10),64,64), MakeRect(boss5.y+309,boss5.x+18,64,64));
			}
			}
			else{
				point+=20000;
				stage=0;
				StageTimeMin=1;
				StageTimeSec=59;
				textDisplay=300;
				topattack=3;
				topdestroyed=0;
				botattack=3;
				botdestroyed=0;
				midtopattack=3;
				midtopdestroyed=0;
				midbotattack=3;
				midbotdestroyed=0;
				fire_state=0; 
				}
			if (lives<=0){
				topattack=3;
				topdestroyed=0;
				botattack=3;
				botdestroyed=0;
				midtopattack=3;
				midtopdestroyed=0;
				midbotattack=3;
				midbotdestroyed=0;
				fire_state=0;
			}
			
			}

		}
}


int CVICALLBACK game (int panel, int control, int event,
					  void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_TIMER_TICK:
			player_movment();
			CanvasStartBatchDraw (panelHandle, GAME_CANVAS);
			CanvasClear (panelHandle, GAME_CANVAS, VAL_ENTIRE_OBJECT);
			drawHud();
			Background();
			CanvasDrawBitmap (panelHandle, GAME_CANVAS, player, VAL_ENTIRE_OBJECT, MakeRect(y,x,70,70));
			starty=rand()%(540)+40;
			startwidth=rand()%400;
			if (LivesFlag==1&&PowerLivesDisplay>=0){
				PowerLivesDisplay--;
				CanvasDrawBitmap(panelHandle,GAME_CANVAS,heart,VAL_ENTIRE_OBJECT, MakeRect(0,170,30,30));
				if(PowerLivesDisplay==0){
					LivesFlag=0;
					PowerLivesDisplay=50;
				}
			}
			if (PointFlag==1&&PowerPointsDisplay>=0){
				PowerPointsDisplay--;
				SetCtrlAttribute (panelHandle, GAME_CANVAS, ATTR_PEN_FILL_COLOR, VAL_DK_GREEN);
				CanvasDrawText (panelHandle, GAME_CANVAS, "+500", "TIMEFONT",MakeRect(10,(9*canvas_width/10)-300,20,100) , VAL_CENTER);
				if(PowerPointsDisplay==0){
					PointFlag=0;
					PowerPointsDisplay=50;
				}
			}
			switch (stage){
				case 1:
					if (StageTimeSec==0&&StageTimeMin==0){
						Stage1Boss();
					}
					else if (textDisplay>0){
						if (textDisplay==100) 	BASS_ChannelPlay(stage1,TRUE);
							SetCtrlAttribute (panelHandle, GAME_CANVAS, ATTR_PEN_FILL_COLOR, VAL_TRANSPARENT);
							CanvasDrawText (panelHandle, GAME_CANVAS, "        Stage 1\n\n         MARS\n\n        Begin!", "STAGEFONT", MakeRect(canvas_height/2-100,canvas_width/2-300,VAL_KEEP_SAME_SIZE,VAL_KEEP_SAME_SIZE), VAL_CENTER);
							textDisplay--;
							can_fire=0;
							CurrentStageTime=30;
							StageTimeMin=1;
							StageTimeSec=30;
					}
					else{
						can_fire=1;
						Time();
						Stage1();
					}
					break;
				case 2:
					if (StageTimeSec==0&&StageTimeMin==0){
						BASS_ChannelStop(stage2);
						stage=3;
						point+=10000;
						StageTimeMin=1;
						StageTimeSec=30;
						CurrentStageTime=30;
						textDisplay=100;
					}
					else if (textDisplay>0){
						if (textDisplay==100) 	BASS_ChannelPlay(stage2,TRUE);
							BASS_ChannelStop(stage1boss);
							SetCtrlAttribute (panelHandle, GAME_CANVAS, ATTR_PEN_FILL_COLOR, VAL_TRANSPARENT);
							CanvasDrawText (panelHandle, GAME_CANVAS, "        Stage 2\n\nDESTROYED SPACE STATION\n\n        Begin!", "STAGEFONT" , MakeRect(canvas_height/2-100,canvas_width/2-300,VAL_KEEP_SAME_SIZE,VAL_KEEP_SAME_SIZE), VAL_CENTER);
							textDisplay--;
							can_fire=0;
							CurrentStageTime=59;
							StageTimeMin=0;
					     	StageTimeSec=59;
						 	timeCounter=0;
						}
					else{
						can_fire=1;
						Time();
						Stage2();
					}
					break;
				case 3:
					if (StageTimeSec==0&&StageTimeMin==0){
						Stage3Boss();
					}
					else if (textDisplay>0){
						if (textDisplay==100) 	BASS_ChannelPlay(stage3,TRUE);
						SetCtrlAttribute (panelHandle, GAME_CANVAS, ATTR_PEN_FILL_COLOR, VAL_TRANSPARENT);
						CanvasDrawText (panelHandle, GAME_CANVAS, "        Stage 3\n\n       ENEMY BASE\n\n        Begin!", "STAGEFONT", MakeRect(canvas_height/2-100,canvas_width/2-300,VAL_KEEP_SAME_SIZE,VAL_KEEP_SAME_SIZE), VAL_CENTER);
						textDisplay--;
						can_fire=0;
						CurrentStageTime=30;
						StageTimeMin=1;
						StageTimeSec=30;
						timeCounter=0;
					}
					else{
						can_fire=1;
						Time();
						Stage3();
					}
					break;
				case 4:
					if (StageTimeSec==0&&StageTimeMin==0){
						textDisplay=100;
						stage=5;
						BASS_ChannelStop(stage4);
						point+=10000;
						textDisplay=100;
					}
					else if (textDisplay>0){
						if (textDisplay==100) 	BASS_ChannelPlay(stage4,TRUE);
						BASS_ChannelStop(stage3boss);
						SetCtrlAttribute (panelHandle, GAME_CANVAS, ATTR_PEN_FILL_COLOR, VAL_TRANSPARENT);
						CanvasDrawText (panelHandle, GAME_CANVAS, "        Stage 4\n\n   MOTHERSHIP APPROACH\n\n        Begin!", "STAGEFONT", MakeRect(canvas_height/2-100,canvas_width/2-300,VAL_KEEP_SAME_SIZE,VAL_KEEP_SAME_SIZE), VAL_CENTER);
						textDisplay--;
						can_fire=0;
						CurrentStageTime=59;
						StageTimeMin=0;
						StageTimeSec=59;
						timeCounter=0;
					}
					else{
						can_fire=1;
						Time();
						Stage4();
					}
					break;
				case 5:
					if (textDisplay>0){
						if (textDisplay==100) 	BASS_ChannelPlay(stage5,TRUE);
						SetCtrlAttribute (panelHandle, GAME_CANVAS, ATTR_PEN_FILL_COLOR, VAL_TRANSPARENT);
						CanvasDrawText (panelHandle, GAME_CANVAS, "        Stage 5\n\n      MOTHERSHIP\n\n        Begin!", "STAGEFONT", MakeRect(canvas_height/2-100,canvas_width/2-300,VAL_KEEP_SAME_SIZE,VAL_KEEP_SAME_SIZE), VAL_CENTER);
						textDisplay--;
						StageTimeMin=0;
						StageTimeSec=0;
					}
					else{
						Stage5Boss();
					}
					break;
				case 0:
					BASS_ChannelStop(stage5);
					if (textDisplay>0){
						if (textDisplay==300) 	BASS_ChannelPlay(Victory,TRUE);
						SetCtrlAttribute (panelHandle, GAME_CANVAS, ATTR_PEN_FILL_COLOR, VAL_TRANSPARENT);
						CanvasDrawText (panelHandle, GAME_CANVAS, "        VICTORY!\n\nGame Over, Thanks for playing!", "STAGEFONT" , MakeRect(canvas_height/2-100,canvas_width/2-300,VAL_KEEP_SAME_SIZE,VAL_KEEP_SAME_SIZE), VAL_CENTER);
						textDisplay--;
						can_fire=0;
					}
					else{
						HidePanel(panelHandle);
						SetCtrlAttribute (panelHandle, GAME_TIMER, ATTR_ENABLED, 0);
						DisplayPanel(panelHandle2);
						PlayerDied();
						BASS_ChannelStop(Victory);
						BASS_ChannelPlay(titlesong,TRUE);
					}
					break;
				case -1:
					if (textDisplay>0){
						SetCtrlAttribute (panelHandle, GAME_CANVAS, ATTR_PEN_FILL_COLOR, VAL_BLACK);
						CanvasDrawRect (panelHandle, GAME_CANVAS, MakeRect(40,0,canvas_height,canvas_width), VAL_DRAW_FRAME_AND_INTERIOR);
						SetCtrlAttribute (panelHandle, GAME_CANVAS, ATTR_PEN_FILL_COLOR, VAL_TRANSPARENT);
						CanvasDrawText (panelHandle, GAME_CANVAS, "        You Lose!\n\n  Game Over, Try Again!", "STAGEFONT" , MakeRect(canvas_height/2-100,canvas_width/2-300,VAL_KEEP_SAME_SIZE,VAL_KEEP_SAME_SIZE), VAL_CENTER);
						textDisplay--;
						can_fire=0;
					}
					else{
						HidePanel(panelHandle);
						SetCtrlAttribute (panelHandle, GAME_TIMER, ATTR_ENABLED, 0);
						BASS_ChannelStop(stage1);
						BASS_ChannelStop(stage1boss);
						BASS_ChannelStop(stage2);
						BASS_ChannelStop(stage3);
						BASS_ChannelStop(stage3boss);
						BASS_ChannelStop(stage4);
						BASS_ChannelStop(stage5);
						BASS_ChannelStop(Victory);
						BASS_ChannelPlay(titlesong,TRUE);
						DisplayPanel(panelHandle2);
					}
					break;					
			}
			CanvasEndBatchDraw (panelHandle, GAME_CANVAS);
			break;
	}
	return 0;
}

int CVICALLBACK codeLink (int panel, int control, int event,
						  void *callbackData, int eventData1, int eventData2)
{   switch (event)
	{
		case EVENT_COMMIT:
		InetLaunchDefaultWebBrowser ("https://en.wikipedia.org/wiki/Konami_Code");
		break;
	}
	return 0;
}

void CVICALLBACK OpenAbout (int menuBar, int menuItem, void *callbackData,
							int panel)
{
	DisplayPanel (panelHandle3);
}


int CVICALLBACK TimeBullets (int panel, int control, int event,
							 void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_TIMER_TICK:
			if (can_fire) Fire();
			break;
	}
	return 0;
}

void CVICALLBACK OpenSettings (int menuBar, int menuItem, void *callbackData,
							   int panel)
{
	DisplayPanel(panelHandle6);
}

void CVICALLBACK OpenDemo (int menuBar, int menuItem, void *callbackData,
						   int panel)
{
	OpenDocumentInDefaultViewer ("Gladius.avi", VAL_NO_ZOOM);
}

void CVICALLBACK OpenHelp (int menuBar, int menuItem, void *callbackData,
						   int panel)
{
	OpenDocumentInDefaultViewer ("Gladius.docx", VAL_NO_ZOOM);
}

int CVICALLBACK SoundSet (int panel, int control, int event,
						  void *callbackData, int eventData1, int eventData2)
{
	double sfxlevel,musiclevel;
	switch (event)
	{
		case EVENT_VAL_CHANGED:
			GetCtrlVal (panelHandle6, SETTINGS_SFXSLIDE, &sfxlevel);
			GetCtrlVal (panelHandle6, SETTINGS_MUSICSLIDE, &musiclevel);
			//music
			BASS_ChannelSetAttribute(titlesong, BASS_ATTRIB_VOL, musiclevel);
			BASS_ChannelSetAttribute(stage1, BASS_ATTRIB_VOL, musiclevel);
			BASS_ChannelSetAttribute(stage1boss, BASS_ATTRIB_VOL, musiclevel);
			BASS_ChannelSetAttribute(stage2, BASS_ATTRIB_VOL, musiclevel);
			BASS_ChannelSetAttribute(stage3, BASS_ATTRIB_VOL, musiclevel);
			BASS_ChannelSetAttribute(stage3boss, BASS_ATTRIB_VOL, musiclevel);
			BASS_ChannelSetAttribute(stage4, BASS_ATTRIB_VOL, musiclevel);
			BASS_ChannelSetAttribute(stage5, BASS_ATTRIB_VOL, musiclevel);
			BASS_ChannelSetAttribute(Victory, BASS_ATTRIB_VOL, musiclevel);
			//sfx
			BASS_ChannelSetAttribute(playerfire, BASS_ATTRIB_VOL, sfxlevel);
			BASS_ChannelSetAttribute(Boss1Firing, BASS_ATTRIB_VOL, Boss1Firing);			
			BASS_ChannelSetAttribute(Enemy4Explode, BASS_ATTRIB_VOL, Boss1Firing);			
			BASS_ChannelSetAttribute(Enemy1Explode, BASS_ATTRIB_VOL, Boss1Firing);			
			BASS_ChannelSetAttribute(Enemy3Explode, BASS_ATTRIB_VOL, Boss1Firing);			
			BASS_ChannelSetAttribute(Cheat, BASS_ATTRIB_VOL, Boss1Firing);			
			BASS_ChannelSetAttribute(BossExplode, BASS_ATTRIB_VOL, Boss1Firing);			
			BASS_ChannelSetAttribute(Boss3Hatch, BASS_ATTRIB_VOL, Boss1Firing);			
			BASS_ChannelSetAttribute(PlayerExplode, BASS_ATTRIB_VOL, Boss1Firing);			
			BASS_ChannelSetAttribute(Boss5VentExplode, BASS_ATTRIB_VOL, Boss1Firing);			
			BASS_ChannelSetAttribute(Boss5EnergyBall, BASS_ATTRIB_VOL, Boss1Firing);			
			BASS_ChannelSetAttribute(Boss5Missiles, BASS_ATTRIB_VOL, Boss1Firing);			
			BASS_ChannelSetAttribute(Boss3Firing, BASS_ATTRIB_VOL, Boss1Firing);			
			BASS_ChannelSetAttribute(Enemy3Firing, BASS_ATTRIB_VOL, Boss1Firing);			
			break;
	}
	return 0;
}

int CVICALLBACK exitSettings (int panel, int event, void *callbackData,
							  int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_GOT_FOCUS:

			break;
		case EVENT_LOST_FOCUS:

			break;
		case EVENT_CLOSE:
 		HidePanel(panelHandle6);
			break;
	}
	return 0;
}

int CVICALLBACK ChangeDiff (int panel, int control, int event,
							void *callbackData, int eventData1, int eventData2)
{
	int diff;
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal (panelHandle6, SETTINGS_DIFFICULTY, &diff);
			switch(diff){
				case 0:
					SetCtrlVal (panelHandle4, CHEATMENU_LIVES, 30);
					break;
				case 1:
					SetCtrlVal (panelHandle4, CHEATMENU_LIVES, 5);
					break;
				case 2:
					SetCtrlVal (panelHandle4, CHEATMENU_LIVES, 1);
					SetCtrlVal (panelHandle4, CHEATMENU_POWERUPS, 0);
					break;
			}
			break;
	}
	return 0;
}
