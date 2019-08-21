#ifndef PAGE_H
#define PAGE_H
#include"HTML/HTML.h"
#include<iostream>
namespace srv::view{
	class Page{
		public:
			Page();
			~Page();
			void setName(std::string);
			std::string getName();
			HTML::Document& getDocument();
    			friend std::ostream& operator<<(std::ostream& s,Page& p);
		protected:
		private:
			std::string mName;
			HTML::Document mDocument;
	};
}
#endif
