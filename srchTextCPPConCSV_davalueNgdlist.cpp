static char sccsid[] =
"@(#)$Workfile: srchTextCPPConCSV.cpp $$Revision: 8 $$Date: 20/06/06 15:11 $$NoKeywords: $";
/********************************************************************************************
*
*	srchTextCPPConCSV:CSVファイルを取り込み、正規表現で検索する
*		(davalue,gdlistをC++化した)
*
*********************************************************************************************/

#include <iostream>
#include <fstream>
#include <regex>
#include "davalue.h"
#include "gdlist.h"

using namespace std;

void	regChrEsc(unsigned char *str);


int main()
{
	const char *strDaiBnr = "大分類";//出力(使用)する項目の見出し
	const char *strChuBnr = "中分類";
	const char *strShoBnr = "小分類";
	const char *strKmk =	"項目";

	const char *csvFlnm = "N:/CMD/家計簿/家計簿.csv";
	ifstream* ifCSV = new ifstream;
	int csvRecCnt = 0;

	struct da_value_s *daMidasi;	//見出しを出現位置順に保管する(CSVの全ての見出し)
	struct da_value_s *daData;		//データ項目を出現位置順に保管する(行ごとに一時使用)

	struct gd_list *DataList;		//出力および検索に使用するデータ項目をユニークにして保持する
	/* mode:DUP,key:NULL,data:str(大分類、中分類、小分類、項目をtab区切りでつなぐ) */
	struct gd_list *findItem;		//データ項目

	typedef regex_token_iterator<const char *> ItemIT_t;	//CSVファイルの行ごとにtabで分割する
	ItemIT_t::regex_type rxDel("\t");
	ItemIT_t end;

	cmatch narrowMatch;

	unsigned char srchText[128];	//検索文字列
	char strbuf[1024];				//汎用文字列

/*	----- procedure */

	daMidasi = da_init(1024);
	daData = da_init(1024);
	DataList = gd_init(U_DUP);

	//CSVファイルを読み込む
	cout << "家計簿検索　読み込み[" << csvFlnm << "]... ";
	ifCSV->open(csvFlnm);
	if (ifCSV->fail()) {
		cout << "ERROR: Cannot open file :" << csvFlnm << endl;
		goto exit_procedure;
	}

	while (ifCSV->good()) {
		ifCSV->getline(strbuf, sizeof(strbuf));
		//cout << strbuf << endl;
		if (strlen(strbuf) <= 0) break;

		if (csvRecCnt == 0) {
			//項目見出し(名称、位置)をdavalueに取得する
			ItemIT_t next(strbuf, strbuf + strlen(strbuf), rxDel);
			for (next = ItemIT_t(strbuf, strbuf + strlen(strbuf), rxDel, -1); next != end; ++next) {
				da_put(daMidasi, (char *)(&next->str()[0]));
			}
			da_fin(daMidasi);
			//for(char **pp = daMidasi->index; *pp != NULL; pp++) cout << (pp == daMidasi->index ? "" : ",") << *pp << endl;
		}
		else {
			//データ項目をdavalueに取得する
			da_reset(daData);
			ItemIT_t next(strbuf, strbuf + strlen(strbuf), rxDel);
			for (next = ItemIT_t(strbuf, strbuf + strlen(strbuf), rxDel, -1); next != end; ++next) {
				da_put(daData, (char *)(&next->str()[0]));
			}
			da_fin(daData);
			//for(char **pp = daData->index; *pp != NULL; pp++) cout << (pp == daData->index ? "" : ",") << *pp << endl;

			//データ項目を出力対象項目(大分類、中分類、小分類、項目をtab区切りでつなぐ)のみに絞る
			sprintf_s(strbuf, "%s\t%s\t%s\t%s",
				da_getVN(daMidasi, daData, strDaiBnr, ""), da_getVN(daMidasi, daData, strChuBnr, ""),
				da_getVN(daMidasi, daData, strShoBnr, ""), da_getVN(daMidasi, daData, strKmk, ""));
			GD_FINDF(findItem, DataList, GD_FIRST(DataList), (strcmp(strbuf, (const char *)item->data) == 0));
			if (findItem == NULL) gd_insert(DataList, NULL, (unsigned char *)strbuf, strlen(strbuf) + 1);
		}
		csvRecCnt++;
	}
	ifCSV->close();
	da_kill(daMidasi);
	da_kill(daData);
	gd_qsort(DataList, gd_data_str_order);
	{int i = 1; //keyの値をone - basedで整理する
		GD_LOOP(DataList, GD_FIRST(DataList))
			item->key = i++;
		GD_END_LOOP
	}
	cout << csvRecCnt << " 件(データ項目件数: " << GD_LAST(DataList)->key << " 件)" << endl;

	//検索を実行する
	while (true)
	{
		cout << "検索文字列(Enter:終了)=";
		cin.getline(strbuf, sizeof(strbuf));
		if (strlen(strbuf) <= 0) break;

		//検索文字列を含む行を検索する
		strcpy_s((char *)srchText, sizeof(srchText), ".*");
		strcat_s((char *)srchText, sizeof(srchText), strbuf);
		strcat_s((char *)srchText, sizeof(srchText), ".*");
		regChrEsc(srchText);
		cout << "検索中:" << srchText << endl;
		ItemIT_t::regex_type rxText((char *)srchText);
		GD_LOOP(DataList, GD_FIRST(DataList))
			if (regex_match((char *)item->data, narrowMatch, rxText)) {			//検索文字列によっては異常終了する
				cout << item->data << endl;
			}
		GD_END_LOOP
	}

exit_procedure:;

}

/*
*	regChrEsc:	漢字コードの2バイト目が正規表現を含む漢字は".."に置き換える
*/
void
regChrEsc(
unsigned char *str)
{
	const char *regChr = "\\[]()^${}.?+*:<>=,|'`#"; //正規表現にかぶりそうな文字
	unsigned char *p;
	unsigned char c;

/*	----- procedure */

	for (p = str,c=*p ; *p != NULL; c = *p) {
		if ((c >= 0x81 && c <= 0x9F) || (c >= 0xE0 && c <= 0xEF)) {	//漢字コードの1バイト目に該当する
			if (strchr(regChr, *(p+1)) != NULL) {
				*p ++ = '.';
				*p ++ = '.';
			}
			else {
				p += 2;
			}
		}
		else {
			p ++;
		}
	}
}