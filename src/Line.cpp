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

int Line::incrementPos(unsigned int curPos){
    int ret = 1;
    if(!currentChar.end()){
    	if((*currentChar)->ch() == '\t')
    		ret = 4-curPos%5;
    	++currentChar;
    }
    else
    	ret = 0;
    return ret;
}

int Line::decrementPos(){
    int ret = -1;
    /*if(cPos > 0){
        if(data[cPos-1]->ch() == 9)
            ret = -4;
        cPos--;
    }
    else
        ret = 0;*/
	if((*currentChar)->prev()){
		--currentChar;
	}
	else
		ret = 0;
    return ret;
}

void Line::set_pos(int newPos){
	/*if(newPos > data.length())
		cPos = data.length();
	else if(newPos > -1)
		cPos = newPos;*/
	currentChar = data.iter_at(newPos);
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
	else if(currentChar == data.end())
		return data[data.length()]->position();
	else
		return (*currentChar)->position();
}
