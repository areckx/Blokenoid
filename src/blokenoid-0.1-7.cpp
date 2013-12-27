#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

constexpr int windowWidth{800}, windowHeight{600};

// Let's create some constants for the Ball class
constexpr float ballRadius{10.f}, ballVelocity(8.f);

// Let's create some constants for the paddle
constexpr float paddleWidth{60.f}, paddleHeight{20.f}, paddleVelocity{6.f};

// Let's define some constants for the bricks
constexpr float blockWidth{60.f}, blockHeight{20.f};
constexpr int countBlocksX{11}, countBlocksY{4}; // blocks column X by rows Y

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
	void update() 
	{
		shape.move(velocity); 
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

// Let's have a class `brick` for the bricks
struct Brick
{
	RectangleShape shape;

	// This boolean value will be used to check
	// whether a brick has been hit or not
	bool destroyed{false};

	// The constructor is almost identical to the `Paddle` one
	Brick(float mX, float mY)
	{
		shape.setPosition(mX, mY);
		shape.setSize({blockWidth, blockHeight});
		shape.setFillColor(Color::Color(32, 128, 76));
		shape.setOrigin(blockWidth / 2.f, blockHeight / 2.f);
	}

	float x()		{ return shape.getPosition().x;		}
	float y() 		{ return shape.getPosition().y;		}
	float left() 		{ return x() - shape.getSize().x / 2.f;	}
	float right()		{ return x() + shape.getSize().x / 2.f;	}
	float top()		{ return y() - shape.getSize().y / 2.f;	}
	float bottom()		{ return y() + shape.getSize().y / 2.f;	}
};

// Dealing with collisions: let's define a generic function
// to check if two shapes are intersecting (colliding)
template<class T1, class T2> bool isIntersecting(T1& mA, T2 mB)  // 2 objects
{
	return mA.right() >= mB.left() && mA.left() <= mB.right()
		&& mA.bottom() >= mB.top() && mA.top() <= mB.bottom();
}
// Let's define a function that deals with paddle/ball collision
void testCollision(Paddle& mPaddle, Ball& mBall)
{
	// If there's no intersection, get out of the function
	if(!isIntersecting(mPaddle, mBall)) return;

	// Otherwise let's "push" the ball upwards
	mBall.velocity.y = -ballVelocity;

	// And let's direct it dependently on the position 
	// where the paddle was hit
	if(mBall.x() < mPaddle.x()) mBall.velocity.x = -ballVelocity;
	else mBall.velocity.x = ballVelocity;
}

			
int main()
{
	Ball ball{windowWidth / 2, windowHeight / 2};

	// Let's create a Paddle `instance`
	Paddle paddle{windowWidth / 2, windowHeight - 50};

	// We will use an `std::vector` to contain any number
	// of `Brick` instances
	vector<Brick> bricks;

	// We will fill up our vector via a 2D `for` loop, creating
	// bricks in a grid-like pattern
	for(int columnsX{0}; columnsX < countBlocksX; ++columnsX)
		for(int rowsY{0}; rowsY < countBlocksY; ++rowsY)
			bricks.emplace_back((columnsX + 1) * (blockWidth + 3) + 22, // emplace ++NEW++
					(rowsY + 2) * (blockHeight + 3));

	RenderWindow window{{windowWidth, windowHeight}, "blokenoid-0.1-7"};
	window.setFramerateLimit(60);

	while(true)
	{
		window.clear(Color::Color(32, 32, 32));

		if(Keyboard::isKeyPressed(Keyboard::Key::Escape)) break;

		// Every loop iteration, we need to update the ball
		ball.update();

		// Let's update our paddle
		paddle.update();

		// Let's test the collision every game loop iteration
		testCollision(paddle, ball);

		window.draw(ball.shape);

		// then we need to draw the damn paddle!
		window.draw(paddle.shape);

		// We must draw every brick on the window!
		// Let's use a modern C++11 `foreach` loop that allows
		// us to intuitively say: "draw every `brick` in `bricks`"
		for(auto& brick : bricks) window.draw(brick.shape);
		window.display();
	}
	return 0;
}
