#include <iostream>
#include "App/App.h"
#include "Utilities/Timer.h"

int main()
{
	Utility::Timer* timer = new Utility::Timer();
	App app{timer};
	while (app.getIsRunning()) {
		app.Run();
	}
}
