#include <lead_engine.h>

class ExampleLayer : public le::Layer
{
public:
	ExampleLayer() : Layer("Example")
	{
	}

	void update() override
	{
		LE_INFO("ExampleLayer::Update");
	}


	void onEvent(le::Event& event) override
	{
		LE_TRACE("{0}", event);
	}
};

class Sandbox : public le::App
{
public:
	Sandbox()
	{
		pushLayer(new ExampleLayer());
	}
	~Sandbox()
	{

	}
};

le::App* le::create_app()
{
	return new Sandbox();
}