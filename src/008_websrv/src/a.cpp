#include<windows.h>
#include<unistd.h>
#include<signal.h>
#include<mongoosecpp/Server.h>
#include<mongoosecpp/WebController.h>
#include<iostream>
#include<json/json.h>
#include"srv/ctl/ctl.h"
int main(int argc,char** argv){
	srv::ctl::MyController myController;
	Mongoose::Server server(8080);
	server.registerController(&myController);
	server.setOption("enable_directory_listing","yes");
	server.setOption("document_root","./pub");
	server.start(); 
	while(1){
		Sleep(10000);
	}
}
