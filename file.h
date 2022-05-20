//============================
//
// ファイル設定ヘッター
// Author:hamada ryuuga
//
//============================
#ifndef _FILE_H_			// このマクロ定義がされてなかったら
#define _FILE_H_			// 二重インクルード防止のマクロ定義

//=====================================
// インクルード
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

//構造体
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
// プロトタイプ宣言
//=====================================
// 制御関数
pplx::task<void> GetJson(const wchar_t* cUrl);//読み込み
Set *GetStatus(void);//ゲット
void OutputStatus();
void LoodJson(const wchar_t* cUrl);

#endif
