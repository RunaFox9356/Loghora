//============================
//
// ファイル設定
// Author:hamada ryuuga
//
//============================

#include "file.h"
#include "letter.h"

Set DataSet;
namespace nl = nlohmann;
const char* pathToJSON = "data/input_params.json";

using namespace utility;                    // 文字列変換などの一般的なユーティリティ
using namespace web;                        // URIのような共通の機能
using namespace web::http;                  // 共通のHTTP機能
using namespace web::http::client;          // HTTP クライアントの機能
using namespace concurrency::streams;       // 非同期ストリーム

//============================
// 読み込みます
//============================
pplx::task<void> GetTest(const wchar_t* cUrl)
{
	//std::ifstream ifs(pathToJSON);

	return pplx::create_task([&cUrl]
	{
		// クライアントの設定
		http_client client(cUrl);

		// リクエスト送信
		return client.request(methods::GET);

	})

		.then([](http_response  response)
	{
		// ステータスコード判定
		if (response.status_code() == status_codes::OK)
		{
			// レスポンスボディを表示
			auto body = response.extract_string();
	
			nl::json j;//リストの生成
			j = nlohmann::json::parse(body.get());//リストに文字をぶち込む

			 //こっちで構造体にデータを入れてます
			DataSet.name = StringToWString(UTF8toSjis(j["name"]));
			DataSet.nStatus = j["max_hitpoint"];
			DataSet.main_job = StringToWString(UTF8toSjis(j["main_job"]));
			DataSet.race = StringToWString(UTF8toSjis(j["race"]));
			DataSet.unionsname = StringToWString(UTF8toSjis(j["unions"][{"name"}]));
		}
		else
		{
			assert(false);
		}
	});

}

Set *GetStatus(void)
{
	return &DataSet;
}


////============================
//// 読み込みます
////============================
//void SetStatus()
//{
//	//std::istringstream FileSteam;
//
//	//nl::json j;//リストの生成
//	FileSteam >> j;//リストに文字をぶち込む
//
//
//	 //こっちで構造体にデータを入れてます
//	DataSet.name = StringToWString(UTF8toSjis(j["name"]));
//	DataSet.nStatus = j["max_hitpoint"];
//	DataSet.main_job = StringToWString(UTF8toSjis(j["main_job"]));
//	DataSet.race = StringToWString(UTF8toSjis(j["race"]));
//	DataSet.unionsname = StringToWString(UTF8toSjis(j["unions"]["name"]));
//
//
//}


//============================
// 読み込みます
//============================
//void SetStatus()
//{
//	std::ifstream ifs(pathToJSON);
//
//	if (ifs)
//	{//ファイル開いたとき
//
//		nl::json j;//リストの生成
//		ifs >> j;//リストに文字をぶち込む
//
//				 //こっちはコマンドプロンプト楽々表示君
//				 //std::cout << UTF8toSjis(j["name"]) << std::endl;
//				 //std::cout << j["max_hitpoint"] << std::endl;
//				 //std::cout << UTF8toSjis(j["unions"]["name"]) << std::endl;
//
//				 //こっちで構造体にデータを入れてます
//		DataSet.name = UTF8toSjis(j["name"]);
//		DataSet.nStatus = j["max_hitpoint"];
//		DataSet.main_job = UTF8toSjis(j["main_job"]);
//		DataSet.race = UTF8toSjis(j["race"]);
//		DataSet.unionsname = UTF8toSjis(j["unions"]["name"]);
//	}
//}