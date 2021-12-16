//===----------------------------------------------------------------------===//
//
//                         Carbon
//
// stock.cpp
//
// Identification: carbon/modules/stock/stock.cpp
//
//
// Last Modified : 2021.12.17 Jiawei Wang
//
// Copyright (c) 2021 Angorithm4
//
//===----------------------------------------------------------------------===//

#include <sqlite3.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <iomanip>
#include <vector>
#include <string>


class Stock {
    // Stock object, dealing with all stock-price related operations
public:
    Stock(std::string company, std::string start, std::string end) {
	// initilaize 
	this->company = company;
	this->start = start;
	this->end = end;
	init_db(); // open the db table
    }



    void earn_sma(int offset, double money) {
	// sma earn money test
	offset = offset;
	money = money;
	char *ErrMsg;
	const std::string sql = "Select Close From " + this->company +  // sql statement
	    " where Date between \"" + start + "\" and \"" + end +  "\";";
	const char* csql = sql.c_str();

	int retst = sqlite3_exec(db, csql, earn_sma_callback, (void*) errmsg, &ErrMsg); // execute

	if (retst != SQLITE_OK) {
	    fprintf(stderr, "SQL error: %s\n", ErrMsg);
	    sqlite3_free(ErrMsg);
	} else {
	    // debug
	    printf("Operation done successfully\n");
	}

	sqlite3_close(db);
	return;
    }



private:
    std::string company;
    std::string start;
    std::string end;
    const char* errmsg = "Callback function called";
    sqlite3 *db;      // the db to execute, default "tstockdaily.db"

    // for earn_sma
    static int offset;   
    static double money;  // initial money
    static double highest;

    void init_db() {
	// call after init, open the db table
	char *ErrMsg = 0;
	int retst = 0; // return status
	char *sql;

	// Open database
	retst = sqlite3_open("tstockdaily.db", &db);

	if (retst) {
	    // if != 0, error
	    fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
	    return;
	} else {
	    // debug use
	    printf("Opened database successfully\n");
	}
    }

    static void buy(double &money, double curr, int& shares) {
	// Use all money buy stock as much as I can
	shares = money / curr; // see how many shares we can buy
	double cost = curr * shares;
	money -= cost;
	std::cout << "Buy " << shares << " shares of stock, Account balance: $" 
	    << std::setprecision(10) << money << std::endl;
    }

    static void sell(double &money, double curr, int& shares) {
	// Sell all stocks that I currently hold
	double earned = shares * curr;
	shares = 0;
	money += earned;
	highest = std::max(money, highest);
	std::cout << "Sell all stocks, Earned $" << std::setprecision(10) 
	    << earned << ", Account balance: $" << std::setprecision(10) << money << std::endl;
	std::cout << std::endl;
    }


    static int earn_sma_callback(void *errmsg, int argc, char **argv, char **col) {
	// interface of sqlite execute deal with the data after execute sql in earn_sma()
	
	// Deal with the data, I only need the char **argv, which means the array of strings
	
	double curr;  // current stock price
	double losma; // last offset sma
	double lotal; // last offset total
	char *cp = *col;     // current char pointer
	highest = 0;

	// calculate first offset days:
	for (int i = 0; i < offset; i++) {
	    cp++;
	    lotal += atof(cp);
	}

	// then real stuff
	int t = argc;
	int times = 9;
	int cshare = 0;      // # shares of stock
	bool buyed = false;  // whether we hold stock now

	while (times < t) {
	    // main loop
	    curr = atof(cp+1); // day closing price
	    losma = lotal / offset; // last offset days sma

	    if (curr < losma && !buyed) {
		// if today's closing price less than last offset days sma
	        buy(money, curr, cshare);
		buyed = true;
	    } else if (curr > losma && buyed) {
		sell(money, curr, cshare);
		buyed = false;
	    }

	    
	    lotal -= atof(cp-offset); 
	    lotal += curr;

	    cp += 1;
	    times++;
	}
	
	if (buyed) {
	    money += cshare * curr;
	    highest = std::max(money, highest);
	}

	std::cout << std::endl;
	std::cout << "New Balance: " << std::setprecision(10) << money << std::endl;
	std::cout << "Highest Balance: " << std::setprecision(10) << highest << std::endl;

	return 0;
    }
};


int main() {
    Stock *T = new Stock("Tencent", "2007-01-01", "2021-01-01");
    std::cout << "OK" << std::endl;
    T->earn_sma(10, 100000);
    return 1;
}

























