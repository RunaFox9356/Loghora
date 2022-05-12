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
typedef struct
{
	//
	std::wstring main_job;
	std::wstring race;
	std::wstring unionsname;
	std::wstring name;
	int nStatus;
}Set;


using namespace web;
using namespace web::http;
using namespace web::http::client;
using namespace concurrency::streams;

//=====================================
// プロトタイプ宣言
//=====================================
// 制御関数
pplx::task<void> GetTest(const wchar_t* cUrl);//読み込み
std::string UTF8toSjis(std::string srcUTF8);//変換
Set *GetStatus(void);//ゲット
//void SetStatus(void);

#endif

