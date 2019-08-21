#ifndef SRV_CTL_CTL_H
#define SRV_CTL_CTL_H
#include<mongoosecpp/Server.h>
#include<mongoosecpp/WebController.h>
#include<iostream>
#include"util/pg.h"
namespace srv::ctl{
	class MyController:public Mongoose::WebController{
		public: 
			MyController();
			~MyController();
			void hello(Mongoose::Request &request, Mongoose::StreamResponse &);
			void xas(Mongoose::Request &request, Mongoose::StreamResponse &);
			void setup();
		private:
			util::Pg p;
	};
}
#endif

