#ifndef LINE_H
#define LINE_H
#include <string>

class Line
{
    public:
        Line(std::string initialData="");
        ~Line();

        void prev(Line* value);
        Line* prev() const;
        void next(Line* value);
        Line* next() const;

        void insertBefore(Line* line);
        void insertAfter(Line* line);
        void insert(int ch);
        bool del();
        int incrementPos();
        int decrementPos();
        void set_pos(int newPos);
        unsigned int position() const;

        std::string string() const;
        void append(std::string data);

        void number(const unsigned int& value);
        unsigned int number() const;
    private:
        std::string data;
        Line* _p, *_n;
        unsigned int cPos, _number;
};

#endif // LINE_H
