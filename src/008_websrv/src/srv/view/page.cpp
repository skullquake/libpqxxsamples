#include"srv/view/page.h"
srv::view::Page::Page()
             :mName(std::string())
{
	mDocument.addAttribute("lang","en");
	mDocument.head()
		<<HTML::Meta()
		<<HTML::Meta("viewport","width=device-width,initial-scale=1,shrink-to-fit=no")
		<<HTML::Rel("icon","favicon.ico").addAttribute("type","image/png")
		//<<HTML::Rel("stylesheet","/css/bootstrap.min.css")
		//<<HTML::Rel("stylesheet","/css/font-awesome.css")
		//<<HTML::Rel("stylesheet","/css/themes/blue.css")
		<<HTML::Rel("stylesheet","http://ajax.googleapis.com/ajax/libs/dojo/1.10.4/dojo/resources/dojo.css")
		<<HTML::Rel("stylesheet","http://ajax.googleapis.com/ajax/libs/dojo/1.10.4/dijit/themes/tundra/tundra.css")
		<<HTML::Rel("stylesheet","https://stackpath.bootstrapcdn.com/bootstrap/4.3.1/css/bootstrap.min.css")
			.integrity("sha384-ggOyR0iXCbMQv3Xipma34MD+dH/1fQ784/j6cY/iJTQUOhcWr7x9JvoRxT2MZw1T")
			.crossorigin("anonymous")
		<<HTML::Script("//ajax.googleapis.com/ajax/libs/dojo/1.10.4/dojo/dojo.js");
	;
}
srv::view::Page::~Page(){
}
std::string srv::view::Page::getName(){
	return mName;
}
void srv::view::Page::setName(std::string v){
	mName=v;
	mDocument.title(mName);
}
HTML::Document& srv::view::Page::getDocument(){
	return mDocument;
}
std::ostream& srv::view::operator<<(std::ostream& aStream,view::Page& aPage){
	aStream<<aPage.getDocument();
	return aStream;
}
