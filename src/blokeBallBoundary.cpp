#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

constexpr int windowWidth{800}, windowHeight{600};

// Let's create some constants for the Ball class
constexpr float ballRadius{10.f}, ballVelocity(8.f);

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

int main()
{
	Ball ball{windowWidth / 2, windowHeight / 2};
	RenderWindow window{{windowWidth, windowHeight}, "Blokenoid - 4"};
	window.setFramerateLimit(60);

	while(true)
	{
		window.clear(Color::Color(32, 32, 32));

		if(Keyboard::isKeyPressed(Keyboard::Key::Escape)) break;

		// Every loop iteration, we need to update the ball
		ball.update();

		window.draw(ball.shape);
		window.display();
	}
	return 0;
}
