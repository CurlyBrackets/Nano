#include "Application.h"

Application::Application():
numbers(NULL),display(NULL),control(NULL){

}

Application::~Application(){
    delete numbers;
    delete display;
    delete control;
    Line* temp = first, *temp2;
    while(temp){
        temp2 = temp->next();
        delete temp;
        temp = temp2;
    }
	CWin::end();
}

int Application::run(std::string filename){
    Application theApp;
    return theApp.execute(filename);
}

void Application::init(){
    CWin::start();
    numbers = new CWin(0,0,5,CWin::window_y()-2);
    display = new CWin(5,0,CWin::window_x()-5,CWin::window_y()-2);
    control = new CWin(0,CWin::window_y()-2,CWin::window_x(),2);
    refresh();

    int wb = numbers->createColor(COLOUR_WHITE, COLOUR_BLACK),
        wg = numbers->createColor(COLOUR_GREEN, COLOUR_BLACK);

    numbers->setCol(wg);
    numbers->clear();
    numbers->refresh();

    display->setCol(wb);
    display->clear();
    display->refresh();

    control->setCol(wg);
    control->clear();
    control->refresh();

    running = true;
    xShift = 0;
    topLineNum = 1;
}

int Application::execute(std::string filename){
    init();

    std::ifstream infile(filename.c_str());
    if(infile.is_open()){
        Line* last = NULL;
        std::string temp;
        while(!infile.eof()){
            std::getline(infile, temp);
            if(!last){
                last = new Line(temp);
                current = last;
                top = last;
                first = top;
            }
            else{
                last->next(new Line(temp));
                last->next()->prev(last);
                last = last->next();
            }
        }
    }
    else{
        top = new Line();
        current = top;
        first = current;
    }
    infile.close();
    render();
    renderNumbers();
    renderControl();
    move(0,5);

    while(running){
        int in = getch();
        if(in > 0x1F && in < 0x7F){//typable
		current->insert(in);
        }
        else if(in == 24){
            if(changed && promptBool("Save before exit? [y/n/c]"))
                saveAs();
            running = false;
        }
        else if(in == 259){//up

        }
        else if(in == 258){//dn

        }
        else if(in == 260){//left

        }
        else if(in == 261){//right

        }
        else if(in == 443){//ctrl-left page left

        }
        else if(in == 444){//ctrl-right page right

        }
        else if(in == 265){//F1 help

        }
        else if(in == 6){//^F find

        }
        else if(in == 15){//^O Mode

        }
        else if(in == 19){//^S Save
            save();
        }
        else if(in == 18){//^R Reload

        }
    }
    return 0;
}

void Application::render(){
    display->clear();
    Line* temp = top;

    for(unsigned int i=0;i<23 && temp;i++,temp = temp->next()){
        if(xShift < temp->string().length()){
            display->mv(0,i);
            display->print(temp->string().substr(xShift));
        }
    }
    display->refresh();
}

void Application::renderNumbers(){
    numbers->clear();
    Line* temp = top;
    for(unsigned int i = topLineNum;i<topLineNum + 23 && temp;i++,temp = temp->next()){
        numbers->mv(0,i-topLineNum);
        std::string temp = std::to_string(i);
        for(unsigned int i=0;i<4-temp.length();i++)
            numbers->print(' ');
        numbers->print(temp);
    }
    numbers->refresh();
}

void Application::renderControl(){
    control->mv(0,0);
    control->print("^X Exit    ^");
    control->print(ACS_LARROW);
    control->print(" Page Left  ^F Find    ^O Mode");
    control->mv(0,1);
    control->print("F1 Help    ^");
    control->print(ACS_RARROW);
    control->print(" Page Right ^S Save    ^R Reload");

    control->refresh();
}

int Application::promptBool(std::string str){

    renderControl();
    return 1;
}

std::string Application::promptString(std::string prmpt, std::string def){
    return def;
}

void Application::saveAs(){
    std::string fn = promptString("Filename to write: ", filename);
    if(fn.compare(filename))
        filename = fn;
    save();
}

void Application::save(){
    std::ofstream ofile(filename.c_str());
    for(Line* temp = first;temp;temp = temp->next())
        ofile << temp->string() << std::endl;

    ofile.close();
}
