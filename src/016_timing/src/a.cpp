#include<iostream>
#include<pqxx/pqxx>
#include<chrono>
using namespace std;
using namespace pqxx;
int main(int argc, char* argv[]){
	std::stringstream buffer;
	try{
		connection C(R"(
dbname=test
user=postgres
password=1234!@#$qwerQWER
hostaddr=127.0.0.1
port=5432
)"
		);
	if(!C.is_open()){
		std::cerr<<"Can't open database"<<std::endl;
		return 1;
	}
	{
		std::chrono::time_point<std::chrono::system_clock> t0=std::chrono::system_clock::now();
		std::cout<<std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count()<<": "<<"Dropping table..."<<std::endl;
		buffer.str("");
		buffer<<R"(
			drop table if exists a cascade
		)";
		work W(C);
		W.exec(buffer.str().c_str());
		W.commit();
		std::cout<<std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count()<<": "<<"Done"<<std::endl;
		std::chrono::time_point<std::chrono::system_clock> t1=std::chrono::system_clock::now();
		auto td=std::chrono::duration_cast<std::chrono::milliseconds>(t1-t0);
		std::cout<<std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count()<<": "<<"Elapsed: "<<td.count()<<std::endl;
	}
	{
		std::chrono::time_point<std::chrono::system_clock> t0=std::chrono::system_clock::now();
		std::cout<<std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count()<<": "<<"Creating table..."<<std::endl;
		buffer.str("");
		buffer<<R"(
			create table if not exists a(
				a_id serial primary key,
				value varchar
			)
		)";
		work W(C);
		W.exec(buffer.str().c_str());
		W.commit();
		std::cout<<std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count()<<": "<<"Done"<<std::endl;
		std::chrono::time_point<std::chrono::system_clock> t1=std::chrono::system_clock::now();
		auto td=std::chrono::duration_cast<std::chrono::milliseconds>(t1-t0);
		std::cout<<std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count()<<": "<<"Elapsed: "<<td.count()<<std::endl;
	}
	{
		std::chrono::time_point<std::chrono::system_clock> t0=std::chrono::system_clock::now();
		std::cout<<std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count()<<": "<<"Deleting table rows..."<<std::endl;
		buffer.str("");
		buffer<<R"(
			delete from a
		)";
		work W(C);
		W.exec(buffer.str().c_str());
		W.commit();
		std::cout<<std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count()<<": "<<"Done"<<std::endl;
		std::chrono::time_point<std::chrono::system_clock> t1=std::chrono::system_clock::now();
		auto td=std::chrono::duration_cast<std::chrono::milliseconds>(t1-t0);
		std::cout<<std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count()<<": "<<"Elapsed: "<<td.count()<<std::endl;
	}
	{
		std::chrono::time_point<std::chrono::system_clock> t0=std::chrono::system_clock::now();
		std::cout<<std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count()<<": "<<"Inserting rows..."<<std::endl;
		buffer.str("");
		buffer<<R"(
			insert into a(value) values($1)
		)";
		C.prepare("ps0",buffer.str().c_str());
		work W(C);
		for(int i=0;i<32;i++){
			//std::cout<<std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count()<<": "<<"Inserting row "<<i<<"..."<<std::endl;
			W.prepared("ps0")("Test"+std::to_string(i)).exec();
		}
		W.commit();
		std::cout<<std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count()<<": "<<"Done..."<<std::endl;
		std::chrono::time_point<std::chrono::system_clock> t1=std::chrono::system_clock::now();
		auto td=std::chrono::duration_cast<std::chrono::milliseconds>(t1-t0);
		std::cout<<std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count()<<": "<<"Elapsed: "<<td.count()<<std::endl;
	}
	{
		std::chrono::time_point<std::chrono::system_clock> t0=std::chrono::system_clock::now();
		std::cout<<std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count()<<": "<<"Selecting values..."<<std::endl;
		buffer.str("");
		buffer<<R"(
			select a_id,value from a
		)";
		nontransaction N(C);
		result R(N.exec(buffer.str().c_str()));
		for(int i=0;i<R.columns();i++){
			//std::cout<<R.column_name(i)<<",";
		}
		//std::cout<<std::endl<<"----------------------------------------"<<std::endl;
		for(result::const_iterator c=R.begin();c!=R.end();++c){
			//std::cout<<c[0].as<int>()<<",";
			//std::cout<<c[1].as<std::string>();
			//std::cout<<std::endl;
		}
		std::cout<<std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count()<<": "<<"Done"<<std::endl;
		std::chrono::time_point<std::chrono::system_clock> t1=std::chrono::system_clock::now();
		auto td=std::chrono::duration_cast<std::chrono::milliseconds>(t1-t0);
		std::cout<<std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count()<<": "<<"Elapsed: "<<td.count()<<std::endl;
	}
	C.disconnect();
	}catch(const std::exception &e){
		std::cerr<<e.what()<<std::endl;
		return 1;
	}
	return 0;
}
