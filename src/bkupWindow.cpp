#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

// `constexpr` defines an immutable compile-time value
constexpr int windowWidth{800}, windowHeight{600};

int main()
{
	// Creation of the game window
	RenderWindow window{{windowWidth, windowHeight}, "Blokenoid -1"};
	window.setFramerateLimit(60);

	// Game loop
	while(true)
	{
		// "Clear" the window from previously drawn graphics
	 	window.clear(Color::Color(32,32,32));	

		// If "Escape" is pressed, bloke out of the loop
		// ...break out...
		if(Keyboard::isKeyPressed(Keyboard::Key::Escape)) break;

		// Show the window contents
		window.display();
	}
	return 0;
}

				
