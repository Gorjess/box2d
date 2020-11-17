#include "test.h"
#include <memory>

class foo {
public:
	foo()=delete;

	foo(const float a, const float b) : a_(a), b_(b)
	{
		printf("a:%.2f, b:%.2f", this->a_, this->b_);
	}

private:
	float a_;
	float b_;
};

class hello_world final: public Test {
	
public:
	hello_world()
	{							
		// ground box
		b2BodyDef ground_body_def;		
		ground_body_def.position.Set(0.0f, -10.0f);
		auto *ground_body = m_world->CreateBody(&ground_body_def);
		b2PolygonShape ground_box;
		ground_box.SetAsBox(50.0f, 10.0f);
		ground_body->CreateFixture(&ground_box, 0.0f);

		// dynamic body
		b2BodyDef body_def;
		body_def.type = b2_dynamicBody;
		body_def.position.Set(0.0f, 40.0f);
		auto *dy_body = m_world->CreateBody(&body_def);

		// shape rectangle
		b2PolygonShape dynamic_box;
		dynamic_box.SetAsBox(4.0f, 1.0f);

		// shape circle
		b2CircleShape circle;
		circle.m_radius = 1.0f;
		
		b2FixtureDef fixture_def;
		// fixture_def.shape = &dynamic_box;
		fixture_def.shape = &circle;
		fixture_def.density = 1.0f;
		fixture_def.friction = 0.3f;
		dy_body->CreateFixture(&fixture_def);

		//// simulate
		//auto time_step = 1.0f / 60.0f;
		//auto velocity_iteration = 6;
		//auto position_iteration = 2;

		//for (auto i=1; i < 60; ++i)
		//{
		//	m_world->Step(time_step, velocity_iteration, position_iteration);
		//	auto position = dy_body->GetPosition();
		//	auto angle = dy_body->GetAngle();
		//	printf("%4.2f %4.2f %4.2f\n", position.x, position.y, angle);
		//}
	}

	void Step(Settings& settings) override
	{
		Test::Step(settings);
	}

	static Test* Create()
	{
		return new hello_world;
	}
};

static int test_index = RegisterTest("Testbed", "hello_world", hello_world::Create);