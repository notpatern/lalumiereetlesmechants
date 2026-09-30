#include "App.h"
#include <iostream>

App::App(Utilities::Timer* timer)
{
	m_timer = timer;
}

App::~App()
{
}

void App::Run() {
	std::cout << m_timer->getDeltaTime() << std::endl;
}

