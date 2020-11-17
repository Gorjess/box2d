#include "test.h"
#include <memory>


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

		// paddle
		{
			b2BodyDef bd;
			bd.type = b2_dynamicBody;
			bd.position.Set(0.0f, 0.0f);			
			paddle_ = m_world->CreateBody(&bd);

			b2PolygonShape ball_box;
			ball_box.SetAsBox(4.0f, 1.0f);

			/*b2CircleShape circle;
			circle.m_radius = 1.5f;*/

			b2FixtureDef fd;
			fd.shape = &ball_box;
			fd.density = 1.0f;
			fd.friction = 0.4f;
			paddle_->CreateFixture(&fd);
		}

		// ball
		{
			b2BodyDef bd;
			bd.type = b2_dynamicBody;
			bd.position.Set(0.0f, 20.0f);			
			ball_ = m_world->CreateBody(&bd);			

			b2CircleShape circle;
			circle.m_radius = 1.5f;

			b2FixtureDef fd;
			fd.shape = &circle;
			fd.density = 1.0f;
			fd.friction = 0.4f;
			ball_->CreateFixture(&fd);
		}
	}

	void stop_paddle() const	
	{		
		paddle_->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
		paddle_->SetAngularVelocity(0.0f);
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

	void KeyboardUp(int key) override
	{
		mv_state_ = move_state::ems_idle;
		stop_paddle();
	}

	void Step(Settings& settings) override
	{		
		b2Vec2 pos = paddle_->GetPosition();		
		switch (mv_state_)
		{
		case move_state::ems_left:
			pos.x -= 0.5f;
			break;
		case move_state::ems_right:
			pos.x += 0.5f;
			break;
		default:
			stop_paddle();
			break;
		}
		paddle_->SetTransform(pos, paddle_->GetAngle());		
		
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