#ifndef TABLE_H
#define TABLE_H
#include"HTML/HTML.h"
#include<iostream>
#include<json/json.h>
namespace srv::view{
	class Table{
		public:
			Table();
			~Table();
			void setHeader(std::vector<std::string>);
			void addRow(std::vector<std::string>);
			std::vector<std::vector<std::string>> getRows() const;
			HTML::Element toHtml() const;
			Json::Value toJson() const;
		protected:
		private:
			std::vector<std::string> mVHeader;
			std::vector<std::vector<std::string>> mVRows;
	};
}
#endif
