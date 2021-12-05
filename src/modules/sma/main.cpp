#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <iomanip>
#include <limits>
#include <time.h>
#include <curl/curl.h>
#include "./HandleObj.cpp"

#define QUERY "https://query1.finance.yahoo.com/v7/finance/download/" // searching query

class CurlObj {
public:
    // #1 fetching csv string file from yahoo finance
    CurlObj (std::string url) {
	curl = curl_easy_init();
	if (!curl) {
	    throw std::string("Curl did not initialize.");
	}
	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curlWriter);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &curlBuffer);
	curl_easy_perform(curl);
    }

    static int curlWriter(char *data, int size, int nmemb, std::string *buffer) {
	// function prototype
	int result = 0;
	if (buffer != NULL) {
	    buffer->append(data, size * nmemb);
	    result = size * nmemb;
	}
	return result;
    }

    std::string getData() {
	return curlBuffer;
    }

private:
    CURL * curl;
    std::string curlBuffer;
};

void init_Map(std::unordered_map<std::string, std::string>& TypeMap) {
    // init the Map
    TypeMap["USD/JPY"] = "USDJPY=X";
    TypeMap["USD/HKD"] = "USDHKD=X";
    TypeMap["USD/CNY"] = "USDCNY=X";
    TypeMap["AUD/USD"] = "AUDUSD=X";
    TypeMap["USD/CAD"] = "CAD=X";
    TypeMap["USD/EUR"] = "USDEUR=X";
    TypeMap["GBP/USD"] = "GBPUSD=X";
    TypeMap["USD/THB"] = "USDTHB=X";
    TypeMap["USD/INR"] = "INR=X";

    TypeMap["Tencent"] = "0700.HK";
    TypeMap["ABC"] = "1288.HK";
    TypeMap["Nvida"] = "NVDA";
}

long parse(std::string date) {
    long ret;
    // parse the DD/MM/YYYY into epoch
    struct tm mytm;
    date += " 00:00:00";
    strptime(date.c_str(), "%d/%m/%Y %H:%M:%S", &mytm);

    ret = mktime(&mytm);

    return ret + 28800;
}


int main(int argc, char *argv[]) {
    bool ST = true; // input bool
    std::string Currency = argv[1]; // the input data type
    std::unordered_map<std::string, std::string> TypeMap;
    std::string URL = ""; // fetching URL
    std::string Period = argv[2];
    std::string Start = argv[3];
    std::string End = argv[4];

    int start = 0;
    int end = 0;

    init_Map(TypeMap);
    Currency = TypeMap[Currency];

    std::cout << "Decode your command and generate HTTP request ..." << std::endl;
    std::cout << std::endl;

    // #1.4 User input -> URL
    // 1. Period1 and Period2
    if (Start.size() != 10 || End.size() != 10) throw("Date format error!");
    
    start = parse(Start);
    end = parse(End);

    // 2. Period
    if (Period == "Daily") Period = "1d";
    else if (Period == "Weekly") Period = "1wk";
    else if (Period == "Monthly") Period = "1mo";
    else throw ("Invalid Period!");

    // 3. Conbine into URL
    URL = QUERY + Currency + '?' + "period1=" + std::to_string(start) + '&' + "period2=" + std::to_string(end) + '&' + "interval=" + Period + "&events=history&includeAdjustedClose=true";

    std::cout << "Generate Completed! Now sending the request ..." << std::endl;
    std::cout << std::endl;

    // Step #2 Fetch the CSV Data From Requested URL file and return the string
    CurlObj Test(URL); 
    // std::cout << Test.getData() << std::endl;

    // Step #3 Handle the Data
    HandleObj* TObj = new HandleObj(Test.getData());
    TObj->SMA();
    // TObj->print_monbias();
}
