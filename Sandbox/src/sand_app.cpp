#include <lead_engine.h>

class Sandbox : public le::App
{
public:
	Sandbox()
	{

	}
	~Sandbox()
	{

	}
};

le::App* le::create_app()
{
	return new Sandbox();
}