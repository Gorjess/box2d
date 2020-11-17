#include "test.h"
#include <memory>
#include <iostream>


class hello_world final: public Test {

public:
	hello_world()
	{							
		// ground box
		{			
			b2BodyDef bd;
			bd.position.Set(0.0f, -10.0f);
			ground_body_ = m_world->CreateBody(&bd);

			b2PolygonShape ground_box;
			ground_box.SetAsBox(50.0f, 10.0f);
			ground_body_->CreateFixture(&ground_box, 0.0f);
		}		

		// dynamic body
		{
			b2BodyDef bd;
			bd.type = b2_dynamicBody;
			bd.position.Set(0.0f, 10.0f);
			bd.angle = 10.0f;
			ball_ = m_world->CreateBody(&bd);

			b2PolygonShape ball_box;
			ball_box.SetAsBox(4.0f, 1.0f);

			b2CircleShape circle;
			circle.m_radius = 1.5f;

			b2FixtureDef fd;
			fd.shape = &circle;
			fd.density = 1.0f;
			fd.friction = 0.4f;
			ball_->CreateFixture(&fd);
		}							
	}

	void Keyboard(const int key) override
	{
		switch (key)
		{
		case GLFW_KEY_A:
			this->mv_state_ = move_state::ems_left;
			break;
		case GLFW_KEY_D:
			this->mv_state_ = move_state::ems_right;
			break;
		default:
			this->mv_state_ = move_state::ems_idle;
		}
	}

	void Step(Settings& settings) override
	{		
		b2Vec2 pos = ball_->GetPosition();		
		switch (mv_state_)
		{
		case move_state::ems_left:
			pos.x -= 0.5f;
			break;
		case move_state::ems_right:
			pos.x += 0.5f;
			break;
		default:			
			break;
		}
		ball_->SetTransform(pos, ball_->GetAngle());
		
		Test::Step(settings);
	}

	static Test* Create()
	{
		return new hello_world;
	}

private:
	enum class move_state
	{
		ems_idle,
		ems_left,
		ems_right
	};
	move_state mv_state_ = move_state::ems_idle;

	b2Body* ground_body_;
	b2Body* ball_;
	b2Body* paddle_;
};

static int test_index = RegisterTest("Testbed", "hello_world", hello_world::Create);