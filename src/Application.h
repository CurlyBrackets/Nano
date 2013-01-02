#ifndef APPLICATION_H
#define APPLICATION_H
#include <CWin.hpp>
#include <fstream>
#include <string>
#include <initializer_list>
#include "Line.h"

class Application
{
    public:
        ~Application();

        static int run(std::string filename);
        void init();
        int execute(std::string filename);

        int promptBool(std::string str);
        std::string promptString(std::string prmpt, std::string def = "");
        void save();
        void saveAs();
    private:
        CWin* numbers, *display, *control;
        std::string filename;
        bool running, changed;
        unsigned int xShift, topLineNum;
        std::ofstream log;
        int tempPos;

        Line* top, *current, *first;

        Application();
        void render();//re-render entire screen
        void renderLine();//render current line as per yPos in display
        void renderNumbers();
        void renderControl();

        void updateMove(int my);
        void updateMove(int mx, int my);
        void updateMove();

        bool string_check(std::string str, std::initializer_list<std::string>);
};

#endif // APPLICATION_H
