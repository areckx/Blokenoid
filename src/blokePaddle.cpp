#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

constexpr int windowWidth{800}, windowHeight{600};

// Let's create some constants for the Ball class
constexpr float ballRadius{10.f}, ballVelocity(8.f);

// LEt's create some constants for the paddle
constexpr float paddleWidth{60.f}, paddleHeight{20.f}, paddleVelocity{6.f};

struct Ball
{
	CircleShape shape;

	// 2D vector that stores the Ball's velocity
	Vector2f velocity{-ballVelocity, -ballVelocity};

	Ball(float mX, float mY)
	{
		shape.setPosition(mX, mY);
		shape.setRadius(ballRadius);
		shape.setFillColor(Color::Color(128, 128, 128));
		shape.setOrigin(ballRadius, ballRadius);
	}

	// Let's "update" the ball: move its shape
	// by the current velocity
	void update() { shape.move(velocity); 

	// We need to keep the ball "inside the screen"
	
	// If it's leaving toward the left, we need to set
	// horizontal velocity to a positive value (towards the right)
	if(left() < 0) velocity.x = ballVelocity;

	// Otherwise, if it's leaving towards the right, we need to
	// set horizontal velocity to a negative value (towards the left)
	else if(right() > windowWidth) velocity.x = -ballVelocity;

	// The same idea can be applied for top/bottom collisions
	if(top() < 0) velocity.y = ballVelocity;
	else if(bottom() > windowHeight) velocity.y = -ballVelocity;
}

// we can also create "property" methods to easily
// get commonly used values , i.e., the outside boundary of the ball
float x()		{  return shape.getPosition().x; 	}
float y()		{  return shape.getPosition().y; 	}
float left()		{  return x() - shape.getRadius();	}
float right()		{  return x() + shape.getRadius();	}
float top() 		{  return y() - shape.getRadius();	}
float bottom()		{  return y() + shape.getRadius();	}

};

struct Paddle
{
	// `RectangleShape` is an SFML class that defines 
	// a renderable rectangular shape
	RectangleShape shape;
	Vector2f velocity;

	// As with the ball, we construct the paddle with
	// arguments for initial position and pass the values
	// to the SFML shape
	Paddle(float mX, float mY) 	// horiz/verti
	{
		shape.setPosition(mX, mY);
		// quivalent line:
		// shape.setSize(Vector2f(paddleWidth, paddleHeight));
		shape.setSize({paddleWidth, paddleHeight});
		shape.setFillColor(Color::Color(128, 128, 128));
		shape.setOrigin(paddleWidth / 2.f, paddleHeight / 2.f);
	}

	void update()
	{
		shape.move(velocity);

		// To move the paddle, we check if the user is pressing
		// the left or right arrow key: if so, we change the velocity
		
		// To keep the paddle inside the window, we change the velocity 
		// only if its position is inside the window
		if(Keyboard::isKeyPressed(Keyboard::Key::Left)
				&& left() > 0) velocity.x = -paddleVelocity;
		else if(Keyboard::isKeyPressed(Keyboard::Key::Right)
				&& right() < windowWidth) velocity.x = paddleVelocity;
		
		// if the user isn't pressing anything, stop moving
		else velocity.x = 0;
	}

	float x()		{ return shape.getPosition().x;		}
	float y()		{ return shape.getPosition().y;		}
	float left()		{ return x() - shape.getSize().x / 2.f;	}
	float right()		{ return x() + shape.getSize().x / 2.f;	}
	float top()		{ return y() - shape.getSize().y / 2.f;	}
	float bottom()		{ return y() + shape.getSize().y / 2.f;	}
};
			
int main()
{
	Ball ball{windowWidth / 2, windowHeight / 2};

	// Let's create a Paddle `instance`
	Paddle paddle{windowWidth / 2, windowHeight - 50};

	RenderWindow window{{windowWidth, windowHeight}, "Blokenoid - 5"};
	window.setFramerateLimit(60);

	while(true)
	{
		window.clear(Color::Color(32, 32, 32));

		if(Keyboard::isKeyPressed(Keyboard::Key::Escape)) break;

		// Every loop iteration, we need to update the ball
		ball.update();

		// Let's update our paddle
		paddle.update();

		window.draw(ball.shape);

		// then we need to draw the damn paddle!
		window.draw(paddle.shape);
		window.display();
	}
	return 0;
}
