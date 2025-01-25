//UTF8 $Workfile: srchTextCPPConCSV.cpp $$Revision: 4 $$Date: 20/05/18 19:30 $
//$NoKeywords: $

#include <iostream>
#include <fstream>
#include <regex>
#include <map>
#include <vector>
#include <list>

using namespace std;

const char *strDaiBnr = "大分類";			//出力(使用)する項目の見出し
const char *strChuBnr = "中分類";
const char *strShoBnr ="小分類";
const char *strKmk ="項目";

typedef std::map<char *, int> ItemMap_t;	//(見出し,出現順位)を保管する(CSVの全ての見出し)
ItemMap_t ItemMap;
ItemMap_t::iterator ItemMapIT;

vector<char *> rowVec;						//データ項目を出現位置順に保管する(行ごとに一時使用)
vector<char *>::iterator rowIT;

list<char *> DataList;						//出力および検索に使用する項目をユニークにして保持する
list<char *>::iterator DataListIT;

char  *strValue(const char *itemNm);		//rowVec,ItemMapから見出しに対応するデータ値を取り出す
bool   isNewValue(const char *dataStr);		//DataList上でデータ値(出力する大分類～項目)をユニークにするための判定


int main()
{
	const char *csvFlnm = "G:/Projects/srchTextCPPCSV/x64/Debug/家計簿.csv";
	ifstream* ifCSV = new ifstream;
	int csvRecCnt = 0;

	typedef regex_token_iterator<const char *> ItemIT_t;	//CSVファイルの行ごとにtabで分割する
	ItemIT_t::regex_type rxDel("\t");
	ItemIT_t end;
	cmatch narrowMatch;

	char srchText[128];	//検索文字列
	char strbuf[1024];	//汎用文字列
	int i;				//汎用インデックス

	/*    ----- procedure */

	//CSVファイルを読み込む
	cout << "家計簿検索　読み込み...\n";
	ifCSV->open(csvFlnm);
	if (ifCSV->fail()) {
		cout << "ERROR: Cannot open file :" << csvFlnm << endl;
		goto exit_procedure;
	}

	while (ifCSV->good()) {
		ifCSV->getline(strbuf, sizeof(strbuf));
		//cout << strbuf << endl;
		if (strlen(strbuf) <= 0) break;

		if (csvRecCnt == 0){
			//項目見出し(名称、位置)をMapに取得する
			ItemIT_t next(strbuf, strbuf + strlen(strbuf), rxDel);
			for (next = ItemIT_t(strbuf, strbuf + strlen(strbuf), rxDel, -1),i = 0; next != end; ++next, i++) {
				ItemMap.insert(ItemMap_t::value_type(_strdup((char *)(&next->str()[0])), i));
			}
			//for (ItemMapIT=ItemMap.begin(); ItemMapIT != ItemMap.end(); ItemMapIT++)
			//	std::cout << "Loaded Item == " << ItemMapIT->first << ":" << ItemMapIT->second<< endl;
		}
		else {
			//データ項目をVectorに取得する
			ItemIT_t next(strbuf, strbuf + strlen(strbuf), rxDel);
			for (next = ItemIT_t(strbuf, strbuf + strlen(strbuf), rxDel, -1),i=0; next != end; ++next,i++) {
				rowVec.push_back(_strdup((char *)(&next->str()[0])));
			}
			for (; i < ItemMap.size();i++) {
				rowVec.push_back(_strdup(""));//最後の項目値がNULLの分
			}
			//for (ItemMapIT = ItemMap.begin(); ItemMapIT != ItemMap.end(); ItemMapIT++)
			//	std::cout << "Row Data == " << ItemMapIT->first << ":" << rowVec.at(ItemMapIT->second) << endl;

			//データ項目を出力対象項目(大分類、中分類、小分類、項目をtab区切りでつなぐ)のみに絞る
			sprintf_s(strbuf,"%s\t%s\t%s\t%s",strValue(strDaiBnr),strValue(strChuBnr),strValue(strShoBnr),strValue(strKmk));
			if(isNewValue(strbuf))	DataList.push_back(_strdup(strbuf));

			//データ項目(Vector)を開放する
			for (i = 0; i < ItemMap.size(); i++) free(rowVec.at(i));
			rowVec.erase(rowVec.begin(),rowVec.end());
		}
		csvRecCnt++;
	}
	ifCSV->close();
	cout << "家計簿検索　読み込み... " << csvRecCnt << " 件(データ項目件数: "<< DataList.size() << " 件)"<< endl;


	//検索を実行する
	while (true)
	{
		cout << "検索文字列(Enter:終了)=";
		cin.getline(strbuf,sizeof(strbuf));
		if (strlen(strbuf) <= 0) break;

		//検索文字列を含む行を検索する
		strcpy_s(srchText, ".*");
		strcat_s(srchText, sizeof(srchText), strbuf);
		strcat_s(srchText,  sizeof(srchText), ".*");
		ItemIT_t::regex_type rxText(srchText);
		for (DataListIT = DataList.begin(); DataListIT != DataList.end(); DataListIT++){
			if (regex_match(*DataListIT, narrowMatch, rxText)) {			//検索文字列によっては異常終了する
				cout << *DataListIT << endl;
			}
		}
	}

exit_procedure:;

}

/*
 *	strValue: 項目見出しに対応する(出現位置の)データを取得する
 */
char * 
strValue(
	const char *itemNm) {

	for (ItemMapIT = ItemMap.begin(); ItemMapIT != ItemMap.end(); ItemMapIT++){
		if (strcmp(itemNm,ItemMapIT->first) == 0) {
			return(rowVec.at(ItemMapIT->second));
		}
	}
	return((char *)NULL);
}

/*
 *	isNewValue: 新規の値であることを判定する(uniqueにするため)
 *				(Sort(),Unique()が機能しないので)
 */
bool   
isNewValue(
	const char *dataStr) {
	
	for (DataListIT = DataList.begin(); DataListIT != DataList.end(); DataListIT++) {
		if (strcmp(dataStr, *DataListIT) == 0) {
			return(false);
		}
	}
	return(true);
}

