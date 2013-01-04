#ifndef STRING_H
#define STRING_H
#include <List.h>
#include "Highlighter.h"

class Character:public Node{
	public:
		Character(int ch);
		Character(const Character& b);

		void set_pos(unsigned int num);
		void set_color(int colour=-1);
		int ch() const;
		int colour() const;
		int position() const;
	private:
		int _ch, _col, _pos;
		//static FSM state; //make this or something
};

class String:public List<Character>{};

#endif // STRING_H
