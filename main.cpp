#include "application.h"
#include <iostream>
#include <memory>
#undef main

int main() {
	try{
        std::unique_ptr<Application> app(new Application);
        if(app->start())
            app->loop();
	}catch(const Assertion_Failed_Exception & e){
	    std::cerr << e.what();
	}
	catch(const std::exception & e){
        std::cerr << e.what();
	}
	return 0;
}
