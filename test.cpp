#include <iostream>

int main(int argc, char** argv){
	std::cout << "Test" << std::endl;
	if(argc > 1)
		std::cout << "Multiple args" << std::endl;
	else
		std::cout << "No args" << std::endl;
	return 0;
}
