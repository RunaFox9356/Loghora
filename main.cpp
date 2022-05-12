//=================================================
//	おもちゃ
//
//  Auther： hamadaryuuga
//=================================================
#include <stdio.h>
#include <time.h>
#include "main.h"
#include "input.h"
#include "resource.h"
#include "bg.h"
#include "file.h"

#define MAX_NAME (7)


//グローバル変数(必須)
LPDIRECT3D9 g_pD3D = NULL; //Direct3dオブジェクトへのポインタ
LPDIRECT3DDEVICE9 g_pD3DDevice = NULL; //Direct3dデバイスへのぽいんた
MODE g_mode = MODE_TITLE;//モード
LPD3DXFONT g_pFont = NULL; //フォントのポインタ
int g_nCountFPS;
PLAYER number;	//CHARACTERのアイコンの呼び出し用
bool Debug;
int Collar;


 //===================
 //メイン関数
 //===================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hlnstacePrev, LPSTR ipCmdLine, int nCmdShow)
{
	HWND hWnd;	//Windowハンドル識別子
	MSG msg;	//メッセージを格納する変数
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),										//WNDCLASSEXのメモリサイズ
		CS_HREDRAW | CS_VREDRAW,								//ウインドウのスタイル
		WindowProc,												//Windowプロシージャ  
		0,														//ゼロにする
		0,														//ゼロにする
		hInstance,												//インスタンスハンドル
		LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION)),	//タスクバーのアイコン
		LoadCursor(NULL, IDC_ARROW),							//マウスカーソル
		(HBRUSH)(COLOR_WINDOW + 1),								//クライアントの領域背景色
		MAKEINTRESOURCE(IDR_MENU1) ,							//メニューバー
		CLASS_NAME,					//Windowクラスの名前
		LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION))//ファイルアイコン
	};


	//ウインドウクラスの登録
	RegisterClassEx(&wcex);

	//クライアント領域を指定のサイズに調整
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	//ウインドウを生成
	hWnd = CreateWindowEx(
		0,//拡張Windowsスタイル
		CLASS_NAME,//Windowクラスの名前
		WINDOW_NAME,//Windowの名前
		WS_OVERLAPPEDWINDOW,//Windowスタイル
		CW_USEDEFAULT,//Windowの左上X座標
		CW_USEDEFAULT,//Window左上Y座標
		(rect.right - rect.left),//Window幅
		(rect.bottom - rect.top),//Window高さ
		NULL,//親Windowのハンドル
		NULL,//メニューハンドルまたは個子WindowID
		hInstance,//インスタンスハンドル
		IDI_APPLICATION);//Window作成データ

	DWORD dwCurrentTime;//現在時刻
	DWORD dwExedastTime;//最後更新時刻
	DWORD dwFrameCount; //フレームカウント
	DWORD dwFPSLastTime; //さいごのFPS

	if (FAILED(Init(hInstance, hWnd, TRUE)))//ここをfalseにすると大画面になる
	{//初期化が失敗した場合

		return -1;
	}

	//Windowm表示
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	//分解能の設定
	timeBeginPeriod(1);

	dwCurrentTime = 0;
	dwExedastTime = timeGetTime();

	dwFrameCount = 0;
	dwFPSLastTime = timeGetTime();

	//メッセージループ
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{//windowsの処理
			if (msg.message == WM_QUIT)
			{//WM_QUITメッセージを受けとったらメッセージるーぷを抜ける
				break;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{//DirectXの処理
			dwCurrentTime = timeGetTime();//現在時刻を取得
			if ((dwCurrentTime - dwFPSLastTime) >= 500)
			{//0.5秒経過
			 //FPS計測
				g_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);
				dwFPSLastTime = dwCurrentTime;
				dwFrameCount = 0;
			}

			if ((dwCurrentTime - dwExedastTime) >= (1000 / 60))
			{//60分の1秒経過
				dwExedastTime = dwCurrentTime;//処理開始の時刻[現在時刻]を保存
											  //更新処理
				Update(hWnd);

				//描画処理
				Draw();

				dwFrameCount++;
			}
		}
	}
	//終了処理
	Uninit();


	//分機能を戻す
	timeEndPeriod(1);
	//Windowクラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	return(int)msg.wParam;
}
//========================
//ウィンドウプロシージャ
//========================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//ポイント構造体
//	POINT    pt;

	
	int nID;//返り値を格納
	static HWND hWndEditlnput1;		//入力ウィンドウハンドル(識別子)

	switch (uMsg)
	{

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN: //	キーのメッセージ

		switch (wParam)
		{
		case VK_ESCAPE: //エスケープが押された

			nID = MessageBox(hWnd, L"終了しますか？", L"終わりのコマンド", MB_YESNO | MB_TOPMOST);
			//第一引数をNULLにするとメッセージBOXアクティブウィンドウにならない
			//第一引数をhWndにするとこのウィンドウが親(オーナー)になり、
			//このメッセージBOXを処理しない限りほかの処理ができない
			//入力ウィンドウの生成

			if (nID == IDYES)
			{//Windowを破壊する(WM_DESTROYメッセージを送る)
				DestroyWindow(hWnd);
				break;
			}
			break;
		}
		break;

		//case WM_RBUTTONDOWN:
		//	pt.x = LOWORD(lParam);
		//	pt.y = HIWORD(lParam);
		//	//クライアント座標をスクリーン座標へ変換
		//	ClientToScreen(hWnd, &pt);
		//	//ポップアップメニューを表示
		//	TrackPopupMenu(GetSubMenu(GetMenu(hWnd), 0), TPM_LEFTALIGN, pt.x, pt.y, 0, hWnd, NULL);
		//	break;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		
		{
		case ID_40001:
			//バージョン(A)
			//MessageBox(hWnd, ("更新したこと\n敵消せます\n敵動かせます"), ("マップエディターVer1.1"), MB_OK);

			break;
		case ID_40003:
			//
			Collar = 0;
			break;
		case ID_40004:
			//
			Collar = 1;
			break;
		case ID_40005:
			//
			Collar = 2;
			break;
		case ID_40006:
			//
			Collar = 3;
			break;
		case ID_40008:
			//ゴン
			try
			{
				GetTest(L"https://lhrpg.com/lhz/api/196293.json").wait();
			}
			catch (const std::exception& e)
			{
				printf("Error exception:%s\n", e.what());
				//assert(false);
			}
			number = PLAYER_GON;
			break;
		case ID_40009:
			//アークレリアル
			try
			{
				GetTest(L"https://lhrpg.com/lhz/api/197773.json").wait();
			}
			catch (const std::exception& e)
			{
				printf("Error exception:%s\n", e.what());
				//assert(false);
			}
			number = PLAYER_RERI;
			break;
		case ID_40011:
			//フレリン
			try
			{
				GetTest(L"https://lhrpg.com/lhz/api/196294.json").wait();
			}
			catch (const std::exception& e)
			{
				printf("Error exception:%s\n", e.what());
				//assert(false);
			}
			number = PLAYER_FURE;
			break;
		case ID_40012:
			//マヤ
			try
			{
				GetTest(L"https://lhrpg.com/lhz/api/197343.json").wait();
			}
			catch (const std::exception& e)
			{
				printf("Error exception:%s\n", e.what());
				//assert(false);
			}
			number = PLAYER_MAYA;
			break;
		case ID_40013:
			//オリバー
			try
			{
				GetTest(L"https://lhrpg.com/lhz/api/197969.json").wait();
			}
			catch (const std::exception& e)
			{
				printf("Error exception:%s\n", e.what());
				//assert(false);
			}
			number = PLAYER_ORI;
			break;
		case ID_40015:
			//トト
			try
			{
				GetTest(L"https://lhrpg.com/lhz/api/198061.json").wait();
			}
			catch (const std::exception& e)
			{
				printf("Error exception:%s\n", e.what());
				//assert(false);
			}
			number = PLAYER_TOTO;
			break;
	
		default:
			break;
		}
		GetIcon(number);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//===================
//デバイス取得
//===================
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}


//===================
//初期化処理
//===================
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)//TRUE：ウインドウ/FALSE:フルスクリーン
{
	D3DDISPLAYMODE d3ddm;//ディスプレイモード
	D3DPRESENT_PARAMETERS d3dpp; //プレゼンテーションパラメータ

								 //Direct3dオブジェクトの生成
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_pD3D == NULL)
	{
		return E_FAIL;
	}

	//現在のディスプレイモードを取得
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	//ポリゴンの初期化処理
	ZeroMemory(&d3dpp, sizeof(d3dpp));//パラメータのゼロクリア

	d3dpp.BackBufferWidth = SCREEN_WIDTH;//ゲーム画面サイズ
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;//ゲーム画面サイズ
	d3dpp.BackBufferFormat = d3ddm.Format; //バックばっふぁの形式数
	d3dpp.BackBufferCount = 1;//バックばっふぁの数
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD; //だぶるばっふぁの切り替え（映像信号の同期）
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16; //デプスばっふぁとして１６ｂitを使う
	d3dpp.Windowed = bWindow;//ウインドウモード
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;//リフレッシュレート
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;	//インターバル

																//Direct3Dデバイスの生成
	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&g_pD3DDevice)))
	{
		//Direct3Dデバイスの生成()
		if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&g_pD3DDevice)))
		{
			//Direct3Dデバイスの生成()
			if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp,
				&g_pD3DDevice)))

			{
				return E_FAIL;
			}
		}
	}

	//レジダーステートの設定
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);//カリングの設定
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);//アルファブレンド設定
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);//アルファブレンド設定
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);//アルファブレンド設定


																		//サンプラーステートの設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);//小さいの拡大
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);//大きいの縮小
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);


	//テクスチャステージの設定
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);//ポリゴンとテクスチャの色を混ぜる
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
	//入力処理の初期化処理

	if (FAILED(InitInput(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	//デバック表示用のフォント
	D3DXCreateFont(g_pD3DDevice, 50, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, L"HG創英角ｺﾞｼｯｸUB", &g_pFont);

	//乱数の初期化
	srand((unsigned int)time(0));


	InitBG();

	try
	{
		GetTest(L"https://lhrpg.com/lhz/api/196293.json").wait();
	}
	catch (const std::exception& e)
	{
		printf("Error exception:%s\n", e.what());
		//assert(false);
	}


	return S_OK;
}

//===================
//終了処理
//===================
void Uninit(void)
{
	//終了処理
	UninitBG();
	UninitInput();

	if (g_pFont != NULL)
	{
		g_pFont->Release();
		g_pFont = NULL;
	}

	if (g_pD3D != NULL)
	{
		g_pD3D->Release();
		g_pD3D = NULL;
	}
	if (g_pD3DDevice != NULL)
	{
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}
}

//===================
//更新処理
//===================
void Update(HWND hWnd)
{
	//更新処理
	UpdateInput();

	UpdateBG();
	
}

//===================
//描画処理
//===================
void Draw(void)
{
	//画面クリア
	g_pD3DDevice->Clear(0, NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 0),
		1.0f,
		0);

	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{//成功したとき
		
		DrawBG();

		// 2Dの前に3Dを置く
		GetDevice()->Clear(0, NULL, (D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);
		
#ifdef _DEBUG
		//debugの表示
		DrawDebug();
#endif // DEBUG
		//描画終了
		g_pD3DDevice->EndScene();
	}
	//バックバッファとフロントバッファの入れ替え
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}
//------------------------
//デバック時の表示
//------------------------
void DrawDebug(void)
{
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	WCHAR aStr[MAX_NAME][512];
	Set *Status = GetStatus();
	float a = GetBg();
	
	std::string String[5];
	

	//文字列に代入
	wsprintf(&aStr[0][0], L"%d   %d",(int)a, Status->nStatus);



	rect = { 770,470,SCREEN_WIDTH,SCREEN_HEIGHT };
	//テキストの描画
	g_pFont->DrawText(NULL, &aStr[0][0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));

	rect = { 810,250,SCREEN_WIDTH,SCREEN_HEIGHT };
	////テキストの描画
	g_pFont->DrawText(NULL, Status->race.c_str(), -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));

	rect = { 710,390,SCREEN_WIDTH,SCREEN_HEIGHT };
	////テキストの描画
	g_pFont->DrawText(NULL, Status->main_job.c_str(), -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));

	//名前の座標
	rect = { 510,150,SCREEN_WIDTH,SCREEN_HEIGHT };
	//テキストの描画
	g_pFont->DrawText(NULL, Status->name.c_str(), -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
	
	rect = { 550,780,SCREEN_WIDTH,SCREEN_HEIGHT };
	////テキストの描画
	g_pFont->DrawText(NULL, Status->unionsname.c_str(), -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
	
}

//------------------------
//デバックしてるかしないか
//------------------------
bool IsDebug(void)//取得
{
	return Debug;
}

int CollarData(void)//取得
{
	return Collar;
}

//---------------------------------------
//セットテクスチャ(3d)
//---------------------------------------
void Settex(VERTEX_3D *pVtx, float left, float right, float top, float down)
{
	//テクスチャの座標設定
	pVtx[0].tex = D3DXVECTOR2(left, top);
	pVtx[1].tex = D3DXVECTOR2(right, top);
	pVtx[2].tex = D3DXVECTOR2(left, down);
	pVtx[3].tex = D3DXVECTOR2(right, down);
}

//---------------------------------------
//セットテクスチャ(2d)
//---------------------------------------
void SetUp(VERTEX_2D *pVtx,
	float posx,		//中心点ｘ
	float posy,		//中心点ｙ
	float rotPlayer,		//プレイヤー回転
	float fAngleAttack 		//対角線角度
	, float fLengthAttack)	//対角線ノ長さ
{
	//回転
	pVtx[0].pos.x = posx + sinf(rotPlayer - D3DX_PI + fAngleAttack)*fLengthAttack;
	pVtx[0].pos.y = posy + cosf(rotPlayer - D3DX_PI + fAngleAttack)*fLengthAttack;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = posx + sinf(rotPlayer + D3DX_PI - fAngleAttack)*fLengthAttack;
	pVtx[1].pos.y = posy + cosf(rotPlayer + D3DX_PI - fAngleAttack)*fLengthAttack;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = posx + sinf(rotPlayer + -fAngleAttack)*fLengthAttack;
	pVtx[2].pos.y = posy + cosf(rotPlayer + -fAngleAttack)*fLengthAttack;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = posx + sinf(rotPlayer + fAngleAttack)*fLengthAttack;
	pVtx[3].pos.y = posy + cosf(rotPlayer + fAngleAttack)*fLengthAttack;
	pVtx[3].pos.z = 0.0f;
}

//---------------------------------------
//セットテクスチャ(2d)
//---------------------------------------
void Settex2d(VERTEX_2D *pVtx, float left, float right, float top, float down)
{
	//テクスチャの座標設定
	pVtx[0].tex = D3DXVECTOR2(left, top);
	pVtx[1].tex = D3DXVECTOR2(right, top);
	pVtx[2].tex = D3DXVECTOR2(left, down);
	pVtx[3].tex = D3DXVECTOR2(right, down);

}

//---------------------------------------
//正規化
//---------------------------------------
void Normal(float standard)
{
	//正規化
	if (standard > D3DX_PI)
	{
		standard -= D3DX_PI * 2;
	}
	if (standard < -D3DX_PI)
	{
		standard += D3DX_PI * 2;
	}

}

//---------------------------------------
//セットポス(3d)
//---------------------------------------
void SetNormalpos(VERTEX_3D *pVtx, float XUP, float XDW, float YUP, float YDW, float ZUP, float ZDW)
{
	pVtx[0].pos = D3DXVECTOR3(XDW, YUP, ZUP);
	pVtx[1].pos = D3DXVECTOR3(XUP, YUP, ZUP);
	pVtx[2].pos = D3DXVECTOR3(XDW, YDW, ZDW);
	pVtx[3].pos = D3DXVECTOR3(XUP, YDW, ZDW);
}

//---------------------------------------
//セットポス(2d)
//---------------------------------------
void SetNormalpos2d(VERTEX_2D *pVtx, float XUP, float XDW, float YUP, float YDW)
{
	pVtx[0].pos = D3DXVECTOR3(XUP, YUP, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(XDW, YUP, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(XUP, YDW, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(XDW, YDW, 0.0f);
}

