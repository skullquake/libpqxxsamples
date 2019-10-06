#include<iostream>
#include<pqxx/pqxx>
#include<tuple>
#include<random>
int main(int argc, char* argv[]){
	pqxx::connection* c;
	pqxx::result r;
	std::string host("127.0.0.1");
	std::string port("5432");
	try{
		//--------------------------------------------------------------------------------
		//dummy connect (to template1 to create test for dbnam exists and ?create dbnam)
		//--------------------------------------------------------------------------------
		c=new pqxx::connection(
			"dbname = template1 hostaddr = "+host+" port = "+port
		);
		if(c->is_open()){
			std::cout<<"Opened database successfully: "<<c->dbname()<<std::endl;
		}else{
			std::cout<<"Can't open database"<<std::endl;
			return 1;
		}
		//--------------------------------------------------------------------------------
		//create database
		//--------------------------------------------------------------------------------
		std::string dbnam="test3";
		r=pqxx::nontransaction(*c).exec(std::string("SELECT 1 FROM pg_database WHERE datname = '"+dbnam+"'").c_str());
		if(r.size()>0){
			std::cout<<"Database found"<<std::endl;
		}else{
			std::cout<<"Database not found:creating..."<<std::endl;
			pqxx::nontransaction N(*c);
			r=N.exec(std::string("CREATE DATABASE "+dbnam).c_str());
		}
		//--------------------------------------------------------------------------------
		//disconnect
		//--------------------------------------------------------------------------------
		c->disconnect();
		delete(c);
		//--------------------------------------------------------------------------------
		//connect to actual database
		//--------------------------------------------------------------------------------
		c=new pqxx::connection(
			std::string("dbname = "+dbnam+" hostaddr = "+host+" port = "+port).c_str()
		);
		if(c->is_open()){
			std::cout<<"Opened database successfully: "<<c->dbname()<<std::endl;
		}else{
			std::cout<<"Can't open database"<<std::endl;
			return 1;
		}
		//--------------------------------------------------------------------------------
		//create table
		//--------------------------------------------------------------------------------
		{
			pqxx::work w(*c);
			try{
				w.exec(
					std::string(
						"CREATE TABLE IF NOT EXISTS COMPANY("\
						"	ID INT PRIMARY KEY     NOT NULL,"\
						"	NAME           TEXT    NOT NULL,"\
						"	SURNAME	TEXT    NOT NULL,"\
						"	AGE            INT     NOT NULL,"\
						"	ADDRESS        CHAR(50),"\
						"	SALARY         REAL"\
						");"
					)
				);
				w.commit();
				std::cout<<"Table created successfully"<<std::endl;
			}catch(const std::exception& e){
				std::cerr<<e.what()<<std::endl;
				w.abort();
				throw;
			}
		}
		//--------------------------------------------------------------------------------
		//clean table
		//--------------------------------------------------------------------------------
		{
			pqxx::work w(*c);
			try{
				w.exec(
					std::string(
						"DELETE FROM COMPANY;"
					)
				);
				w.commit();
				std::cout<<"Table cleared successfully"<<std::endl;
			}catch(const std::exception& e){
				w.abort();
				throw;
			}
		}
		//--------------------------------------------------------------------------------
		//populate
		//--------------------------------------------------------------------------------
		{
			pqxx::work w(*c);
			try{
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
				for(int i=0;i<8192;i++){
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
				w.exec(
					oss.str()
				);
				w.commit();
				std::cout<<"Table populated successfully"<<std::endl;
			}catch(const std::exception& e){
				w.abort();
				throw;
			}
		}
		//--------------------------------------------------------------------------------
		//select
		//--------------------------------------------------------------------------------
		{
			try{
				r=pqxx::nontransaction(*c).exec(std::string("SELECT * FROM COMPANY;").c_str());
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
						)
							std::cout<<field->c_str()<<'\t';
						std::cout<<std::endl;
					}
				}else{
					std::cout<<"No results"<<std::endl;
				}
			}catch(const std::exception& e){
				std::cerr<<e.what()<<std::endl;
			}

		}
		//--------------------------------------------------------------------------------
		//disconnect
		//--------------------------------------------------------------------------------
		c->disconnect();
		delete(c);
	}catch(const std::exception &e){
		std::cerr<<e.what()<<std::endl;
		return 1;
	}
   return 0;
}
void sampleSelect(pqxx::connection *c){
	//--------------------------------------------------------------------------------
	//select databases
	//--------------------------------------------------------------------------------
	pqxx::result r;
	r=pqxx::nontransaction(*c).exec(std::string("SELECT * FROM pg_database WHERE datname = 'test'").c_str());
	if(r.size()>0){
		//for loop
		/*
		for(int rownum=0;rownum<r.size();++rownum){
			//const result::tuple row=r[rownum];//v???
			//const pqxx::tuple row=r[rownum];//v???
			const pqxx::row row=r[rownum];
			for(int colnum=0;colnum<row.size();++colnum){
				//const result::field=row[colnum];//v???
				const pqxx::field field=row[colnum];
				std::cout<<field.c_str()<<'\t';
			}
			std::cout<<std::endl;
		}
		*/
		//iterator
		for(
			pqxx::result::const_iterator row=r.begin();
			row!=r.end();
			++row
		){
			for(
				//pqxx::result::tuple::const_iterator field=row->begin();
				pqxx::row::const_iterator field=row->begin();
				field!=row->end();
				++field
			)
				std::cout<<field->c_str()<<'\t';
			std::cout<<std::endl;
		}
	}else{
		std::cout<<"No results"<<std::endl;
	}
}
