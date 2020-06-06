#pragma once

#ifdef LE_PLATFORM_WINDOWS

extern le::App* le::create_app();

int main(int argc, char** argv)
{
	auto app = le::create_app();
	app->run();
	delete app;
}
#endif