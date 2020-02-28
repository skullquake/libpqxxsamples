/*
 * libpqxx one-to-one demo
 */
#include<iostream>
#include<pqxx/pqxx>
using namespace std;
using namespace pqxx;
int main(int argc, char* argv[]){
	std::stringstream buffer;
	int sza=4;
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
				create table if not exists b(
					b_id serial primary key,
					a_id int unique references a(a_id),
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
				insert into a(value) values($1) returning a_id
			)";
			C.prepare("ps0",buffer.str().c_str());
			for(int i=0;i<sza;i++){
				nontransaction N(C);
				result R(N.exec_prepared("ps0","ATest"+std::to_string(i)));
				N.commit();
				int serial=R[0][0].as<int>();
				{
					buffer.str("");
					buffer<<R"(
						insert into b(a_id,value) values($1,$2)
					)";
					C.prepare("ps1",buffer.str().c_str());
					work W(C);
					W.prepared("ps1")(serial)("BTest"+std::to_string(i)).exec();
					W.commit();
				}
			}
		}
		{
			buffer.str("");
			buffer<<R"(
				select
				    a.a_id,
				    a.value,
				    b.b_id,
				    b.value
				from
				    a
				left outer join
				    b
				on
				    a.a_id=b.a_id
			)";
			C.prepare("ps2",buffer.str().c_str());//how to remove prepared statement?
			nontransaction N(C);
			result R(N.exec_prepared("ps2"));
			for(int i=0;i<R.columns();i++){
				std::cout<<R.column_name(i)<<",";
			}
			std::cout<<std::endl<<"----------------------------------------"<<std::endl;
			for(result::const_iterator c=R.begin();c!=R.end();++c){
				std::cout<<c[0].as<int>()<<",";
				std::cout<<c[1].as<std::string>()<<",";
				std::cout<<c[2].as<int>()<<",";
				std::cout<<c[3].as<std::string>();
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
