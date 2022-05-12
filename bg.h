//============================
//
// �w�i�ݒ聕���C�t�o�[�w�b�_�[
// Author:hamada ryuuga
//
//============================
#ifndef _BG_H_
#define _BG_H_

#include"main.h"

//�A�C�R���f�[�^

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

//�\����
typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
	D3DXVECTOR3 move;	//�ړ���
	D3DXCOLOR col; //�J���[
	PLAYER nType;//�e�N�X�`��
}Bg;



//�v���g�^�C�v�錾
void InitBG(void);
void UninitBG(void);
void UpdateBG(void);
void DrawBG(void);
float GetBg(void);
int *GetCollar(void);
void GetIcon(PLAYER number);

#endif // !_BG_H_
