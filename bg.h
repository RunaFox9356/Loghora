//============================
//
// 背景設定＆ライフバーヘッダー
// Author:hamada ryuuga
//
//============================
#ifndef _BG_H_
#define _BG_H_

#include"main.h"

//アイコンデータ

#define ICONPASSGON		(L"Data/TEXTURE/kitune.png")
#define ICONPASSRERI	(L"Data/TEXTURE/RERI.png")
#define ICONPASSHURE	(L"Data/TEXTURE/hure.jpg")
#define ICONPASSMAYA	(L"Data/TEXTURE/Maya.png")
#define ICONPASSORI		(L"Data/TEXTURE/ORI.png")
#define ICONPASSTOTO	(L"Data/TEXTURE/toto.png")

typedef enum
{
	PLAYER_GON = 0,
	PLAYER_RERI,
	PLAYER_FURE,
	PLAYER_MAYA,
	PLAYER_ORI,
	PLAYER_TOTO,
	PLAYER_MAX,
}PLAYER;

//構造体
typedef struct
{
	D3DXVECTOR3 pos;	//位置
	D3DXVECTOR3 move;	//移動量
	D3DXCOLOR col; //カラー
	PLAYER nType;//テクスチャ
}Bg;



//プロトタイプ宣言
void InitBG(void);
void UninitBG(void);
void UpdateBG(void);
void DrawBG(void);
float GetBg(void);
int *GetCollar(void);
void GetIcon(PLAYER number);

#endif // !_BG_H_
