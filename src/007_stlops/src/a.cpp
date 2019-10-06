#include<iostream>
#include<pqxx/pqxx>
#include<tuple>
#include<random>
#include<iostream>
#include<pqxx/pqxx>
#include<tuple>
#include<random>
#include<algorithm>
#include<unordered_map>
#include<algorithm>
#include<vector>
#include"util/pg.h"
int main(int argc, char* argv[]){
	util::Pg p("127.0.0.1","5432");
	p.connect("template1");
	std::string dbnam="test3";
	if(p.execnt("SELECT * FROM pg_database WHERE datname = '"+dbnam+"'").size()>0){
		std::cout<<"Database "<<dbnam<<" exists, not creating"<<std::endl;
	}else{
		std::cout<<"Database "<<dbnam<<" does not exists, creating"<<std::endl;
		p.execnt("CREATE DATABASE "+dbnam);
	}
	p.connect(dbnam);
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
	std::uniform_int_distribution<> d(0,4096);
	std::vector<std::string> names={
		"Jan",
		"Jana",
		"Piet",
		"Pietronella",
		"Klaas",
		"Katryn"
	};
	std::vector<std::string> surnames={
		"Van Wyk",
		"Beukes",
		"Visagie",
		"Modise",
		"Swartz",
		"Booysen"
//A total of 377 Van Wyk babies were born in the year under review, followed by Louw (274), Cloete (201), Jacobs (173), Olyn (127), Visagie (116), Beukes (104), Modise (103), Swartz (94) and Booysen (90).
	};
	std::vector<std::string> location={
		"Barkly West",
		"Campbell",
		"Delportshoop",
		"Douglas",
		"Griquatown",
		"Hartswater",
		"Jan Kempdorp",
		"Kimberley",
		"Modder River",
		"Ulco",
		"Warrenton",
		"Windsorton",
		"Andriesvale",
		"Askham",
		"Augrabies",
		"Danielskuil",
		"Groblershoop",
		"Kakamas",
		"Kanoneiland",
		"Keimoes",
		"Kenhardt",
		"Lime Acres",
		"Louisvale",
		"Mier",
		"Olifantshoek",
		"Onseepkans",
		"Postmasburg",
		"Putsonderwater",
		"Riemvasmaak",
		"Upington",
		"Aggeneys",
		"Alexander Bay",
		"Carolusberg",
		"Concordia",
		"Garies",
		"Hondeklip",
		"Kamieskroon",
		"Kleinzee",
		"Nababeep",
		"Okiep",
		"Pella",
		"Pofadder",
		"Port Nolloth",
		"Soebatsfontein",
		"Springbok",
		"Steinkopf",
		"Britstown",
		"Colesberg",
		"Copperton",
		"De Aar",
		"Hanover",
		"Hopetown",
		"Hutchinson",
		"Loxton",
		"Marydale",
		"Norvalspont",
		"Noupoort",
		"Orania",
		"Petrusville",
		"Philipstown",
		"Prieska",
		"Richmond",
		"Strydenburg",
		"Vanderkloof",
		"Victoria West",
		"Vosburg"
	};
	std::vector<int> ages={15,20,25,30,35,40,45,50,55,60,65,70};
	std::vector<int> salaries={1000,2000,3000,4000};
	std::vector<std::string> statements;
	std::ostringstream oss;
	for(int i=0;i<32*4;i++){
		oss	<<"INSERT INTO COMPANY (ID,NAME,SURNAME,AGE,ADDRESS,SALARY) "<<std::endl
			<<"VALUES("<<std::endl
				<<i<<","<<std::endl
				<<"'"<<names[d(g)%names.size()]<<"',"<<std::endl
				<<"'"<<surnames[d(g)%surnames.size()]<<"',"<<std::endl
				<<ages[d(g)%ages.size()]<<","<<std::endl
				<<"'"<<location[d(g)%location.size()]<<"',"<<std::endl
				<<salaries[d(g)%salaries.size()]<<std::endl
			<<");"<<std::endl
		;
	}
	p.exect(
		oss.str()
	);
	pqxx::result r=p.execnt("SELECT * FROM COMPANY order by ID");
	//conversion to cols
	std::unordered_map<std::string,std::vector<std::string>> hmc;
	if(r.size()>0){
		for(
			pqxx::result::const_iterator row=r.begin();
			row!=r.end();
			++row
		){
			for(
				pqxx::row::const_iterator field=row->begin();
				field!=row->end();
				++field
			){

				//std::cout<<field->name()<<":";
				//std::cout<<field->c_str()<<'\t';
				hmc[field->name()].push_back(field->c_str());
			}
		}
	}else{
		std::cout<<"No results"<<std::endl;
	}
	//for(std::pair<std::string,std::vector<std::string>& col:hmc){
	//std::pair<const std::__cxx11::basic_string<char>, std::vector<std::__cxx11::basic_string<char> > >
	for(std::pair<std::string,std::vector<std::string>> itcol:hmc){
		std::cout<<"--------------------------------------------------------------------------------"<<std::endl;
		std::cout<<itcol.first<<std::endl;
		std::cout<<"--------------------------------------------------------------------------------"<<std::endl;
		std::vector<std::string>::iterator it=itcol.second.begin();
		while(it!=itcol.second.end()){
			it=std::adjacent_find(
				++it,
				itcol.second.end(),
				[](std::string i,std::string j){
					return (i==j);
				}
			);
			if(it!=itcol.second.end())
				std::cout<<std::distance(itcol.second.begin(),it)<<"\t"<<*it<<std::endl;//*std::prev(it)<<std::endl;

			}
		}

	p.disconnect();
	return 0;
}


