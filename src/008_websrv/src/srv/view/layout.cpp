#include"srv/view/layout.h"
srv::view::Layout::Layout()
             :mName(std::string())
{
	mMenu.id("layout-menu");
	mBody.id("layout-body");
}
srv::view::Layout::~Layout(){
}
HTML::Div & srv::view::Layout::getMenu(){
	return mMenu;
}
HTML::Div & srv::view::Layout::getBody(){
	return mBody;
}
std::ostream& srv::view::operator<<(std::ostream& aStream,view::Layout& aLayout){
	aStream<<aLayout.getMenu()<<aLayout.getBody();
	return aStream;
}
