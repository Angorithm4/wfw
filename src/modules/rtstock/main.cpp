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
    /**
     * @brief Constructor for a Data object.
     *
     * @param NAME The ticker of the company.
     * @param HTML The html to pull info from.
     */
    Data (std::string NAME, std::string HTML) {
	name = NAME;
	html = HTML;
	fillData();
    }
    
    /**
     * @brief Picks through the html to find the needed data.
     */
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
	std::cout << Want << std::endl;

    }
    
    /**
     * @brief Prints out all the stock data for a company.
     */
    void printData() {
	std::cout << "\n" << name << ":" << std::endl;
	std::cout << "\tCURRENT\t\t" << current << "\n\tLOW\t\t\t" << low <<
		    "\n\tHIGH\t\t" << high << "\n\tCHANGE\t\t" << sign << chg <<
		    "\n\t%CHANGE\t\t" << sign << pChg << "%" << std::endl;
    }
};


//CurlObj is used to get the html from the given webpage
class CurlObj {
public:
    /**
     * @brief Constructor for a curl object.
     *
     * @param url The url of the page to scrape.
     */
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
//credit to stack overflow for help on the CurlObj class

int main() {
    
    //the tickers of companies
    std::vector<std::string> names;
    std::string numCompaniesString;
    
    //get number of companies to research
    std::cout << "Enter the number of companies whose stocks you would like to analyze: ";
    int numCompanies = -1;
    while (numCompanies < 0) {
        try {
            std::cin >> numCompaniesString;
            numCompanies = std::stoi(numCompaniesString);
            if (numCompanies < 0 ) {
                std::cout << "That is not a valid number. Enter any number greater than 0: ";
            }
        } catch (std::exception e) {
            std::cout << "That is not a valid number. Enter any number greater than 0: ";
        }
    }
    
    //enter the wanted companies' tickers
    for (int i = 0; i < numCompanies; ++i) {
        std::string name;
        std::cout << "Enter the ticker symbol of a company: ";
        std::cin >> name;
        std::transform(name.begin(), name.end(), name.begin(), ::tolower);
        names.push_back(name);
    }
    
    std::cout << "Please wait for your data. This may take a few seconds." << std::endl << std::endl;
    
    std::vector<Data> dataObjs;
    
    //get all the data and print it out
    for (size_t i = 0; i < names.size(); i++) {
        std::string name = names[i];
        std::string address = "http://finance.yahoo.com/quote/" + name;
        CurlObj temp(address);
	Data* data = new Data(name, temp.getData());

        // try {
            // Data* data = new Data(name, temp.getData());
//            data->printData();
        // } catch (std::exception e) {
          //  std::cout << "\n" << name << " is not a valid ticker." << std::endl;
        // }
    }
    
    std::cout << std::endl;
    
    return 0;
}
