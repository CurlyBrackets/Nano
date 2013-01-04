#include "String.h"

Character(int ch):
Node(),_ch(ch),pos(-1),_col(COLOUR_WHITE){

}

Character(const Character& b):
_ch(b.ch()),_col(b.colour()),_pos(b.position()){
}

void Character::set_pos(unsigned int num){
	_pos = num;
	if(next())
		next()->set_pos((ch() == '\t')?num+(4-num%5):num+1);
}

void Character::set_color(int colour){
	if(colour == -1){
		if(prev() && prev()->ch() != 0x20 && prev()->ch() != '\t')
			prev()->set_colour(-1);
		else{
			SDtGNS DFYH SFXHG FTBYGNHJDG
		}
	}
	else{
		if(next() && next->ch() != 0x20 && prev->ch() != 9 && )
			next()->
	}
}
