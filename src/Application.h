#ifndef APPLICATION_H
#define APPLICATION_H
#include <CWin.hpp>
#include <fstream>
#include <string>
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
        int xShift, topLineNum;

        Line* top, *current, *first;

        Application();
        void render();
        void renderNumbers();

        void renderControl();
};

#endif // APPLICATION_H
