#include<iostream>
#include<pqxx/pqxx>
#include<tuple>
#include<random>
#include<iostream>
#include<pqxx/pqxx>
#include<tuple>
#include<random>
#include<algorithm>
#include"util/pg.h"
int main(int argc, char* argv[]){
	util::Pg p("127.0.0.1","5432");
	p.connect("template1");
	p.execnt("SELECT * FROM pg_database");
	p.connect("test3");
	p.exect(
		"DROP TABLE IF EXISTS COMPANY;"
	);
	p.exect(
		"CREATE TABLE IF NOT EXISTS COMPANY("\
		"	ID INT PRIMARY KEY     NOT NULL,"\
		"	NAME           TEXT    NOT NULL,"\
		"	SURNAME	TEXT    NOT NULL,"\
		"	AGE            INT     NOT NULL,"\
		"	ADDRESS        CHAR(50),"\
		"	SALARY         REAL"\
		");"

	);
	std::random_device rd;
	std::mt19937 g(rd());
	std::uniform_int_distribution<> d(0,100);
	std::vector<std::string> names={
		"Jan",
		"Jana",
		"Piet",
		"Pietronella",
		"Klaas",
		"Katryn"
	};
	std::vector<std::string> surnames={
		"Grove",
		"Nel",
		"van Wyk",
		"Du Plesis"
	};
	std::vector<std::string> location={
		"Springbok",
		"Garies",
		"Koekenaap",
		"Pofadder",
		"Aggeneys",
		"Nababeep"
	};
	std::vector<int> ages={15,20,25,30,35,40,45,50,55,60,65,70};
	std::vector<int> salaries={1000,2000,3000,4000};
	std::vector<std::string> statements;
	std::ostringstream oss;
	for(int i=0;i<8192*4;i++){
		oss	<<"INSERT INTO COMPANY (ID,NAME,SURNAME,AGE,ADDRESS,SALARY) "<<std::endl
			<<"VALUES("<<std::endl
				<<i<<","<<std::endl
				<<"'"<<names[d(g)%names.size()]<<"',"<<std::endl
				<<"'"<<surnames[d(g)%surnames.size()]<<"',"<<std::endl
				<<ages[d(g)%ages.size()]<<","<<std::endl
				<<"'"<<location[d(g)%surnames.size()]<<"',"<<std::endl
				<<salaries[d(g)%salaries.size()]<<std::endl
			<<");"<<std::endl
		;
	}
	p.exect(
		oss.str()
	);
	pqxx::result r=p.execnt("SELECT * FROM COMPANY");
	//util::Pg::printr(r);
	p.disconnect();
	std::vector<int> foo = {3,5,7,11,13,17,19,23};
	if(std::all_of(foo.begin(), foo.end(), [](int i){return i%2;}))
		std::cout << "All the elements are odd numbers.\n";
	return 0;
}
