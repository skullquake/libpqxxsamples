#ifndef MENU_H
#define MENU_H
#include"HTML/HTML.h"
#include<iostream>
#include<json/json.h>
namespace srv::view{
	class Menu{
		public:
			Menu();
			~Menu();
			void addItem(std::pair<std::string,std::string>);
			std::vector<std::pair<std::string,std::string>> getItems() const;
			HTML::Div toHtml() const;
			Json::Value toJson() const;
		protected:
		private:
			std::vector<std::pair<std::string,std::string>> mVItems;
			std::string mSSOC;
	};
}
#endif
