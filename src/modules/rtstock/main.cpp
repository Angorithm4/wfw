#include <iostream>
#include <curl/curl.h>
#include <vector>
#include <map>
#include <algorithm>
#include <string>

class Data {
private:
    std::string html;
    std::string sign;
    std::string name;
    double low;
    double high;
    double current;
    double chg;
    double pChg;
    
public:
    Data (std::string NAME, std::string HTML) {
	name = NAME;
	html = HTML;
	fillData();
    }
    
    void fillData() {
	std::string title = "data-reacteid=\"29\">";
	std::string key = "<span class=\"Trsdu(0.3s) Fw(b) Fz(36px) Mb(-4px) D(ib)\" data-reactid=\"29\">";


	size_t indexTitle = html.find(key) + key.size();
	int offset = 0;
	for (int i = indexTitle; i < INT_MAX; i++) {
	    if (html[i] - '0' <= 9 && html[i] - '0' >= 0 || html[i] == '.' || html[i] == ',') 
		offset++;
	    else break;
	}
	std::string Want = html.substr(indexTitle, offset);
	if (Want == "") std::cout << "Closed" << std::endl;
	else std::cout << Want << std::endl;
    }
    
};


//CurlObj is used to get the html from the given webpage
class CurlObj {
public:
    CurlObj (std::string url) {
        curl = curl_easy_init();
        if (!curl) {
            throw std::string ("Curl did not initialize.");
        }

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &CurlObj::curlWriter);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &curlBuffer);
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
        curl_easy_perform(curl);
    };
    
    static int curlWriter(char *data, int size, int nmemb, std::string *buffer) {
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
    
protected:
    CURL * curl;
    std::string curlBuffer;
};

int main(int argc, char *argv[]) {
    //the tickers of companies
    std::string name = argv[1];
    
    std::vector<Data> dataObjs;
    
    //get all the data and print it out
    std::string address = "http://finance.yahoo.com/quote/" + name;
    CurlObj temp(address);
    Data* data = new Data(name, temp.getData());
    
    return 0;
}
