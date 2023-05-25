#pragma once
uint32_t mergedFlags(std::set<uint32_t> flagSet){
	std::set<uint32_t>::iterator it=flagSet.begin();
	uint32_t resltWinFlag=*it;
	it++;
	while(it!=flagSet.end()){
		resltWinFlag|=*it;
		it++;
    }
    
    return resltWinFlag;
}

void dbgHere(){
    std::cout<<"\033[2;31m\n----\n  ----\n    HERE!    \n  ----\n----\n\033[0;37m";
}

#include "profiler.cpp"
#include "logF/logF.h"
