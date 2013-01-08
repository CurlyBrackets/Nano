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
	log.close();
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
    tempPos = -1;

    log.open("log.txt");
}

int Application::execute(std::string filename){
    init();
    this->filename = filename;

    load();
    render();
    renderNumbers();
    renderControl();
    move(0,5);
    display->mv(0,0);
    current->set_pos(0);

    while(running){
        int in = getch();
        if(in == 258 || in == 259 || in == 339 || in == 338){
			if(tempPos == -1)
				tempPos = current->position();
        }
        else{
        	tempPos = -1;
        }

        if(in > 0x1F && in < 0x7F){//typable
			current->insert(in);
			changed = true;
			//render();//should be a single line refresh in the end
			renderLine();
			current->incrementPos(display->xPos());
			updateMove();
        }
        else if(in == CWin::key_backspace()){
			if(current->decrementPos()){
				log << "normal" << std::endl;
				changed = true;
				current->del();
				renderLine();
				updateMove();
			}
			else if(!display->xPos() && current->prev()){
				log << "fgsfds" << std::endl;
				changed = true;
				current->prev()->set_pos(current->prev()->string().length());//move the cursor to the joining point
				current->prev()->append(current->string());//move the strings together
				//some magic in order to pop the one node
				current = current->prev();
				Line* temp = current->next();
				if(temp->next()){
					temp->next()->prev(current);
					current->next(temp->next());
				}
				else
					current->next(NULL);
				delete temp;
				current->set_num(current->number());

				updateMove(-1);
				render();
				renderNumbers();
			}
        }
        else if(in == 330){//del
			if(!current->del()){
				renderLine();
			}
			else if(current->next()){
				current->append(current->next()->string());
				Line* temp = current->next();
				if(temp->next()){
					current->next(temp->next());
					current->next()->prev(current);
				}
				else
					current->next(NULL);
				delete temp;
				current->set_num(current->number());

				render();
				renderNumbers();
			}
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
				current->set_pos(tempPos);
				if(current->position() < xShift){
					if(current->position()-display->xMax() < 0)
						xShift = 0;
					else
						xShift = current->position()-display->xMax();
					render();
				}
				if(display->yPos() == display->yMax()-1){
					top = top->next();
					updateMove();
					render();
					renderNumbers();
				}
				else
					updateMove(1);
			}
        }
        else if(in == 259){//up
			if(current->prev()){
				current = current->prev();
				current->set_pos(tempPos);
				if(display->yPos() == 0 && top->prev()){
					top = top->prev();
					updateMove();
					render();
					renderNumbers();
				}
				else
					updateMove(-1);
			}
        }
        else if(in == 260){//left
        	int shift = current->decrementPos();
			if((int)display->xPos()+shift < 0 && xShift){
				xShift += shift;
				render();
			}
			else if(display->xPos()){
				updateMove();
			}
        }
        else if(in == 261){//right
			int shift = current->incrementPos(display->xPos());
			if(display->xPos()+shift > display->xMax()){
				xShift += shift;
				render();
			}
			else if(shift){
				updateMove();
			}
        }
        else if(in == CWin::key_end()){
        	current->set_pos(current->string().length());
        	if(current->position() < display->xMax()){
				updateMove();
			}
        	else{
        		xShift = current->position()-display->xMax()+1;
				updateMove();
				render();
        	}
        }
        else if(in == 262){//home
        	int pos = 0, cpos = 0;
        	while(current->string()[cpos]->ch() < 0x20 || current->string()[cpos]->ch() > 0x7E){
        		if(current->string()[cpos]->ch() == 9)
        			pos += 4; //TAB-ADJUST
				else
					pos++;
				cpos++;
        	}
        	xShift = 0;
			current->set_pos(cpos);
			updateMove();
			render();
        }
        else if(in == 443){//ctrl-left page left
			if(xShift){
				xShift--;
				render();
			}
        }
        else if(in == 444){//ctrl-right page right
			xShift++;
			render();
        }
        else if(in == 265){//F1 help

        }
        else if(in == 6){//^F find
			std::string what = promptString("String to search: ");

		}
        else if(in == 15){//^O Mode

        }
        else if(in == 19){//^S Save
            save();
        }
        else if(in == 18){//^R Reload

        }
        else if(in == 339){//page up
			if(top->prev()){
				top = top->prev();
				render();
				renderNumbers();
				updateMove(1);
				if(display->yPos() == display->yMax()){
					current->prev()->set_pos(tempPos);
					current = current->prev();
					updateMove(-1);
				}
			}
        }
        else if(in == 338){//page dn
			if(top->next()){//should be a check for the bottom?
				top = top->next();
				render();
				renderNumbers();
				updateMove(-1);
				if(display->yPos() == (unsigned int)-1){
					current->next()->set_pos(tempPos);
					current = current->next();
					updateMove(1);
				}
			}
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
            unsigned int pos = 0;
            for(auto it = temp->string().iter_at(xShift);it != temp->string().end() && pos < display->xMax();++it){
            	if((*it)->ch() > 0x1F && (*it)->ch() < 0x7F){
					pos++;
            		display->print((*it)->ch());
            	}
            	else if((*it)->ch() == '\t'){
					pos += 4-display->xPos()%5;
            		for(int i2=4-display->xPos()%5;i2>-1;i2--)
            			display->print(' ');
            	}
            }
        }
    }
    display->refresh();
    display->mv(tx,ty);
}

void Application::renderLine(){
	int tx = display->xPos();
	display->mv(0, display->yPos());
	unsigned int pos = 0;
	for(auto it = current->string().iter_at(xShift);it != current->string().end() && pos < display->xMax();++it){
		if((*it)->ch() > 0x1F && (*it)->ch() < 0x7F){
			pos++;
			display->print((*it)->ch());
		}
		else if((*it)->ch() == '\t'){
			pos += 4-display->xPos()%5;
			for(int i2=4-display->xPos()%5;i2>-1;i2--)
				display->print(' ');
		}
	}
	for(;pos<display->xMax();pos++)
		display->print(' ');
	display->refresh();
	display->mv(tx, display->yPos()-1);
}

void Application::renderNumbers(){
    numbers->clear();
    Line* temp = top;
    for(unsigned int i = top->number();i<top->number() + numbers->yMax() && temp;i++,temp = temp->next()){
        numbers->mv(0,i-top->number());
        std::string temp = to_string(i);
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
	for(auto it = ini.begin();it != ini.end();++it){
		if(!str.compare(*it))
			return true;
	}
	return false;
}

void Application::updateMove(int mx, int my){
	display->mv(mx, my);
	move(display->yPos(), 5+display->xPos());
}

void Application::updateMove(int my){
	display->mv(current->position()-xShift, display->yPos()+my);
	move(display->yPos(), 5+display->xPos());
}

void Application::updateMove(){
	display->mv(current->position()-xShift, display->yPos());
	move(display->yPos(), 5+display->xPos());
}

void Application::find(std::string what){
	int matchPos;
	for(unsigned int i=current->position();i<current->string().length();i++){

	}
	Line* temp = current->next();
	while(temp != current){
		if(!temp)
			temp = first;
		for(unsigned int i=0;i<temp->string().length();i++){
			for(matchPos = 0;matchPos < what.length() && what[matchPos] == temp->string()[i]->ch();i++,matchPos++);

			if(matchPos == what.length()){
				current = temp;
				Line* iter = top;
				for(unsigned int i2=0;i2<display->yMax() && iter;i2++){
					if(iter == current){
						display->mv(0,i2);
						iter = NULL;
					}
					iter = iter->next();
				}
				if(!iter){
					current = top;
					display->mv(0,0);
				}
				if(i > display->xMax())
					xShift = i-display->xMax();
				else
					xShift = 0;
				current->set_pos(i);
				updateMove();
				return;
			}
		}
	}

	for(unsigned int i =0;i<current->position();i++){
		//match check
	}
}

void Application::load(){
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
                if(first->string().length() > 0)
                	first->string()[0]->set_pos(0);
            }
            else{
                last->next(new Line(temp));
                last->next()->prev(last);
                last = last->next();
				if(last->string().length() > 0)
                	last->string()[0]->set_pos(0);
            }
        }
    }
    else{
        top = new Line();
        current = top;
        first = current;
    }
    infile.close();
    first->set_num(1);
}

std::string Application::to_string(unsigned int i){
	std::string temp, ret;
	while(i > 0){
		temp += (i%10)+48;
		i /= 10;
	}
	for(auto it = temp.rbegin();it != temp.rend();++it)
		ret += *it;
	return ret;
}
std::string Application::to_string(int i){
	if(i < 0)
		return "-"+to_string((unsigned int)-i);
	else
		return to_string((unsigned int)i);
}
