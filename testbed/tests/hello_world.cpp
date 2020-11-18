#include "test.h"
#include <memory>


class hello_world final: public Test {

public:
	hello_world()
	{
		m_world->SetGravity(b2Vec2(0.0f, 0.0f));
		
		// ground box
		{			
			b2BodyDef bd;
			bd.position.Set(0.0f, 0.0f);
			ground_body_ = m_world->CreateBody(&bd);
			ground_body_->SetBullet(true);

			// shape definition
			b2PolygonShape polygon_shape;
			polygon_shape.SetAsBox(5.0f, 5.0f);

			auto create_barrier = [&](const b2Vec2& pos, const b2Vec2& center, const float& angle)
			{
				polygon_shape.SetAsBox(pos.x, pos.y, center, angle);
				ground_body_->CreateFixture(&polygon_shape, 0.0f);
			};

			// walls
			create_barrier(b2Vec2(20.0f, 1.0f), b2Vec2(0.0f, 0.0f), 0.0f);     // bottom
			create_barrier(b2Vec2(20.0f, 1.0f), b2Vec2(0.0f, 40.0f), 0.0f);	// top
			create_barrier(b2Vec2(1.0f, 20.0f), b2Vec2(-20.0f, 20.0f), 0.0f);	// left
			create_barrier(b2Vec2(1.0f, 20.0f), b2Vec2(20.0f, 20.0f), 0.0f);	// right					
		}		

		// paddle
		{
			b2BodyDef bd;
			bd.type = b2_dynamicBody;
			bd.position.Set(20.0f, 1.0f);
			paddle_ = m_world->CreateBody(&bd);

			b2PolygonShape paddle_box;
			paddle_box.SetAsBox(4.0f, 1.0f, b2Vec2(4.0f, 1.0f), 0.0f);			

			b2FixtureDef fd;
			fd.shape = &paddle_box;
			fd.density = 1.0f;

			paddle_->CreateFixture(&fd);
			paddle_->SetBullet(true);
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
			fd.density = 10.0f;			
			fd.restitution = 1.0f;
			ball_->CreateFixture(&fd);

			ball_->SetBullet(true);
			ball_->SetLinearVelocity(b2Vec2{ 10.0f, 10.0f });		
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
		case move_state::ems_idle:
			stop_paddle();			
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