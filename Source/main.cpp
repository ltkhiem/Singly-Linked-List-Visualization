#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <string>
#include <sstream>
#include <cmath>
#include <cstdlib>
#include "LTexture.h"
#include "LButton.h"
#include "TextBox.h"
#include "SLList.h"
#include "Queue.h"

const int SCREEN_WIDTH = 1300;
const int SCREEN_HEIGHT = 700;
const int TOTAL_BUTTONS = 1;
const int BUTTONS_SPRITE_TOTAL = 4;
const int SPACE_BETWEEN_NODE = 70, NODE_SIZE = 110;
const int VelX=25, VelY=25;

enum stepstep{
    STEP_NULL=0,
    STEP_INSERT=1,
    STEP_TRAVERSE=2,
    STEP_SEARCH=3,
    STEP_DELETE=4,
    STEP_LOAD=5,
    STEP_ADD=6,
    STEP_ADDLIST=7,
    STEP_INIT=8
};

bool init();            //Init windows
bool loadMedia();       //Load image, font
void close();           //Close window, free surface
void InitButton();      //Init size and position of button
void DrawLeftMenu();    //Draw the menu on the left
void InputFromKeyboard(SDL_Event *e);   //Handle input from keyboard
void handleInputEvent(SDL_Event* e);    //Check when user click on textbox, the start input
void InitNodePos();                     //Init positions for node
void Display_Node(int ignore=1000000);  //Displaying node to screen
void on_Add();                          //Add
void on_Insert(int pos);                //Insert to specific position
void on_LoadFile();                     //Load from file, file name : "Filelist.txt"
void on_SaveFile();                     //Save to file
void on_Find();                         //Find a number
void on_Next();                         //Go next
void on_Prev();                         //Go previous

bool startInput=false, renderText=false, NodeMoving=false, btnStart_clicked=false, btnDel_clicked=false, btnRandNum_clicked=false;
bool btnInit_clicked=false, btnRandList_clicked=false, btnInsHead_clicked=false, btnInsTail_clicked=false, btnInsRand_clicked=false;
bool btnLoadFile_clicked=false, btnSaveFile_clicked=false, btnTraverse_clicked=false, Traverse=false, btnFind_clicked=false, isFound=false, isNext=false, isPrev=false, isInit=false;
bool btnNext_clicked=false, btnPrev_clicked=false, btnStep_clicked=false, btnRunOnce_clicked=false;
bool btnDelHead_clicked=false, btnDelTail_clicked=false, btnDelNode_clicked=false;
bool is_once=true, ssAdd=false, ssAddList=false, btnAlert_visible=false;
LQueue qNode;
int empty1,empty2, insert_now=0, ignore_pos=-1, scrollY=0, add_now=0, addlist_now=-1;
int found=-1, temp1=-1, temp2=-1, pos=-1, posFound=-1 , delete_now=-1, load_now=-1, init_now=-1;
int Rendered_Node=0, gBtnNode_clickPos=0, skipPos=10000;
std::string inputText = "Value", val="Value";
stepstep ProcessType=STEP_NULL;

struct STexture
{
    SDL_Texture* gSprite=NULL;
    int w, h;
};

SDL_Texture* loadTexture( std::string path, int &w, int &h);
SDL_Texture* gBg=NULL, *gMenu=NULL;

STexture gBtnAddS1, gBtnAddS2, gBtnAddS3, gBtnRandNumS1, gBtnRandNumS2, gBtnRandNumS3, gBtnInitS1, gBtnInitS2, gBtnInitS3;
STexture gBtnRandListS1, gBtnRandListS2, gBtnRandListS3, gNodeSprite[3], gArrowSprite;
STexture gBtnInsHeadS1, gBtnInsHeadS2, gBtnInsHeadS3, gBtnInsTailS1, gBtnInsTailS2, gBtnInsTailS3, gBtnInsRandS1, gBtnInsRandS2, gBtnInsRandS3;
STexture gBtnLoadFileS1, gBtnLoadFileS2, gBtnLoadFileS3, gBtnSaveFileS1, gBtnSaveFileS2, gBtnSaveFileS3;
STexture gBtnTraverseS1, gBtnTraverseS2, gBtnTraverseS3, gBtnFindS1, gBtnFindS2, gBtnFindS3;
STexture gBtnNextS1, gBtnNextS2, gBtnNextS3, gBtnPrevS1, gBtnPrevS2, gBtnPrevS3, gBtnStepS1, gBtnRunOnceS1;
STexture gBtnDelHeadS1, gBtnDelHeadS2, gBtnDelHeadS3, gBtnDelTailS1, gBtnDelTailS2, gBtnDelTailS3, gBtnDelNodeS1, gBtnDelNodeS2, gBtnDelNodeS3;
STexture gBorder, gBorderFound, gAlert;

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;

SDL_Rect rArrow, DestR, InpValRect, rBtnAdd, rBtnRandNum, rBtnInit, rBtnRandList, rNodePos[100], rMove;
SDL_Rect rBtnInsHead, rBtnInsTail, rBtnInsRand;
SDL_Rect rBtnLoadFile, rBtnSaveFile, rBtnTraverse, rTraverse, rBtnFind, rFound, rRect;
SDL_Rect rBtnNext, rBtnPrev, rBtnRunOnce, rBtnStep;
SDL_Rect rBtnDelHead, rBtnDelTail, rBtnDelNode, rShape, rAlert;

TTF_Font *gFont = NULL, *gFontValueSmall = NULL, *gFontValueLarge=NULL;

LTexture gCommand, gValue, gNodeValue;

LButton gBtnAdd, gBtnRandNum, gBtnInit, gBtnRandList, gBtnNode[100];
LButton gBtnInsHead, gBtnInsTail, gBtnInsRand;
LButton gBtnLoadFile, gBtnSaveFile, gBtnTraverse, gBtnFind;
LButton gBtnNext, gBtnPrev, gBtnRunOnce, gBtnStep;
LButton gBtnDelHead, gBtnDelTail, gBtnDelNode, gBtnAlert;

TextBox tbxInput;


SLList MyList, TmpList;

//==================================================INIT WINDOW===================================================================
bool init()
{
	bool success = true;

	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		gWindow = SDL_CreateWindow( "Linked List Animation", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}


                // ----------------------------Init Text Input -----------------------------------------
				if( TTF_Init() == -1 )
				{
					printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
					success = false;
				}
			}
		}
	}
    DestR.x = 0;
    DestR.y = 0;
    DestR.w = SCREEN_WIDTH/7;
    DestR.h = SCREEN_HEIGHT;

    SDL_Rect tbxSize;
    tbxSize = {0, 50, DestR.w-40, 40};
    tbxInput.setSize(tbxSize);

    rShape = {DestR.w+SPACE_BETWEEN_NODE,30,NODE_SIZE,NODE_SIZE};
	return success;
}

//======================================================LOAD MEDIA================================================================

bool loadMedia()
{
	bool success = true;
	// LOAD IMAGE
    gBg = loadTexture("images/bg.jpg",empty1,empty2);
    if (gBg==NULL)
    {
        printf("Failed to load bg.jpg");
        success=false;
    }
    gMenu = loadTexture("images/menu.png",empty1,empty2);
    if (gMenu==NULL)
    {
        printf("Failed to load menu.png");
        success=false;
    }
    // LOAD FONT
    gFont = TTF_OpenFont( "fonts/Vari.ttf", 23 );
	if( gFont == NULL )
	{
		printf( "Failed to load Vari font! SDL_ttf Error: %s\n", TTF_GetError() );
		success = false;
	}
	gFontValueSmall = TTF_OpenFont( "fonts/vari.ttf", 19 );
	if( gFontValueSmall == NULL )
	{
		printf( "Failed to load Vari font! SDL_ttf Error: %s\n", TTF_GetError() );
		success = false;
	}
	gFontValueLarge = TTF_OpenFont("fonts/vari.ttf", 27);
	if (gFontValueLarge == NULL)
    {
        printf( "Failed to load Vari font! SDL_ttf Error: %s\n", TTF_GetError() );
		success = false;
    }

	else
	{
        gCommand.setFont(gFont);
		SDL_Color textColor = { 0xFF, 0xFF, 0xFF };
		if( !gCommand.loadFromRenderedText(gRenderer, "Input Value", textColor ) )
		{
			printf( "Failed to render text texture!\n" );
			success = false;
		}
		gValue.setFont(gFont);
		if( !gValue.loadFromRenderedText(gRenderer, "Value", textColor ) )
		{
			printf( "Failed to render text texture!\n" );
			success = false;
		}
		gNodeValue.setFont(gFontValueLarge);
	}

//	BUTTON LOADING
    //BtnAdd
    gBtnAddS1.gSprite = loadTexture("images/Add_up.png",gBtnAddS1.w,gBtnAddS1.h);
    if (gBtnAddS1.gSprite==NULL)
    {
        printf("Failed to load Add_up.png");
        success=false;
    }
    gBtnAddS2.gSprite = loadTexture("images/Add_down.png",gBtnAddS2.w,gBtnAddS2.h);
    if (gBtnAddS2.gSprite==NULL)
    {
        printf("Failed to load Add_down.png");
        success=false;
    }
    gBtnAddS3.gSprite = loadTexture("images/Add_over.png",gBtnAddS3.w,gBtnAddS3.h);
    if (gBtnAddS3.gSprite==NULL)
    {
        printf("Failed to load Add_over.png");
        success=false;
    }
    //Btn Rand Num
    gBtnRandNumS1.gSprite = loadTexture("images/RandNum_up.png",gBtnRandNumS1.w,gBtnRandNumS1.h);
    if (gBtnRandNumS1.gSprite==NULL)
    {
        printf("Failed to load RandNum_up.png");
        success=false;
    }
    gBtnRandNumS2.gSprite = loadTexture("images/RandNum_down.png",gBtnRandNumS2.w,gBtnRandNumS2.h);
    if (gBtnRandNumS2.gSprite==NULL)
    {
        printf("Failed to load RandNum_down.png");
        success=false;
    }
    gBtnRandNumS3.gSprite = loadTexture("images/RandNum_over.png",gBtnRandNumS3.w,gBtnRandNumS3.h);
    if (gBtnRandNumS3.gSprite==NULL)
    {
        printf("Failed to load RandNum_over.png");
        success=false;
    }

    //BtnInit
    gBtnInitS1.gSprite = loadTexture("images/Init_up.png",gBtnInitS1.w,gBtnInitS1.h);
    if (gBtnInitS1.gSprite==NULL)
    {
        printf("Failed to load Init_up.png");
        success=false;
    }
    gBtnInitS2.gSprite = loadTexture("images/Init_down.png",gBtnInitS2.w,gBtnInitS2.h);
    if (gBtnInitS2.gSprite==NULL)
    {
        printf("Failed to load Init_down.png");
        success=false;
    }
    gBtnInitS3.gSprite = loadTexture("images/Init_over.png",gBtnInitS3.w,gBtnInitS3.h);
    if (gBtnInitS3.gSprite==NULL)
    {
        printf("Failed to load Init_over.png");
        success=false;
    }
    //Btn RandList
    gBtnRandListS1.gSprite = loadTexture("images/RandList_up.png",gBtnRandListS1.w,gBtnRandListS1.h);
    if (gBtnRandListS1.gSprite==NULL)
    {
        printf("Failed to load RandList_up.png");
        success=false;
    }
    gBtnRandListS2.gSprite = loadTexture("images/RandList_down.png",gBtnRandListS2.w,gBtnRandListS2.h);
    if (gBtnRandListS2.gSprite==NULL)
    {
        printf("Failed to load RandList_down.png");
        success=false;
    }
    gBtnRandListS3.gSprite = loadTexture("images/RandList_over.png",gBtnRandListS3.w,gBtnRandListS3.h);
    if (gBtnRandListS3.gSprite==NULL)
    {
        printf("Failed to load RandList_over.png");
        success=false;
    }
        //Btn InsHead
    gBtnInsHeadS1.gSprite = loadTexture("images/InsHead_up.png",gBtnInsHeadS1.w,gBtnInsHeadS1.h);
    if (gBtnInsHeadS1.gSprite==NULL)
    {
        printf("Failed to load InsHead_up.png");
        success=false;
    }
    gBtnInsHeadS2.gSprite = loadTexture("images/InsHead_down.png",gBtnInsHeadS2.w,gBtnInsHeadS2.h);
    if (gBtnInsHeadS2.gSprite==NULL)
    {
        printf("Failed to load InsHead_down.png");
        success=false;
    }
    gBtnInsHeadS3.gSprite = loadTexture("images/InsHead_over.png",gBtnInsHeadS3.w,gBtnInsHeadS3.h);
    if (gBtnInsHeadS3.gSprite==NULL)
    {
        printf("Failed to load InsHead_over.png");
        success=false;
    }
    //Btn InsTail
    gBtnInsTailS1.gSprite = loadTexture("images/InsTail_up.png",gBtnInsTailS1.w,gBtnInsTailS1.h);
    if (gBtnInsTailS1.gSprite==NULL)
    {
        printf("Failed to load InsTail_up.png");
        success=false;
    }
    gBtnInsTailS2.gSprite = loadTexture("images/InsTail_down.png",gBtnInsTailS2.w,gBtnInsTailS2.h);
    if (gBtnInsTailS2.gSprite==NULL)
    {
        printf("Failed to load InsTail_down.png");
        success=false;
    }
    gBtnInsTailS3.gSprite = loadTexture("images/InsTail_over.png",gBtnInsTailS3.w,gBtnInsTailS3.h);
    if (gBtnInsTailS3.gSprite==NULL)
    {
        printf("Failed to load InsTail_over.png");
        success=false;
    }
        //Btn InsRand
    gBtnInsRandS1.gSprite = loadTexture("images/InsRand_up.png",gBtnInsRandS1.w,gBtnInsRandS1.h);
    if (gBtnInsRandS1.gSprite==NULL)
    {
        printf("Failed to load InsRand_up.png");
        success=false;
    }
    gBtnInsRandS2.gSprite = loadTexture("images/InsRand_down.png",gBtnInsRandS2.w,gBtnInsRandS2.h);
    if (gBtnInsRandS2.gSprite==NULL)
    {
        printf("Failed to load InsRand_down.png");
        success=false;
    }
    gBtnInsRandS3.gSprite = loadTexture("images/InsRand_over.png",gBtnInsRandS3.w,gBtnInsRandS3.h);
    if (gBtnInsRandS3.gSprite==NULL)
    {
        printf("Failed to load InsRand_over.png");
        success=false;
    }
    //Btn LoadFile
    gBtnLoadFileS1.gSprite = loadTexture("images/LoadList_up.png",gBtnLoadFileS1.w,gBtnLoadFileS1.h);
    if (gBtnLoadFileS1.gSprite==NULL)
    {
        printf("Failed to load LoadList_up.png");
        success=false;
    }
    gBtnLoadFileS2.gSprite = loadTexture("images/LoadList_down.png",gBtnLoadFileS2.w,gBtnLoadFileS2.h);
    if (gBtnLoadFileS2.gSprite==NULL)
    {
        printf("Failed to load LoadList_down.png");
        success=false;
    }
    gBtnLoadFileS3.gSprite = loadTexture("images/LoadList_over.png",gBtnLoadFileS3.w,gBtnLoadFileS3.h);
    if (gBtnLoadFileS3.gSprite==NULL)
    {
        printf("Failed to load LoasList_over.png");
        success=false;
    }
    //Btn SaveFile
    gBtnSaveFileS1.gSprite = loadTexture("images/SaveFile_up.png",gBtnSaveFileS1.w,gBtnSaveFileS1.h);
    if (gBtnSaveFileS1.gSprite==NULL)
    {
        printf("Failed to load SaveFile_up.png");
        success=false;
    }
    gBtnSaveFileS2.gSprite = loadTexture("images/SaveFile_down.png",gBtnSaveFileS2.w,gBtnSaveFileS2.h);
    if (gBtnSaveFileS2.gSprite==NULL)
    {
        printf("Failed to load SaveFile_down.png");
        success=false;
    }
    gBtnSaveFileS3.gSprite = loadTexture("images/SaveFile_over.png",gBtnSaveFileS3.w,gBtnSaveFileS3.h);
    if (gBtnSaveFileS3.gSprite==NULL)
    {
        printf("Failed to load SaveFile_over.png");
        success=false;
    }
    // Btn Traverse
    gBtnTraverseS1.gSprite = loadTexture("images/Traverse_up.png",gBtnTraverseS1.w,gBtnTraverseS1.h);
    if (gBtnTraverseS1.gSprite==NULL)
    {
        printf("Failed to load Traverse_up.png");
        success=false;
    }
    gBtnTraverseS2.gSprite = loadTexture("images/Traverse_down.png",gBtnTraverseS2.w,gBtnTraverseS2.h);
    if (gBtnTraverseS2.gSprite==NULL)
    {
        printf("Failed to load Traverse_down.png");
        success=false;
    }
    gBtnTraverseS3.gSprite = loadTexture("images/Traverse_over.png",gBtnTraverseS3.w,gBtnTraverseS3.h);
    if (gBtnTraverseS3.gSprite==NULL)
    {
        printf("Failed to load Traverse_over.png");
        success=false;
    }
    //Btn Find
    gBtnFindS1.gSprite = loadTexture("images/FindVal_up.png",gBtnFindS1.w,gBtnFindS1.h);
    if (gBtnFindS1.gSprite==NULL)
    {
        printf("Failed to load FindVal_up.png");
        success=false;
    }
    gBtnFindS2.gSprite = loadTexture("images/FindVal_down.png",gBtnFindS2.w,gBtnFindS2.h);
    if (gBtnFindS2.gSprite==NULL)
    {
        printf("Failed to load FindVal_down.png");
        success=false;
    }
    gBtnFindS3.gSprite = loadTexture("images/FindVal_over.png",gBtnFindS3.w,gBtnFindS3.h);
    if (gBtnFindS3.gSprite==NULL)
    {
        printf("Failed to load FindVal_over.png");
        success=false;
    }
        //Btn Next
    gBtnNextS1.gSprite = loadTexture("images/Next_up.png",gBtnNextS1.w,gBtnNextS1.h);
    if (gBtnNextS1.gSprite==NULL)
    {
        printf("Failed to load Next_up.png");
        success=false;
    }
    gBtnNextS2.gSprite = loadTexture("images/Next_down.png",gBtnNextS2.w,gBtnNextS2.h);
    if (gBtnNextS2.gSprite==NULL)
    {
        printf("Failed to load Next_down.png");
        success=false;
    }
    gBtnNextS3.gSprite = loadTexture("images/Next_over.png",gBtnNextS3.w,gBtnNextS3.h);
    if (gBtnNextS3.gSprite==NULL)
    {
        printf("Failed to load Next_over.png");
        success=false;
    }
        //Btn Prev
    gBtnPrevS1.gSprite = loadTexture("images/Prev_up.png",gBtnPrevS1.w,gBtnPrevS1.h);
    if (gBtnPrevS1.gSprite==NULL)
    {
        printf("Failed to load Prev_up.png");
        success=false;
    }
    gBtnPrevS2.gSprite = loadTexture("images/Prev_down.png",gBtnPrevS2.w,gBtnPrevS2.h);
    if (gBtnPrevS2.gSprite==NULL)
    {
        printf("Failed to load Prev_down.png");
        success=false;
    }
    gBtnPrevS3.gSprite = loadTexture("images/Prev_over.png",gBtnPrevS3.w,gBtnPrevS3.h);
    if (gBtnPrevS3.gSprite==NULL)
    {
        printf("Failed to load Prev_over.png");
        success=false;
    }
        //Btn DelHead
    gBtnDelHeadS1.gSprite = loadTexture("images/DeleteHead_up.png",gBtnDelHeadS1.w,gBtnDelHeadS1.h);
    if (gBtnDelHeadS1.gSprite==NULL)
    {
        printf("Failed to load DeleteHead_up.png");
        success=false;
    }
    gBtnDelHeadS2.gSprite = loadTexture("images/DeleteHead_down.png",gBtnDelHeadS2.w,gBtnDelHeadS2.h);
    if (gBtnDelHeadS2.gSprite==NULL)
    {
        printf("Failed to load DeleteHead_down.png");
        success=false;
    }
    gBtnDelHeadS3.gSprite = loadTexture("images/DeleteHead_over.png",gBtnDelHeadS3.w,gBtnDelHeadS3.h);
    if (gBtnDelHeadS3.gSprite==NULL)
    {
        printf("Failed to load DeleteHead_over.png");
        success=false;
    }
            //Btn DelTail
    gBtnDelTailS1.gSprite = loadTexture("images/DeleteTail_up.png",gBtnDelTailS1.w,gBtnDelTailS1.h);
    if (gBtnDelTailS1.gSprite==NULL)
    {
        printf("Failed to load DeleteTail_up.png");
        success=false;
    }
    gBtnDelTailS2.gSprite = loadTexture("images/DeleteTail_down.png",gBtnDelTailS2.w,gBtnDelTailS2.h);
    if (gBtnDelTailS2.gSprite==NULL)
    {
        printf("Failed to load DeleteTail_down.png");
        success=false;
    }
    gBtnDelTailS3.gSprite = loadTexture("images/DeleteTail_over.png",gBtnDelTailS3.w,gBtnDelTailS3.h);
    if (gBtnDelTailS3.gSprite==NULL)
    {
        printf("Failed to load DeleteTail_over.png");
        success=false;
    }
            //Btn DelNode
    gBtnDelNodeS1.gSprite = loadTexture("images/DeleteNode_up.png",gBtnDelNodeS1.w,gBtnDelNodeS1.h);
    if (gBtnDelNodeS1.gSprite==NULL)
    {
        printf("Failed to load DeleteNode_up.png");
        success=false;
    }
    gBtnDelNodeS2.gSprite = loadTexture("images/DeleteNode_down.png",gBtnDelNodeS2.w,gBtnDelNodeS2.h);
    if (gBtnDelNodeS2.gSprite==NULL)
    {
        printf("Failed to load DeleteNode_down.png");
        success=false;
    }
    gBtnDelNodeS3.gSprite = loadTexture("images/DeleteNode_over.png",gBtnDelNodeS3.w,gBtnDelNodeS3.h);
    if (gBtnDelNodeS3.gSprite==NULL)
    {
        printf("Failed to load DeleteNode_over.png");
        success=false;
    }
        //Btn RunOnce
    gBtnRunOnceS1.gSprite = loadTexture("images/RunOnce.png",gBtnRunOnceS1.w,gBtnRunOnceS1.h);
    if (gBtnRunOnceS1.gSprite==NULL)
    {
        printf("Failed to load RunOnce.png");
        success=false;
    }
        //Btn Step
    gBtnStepS1.gSprite = loadTexture("images/Step_down.png",gBtnStepS1.w,gBtnStepS1.h);
    if (gBtnStepS1.gSprite==NULL)
    {
        printf("Failed to load Step_down.png");
        success=false;
    }
    gBorder.gSprite = loadTexture("images/Border.png",gBorder.w,gBorder.h);
    if (gBorder.gSprite==NULL)
    {
        printf("Failed to load Border.png");
        success=false;
    }
    gBorderFound.gSprite = loadTexture("images/Border_Found.png",gBorderFound.w,gBorderFound.h);
    if (gBorderFound.gSprite==NULL)
    {
        printf("Failed to load Border_Found.png");
        success=false;
    }

//  LINKED LIST'S NODE TEXTURE
    gNodeSprite[1].gSprite = loadTexture("images/node_up.png",gNodeSprite[1].w,gNodeSprite[1].h);
    if (gNodeSprite[1].gSprite==NULL)
    {
        printf("Failed to load node_up.png");
        success = false;
    }
    gNodeSprite[2].gSprite = loadTexture("images/node_down.png",gNodeSprite[2].w, gNodeSprite[2].h);
    if (gNodeSprite[2].gSprite==NULL)
    {
        printf("Failed to load node_down.png");
        success = false;
    }
    gArrowSprite.gSprite = loadTexture("images/arrow.png", gArrowSprite.w, gArrowSprite.h);
    if (gArrowSprite.gSprite==NULL)
    {
        printf("Failed to load arrow.png");
        success = false;
    }
    gAlert.gSprite = loadTexture("images/complete.png", gAlert.w, gAlert.h);
    if (gAlert.gSprite==NULL)
    {
        printf("Failed to load complete.png");
        success = false;
    }

    InitNodePos();
    InitButton();


	return success;
}

//==========================================================CLOSE PROGRAM=========================================================

void close()
{
    gCommand.free();

    TTF_CloseFont( gFont );
    gFont = NULL;
    gCommand.setFont(NULL);

	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

//====================================================LOAD TEXTURE================================================================

SDL_Texture* loadTexture( std::string path, int &w, int &h )
{
	SDL_Texture* newTexture = NULL;

	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );      //Load image to surface
	if( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
	}
	else
	{
        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
		if( newTexture == NULL )
		{
			printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		}
		else
        {
            w = loadedSurface->w;
            h = loadedSurface->h;
        }

		SDL_FreeSurface( loadedSurface );
	}

	return newTexture;
}

//====================================================DRAW LEFT MENU==============================================================
void DrawLeftMenu()
{
    //MENU BACKGROUND
    SDL_RenderCopy(gRenderer,gMenu,NULL,&DestR);

    //DISPLAY TEXT
    gCommand.render(gRenderer, 15, 10 );

    //DRAW
    InpValRect = {0, 50, DestR.w-30, 40};
    SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
    SDL_RenderDrawRect(gRenderer,&InpValRect);

}

//===================================================BUTTONS HANDLE================================================================
void HandleButtonsEvent(SDL_Event *e)
{
    switch (gBtnAdd.GetCurrentSprite())
    {
        case gBtnAdd.BUTTON_SPRITE_MOUSE_OVER_MOTION  : SDL_RenderCopy(gRenderer,gBtnAddS3.gSprite,NULL,&rBtnAdd); break;
        case gBtnAdd.BUTTON_SPRITE_MOUSE_DOWN         : {
                                                                SDL_RenderCopy(gRenderer,gBtnAddS2.gSprite,NULL,&rBtnAdd);
                                                                if (!btnStart_clicked)
                                                                {
                                                                    posFound=-1;
                                                                    isFound=false;
                                                                    found=-1;
                                                                    temp2=-1;
                                                                    if (is_once)
                                                                        on_Add();
                                                                    else
                                                                    {
                                                                        ProcessType=STEP_ADD;
                                                                        add_now=0;
                                                                    }
                                                                    btnStart_clicked=true;
                                                                }
                                                                break;
                                                          }
        case gBtnAdd.BUTTON_SPRITE_MOUSE_UP           : {
                                                                SDL_RenderCopy(gRenderer,gBtnAddS1.gSprite,NULL,&rBtnAdd);
                                                                btnStart_clicked=false;
                                                                break;
                                                          }
        case gBtnAdd.BUTTON_SPRITE_MOUSE_OUT          : SDL_RenderCopy(gRenderer,gBtnAddS1.gSprite,NULL,&rBtnAdd); break;
    }

    switch (gBtnRandNum.GetCurrentSprite())
    {
        case gBtnRandNum.BUTTON_SPRITE_MOUSE_OVER_MOTION  : SDL_RenderCopy(gRenderer,gBtnRandNumS3.gSprite,NULL,&rBtnRandNum); break;
        case gBtnRandNum.BUTTON_SPRITE_MOUSE_DOWN         : {
                                                                SDL_RenderCopy(gRenderer,gBtnRandNumS2.gSprite,NULL,&rBtnRandNum);
                                                                if (!btnRandNum_clicked)
                                                                {
                                                                    posFound=-1;
                                                                    pos=-1;
                                                                    isFound=false;
                                                                    found=-1;
                                                                    temp2=-1;
                                                                    int x=rand()%1000000000;
                                                                    std::stringstream ss;
                                                                    ss << x;
                                                                    inputText = ss.str();
                                                                    renderText=true;
                                                                    if (is_once) on_Add();
                                                                    else
                                                                        {
                                                                            add_now=0;
                                                                            ProcessType=STEP_ADD;
                                                                        }
                                                                    btnRandNum_clicked=true;
                                                                }
                                                                break;
                                                          }
        case gBtnRandNum.BUTTON_SPRITE_MOUSE_UP           : {
                                                                SDL_RenderCopy(gRenderer,gBtnRandNumS1.gSprite,NULL,&rBtnRandNum);
                                                                btnRandNum_clicked=false;
                                                                break;
                                                          }
        case gBtnRandNum.BUTTON_SPRITE_MOUSE_OUT          : SDL_RenderCopy(gRenderer,gBtnRandNumS1.gSprite,NULL,&rBtnRandNum); break;
    }

    switch (gBtnInit.GetCurrentSprite())
    {
        case gBtnInit.BUTTON_SPRITE_MOUSE_OVER_MOTION  : SDL_RenderCopy(gRenderer,gBtnInitS3.gSprite,NULL,&rBtnInit); break;
        case gBtnInit.BUTTON_SPRITE_MOUSE_DOWN         : {
                                                                SDL_RenderCopy(gRenderer,gBtnInitS2.gSprite,NULL,&rBtnInit);
                                                                if (!btnInit_clicked)
                                                                {
                                                                    posFound=-1;
                                                                    pos=-1;
                                                                    isFound=false;
                                                                    found=-1;
                                                                    temp2=-1;

                                                                    if (is_once==true){
                                                                        Rendered_Node=0;
                                                                        MyList.RemoveAll();
                                                                        MyList.Init();
                                                                    }
                                                                    else{
                                                                        if (!MyList.isEmpty()){
                                                                            TmpList.RemoveAll();
                                                                            TmpList.Init();
                                                                            gBtnNode_clickPos=0;
                                                                            init_now=0;
                                                                            ProcessType=STEP_INIT;
                                                                        }
                                                                    }
                                                                    btnInit_clicked=true;
                                                                }
                                                                break;
                                                          }
        case gBtnInit.BUTTON_SPRITE_MOUSE_UP           : {
                                                                SDL_RenderCopy(gRenderer,gBtnInitS1.gSprite,NULL,&rBtnInit);
                                                                btnInit_clicked=false;
                                                                break;
                                                          }
        case gBtnInit.BUTTON_SPRITE_MOUSE_OUT          : SDL_RenderCopy(gRenderer,gBtnInitS1.gSprite,NULL,&rBtnInit); break;
    }

    switch (gBtnRandList.GetCurrentSprite())
    {
        case gBtnRandList.BUTTON_SPRITE_MOUSE_OVER_MOTION  : SDL_RenderCopy(gRenderer,gBtnRandListS3.gSprite,NULL,&rBtnRandList); break;
        case gBtnRandList.BUTTON_SPRITE_MOUSE_DOWN         : {
                                                                SDL_RenderCopy(gRenderer,gBtnRandListS2.gSprite,NULL,&rBtnRandList);
                                                                if (!btnRandList_clicked)
                                                                {
                                                                    posFound=-1;
                                                                    pos=-1;
                                                                    isFound=false;
                                                                    found=-1;
                                                                    temp2=-1;
                                                                    Rendered_Node=0;
                                                                    MyList.RemoveAll();
                                                                    MyList.Init();
                                                                    for (int i=1; i<=15; i++)
                                                                    {
                                                                        int x=rand()%1000000000;
                                                                        std::stringstream ss;
                                                                        ss << x;
                                                                        inputText = ss.str();
                                                                        renderText=true;
                                                                        if (is_once) on_Add();
                                                                        else MyList.Append(inputText);
                                                                    }
                                                                    if (!is_once)
                                                                    {
                                                                        ProcessType=STEP_ADDLIST;
                                                                        addlist_now=0;
                                                                    }
                                                                    btnRandList_clicked=true;
                                                                }
                                                                break;
                                                          }
        case gBtnRandList.BUTTON_SPRITE_MOUSE_UP           : {
                                                                SDL_RenderCopy(gRenderer,gBtnRandListS1.gSprite,NULL,&rBtnRandList);
                                                                btnRandList_clicked=false;
                                                                break;
                                                          }
        case gBtnRandList.BUTTON_SPRITE_MOUSE_OUT          : SDL_RenderCopy(gRenderer,gBtnRandListS1.gSprite,NULL,&rBtnRandList); break;
    }

    switch (gBtnInsHead.GetCurrentSprite())
    {
        case gBtnInsHead.BUTTON_SPRITE_MOUSE_OVER_MOTION  : SDL_RenderCopy(gRenderer,gBtnInsHeadS3.gSprite,NULL,&rBtnInsHead); break;
        case gBtnInsHead.BUTTON_SPRITE_MOUSE_DOWN         : {
                                                                SDL_RenderCopy(gRenderer,gBtnInsHeadS2.gSprite,NULL,&rBtnInsHead);
                                                                if (!btnInsHead_clicked)
                                                                {
                                                                    posFound=-1;
                                                                    isFound=false;
                                                                    found=-1;
                                                                    temp2=-1;
                                                                    if (is_once)
                                                                    {
                                                                        on_Insert(0);
                                                                    }
                                                                    else
                                                                    {
                                                                        gBtnNode_clickPos=0;
                                                                        insert_now=0;
                                                                        ProcessType=STEP_INSERT;
                                                                    }
                                                                    btnInsHead_clicked=true;
                                                                }
                                                                break;
                                                          }
        case gBtnInsHead.BUTTON_SPRITE_MOUSE_UP           : {
                                                                SDL_RenderCopy(gRenderer,gBtnInsHeadS1.gSprite,NULL,&rBtnInsHead);
                                                                btnInsHead_clicked=false;
                                                                break;
                                                          }
        case gBtnInsHead.BUTTON_SPRITE_MOUSE_OUT          : SDL_RenderCopy(gRenderer,gBtnInsHeadS1.gSprite,NULL,&rBtnInsHead); break;
    }

    switch (gBtnInsTail.GetCurrentSprite())
    {
        case gBtnInsTail.BUTTON_SPRITE_MOUSE_OVER_MOTION  : SDL_RenderCopy(gRenderer,gBtnInsTailS3.gSprite,NULL,&rBtnInsTail); break;
        case gBtnInsTail.BUTTON_SPRITE_MOUSE_DOWN         : {
                                                                SDL_RenderCopy(gRenderer,gBtnInsTailS2.gSprite,NULL,&rBtnInsTail);
                                                                if (!btnInsTail_clicked)
                                                                {
                                                                    pos=-1;
                                                                    posFound=-1;
                                                                    isFound=false;
                                                                    found=-1;
                                                                    temp2=-1;
                                                                    if (is_once)
                                                                    {

                                                                        on_Add();
                                                                    }
                                                                    else
                                                                    {
                                                                        insert_now=15;
                                                                        ProcessType=STEP_INSERT;
                                                                    }
                                                                    btnInsTail_clicked=true;
                                                                }
                                                                break;
                                                          }
        case gBtnInsTail.BUTTON_SPRITE_MOUSE_UP           : {
                                                                SDL_RenderCopy(gRenderer,gBtnInsTailS1.gSprite,NULL,&rBtnInsTail);
                                                                btnInsTail_clicked=false;
                                                                break;
                                                          }
        case gBtnInsTail.BUTTON_SPRITE_MOUSE_OUT          : SDL_RenderCopy(gRenderer,gBtnInsTailS1.gSprite,NULL,&rBtnInsTail); break;
    }

    switch (gBtnInsRand.GetCurrentSprite())
    {
        case gBtnInsRand.BUTTON_SPRITE_MOUSE_OVER_MOTION  : SDL_RenderCopy(gRenderer,gBtnInsRandS3.gSprite,NULL,&rBtnInsRand); break;
        case gBtnInsRand.BUTTON_SPRITE_MOUSE_DOWN         : {
                                                                SDL_RenderCopy(gRenderer,gBtnInsRandS2.gSprite,NULL,&rBtnInsRand);
                                                                if (!btnInsRand_clicked)
                                                                {
                                                                    pos=-1;
                                                                    posFound=-1;
                                                                    isFound=false;
                                                                    found=-1;
                                                                    temp2=-1;
                                                                    if (is_once)
                                                                    {

                                                                        on_Insert(gBtnNode_clickPos);
                                                                    }
                                                                    else
                                                                    {
                                                                        insert_now=0;
                                                                        ProcessType=STEP_INSERT;
                                                                    }
                                                                    btnInsRand_clicked=true;
                                                                }
                                                                break;
                                                          }
        case gBtnInsRand.BUTTON_SPRITE_MOUSE_UP           : {
                                                                SDL_RenderCopy(gRenderer,gBtnInsRandS1.gSprite,NULL,&rBtnInsRand);
                                                                btnInsRand_clicked=false;
                                                                break;
                                                          }
        case gBtnInsRand.BUTTON_SPRITE_MOUSE_OUT          : SDL_RenderCopy(gRenderer,gBtnInsRandS1.gSprite,NULL,&rBtnInsRand); break;
    }

    switch (gBtnLoadFile.GetCurrentSprite())
    {

        case gBtnLoadFile.BUTTON_SPRITE_MOUSE_OVER_MOTION  : SDL_RenderCopy(gRenderer,gBtnLoadFileS3.gSprite,NULL,&rBtnLoadFile); break;
        case gBtnLoadFile.BUTTON_SPRITE_MOUSE_DOWN         : {
                                                                SDL_RenderCopy(gRenderer,gBtnLoadFileS2.gSprite,NULL,&rBtnLoadFile);
                                                                if (!btnLoadFile_clicked)
                                                                {
                                                                    pos=-1;
                                                                    posFound=-1;
                                                                    isFound=false;
                                                                    found=-1;
                                                                    temp2=-1;
                                                                    if (is_once==true)
                                                                        on_LoadFile();
                                                                    else{
                                                                        on_LoadFile();
                                                                        load_now=0;
                                                                        ProcessType=STEP_LOAD;
                                                                    }
                                                                    btnLoadFile_clicked=true;
                                                                }
                                                                break;
                                                          }
        case gBtnLoadFile.BUTTON_SPRITE_MOUSE_UP           : {
                                                                SDL_RenderCopy(gRenderer,gBtnLoadFileS1.gSprite,NULL,&rBtnLoadFile);
                                                                btnLoadFile_clicked=false;
                                                                break;
                                                          }
        case gBtnLoadFile.BUTTON_SPRITE_MOUSE_OUT          : SDL_RenderCopy(gRenderer,gBtnLoadFileS1.gSprite,NULL,&rBtnLoadFile); break;
    }

    switch (gBtnSaveFile.GetCurrentSprite())
    {

        case gBtnSaveFile.BUTTON_SPRITE_MOUSE_OVER_MOTION  : SDL_RenderCopy(gRenderer,gBtnSaveFileS3.gSprite,NULL,&rBtnSaveFile); break;
        case gBtnSaveFile.BUTTON_SPRITE_MOUSE_DOWN         : {
                                                                SDL_RenderCopy(gRenderer,gBtnSaveFileS2.gSprite,NULL,&rBtnSaveFile);
                                                                if (!btnSaveFile_clicked)
                                                                {
                                                                    pos=-1;
                                                                    posFound=-1;
                                                                    isFound=false;
                                                                    found=-1;
                                                                    temp2=-1;
                                                                    on_SaveFile();
                                                                    btnSaveFile_clicked=true;
                                                                }
                                                                break;
                                                          }
        case gBtnSaveFile.BUTTON_SPRITE_MOUSE_UP           : {
                                                                SDL_RenderCopy(gRenderer,gBtnSaveFileS1.gSprite,NULL,&rBtnSaveFile);
                                                                btnSaveFile_clicked=false;
                                                                break;
                                                            }
        case gBtnSaveFile.BUTTON_SPRITE_MOUSE_OUT          : SDL_RenderCopy(gRenderer,gBtnSaveFileS1.gSprite,NULL,&rBtnSaveFile); break;
    }

    switch (gBtnTraverse.GetCurrentSprite())
    {

        case gBtnTraverse.BUTTON_SPRITE_MOUSE_OVER_MOTION  : SDL_RenderCopy(gRenderer,gBtnTraverseS3.gSprite,NULL,&rBtnTraverse); break;
        case gBtnTraverse.BUTTON_SPRITE_MOUSE_DOWN         : {
                                                                SDL_RenderCopy(gRenderer,gBtnTraverseS2.gSprite,NULL,&rBtnTraverse);
                                                                if (!btnTraverse_clicked)
                                                                {
                                                                    //on_Traverse();
                                                                    pos=0;
                                                                    posFound=-1;
                                                                    Traverse=true;
                                                                    btnTraverse_clicked=true;
                                                                }
                                                                break;
                                                          }
        case gBtnTraverse.BUTTON_SPRITE_MOUSE_UP           : {
                                                                SDL_RenderCopy(gRenderer,gBtnTraverseS1.gSprite,NULL,&rBtnTraverse);
                                                                btnTraverse_clicked=false;
                                                                break;
                                                            }
        case gBtnTraverse.BUTTON_SPRITE_MOUSE_OUT          : SDL_RenderCopy(gRenderer,gBtnTraverseS1.gSprite,NULL,&rBtnTraverse); break;
    }

    switch (gBtnFind.GetCurrentSprite())
    {

        case gBtnFind.BUTTON_SPRITE_MOUSE_OVER_MOTION  : SDL_RenderCopy(gRenderer,gBtnFindS3.gSprite,NULL,&rBtnFind); break;
        case gBtnFind.BUTTON_SPRITE_MOUSE_DOWN         : {
                                                                SDL_RenderCopy(gRenderer,gBtnFindS2.gSprite,NULL,&rBtnFind);
                                                                if (!btnFind_clicked)
                                                                {
                                                                    pos=-1;
                                                                    posFound=-1;
                                                                    isFound=true;
                                                                    temp2=-1;
                                                                    on_Find();
                                                                    btnFind_clicked=true;
                                                                }
                                                                break;
                                                          }
        case gBtnFind.BUTTON_SPRITE_MOUSE_UP           : {
                                                                SDL_RenderCopy(gRenderer,gBtnFindS1.gSprite,NULL,&rBtnFind);
                                                                btnFind_clicked=false;
                                                                break;
                                                            }
        case gBtnFind.BUTTON_SPRITE_MOUSE_OUT          : SDL_RenderCopy(gRenderer,gBtnFindS1.gSprite,NULL,&rBtnFind); break;
    }

    if (is_once)
    {
        switch (gBtnRunOnce.GetCurrentSprite())
        {

            case gBtnRunOnce.BUTTON_SPRITE_MOUSE_OVER_MOTION  : SDL_RenderCopy(gRenderer,gBtnRunOnceS1.gSprite,NULL,&rBtnRunOnce); break;
            case gBtnRunOnce.BUTTON_SPRITE_MOUSE_DOWN         : {
                                                                    SDL_RenderCopy(gRenderer,gBtnRunOnceS1.gSprite,NULL,&rBtnRunOnce);
                                                                    if (!btnRunOnce_clicked)
                                                                    {
                                                                        pos=-1;
                                                                        posFound=-1;
                                                                        found=-1;
                                                                        isFound=false;
                                                                        is_once=false;
                                                                        btnRunOnce_clicked=true;
                                                                    }
                                                                    break;
                                                              }
            case gBtnRunOnce.BUTTON_SPRITE_MOUSE_UP           : {
                                                                    SDL_RenderCopy(gRenderer,gBtnRunOnceS1.gSprite,NULL,&rBtnRunOnce);
                                                                    btnRunOnce_clicked=false;
                                                                    break;
                                                                }
            case gBtnRunOnce.BUTTON_SPRITE_MOUSE_OUT          : SDL_RenderCopy(gRenderer,gBtnRunOnceS1.gSprite,NULL,&rBtnRunOnce); break;
        }
    }
    else
    {
        switch (gBtnStep.GetCurrentSprite())
        {

            case gBtnStep.BUTTON_SPRITE_MOUSE_OVER_MOTION  : SDL_RenderCopy(gRenderer,gBtnStepS1.gSprite,NULL,&rBtnStep); break;
            case gBtnStep.BUTTON_SPRITE_MOUSE_DOWN         : {
                                                                    SDL_RenderCopy(gRenderer,gBtnStepS1.gSprite,NULL,&rBtnStep);
                                                                    if (!btnStep_clicked)
                                                                    {
                                                                        pos=-1;
                                                                        posFound=-1;
                                                                        found=-1;
                                                                        is_once=true;
                                                                        isFound=false;
                                                                        btnStep_clicked=true;
                                                                    }
                                                                    break;
                                                              }
            case gBtnStep.BUTTON_SPRITE_MOUSE_UP           : {
                                                                    SDL_RenderCopy(gRenderer,gBtnStepS1.gSprite,NULL,&rBtnStep);
                                                                    btnStep_clicked=false;
                                                                    break;
                                                                }
            case gBtnStep.BUTTON_SPRITE_MOUSE_OUT          : SDL_RenderCopy(gRenderer,gBtnStepS1.gSprite,NULL,&rBtnStep); break;
        }

        switch (gBtnNext.GetCurrentSprite())
        {

            case gBtnNext.BUTTON_SPRITE_MOUSE_OVER_MOTION  : SDL_RenderCopy(gRenderer,gBtnNextS3.gSprite,NULL,&rBtnNext); break;
            case gBtnNext.BUTTON_SPRITE_MOUSE_DOWN         : {
                                                                    SDL_RenderCopy(gRenderer,gBtnNextS2.gSprite,NULL,&rBtnNext);
                                                                    if (!btnNext_clicked)
                                                                    {
                                                                        isNext=true;
                                                                        if (pos==-1 && posFound==-1)
                                                                            isNext=false;
                                                                        on_Next();
                                                                        btnNext_clicked=true;
                                                                    }
                                                                    break;
                                                              }
            case gBtnNext.BUTTON_SPRITE_MOUSE_UP           : {
                                                                    SDL_RenderCopy(gRenderer,gBtnNextS1.gSprite,NULL,&rBtnNext);
                                                                    btnNext_clicked=false;
                                                                    break;
                                                                }
            case gBtnNext.BUTTON_SPRITE_MOUSE_OUT          : SDL_RenderCopy(gRenderer,gBtnNextS1.gSprite,NULL,&rBtnNext); break;
        }

        switch (gBtnPrev.GetCurrentSprite())
        {

            case gBtnPrev.BUTTON_SPRITE_MOUSE_OVER_MOTION  : SDL_RenderCopy(gRenderer,gBtnPrevS3.gSprite,NULL,&rBtnPrev); break;
            case gBtnPrev.BUTTON_SPRITE_MOUSE_DOWN         : {
                                                                    SDL_RenderCopy(gRenderer,gBtnPrevS2.gSprite,NULL,&rBtnPrev);
                                                                    if (!btnPrev_clicked)
                                                                    {
                                                                        isPrev=true;
                                                                        if (pos==-1 && posFound==-1)
                                                                            isPrev=false;
                                                                        on_Prev();
                                                                        btnPrev_clicked=true;
                                                                    }
                                                                    break;
                                                              }
            case gBtnPrev.BUTTON_SPRITE_MOUSE_UP           : {
                                                                    SDL_RenderCopy(gRenderer,gBtnPrevS1.gSprite,NULL,&rBtnPrev);
                                                                    btnPrev_clicked=false;
                                                                    break;
                                                                }
            case gBtnPrev.BUTTON_SPRITE_MOUSE_OUT          : SDL_RenderCopy(gRenderer,gBtnPrevS1.gSprite,NULL,&rBtnPrev); break;
        }
    }

    switch (gBtnDelHead.GetCurrentSprite())
    {

        case gBtnDelHead.BUTTON_SPRITE_MOUSE_OVER_MOTION  : SDL_RenderCopy(gRenderer,gBtnDelHeadS3.gSprite,NULL,&rBtnDelHead); break;
        case gBtnDelHead.BUTTON_SPRITE_MOUSE_DOWN         : {
                                                                SDL_RenderCopy(gRenderer,gBtnDelHeadS2.gSprite,NULL,&rBtnDelHead);
                                                                if (!btnDelHead_clicked)
                                                                {
                                                                    pos=-1;
                                                                    posFound=-1;
                                                                    isFound=false;
                                                                    found=-1;
                                                                    temp2=-1;
                                                                    if (is_once)
                                                                    {
                                                                        if (!MyList.isEmpty()){
                                                                            MyList.DeleteX(0);
                                                                            Rendered_Node--;
                                                                        }
                                                                    }
                                                                    else
                                                                    {
                                                                        //insert_now=0;
                                                                        if (!MyList.isEmpty()){
                                                                            gBtnNode_clickPos=0;
                                                                            delete_now=0;
                                                                            ProcessType=STEP_DELETE;
                                                                        }
                                                                    }
                                                                    btnDelHead_clicked=true;
                                                                }
                                                                break;
                                                          }
        case gBtnDelHead.BUTTON_SPRITE_MOUSE_UP           : {
                                                                SDL_RenderCopy(gRenderer,gBtnDelHeadS1.gSprite,NULL,&rBtnDelHead);
                                                                btnDelHead_clicked=false;
                                                                break;
                                                            }
        case gBtnDelHead.BUTTON_SPRITE_MOUSE_OUT          : SDL_RenderCopy(gRenderer,gBtnDelHeadS1.gSprite,NULL,&rBtnDelHead); break;
    }
    switch (gBtnDelTail.GetCurrentSprite())
    {

        case gBtnDelTail.BUTTON_SPRITE_MOUSE_OVER_MOTION  : SDL_RenderCopy(gRenderer,gBtnDelTailS3.gSprite,NULL,&rBtnDelTail); break;
        case gBtnDelTail.BUTTON_SPRITE_MOUSE_DOWN         : {
                                                                SDL_RenderCopy(gRenderer,gBtnDelTailS2.gSprite,NULL,&rBtnDelTail);
                                                                if (!btnDelTail_clicked)
                                                                {
                                                                    pos=-1;
                                                                    posFound=-1;
                                                                    isFound=false;
                                                                    found=-1;
                                                                    temp2=-1;
                                                                    if (is_once)
                                                                    {
                                                                        if (!MyList.isEmpty()){
                                                                            MyList.DeleteX(MyList.GetSize()-1);
                                                                            Rendered_Node--;
                                                                        }
                                                                    }
                                                                    else
                                                                    {
                                                                        //insert_now=0;
                                                                        if (!MyList.isEmpty()){
                                                                            gBtnNode_clickPos=MyList.GetSize()-1;
                                                                            delete_now=0;
                                                                            ProcessType=STEP_DELETE;
                                                                        }
                                                                    }
                                                                    btnDelTail_clicked=true;
                                                                }
                                                                break;
                                                          }
        case gBtnDelTail.BUTTON_SPRITE_MOUSE_UP           : {
                                                                SDL_RenderCopy(gRenderer,gBtnDelTailS1.gSprite,NULL,&rBtnDelTail);
                                                                btnDelTail_clicked=false;
                                                                break;
                                                            }
        case gBtnDelTail.BUTTON_SPRITE_MOUSE_OUT          : SDL_RenderCopy(gRenderer,gBtnDelTailS1.gSprite,NULL,&rBtnDelTail); break;
    }
    switch (gBtnDelNode.GetCurrentSprite())
    {

        case gBtnDelNode.BUTTON_SPRITE_MOUSE_OVER_MOTION  : SDL_RenderCopy(gRenderer,gBtnDelNodeS3.gSprite,NULL,&rBtnDelNode); break;
        case gBtnDelNode.BUTTON_SPRITE_MOUSE_DOWN         : {
                                                                SDL_RenderCopy(gRenderer,gBtnDelNodeS2.gSprite,NULL,&rBtnDelNode);
                                                                if (!btnDelNode_clicked)
                                                                {
                                                                    pos=-1;
                                                                    posFound=-1;
                                                                    isFound=false;
                                                                    found=-1;
                                                                    temp2=-1;
                                                                    if (is_once)
                                                                    {
                                                                        if (!MyList.isEmpty()){
                                                                            if (gBtnNode_clickPos<MyList.GetSize()){
                                                                                MyList.DeleteX(gBtnNode_clickPos);
                                                                                Rendered_Node--;
                                                                            }
                                                                        }
                                                                    }
                                                                    else
                                                                    {
                                                                        if (!MyList.isEmpty()){
                                                                            gBtnNode_clickPos=0;
                                                                            delete_now=0;
                                                                            ProcessType=STEP_DELETE;
                                                                        }
                                                                    }
                                                                    btnDelNode_clicked=true;
                                                                }
                                                                break;
                                                          }
        case gBtnDelNode.BUTTON_SPRITE_MOUSE_UP           : {
                                                                SDL_RenderCopy(gRenderer,gBtnDelNodeS1.gSprite,NULL,&rBtnDelNode);
                                                                btnDelNode_clicked=false;
                                                                break;
                                                            }
        case gBtnDelNode.BUTTON_SPRITE_MOUSE_OUT          : SDL_RenderCopy(gRenderer,gBtnDelNodeS1.gSprite,NULL,&rBtnDelNode); break;
    }


        switch (gBtnAlert.GetCurrentSprite())
        {

            case gBtnAlert.BUTTON_SPRITE_MOUSE_DOWN     :    {
                                                                posFound=-1;
                                                                btnAlert_visible = false;
                                                                break;
                                                            }
        }

}

//====================================================INPUT TEXTBOX================================================================

void InputFromKeyboard(SDL_Event *e)
{
    SDL_Color textColor = { 0xFF, 0xFF, 0xFF };
    if(tbxInput.getCurrentState()==tbxInput.MOUSE_DOWN)
    {
        found=-1;
        posFound=-1;
        if (!startInput)
        {
            SDL_StartTextInput();
            startInput=true;
        }
    }

    if (tbxInput.getCurrentState()==tbxInput.MOUSE_OUT_DOWN || gBtnAdd.Clicked() || e->key.keysym.sym==SDLK_RETURN)
    {
        if (startInput)
        {
            SDL_StopTextInput();
            startInput=false;
        }
    }

    //Render text if have changes
    if( renderText )
    {
        if( inputText != "" )
        {
            gValue.loadFromRenderedText(gRenderer, inputText.c_str(), textColor );
        }
        else
        {
            gValue.loadFromRenderedText(gRenderer, " ", textColor );
        }
    }
    gValue.render(gRenderer,InpValRect.x+(InpValRect.w-gValue.getWidth())/2 , InpValRect.y);


}

void handleInputEvent(SDL_Event *e)
{
    if (startInput)
    {
        if( e->type == SDL_KEYDOWN )
        {
            if( e->key.keysym.sym == SDLK_BACKSPACE && inputText.length() > 0 )
            {
                inputText.pop_back();
                renderText = true;
            }
            else if( e->key.keysym.sym == SDLK_c && SDL_GetModState() & KMOD_CTRL )
            {
                SDL_SetClipboardText( inputText.c_str() );
            }
            else if( e->key.keysym.sym == SDLK_v && SDL_GetModState() & KMOD_CTRL )
            {
                inputText = SDL_GetClipboardText();
                renderText = true;
            }
        }
        else if( e->type == SDL_TEXTINPUT )
        {
            if( !( ( e->text.text[ 0 ] == 'c' || e->text.text[ 0 ] == 'C' ) && ( e->text.text[ 0 ] == 'v' || e->text.text[ 0 ] == 'V' ) && SDL_GetModState() & KMOD_CTRL ) )
            {
                if(inputText.length()<9) inputText += e->text.text;
                renderText = true;
            }
        }
    }
}

//================================================HANDLE SCROLL EVENTS============================================================
void handleScrollEvent(SDL_Event* e)
{
    if (e->type==SDL_MOUSEWHEEL)
    {
        int y=e->wheel.y;
        if (y<0) scrollY=-10;
        else scrollY = 10;
        for (int i=0; i<100; i++)
        {
            rNodePos[i].y += scrollY;
            gBtnNode[i].setSize(rNodePos[i]);
        }

        rShape = {DestR.w+SPACE_BETWEEN_NODE,rShape.y+scrollY,NODE_SIZE,NODE_SIZE};
    }
}

//=====================================================INIT BUTTTON===============================================================

void InitButton()
{
    rBtnAdd= {0,100,gBtnAddS1.w/6,gBtnAddS1.h/8};
    gBtnAdd.setSize(rBtnAdd);
    for (int i=0; i<100; i++)
    {
        gBtnNode[i].setSize(rNodePos[i]);
    }
    rBtnRandNum= {0,140,gBtnRandNumS1.w/6,gBtnRandNumS1.h/8};
    gBtnRandNum.setSize(rBtnRandNum);
    rBtnInit= {0,180,gBtnInitS1.w/6,gBtnInitS1.h/8};
    gBtnInit.setSize(rBtnInit);
    rBtnRandList= {0,220,gBtnRandListS1.w/6,gBtnRandListS1.h/8};
    gBtnRandList.setSize(rBtnRandList);
    rBtnInsHead= {0,260,gBtnInsHeadS1.w/6,gBtnInsHeadS1.h/8};
    gBtnInsHead.setSize(rBtnInsHead);
    rBtnInsTail= {0,300,gBtnInsTailS1.w/6,gBtnInsTailS1.h/8};
    gBtnInsTail.setSize(rBtnInsTail);
    rBtnInsRand= {0,340,gBtnInsRandS1.w/6,gBtnInsRandS1.h/8};
    gBtnInsRand.setSize(rBtnInsRand);
    rBtnDelHead= {0,380,gBtnDelHeadS1.w/6,gBtnDelHeadS1.h/8};
    gBtnDelHead.setSize(rBtnDelHead);
    rBtnDelTail= {0,420,gBtnDelTailS1.w/6,gBtnDelTailS1.h/8};
    gBtnDelTail.setSize(rBtnDelTail);
    rBtnDelNode= {0,460,gBtnDelNodeS1.w/6,gBtnDelNodeS1.h/8};
    gBtnDelNode.setSize(rBtnDelNode);
    rBtnLoadFile= {0,500,gBtnLoadFileS1.w/6,gBtnLoadFileS1.h/8};
    gBtnLoadFile.setSize(rBtnLoadFile);
    rBtnSaveFile= {0,540,gBtnSaveFileS1.w/6,gBtnSaveFileS1.h/8};
    gBtnSaveFile.setSize(rBtnSaveFile);
    rBtnTraverse= {0,580,gBtnTraverseS1.w/6,gBtnTraverseS1.h/8};
    gBtnTraverse.setSize(rBtnTraverse);
    rBtnFind= {0,620,gBtnFindS1.w/6,gBtnFindS1.h/8};
    gBtnFind.setSize(rBtnFind);

    rBtnNext= {gBtnPrevS1.w/6+gBtnStepS1.w/6+6,660,gBtnNextS1.w/6,gBtnNextS1.h/8};
    gBtnNext.setSize(rBtnNext);
    rBtnPrev= {0,660,gBtnPrevS1.w/6,gBtnPrevS1.h/8};
    gBtnPrev.setSize(rBtnPrev);
    rBtnRunOnce= {0,660,gBtnRunOnceS1.w/6,gBtnRunOnceS1.h/8};
    gBtnRunOnce.setSize(rBtnRunOnce);
    rBtnStep= {gBtnPrevS1.w/6+3,660,gBtnStepS1.w/6,gBtnStepS1.h/8};
    gBtnStep.setSize(rBtnStep);
    rAlert = {DestR.w+SPACE_BETWEEN_NODE,300,gAlert.w/2.1, gAlert.h/2};
    gBtnAlert.setSize(rAlert);

}

//=================================================INIT NODES' POSITION===========================================================

void InitNodePos()
{
    int mul=1;
    rNodePos[0] = {DestR.w+SPACE_BETWEEN_NODE, 250, NODE_SIZE, NODE_SIZE};
    for(int i=1; i<99; i++)
    {
        if ((i+1)%6!=1)
            rNodePos[i] = {rNodePos[i-1].x + mul*(NODE_SIZE + SPACE_BETWEEN_NODE), rNodePos[i-1].y, NODE_SIZE, NODE_SIZE};
        else
        {
            rNodePos[i] = {rNodePos[i-1].x, rNodePos[i-1].y + SPACE_BETWEEN_NODE + NODE_SIZE, NODE_SIZE, NODE_SIZE};
            mul=-mul;
        }
    }
}

//=====================================================ADD NODE===================================================================

void on_Add()
{
    MyList.Append(inputText);
    qNode.Push(MyList.GetSize()-1);
}

void on_Insert(int pos)
{
    MyList.Insert(inputText, pos);
    ignore_pos=pos;
    insert_now=3;
    qNode.Push(pos);

}

//=====================================================LOAD NODE FROM FILE===================================================================
void on_LoadFile(){
    found=-1;
    MyList.RemoveAll();
    MyList.Init();
    Rendered_Node=0;
    if(!MyList.LoadFile("FileList.txt"))
        printf("Error!!! Cannot load list from file!!!");
    else{
        if (is_once==true){
            for (int i=0; i<MyList.GetSize(); i++)
                qNode.Push(i);
        }
    }
}

//=====================================================SAVE NODE TO FILE===================================================================
void on_SaveFile(){
    MyList.SaveFile("FileList.txt");
}

//=====================================================FIND A NODE===================================================================
void on_Find(){
    found=MyList.Find(inputText);
}

//=====================================================NODE ANIMATION=============================================================

bool is_Arrived(int x)
{
    return (rMove.x==rNodePos[x].x && rMove.y==rNodePos[x].y);
}

void Node_Move(int x)
{
    rMove.x += VelX;
    if (rMove.x > rNodePos[x].x)
        rMove.x = rNodePos[x].x;
    rMove.y += VelY;
    if (rMove.y > rNodePos[x].y)
        rMove.y = rNodePos[x].y;
    SDL_RenderCopy(gRenderer, gNodeSprite[1].gSprite, NULL, &rMove);
}

void Display_Node(int ignore)
{
    SDL_Color textColor = { 0x00, 0x00, 0x00 };
    if (ssAdd)
    {
        SDL_RenderCopy(gRenderer,gNodeSprite[1].gSprite,NULL,&rShape);
        if (inputText.length()>5) gNodeValue.setFont(gFontValueSmall);
        else gNodeValue.setFont(gFontValueLarge);
        gNodeValue.loadFromRenderedText(gRenderer,inputText,textColor);
        gNodeValue.render(gRenderer,rShape.x+(rShape.w-gNodeValue.getWidth())/2, rShape.y+(rShape.h-gNodeValue.getHeight())/2);
    }

    if (!NodeMoving)
    {
        rMove = {rNodePos[0].x,50,NODE_SIZE,NODE_SIZE};
    }
    if (!qNode.isEmpty())
    {
        if (!is_Arrived(qNode.GetFirst()))
        {
            NodeMoving=true;
            Node_Move(qNode.GetFirst());
        }
        else
        {
            qNode.Pop();
            NodeMoving=false;
            Rendered_Node++;
            if (insert_now>1)
            {
                ignore_pos=-1;
                ignore=-1;
            }
            else if (delete_now>1){
                ignore_pos=-1;
                ignore=-1;
            }
        }
    }
    MyList.InitScan();
//    SDL_Color textColor = { 0x00, 0x00, 0x00 };
    SDL_Texture *gData=NULL;

    if (isFound==true){
        if (is_once==false){
            posFound=0;
            isFound=false;
        }
        temp2++;
        if (temp2==10*MyList.GetSize()){
            temp2=-1;
            isFound=false;
        }
    }

    //Set sign to Traverse if traverse button is clicked
    if (Traverse==true){
        isFound=false;
        if (is_once==false){
            Traverse=false;
        }
        else temp1++;
        found=-1;
        temp2=-1;
        if (temp1==10*MyList.GetSize()){
            temp1=-1;
            Traverse=false;
        }
    }

    int skip=0;
    for (int i=0; i<Rendered_Node; i++)
    {
        //Display node
        if (i==ignore)
        {
            MyList.ScanWholeList();
            skip=1;
        }
        int sprite=1;
        if(gBtnNode[i].GetCurrentSprite()==gBtnNode[i].BUTTON_SPRITE_MOUSE_DOWN) sprite=2;
        SDL_RenderCopy(gRenderer,gNodeSprite[sprite].gSprite,NULL,&rNodePos[i+skip]);
        if (sprite==2)
        {
            //pos=-1;
            gBtnNode_clickPos=i;
        }

        //Draw a yellow rectangle to sign the button clicked position
        if ((delete_now==0 || insert_now==0 || init_now!=-1) && (ProcessType==STEP_DELETE || ProcessType==STEP_INSERT || ProcessType==STEP_INIT)){
            rRect={rNodePos[gBtnNode_clickPos].x-2.5, rNodePos[gBtnNode_clickPos].y-2.5, rNodePos[gBtnNode_clickPos].w+5, rNodePos[gBtnNode_clickPos].h+5};
            SDL_SetRenderDrawColor(gRenderer,0xFF, 0xFF, 0, NULL);
            SDL_RenderDrawRect(gRenderer,&rRect);
        }

        if (is_once==true){
            //If the number is not found, then traverse the list
            if (found<0 && isFound==true){
                rFound={rNodePos[temp2/10].x-2.5, rNodePos[temp2/10].y-2.5, rNodePos[i].w+5, rNodePos[i].h+5};
                SDL_SetRenderDrawColor(gRenderer,0xFF, 0xFF, 0, NULL);
                SDL_RenderDrawRect(gRenderer,&rFound);
            }

            //If the number is found, then traverse the list until the node is found
            if (found>=0){
                if (temp2/10==found){
                    isFound=false;
                    rFound={rNodePos[temp2/10].x-2.5, rNodePos[temp2/10].y-2.5, rNodePos[i].w+5, rNodePos[i].h+5};
                    SDL_SetRenderDrawColor(gRenderer,0xFF, 0xFF, 0, NULL);
                    SDL_RenderDrawRect(gRenderer,&rFound);
                }
                else{
                    rFound={rNodePos[temp2/10].x-2.5, rNodePos[temp2/10].y-2.5, rNodePos[i].w+5, rNodePos[i].h+5};
                    SDL_SetRenderDrawColor(gRenderer,0xFF, 0xFF, 0, NULL);
                    SDL_RenderDrawRect(gRenderer,&rFound);
                }

            }
        }
        else{
            if (posFound>=0){
                if (isPrev==true){
                    if (posFound-1>=0)
                        posFound--;
                    isPrev=false;
                }

                if (isNext==true){
                    if (posFound+1<=MyList.GetSize() && posFound!=found)
                        posFound++;
                    else if (posFound==found || posFound+1>=MyList.GetSize()){
                        //posFound=-1;
                        btnAlert_visible=true;
                    }
                    isNext=false;
                }
            }

            if (posFound<MyList.GetSize()){
                rFound={rNodePos[posFound].x-2.5, rNodePos[posFound].y-2.5, rNodePos[posFound].w+5, rNodePos[posFound].h+5};
                SDL_SetRenderDrawColor(gRenderer,0xFF, 0xFF, 0, NULL);
                SDL_RenderDrawRect(gRenderer,&rFound);
            }


        }

        if (is_once==true && !MyList.isEmpty() && temp1>=0){
                rTraverse={rNodePos[temp1/10].x-2.5, rNodePos[temp1/10].y-2.5, rNodePos[i].w+5, rNodePos[i].h+5};
                SDL_SetRenderDrawColor(gRenderer,0xFF, 0xFF, 0, NULL);
                SDL_RenderDrawRect(gRenderer,&rTraverse);
        }
        if (is_once==false && !MyList.isEmpty() && pos>=0 && pos<=MyList.GetSize()){
            if (isPrev==true){
                if (pos-1>=0)
                    pos--;
                isPrev=false;
            }

            if (isNext==true){
                if (pos+1<MyList.GetSize()){
                    pos++;
                }
                else{
                    pos=-1;
                    btnAlert_visible=true;
                }
                isNext=false;
            }
            //printf("Before render: %d \n", pos);
            rTraverse={rNodePos[pos].x-2.5, rNodePos[pos].y-2.5, rNodePos[pos].w+5, rNodePos[pos].h+5};
            SDL_SetRenderDrawColor(gRenderer,0xFF, 0xFF, 0, NULL);
            SDL_RenderDrawRect(gRenderer,&rTraverse);

        }

        //Display data
        if (MyList.GetCurrentData().length()>5) gNodeValue.setFont(gFontValueSmall);
        else gNodeValue.setFont(gFontValueLarge);
        gNodeValue.loadFromRenderedText(gRenderer, MyList.GetCurrentData() , textColor);
        gNodeValue.render(gRenderer,rNodePos[i+skip].x+(rNodePos[i+skip].w-gNodeValue.getWidth())/2, rNodePos[i+skip].y+(rNodePos[i+skip].h-gNodeValue.getHeight())/2);
        MyList.ScanWholeList();

        //Display arrow
        if (i>0)
        {
            double angle=0;
            SDL_RendererFlip flip=SDL_FLIP_NONE;
            rArrow = {rNodePos[i-1+skip].x+rNodePos[i-1+skip].w, rNodePos[i-1+skip].y+30, SPACE_BETWEEN_NODE, 50};
            if ((i+skip)%6==0)
            {
                angle=90;
                flip=SDL_FLIP_NONE;
                rArrow = {rNodePos[i-1+skip].x+20, rNodePos[i-1+skip].y+rNodePos[i-1+skip].h, SPACE_BETWEEN_NODE, 50};
            }
            else
            {
                angle=0;
                if ((i/6)%2==0)
                {
                    flip=SDL_FLIP_NONE;
                    rArrow = {rNodePos[i-1+skip].x+rNodePos[i-1+skip].w, rNodePos[i-1+skip].y+30, SPACE_BETWEEN_NODE, 50};
                }
                else
                {
                    flip=SDL_FLIP_HORIZONTAL;
                    rArrow = {rNodePos[i+skip].x+rNodePos[i+skip].w, rNodePos[i+skip].y+30, SPACE_BETWEEN_NODE, 50};
                }
            }
            SDL_RenderCopyEx(gRenderer, gArrowSprite.gSprite, NULL, &rArrow, angle, NULL, flip);
        }
    }


    //Render Alert
    if (btnAlert_visible) SDL_RenderCopy(gRenderer,gAlert.gSprite,NULL,&rAlert);

}

//================================================== SBS : BUTTON NEXT ===========================================================
void on_Next()
{
    switch (ProcessType)
    {
        case STEP_INSERT :  {
                                if (insert_now==15)
                                {
                                    ssAdd=true;
                                    insert_now++;
                                }   else
                                if (insert_now==16)
                                {
                                    ssAdd=false;
                                    on_Add();
                                    insert_now++;
                                    ProcessType=STEP_NULL;
                                    btnAlert_visible=true;
                                }


                                if (insert_now==0)
                                {
                                    ssAdd=true;
                                    insert_now++;
                                }
                                else
                                if (insert_now==1)
                                {
                                    MyList.Insert(inputText,gBtnNode_clickPos);
                                    ignore_pos=gBtnNode_clickPos;
                                    insert_now++;
                                } else
                                if (insert_now==2)
                                {
                                    ssAdd=false;
                                    qNode.Push(gBtnNode_clickPos);
                                    insert_now++;
                                }
                                else if (insert_now==3)
                                {
                                    ProcessType=STEP_NULL;
                                    btnAlert_visible=true;
                                }

                                break;
                            }
        case STEP_NULL  :   {
                                add_now=0;
                                delete_now=0;
                                insert_now=0;
                                break;
                            }

        case STEP_DELETE :  {
                                if (delete_now==0)
                                {
                                    val=MyList.GetValue(gBtnNode_clickPos);
                                    Rendered_Node--;
                                    ignore_pos=gBtnNode_clickPos;
                                    delete_now++;
                                } else
                                if (delete_now==1)
                                {
                                    MyList.DeleteX(gBtnNode_clickPos);
                                    if (gBtnNode_clickPos>=MyList.GetSize()){
                                        ProcessType=STEP_NULL;
                                        btnAlert_visible=true;
                                    }
                                    ignore_pos=-1;
                                    delete_now++;
                                }
                                else{
                                        ProcessType=STEP_NULL;
                                        btnAlert_visible=true;
                                    }

                                break;
                            }

        case STEP_LOAD :    {
                                if (load_now<MyList.GetSize()){
                                    qNode.Push(load_now);
                                    load_now++;
                                }
                                else{
                                    load_now=-1;
                                    ProcessType=STEP_NULL;
                                    btnAlert_visible=true;
                                }
                                break;
                            }

        case STEP_ADD   :   {
                                if (add_now==0)
                                {
                                    ssAdd=true;
                                    add_now++;
                                }
                                else if (add_now==1)
                                {
                                    on_Add();
                                    ssAdd=false;
                                    add_now++;
                                }
                                else
                                {
                                    btnAlert_visible=true;
                                    add_now++;
                                    ProcessType=STEP_NULL;
                                }
                                break;
                            }
        case STEP_ADDLIST : {
                                if (addlist_now<MyList.GetSize())
                                {
                                    qNode.Push(addlist_now);
                                    inputText=MyList.GetValue(addlist_now);
                                    renderText=true;
                                    addlist_now++;
                                }
                                else{
                                    addlist_now=-1;
                                    ProcessType=STEP_NULL;
                                    btnAlert_visible=true;
                                }
                                break;
                            }
        case STEP_INIT:     {
                                if (init_now==0){
                                    Rendered_Node--;
                                    ignore_pos=gBtnNode_clickPos;
                                    init_now++;
                                    gBtnNode_clickPos++;
                                }
                                else if (init_now==1){
                                    TmpList.Append(MyList.GetValue(gBtnNode_clickPos-1));
                                    MyList.DeleteX(gBtnNode_clickPos-1);
                                    gBtnNode_clickPos--;
                                    ignore_pos=-1;
                                    //init_now++;
                                    if (MyList.isEmpty()){
                                        init_now=-1;
                                        MyList.RemoveAll();
                                        MyList.Init();
                                        btnAlert_visible=true;
                                        ProcessType=STEP_NULL;
                                    }
                                    else{
                                        init_now=0;
                                    }
                                }

                                break;
                            }
    }
}

//================================================== SBS : BUTTON PREV ===========================================================

void on_Prev()
{
    switch (ProcessType)
    {
        case STEP_INSERT :  {
                                if (insert_now==16)
                                {
                                    ssAdd=false;
                                    insert_now--;
                                }

                                if (insert_now==1)
                                {
                                    ssAdd=false;
                                    insert_now--;
                                }
                                else
                                if (insert_now==2)
                                {
                                    MyList.Delete(gBtnNode_clickPos);
                                    ignore_pos=-1;
                                    insert_now--;
                                }
                                else if (insert_now==3)
                                {
                                    ssAdd=true;
                                    Rendered_Node--;
                                    ignore_pos=gBtnNode_clickPos;
                                    insert_now--;
                                }
                                break;
                            }

        case STEP_ADD   :   {
                                if (add_now==1)
                                {
                                    ssAdd=false;
                                    add_now--;
                                }
                                else if (add_now==2)
                                {
                                    ssAdd=true;
                                    MyList.DeleteX(MyList.GetSize()-1);
                                    Rendered_Node--;
                                    add_now--;
                                }
                                break;

                            }
        case STEP_DELETE :  {
                                if (delete_now==1)
                                {
                                    Rendered_Node++;
                                    ignore_pos=-1;
                                    delete_now--;
                                }
                                else if (delete_now==2)
                                {
                                    MyList.Insert(val, gBtnNode_clickPos);
                                    ignore_pos=gBtnNode_clickPos;
                                    delete_now--;
                                }
                                break;
                            }
        case STEP_LOAD :    {
                                if (load_now>0){
                                    Rendered_Node--;
                                    load_now--;
                                }
                                break;
                            }
        case STEP_ADDLIST : {
                                if (addlist_now>0){
                                    Rendered_Node--;
                                    addlist_now--;
                                    inputText=MyList.GetValue(addlist_now);
                                    renderText=true;
                                }
                                break;
                            }
        case STEP_INIT :    {
                                if (init_now==0){
                                    if (!TmpList.isEmpty())
                                    {
                                        init_now=1;
                                        MyList.Insert_Head(TmpList.Pop());
                                        ignore_pos=gBtnNode_clickPos;
                                        gBtnNode_clickPos++;
                                    }

                                }
                                else if (init_now==1){
                                    init_now--;
                                    gBtnNode_clickPos--;
                                    ignore_pos=-1;
                                    Rendered_Node++;
                                }
                                break;
                            }
    }
}

//=====================================================MAIN PROGRAM===============================================================

int main( int argc, char* args[] )
{
	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		if( !loadMedia() )
		{
			printf( "Failed to load media!\n" );
		}
		else
		{
			bool quit = false;

			SDL_Event e;

			while( !quit )
			{
			    //Check when will render again the text
			    renderText = false;
				while( SDL_PollEvent( &e ) != 0 )
				{
				    // Handle quit event
					if( e.type == SDL_QUIT )
					{
						quit = true;
					}

					 //Handle button events
					if (ProcessType==STEP_NULL)
					{
					    if (!is_once)
                        {
                            btnStart_clicked=false;
                            btnRandNum_clicked=false;
                            btnInsHead_clicked=false;
                            btnInsTail_clicked=false;
                            btnInsRand_clicked=false;
                            btnSaveFile_clicked=false;
                            btnTraverse_clicked=false;
                            btnFind_clicked=false;
                            btnDelHead_clicked=false;
                            btnDelTail_clicked=false;
                            btnDelNode_clicked=false;
                        }

	                    gBtnAdd.handleEvent(&e);
                        gBtnRandNum.handleEvent(&e);
                        gBtnInsHead.handleEvent(&e);
                        gBtnInsTail.handleEvent(&e);
                        gBtnInsRand.handleEvent(&e);
                        gBtnSaveFile.handleEvent(&e);
                        gBtnTraverse.handleEvent(&e);
                        gBtnFind.handleEvent(&e);
                        gBtnRunOnce.handleEvent(&e);
                        gBtnStep.handleEvent(&e);
                        gBtnDelHead.handleEvent(&e);
                        gBtnDelTail.handleEvent(&e);
                        gBtnDelNode.handleEvent(&e);



                        if (!NodeMoving)
                       {
                            btnRandList_clicked=false;
                            btnLoadFile_clicked=false;
                            btnInit_clicked=false;

                            gBtnInit.handleEvent(&e);
                            gBtnRandList.handleEvent(&e);
                            gBtnLoadFile.handleEvent(&e);
                        }
					}
					gBtnAlert.handleEvent(&e);
                    gBtnNext.handleEvent(&e);
                    gBtnPrev.handleEvent(&e);


                    //Handle textbox events
                    tbxInput.handleEvent(&e);

                    //Handle input events
                    handleInputEvent(&e);

                    //Handle node events
                    for (int i=0; i<100; i++)
                        gBtnNode[i].handleEvent(&e);

                    //Handle scroll events
                    handleScrollEvent(&e);

				}

                SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
                SDL_RenderClear( gRenderer );

                //Render background
                SDL_RenderCopy(gRenderer,gBg,NULL,NULL);


                DrawLeftMenu();
                HandleButtonsEvent(&e);
                InputFromKeyboard(&e);
                Display_Node(ignore_pos);

                //SDL_RenderCopy(gRenderer,gAlert.gSprite,NULL,&rAlert);
				SDL_RenderPresent( gRenderer );
			}
		}
	}
	close();
	return 0;
}


