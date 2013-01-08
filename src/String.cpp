#include "String.h"

std::ostream& operator<<(std::ostream &out, const String& str){
	for(auto it=str.cbegin();it != str.cend();++it)
		out << (char)((*it)->ch());
	return out;
}

Character::Character(int ch):
_ch(ch),_col(COLOUR_WHITE),_pos(-1),_n(NULL),_p(NULL){

}

Character::Character(const Character& b):
_ch(b.ch()),_col(b.colour()),_pos(b.position()),_n(NULL),_p(NULL){
}

int Character::ch() const{
	return _ch;
}
int Character::colour() const{
	return _col;
}
int Character::position() const{
	return _pos;
}

void Character::set_pos(unsigned int num){
	_pos = num;
	if(next())
		next()->set_pos((ch() == '\t')?num+(4-num%5):num+1);
}

void Character::set_color(int colour){
	/*if(colour == -1){
		if(prev() && prev()->ch() != 0x20 && prev()->ch() != '\t')
			prev()->set_colour(-1);
		else{
			SDtGNS DFYH SFXHG FTBYGNHJDG
		}
	}
	else{
		if(next() && next->ch() != 0x20 && prev->ch() != 9 && )
			next()->
	}*/
	_col = COLOUR_WHITE;
}

void Character::next(Character* node){
	_n = node;
}
void Character::prev(Character* node){
	_p = node;
}

Character* Character::next() const{
	return _n;
}
Character* Character::prev() const{
	return _p;
}

String::String(std::string str){
	for(unsigned int i=0;i<str.length();i++)
		push_back(new Character(str[i]));
}

String String::substr(unsigned int start, unsigned int len){
	String ret;
	if(len == (unsigned int)-1){
		for(auto it = iter_at(start);it != end();++it)
			ret.push_back(new Character(**it));
	}
	else{
		for(auto it = iter_at(start);start < len && it != end();start++)
			ret.push_back(new Character(**it));
	}
	return ret;
}
