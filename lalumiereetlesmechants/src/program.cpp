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

	// en gros ça tourne je met un commentaire juste pour dire que ça tourne et que le programme.cpp
	// c'est juste le main en gros
}
