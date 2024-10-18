#include "SFML/Graphics.hpp"
#include <cstdlib>
#include <string>

using namespace sf;
using namespace std;

#define WIDTH 600
#define HEIGHT 300

struct Position
{
	int x;
	int y;
};

int main()
{
	RenderWindow window(VideoMode(WIDTH, HEIGHT), "Dinosaur Game. By Q");
	window.setFramerateLimit(60);

	Texture t1;
	Texture t2;
	t1.loadFromFile("images/dino1.png");
	t2.loadFromFile("images/dino2.png");

	Sprite dinoArr[2];
	dinoArr[0] = Sprite(t1);
	dinoArr[1] = Sprite(t2);

	static const int DINO_Y_BOTTOM = HEIGHT - t1.getSize().y;
	Position dinoPos;
	dinoPos.x = 50;
	dinoPos.y = DINO_Y_BOTTOM;

	int index = 0;
	float frame = 0.f;
	float frameSpeed = 0.4f;
	const int changeCount = 5;

	const int gravity = 5;
	bool isJumping = false;
	bool isBottom = true;

	//tree
	Texture t3;
	t3.loadFromFile("images/tree.png");
	Sprite tree(t3);

	static const int TREE_Y_BOTTOM = HEIGHT - t3.getSize().y;
	Position treePos;
	treePos.x = WIDTH - 20;
	treePos.y = TREE_Y_BOTTOM;

	const int treeSpeed = 4;

	//coin
	Texture t4;
	t4.loadFromFile("images/coin.png");
	Sprite coin(t4);

	static const int COIN_Y_BOTTOM = HEIGHT - t4.getSize().y - 200;
	srand(time(NULL));
	int randomX = 0;
	Position coinPos;
	
	//coinPos.x = WIDTH - 20;
	coinPos.x = randomX;
	coinPos.y = COIN_Y_BOTTOM;

	//get size
	int dinoWidth = t1.getSize().x;
	int dinoHeight = t1.getSize().y;
	int treeWidth = t3.getSize().x;
	int treeHeight = t3.getSize().y;

	int coinWidth = t4.getSize().x;
	int coinHeight = t4.getSize().y;

	bool isGameOver = false;

	//text
	Font font;
	if (!font.loadFromFile("ariblk.ttf"))
	{
		return -1;
	}

	Text text;
	text.setFont(font);
	text.setString("Collision detected");
	text.setCharacterSize(24);
	text.setFillColor(Color::Red);
	text.setPosition(180, 60);

	int score = 0;
	Text scoreText;
	scoreText.setFont(font);
	scoreText.setString("Score: 0");
	scoreText.setCharacterSize(15);
	scoreText.setFillColor(Color::Black);
	scoreText.setPosition(520, 10);

	while (window.isOpen())
	{
		//check collision
		bool isCollidingHorizontally = (dinoPos.x + dinoWidth >= treePos.x + 30 && dinoPos.x <= treePos.x + treeWidth);
		bool isCollidingVertically = (dinoPos.y + dinoHeight >= treePos.y + 9);

		bool isColliding = isCollidingHorizontally && isCollidingVertically;

		//check eat coin
		bool isEatenHorizontally = (dinoPos.x + dinoWidth >= coinPos.x && dinoPos.x <= coinPos.x);
		bool isEatenVertically = (dinoPos.y + dinoHeight >= coinPos.y && dinoPos.y <= coinPos.y);

		bool isEaten = isEatenHorizontally && isEatenVertically;

		if (isColliding)
		{
			isGameOver = true;
			window.draw(text);
			window.display();
			while (window.isOpen())
			{
				Event e;
				while (window.pollEvent(e))
				{
					if (e.type == Event::Closed)
					{
						window.close();
					}				
					else if (e.type == Event::KeyPressed)
					{
						isGameOver = false;
						break;
					}	
				}	
			}
		}
		
		//dino jump
		if (Keyboard::isKeyPressed(Keyboard::Space))
		{
			if (isBottom && !isJumping)
			{
				isJumping = true;
				isBottom = false;
			}
		}
		
		//dino jump(up and down)
		if (isJumping)
		{
			dinoPos.y -= gravity;
		}
		else
		{
			dinoPos.y += gravity;
		}

		//dino jump limit, dino bottom limit
		if (dinoPos.y >= DINO_Y_BOTTOM)
		{
			dinoPos.y = DINO_Y_BOTTOM;
			isBottom = true;
		}
		if (dinoPos.y <= DINO_Y_BOTTOM - 150)
		{
			isJumping = false;
		}

		//dino step
		frame += frameSpeed;
		if (frame > changeCount)
		{
			frame -= changeCount;
			++index;
			if (index >= 2)
			{
				index = 0;
			}
		}

		//tree move
		if (treePos.x <= 0)
		{
			treePos.x = WIDTH;
		}
		else
		{
			treePos.x -= treeSpeed;
		}

		//coin move
		if (coinPos.x <= 0)
		{
			randomX = rand() % (WIDTH - t4.getSize().x);
			coinPos.x = randomX;
		}
		else
		{
			coinPos.x -= treeSpeed;
		}

		//tree position
		tree.setPosition(treePos.x, treePos.y);

		//dino position
		dinoArr[index].setPosition(dinoPos.x, dinoPos.y);

		//coin position
		coin.setPosition(coinPos.x, coinPos.y);

		//check eats coin
		if (isEaten)
		{
			score++;
			scoreText.setString("Score: " + to_string(score));
			randomX = rand() % (WIDTH - t4.getSize().x);
			coinPos.x = randomX;
		}
	

		window.clear(Color::White); // Color background
		window.draw(dinoArr[index]);
		window.draw(tree);
		window.draw(coin);
		window.draw(scoreText);
		window.display();
	}

	return 0;
}