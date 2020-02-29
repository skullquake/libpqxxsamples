/*
 * libpqxx generate_series
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
				select * from generate_series(2,4)
			)";
			C.prepare("ps4",buffer.str().c_str());
			nontransaction N(C);
			result R(N.exec_prepared("ps4"));
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
			N.commit();
		}
		{
			buffer.str("");
			buffer<<R"(
				select * from generate_series(10,0,-2)
			)";
			C.prepare("ps3",buffer.str().c_str());
			nontransaction N(C);
			result R(N.exec_prepared("ps3"));
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
			N.commit();
		}

		C.disconnect();
	}catch(const std::exception &e){
		std::cerr<<e.what()<<std::endl;
		return 1;
	}
	return 0;
}

