#include"util/pg.h"
namespace util{
	Pg::Pg(std::string aHost="127.0.0.1",std::string aPort="5000")
	:mHost(aHost),
	 mPort(aPort)
	{
		std::cout<<"Pg()"<<std::endl;
	};
	Pg::~Pg(){
		std::cout<<"~Pg()"<<std::endl;
		this->disconnect();
	};
	int Pg::connect(std::string aDbName=""){
		std::cout<<"int connect(std::string aDbName="")"<<std::endl;
		int ret;
		if(aDbName.length()>0){
			if(this->mCon==NULL){
				std::cout<<"Discarding old connection..."<<std::endl;
				this->disconnect();
			}else{
			}

			try{
				this->mCon=new pqxx::connection(
					"dbname = "+aDbName+" hostaddr = "+this->mHost+" port = "+this->mPort
				);
				if(this->mCon->is_open()){
					std::cout<<"Opened database successfully: "<<this->mCon->dbname()<<std::endl;
					ret=0;
				}else{
					std::cout<<"Can't open database"<<std::endl;
					ret=-1;
				}
			}catch(const std::exception& e){
				std::cerr<<e.what()<<std::endl;
				ret=-1;
			}

		}else{
			std::cerr<<"aDbName \"\":Aborting..."<<std::endl;
			ret=-1;
		}
		return ret;
	};
	int Pg::disconnect(){
		std::cout<<"int disconnect(){";
		int ret;
		try{
			if(this->mCon!=NULL){
				if(this->mCon->is_open()){
					this->mCon->disconnect();
					ret=0;
				}else{
					ret=-1;
				}
			}else{
				std::cerr<<"mCon NULL:Aborting..."<<std::endl;
				ret=-1;
			}

		}catch(const std::exception& e){
			std::cerr<<e.what()<<std::endl;
			ret=-1;
		}
		return ret;
	};
	pqxx::result Pg::execnt(std::string s){
		std::cout<<"int execnt(std::string s)"<<std::endl;
		pqxx::result r;
		if(s.length()>0){
			if(this->mCon!=NULL){
				if(this->mCon->is_open()){
					try{
						pqxx::nontransaction nt(*(this->mCon));
						r=nt.exec(s.c_str());
					}catch(const std::exception& e){
						std::cerr<<e.what()<<std::endl;
					}
				}else{
					std::cerr<<"mCon not open"<<std::endl;
				}
			}else{
				std::cerr<<"mCon NULL:Aborting..."<<std::endl;
			}
		}else{
			std::cerr<<"Query empty:Aborting..."<<std::endl;
		}
		return r;
	};
	int Pg::exect(std::string s){
		int ret;
		if(s.length()>0){
			if(this->mCon!=NULL){
				if(this->mCon->is_open()){
					{
						pqxx::work w(*(this->mCon));
						try{
							w.exec(
								s
							);
							w.commit();
						}catch(const std::exception& e){
							std::cerr<<e.what()<<std::endl;
							w.abort();
							throw;
						}
					}
				}else{
					std::cerr<<"mCon not open"<<std::endl;
					ret=-1;
				}
			}else{
				std::cerr<<"mCon NULL:Aborting..."<<std::endl;
				ret=-1;
			}
		}else{
			std::cerr<<"Query empty:Aborting..."<<std::endl;
			ret=-1;
		}
		return ret;
	};
	void Pg::printr(pqxx::result &r){
		if(r.size()>0){
			std::cout<<"Executed"<<std::endl;
			for(
				pqxx::result::const_iterator row=r.begin();
				row!=r.end();
				++row
			){
				for(
					pqxx::row::const_iterator field=row->begin();
					field!=row->end();
					++field
				)
					std::cout<<field->c_str()<<'\t';
				std::cout<<std::endl;
			}
		}else{
			std::cout<<"No results"<<std::endl;
		}
	};
}
