//============================
//
// �t�@�C���ݒ�w�b�^�[
// Author:hamada ryuuga
//
//============================
#ifndef _FILE_H_			// ���̃}�N����`������ĂȂ�������
#define _FILE_H_			// ��d�C���N���[�h�h�~�̃}�N����`

//=====================================
// �C���N���[�h
//=====================================
#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>
#include "nlohmann/json.hpp"
#include <cpprest/http_client.h>
#include <cpprest/filestream.h>

#include <stdio.h>
#include <tchar.h>
#include <locale.h>
#include <string>
#include <iostream>
#include <windows.h>
#include <sstream>

//�\����
struct Set
{
	//
	std::wstring main_job;
	std::wstring race;
	std::wstring unionsname;
	std::wstring name;
	int nStatus;
};

//=====================================
// �v���g�^�C�v�錾
//=====================================
// ����֐�
pplx::task<void> GetJson(const wchar_t* cUrl);//�ǂݍ���
Set *GetStatus(void);//�Q�b�g
void OutputStatus();
void LoodJson(const wchar_t* cUrl);

#endif
