#ifndef UTIL_PG_H
#define UTIL_PH_H
#include<iostream>
#include<pqxx/pqxx>
#include<tuple>
#include<random>
#include<iostream>
#include<pqxx/pqxx>
#include<tuple>
#include<random>
namespace util{
	class Pg{
		public:
			Pg(std::string,std::string);
			~Pg();
			int connect(std::string);
			int disconnect();
			pqxx::result execnt(std::string);
			int exect(std::string);
			static void printr(pqxx::result&);
		private:
			std::string mHost;
			std::string mPort;
			pqxx::connection* mCon;
		protected:
	};
}
#endif
