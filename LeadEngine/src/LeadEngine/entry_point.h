#pragma once

#ifdef LE_PLATFORM_WINDOWS

extern le::App* le::create_app();

int main(int argc, char** argv)
{
	le::Log::init();
	LE_CORE_WARN("Initialised log.");
	LE_INFO("Hi I'm the App!");

	auto app = le::create_app();
	app->run();
	delete app;
}
#endif