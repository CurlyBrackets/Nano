#ifndef LINE_H
#define LINE_H
#include <string>
#include "String.h"

class Line
{
    public:
        //Line(std::string initialData="");
        Line(String initialData=String(""));
        ~Line();

        void prev(Line* value);
        Line* prev() const;
        void next(Line* value);
        Line* next() const;

        void insertBefore(Line* line);
        void insertAfter(Line* line);
        void insert(int ch);
        bool del();
        int incrementPos(unsigned int curPos);
        int decrementPos();
        void set_pos(int newPos);
        unsigned int position() const;
        unsigned int cursor_position();

        //std::string string() const;
        //void append(std::string data);
        String string();
       	void append(String data);

        void number(const unsigned int& value);
        unsigned int number() const;
        void set_num(unsigned int num);
    private:
        //std::string data;
        String data;
        Line* _p, *_n;
        unsigned int cPos, _number;
        String::iterator currentChar;
};

#endif // LINE_H
