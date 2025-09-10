#include <iostream>
#include <vector>
#include <utility>

#include <chrono>
#include <thread>
#include <conio.h>

struct Tail {
	int y, x;
};

void fill_field(char** field, int center_x = 10, int center_y = 10, size_t rows = 22, size_t cols = 22) {
	for (int i = 0; i < rows; i++) {
		for (int z = 0; z < cols; z++) {
			if (i == 0) field[i][z] = '-';
			else if (i == rows - 1) field[i][z] = '_';
			else if ((z == 0 || z == cols - 1) && (i != 0 || i != rows - 1)) field[i][z] = '|';
			else if (i == center_y && z == center_x) field[i][z] = 'O';    //условный центр
			else field[i][z] = ' ';
		}
	}
}

void print_field(char** arr, size_t rows = 22, size_t cols = 22) {
	for (int i = 0; i < rows; i++) {
		for (int z = 0; z < cols; z++) {
			std::cout << arr[i][z] << " ";
		}
		std::cout << std::endl;
	}
}

inline void fruit(char** arr, int& x, int& y) {
	x = 1 + rand() % 20;
	y = 1 + rand() % 20;
	while (arr[y][x] != ' ') {
		x = 1 + rand() % 20;
		y = 1 + rand() % 20;
	}
}

void update_tail(std::vector<Tail>& vec, int head_x, int head_y) {
	if (!vec.empty()) {
		for (int i = vec.size() - 1; i > 0; i--) {
			vec[i] = vec[i - 1];
		}
		vec[0] = { head_y, head_x };
	}
}


int main() {
	srand(time(NULL));
	const size_t rows = 22, cols = 22;
	char** field = new char* [rows];
	std::vector<Tail> tail;

	for (int i = 0; i < rows; i++) {
		field[i] = new char[cols];
	}
	int size = 0, old_size = 0;

	char next_step, current_direction = 'd';

	int x = 10, y = 10, f_x = 0, f_y = 0;

	bool loss = false, check = false;

	fill_field(field);

	while (size != (rows - 2) * (cols - 2) && loss != true) {

		system("cls");
		std::cout << "Size: " << size << std::endl;
		std::cout << "\t\tW - up\nA - left\tS - down\tD - right" << std::endl;

		if (size > old_size || check == false) {
			if (check != false) old_size++;
			check = true;
			fruit(field, f_x, f_y);
		}
		fill_field(field, x, y);
		field[f_y][f_x] = 36;

		if (!tail.empty()) {
			for (auto segment : tail) {
				field[segment.y][segment.x] = '~';
			}
		}
		print_field(field, rows, cols);

		auto startTime = std::chrono::steady_clock::now();
		auto endTime = startTime + std::chrono::seconds(1);

		while (std::chrono::steady_clock::now() < endTime) {
			if (_kbhit()) {
				char key = _getch();
				key = tolower(key);

				if ((key == 'w' && current_direction != 's') ||
					(key == 'a' && current_direction != 'd') ||
					(key == 's' && current_direction != 'w') ||
					(key == 'd' && current_direction != 'a')) {
					current_direction = key;
				}
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}
		int oldX = x, oldY = y;
		switch (current_direction) {
		case 'w': {
			next_step = field[y - 1][x];
			if (next_step == '$') {
				size++;
				tail.push_back({ y, x });
			}
			if (next_step == '-' || next_step == '~') loss = true;
			y--;
			break;
		}
		case 'a': {
			next_step = field[y][x - 1];
			if (next_step == '$') {
				size++;
				tail.push_back({ y, x });
			}
			if (next_step == '|' || next_step == '~') loss = true;
			x--;
			break;
		}
		case 's': {
			next_step = field[y + 1][x];
			if (next_step == '$') {
				size++;
				tail.push_back({ y, x });
			}
			if (next_step == '_' || next_step == '~') loss = true;
			y++;
			break;
		}
		case 'd': {
			next_step = field[y][x + 1];
			if (next_step == '$') {
				size++;
				tail.push_back({ y, x });
			}
			if (next_step == '|' || next_step == '~') loss = true;
			x++;
			break;
		}
		}
		update_tail(tail, oldX, oldY);
	}
	if (loss == true) {
		system("cls");
		std::cout << "You lose!";
	}
	else std::cout << "Winner!";

	for (int i = 0; i < rows; i++) {
		delete[] field[i];
	}
	delete[] field;
}