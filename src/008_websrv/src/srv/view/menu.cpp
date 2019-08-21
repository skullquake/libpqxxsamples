#include"srv/view/menu.h"
srv::view::Menu::Menu(){
}
srv::view::Menu::~Menu(){
}
std::vector<std::pair<std::string,std::string>> srv::view::Menu::getItems() const{
	return mVItems;
}
void srv::view::Menu::addItem(std::pair<std::string,std::string> aItem){
	mVItems.push_back(aItem);
}
HTML::Div srv::view::Menu::toHtml() const{
	HTML::Element ul("ul");
	ul.cls("navbar-nav");
	for(
		std::vector<std::pair<std::string,std::string>>::const_iterator it=mVItems.begin();
		it!=mVItems.end();
		it++
	){
		ul<<HTML::Element("li")
			<<(
				HTML::Element("a")
					.cls("nav-link")
					.addAttribute("href",(*it).second)
						<<(*it).first
			)
		;
	}
	HTML::Div r;
	r.cls("navbar navbar-expand-sm bg-light");
	r.id("Menu");
	r<<std::move(ul);
	return r;
}
Json::Value srv::view::Menu::toJson() const{
	Json::Value r;
	/*
	for(
		std::vector<std::string>::const_iterator it=mVItems.begin();
		it!=mVItems.end();
		it++
	){

		r[*it]="#";
	}
	*/
	return r;
}
