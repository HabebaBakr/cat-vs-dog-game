#include <SFML/Graphics.hpp>
#include <SFML/system.hpp>
#include <SFML/window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <iostream>
#include <math.h>
#include <ctime>
#include <vector>
#include <string>
#include <vector>
#include <utility>
#include<windows.h>
using namespace std;
using namespace sf;
struct HealthBar
{
	RectangleShape back, front[5];
	int maxHits = 5;
	int takenHits = 0;
	bool isdied = false;

	HealthBar(Vector2f size, Vector2f position, Color backColor, Color fronColor) {
		back.setSize(size);
		back.setPosition(position);
		back.setFillColor(backColor);

		for (int i = 0; i < 5; i++) {
			front[i].setSize(Vector2f((size.x / 5), size.y));
			front[i].setPosition(Vector2f(position.x + (i * 100), position.y));
			front[i].setFillColor(fronColor);
		}
	}

	void takeHit()
	{
		if (takenHits >= maxHits) return;
		takenHits += 1;
		front[maxHits - takenHits].setScale(0, 0);
		if (takenHits == maxHits) isdied = true;
	}

	void draw(RenderWindow& window) {
		window.draw(back);
		for (int i = 0; i < 5; i++) window.draw(front[i]);
	}

};

float calcuateThrowSpeedFromHoldingTime(float time)
{
	return -(20.f * time);
}

int main()
{
	srand(time(0));
	RenderWindow window(VideoMode(1210, 680), "cat vs dog");

	// Constants
	const int HIGHT_THRESHOLD = 200;

	float  time = 0, DeltaTime = 0;

	int dogAnimationIndecator = 0, throwSpeed = 0;

	bool isDogTurn = true, canPlay = true, isPressing = false, game = true,play=true;

	//time
	Clock clock, deltaTimeClock, dogAnimationClock, catClock;

	//background
	Texture backgroundTexture;
	backgroundTexture.loadFromFile("image/background2.png");
	Sprite background(backgroundTexture);
	background.setScale(0.9, 0.9);

	//wall
	Texture wallTexture;
	wallTexture.loadFromFile("image/wall2.png");
	Sprite wall(wallTexture);
	wall.setPosition(615, 310);
	wall.setOrigin(wall.getGlobalBounds().width / 2, wall.getGlobalBounds().height / 2);
	wall.setScale(0.9, 0.9);

	//cat
	Texture catTexture;
	catTexture.loadFromFile("image/cat.png");
	Sprite cat(catTexture);
	cat.setPosition(460, 520);
	cat.setOrigin(cat.getGlobalBounds().width / 2, cat.getGlobalBounds().height / 2);
	cat.setScale(0.7, 0.7);
	cat.setTextureRect(IntRect(0, 0, 228, 155));

	//dog
	Texture dogTexture;
	dogTexture.loadFromFile("image/dog.png");
	Sprite dog(dogTexture);
	dog.setPosition(1520, 600);
	dog.setOrigin(dog.getGlobalBounds().width / 2, dog.getGlobalBounds().height / 2);
	dog.setScale(0.7, 0.7);
	dog.setTextureRect(IntRect(0, 0, 328.6, 224));
	Vector2u texturesize = dogTexture.getSize();
	texturesize.x /= 5;
	texturesize.y /= 2;

	//cat health
	HealthBar catHp(Vector2f(500.f, 20.f), Vector2f(20.f, 20.f), Color(50, 50, 50, 200), Color(250, 20, 20, 200));

	//dog health
	HealthBar dogHp(Vector2f(500.f, 20.f), Vector2f(680.f, 20.f), Color(50, 50, 50, 200), Color(250, 20, 20, 200));


	//bone
	Texture boneTexture;
	boneTexture.loadFromFile("image/bone.png");
	Sprite bone(boneTexture);
	bone.setPosition(950, 450);
	bone.setScale(0.7, 0.7);

	//rubbish
	Texture rubbishTexture;
	rubbishTexture.loadFromFile("image/rubbish.png");
	Sprite rubbish(rubbishTexture);
	rubbish.setPosition(110, 450);
	rubbish.setScale(0.7, 0.7);



	//PAUSE
	RectangleShape pause(Vector2f(100, 100));
	pause.setPosition(550, 0);
	Texture tx;
	tx.loadFromFile("image/pause4.png");
	pause.setTexture(&tx);


	//CONTINUE
	RectangleShape Continue(Vector2f(350, 250));
	Continue.setPosition(400, 200);
	Texture tx2;
	tx2.loadFromFile("image/continue2.png");
	Continue.setTexture(&tx2);

	//game over
	RectangleShape gameover(Vector2f(490, 400));
	gameover.setPosition(380, 100);
	Texture tx3;
	tx3.loadFromFile("image/game over.png");
	gameover.setTexture(&tx3);


	// cat pain sound
	SoundBuffer soundBuffer_cat;
	Sound sound_cat;
	if (!soundBuffer_cat.loadFromFile("image/cat2.wav")) {
		cout << " i cant find cat2.wav " << endl;
	}
	sound_cat.setBuffer(soundBuffer_cat);


	// DOG pain sound
	SoundBuffer soundBuffer_dog;
	Sound sound_dog;
	if (!soundBuffer_dog.loadFromFile("image/dog2.wav")) {
		cout << " i cant find dog2.wav " << endl;
	}
	sound_dog.setBuffer(soundBuffer_dog);


	//cat laughing sound
	SoundBuffer soundBuffer_laughcat;
	Sound sound_laughcat;
	if (!soundBuffer_laughcat.loadFromFile("image/cat laugh.wav")) {
		cout << " i cant find cat2.wav " << endl;
	}
	sound_laughcat.setBuffer(soundBuffer_laughcat);

	//dog laughing sound
	SoundBuffer soundBuffer_laughdog;
	Sound sound_laughdog;
	if (!soundBuffer_laughdog.loadFromFile("image/dog laugh.wav")) {
		cout << " i cant find dog2.wav " << endl;
	}
	sound_laughdog.setBuffer(soundBuffer_laughdog);



	// hit sound
	SoundBuffer soundBuffer_hit;
	Sound sound_hit;
	if (!soundBuffer_hit.loadFromFile("image/hit.wav")) {
		cout << " i cant find hit.wav " << endl;
	}
	sound_hit.setBuffer(soundBuffer_hit);

	// game sound
	SoundBuffer soundBuffer_game;
	Sound sound_game;
	if (!soundBuffer_game.loadFromFile("image/game2.wav")) {
		cout << " i cant find game2.wav " << endl;
	}
	sound_game.setBuffer(soundBuffer_game);
	sound_game.play();

	while (window.isOpen())
	{

		Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();

			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
				Vector2i mousepos = Mouse::getPosition(window);

				if (mousepos.x > 568 && mousepos.x < 632 && mousepos.y>16 && mousepos.y < 81) {
					game = false;
				}


				if (mousepos.x > 412 && mousepos.x < 740 && mousepos.y >265 && mousepos.y < 385) {
					game = true;
				}
			}

			if (game&&play) {
				if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left && isDogTurn) {
					isPressing = true;
					//calculate time
					clock.restart();
				}

				if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left && isDogTurn) {
					isPressing = false;
					time = clock.getElapsedTime().asSeconds()+2;
					throwSpeed = calcuateThrowSpeedFromHoldingTime(time);
					canPlay = false;
					dogAnimationIndecator = 3;
					dogAnimationClock.restart();
				}

			}

		}

		if (game && play) {
			if (isPressing) {
				if (dogAnimationClock.getElapsedTime().asSeconds() > 1.f)
					dogAnimationIndecator = 2;
				else
					dogAnimationIndecator = 1;
			}

			if (canPlay && !isDogTurn && throwSpeed == 0) {
				if (catClock.getElapsedTime().asSeconds() > 3.f) {
					time = (rand() % 5) + 1;
					throwSpeed = calcuateThrowSpeedFromHoldingTime(time);
					
				}
			}

			dog.setTextureRect(IntRect(texturesize.x * dogAnimationIndecator, texturesize.y * 0, texturesize.x, texturesize.y));

			if (throwSpeed != 0) {
				if (isDogTurn)
					bone.move(-500 * DeltaTime, 5 * DeltaTime * throwSpeed);
				else
					rubbish.move(500 * DeltaTime, 5 * DeltaTime * throwSpeed);
			}
			//change the direction of the velocity
			if (bone.getPosition().y < HIGHT_THRESHOLD || rubbish.getPosition().y < HIGHT_THRESHOLD) {
				throwSpeed *= -1;
			}


			if ((bone.getPosition().y >= 690 && isDogTurn) || (bone.getScale().x == 0 && isDogTurn)) {
				isDogTurn = false;
				throwSpeed = 0;
				canPlay = true;
				dogAnimationIndecator = 0;
				catClock.restart();
				rubbish.setPosition(120, 450);
				rubbish.setScale(0.7, 0.7);
			}

			if ((rubbish.getPosition().y >= 690 && !isDogTurn) || (rubbish.getScale().x == 0 && !isDogTurn))
			{
				throwSpeed = 0;
				canPlay = false;
				isDogTurn = true;
				bone.setPosition(950, 450);
				bone.setScale(0.7, 0.7);

			}
			//cat collision 
			if (bone.getGlobalBounds().intersects(cat.getGlobalBounds())) {
				catHp.takeHit();
				bone.setScale(0, 0);
				sound_hit.play();
				sound_cat.play();
				sound_laughdog.play();
			}
			//dog collision
			if (rubbish.getGlobalBounds().intersects(dog.getGlobalBounds())) {
				dogHp.takeHit();
				rubbish.setScale(0, 0);
				sound_hit.play();
				sound_dog.play();
				sound_laughcat.play();
			}
			//who is the winner??
			if (dogHp.isdied) {
				cout << "Cat Wins\n";
				play = false;

			}
			if (catHp.isdied) {
				cout << "Dog Wins\n";
				play = false;
			}
		}

		window.clear();

		window.draw(background);
		window.draw(wall);
		window.draw(dog);
		window.draw(cat);
		window.draw(bone);
		window.draw(rubbish);

		catHp.draw(window);
		dogHp.draw(window);

		window.draw(pause);

		if (!game)
			window.draw(Continue);
		if (catHp.isdied || dogHp.isdied)
			window.draw(gameover);


		window.display();

		DeltaTime = deltaTimeClock.restart().asSeconds();

	}
	return 0;
}