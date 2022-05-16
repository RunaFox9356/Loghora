//============================
//
// 背景設定＆ライフバー
// Author:hamada ryuuga
//
//============================
#include"bg.h"
#include "input.h"
#include "file.h"

#define NUM_BG (4)//背景枚数
#define NOW_LENGTH (7.5f)
#define MAXPOS (1110.0f)
#define MINPOS (350.0f)

#define ROUTELENGTH (MAXPOS - MINPOS)

//スタティック変数
static LPDIRECT3DTEXTURE9 s_pTextureBG[NUM_BG] = {}; //テクスチャのポインタ
static LPDIRECT3DVERTEXBUFFER9 s_pVtxBuffBG = NULL; //頂点バッファの設定

static LPDIRECT3DTEXTURE9 s_pTextureIcon[PLAYER_MAX] = {}; //テクスチャのポインタ
static LPDIRECT3DVERTEXBUFFER9 s_pVtxBuffIcon = NULL; //頂点バッファの設定

static LPDIRECT3DVERTEXBUFFER9 s_pVtxBuff = NULL; //頂点バッファの設定

static D3DXVECTOR3 s_moveBG;	//移動量
static D3DXVECTOR3 s_posBG[NUM_BG];	//座標

static Bg s_nBgnCount;

static float txeX,txeY;
static float s_fHpLine, s_fLength;
static int s_nCollar;
static int s_nNowLife;


//================
//初期化処理
//================
void InitBG(void)
{
	LPDIRECT3DDEVICE9  pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	s_nBgnCount.nType = PLAYER_GON;
	s_nBgnCount.pos = D3DXVECTOR3(MINPOS, 0.0f, 0.0f);
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		ICONPASSGON,
		&s_pTextureIcon[PLAYER_GON]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		ICONPASSRERI,
		&s_pTextureIcon[PLAYER_RERI]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		ICONPASSHURE,
		&s_pTextureIcon[PLAYER_FURE]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		ICONPASSMAYA,
		&s_pTextureIcon[PLAYER_MAYA]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		ICONPASSORI,
		&s_pTextureIcon[PLAYER_ORI]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		ICONPASSTOTO,
		&s_pTextureIcon[PLAYER_TOTO]);

	//頂点バッファ
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,//ここ頂点フォーマット
		&s_pVtxBuffIcon,
		NULL);

	VERTEX_2D*pVtx; //頂点へのポインタ
					//頂点バッファをアンロック
	s_pVtxBuffIcon->Lock(0, 0, (void**)&pVtx, 0);

	s_posBG[0] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	SetNormalpos2d(pVtx,
		120.0f,
		400.0f,
		100.0f,
		330.0f);

	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//テクスチャの座標設定
	Settex2d(pVtx, 0.0f, 1.0f, 0.0f, 1.0f);

	//頂点バッファをアンロック
	s_pVtxBuffIcon->Unlock();

	s_fHpLine = 0.0f;
	s_fLength = 350.0f;
	s_nCollar = 0;

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		L"Data/TEXTURE/bluehp.png",
		&s_pTextureBG[0]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		L"Data/TEXTURE/yellowhp.png",
		&s_pTextureBG[1]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		L"Data/TEXTURE/pinkhp.png",
		&s_pTextureBG[2]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		L"Data/TEXTURE/greenhp.png",
		&s_pTextureBG[3]);

	//頂点バッファ
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,//ここ頂点フォーマット
		&s_pVtxBuffBG,
		NULL);

	//頂点バッファをアンロック
	s_pVtxBuffBG->Lock(0, 0, (void**)&pVtx, 0);
	
	s_posBG[0] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	SetNormalpos2d(pVtx,
		0.0f,
		SCREEN_WIDTH,
		0.0f,
		SCREEN_HEIGHT);

	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	txeX = 0.0f;
	txeY = 0.0f;

	//頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	
	//テクスチャの座標設定
	Settex2d(pVtx, 0.0f, 1.0f, 0.0f, 1.0f);
	
	//頂点バッファをアンロック
	s_pVtxBuffBG->Unlock();


	//頂点バッファ
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,//ここ頂点フォーマット
		&s_pVtxBuff,
		NULL);

	//頂点バッファをアンロック
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	SetNormalpos2d(pVtx,
		50.0f,
		MAXPOS,
		113.0f,
		190.0f);

	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(0.0f, 1.0f, 0.5f, 1.0f);
	pVtx[1].col = D3DXCOLOR(0.0f, 0.5f, 0.5f, 1.0f);
	pVtx[2].col = D3DXCOLOR(0.0f, 1.0f, 0.5f, 1.0f);
	pVtx[3].col = D3DXCOLOR(0.0f, 0.5f, 0.5f, 1.0f);

	//頂点バッファをアンロック
	s_pVtxBuffBG->Unlock();

}

//破棄
void UninitBG(void)
{

	for (int nCnt = 0; nCnt < NUM_BG; nCnt++)
	{
		//テクスチャの破棄
		if (s_pTextureBG[nCnt] != NULL)
		{
			s_pTextureBG[nCnt]->Release();
			s_pTextureBG[nCnt] = NULL;
		}
	}
	//頂点バッファの破棄
	if (s_pVtxBuffBG != NULL)
	{
		s_pVtxBuffBG->Release();
		s_pVtxBuffBG = NULL;
	}

	//頂点バッファの破棄
	if (s_pVtxBuff != NULL)
	{
		s_pVtxBuff->Release();
		s_pVtxBuff = NULL;
	}

	for (int nCnt = 0; nCnt < PLAYER_MAX; nCnt++)
	{
		//頂点バッファの破棄
		if (s_pTextureIcon[nCnt] != NULL)
		{
			s_pTextureIcon[nCnt]->Release();
			s_pTextureIcon[nCnt] = NULL;
		}
	}

	//頂点バッファの破棄
	if (s_pVtxBuffIcon != NULL)
	{
		s_pVtxBuffIcon->Release();
		s_pVtxBuffIcon = NULL;
	}
}
//==================
//更新処理
//==================
void UpdateBG(void)
{
	Set *Status = GetStatus();
	VERTEX_2D*pVtx; //頂点へのポインタ
					//頂点バッファをアンロック
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	//テクスチャの座標設定

	//if (GetMousePress(MOUSE_INPUT_LEFT))			//マウスのプレス処理			//マウスのプレス処理
	//{
	//	D3DXVECTOR3 Mouse = GetMouse();					//マウスの画面内の位置

	//	s_nBgnCount.move.x = Mouse.x - s_nBgnCount.pos.x ;
	//}	

	////HPの数値などを変えるやーつ
	//if (s_nBgnCount.pos.x >= s_fLength + ROUTELENGTH / Status->nStatus)
	//{
	//	
	//	s_fLength += ROUTELENGTH / Status->nStatus;
	//	s_fHpLine += 1.0f;
	//}

	////HPがマックス超えたときの処理
	//if (s_nBgnCount.pos.x <= s_fLength - ROUTELENGTH / Status->nStatus)
	//{
	//	s_fLength -= ROUTELENGTH / Status->nStatus;
	//	s_fHpLine -= 1.0f;
	//}

	//int wheel = GetMouseWheel();
	//if (s_fHpLine < s_nNowLife)
	//{
	//	s_nBgnCount.pos.x += ROUTELENGTH / Status->nStatus;
	//	s_fLength += ROUTELENGTH / Status->nStatus;
	//	s_fHpLine += 1.0f;

	//}
	//else if (s_fHpLine > s_nNowLife)
	//{
	//	s_nBgnCount.pos.x -= ROUTELENGTH / Status->nStatus;
	//	s_fLength -= ROUTELENGTH / Status->nStatus;
	//	s_fHpLine -= 1.0f;
	//}
	if (s_fHpLine < s_nNowLife)
	{
		s_nBgnCount.pos.x += ROUTELENGTH / Status->nStatus *(s_nNowLife-s_fHpLine);
		s_fLength += ROUTELENGTH / Status->nStatus*(s_nNowLife - s_fHpLine);
		s_fHpLine = s_nNowLife;

	}
	else if (s_fHpLine > s_nNowLife)
	{
		s_nBgnCount.pos.x -= ROUTELENGTH / Status->nStatus*(s_fHpLine -s_nNowLife);
		s_fLength -= ROUTELENGTH / Status->nStatus*(s_fHpLine -s_nNowLife);
		s_fHpLine = s_nNowLife;
	}

	if (s_fHpLine <= Status->nStatus / 8)
	{
		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(0.5f, 0.0f, 0.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(0.5f, 0.0f, 0.0f, 1.0f);

	}
	else if (s_fHpLine <= Status->nStatus / 2)
	{
		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(0.5f, 0.5f, 0.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(0.5f, 0.5f, 0.0f, 1.0f);
	}
	else
	{
		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(0.0f, 1.0f, 0.5f, 1.0f);
		pVtx[1].col = D3DXCOLOR(0.0f, 0.5f, 0.5f, 1.0f);
		pVtx[2].col = D3DXCOLOR(0.0f, 1.0f, 0.5f, 1.0f);
		pVtx[3].col = D3DXCOLOR(0.0f, 0.5f, 0.5f, 1.0f);
	}

	s_nBgnCount.move.x += (0.0f - s_nBgnCount.move.x)*0.05f;//（目的の値-現在の値）＊減衰係数	

	s_nBgnCount.pos += s_nBgnCount.move;

	SetNormalpos2d(pVtx,
		350.0f,
		s_nBgnCount.pos.x,
		390.0f,
		660.0f);

	if (s_fHpLine < 0)
	{
		s_fLength = MINPOS;
		s_fHpLine = (float)Status->nStatus;
	
		SetNormalpos2d(pVtx,
			350.0f,
			MINPOS,
			390.0f,
			660.0f);
	}

	//HPがマイナス超えたときの処理
	if (s_nBgnCount.pos.x <= MINPOS)
	{
		s_fHpLine = 0;
		s_fLength = MINPOS;
		SetNormalpos2d(pVtx,
			350.0f,
			MINPOS,
			390.0f,
			660.0f);
	}

	//頂点バッファをアンロック
	s_pVtxBuff->Unlock();
}
//==================
//描画処理
//==================
void DrawBG(void)
{

	LPDIRECT3DDEVICE9 pDevice;//デバイスのポインタ
//	int nCntEnemy;

	s_nCollar = CollarData();//取得

	pDevice = GetDevice();		//代入


	//---------------------------------------
	//ゲージ
	//---------------------------------------
	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, NULL);

	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,
		0,
		2);

	//---------------------------------------
	//枠
	//---------------------------------------
	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, s_pVtxBuffBG, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, s_pTextureBG[s_nCollar]);

	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,
		0,
		2);

	//---------------------------------------
	//アイコン
	//---------------------------------------
	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, s_pVtxBuffIcon, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, s_pTextureIcon[s_nBgnCount.nType]);

	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,
		0,
		2);
}



float GetBg(void)
{
	return s_fHpLine;
}




void GetIcon(PLAYER number)
{
	s_nBgnCount.nType = number;
}



void SetLife(int nowLife)
{

	s_nNowLife = nowLife;

}