//*** игра крестики нолики ***//
#include <iostream>
#include <conio.h>
#include <time.h>


#define pl_len 32
#define ai_size 3

bool output(char **matrix);
bool input(char **matrix, char token, char(&pl)[pl_len]);
bool processing(char **matrix);

int player_f = 0;//флаг выбора игрока
bool first_f = false;//флаг, по которому печатаются данные, которые мы ввели и кто первый походил (нужно это после очистки экрана и перезагрузки ф-и: processing_fm)
bool error_f = false;//флаг, который регистрирует ошибку ввода
bool win_f = false;//флаг, который регистрирует победу
bool exit_f = false;//флаг, который регистрирует, что игра завершилась
char first[pl_len] = "";//массив, где записываем имя, кто первый начал игру
int array_input[ai_size * 3] = { 0,0,0,0,0,0,0,0,0 }, ai_count = 0;

//ф-я обработки ввода данных
bool processing_fm(char *pl1, char *pl2)
{
	srand(time(NULL));
	std::cout << "\t*** игра крестики нолики ***\n" << std::endl;
	std::cout << "Введите имя первого игрока: ";
	if (!first_f)
		std::cin.getline(pl1, pl_len); else
		std::cout << pl1 << "\n";
	std::cout << "Введите имя второго игрока: ";
	if (!first_f)
		std::cin.getline(pl2, pl_len); else
		std::cout << pl2 << "\n";
	std::cout << "Игру начал: ";
	if (first_f != 0) {
		std::cout << first << std::endl;
	}
	else {
		if (rand() & 1)
		{
			player_f = 1;
			std::cout << pl1 << std::endl;
			strncpy_s(first, pl1, pl_len);
		}
		else
		{
			player_f = 2;
			std::cout << pl2 << std::endl;
			strncpy_s(first, pl2, pl_len);
		}
	}
	return true;
}

typedef bool(*START)(char*, char*);

int first_move(char(&pl1)[pl_len], char(&pl2)[pl_len], START pl)
{
	return pl(pl1, pl2);
}

//главная ф-я, где присутствует контроль за всем :)
int main()
{
	setlocale(0, "");
	int i = 0, j = 0, count = 0x30;
	char player1[pl_len] = "";//объявляем массив для первого игрока
	char player2[pl_len] = "";//объявляем массив для второго игрока
							  //создаем матрицу 3x3
	char **matrix = new char*[ai_size];
	for (int i(0); i<ai_size; ++i)
		matrix[i] = new char[ai_size];
	first_f = first_move(player1, player2, processing_fm);//вводим данные и определяем случайным способом, чей первый ход
														  //заполняем матрицу от 1 до 9
	do {
		do {
			matrix[i][j] = ++count;
		} while (++j<ai_size);
		j = 0;
	} while (++i<ai_size);
	i = 0, j = 0;
	while (1) {//запускаем бесконечный цикл
		system("cls");//очищаем консоль
		first_move(player1, player2, processing_fm);//печатаем информацию, которую мы ввели
		output(matrix);//запускаем ф-ю вывода матрицы в консоль
					   //std::cout<<"flag win:"<<win_f<<"\n";//тестирующий флаг победы
		if ((ai_count<ai_size * 3) || win_f) {//проверяем заполнены ли все 9 ячеек, или есть победа.
			if (win_f)//если победа
				std::cout << "Победил: ";
			//проверяем чей ход
			if (player_f == 1)
			{
				//когда нет победы и играет первый игрок, то записываем в матрицу "X", иначе печатаем, что победитель - второй игрок
				if (!win_f)
				{
					error_f = input(matrix, 'X', player1);
				}//запускаем ф-ю ввода ячейки матрицы "X", результатом будет правильность ввода
				else {
					std::cout << player2 << std::endl;
					exit_f = 1;
				}
			}
			else if (player_f == 2)
			{
				//когда нет победы и играет второй игрок, то записываем в матрицу "0", иначе печатаем, что победитель - первый игрок
				if (!win_f)
				{
					error_f = input(matrix, '0', player2);
				}//запускаем ф-ю ввода ячейки матрицы "0", результатом будет правильность ввода
				else {
					std::cout << player1 << std::endl;
					exit_f = 1;
				}
			}
			//когда мы не правильно ввели то чистим экран и выводим сообщение об ошибке ввода, срабатывает, когда нет окончание игры
			if (!exit_f) {
				if (error_f) {
					system("cls");
					std::cout << "Ошибка ввода!\n";
					error_f = false;
					_getch();//останавливаем консоль

				}
				else//иначе запускаем ф-ю проверки победы
					win_f = processing(matrix);
			}
		}
		else {//когда заполнено 9 ячеек и нет победы, то печатаем ничью
			std::cout << "Ничья!" << std::endl;
			exit_f = 1;
		}

		if (exit_f) {
			std::cout << "\n";
			std::cout << "Для выхода из игры нажмите любую клавишу...\n";
			_getch();//останавливаем консоль
			break;//выходим из цикла
		}
	}
	//очищаем память
	for (int i(0); i<ai_size; ++i)
		delete[] matrix[i];
	delete[] matrix;
	return 0;
}

//ф-я ввода "X" и "0"
bool input(char **matrix, char token, char(&pl)[pl_len])
{
	int value;
	char ch_value[256] = "";
	std::cout << "Ходит " << pl << ": ";
	std::cin >> ch_value;
	value = atoi(ch_value);
	for (int i = 0; i<ai_count; i++)
		if (value == array_input[i])
			return true;
	array_input[ai_count++] = value;
	value += 0x30;
	if (!(value>0x30 && value <= 0x39)) {
		array_input[ai_count] = 0;
		ai_count = ai_count - 1;
		return true;
	}
	int i, j;
	for (i = 0; i<ai_size; ++i, j = 0)
		for (j = 0; j<ai_size; ++j)
			if (matrix[i][j] == value)
				matrix[i][j] = token;

	if (player_f == 1) { player_f = 2; }
	else
		if (player_f == 2) { player_f = 1; }
	return false;
}

//ф-я вывода матрицы
bool output(char **matrix)
{
	int i = 0;
	int j = 0;
	std::cout << std::endl;
	for (;i<ai_size;) {
		for (;j<ai_size;) {
			std::cout << "\t" << matrix[i][j];
			j++;
		}
		i++; j = 0;
		std::cout << std::endl << std::endl;
	}
	return false;
}

//ф-я обработки выигрышных комбинаций "X" и "0"
bool processing(char **matrix)
{
	for (int i = 0, j = 0; i<ai_size; i++)
		if ((matrix[i][j] == matrix[i][j + 1]) && (matrix[i][j + 1] == matrix[i][j + 2]))
			return true;
	for (int i = 0, j = 0; j<ai_size; j++)
		if ((matrix[i][j] == matrix[i + 1][j]) && (matrix[i + 1][j] == matrix[i + 2][j]))
			return true;
	int i = 0, j = 0;
	if ((matrix[i][j] == matrix[i + 1][j + 1]) && (matrix[i + 1][j + 1] == matrix[i + 2][j + 2]) ||
		(matrix[i][j + 2] == matrix[i + 1][j + 1]) && (matrix[i + 1][j + 1] == matrix[i + 2][j]))
		return true;
	return false;
}
