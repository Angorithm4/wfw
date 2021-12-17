//===----------------------------------------------------------------------===//
//
//                         Carbon
//
// stock.cpp
//
// Identification: carbon/indicator/stock/stock.cpp
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

    int earn_sma(int offset, double money) {
	// sma earn money test
	this->offset = offset;
	this->money = money;
	char *ErrMsg;
	const std::string sql = "Select Close From " + this->company +  // sql statement
	    " where Date between \"" + start + "\" and \"" + end +  "\";";
	const char* csql = sql.c_str();
	const char* pzTail;

	// execute
	sqlite3_stmt* stmt = NULL;

	int retst = sqlite3_prepare_v2(db, csql, strlen(csql), &stmt, &pzTail);

	this->daycv = {};
	while ((retst = sqlite3_step(stmt)) == SQLITE_ROW) {
	    const char* Close = reinterpret_cast<const char*> (sqlite3_column_text(stmt, 0));
	    std::string sclose(Close);
	    if (sclose == "null") {
		continue;
	    }
	    daycv.push_back(std::stod(sclose));
	}


	// deal with daycv

	double curr;  // current stock price
	double losma; // last offset sma
	double lotal; // last offset total

	// calculate first offset days:
	for (int i = 0; i < offset; i++) {
	    lotal += daycv[i];
	}

	// then real stuff (iterator)
	int cshare = 0; // # shares of stock
	bool buyed = false; // whether we hold stock now

	std::vector<double>::iterator vdi = daycv.begin() + offset;

	for (auto it = vdi; it < daycv.end(); it++) {
	    // main loop
	    curr = *it; // daily close price
	    losma = lotal / offset;

	    if (curr < losma && !buyed) {
		buy(money, curr, cshare);
		buyed = true;
	    } else if (curr > losma && buyed) {
		sell(money, curr, cshare);
		buyed = false;
	    }

	    lotal -= *(it-offset);
	    lotal += curr;
	}

	if (buyed) {
	    money += cshare * curr;
	    this->highest = std::max(money, this->highest);
	}

	std::cout << std::endl;
	std::cout << "New Balance: " << std::setprecision(10) << money << std::endl;
	std::cout << "Highest Balance: " << std::setprecision(10) << this->highest << std::endl;

	sqlite3_close(db);
	return 1;
    }



private:
    std::string company;
    std::string start;
    std::string end;
    sqlite3 *db;      // the db to execute, default "tstockdaily.db"

    // for earn_sma
    int offset;   
    double money;  // initial money
    double highest;
    std::vector<double> daycv; // each day's close value


    void init_db() {
	// call after init, open the db table
	char *ErrMsg = 0;
	int retst = 0; // return status
	char *sql;

	// Open database
	retst = sqlite3_open("../../db/indicator/stockdaily.db", &db);

	if (retst) {
	    // if != 0, error
	    fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
	    return;
	} else {
	    // debug use
	    printf("Opened database successfully\n");
	}
    }

    void buy(double &money, double curr, int& shares) {
	// Use all money buy stock as much as I can
	shares = money / curr; // see how many shares we can buy
	double cost = curr * shares;
	money -= cost;
	std::cout << "Buy " << shares << " shares of stock, Account balance: $" 
	    << std::setprecision(10) << money << std::endl;
    }

    void sell(double &money, double curr, int& shares) {
	// Sell all stocks that I currently hold
	double earned = shares * curr;
	shares = 0;
	money += earned;
	this->highest = std::max(money, this->highest);
	std::cout << "Sell all stocks, Earned $" << std::setprecision(10) 
	    << earned << ", Account balance: $" << std::setprecision(10) << money << std::endl;
	std::cout << std::endl;
    }

};


int main(int argc, char** argv) {
    std::string type(argv[1]);
    if (type == "sma_single_earn") {
	if (argc != 7) {
	    std::cout << "Argument Error!" << std::endl;
	    return -1;
	}
	std::string company(argv[2]);
	std::string start(argv[3]);
	std::string end(argv[4]);
	int offset = std::stoi(argv[5]);
	int money = std::stoi(argv[6]);
	Stock *T = new Stock(company, start, end);
	T->earn_sma(offset, money);
    }
    return 1;
}

