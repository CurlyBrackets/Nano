#include "Line.h"

Line::Line(String initialData):
data(initialData),_p(NULL),_n(NULL),cPos(0),currentChar(data.begin()){
    //ctor
}

Line::~Line()
{
    //dtor
}

void Line::prev(Line* value){
	_p = value;
}

Line* Line::prev() const{
	return _p;
}

void Line::next(Line* value){
	_n = value;
}

Line* Line::next() const{
	return _n;
}

void Line::insertBefore(Line* line){
    if(prev()){
        prev()->next(line);
        line->prev(prev());
    }
    prev(line);
    line->next(this);
}

void Line::insertAfter(Line* line){
    if(next()){
        next()->prev(line);
        line->next(next());
    }
    next(line);
    line->prev(this);
}

void Line::insert(int ch){
    if(ch == CWin::key_enter()){
        /*insertAfter(new Line(data.substr(cPos)));
        data = data.substr(0, cPos-1);*/
        //move to next line
    }
    else{
        data.insert(new Character(ch), currentChar);
        (*currentChar)->prev()->set_pos((*currentChar)->position());//update positions?
    }
}

bool Line::del(){
	if(currentChar != data.end()){
		String::iterator it = currentChar;
		++currentChar;
		delete data.pop(it);
		return false;
	}
	else
		return true;
}

void Line::incrementPos(){
    if(!currentChar.end())
    	++currentChar;
}

void Line::decrementPos(){
	if(currentChar.end())
		currentChar = data.iter_at(data.length()-1);
	else if((*currentChar)->prev())
		--currentChar;
}

void Line::set_pos(int newPos){
	if(newPos > data.length() || newPos < 0)
		currentChar = data.end();
	else
		currentChar = data.iter_at(newPos);
}

void Line::set_pos(String::iterator it){
	currentChar = it;
}

String Line::string(){
    return data;
}

unsigned int Line::position() const{
	return cPos;
}

void Line::append(String data){
	this->data += data;
}

void Line::number(const unsigned int& value){
	_number = value;
}

unsigned int Line::number() const{
	return _number;
}

void Line::set_num(unsigned int num){
	number(num);
	if(next())
	 	next()->set_num(num+1);
}

unsigned int Line::cursor_position(){
	if(!data.length())
		return 0;
	else if(currentChar.end())
		return data[data.length()-1]->position()+1;
	else
		return (*currentChar)->position();
}
