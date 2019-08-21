#include"srv/view/table.h"
srv::view::Table::Table(){
}
srv::view::Table::~Table(){
}
std::vector<std::vector<std::string>> srv::view::Table::getRows() const{
	return mVRows;
}
void srv::view::Table::addRow(std::vector<std::string> aVRow){
	mVRows.push_back(aVRow);
}
void srv::view::Table::setHeader(std::vector<std::string> aVHeader){
	mVHeader=aVHeader;
}
HTML::Element srv::view::Table::toHtml() const{
	HTML::Element table("table");
	table.cls("table table-striped table-sm");
	table.id("id");
	HTML::Element tbody("tbody");
	HTML::Element thead("thead");
	HTML::Element theadtr("tr");
	for(
		std::vector<std::string>::const_iterator ithdr=mVHeader.begin();
		ithdr!=mVHeader.end();
		ithdr++
	){
		HTML::Element td("th");
		td<<(*ithdr);
		theadtr<<std::move(td);
	}
	thead<<std::move(theadtr);
	for(
		std::vector<std::vector<std::string>>::const_iterator itrow=mVRows.begin();
		itrow!=mVRows.end();
		itrow++
	){
		HTML::Element tr("tr");
		for(
			std::vector<std::string>::const_iterator itcol=(*itrow).begin();
			itcol!=(*itrow).end();
			itcol++
		){
			HTML::Element td("td");
			td<<(*itcol);
			tr<<std::move(td);
		}
		tbody<<std::move(tr);
	}
	table<<std::move(thead);
	table<<std::move(tbody);
	return table;
}
Json::Value srv::view::Table::toJson() const{
	Json::Value r;
	/*
	for(
		std::vector<std::vector<std::string>>::const_iterator itrow=mVRows.begin();
		itrow!=mVRows.end();
		itrow++
	){
		for(
			std::vector<std::string>::const_iterator itcol=(*itrow).begin();
			itcol!=(*itrow).end();
			itcol++
		){
			(*itcol);
		}
		tbody<<std::move(tr);
	}
	*/
	return r;
}
