#include "Application.h"

int main(int argc, char** argv){
    if(argc != 2){
        return -1;
    }
    return Application::run(argv[1]);
}
