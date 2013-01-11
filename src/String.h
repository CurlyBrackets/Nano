#ifndef STRING_H
#define STRING_H
#include <List.h>
#include <ostream>
#include <string>
#include "Highlighter.h"

class Character{
	public:
		Character(int ch);
		Character(const Character& b);

		void set_pos(unsigned int num);
		void set_pos();
		void set_color(int colour=-1);
		int ch() const;
		int colour() const;
		int position() const;

		void next(Character* node);
		void prev(Character* node);
		Character* next() const;
		Character* prev() const;
	private:
		int _ch, _col, _pos;
		Character* _n, *_p;
		//static FSM state; //make this or something
};

class String:public List<Character>{
	public:
		String(std::string str="");
		String substr(unsigned int start, unsigned int len=-1);
};

extern std::ostream& operator<<(std::ostream &out, const String& str);

#endif // STRING_H
