//============================
//
// ����ݒ�
// Author:hamada ryuuga
//
//============================
#ifndef _LETTER_H_			// ���̃}�N����`������ĂȂ�������
#define _LETTER_H_			// ��d�C���N���[�h�h�~�̃}�N����`

//=====================================
// �C���N���[�h
//=====================================
#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <locale.h>
#include <string>
#include <iostream>
#include <windows.h>
#include <sstream>
#include "nlohmann/json.hpp"
#include <cpprest/http_client.h>
#include <cpprest/filestream.h>


//=====================================
// �v���g�^�C�v�錾
//=====================================
// ����֐�
std::string WStringToString(std::wstring oWString);
std::wstring StringToWString(std::string oString);
std::string UTF8toSjis(std::string srcUTF8);
std::string SjistoUTF8(std::string srcSjis);
#endif