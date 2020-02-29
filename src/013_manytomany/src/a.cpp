/*
 * libpqxx many-to-many demo
 */
#include<iostream>
#include<pqxx/pqxx>
#include<vector>
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
				drop table if exists a cascade
			)";
			work W(C);
			W.exec(buffer.str().c_str());
			W.commit();
		}
		{
			buffer.str("");
			buffer<<R"(
				drop table if exists b cascade
			)";
			work W(C);
			W.exec(buffer.str().c_str());
			W.commit();
		}
		{
			buffer.str("");
			buffer<<R"(
				drop table if exists a_b cascade
			)";
			work W(C);
			W.exec(buffer.str().c_str());
			W.commit();
		}
		//create a
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
		//create b
		{
			buffer.str("");
			buffer<<R"(
				create table if not exists b(
					b_id serial primary key,
					value varchar
				)
			)";
			work W(C);
			W.exec(buffer.str().c_str());
			W.commit();
		}
		//create a_b
		{
			buffer.str("");
			buffer<<R"(
				create table if not exists a_b(
					a_id int references a(a_id),
					b_id int references b(b_id)
				)
			)";
			work W(C);
			W.exec(buffer.str().c_str());
			W.commit();
		}
		//populate a
		{
			buffer.str("");
			buffer<<R"(
				insert into a(value) values($1) returning a_id
			)";
			C.prepare("ps0",buffer.str().c_str());
			for(int i=0;i<sza;i++){
				nontransaction N(C);
				result R(N.exec_prepared("ps0","ATest"+std::to_string(i)));
				N.commit();
				vaid.push_back(R[0][0].as<int>());
			}
		}
		//populate b
		{
			buffer.str("");
			buffer<<R"(
				insert into b(value) values($1) returning b_id
			)";
			C.prepare("ps1",buffer.str().c_str());
			for(int i=0;i<sza;i++){
				nontransaction N(C);
				result R(N.exec_prepared("ps1","BTest"+std::to_string(i)));
				N.commit();
				vbid.push_back(R[0][0].as<int>());
			}
		}
		//populate a_b
		{
			for(auto aid:vaid){
				for(auto bid:vbid){
					buffer.str("");
					buffer<<R"(
						insert into a_b(a_id,b_id)values($1,$2) 
					)";
					C.prepare("ps2",buffer.str().c_str());
					work W(C);
					W.prepared("ps2")(aid)(bid).exec();
					W.commit();

				}
			}
		}
		{
			buffer.str("");
			buffer<<R"(
				select 
				    a.a_id as a_id,
				    b.b_id as b_id,
				    a.value as aval,
				    b.value as bval
				from
				    a
				join
				    a_b
				on
				    a.a_id=a_b.b_id
				join
				    b
				on
				    b.b_id=a_b.b_id
				;
			)";
			C.prepare("ps3",buffer.str().c_str());//how to remove prepared statement?
			nontransaction N(C);
			result R(N.exec_prepared("ps3"));
			for(int i=0;i<R.columns();i++){
				std::cout<<R.column_name(i)<<",";
			}
			std::cout<<std::endl<<"----------------------------------------"<<std::endl;
			for(result::const_iterator c=R.begin();c!=R.end();++c){
				std::cout<<c[0].as<int>()<<",";
				std::cout<<c[1].as<int>()<<",";
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

