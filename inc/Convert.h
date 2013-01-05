#ifndef CURLY_CONVERT_H
#define CURLY_CONVERT_H
#include <string>

namespace Curly{
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
	std::string to_string(int i){
		if(i < 0)
			return "-"+to_string((unsigned int)-i);
		else
			return to_string((unsigned int)i);
	}
}

#endif // CURLY_CONVERT_H
