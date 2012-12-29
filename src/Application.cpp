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
    this->filename = filename;

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
    display->mv(0,0);
    current->set_pos(0);

    while(running){
        int in = getch();
        if(in > 0x1F && in < 0x7F){//typable
			current->insert(in);
			changed = true;
			//render();//should be a single line refresh in the end
			renderLine();
			display->mv(display->xPos()+current->incrementPos(), display->yPos());
			move(display->yPos(), 5+display->xPos());
        }
        else if(in == CWin::key_backspace()){
			int shift = current->decrementPos();
			if(shift){
				changed = true;
				current->del();
				renderLine();
				display->mv(display->xPos()+shift, display->yPos());
				move(display->yPos(), 5+display->xPos());
			}
        }
        else if(in == 330){
			current->del();
			renderLine();
			changed = true;
        }
        else if(in == 24){//^X
            if(changed){
            	int temp = promptBool("Save before exit? [y/n/c] ");
            	if(temp == 1)
					saveAs();
                if(temp > -1)
                	running = false;
        	}
        	else
				running = false;
        }
        else if(in == 258){//dn todo check scrolling things
			if(current->next()){
				current = current->next();
				current->set_pos(current->prev()->position());
				if(current->position() < xShift){
					if(current->position()-display->xMax() < 0)
						xShift = 0;
					else
						xShift = current->position()-display->xMax();
					render();
				}
				display->mv(current->position()-xShift, display->yPos()+1);
				move(display->yPos(), 5+display->xPos());
				if(display->yPos() == display->yMax())
					top = top->next();
			}
        }
        else if(in == 259){//up
			if(current->prev()){
				current = current->prev();
				current->set_pos(current->next()->position());
				display->mv(current->position()-xShift, display->yPos()-1);
				move(display->yPos(), 5+display->xPos());
				if(display->yPos() == 0 && top->prev())
					top = top->prev();
			}
        }
        else if(in == 260){//left
        	int shift = current->decrementPos();
			if(display->xPos()+shift > 0 && xShift){
				xShift += shift;
				render();
			}
			else if(display->xPos()){
				display->mv(display->xPos()+shift, display->yPos());
				move(display->yPos(), 5+display->xPos());
			}
        }
        else if(in == 261){//right
			int shift = current->incrementPos();
			if(display->xPos()+shift > display->xMax()){
				xShift += shift;
				render();
			}
			else if(shift){
				display->mv(display->xPos()+shift, display->yPos());
				move(display->yPos(), 5+display->xPos());
			}
        }
        else if(in == CWin::key_end()){
        	current->set_pos(current->string().length());
        	if(current->position() < display->xMax()){
        		display->mv(current->position(), display->yPos());
				move(display->yPos(), 5+display->xPos());
			}
        	else{
        		xShift = current->position()-display->xMax();
        		display->mv(current->position()-xShift, display->yPos());
				move(display->yPos(), 5+display->xPos());
				render();
        	}
        }
        else if(in == 262){//home
        	int pos = 0, cpos = 0;
        	while(current->string()[cpos] < 0x20 || current->string()[cpos] > 0x7E){
        		if(current->string()[cpos] == 9)
        			pos += 4; //TAB-ADJUST
				else
					pos++;
				cpos++;
        	}
			current->set_pos(cpos);
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
	int tx = display->xPos(), ty = display->yPos();
    display->clear();
    Line* temp = top;

    for(unsigned int i=0;i<display->yMax() && temp;i++,temp = temp->next()){
        if(xShift < temp->string().length()){
            display->mv(0,i);
            display->print(temp->string().substr(xShift));
        }
    }
    display->refresh();
    display->mv(tx,ty);
}

void Application::renderLine(){
	int tx = display->xPos();
	display->mv(0, display->yPos());
	for(unsigned int i=0;i<display->xMax();i++){
		if(i + xShift < current->string().length())
			display->print(current->string()[i+xShift]);
		else
			display->print(' ');
	}
	display->refresh();
	display->mv(tx, display->yPos()-1);
}

void Application::renderNumbers(){
    numbers->clear();
    Line* temp = top;
    for(unsigned int i = topLineNum;i<topLineNum + numbers->yMax() && temp;i++,temp = temp->next()){
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
	control->clear();
	control->print(str, 0, 0);
	control->refresh();
	std::string input;
	do{
		if(!input.empty()){
			control->print("Please enter a variant of yes, no, or cancel into the box", 0, 1);
			control->refresh();
		}
		input = control->input(str.length(),0, control->xMax()-str.length());
	}while(!string_check(input, {"y", "Y", "Yes", "yes", "n", "N", "no", "No", "c", "C", "cancel", "Cancel"}));
    renderControl();
    if(input[0] == 'y' || input[0] == 'Y')
    	return 1;
	else if(input[0] == 'n' || input[0] == 'N')
		return 0;
	else
	    return -1;
}

std::string Application::promptString(std::string prmpt, std::string def){
	control->clear();
	control->print(prmpt, 0, 0);
	control->refresh();
    return control->input(prmpt.length(), 0, control->xMax()-prmpt.length(), def);
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

bool Application::string_check(std::string str, std::initializer_list<std::string> ini){
	for(std::string x:ini){
		if(!str.compare(x))
			return true;
	}
	return false;
}
