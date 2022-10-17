#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include<iostream>
#include<string>
#include<time.h>
#include<Windows.h>
int tetramino[7][4] =
{
	1,3,5,7,
	2,3,4,5,
	3,5,7,6,
	3,5,7,4,
	3,5,4,6,
	2,3,5,7,
	2,4,5,7,
};
const int minX = 10;
const int maxX = 20;
const int maxY = 18;
const int X = maxX - minX;

const std::string way = "source\\";

int field[maxY][X] = { 0 }; // Игровое поле

void delFigure();
struct element {
	int x = 0;
	int y = 0;
} figure[4], buf[4]; // Тетрамино;

bool check(bool &game) {
	for (int i = 0; i < 4; i++) {
		if (figure[i].x < minX || figure[i].x >= maxX || figure[i].y >= maxY) return 0;
		if (field[figure[i].y][figure[i].x]) {
			if (figure[i].y < 3) {		
				game = false;
			}
			return 0;
		}
	
	}
	return 1;
}
void destruction(int &game_points) {
	for (int i = maxY - 2; i > 0; i--)
	{
		int count = 0;
		for (int j = minX; j < maxX; j++)
		{
			if (field[i][j]) {
				count++;
			}
		}
		if (count == X) {
			game_points+=X;
			for (int k = i; k > 1; k--) {
				for (int j = minX; j < maxX; j++)
				{
					field[k][j] = field[k - 1][j];

				}
			}
			i--;
		}
	}
}
void create_figure(int& colorNum,int &num) {
	colorNum = rand() % 7 + 1;
	num = rand() % 7;

	for (int i = 0; i < 4; i++) {
		figure[i].x = tetramino[num][i] % 2 + 15;
		figure[i].y = tetramino[num][i] / 2;
	}

}
void Move(int &dx,bool &game) {
	for (int i = 0; i < 4; i++) {
		buf[i] = figure[i];
		figure[i].x += dx;
	}
	if (!check(game)) {
		for (int i = 0; i < 4; i++) {
			figure[i] = buf[i];
		}

	}
	dx = 0;
}
void DownMove(bool& game) {
	for (int i = 0; i < 4; i++) {
		buf[i] = figure[i];
		figure[i].y += 1;
	}
}
void Rotate(bool& game) {
	element centr = figure[1];
	for (int i = 0; i < 4; i++) {
		buf[i] = figure[i];
		int dY = figure[i].y - centr.y; //y-y0
		int dX = figure[i].x - centr.x; //x-x0
		figure[i].x = centr.x - dX;
		figure[i].y = centr.y + dY;
	}
	if (!check(game)) {
		for (int i = 0; i < 4; i++) {
			figure[i] = buf[i];
		}
	}
}
void clearField() {
	for (int i = 0; i < maxY - 1; i++) {
		for (int j = minX; j < maxX; j++) {
			if (field[i][j] = 0) {

			}
		}
	}
}
void delFigure() {
	for (int i = 0; i < 4; i++) {
		figure[i].x =0;
		figure[i].y = 0;
	}
}
void GameisOver(sf::RenderWindow &window) {

	// Текст

	sf::Text gameIsOver;
	sf::Font font;
	if (!font.loadFromFile(way + "font.ttf"))
		return;
	gameIsOver.setFont(font);
	gameIsOver.setCharacterSize(80);
	gameIsOver.setPosition(300, 150);
	gameIsOver.setStyle(sf::Text::Bold);
	gameIsOver.setFillColor(sf::Color::Red);
	gameIsOver.setString("The game is over!");
	window.draw(gameIsOver);
	window.display();
}
void Reset() {

}
int main()
{
	//SetConsoleCP(1251);
	//SetConsoleOutputCP(1251);

	bool game = true;

	int num = 0; // Тип фигуры
	int colorNum = 0; // Цвет фигуры

	int game_points = 0; // Очки

	int dx = 0; // Смещение горизонтальное
	bool rotate = false; // Вращение
	bool move = false; // Движение
	bool begin = true;


	float timer = 0; // Время Таймер
	float delay = 0.3; // Время Задержка

	int count = 0;

	sf::Clock clock;// Таймер

	srand(std::time(0));

	// Задний фон
	sf::Texture backgroundTexture;
	if (!backgroundTexture.loadFromFile(way + "pict.jpg"))
		return -1;
	sf::Sprite background(backgroundTexture);
	float sizeX = backgroundTexture.getSize().x;
	float sizeY = backgroundTexture.getSize().y;
	sf::RenderWindow window(sf::VideoMode(sizeX, sizeY), "Tetris"); // Окно игры

	// Текстура и спрайт для фигурок
	sf::Texture elTexture;
	sf::Sprite el;

	// Текст
	sf::Font font;
	if (!font.loadFromFile(way + "font.ttf"))
		return -1;
	sf::Text text;
	text.setFont(font);
	text.setCharacterSize(80);
	text.setStyle(sf::Text::Bold);
	text.setFillColor(sf::Color(34, 139, 34));
	text.setPosition(930, 10);

	// Музыка
	sf::Music music;
	if (!music.openFromFile(way + "music.wav"))
		return -1; // error
	music.setLoop(true);
	music.play();

	// Мелодия окончания игр
	sf::SoundBuffer buffer;
    sf::Sound overGame;
	if (!buffer.loadFromFile(way+"0.wav"))
		return -1;
	overGame.setBuffer(buffer);
	overGame.setLoop(true);

	// Табло с информацией
	sf::Font fontArrow;
	if (!fontArrow.loadFromFile(way + "arrow.ttf"))
		return -1;
	sf::Text infoText;
	infoText.setFont(font);
	std::string s =
		"Control:\
		\n    - Left\
		\n    - Right\
		\n    - Fast\
        \nTab - Restart";
	infoText.setString(s);
	infoText.setCharacterSize(40);
	infoText.setStyle(sf::Text::Bold);
	infoText.setFillColor(sf::Color(25, 25, 112));
	infoText.setPosition(80, 60);
	sf::Text arrowText;
	arrowText.setFont(fontArrow);
	s =
		"\
		\nB\
		\nA\
		\nD\n";
	arrowText.setString(s);
	arrowText.setCharacterSize(50);
	arrowText.setStyle(sf::Text::Bold);
	arrowText.setFillColor(sf::Color(25, 25, 112));
	arrowText.setPosition(80, 50);

	// Start текст
	sf::Text startText;
	startText.setFont(font);
	startText.setString("START");
	startText.setCharacterSize(100);
	startText.setStyle(sf::Text::Bold);
	startText.setFillColor(sf::Color(199, 21, 133));
	startText.setPosition(470, 200);

	while (window.isOpen()) {

		delay = 0.3;

		sf::Event event;

		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
				game = false;
			}
			if (event.type == sf::Event::KeyPressed) {
				{
					if (event.key.code == sf::Keyboard::Left) dx = -1, move = true;
					else if (event.key.code == sf::Keyboard::Right) dx = 1, move = true;
					else if (event.key.code == sf::Keyboard::Space) rotate = true;
					else if (event.key.code == sf::Keyboard::Down) delay = 0.01;
					else if (event.key.code == sf::Keyboard::Tab) {
						count = 0;
						clearField();
						delFigure();
						colorNum = 0;
						game_points = 0;
						if (!game) {
						
							game = true;
							overGame.stop();
							music.play();
						}
						
					}
				}
			}
		}
	
		if (game) {
			// Первое появление фигуры
			if (begin) {
				create_figure(colorNum, num);
				begin = false;
			}


			float time = clock.getElapsedTime().asSeconds();
			clock.restart();
			timer += time;

			// Горизонтальное перемещение
			if (move) {
				Move(dx, game);
				move = false;
			}

			// Вращение
			if (rotate) {
				Rotate(game);
				rotate = false;
			}

			// Движение вниз
			if (timer > delay) {

				DownMove(game);
				if (!check(game) && game) {
					for (int i = 0; i < 4; i++) field[buf[i].y][buf[i].x] = colorNum;
					create_figure(colorNum, num);
					dx = 0;
					move = false;
					rotate = false;
				}
				timer = 0;
			}

			window.clear();
			window.draw(background);

			// Прорисовка рамки
			sf::RectangleShape rect;
			rect.setSize(sf::Vector2f(X * 40, maxY * 40 - 50));
			rect.setFillColor(sf::Color::Transparent);
			rect.setOutlineThickness(7);
			rect.setPosition(minX * 40, maxY / 40 + 15);
			rect.setOutlineColor(sf::Color::Yellow);
			window.draw(rect);

			// Вывод текста
			text.setString("Points: " + std::to_string(game_points));
			window.draw(text);
			// Вывод табло с информацией
			window.draw(infoText);
			window.draw(arrowText);
			if (count <= 150) {
				window.draw(startText);
				count++;
			}
			// Прорисовка кубиков
			for (int i = 0; i < 4; i++) {
				if (figure[i].x == 0 && figure[i].y == 0) break;
				elTexture.loadFromFile(way + std::to_string(colorNum) + ".png");
				el.setTexture(elTexture);
				el.setScale(0.5, 0.5);
				el.setPosition(figure[i].x * 40, figure[i].y * 40);
				window.draw(el);
			}
			for (int i = 0; i < maxY - 1; i++) {
				for (int j = minX; j < maxX; j++) {
					if (field[i][j] == 0) {
						continue;
					}
					else {
						elTexture.loadFromFile(way + std::to_string(field[i][j]) + ".png");
						el.setTexture(elTexture);
						el.setScale(0.5, 0.5);
						el.setPosition(j * 40, i * 40);
						window.draw(el);
					}

				}

			}

			destruction(game_points); // Проверка на собранный ряд
			window.display();
			if (!game) {
				overGame.play();
				music.stop();
				GameisOver(window);

			}
		}
		

	}
}
	

	
	
