#include <iostream>
#include <pqxx/pqxx>
using namespace std;
using namespace pqxx;
int main(int argc, char* argv[]){
	char * sql;
	try{
		connection C(
			"dbname = testdb hostaddr = 127.0.0.1 port = 5432"
	);
	if(C.is_open()){
		cout<<"Opened database successfully: "<<C.dbname()<<endl;
	}else{
		cout<<"Can't open database"<<endl;
		return 1;
	}
	/* Create SQL statement */
	sql=	"CREATE TABLE IF NOT EXISTS COMPANY("\
		"ID INT PRIMARY KEY     NOT NULL,"\
		"NAME           TEXT    NOT NULL,"\
		"AGE            INT     NOT NULL,"\
		"ADDRESS        CHAR(50),"\
		"SALARY         REAL );";
	/* Create a transactional object. */
	work W(C);
	/* Execute SQL query */
	W.exec(sql);
	W.commit();
	cout<<"Table created successfully"<<endl;

	/* Delete from SQL statement */
	sql=	"DELETE FROM COMPANY;";
	/* Create a transactional object. */
	work W1(C);
	/* Execute SQL query */
	W1.exec(sql);
	W1.commit();
	cout<<"Deleted rows"<<endl;


	/* Create SQL statement */
	sql=	"INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY) "\
		"VALUES (1, 'Paul', 32, 'California', 20000.00 ); "\
		"INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY) "\
		"VALUES (2, 'Allen', 25, 'Texas', 15000.00 ); "\
		"INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY)"\
		"VALUES (3, 'Teddy', 23, 'Norway', 20000.00 );"\
		"INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY)"\
		"VALUES (4, 'Mark', 25, 'Rich-Mond ', 65000.00 );"\
		"INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY)"\
		"VALUES (5, 'Ockert', 32, 'Johnannesburg', 5000.00 );"
	;
	/* Create a transactional object. */
	work W3(C);
	/* Execute SQL query */
	W3.exec(sql);
	W3.commit();
	cout<<"Records created successfully"<<endl;

	C.disconnect();
	}catch(const std::exception &e){
		cerr<<e.what()<<std::endl;
		return 1;
	}
   return 0;
}


