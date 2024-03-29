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
#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"
#include <d3d9.h>
#include <tchar.h>

#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui_internal.h>

#define MAX_NAME (7)


//グローバル変数(必須)
LPDIRECT3D9 g_pD3D = NULL; //Direct3dオブジェクトへのポインタ
LPDIRECT3DDEVICE9 g_pD3DDevice = NULL; //Direct3dデバイスへのぽいんた

D3DPRESENT_PARAMETERS    g_d3dpp = {};

ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

MODE g_mode = MODE_TITLE;//モード
LPD3DXFONT g_pFont = NULL; //フォントのポインタ
int g_nCountFPS;
PLAYER number;	//CHARACTERのアイコンの呼び出し用
bool Debug;
int Collar;

bool show_demo_window = true;//基本の呼び出し
bool show_another_window = false;//もう一つ呼び出し
static char Txet[8] = "";

//=================================================
//プロトタイプ宣言
//=================================================
void ResetDevice();
bool ImGuiTxet(bool show_demo_window, bool show_another_window);
int Botan(int nLife);
// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
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

	// Show the window
	::ShowWindow(hWnd, SW_SHOWDEFAULT);
	::UpdateWindow(hWnd);

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	
	io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\meiryo.ttc", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());


	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// Setup Platform/Renderer backends
	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX9_Init(g_pD3DDevice);

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
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
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
				show_another_window = ImGuiTxet(show_demo_window, show_another_window);
			
				Update(hWnd);

				//描画処理
				Draw();

				dwFrameCount++;
			}
		}

	}

	//終了処理
	Uninit();

	::DestroyWindow(hWnd);
	::UnregisterClass(wcex.lpszClassName, wcex.hInstance);

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

	if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

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
				GetJson(L"https://lhrpg.com/lhz/api/196293.json").wait();
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
				GetJson(L"https://lhrpg.com/lhz/api/197773.json").wait();
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
				GetJson(L"https://lhrpg.com/lhz/api/196294.json").wait();
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
				GetJson(L"https://lhrpg.com/lhz/api/197343.json").wait();
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
				GetJson(L"https://lhrpg.com/lhz/api/197969.json").wait();
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
				GetJson(L"https://lhrpg.com/lhz/api/198061.json").wait();
			}
			catch (const std::exception& e)
			{
				printf("Error exception:%s\n", e.what());
				//assert(false);
			}
			number = PLAYER_TOTO;
			break;
		case ID_40016:
			show_demo_window = !show_demo_window;
			break;
		case ID_40018:
			//LoodJson(L"data/JSON/output_params.json");
			OutputStatus();
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
		GetJson(L"https://lhrpg.com/lhz/api/196293.json").wait();
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


	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

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

	 ////ここが背景ドロー


		DrawBG();
		//debugの表示
		DrawDebug();
		
		//D3DCOLOR clear_col_dx = D3DCOLOR_RGBA((int)(clear_color.x*clear_color.w*0.0f), (int)(clear_color.y*clear_color.w*255.0f), (int)(clear_color.z*clear_color.w*255.0f), (int)(clear_color.w*255.0f));
		//g_pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, clear_col_dx, 1.0f, 0);

		//g_pD3DDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
		//g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
		//g_pD3DDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);

		HRESULT result = g_pD3DDevice->Present(NULL, NULL, NULL, NULL);


		if (result == D3DERR_DEVICELOST && g_pD3DDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
			ResetDevice();
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
		
	
	
		// 2Dの前に3Dを置く
		GetDevice()->Clear(0, NULL, (D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);
		
#ifdef _DEBUG

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

void ResetDevice()
{
	ImGui_ImplDX9_InvalidateDeviceObjects();
	HRESULT hr = g_pD3DDevice->Reset(&g_d3dpp);
	if (hr == D3DERR_INVALIDCALL)
		IM_ASSERT(0);
	ImGui_ImplDX9_CreateDeviceObjects();
}

// ImGui Bezier widget. @r-lyeh, public domain
// v1.02: add BezierValue(); comments; usage
// v1.01: out-of-bounds coord snapping; custom border width; spacing; cosmetics
// v1.00: initial version
//
// [ref] http://robnapier.net/faster-bezier
// [ref] http://easings.net/es#easeInSine
//
// Usage:
// {  static float v[] = { 0.390f, 0.575f, 0.565f, 1.000f }; 
//    ImGui::Bezier( "easeOutSine", v );       // draw
//    float y = ImGui::BezierValue( 0.5f, v ); // x delta in [0..1] range
// }



namespace ImGui
{
	template<int steps>
	void bezier_table(ImVec2 P[4], ImVec2 results[steps + 1]) {
		static float C[(steps + 1) * 4], *K = 0;
		if (!K) {
			K = C;
			for (unsigned step = 0; step <= steps; ++step) {
				float t = (float)step / (float)steps;
				C[step * 4 + 0] = (1 - t)*(1 - t)*(1 - t);   // * P0
				C[step * 4 + 1] = 3 * (1 - t)*(1 - t) * t; // * P1
				C[step * 4 + 2] = 3 * (1 - t) * t*t;     // * P2
				C[step * 4 + 3] = t*t*t;               // * P3
			}
		}
		for (unsigned step = 0; step <= steps; ++step) {
			ImVec2 point = {
				K[step * 4 + 0] * P[0].x + K[step * 4 + 1] * P[1].x + K[step * 4 + 2] * P[2].x + K[step * 4 + 3] * P[3].x,
				K[step * 4 + 0] * P[0].y + K[step * 4 + 1] * P[1].y + K[step * 4 + 2] * P[2].y + K[step * 4 + 3] * P[3].y
			};
			results[step] = point;
		}
	}

	float BezierValue(float dt01, float P[4]) {
		enum { STEPS = 256 };
		ImVec2 Q[4] = { { 0, 0 },{ P[0], P[1] },{ P[2], P[3] },{ 1, 1 } };
		ImVec2 results[STEPS + 1];
		bezier_table<STEPS>(Q, results);
		return results[(int)((dt01 < 0 ? 0 : dt01 > 1 ? 1 : dt01) * STEPS)].y;
	}

	int Bezier(const char *label, float P[4]) {
		// visuals
		enum { SMOOTHNESS = 64 }; // curve smoothness: the higher number of segments, the smoother curve
		enum { CURVE_WIDTH = 4 }; // main curved line width
		enum { LINE_WIDTH = 1 }; // handlers: small lines width
		enum { GRAB_RADIUS = 6 }; // handlers: circle radius
		enum { GRAB_BORDER = 2 }; // handlers: circle border width

		const ImGuiStyle& Style = GetStyle();
		const ImGuiIO& IO = GetIO();
		ImDrawList* DrawList = GetWindowDrawList();
		ImGuiWindow* Window = GetCurrentWindow();
		if (Window->SkipItems)
			return false;

		// header and spacing
		int changed = SliderFloat4(label, P, 0, 1, "%.3f", 1.0f);
		int hovered = IsItemActive() || IsItemHovered(); // IsItemDragged() ?
		Dummy(ImVec2(0, 3));

		// prepare canvas
		const float avail = GetContentRegionAvail().x;
		const float dim = ImMin(avail, 128.f);
		ImVec2 Canvas(dim, dim);

		ImRect bb(Window->DC.CursorPos, Window->DC.CursorPos + Canvas);
		ItemSize(bb);
		if (!ItemAdd(bb, NULL))
			return changed;

		const ImGuiID id = Window->GetID(label);
		// hovered |= 0 != IsItemHovered(ImRect(bb.Min, bb.Min + ImVec2(avail,dim)), id);

		RenderFrame(bb.Min, bb.Max, GetColorU32(ImGuiCol_FrameBg, 1), true, Style.FrameRounding);

		// background grid
		for (int i = 0; i <= Canvas.x; i += (Canvas.x / 4)) {
			DrawList->AddLine(
				ImVec2(bb.Min.x + i, bb.Min.y),
				ImVec2(bb.Min.x + i, bb.Max.y),
				GetColorU32(ImGuiCol_TextDisabled));
		}
		for (int i = 0; i <= Canvas.y; i += (Canvas.y / 4)) {
			DrawList->AddLine(
				ImVec2(bb.Min.x, bb.Min.y + i),
				ImVec2(bb.Max.x, bb.Min.y + i),
				GetColorU32(ImGuiCol_TextDisabled));
		}

		// eval curve
		ImVec2 Q[4] = { { 0, 0 },{ P[0], P[1] },{ P[2], P[3] },{ 1, 1 } };
		ImVec2 results[SMOOTHNESS + 1];
		bezier_table<SMOOTHNESS>(Q, results);

		// control points: 2 lines and 2 circles
		{
			char buf[128];
			sprintf(buf, "0##%s", label);

			// handle grabbers
			for (int i = 0; i < 2; ++i)
			{
				ImVec2 pos = ImVec2(P[i * 2 + 0], 1 - P[i * 2 + 1]) * (bb.Max - bb.Min) + bb.Min;
				SetCursorScreenPos(pos - ImVec2(GRAB_RADIUS, GRAB_RADIUS));
				InvisibleButton((buf[0]++, buf), ImVec2(2 * GRAB_RADIUS, 2 * GRAB_RADIUS));
				if (IsItemActive() || IsItemHovered())
				{
					SetTooltip("(%4.3f, %4.3f)", P[i * 2 + 0], P[i * 2 + 1]);
				}
				if (IsItemActive() && IsMouseDragging(0))
				{
					P[i * 2 + 0] += GetIO().MouseDelta.x / Canvas.x;
					P[i * 2 + 1] -= GetIO().MouseDelta.y / Canvas.y;
					changed = true;
				}
			}

			if (hovered || changed) DrawList->PushClipRectFullScreen();

			// draw curve
			{
				ImColor color(GetStyle().Colors[ImGuiCol_PlotLines]);
				for (int i = 0; i < SMOOTHNESS; ++i) {
					ImVec2 p = { results[i + 0].x, 1 - results[i + 0].y };
					ImVec2 q = { results[i + 1].x, 1 - results[i + 1].y };
					ImVec2 r(p.x * (bb.Max.x - bb.Min.x) + bb.Min.x, p.y * (bb.Max.y - bb.Min.y) + bb.Min.y);
					ImVec2 s(q.x * (bb.Max.x - bb.Min.x) + bb.Min.x, q.y * (bb.Max.y - bb.Min.y) + bb.Min.y);
					DrawList->AddLine(r, s, color, CURVE_WIDTH);
				}
			}

			// draw lines and grabbers
			float luma = IsItemActive() || IsItemHovered() ? 0.5f : 1.0f;
			ImVec4 pink(1.00f, 0.00f, 0.75f, luma), cyan(0.00f, 0.75f, 1.00f, luma);
			ImVec4 white(GetStyle().Colors[ImGuiCol_Text]);
			ImVec2 p1 = ImVec2(P[0], 1 - P[1]) * (bb.Max - bb.Min) + bb.Min;
			ImVec2 p2 = ImVec2(P[2], 1 - P[3]) * (bb.Max - bb.Min) + bb.Min;
			DrawList->AddLine(ImVec2(bb.Min.x, bb.Max.y), p1, ImColor(white), LINE_WIDTH);
			DrawList->AddLine(ImVec2(bb.Max.x, bb.Min.y), p2, ImColor(white), LINE_WIDTH);
			DrawList->AddCircleFilled(p1, GRAB_RADIUS, ImColor(white));
			DrawList->AddCircleFilled(p1, GRAB_RADIUS - GRAB_BORDER, ImColor(pink));
			DrawList->AddCircleFilled(p2, GRAB_RADIUS, ImColor(white));
			DrawList->AddCircleFilled(p2, GRAB_RADIUS - GRAB_BORDER, ImColor(cyan));

			if (hovered || changed) DrawList->PopClipRect();

			// restore cursor pos
			SetCursorScreenPos(ImVec2(bb.Min.x, bb.Max.y + GRAB_RADIUS)); // :P
		}

		return changed;
	}

	void ShowBezierDemo()
	{
		{ static float v[] = { 0.000f, 0.000f, 1.000f, 1.000f }; Bezier("easeLinear", v); }
		{ static float v[] = { 0.470f, 0.000f, 0.745f, 0.715f }; Bezier("easeInSine", v); }
		{ static float v[] = { 0.390f, 0.575f, 0.565f, 1.000f }; Bezier("easeOutSine", v); }
		{ static float v[] = { 0.445f, 0.050f, 0.550f, 0.950f }; Bezier("easeInOutSine", v); }
		{ static float v[] = { 0.550f, 0.085f, 0.680f, 0.530f }; Bezier("easeInQuad", v); }
		{ static float v[] = { 0.250f, 0.460f, 0.450f, 0.940f }; Bezier("easeOutQuad", v); }
		{ static float v[] = { 0.455f, 0.030f, 0.515f, 0.955f }; Bezier("easeInOutQuad", v); }
		{ static float v[] = { 0.550f, 0.055f, 0.675f, 0.190f }; Bezier("easeInCubic", v); }
		{ static float v[] = { 0.215f, 0.610f, 0.355f, 1.000f }; Bezier("easeOutCubic", v); }
		{ static float v[] = { 0.645f, 0.045f, 0.355f, 1.000f }; Bezier("easeInOutCubic", v); }
		{ static float v[] = { 0.895f, 0.030f, 0.685f, 0.220f }; Bezier("easeInQuart", v); }
		{ static float v[] = { 0.165f, 0.840f, 0.440f, 1.000f }; Bezier("easeOutQuart", v); }
		{ static float v[] = { 0.770f, 0.000f, 0.175f, 1.000f }; Bezier("easeInOutQuart", v); }
		{ static float v[] = { 0.755f, 0.050f, 0.855f, 0.060f }; Bezier("easeInQuint", v); }
		{ static float v[] = { 0.230f, 1.000f, 0.320f, 1.000f }; Bezier("easeOutQuint", v); }
		{ static float v[] = { 0.860f, 0.000f, 0.070f, 1.000f }; Bezier("easeInOutQuint", v); }
		{ static float v[] = { 0.950f, 0.050f, 0.795f, 0.035f }; Bezier("easeInExpo", v); }
		{ static float v[] = { 0.190f, 1.000f, 0.220f, 1.000f }; Bezier("easeOutExpo", v); }
		{ static float v[] = { 1.000f, 0.000f, 0.000f, 1.000f }; Bezier("easeInOutExpo", v); }
		{ static float v[] = { 0.600f, 0.040f, 0.980f, 0.335f }; Bezier("easeInCirc", v); }
		{ static float v[] = { 0.075f, 0.820f, 0.165f, 1.000f }; Bezier("easeOutCirc", v); }
		{ static float v[] = { 0.785f, 0.135f, 0.150f, 0.860f }; Bezier("easeInOutCirc", v); }
		{ static float v[] = { 0.600f, -0.28f, 0.735f, 0.045f }; Bezier("easeInBack", v); }
		{ static float v[] = { 0.175f, 0.885f, 0.320f, 1.275f }; Bezier("easeOutBack", v); }
		{ static float v[] = { 0.680f, -0.55f, 0.265f, 1.550f }; Bezier("easeInOutBack", v); }
		// easeInElastic: not a bezier
		// easeOutElastic: not a bezier
		// easeInOutElastic: not a bezier
		// easeInBounce: not a bezier
		// easeOutBounce: not a bezier
		// easeInOutBounce: not a bezier
	}
}

bool ImGuiTxet(bool show_demo_window,bool show_another_window)
{
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();


	// Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
	if(show_demo_window)
	{
		Set *Status = GetStatus();
		static float f = 0.0f;
		static int counter = 0;
		static int nLife = 0;
		
		ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

		ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too
		ImGui::InputText("textbox 1", Txet, sizeof(Txet));
		//ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
		ImGui::Checkbox("Another Window", &show_another_window);

		ImGui::SliderInt("LIFE", &nLife, 0, Status->nStatus);

		int a;
		if (ImGui::TreeNode("Effecttree3", "Gradation"))
		{
			ImGui::RadioButton("RPlus GSubtract", &a, 1);
			ImGui::RadioButton("GPlus BSubtract", &a, 2);
			ImGui::RadioButton("BPlus RSubtract", &a, 3);
			ImGui::RadioButton("Gradation None", &a, 0);

			//ツリーを閉じる
			ImGui::TreePop();
		}

		SetLife(nLife);

		ImGui::SliderFloat("float", &f, 0.0f, 93.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
		ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

		nLife = Botan(nLife);
	
		static float v[] = { 0.390f, 0.575f, 0.565f, 1.000f };
		ImGui::Bezier( "あああ", v );       // draw
		float y = ImGui::BezierValue( 0.5f, v ); // x delta in [0..1] range

		{ static float v[] = { 0.680f, -0.55f, 0.265f, 1.550f }; ImGui::Bezier("easeInOutBack", v); }

		if (nLife < 0)
		{
			nLife = 0;
		}
		if (nLife > Status->nStatus)
		{
			nLife = Status->nStatus;
		}
		ImGui::SameLine();

		//ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}

	// 3. Show another simple window.
	if (show_another_window)
	{
		ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		ImGui::Text("Hello from another window!");
		if (ImGui::Button("Close Me"))
		{
			show_another_window = false;
		}
		ImGui::End();
	}	
	





	return show_another_window;

}


int Botan(int nLife)
{
	if (ImGui::Button("1++"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
	{
		nLife++;
	}
	if (ImGui::Button("1--"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
	{
		nLife--;
	}
	if (ImGui::Button("5++"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
	{
		nLife += 5;
	}
	if (ImGui::Button("5--"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
	{
		nLife -= 5;
	}
	if (ImGui::Button("10++"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
	{
		nLife += 10;
	}
	if (ImGui::Button("10--"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
	{
		nLife -= 10;
	}
	return nLife;
}