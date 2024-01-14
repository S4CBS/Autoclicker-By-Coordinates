#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <Windows.h>
#include <thread>
#include <conio.h>


using namespace std;

struct Coordinate {
    int x, y;
};

// Флаг для хранения состояния программы
bool isRunning = false;

// Функция для разбора строки и извлечения координат
Coordinate parseCoordinates(const string& line) {
    Coordinate result;
    char delimiter;
    istringstream stream(line);
    stream >> result.x >> delimiter >> result.y;
    return result;
}

// Функция для чтения координат из файла
vector<Coordinate> readCoordinatesFromFile(const string& filename) {
    vector<Coordinate> coordinates;
    ifstream file(filename);

    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            coordinates.push_back(parseCoordinates(line));
        }
        file.close();
    }
    else {
        cerr << "Unable to open file: " << filename << endl;
    }

    return coordinates;
}

// Функция для выполнения клика мышью по заданным координатам
void clickMouse(int x, int y) {
    SetCursorPos(x, y);
    mouse_event(MOUSEEVENTF_LEFTDOWN, x, y, 0, 0);
    mouse_event(MOUSEEVENTF_LEFTUP, x, y, 0, 0);
}

// Функция для обработки нажатия клавиш
void handleKeyPress() {
    while (true) {
        if ((GetAsyncKeyState('O') & 0x8000) || (GetAsyncKeyState(0xCE) & 0x8000)) {
            system("cls");
            isRunning = !isRunning;
            if (isRunning) {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN);
                cout << "Program started. Press 'O' to toggle." << endl;
            }
            else {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
                cout << "Program stopped. Press 'O' to toggle." << endl;
            }
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            // Ждем, чтобы избежать множественных переключений при удерживании клавиши
            this_thread::sleep_for(chrono::milliseconds(1000));
        }
        else if (GetAsyncKeyState(VK_INSERT) & 0x8000) {  // Клавиша Insert
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
            cout << "Program closed by user, wait 5 seconds to exit." << endl;
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            this_thread::sleep_for(chrono::seconds(5));
            exit(0);
        }
        this_thread::sleep_for(chrono::milliseconds(100));
    }
}

int main() {
    // Устанавливаем иконку для консольного окна
    HANDLE hIcon = LoadImage(nullptr, L"likee.ico", IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE);
    if (hIcon != nullptr) {
        SendMessage(GetConsoleWindow(), WM_SETICON, ICON_SMALL, reinterpret_cast<LPARAM>(hIcon));
        SendMessage(GetConsoleWindow(), WM_SETICON, ICON_BIG, reinterpret_cast<LPARAM>(hIcon));
    }

    char answer;
    cout << "Do you want to set marks for the autoclicker? (y/n): ";
    cin >> answer;

    if (answer == 'y' || answer == 'Y') {
        system("cls");
        // Запуск get_cords.exe
        system("get_cords.exe");
        system("cls");
    }
    else if (answer == 'n' || answer == 'N') {
        const string filename = "recorded_coordinates.cfg";
        ifstream configFile(filename);

        if (!configFile.is_open()) {
            cerr << "File " << filename << " not found. Countdown initiated." << endl;

            for (int i = 10; i >= 0; --i) {
                cout << "Countdown: " << i << endl;
                this_thread::sleep_for(chrono::seconds(1));
            }

            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
            cout << "Program closed. File " << filename << " not found." << endl;
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

            return 0;  // Завершение программы после отсчета
        }
        system("cls");
    }

    vector<Coordinate> coordinates = readCoordinatesFromFile("recorded_coordinates.cfg");

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN);
    cout << "Enter the interval between cursor movements and clicks (in seconds): ";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

    int intervalSeconds;
    cin >> intervalSeconds;
    system("cls");

    // Выводим подсказку после указания интервала
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN);
    cout << "Use 'O' (English) or 'O' (Russian) to enable and disable the script. Use 'Insert' to close the program." << endl;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

    // Запустить поток для обработки нажатий клавиш
    thread keyPressThread(handleKeyPress);

    while (true) {
        if (isRunning) {
            for (const auto& coord : coordinates) {
                if (!isRunning) {
                    break;
                }
                clickMouse(coord.x, coord.y);
                this_thread::sleep_for(chrono::seconds(intervalSeconds));  // Подождать указанный интервал перед следующим кликом
            }
        }
        else {
            // Если программа не выполняется, подождать 1 секунду
            this_thread::sleep_for(chrono::seconds(1));
        }
    }

    // Ожидание завершения потока
    keyPressThread.join();

    return 0;
}
