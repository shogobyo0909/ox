//1T4 03 鹿嶋章吾//
//参考資料http://www.c3.club.kyutech.ac.jp/gamewiki/index.php?%A5%DE%A5%EB%A5%D0%A5%C4%A5%B2%A1%BC%A5%E0//

#include <stdio.h>
#include <ctype.h>
#include <time.h>
#include <string.h>
#ifdef _WIN32
	#include <conio.h>
#endif
#include <stdlib.h>


#define MAX_HEIGHT 3
#define MAX_WIDTH 3



int g_mas[MAX_WIDTH][MAX_HEIGHT];
int g_turn;



typedef int bool;
enum
{
	false = 0,
	true = 1
};

enum
{
	NONE,//何もしない
	MARU,//○のターン
	BATU//×のターン
};

//プロトタイプ宣言
void GetString(char *buffer);
void Init();
void Clear();
void DrawClear();
bool Run();
int GetNumber();
void BusyWait();
void Player1(int *,int *);
void Player2(int *,int *);
bool Evaluate(int width,int height,int kind);
bool IsClear();
bool LineEva(int,int,int,int);


//====================================
//ユーザーの入力待ち関数
//====================================
void BusyWait()
{
#ifdef _WIN32
	getch();
#endif
}


//==================================
//マスすべてをNONEにしてクリアします。
//==================================
void Clear()
{
	int width,height;
	for(width = 0; width < MAX_WIDTH; width++)
	{
		for(height = 0; height < MAX_HEIGHT; height++)
		{
			g_mas[width][height] = NONE;
		}
	}
}

//====================================
//与えられた引数 1 行を描画します。
//====================================
void DrawLine(char *buffer)
{
	puts(buffer);
}

//====================================
//画面の描画をクリアします。
//====================================
void DrawClear()
{
#ifdef _WIN32
	system("cls");
#else
	system("clear");
#endif
}



//====================================
//マスを表示します。
//====================================
void DrawGame()
{
	char buffer[100];
	DrawClear();
	GetString(buffer);
	printf(buffer);	
}

//====================================
//ゲームの初期化をします。
//====================================
void Init()
{
	Clear();
	srand((unsigned int)time(NULL));
	g_turn = MARU;
}

//==================================
//メインファイルです。
//わずか4行で構成されています。
//==================================
int main()
{
	Init();
	while(Run())
		;
	return 0;
}

//====================================
//数字を取得します。
//scanfは危険なので使いません。
//====================================
int GetNumber()
{
	char buffer[100];
	int length,out,i;
	fgets(buffer,100,stdin);
	length=(int)strlen(buffer);
	for(i = 0 ; i < length; i++)
	{
		//改行はfgetsを使ってくるために入ってくる。	
		if(!isdigit(buffer[i]) && buffer[i] != '\n')
		{
			return -1;//失敗なので-1を返す。
		}
	}
	sscanf(buffer,"%d",&out);
	return out;
}


//===========================
//ドローゲームかを判定します。
//===========================
bool IsDraw()
{
	int width,height;

	for(width = 0; width < MAX_WIDTH; width++)
	{
		for(height = 0; height < MAX_HEIGHT; height++)
		{
			if(g_mas[width][height] == NONE)
			{
				return 0;//ドローゲームじゃないね
			}
		}
	}
	return 1;//ドローゲームだったよ
}




//==============================================
//与えられたマスに置けるか置けないかを判定します。
//==============================================
bool Evaluate(int width,int height,int kind)
{
	bool A = 0 <= width && width < MAX_WIDTH;
	bool B = 0 <= height && height < MAX_HEIGHT;
	if(A&&B)
	{
		if(g_mas[width][height] == NONE)
		{
			g_mas[width][height] = kind;
			return 1;//成功
		}
	}
	DrawLine("入力ミスもう一度");
	BusyWait();
	return 0;//失敗
}

//=================================================
//引数に与えられた1 行がそろっているかを判定します。
//=================================================
bool LineEva(int width,int height,int val_x,int val_y)
{	
	int kind = g_mas[width][height];
	int width2 = width + val_x;
	int height2 = height + val_y;
	bool A = height2 < MAX_HEIGHT;
	bool B = width2 < MAX_WIDTH;
	bool C = height2 >= 0;
	bool D = width2 >= 0;
	if(kind != NONE)
	{
		while(A && B && C && D)//範囲内だよ
		{
			int kind2 = g_mas[width2][height2];
			if(kind2 != kind)
			{
				return 0;//終了条件みたさないよ
			}
			width2 += val_x;
			height2 += val_y;
			A = height2 < MAX_HEIGHT;
			B = width2 < MAX_WIDTH;
			C = height2 >= 0;
			D = width2 >= 0;
		}
		return 1;//終了条件満たしたよ
	}
	return 0;//満たさないよ
}


///=============================
//クリア判定をします。
//=============================
bool IsClear()
{
	int height,width;

	//行を判定
	for(height = 0; height < MAX_HEIGHT; height++)
	{
		if(LineEva(0,height,1,0))
		{
			return 1;
		}
	}
	//列を判定
	for(width = 0; width < MAX_WIDTH; width++)
	{
		if(LineEva(width,0,0,1))
		{
			return 1;
		}
	}
	//左上から評価
	if(LineEva(0,0,1,1))
	{
		return 1;
	}
	//右上から評価
	if(LineEva(MAX_WIDTH - 1,0,-1,1))
	{
		return 1;
	}
	//左下から
	if(LineEva(0,MAX_HEIGHT - 1,1,-1))
	{
		return 1;
	}
	//右下から
	if(LineEva(MAX_WIDTH - 1,MAX_HEIGHT - 1,-1,-1))
	{
		return 1;
	}
	return 0;//クリア条件満たさないよ
}

///=============================
//描画する文字列を取得します。
//出力はbufferⅱ格納されます。
//=============================
void GetString(char *buffer)
{
	char tmp[10];
	int width;
	int height;
	strcpy(buffer,"　");
	for(width = 0; width < MAX_WIDTH; width++)
	{
		//まずここで横の数字を書き込みます。
		sprintf(tmp,"%2d",width);
		strcat(buffer,tmp);
	}
	strcat(buffer,"\n");
	for(height = 0;height < MAX_HEIGHT; height++)
	{
		//縦の数字を書き込みます。
		sprintf(tmp,"%2d",height);
		strcat(buffer,tmp);
		
		//ここで数字を○や×を描画します。
		for(width = 0; width < MAX_WIDTH; width++)
		{
			int kind = g_mas[width][height];
			if(kind == NONE)
			{
				strcat(buffer,"　");
			}
			else if(kind == MARU)
			{
				strcat(buffer,"☆");
			}
			else if(kind == BATU)
			{
				strcat(buffer,"★");
			}
		}

		strcat(buffer,"\n");
	}
}

//=====================================
//プレイヤー1のキーを取得します。
//=====================================
void Player1(int *width,int *height)
{
	DrawLine("☆の横座標を入力してください");
	*width = GetNumber();
	DrawLine("☆の縦座標を入力してください");
	*height = GetNumber();
}

//=====================================
//プレイヤー 2 のキーを取得します。
//ここでコンピュータを作ってください。
//=====================================
void Player2(int *width,int *height)
{

	bool A = 1;
	
	while(A)
	{
		*width = rand() % MAX_WIDTH;
		*height = rand() % MAX_HEIGHT;

		A = g_mas[*width][*height] != NONE;
	}
}

//=========================================
//ここはプログラムの更新をするところです。
//=========================================
bool Run()
{
	int clear;
	bool isok = 0;

	//プレイヤの行動
	while(!isok)
	{
		int width,height;
		DrawClear();
		DrawGame();
		if(g_turn == MARU)
		{
			Player1(&width,&height);
		}
		else
		{
			Player2(&width,&height);
		}
		//プレイヤーの指定位置が置けなかったかを判定
		isok = Evaluate(width,height,g_turn);
	}

	//ドロー判定
	if(IsDraw())
	{
		DrawGame();
		DrawLine("☆ドロー★");
		BusyWait();
		Clear();
	}


	//クリア判定
	clear = IsClear();
	if(clear == 1)
	{
		DrawGame();
		if(g_turn==BATU)
		{
			DrawLine("★が勝ったよ");
		}
		else
		{
			DrawLine("☆が勝ったよ");
		}
		BusyWait();
	}

	//=======================
	//ここでターン変更
	//========================
	if(g_turn == BATU)
	{
		g_turn = MARU;
	}
	else
	{
		g_turn = BATU;
	}
	return clear == 0;
}