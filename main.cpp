//=================================================
//	��������
//
//  Auther�F hamadaryuuga
//=================================================
#include <stdio.h>
#include <time.h>
#include "main.h"
#include "input.h"
#include "resource.h"
#include "bg.h"
#include "file.h"

#define MAX_NAME (7)


//�O���[�o���ϐ�(�K�{)
LPDIRECT3D9 g_pD3D = NULL; //Direct3d�I�u�W�F�N�g�ւ̃|�C���^
LPDIRECT3DDEVICE9 g_pD3DDevice = NULL; //Direct3d�f�o�C�X�ւ̂ۂ���
MODE g_mode = MODE_TITLE;//���[�h
LPD3DXFONT g_pFont = NULL; //�t�H���g�̃|�C���^
int g_nCountFPS;
PLAYER number;	//CHARACTER�̃A�C�R���̌Ăяo���p
bool Debug;
int Collar;


 //===================
 //���C���֐�
 //===================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hlnstacePrev, LPSTR ipCmdLine, int nCmdShow)
{
	HWND hWnd;	//Window�n���h�����ʎq
	MSG msg;	//���b�Z�[�W���i�[����ϐ�
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),										//WNDCLASSEX�̃������T�C�Y
		CS_HREDRAW | CS_VREDRAW,								//�E�C���h�E�̃X�^�C��
		WindowProc,												//Window�v���V�[�W��  
		0,														//�[���ɂ���
		0,														//�[���ɂ���
		hInstance,												//�C���X�^���X�n���h��
		LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION)),	//�^�X�N�o�[�̃A�C�R��
		LoadCursor(NULL, IDC_ARROW),							//�}�E�X�J�[�\��
		(HBRUSH)(COLOR_WINDOW + 1),								//�N���C�A���g�̗̈�w�i�F
		MAKEINTRESOURCE(IDR_MENU1) ,							//���j���[�o�[
		CLASS_NAME,					//Window�N���X�̖��O
		LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION))//�t�@�C���A�C�R��
	};


	//�E�C���h�E�N���X�̓o�^
	RegisterClassEx(&wcex);

	//�N���C�A���g�̈���w��̃T�C�Y�ɒ���
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	//�E�C���h�E�𐶐�
	hWnd = CreateWindowEx(
		0,//�g��Windows�X�^�C��
		CLASS_NAME,//Window�N���X�̖��O
		WINDOW_NAME,//Window�̖��O
		WS_OVERLAPPEDWINDOW,//Window�X�^�C��
		CW_USEDEFAULT,//Window�̍���X���W
		CW_USEDEFAULT,//Window����Y���W
		(rect.right - rect.left),//Window��
		(rect.bottom - rect.top),//Window����
		NULL,//�eWindow�̃n���h��
		NULL,//���j���[�n���h���܂��͌qWindowID
		hInstance,//�C���X�^���X�n���h��
		IDI_APPLICATION);//Window�쐬�f�[�^

	DWORD dwCurrentTime;//���ݎ���
	DWORD dwExedastTime;//�Ō�X�V����
	DWORD dwFrameCount; //�t���[���J�E���g
	DWORD dwFPSLastTime; //��������FPS

	if (FAILED(Init(hInstance, hWnd, TRUE)))//������false�ɂ���Ƒ��ʂɂȂ�
	{//�����������s�����ꍇ

		return -1;
	}

	//Windowm�\��
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	//����\�̐ݒ�
	timeBeginPeriod(1);

	dwCurrentTime = 0;
	dwExedastTime = timeGetTime();

	dwFrameCount = 0;
	dwFPSLastTime = timeGetTime();

	//���b�Z�[�W���[�v
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{//windows�̏���
			if (msg.message == WM_QUIT)
			{//WM_QUIT���b�Z�[�W���󂯂Ƃ����烁�b�Z�[�W��[�Ղ𔲂���
				break;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{//DirectX�̏���
			dwCurrentTime = timeGetTime();//���ݎ������擾
			if ((dwCurrentTime - dwFPSLastTime) >= 500)
			{//0.5�b�o��
			 //FPS�v��
				g_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);
				dwFPSLastTime = dwCurrentTime;
				dwFrameCount = 0;
			}

			if ((dwCurrentTime - dwExedastTime) >= (1000 / 60))
			{//60����1�b�o��
				dwExedastTime = dwCurrentTime;//�����J�n�̎���[���ݎ���]��ۑ�
											  //�X�V����
				Update(hWnd);

				//�`�揈��
				Draw();

				dwFrameCount++;
			}
		}
	}
	//�I������
	Uninit();


	//���@�\��߂�
	timeEndPeriod(1);
	//Window�N���X�̓o�^������
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	return(int)msg.wParam;
}
//========================
//�E�B���h�E�v���V�[�W��
//========================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//�|�C���g�\����
//	POINT    pt;

	
	int nID;//�Ԃ�l���i�[
	static HWND hWndEditlnput1;		//���̓E�B���h�E�n���h��(���ʎq)

	switch (uMsg)
	{

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN: //	�L�[�̃��b�Z�[�W

		switch (wParam)
		{
		case VK_ESCAPE: //�G�X�P�[�v�������ꂽ

			nID = MessageBox(hWnd, L"�I�����܂����H", L"�I���̃R�}���h", MB_YESNO | MB_TOPMOST);
			//��������NULL�ɂ���ƃ��b�Z�[�WBOX�A�N�e�B�u�E�B���h�E�ɂȂ�Ȃ�
			//��������hWnd�ɂ���Ƃ��̃E�B���h�E���e(�I�[�i�[)�ɂȂ�A
			//���̃��b�Z�[�WBOX���������Ȃ�����ق��̏������ł��Ȃ�
			//���̓E�B���h�E�̐���

			if (nID == IDYES)
			{//Window��j�󂷂�(WM_DESTROY���b�Z�[�W�𑗂�)
				DestroyWindow(hWnd);
				break;
			}
			break;
		}
		break;

		//case WM_RBUTTONDOWN:
		//	pt.x = LOWORD(lParam);
		//	pt.y = HIWORD(lParam);
		//	//�N���C�A���g���W���X�N���[�����W�֕ϊ�
		//	ClientToScreen(hWnd, &pt);
		//	//�|�b�v�A�b�v���j���[��\��
		//	TrackPopupMenu(GetSubMenu(GetMenu(hWnd), 0), TPM_LEFTALIGN, pt.x, pt.y, 0, hWnd, NULL);
		//	break;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		
		{
		case ID_40001:
			//�o�[�W����(A)
			//MessageBox(hWnd, ("�X�V��������\n�G�����܂�\n�G�������܂�"), ("�}�b�v�G�f�B�^�[Ver1.1"), MB_OK);

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
			//�S��
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
			//�A�[�N�����A��
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
			//�t������
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
			//�}��
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
			//�I���o�[
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
			//�g�g
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
//�f�o�C�X�擾
//===================
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}


//===================
//����������
//===================
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)//TRUE�F�E�C���h�E/FALSE:�t���X�N���[��
{
	D3DDISPLAYMODE d3ddm;//�f�B�X�v���C���[�h
	D3DPRESENT_PARAMETERS d3dpp; //�v���[���e�[�V�����p�����[�^

								 //Direct3d�I�u�W�F�N�g�̐���
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_pD3D == NULL)
	{
		return E_FAIL;
	}

	//���݂̃f�B�X�v���C���[�h���擾
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	//�|���S���̏���������
	ZeroMemory(&d3dpp, sizeof(d3dpp));//�p�����[�^�̃[���N���A

	d3dpp.BackBufferWidth = SCREEN_WIDTH;//�Q�[����ʃT�C�Y
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;//�Q�[����ʃT�C�Y
	d3dpp.BackBufferFormat = d3ddm.Format; //�o�b�N�΂��ӂ��̌`����
	d3dpp.BackBufferCount = 1;//�o�b�N�΂��ӂ��̐�
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD; //���Ԃ�΂��ӂ��̐؂�ւ��i�f���M���̓����j
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16; //�f�v�X�΂��ӂ��Ƃ��ĂP�U��it���g��
	d3dpp.Windowed = bWindow;//�E�C���h�E���[�h
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;//���t���b�V�����[�g
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;	//�C���^�[�o��

																//Direct3D�f�o�C�X�̐���
	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&g_pD3DDevice)))
	{
		//Direct3D�f�o�C�X�̐���()
		if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&g_pD3DDevice)))
		{
			//Direct3D�f�o�C�X�̐���()
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

	//���W�_�[�X�e�[�g�̐ݒ�
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);//�J�����O�̐ݒ�
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);//�A���t�@�u�����h�ݒ�
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);//�A���t�@�u�����h�ݒ�
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);//�A���t�@�u�����h�ݒ�


																		//�T���v���[�X�e�[�g�̐ݒ�
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);//�������̊g��
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);//�傫���̏k��
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);


	//�e�N�X�`���X�e�[�W�̐ݒ�
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);//�|���S���ƃe�N�X�`���̐F��������
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
	//���͏����̏���������

	if (FAILED(InitInput(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	//�f�o�b�N�\���p�̃t�H���g
	D3DXCreateFont(g_pD3DDevice, 50, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, L"HG�n�p�p�޼��UB", &g_pFont);

	//�����̏�����
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
//�I������
//===================
void Uninit(void)
{
	//�I������
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
//�X�V����
//===================
void Update(HWND hWnd)
{
	//�X�V����
	UpdateInput();

	UpdateBG();
	
}

//===================
//�`�揈��
//===================
void Draw(void)
{
	//��ʃN���A
	g_pD3DDevice->Clear(0, NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 0),
		1.0f,
		0);

	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{//���������Ƃ�
		
		DrawBG();

		// 2D�̑O��3D��u��
		GetDevice()->Clear(0, NULL, (D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);
		
#ifdef _DEBUG
		//debug�̕\��
		DrawDebug();
#endif // DEBUG
		//�`��I��
		g_pD3DDevice->EndScene();
	}
	//�o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}
//------------------------
//�f�o�b�N���̕\��
//------------------------
void DrawDebug(void)
{
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	WCHAR aStr[MAX_NAME][512];
	Set *Status = GetStatus();
	float a = GetBg();
	
	std::string String[5];
	

	//������ɑ��
	wsprintf(&aStr[0][0], L"%d   %d",(int)a, Status->nStatus);



	rect = { 770,470,SCREEN_WIDTH,SCREEN_HEIGHT };
	//�e�L�X�g�̕`��
	g_pFont->DrawText(NULL, &aStr[0][0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));

	rect = { 810,250,SCREEN_WIDTH,SCREEN_HEIGHT };
	////�e�L�X�g�̕`��
	g_pFont->DrawText(NULL, Status->race.c_str(), -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));

	rect = { 710,390,SCREEN_WIDTH,SCREEN_HEIGHT };
	////�e�L�X�g�̕`��
	g_pFont->DrawText(NULL, Status->main_job.c_str(), -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));

	//���O�̍��W
	rect = { 510,150,SCREEN_WIDTH,SCREEN_HEIGHT };
	//�e�L�X�g�̕`��
	g_pFont->DrawText(NULL, Status->name.c_str(), -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
	
	rect = { 550,780,SCREEN_WIDTH,SCREEN_HEIGHT };
	////�e�L�X�g�̕`��
	g_pFont->DrawText(NULL, Status->unionsname.c_str(), -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
	
}

//------------------------
//�f�o�b�N���Ă邩���Ȃ���
//------------------------
bool IsDebug(void)//�擾
{
	return Debug;
}

int CollarData(void)//�擾
{
	return Collar;
}

//---------------------------------------
//�Z�b�g�e�N�X�`��(3d)
//---------------------------------------
void Settex(VERTEX_3D *pVtx, float left, float right, float top, float down)
{
	//�e�N�X�`���̍��W�ݒ�
	pVtx[0].tex = D3DXVECTOR2(left, top);
	pVtx[1].tex = D3DXVECTOR2(right, top);
	pVtx[2].tex = D3DXVECTOR2(left, down);
	pVtx[3].tex = D3DXVECTOR2(right, down);
}

//---------------------------------------
//�Z�b�g�e�N�X�`��(2d)
//---------------------------------------
void SetUp(VERTEX_2D *pVtx,
	float posx,		//���S�_��
	float posy,		//���S�_��
	float rotPlayer,		//�v���C���[��]
	float fAngleAttack 		//�Ίp���p�x
	, float fLengthAttack)	//�Ίp���m����
{
	//��]
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
//�Z�b�g�e�N�X�`��(2d)
//---------------------------------------
void Settex2d(VERTEX_2D *pVtx, float left, float right, float top, float down)
{
	//�e�N�X�`���̍��W�ݒ�
	pVtx[0].tex = D3DXVECTOR2(left, top);
	pVtx[1].tex = D3DXVECTOR2(right, top);
	pVtx[2].tex = D3DXVECTOR2(left, down);
	pVtx[3].tex = D3DXVECTOR2(right, down);

}

//---------------------------------------
//���K��
//---------------------------------------
void Normal(float standard)
{
	//���K��
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
//�Z�b�g�|�X(3d)
//---------------------------------------
void SetNormalpos(VERTEX_3D *pVtx, float XUP, float XDW, float YUP, float YDW, float ZUP, float ZDW)
{
	pVtx[0].pos = D3DXVECTOR3(XDW, YUP, ZUP);
	pVtx[1].pos = D3DXVECTOR3(XUP, YUP, ZUP);
	pVtx[2].pos = D3DXVECTOR3(XDW, YDW, ZDW);
	pVtx[3].pos = D3DXVECTOR3(XUP, YDW, ZDW);
}

//---------------------------------------
//�Z�b�g�|�X(2d)
//---------------------------------------
void SetNormalpos2d(VERTEX_2D *pVtx, float XUP, float XDW, float YUP, float YDW)
{
	pVtx[0].pos = D3DXVECTOR3(XUP, YUP, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(XDW, YUP, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(XUP, YDW, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(XDW, YDW, 0.0f);
}

