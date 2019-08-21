#include<iostream>
#include<pqxx/pqxx>
using namespace std;
using namespace pqxx;
int main(int argc, char* argv[]){
	std::stringstream buffer;
	try{
		connection C("dbname=test hostaddr=127.0.0.1 port=5432");
	if(C.is_open()){
		std::cout<<"Opened database successfully: "<<C.dbname()<<std::endl;
	}else{
		std::cerr<<"Can't open database"<<std::endl;
		return 1;
	}
	buffer.str("");
	buffer	<<"CREATE TABLE IF NOT EXISTS COMPANY("<<std::endl
		<<"ID INT PRIMARY KEY     NOT NULL,"<<std::endl
		<<"NAME           TEXT    NOT NULL,"<<std::endl
		<<"AGE            INT     NOT NULL,"<<std::endl
		<<"ADDRESS        CHAR(50),"<<std::endl
		<<"SALARY         REAL );";
	work W(C);
	W.exec(buffer.str().c_str());
	W.commit();
	std::cout<<"Table created successfully"<<std::endl;
	buffer.str("");
	buffer	<<"DELETE FROM COMPANY;";
	work W1(C);
	W1.exec(buffer.str().c_str());
	W1.commit();
	std::cout<<"Deleted rows"<<std::endl;
	std::vector<std::string> v;
	v.push_back("foo");
	v.push_back("bar");
	v.push_back("baz");
	v.push_back("qux");
	for(int i=1;i<4096;i++){
		buffer.str("");
		buffer	<<"INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY) "
			<<"VALUES ("<<i<<", '"<<v[rand()%v.size()]<<"', "<<rand()%50<<", '"<<v[rand()%v.size()]<<"', "<<rand()%5000<<");";
		work W3(C);
		W3.exec(buffer.str().c_str());
		W3.commit();
	}
	std::cout<<"Records created successfully"<<std::endl;
	buffer.str("");
	buffer<<"SELECT * from COMPANY";
	nontransaction N(C);
	result R(N.exec(buffer.str().c_str()));
	for(result::const_iterator c=R.begin();c!=R.end();++c){
		std::cout<<"ID      = "<<c[0].as<int>()<<std::endl;
		std::cout<<"Name    = "<<c[1].as<string>()<<std::endl;
		std::cout<<"Age     = "<<c[2].as<int>()<<std::endl;
		std::cout<<"Address = "<<c[3].as<string>()<<std::endl;
		std::cout<<"Salary  = "<<c[4].as<float>()<<std::endl;
	}
	std::cout<<"Operation done successfully"<<std::endl;
	C.disconnect();
	}catch(const std::exception &e){
		std::cerr<<e.what()<<std::endl;
		return 1;
	}
	return 0;
}
