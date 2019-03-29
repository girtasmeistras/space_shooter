#include "application.h"
#undef main

int main() {

	Application app;
	if(app.start() == 1)
        app.loop();

	return 0;
}
