#include <iostream>
#include <string>
#include <CWin.hpp>

std::string to_string(unsigned int i){
	std::string temp, ret;
	while(i > 0){
		temp += (i%10)+48;
		i /= 10;
	}
	for(auto it = temp.rbegin();it != temp.rend();++it)
		ret += *it;
	return ret;
}

int main(int argc, char** argv){
	std::cout << "Test" << std::endl;
	CWin::start();
	CWin* win = new CWin(0,0,CWin::window_x(), CWin::window_y(), true);
	refresh();
	for(int in=getch();in != 27;){
		win->print(to_string(in)+"\n");
		win->refresh();
		in = getch();
	}
	delete win;
	CWin::end();
	return 0;
}
