
#include<iostream>
#include<pqxx/pqxx>
using namespace std;
using namespace pqxx;
int main(int argc, char* argv[]){
	std::stringstream buffer;
	try{
		connection C(R"(
dbname=test
user=test
password=1
hostaddr=127.0.0.1
port=6000
)"
		);
	if(!C.is_open()){
		std::cerr<<"Can't open database"<<std::endl;
		return 1;
	}
	{
		buffer.str("");
		buffer<<R"(
			drop table if exists a cascade
		)";
		work W(C);
		W.exec(buffer.str().c_str());
		W.commit();
	}
	{
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
	}
	{
		buffer.str("");
		buffer<<R"(
			delete from a
		)";
		work W(C);
		W.exec(buffer.str().c_str());
		W.commit();
	}
	{
		buffer.str("");
		buffer<<R"(
			insert into a(value) values($1)
		)";
		C.prepare("ps0",buffer.str().c_str());
		work W(C);
		for(int i=0;i<8;i++){
			W.prepared("ps0")("Test"+std::to_string(i)).exec();
		}
		W.commit();
	}
	{
		buffer.str("");
		buffer<<R"(
			select a_id,value from a
		)";
		nontransaction N(C);
		result R(N.exec(buffer.str().c_str()));
		for(int i=0;i<R.columns();i++){
			std::cout<<R.column_name(i)<<",";
		}
		std::cout<<std::endl<<"----------------------------------------"<<std::endl;
		for(result::const_iterator c=R.begin();c!=R.end();++c){
			std::cout<<c[0].as<int>()<<",";
			std::cout<<c[1].as<std::string>();
			std::cout<<std::endl;
		}
	}
	C.disconnect();
	}catch(const std::exception &e){
		std::cerr<<e.what()<<std::endl;
		return 1;
	}
	return 0;
}
