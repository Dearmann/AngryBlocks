#include "screen.h"
#include "converter.h"
#include <iostream>
#include <fstream>

void Screen::createMapMenu()
{
	map_background_texture.loadFromFile("textures/background2_sky.png");
	map_background.setSize(sf::Vector2f(screen_width, screen_height));
	map_background.setTexture(&map_background_texture);
	map_menu_window.setFillColor(sf::Color(245, 227, 169));
	map_menu_window.setOutlineColor(sf::Color(241, 195, 131));
	map_menu_window.setOutlineThickness(3);
	map_menu_window.setSize(sf::Vector2f(400, 100));
	map_menu_window.setOrigin(400 / 2, 100 / 2);
	map_menu_window.setPosition(screen_width / 2, screen_height / 2);
	float menu_left_pos = map_menu_window.getGlobalBounds().left;
	float menu_top_pos = map_menu_window.getGlobalBounds().top;
	for (int i = 0; i < 5; i++)
	{
		level_icon[i]->setButtonPosition(menu_left_pos + 25 + 75 * i, menu_top_pos + 25);
	}
	level_icon[0]->allow_to_click = true;
	for (int i = 1; i < 5; i++)
	{
		level_icon[i]->allow_to_click = false;
	}
}
void Screen::createWinLoseWindow()
{
	win_lose_window.setFillColor(sf::Color(245, 227, 169));
	win_lose_window.setOutlineColor(sf::Color(241, 195, 131));
	win_lose_window.setOutlineThickness(3);
	win_lose_window.setSize(sf::Vector2f(400, 220));
	win_lose_window.setOrigin(400 / 2, 220 / 2);
	win_lose_window.setPosition(screen_width / 2, screen_height / 2);
	float ws_left_pos = win_lose_window.getGlobalBounds().left;
	float ws_top_pos = win_lose_window.getGlobalBounds().top;
	float ws_height = win_lose_window.getGlobalBounds().height;
	float ws_width = win_lose_window.getGlobalBounds().width;

	win_restart = new Button(ws_left_pos + 25,
		ws_top_pos + ws_height - 60.f, 160.f, 40.f);
	win_restart->text.setString("Restart");

	lose_restart = new Button(ws_left_pos + 95,
		ws_top_pos + ws_height - 65.f, 200.f, 50.f);
	lose_restart->text.setString("Restart");

	next = new Button(ws_left_pos + ws_width - 185,
		ws_top_pos + ws_height - 60.f, 160.f, 40.f);
	next->text.setString("Next");
	next->text.setPosition(
		next->rectangle_button.getPosition().x + next->size_x / 4.7,
		next->rectangle_button.getPosition().y - next->size_y / 6
	);

	win_lose_score_text.setFillColor(sf::Color(255, 255, 255));
	win_lose_score_text.setPosition(ws_left_pos + 75, ws_top_pos + 60);
	win_lose_score_text.setCharacterSize(40);
	win_lose_score_text.setFont(font);

	win_text.setFillColor(sf::Color(255, 255, 255));
	win_text.setPosition(ws_left_pos + 4, ws_top_pos);
	win_text.setCharacterSize(50);
	win_text.setFont(font);
	win_text.setString("Level completed!");

	lose_text.setFillColor(sf::Color(255, 255, 255));
	lose_text.setPosition(ws_left_pos + 58, ws_top_pos);
	lose_text.setCharacterSize(50);
	lose_text.setFont(font);
	lose_text.setString("Level failed!");

	highscore_text[0].setFillColor(sf::Color(255, 255, 255));
	highscore_text[0].setPosition(ws_left_pos + 75, ws_top_pos + 110);
	highscore_text[0].setCharacterSize(30);
	highscore_text[0].setFont(font);
}

void Screen::saveLevelLayout()
{
	std::ofstream level_layout_file("levels/level_layout", std::ios::binary);
	if (level_layout_file.is_open())
		level_layout_file.write((char*)level_design, sizeof(level_design));
	else
		std::cout << "Unable to save binary file" << std::endl;
	level_layout_file.close();
}

void Screen::unlockNextLevels()
{
	for (int i = 0; i < 4; i++)
	{
		if (scores[i] > 0)
			level_icon[i + 1]->allow_to_click = true;
	}
}

void Screen::setActiveLevel(Level* active_level)
{
	this->active_level = active_level;
	this->active_level_objects = active_level->getObjects();
}
void Screen::loadChosenLevel(int level_number)
{
	Level* level_to_delete = active_level;
	if (level_number + 1 <= 3)
		active_level = new Level(level_number + 1, level_number + 1, GroundType::Normal);
	else
		active_level = new Level(level_number + 1, level_number + 1, GroundType::Moon);
	active_level->makeLevel();
	interpretLevelLayout(level_number);
	setActiveLevel(active_level);
	for (int i = 0; i < 3; i++)
	{
		lives_img[i].setTexture(&active_level->getThrowableByNumber(i)->texture);
	}
	delete level_to_delete;
}

void Screen::renderActiveThrowable()
{
	window.draw(active_level->getActiveThrowable()->graphics);
}
void Screen::renderTrajectory()
{
	float position_x, position_y;
	for (int i = 0; i <= 17; i++)
	{
		position_x = active_level->getActiveThrowable()->getTrajectoryX(mouse_position.x, i * 2);
		position_y = active_level->getActiveThrowable()->getTrajectoryY(mouse_position.y, i * 2);
		trajectory_point[i].setPosition(position_x, position_y);
		window.draw(trajectory_point[i]);
	}
}
void Screen::updateObjects()
{
	b2World* active_world = active_level->getWorldPointer();
	active_world->Step(1 / 60.f, 8, 3);
	for (int i = 0; i < active_level_objects->size(); i++)
	{
		if ((*active_level_objects)[i]->physics != nullptr)
		{
			(*active_level_objects)[i]->updatePosition();
			window.draw((*active_level_objects)[i]->graphics);
		}
	}
}
void Screen::showLives()
{
	if (active_level->getThrowableByNumber(0)->getThrowableState() != ThrowableState::Used)
	{
		lives_text[0].setFillColor(sf::Color(255, 255, 255, 230));
		lives_img[0].setFillColor(sf::Color(255, 255, 255, 230));
	}
	else
	{
		lives_text[0].setFillColor(sf::Color(255, 255, 255, 100));
		lives_img[0].setFillColor(sf::Color(255, 255, 255, 100));
	}
	window.draw(lives_text[0]);
	window.draw(lives_img[0]);
	if (active_level->getThrowableByNumber(1)->getThrowableState() != ThrowableState::Used)
	{
		lives_text[1].setFillColor(sf::Color(255, 255, 255, 230));
		lives_img[1].setFillColor(sf::Color(255, 255, 255, 230));
	}
	else
	{
		lives_text[1].setFillColor(sf::Color(255, 255, 255, 100));
		lives_img[1].setFillColor(sf::Color(255, 255, 255, 100));
	}
	window.draw(lives_text[1]);
	window.draw(lives_img[1]);
	if (active_level->getThrowableByNumber(2)->getThrowableState() != ThrowableState::Used)
	{
		lives_text[2].setFillColor(sf::Color(255, 255, 255, 230));
		lives_img[2].setFillColor(sf::Color(255, 255, 255, 230));
	}
	else
	{
		lives_text[2].setFillColor(sf::Color(255, 255, 255, 100));
		lives_img[2].setFillColor(sf::Color(255, 255, 255, 100));
	}
	window.draw(lives_text[2]);
	window.draw(lives_img[2]);
}

void Screen::handleLevelEvents()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
	{
		loadChosenLevel(active_level->getLevelNumber() - 1);
		game_state = GameState::Level;
	}
	if (game_state == GameState::Level)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1))
		{
			active_level->setActiveThrowable(0);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2))
		{
			active_level->setActiveThrowable(1);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3))
		{
			active_level->setActiveThrowable(2);
		}
		if (active_level->getActiveThrowable() != nullptr)
		{
			if (event.type == sf::Event::MouseButtonPressed &&
				active_level->getActiveThrowable()->getThrowableState() != ThrowableState::Used &&
				!map_button->isHovered(mouse_position))
			{
				active_level->getActiveThrowable()->setThrowableState(ThrowableState::Aiming);
			}
			if (event.type == sf::Event::MouseButtonReleased &&
				active_level->getActiveThrowable()->getThrowableState() == ThrowableState::Aiming)
			{
				active_level->getActiveThrowable()->launch(mouse_position.x, mouse_position.y);
				active_level->throwableUsed();
			}
			if (event.type == sf::Event::MouseButtonPressed &&
				active_level->getActiveThrowable()->throwable_type == ThrowableType::Bombarding &&
				active_level->getActiveThrowable()->getThrowableState() == ThrowableState::Used)
			{
				active_level->createBombardingAdds();
			}
		}
	}
	else if (game_state == GameState::Lose)
	{
		if (lose_restart->isClicked(window, event))
		{
			loadChosenLevel(active_level->getLevelNumber() - 1);
			game_state = GameState::Level;
		}
	}
	else if (game_state == GameState::Win)
	{
		if (win_restart->isClicked(window, event))
		{
			loadChosenLevel(active_level->getLevelNumber() - 1);
			game_state = GameState::Level;
		}
		if (next->isClicked(window, event))
		{
			if (active_level->getLevelNumber() >= 5)
				loadChosenLevel(active_level->getLevelNumber() - 1);
			else
				loadChosenLevel(active_level->getLevelNumber());
			game_state = GameState::Level;
		}
	}
}
void Screen::handleMapMenuEvents()
{
	for (int i = 0; i < 5; i++)
	{
		if (level_icon[i]->isClicked(window, event))
		{
			loadChosenLevel(i);
			game_state = GameState::Level;
		}
	}
}

void Screen::createLayoutObject(ObstacleType obstacle_type, int k, int j, int number_of_repetition)
{
	float beginning = 35.f / 2;
	active_level->createObstacle(obstacle_type,
		beginning + 35.f * k - 35.f / 2 * number_of_repetition,
		beginning + 35.f * j,
		35.f + 35.f * number_of_repetition);
}
void Screen::createLayoutObject(ObjectType object_type, int k, int j)
{
	float beginning = 35.f / 2;
	if (object_type == ObjectType::Enemy)
	{
		active_level->createEnemy(beginning + 35.f * k, beginning + 35.f * j);
	}
	else if (object_type == ObjectType::Ground)
	{
		active_level->createGround(active_level->getLevelType(),
			beginning + 35.f * k,
			beginning + 35.f * j);
	}
}
void Screen::interpretLevelLayout(int level_number)
{
	//float multiplier_x = screen_width / 35.f - 1;
	//float multiplier_y = screen_height / 35.f - 2;
	char block_type;
	char next_block_type;
	int number_of_repetition;
	for (int j = 0; j < 22; j++)
	{
		number_of_repetition = 0;
		for (int k = 0; k < 40; k++)
		{
			block_type = level_design[level_number][j][k];
			next_block_type = level_design[level_number][j][k + 1];
			switch (block_type)
			{
			case 'w':
				if (block_type == next_block_type)
				{
					number_of_repetition++;
					break;
				}
				createLayoutObject(ObstacleType::Wood, k, j, number_of_repetition);
				number_of_repetition = 0;
				break;
			case 'W':
				if (block_type == next_block_type)
				{
					number_of_repetition++;
					break;
				}
				createLayoutObject(ObstacleType::Wood, k, j, number_of_repetition);
				number_of_repetition = 0;
				break;
			case 's':
				if (block_type == next_block_type)
				{
					number_of_repetition++;
					break;
				}
				createLayoutObject(ObstacleType::Stone, k, j, number_of_repetition);
				number_of_repetition = 0;
				break;
			case 'S':
				if (block_type == next_block_type)
				{
					number_of_repetition++;
					break;
				}
				createLayoutObject(ObstacleType::Stone, k, j, number_of_repetition);
				number_of_repetition = 0;
				break;
			case 'm':
				if (block_type == next_block_type)
				{
					number_of_repetition++;
					break;
				}
				createLayoutObject(ObstacleType::Metal, k, j, number_of_repetition);
				number_of_repetition = 0;
				break;
			case 'M':
				if (block_type == next_block_type)
				{
					number_of_repetition++;
					break;
				}
				createLayoutObject(ObstacleType::Metal, k, j, number_of_repetition);
				number_of_repetition = 0;
				break;
			case 'e':
				createLayoutObject(ObjectType::Enemy, k, j);
				break;
			case 'g':
				createLayoutObject(ObjectType::Ground, k, j);
				break;
			default:
				break;
			}
		}
	}
}

Screen::Screen(int screen_width, int screen_height)
{
	this->screen_width = screen_width;
	this->screen_height = screen_height;
	font.loadFromFile("external/OriginalSurfer-Regular.ttf");
	for (int i = 0; i <= 17; i++)
	{
		trajectory_point[i].setFillColor(sf::Color(100, 255, 100, 180));
		trajectory_point[i].setRadius(2);
	}
	map_button = new Button("textures/maps_p.png", "textures/maps_a.png",
		0.75, 0.75);
	map_button->setPosition(screen_width - map_button->getGlobalBounds().width - 10, 10);
	for (int i = 0; i < 5; i++)
	{
		level_icon[i] = new Button(25 + 75 * i, 25, 25.f);
		level_icon[i]->text.setString(std::to_string(i + 1));
	}
	score_text.setFillColor(sf::Color::White);
	score_text.setPosition(0, 0);
	score_text.setCharacterSize(35);
	score_text.setFont(font);
	for (int i = 0; i < 3; i++)
	{
		lives_text[i].setFillColor(sf::Color(255, 255, 255, 128));
		lives_text[i].setPosition(0, 65 + 30 * i);
		lives_text[i].setCharacterSize(30);
		lives_text[i].setFont(font);
		std::string text = std::to_string(i + 1) + ".";
		lives_text[i].setString(text);
	}
	lives_text[0].setPosition(6, 65);
	for (int i = 0; i < 3; i++)
	{
		lives_img[i].setSize(sf::Vector2f(30, 30));
		lives_img[i].setPosition(30, 69 + 30 * i);
		lives_img[i].setFillColor(sf::Color(255, 255, 255, 128));
	}
	highscore_text[1].setFillColor(sf::Color(255, 255, 255));
	highscore_text[1].setPosition(0, 38);
	highscore_text[1].setCharacterSize(22);
	highscore_text[1].setFont(font);
	createMapMenu();
	createWinLoseWindow();
}
Screen::~Screen()
{
	delete active_level;
	active_level = nullptr;
	for (int i = 0; i < 5; i++)
	{
		delete level_icon[i];
		level_icon[i] = nullptr;
	}
	delete win_restart;
	delete lose_restart;
	delete next;
	delete map_button;
}

void Screen::loadScore(SaveType save_type)
{
	if (save_type == SaveType::Text)
	{
		std::string line;
		std::ifstream save_file("levels/save.txt");
		if (save_file.is_open())
		{
			for (int i = 0; i < 5; i++)
			{
				save_file >> line;
				scores[i] = std::stoi(line);
			}
		}
		else
			std::cout << "Unable to open text file" << std::endl;
		save_file.close();
	}
	else if (save_type == SaveType::Binary)
	{
		std::ifstream save_file("levels/save", std::ios::binary);
		if (save_file.is_open())
		{
			save_file.read((char*)scores, sizeof(scores));
		}
		else
			std::cout << "Unable to open binary file" << std::endl;
		save_file.close();
	}
}
void Screen::loadLevelsLayout(SaveType save_type)
{
	if (save_type == SaveType::Text)
	{
		std::ifstream level_layout_file("levels/level_layout.txt");
		std::string line;
		if (level_layout_file.is_open())
		{
			for (int i = 0; i < 5; i++)
			{
				level_layout_file >> line; // This line consists of level number
				for (int j = 0; j < 22; j++)
				{
					level_layout_file >> line;
					if (/*line.length() == 40 || */line.length() == 41)
						for (int k = 0; k < 41; k++)
							level_design[i][j][k] = line[k];
					else
					{
						std::cout << "Line is not 41 chars!" << std::endl;
						return;
					}
				}
			}
		}
		else
			std::cout << "Unable to open text file" << std::endl;
		level_layout_file.close();
	}
	else if (save_type == SaveType::Binary)
	{
		std::ifstream level_layout_file("levels/level_layout", std::ios::binary);
		if (level_layout_file.is_open())
		{
			level_layout_file.read((char*)level_design, sizeof(level_design));
		}
		else
			std::cout << "Unable to open binary file" << std::endl;
		level_layout_file.close();
	}
}
void Screen::saveScore(SaveType save_type)
{
	if (save_type == SaveType::Text)
	{
		std::ofstream save_file("levels/save.txt");
		if (save_file.is_open())
		{
			for (int i = 0; i < 5; i++)
			{
				save_file << std::to_string(scores[i]) << std::endl;
			}
		}
		else
			std::cout << "Unable to save text file" << std::endl;
		save_file.close();
	}
	else if (save_type == SaveType::Binary)
	{
		std::ofstream save_file("levels/save", std::ios::binary);
		if (save_file.is_open())
		{
			save_file.write((char*)scores, sizeof(scores));
		}
		else
			std::cout << "Unable to save binary file" << std::endl;
		save_file.close();
	}
}

void Screen::createWindow()
{
	unlockNextLevels();
	window.create(sf::VideoMode(screen_width, screen_height, 32), "AngryBlocks", sf::Style::Close);
	window.setFramerateLimit(60);
	float ws_left_pos = win_lose_window.getGlobalBounds().left;
	float ws_top_pos = win_lose_window.getGlobalBounds().top;
	std::string score_string;
	while (window.isOpen())
	{
		mouse_position = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));
		if (game_state == GameState::Level ||
			game_state == GameState::Win ||
			game_state == GameState::Lose)
		{
			window.draw(active_level->getBackgroud());
			if (active_level->getActiveThrowable() != nullptr)
			{
				if (active_level->getActiveThrowable()->getThrowableState() != ThrowableState::Used)
					renderActiveThrowable();
				if (active_level->getActiveThrowable()->getThrowableState() == ThrowableState::Aiming)
					renderTrajectory();
				active_level->checkThrowableMovement();
				active_level->createClusterAdds();
			}
			updateObjects();
			active_level->destroyFlaggedObjects();
			score_string = "Score: " + std::to_string(active_level->getScore());
			if (game_state == GameState::Level && active_level->isLevelLost())
			{
				game_state = GameState::Lose;
			}
			if ((game_state == GameState::Level || game_state == GameState::Lose)
				&& active_level->isLevelWon())
			{
				game_state = GameState::Win;
				active_level->addPointsForUnusedThrows();
				score_string = "Score: " + std::to_string(active_level->getScore());
				if (active_level->getLevelNumber() < 5)
					next->allow_to_click = true;
				else
					next->allow_to_click = false;
			}
			score_text.setString(score_string);
			win_lose_score_text.setString(score_string);
			highscore_text[0].setString("Highscore: "
				+ std::to_string(scores[active_level->getLevelNumber() - 1]));
			highscore_text[1].setString("Highscore: "
				+ std::to_string(scores[active_level->getLevelNumber() - 1]));
			window.draw(score_text);
			window.draw(highscore_text[1]);
			showLives();
			// Fix for score positioning depending on 3 or 4 digit score
			if (active_level->getScore() >= 1000)
				win_lose_score_text.setPosition(ws_left_pos + 75, ws_top_pos + 60);
			else
				win_lose_score_text.setPosition(ws_left_pos + 87, ws_top_pos + 60);
			if (game_state == GameState::Win || game_state == GameState::Lose)
			{
				window.draw(win_lose_window);
				if (game_state == GameState::Win)
				{
					// Save score if new highscore has been set
					if (active_level->getScore() > scores[active_level->getLevelNumber() - 1])
						scores[active_level->getLevelNumber() - 1] = active_level->getScore();
					unlockNextLevels();
					window.draw(win_text);
					window.draw(win_lose_score_text);
					window.draw(highscore_text[0]);
					win_restart->drawTo(window);
					next->drawTo(window);
				}
				else if (game_state == GameState::Lose)
				{
					window.draw(lose_text);
					window.draw(win_lose_score_text);
					window.draw(highscore_text[0]);
					lose_restart->drawTo(window);
				}
			}
		}
		// Map menu
		else if (game_state == GameState::MapMenu)
		{
			if (active_level == nullptr)
				window.draw(map_background);
			else
			{
				window.draw(active_level->getBackgroud());
			}
			window.draw(map_menu_window);
			for (int i = 0; i < 5; i++)
			{
				level_icon[i]->drawTo(window);
			}
		}
		if (active_level != nullptr)
			window.draw(*map_button);
		while (window.pollEvent(event))
		{
			if (game_state == GameState::Level ||
				game_state == GameState::Win ||
				game_state == GameState::Lose)
			{
				handleLevelEvents();
			}
			else if (game_state == GameState::MapMenu)
			{
				handleMapMenuEvents();
			}
			if (active_level != nullptr
				&& (map_button->isClicked(window, event)
					|| event.type == sf::Event::KeyPressed && sf::Keyboard::isKeyPressed(sf::Keyboard::M)))
			{
				if (game_state != GameState::MapMenu)
				{
					previous_game_state = game_state;
					game_state = GameState::MapMenu;
				}
				else
				{
					game_state = previous_game_state;
				}
			}
			if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
				window.close();
			if (event.type == sf::Event::KeyPressed && sf::Keyboard::isKeyPressed(sf::Keyboard::F5))
				saveLevelLayout();
		}
		window.display();
	}
}