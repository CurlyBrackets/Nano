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
        int incrementPos();
        int decrementPos();

        std::string string() const;
    private:
        std::string data;
        Line* _p, *_n;
        unsigned int cPos;
};

#endif // LINE_H
