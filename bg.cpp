//============================
//
// �w�i�ݒ聕���C�t�o�[
// Author:hamada ryuuga
//
//============================
#include"bg.h"
#include "input.h"
#include "file.h"

#define NUM_BG (4)//�w�i����
#define NOW_LENGTH (7.5f)
#define MAXPOS (1110.0f)
#define MINPOS (350.0f)

#define ROUTELENGTH (MAXPOS - MINPOS)

//�X�^�e�B�b�N�ϐ�
static LPDIRECT3DTEXTURE9 s_pTextureBG[NUM_BG] = {}; //�e�N�X�`���̃|�C���^
static LPDIRECT3DVERTEXBUFFER9 s_pVtxBuffBG = NULL; //���_�o�b�t�@�̐ݒ�

static LPDIRECT3DTEXTURE9 s_pTextureIcon[PLAYER_MAX] = {}; //�e�N�X�`���̃|�C���^
static LPDIRECT3DVERTEXBUFFER9 s_pVtxBuffIcon = NULL; //���_�o�b�t�@�̐ݒ�

static LPDIRECT3DVERTEXBUFFER9 s_pVtxBuff = NULL; //���_�o�b�t�@�̐ݒ�

static D3DXVECTOR3 s_moveBG;	//�ړ���
static D3DXVECTOR3 s_posBG[NUM_BG];	//���W

static Bg s_nBgnCount;

static float txeX,txeY;
static float s_fHpLine, s_fLength;
static int s_nCollar;
static int s_nNowLife;


//================
//����������
//================
void InitBG(void)
{
	LPDIRECT3DDEVICE9  pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	s_nBgnCount.nType = PLAYER_GON;
	s_nBgnCount.pos = D3DXVECTOR3(MINPOS, 0.0f, 0.0f);
	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		ICONPASSGON,
		&s_pTextureIcon[PLAYER_GON]);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		ICONPASSRERI,
		&s_pTextureIcon[PLAYER_RERI]);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		ICONPASSHURE,
		&s_pTextureIcon[PLAYER_FURE]);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		ICONPASSMAYA,
		&s_pTextureIcon[PLAYER_MAYA]);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		ICONPASSORI,
		&s_pTextureIcon[PLAYER_ORI]);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		ICONPASSTOTO,
		&s_pTextureIcon[PLAYER_TOTO]);

	//���_�o�b�t�@
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,//�������_�t�H�[�}�b�g
		&s_pVtxBuffIcon,
		NULL);

	VERTEX_2D*pVtx; //���_�ւ̃|�C���^
					//���_�o�b�t�@���A�����b�N
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

	//���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//�e�N�X�`���̍��W�ݒ�
	Settex2d(pVtx, 0.0f, 1.0f, 0.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N
	s_pVtxBuffIcon->Unlock();

	s_fHpLine = 0.0f;
	s_fLength = 350.0f;
	s_nCollar = 0;

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		L"Data/TEXTURE/bluehp.png",
		&s_pTextureBG[0]);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		L"Data/TEXTURE/yellowhp.png",
		&s_pTextureBG[1]);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		L"Data/TEXTURE/pinkhp.png",
		&s_pTextureBG[2]);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		L"Data/TEXTURE/greenhp.png",
		&s_pTextureBG[3]);

	//���_�o�b�t�@
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,//�������_�t�H�[�}�b�g
		&s_pVtxBuffBG,
		NULL);

	//���_�o�b�t�@���A�����b�N
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

	//���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	
	//�e�N�X�`���̍��W�ݒ�
	Settex2d(pVtx, 0.0f, 1.0f, 0.0f, 1.0f);
	
	//���_�o�b�t�@���A�����b�N
	s_pVtxBuffBG->Unlock();


	//���_�o�b�t�@
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,//�������_�t�H�[�}�b�g
		&s_pVtxBuff,
		NULL);

	//���_�o�b�t�@���A�����b�N
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

	//���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(0.0f, 1.0f, 0.5f, 1.0f);
	pVtx[1].col = D3DXCOLOR(0.0f, 0.5f, 0.5f, 1.0f);
	pVtx[2].col = D3DXCOLOR(0.0f, 1.0f, 0.5f, 1.0f);
	pVtx[3].col = D3DXCOLOR(0.0f, 0.5f, 0.5f, 1.0f);

	//���_�o�b�t�@���A�����b�N
	s_pVtxBuffBG->Unlock();

}

//�j��
void UninitBG(void)
{

	for (int nCnt = 0; nCnt < NUM_BG; nCnt++)
	{
		//�e�N�X�`���̔j��
		if (s_pTextureBG[nCnt] != NULL)
		{
			s_pTextureBG[nCnt]->Release();
			s_pTextureBG[nCnt] = NULL;
		}
	}
	//���_�o�b�t�@�̔j��
	if (s_pVtxBuffBG != NULL)
	{
		s_pVtxBuffBG->Release();
		s_pVtxBuffBG = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (s_pVtxBuff != NULL)
	{
		s_pVtxBuff->Release();
		s_pVtxBuff = NULL;
	}

	for (int nCnt = 0; nCnt < PLAYER_MAX; nCnt++)
	{
		//���_�o�b�t�@�̔j��
		if (s_pTextureIcon[nCnt] != NULL)
		{
			s_pTextureIcon[nCnt]->Release();
			s_pTextureIcon[nCnt] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (s_pVtxBuffIcon != NULL)
	{
		s_pVtxBuffIcon->Release();
		s_pVtxBuffIcon = NULL;
	}
}
//==================
//�X�V����
//==================
void UpdateBG(void)
{
	Set *Status = GetStatus();
	VERTEX_2D*pVtx; //���_�ւ̃|�C���^
					//���_�o�b�t�@���A�����b�N
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	//�e�N�X�`���̍��W�ݒ�

	//if (GetMousePress(MOUSE_INPUT_LEFT))			//�}�E�X�̃v���X����			//�}�E�X�̃v���X����
	//{
	//	D3DXVECTOR3 Mouse = GetMouse();					//�}�E�X�̉�ʓ��̈ʒu

	//	s_nBgnCount.move.x = Mouse.x - s_nBgnCount.pos.x ;
	//}	

	////HP�̐��l�Ȃǂ�ς����[��
	//if (s_nBgnCount.pos.x >= s_fLength + ROUTELENGTH / Status->nStatus)
	//{
	//	
	//	s_fLength += ROUTELENGTH / Status->nStatus;
	//	s_fHpLine += 1.0f;
	//}

	////HP���}�b�N�X�������Ƃ��̏���
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
		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(0.5f, 0.0f, 0.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(0.5f, 0.0f, 0.0f, 1.0f);

	}
	else if (s_fHpLine <= Status->nStatus / 2)
	{
		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(0.5f, 0.5f, 0.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(0.5f, 0.5f, 0.0f, 1.0f);
	}
	else
	{
		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(0.0f, 1.0f, 0.5f, 1.0f);
		pVtx[1].col = D3DXCOLOR(0.0f, 0.5f, 0.5f, 1.0f);
		pVtx[2].col = D3DXCOLOR(0.0f, 1.0f, 0.5f, 1.0f);
		pVtx[3].col = D3DXCOLOR(0.0f, 0.5f, 0.5f, 1.0f);
	}

	s_nBgnCount.move.x += (0.0f - s_nBgnCount.move.x)*0.05f;//�i�ړI�̒l-���݂̒l�j�������W��	

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

	//HP���}�C�i�X�������Ƃ��̏���
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

	//���_�o�b�t�@���A�����b�N
	s_pVtxBuff->Unlock();
}
//==================
//�`�揈��
//==================
void DrawBG(void)
{

	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�̃|�C���^
//	int nCntEnemy;

	s_nCollar = CollarData();//�擾

	pDevice = GetDevice();		//���


	//---------------------------------------
	//�Q�[�W
	//---------------------------------------
	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, NULL);

	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,
		0,
		2);

	//---------------------------------------
	//�g
	//---------------------------------------
	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, s_pVtxBuffBG, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, s_pTextureBG[s_nCollar]);

	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,
		0,
		2);

	//---------------------------------------
	//�A�C�R��
	//---------------------------------------
	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, s_pVtxBuffIcon, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
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