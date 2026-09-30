#include <iostream>
#include "App/App.h"
#include "Utilities/Timer.h"
#include "Utilities/Vector2.h"

int main()
{
	Utilities::Timer* timer = new Utilities::Timer();
	App app{timer};
	while (app.getIsRunning()) {
		app.Run();
	}
}
