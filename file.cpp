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

nl::json j;//リストの生成
using namespace utility;                    // 文字列変換などの一般的なユーティリティ
using namespace web;                        // URIのような共通の機能
using namespace web::http;                  // 共通のHTTP機能
using namespace web::http::client;          // HTTP クライアントの機能
using namespace concurrency::streams;       // 非同期ストリーム

//============================
// 読み込みます(引数URL　使い方)GetJson(L"URLここに書く").wait(); <- L必須
//============================
pplx::task<void> GetJson(const wchar_t* cUrl)
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

			j = nlohmann::json::parse(body.get());//リストに文字をぶち込む

			//こっちで構造体にデータを入れてます
			//文字は変換つけないとばぐるぞ＾＾これ-＞UTF8toSjis()
			DataSet.name = StringToWString(UTF8toSjis(j["name"]));
			DataSet.nStatus = j["max_hitpoint"];
			DataSet.main_job = StringToWString(UTF8toSjis(j["main_job"]));
			DataSet.race = StringToWString(UTF8toSjis(j["race"]));
			DataSet.unionsname = StringToWString(UTF8toSjis(j["unions"][0]["name"]));
		}
		else
		{
			assert(false);
		}
	});
}

//============================
//ゲット関数
//============================
Set *GetStatus(void)
{
	return &DataSet;
}

//============================
// 全部出力（置くだけ）
//============================
void OutputStatus()
{
	auto jobj = j.dump();

	std::ofstream writing_file;
	const std::string pathToJSON = "data/JSON/output_params.json";
	writing_file.open(pathToJSON, std::ios::out);
	writing_file << jobj << std::endl;
	writing_file.close();
}

void LoodJson(const wchar_t* cUrl)
{
	std::ifstream ifs(cUrl);

	if (ifs)
	{
		nl::json j;
		ifs >> j;


		//こっちで構造体にデータを入れてます//文字は変換つけないとばぐるぞ＾＾これ-＞UTF8toSjis()
		DataSet.name = StringToWString(UTF8toSjis(j["name"]));
		DataSet.nStatus = j["max_hitpoint"];
		DataSet.main_job = StringToWString(UTF8toSjis(j["main_job"]));
		DataSet.race = StringToWString(UTF8toSjis(j["race"]));
		DataSet.unionsname = StringToWString(UTF8toSjis(j["unions"][0]["name"]));

	}

}