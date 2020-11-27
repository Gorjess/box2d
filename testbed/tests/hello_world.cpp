#include "test.h"
#include <memory>
#include <vector>

class hello_world final: public Test {

public:
	hello_world()
	{
		m_world->SetGravity(b2Vec2(0.0f, 0.0f));

		// barriers
		{
			const auto angle = .0f;
			const auto center = b2Vec2(.0f, .0f);
			auto create_barrier = [&](const b2Vec2& shape, const b2Vec2& pos)
			{
				b2BodyDef bd;
				bd.position.Set(pos.x, pos.y);
				auto* body = m_world->CreateBody(&bd);				

				b2PolygonShape polygon_shape;
				polygon_shape.SetAsBox(shape.x, shape.y);
				body->CreateFixture(&polygon_shape, 0.0f);

				body->SetBullet(true);
				m_barriers_.push_back(body);
			};

			// walls
			auto* rect = new b2Vec2(20.f, 40.f);
			assert(rect);

			const auto factor = .5f;

			constexpr auto wall_thickness = 1.5f;
			const auto vertical_barrier = b2Vec2(wall_thickness * factor, rect->y * factor);

			constexpr auto h_bar_length = 1.f;
			const auto horizonal_barrier = b2Vec2(h_bar_length * factor, wall_thickness * factor);

			create_barrier(vertical_barrier, b2Vec2(.0f, .0f));					// left
			create_barrier(vertical_barrier, b2Vec2(rect->x, .0f));	// right
			create_barrier(horizonal_barrier, b2Vec2(.0f, .0f/*wall_thickness, rect->y - wall_thickness*/));		// left top
			create_barrier(horizonal_barrier, b2Vec2(rect->x - h_bar_length, rect->y - wall_thickness)); // right top		

			printf("bar0: %.2f, %.2f\n", m_barriers_[0]->GetPosition().x, m_barriers_[0]->GetPosition().y);
			printf("bar1: %.2f, %.2f\n", m_barriers_[1]->GetPosition().x, m_barriers_[1]->GetPosition().y);
		}
		
		// ground box
		/*{			
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
			create_barrier(b2Vec2(50.0f, 50.0f), b2Vec2(0.0f, 0.0f), 0.0f);     // bottom
			//create_barrier(b2Vec2(20.0f, 1.0f), b2Vec2(0.0f, 40.0f), 0.0f);	// top
			//create_barrier(b2Vec2(1.0f, 20.0f), b2Vec2(-20.0f, 20.0f), 0.0f);	// left
			//create_barrier(b2Vec2(1.0f, 20.0f), b2Vec2(20.0f, 20.0f), 0.0f);	// right					
		}*/		

		// paddle
		{
			b2BodyDef bd;
			bd.type = b2_dynamicBody;
			bd.position.Set(2.0f, .0f);
			paddle_ = m_world->CreateBody(&bd);

			b2PolygonShape paddle_box;
			paddle_box.SetAsBox(.5f, .25f, b2Vec2(4.0f, 1.0f), 0.0f);			

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
			bd.position.Set(2.0f, 2.0f);			
			ball_ = m_world->CreateBody(&bd);			

			b2CircleShape circle;
			circle.m_radius = .7f;

			b2FixtureDef fd;
			fd.shape = &circle;
			fd.density = 1.0f;			
			fd.restitution = 1.0f;
			ball_->CreateFixture(&fd);

			ball_->SetBullet(true);
			ball_->SetLinearVelocity(b2Vec2{ 8.0f, .0});		
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

		const auto ball_pos = ball_->GetPosition();
		printf("ball:%.2f\n", ball_pos.x);
		
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
	std::vector<b2Body*> m_barriers_;
};

static int test_index = RegisterTest("Testbed", "hello_world", hello_world::Create);