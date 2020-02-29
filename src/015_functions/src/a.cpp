/*
 * libpqxx function example
 */
#include<iostream>
#include<pqxx/pqxx>
#include<vector>
#include<tuple>
using namespace std;
using namespace pqxx;
int main(int argc, char* argv[]){
	std::stringstream buffer;
	int sza=4;
	int szb=4;
	std::vector<int> vaid;
	std::vector<int> vbid;
	try{
		connection C(R"(
			dbname=test
			user=test
			password=1
			hostaddr=127.0.0.1
			port=6000
		)");
		if(!C.is_open()){
			std::cerr<<"Can't open database"<<std::endl;
			return 1;
		}
		{
			buffer.str("");
			buffer<<R"(
				drop function if exists fn_test;
				create or replace function fn_test(a integer)
					returns integer as
				$$
				declare
					ret int;
				begin
					ret:=a*10;
					return ret;
				end;
				$$
				language plpgsql
			)";
			work w(C);
			w.exec(buffer.str().c_str());
			w.commit();
		}
		{
			buffer.str("");
			buffer<<R"(
				select * from fn_test($1)
			)";
			C.prepare("ps0",buffer.str().c_str());
			work w(C);
			pqxx::result R=w.prepared("ps0")(1234).exec();
			for(int i=0;i<R.columns();i++){
				std::cout<<R.column_name(i)<<",";
			}
			std::cout<<std::endl<<"----------------------------------------"<<std::endl;
			for(auto r=R.begin();r!=R.end();++r){
				for(auto f=r->begin();f!=r->end();++f){
					std::cout<<f.as<std::string>()<<",";
				}
				std::cout<<std::endl;
			}
			w.commit();
		}

		C.disconnect();
	}catch(const std::exception &e){
		std::cerr<<e.what()<<std::endl;
		return 1;
	}
	return 0;
}

