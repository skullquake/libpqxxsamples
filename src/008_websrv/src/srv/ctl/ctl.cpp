#include"srv/ctl/ctl.h"
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
//#include"util/pg.h"
#include"srv/view/page.h"
#include"srv/view/layout.h"
#include"srv/view/table.h"
#include"srv/view/menu.h"
namespace srv::ctl{
	void rs2table(){
	}
	void dbhtml(Mongoose::Request &request,Mongoose::StreamResponse &response,util::Pg& p){
		response.setHeader("Content-type","text/html");
		view::Page page;
		view::Layout layout;
		view::Menu menu;
		std::vector<std::pair<std::string,std::string>> vmnuitm={
			std::make_pair("Drop",		"/xas?cmd=dbdrop"),
			std::make_pair("Init",		"/xas?cmd=dbinit"),
			std::make_pair("HTML",		"/xas?cmd=dbhtml"),
			std::make_pair("List",		"/xas?cmd=dblist"),
			std::make_pair("Update",	"/xas?cmd=dbupdate")
		};
		for(
			std::vector<std::pair<std::string,std::string>>::const_iterator itmnuitm=vmnuitm.begin();
			itmnuitm!=vmnuitm.end();
			itmnuitm++
		){
			menu.addItem(*itmnuitm);
		}
		layout.getMenu()<<std::move(menu.toHtml());
		view::Table _table;
		//p.connect("test3");
		pqxx::result r=p.execnt("SELECT * FROM COMPANY order by ID");
		if(r.size()>0){
			//print header
			std::vector<std::string> vhdr;
			pqxx::result::const_iterator row=r.begin();
			for(
				pqxx::row::const_iterator field=row->begin();
				field!=row->end();
				++field
			){

				vhdr.push_back(field->name());
			}
			_table.setHeader(vhdr);
			for(
				pqxx::result::const_iterator row=r.begin();
				row!=r.end();
				++row
			){
				std::vector<std::string> vdat;
				for(
					pqxx::row::const_iterator field=row->begin();
					field!=row->end();
					++field
				){

					vdat.push_back(field->c_str());
				}
				_table.addRow(vdat);
			}
		}
		/*
		std::vector<std::string> vhdr={"hdr0","hdr1","hdr2"};
		std::vector<std::string> vdat={"foo","bar","baz"};
		_table.setHeader(vhdr);
		_table.addRow(vdat);
		_table.addRow(vdat);
		_table.addRow(vdat);
		*/
		layout.getBody()<<std::move(_table.toHtml());
		Json::FastWriter fw;
		HTML::Element table("table");
		table.addAttribute("class","table table-striped");
		layout.getBody()<<(HTML::Script()<<"\
			//alert(dojo.query('.personview'));\
	"
		);
		page.getDocument()<<std::move(layout.getMenu());;
		page.getDocument()<<std::move(layout.getBody());;
		response<<page;
	}
	void dbupdate(Mongoose::Request &request,Mongoose::StreamResponse &response,util::Pg& p){
		std::string dbnam="test3";
		/*
		p.connect("template1");
		if(p.execnt("SELECT * FROM pg_database WHERE datname = '"+dbnam+"'").size()>0){
			std::cout<<"Database "<<dbnam<<" exists, not creating"<<std::endl;
		}else{
			std::cout<<"Database "<<dbnam<<" does not exists, creating"<<std::endl;
			p.execnt("CREATE DATABASE "+dbnam);
		}
		*/
		p.connect(dbnam);
		/*
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
		*/
		p.exect(
			"DELETE FROM COMPANY;"
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
		response.setHeader("Content-type","text/plain");
		if(r.size()>0){
			//print header
			pqxx::result::const_iterator row=r.begin();
			for(
				pqxx::row::const_iterator field=row->begin();
				field!=row->end();
				++field
			){

				response<<field->name()<<"\t";
			}
			response<<std::endl;
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

					//response<<field->name()<<":";
					response<<field->c_str()<<'\t';
					//hmc[field->name()].push_back(field->c_str());
				}
				response<<std::endl;
			}
		}else{
			std::cout<<"No results"<<std::endl;
		}
		//p.disconnect();
	}
	void dbinit(Mongoose::Request &request,Mongoose::StreamResponse &response,util::Pg& p){
		std::string dbnam="test3";
		p.connect("template1");
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
		response<<"database initialized";
	}
	void dbdrop(Mongoose::Request &request,Mongoose::StreamResponse &response,util::Pg& p){
		std::string dbnam="test3";
		p.connect("template1");
		if(p.execnt("drop database "+dbnam).size()>0){
			p.execnt("drop database "+dbnam);
			cout<<"Database "<<dbnam<<" dropped"<<std::endl;
		}else{
			std::cout<<"Database "<<dbnam<<" does not exists"<<std::endl;
		}
	}
	void dbtbldrop(Mongoose::Request &request,Mongoose::StreamResponse &response,util::Pg& p){
		std::string dbnam="test3";
		p.connect(dbnam);
		p.exect(
			"delete from COMPANY;"
		);
		response<<"table dropped"<<std::endl;
	}
	void dbtblclr(Mongoose::Request &request,Mongoose::StreamResponse &response,util::Pg& p){
		std::string dbnam="test3";
		p.connect(dbnam);
		p.exect(
			"DROP TABLE IF EXISTS COMPANY;"
		);
		response<<"table cleared"<<std::endl;
	}
	MyController::MyController()
	             :Mongoose::WebController(),
		      p("127.0.0.1","5000")
	{
		p.connect("test3");
	}
	MyController::~MyController(){
		p.disconnect();
	}
	void MyController::hello(Mongoose::Request &request, Mongoose::StreamResponse &response){
		response<<"hello";
	}
	void MyController::xas(Mongoose::Request &request, Mongoose::StreamResponse &response){
		std::string cmd=request.get("cmd");
		if(cmd.length()>0){
				if(!cmd.compare("dbinit")){
					dbinit(request,response,this->p);
					response.setCode(303);
					response.setHeader("Content-Location","/xas?cmd=dbhtml");
					response.setHeader("Location","/xas?cmd=dbhtml");
				}else
				if(!cmd.compare("dbupdate")){
					dbupdate(request,response,this->p);
					response.setCode(303);
					response.setHeader("Content-Location","/xas?cmd=dbhtml");
					response.setHeader("Location","/xas?cmd=dbhtml");
				}else
				if(!cmd.compare("dbdrop")){
					dbdrop(request,response,this->p);
					response.setCode(303);
					response.setHeader("Content-Location","/xas?cmd=dbhtml");
					response.setHeader("Location","/xas?cmd=dbhtml");
				}else
				if(!cmd.compare("dbtbldrop")){
					dbtbldrop(request,response,this->p);
					response.setCode(303);
					response.setHeader("Content-Location","/xas?cmd=dbhtml");
					response.setHeader("Location","/xas?cmd=dbhtml");
				}else
				if(!cmd.compare("dbtblclr")){
					dbtblclr(request,response,this->p);
					response.setCode(303);
					response.setHeader("Content-Location","/xas?cmd=dbhtml");
					response.setHeader("Location","/xas?cmd=dbhtml");
				}else
				if(!cmd.compare("dbhtml")){
					dbhtml(request,response,this->p);
				}else
				if(!cmd.compare("qux")){
					response<<"xas:command qux";
				}else{
					response<<"xas:invalid command";
				}
		}else{
			response<<"xas:narg";
		};
	}
	void MyController::setup(){
		addRoute("GET","/hello",MyController,hello);
		addRoute("GET","/xas",MyController,xas);
	}
}





