//1T4 03 �����͌�//
//�Q�l����http://www.c3.club.kyutech.ac.jp/gamewiki/index.php?%A5%DE%A5%EB%A5%D0%A5%C4%A5%B2%A1%BC%A5%E0//

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
	NONE,//�������Ȃ�
	MARU,//���̃^�[��
	BATU//�~�̃^�[��
};

//�v���g�^�C�v�錾
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
//���[�U�[�̓��͑҂��֐�
//====================================
void BusyWait()
{
#ifdef _WIN32
	getch();
#endif
}


//==================================
//�}�X���ׂĂ�NONE�ɂ��ăN���A���܂��B
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
//�^����ꂽ���� 1 �s��`�悵�܂��B
//====================================
void DrawLine(char *buffer)
{
	puts(buffer);
}

//====================================
//��ʂ̕`����N���A���܂��B
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
//�}�X��\�����܂��B
//====================================
void DrawGame()
{
	char buffer[100];
	DrawClear();
	GetString(buffer);
	printf(buffer);	
}

//====================================
//�Q�[���̏����������܂��B
//====================================
void Init()
{
	Clear();
	srand((unsigned int)time(NULL));
	g_turn = MARU;
}

//==================================
//���C���t�@�C���ł��B
//�킸��4�s�ō\������Ă��܂��B
//==================================
int main()
{
	Init();
	while(Run())
		;
	return 0;
}

//====================================
//�������擾���܂��B
//scanf�͊댯�Ȃ̂Ŏg���܂���B
//====================================
int GetNumber()
{
	char buffer[100];
	int length,out,i;
	fgets(buffer,100,stdin);
	length=(int)strlen(buffer);
	for(i = 0 ; i < length; i++)
	{
		//���s��fgets���g���Ă��邽�߂ɓ����Ă���B	
		if(!isdigit(buffer[i]) && buffer[i] != '\n')
		{
			return -1;//���s�Ȃ̂�-1��Ԃ��B
		}
	}
	sscanf(buffer,"%d",&out);
	return out;
}


//===========================
//�h���[�Q�[�����𔻒肵�܂��B
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
				return 0;//�h���[�Q�[������Ȃ���
			}
		}
	}
	return 1;//�h���[�Q�[����������
}




//==============================================
//�^����ꂽ�}�X�ɒu���邩�u���Ȃ����𔻒肵�܂��B
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
			return 1;//����
		}
	}
	DrawLine("���̓~�X������x");
	BusyWait();
	return 0;//���s
}

//=================================================
//�����ɗ^����ꂽ1 �s��������Ă��邩�𔻒肵�܂��B
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
		while(A && B && C && D)//�͈͓�����
		{
			int kind2 = g_mas[width2][height2];
			if(kind2 != kind)
			{
				return 0;//�I�������݂����Ȃ���
			}
			width2 += val_x;
			height2 += val_y;
			A = height2 < MAX_HEIGHT;
			B = width2 < MAX_WIDTH;
			C = height2 >= 0;
			D = width2 >= 0;
		}
		return 1;//�I����������������
	}
	return 0;//�������Ȃ���
}


///=============================
//�N���A��������܂��B
//=============================
bool IsClear()
{
	int height,width;

	//�s�𔻒�
	for(height = 0; height < MAX_HEIGHT; height++)
	{
		if(LineEva(0,height,1,0))
		{
			return 1;
		}
	}
	//��𔻒�
	for(width = 0; width < MAX_WIDTH; width++)
	{
		if(LineEva(width,0,0,1))
		{
			return 1;
		}
	}
	//���ォ��]��
	if(LineEva(0,0,1,1))
	{
		return 1;
	}
	//�E�ォ��]��
	if(LineEva(MAX_WIDTH - 1,0,-1,1))
	{
		return 1;
	}
	//��������
	if(LineEva(0,MAX_HEIGHT - 1,1,-1))
	{
		return 1;
	}
	//�E������
	if(LineEva(MAX_WIDTH - 1,MAX_HEIGHT - 1,-1,-1))
	{
		return 1;
	}
	return 0;//�N���A�����������Ȃ���
}

///=============================
//�`�悷�镶������擾���܂��B
//�o�͂�buffer�A�i�[����܂��B
//=============================
void GetString(char *buffer)
{
	char tmp[10];
	int width;
	int height;
	strcpy(buffer,"�@");
	for(width = 0; width < MAX_WIDTH; width++)
	{
		//�܂������ŉ��̐������������݂܂��B
		sprintf(tmp,"%2d",width);
		strcat(buffer,tmp);
	}
	strcat(buffer,"\n");
	for(height = 0;height < MAX_HEIGHT; height++)
	{
		//�c�̐������������݂܂��B
		sprintf(tmp,"%2d",height);
		strcat(buffer,tmp);
		
		//�����Ő���������~��`�悵�܂��B
		for(width = 0; width < MAX_WIDTH; width++)
		{
			int kind = g_mas[width][height];
			if(kind == NONE)
			{
				strcat(buffer,"�@");
			}
			else if(kind == MARU)
			{
				strcat(buffer,"��");
			}
			else if(kind == BATU)
			{
				strcat(buffer,"��");
			}
		}

		strcat(buffer,"\n");
	}
}

//=====================================
//�v���C���[1�̃L�[���擾���܂��B
//=====================================
void Player1(int *width,int *height)
{
	DrawLine("���̉����W����͂��Ă�������");
	*width = GetNumber();
	DrawLine("���̏c���W����͂��Ă�������");
	*height = GetNumber();
}

//=====================================
//�v���C���[ 2 �̃L�[���擾���܂��B
//�����ŃR���s���[�^������Ă��������B
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
//�����̓v���O�����̍X�V������Ƃ���ł��B
//=========================================
bool Run()
{
	int clear;
	bool isok = 0;

	//�v���C���̍s��
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
		//�v���C���[�̎w��ʒu���u���Ȃ��������𔻒�
		isok = Evaluate(width,height,g_turn);
	}

	//�h���[����
	if(IsDraw())
	{
		DrawGame();
		DrawLine("���h���[��");
		BusyWait();
		Clear();
	}


	//�N���A����
	clear = IsClear();
	if(clear == 1)
	{
		DrawGame();
		if(g_turn==BATU)
		{
			DrawLine("������������");
		}
		else
		{
			DrawLine("������������");
		}
		BusyWait();
	}

	//=======================
	//�����Ń^�[���ύX
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