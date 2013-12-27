#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

constexpr int windowWidth{800}, windowHeight{600};
constexpr float ballRadius{10.f};

// Let's create a class for our Ball
// `struct` == `class` in C++
struct Ball // `struct` is implicitly `public`
{
	// CircleShape is an SFML class that
	// defines a renderable circle
	CircleShape shape;

	// Let's create the Ball constructor
	// argument mX -> starting x coordinate
	// argument xY -> starting y coordinate
	Ball(float mX, float mY)
	{
		// Apply position, radius, color and origin
		// to the CircleShape `shape`
		shape.setPosition(mX, mY);
		shape.setRadius(ballRadius);
		shape.setFillColor(Color::Color(128, 128, 128));
		shape.setOrigin(ballRadius, ballRadius);
	}
};

int main()
{
	// Let's create an instance of `Ball`
	// positioned at the center of the window
	Ball ball(windowWidth / 2, windowHeight / 2);

	RenderWindow window{{windowWidth, windowHeight}, "Blokenoid - 2"};
	window.setFramerateLimit(60);

	while(true)
	{
		window.clear(Color::Color(32, 32, 32));

		if(Keyboard::isKeyPressed(Keyboard::Key::Escape)) break;

		// Let's render the Ball instance on the window
		window.draw(ball.shape);
		window.display();
	}
	return 0;
}
