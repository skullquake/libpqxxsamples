#include<stdio.h>
#include<time.h>
#include<string>
#include<stdlib.h>
#include<iostream>
#include<sstream>
#include<fstream>
#include<json/json.h>


#include <iostream>
#include <pqxx/pqxx>

#include <algorithm>
#include <iostream>
#include <random>
#include <vector>

using namespace std;
using namespace pqxx;
int main(int argc, char* argv[]){
	char * sql;
	std::cout<<"Opening database...";
	try{
		connection C(
			"dbname = test hostaddr = 127.0.0.1 port = 5432"
		);
		if(C.is_open()){
			std::cout<<"done"<<std::endl;
		}else{
			std::cout<<"Failed to open database"<<std::endl;
			return 1;
		}

		work W(C);

		W.exec(
			"create table if not exists orders(id serial not null primary key,info json not null);"
		);
		/*
		W.exec(
			"delete from orders;"
		);
		*/
		std::vector<std::string> vcus;
		std::stringstream ss;
		for(int i=0;i<32;i++){
			ss.str(std::string());
			ss<<"Customer_"<<i;
			vcus.push_back(ss.str());
		}
		std::vector<std::string> vprod;
		for(int i=0;i<32;i++){
			ss.str(std::string());
			ss<<"Product_"<<i;
			vprod.push_back(ss.str());
		}
		srand(time(0));
		for(long i=0;i<4096*8;i++){
			Json::Value body;
			body["customer"]=vcus[rand()%vcus.size()];
			body["items"]=Json::objectValue;
			body["items"]["product"]=vprod[rand()%vprod.size()];
			body["items"]["qty"]=rand()%10;
			Json::FastWriter fastWriter;
			std::string str_body=fastWriter.write(body);
			W.exec(
				"insert into orders (info)values('"+str_body+"')"
			);
		}
		W.commit();

		/*
		nontransaction N(C);
		result R;
		//select into json example
		R=( N.exec("\
			select \
			info ->> 'customer' as customer,\
			info -> 'items' ->> 'product' as product,\
			info -> 'items' ->> 'qty' as qty,\
			info -> 'items' as itemsjson,\
			info as infojson\
			from orders;\
			"));
		for (result::const_iterator c = R.begin(); c != R.end(); ++c) {
			std::cout<<"["<<c[0].as<std::string>();
			std::cout<<","<<c[1].as<std::string>();
			std::cout<<","<<c[2].as<std::string>();
			std::cout<<"]"<<std::endl;
			std::cout<<"itemsjson: "<<c[3].as<std::string>()<<std::endl;;
			std::cout<<"infojson: "<<c[4].as<std::string>()<<std::endl;;
		}
		//aggregate example
		R=( N.exec("\
			select\
			avg(\
				cast(\
					info->'items'->>'qty' as integer\
				)\
			)\
			from orders\
			"));
		for (result::const_iterator c = R.begin(); c != R.end(); ++c) {
			std::cout<<"avg(items.qty): : "<<c[0].as<float>()<<std::endl;;
		}
		//json_each() example

		R=( N.exec("\
				select\
				json_each (info)\
				from\
				orders;\
			"));
		for (result::const_iterator c = R.begin(); c != R.end(); ++c) {
			std::cout<<"for_each(): "<<c[0].as<string>()<<std::endl;;
		}

		//json_object_keys () example
		R=( N.exec("\
				select\
				json_object_keys (info->'items')\
				from\
				orders;\
			"));
		for (result::const_iterator c = R.begin(); c != R.end(); ++c) {
			std::cout<<"json_object_keys(): "<<c[0].as<string>()<<std::endl;;
		}

		//json_typeof() example
		R=( N.exec("\
				select\
				json_typeof (info->'items'->'product') as type_product,\
				json_typeof (info->'items'->'qty') as type_qty\
				from\
				orders;\
			"));
		for (result::const_iterator c = R.begin(); c != R.end(); ++c) {
			std::cout<<"json_typeof(product): "<<c[0].as<string>()<<std::endl;;
			std::cout<<"json_typeof(qty): "<<c[1].as<string>()<<std::endl;;
		}




		R=( N.exec("select pg_database_size('test');"));
		for (result::const_iterator c = R.begin(); c != R.end(); ++c) {
			std::cout<<"Database size: "<<c[0].as<long>()/1024.0<<" kB"<<std::endl;
		}
		*/
		C.disconnect();
		std::cout<<"done"<<std::endl;
	}catch(const std::exception &e){
		cerr<<e.what()<<std::endl;
		return 1;
	}
	return 0;
}



