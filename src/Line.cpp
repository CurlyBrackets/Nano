#include "Line.h"

Line::Line(std::string initialData):
data(initialData),_p(NULL),_n(NULL),cPos(0){
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
    if(ch == 13){
        insertAfter(new Line(data.substr(cPos)));
        data = data.substr(0, cPos-1);
    }
    else{
        data.insert(cPos,1, ch);
        incrementPos();
    }
}
int Line::incrementPos(){
    int ret = 1;
    if(cPos < data.length()){
        if(data[cPos] == 9)
            ret = 4;
        cPos++;
    }
    else
        ret = 0;
    return ret;
}

int Line::decrementPos(){
    int ret = -1;
    if(cPos > 0){
        if(data[cPos-1] == 9)
            ret = -4;
        cPos--;
    }
    else
        ret = 0;
    return ret;
}

std::string Line::string() const{
    return data;
}
