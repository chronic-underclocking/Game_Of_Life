// A few sections must be commented or uncommented if compiling on Visual Studio
// There are tips above these sections
// Codeblocks with default GCC compiler is recommended for better performance

// For codeblocks' default GCC compiler, comment this if compiling on Visual Studio
//#define _WIN32_WINNT 0x0500

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <windows.h>
#include <cstdio>
#include <vector>
#include <fstream>
#include <string>
#include <conio.h>

using namespace std;

#define random ((rand() % 100)+1)
typedef vector<vector<bool>> world;

world board;
bool bckgrnd;
int delay;
int color;
bool rainbow;
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
HWND console = GetConsoleWindow();
RECT r;

// For codeblocks' default GCC compiler, comment this entire section if compiling on Visual Studio
// Section starts here
/*typedef struct _CONSOLE_FONT_INFOEX
{
    ULONG cbSize;
    DWORD nFont;
    COORD dwFontSize;
    UINT  FontFamily;
    UINT  FontWeight;
    WCHAR FaceName[LF_FACESIZE];
}CONSOLE_FONT_INFOEX, *PCONSOLE_FONT_INFOEX;

#ifdef __cplusplus
extern "C" {
#endif
BOOL WINAPI SetCurrentConsoleFontEx(HANDLE hConsoleOutput, BOOL bMaximumWindow, PCONSOLE_FONT_INFOEX
lpConsoleCurrentFontEx);
#ifdef __cplusplus
}
#endif
*/
// Section ends here

CONSOLE_FONT_INFOEX cfi;

void setCursorPosition(int x, int y)
{
    std::cout.flush();
    COORD coord = { (SHORT) x, (SHORT) y};
    SetConsoleCursorPosition(hConsole, coord);
}

void print(world w)
{
	if (!bckgrnd)
	{
		for (auto row : w)
		{
			for (auto cell : row)
			{
				if (cell == 1) cout << u8"\u25A0";
				else cout << " ";
			}
			cout << endl;
		}
	}

	else
	{
		for (auto row : w)
		{
			for (auto cell : row)
			{
				if (cell == 1) cout << u8"\u25A0";
				else cout << u8"\u25A1";
			}
			cout << endl;
		}
	}
}

void print(world w, world p)
{
	int i = 0;
	int j = 0;
	if (!bckgrnd)
	{
		for (auto row : w)
		{
			for (auto cell : row)
			{
				if(cell == p[i][j++]) continue;
				setCursorPosition(j, i);
				if (cell == 1) cout << u8"\u25A0";
				else cout << " ";
				setCursorPosition(0, 0);
			}
			cout << endl;
			i++;
			j = 0;
		}
	}

	else
	{
		for (auto row : w)
		{
			for (auto cell : row)
			{
				if(cell == p[i][j++]) continue;
				setCursorPosition(j, i);
				if (cell == 1) cout << u8"\u25A0";
				else cout << u8"\u25A1";
				setCursorPosition(0, 0);
			}
			cout << endl;
			i++;
			j = 0;
		}
	}
}

world new_world(int height, int width)
{
	return world(height, vector<bool>(width));
}

world random_state(int height, int width, int chance)
{
	world state = new_world(height, width);

	for (int i = 1; i < height-1; i++)
	{
		for (int j = 1; j < width-1; j++)
		{
			if (random >= (100-chance)) state[i][j] = 1;
		}
	}

	return state;
}

world next_state(world curr)
{
	int height = curr.size();
	int width = curr[0].size();

	world next = new_world(height, width);

	int live_cnt;

	// Apply rules of life
	for (int i = 1; i < height - 1; i++)
	{
		for (int j = 1; j < width - 1; j++)
		{
			live_cnt = curr[i - 1][j - 1] + curr[i - 1][j] + curr[i - 1][j + 1]
				+ curr[i][j - 1] + curr[i][j + 1] + curr[i + 1][j - 1]
				+ curr[i + 1][j] + curr[i + 1][j + 1];
			if (live_cnt == 2 && curr[i][j] == 1) next[i][j] = 1;
			else if (live_cnt == 3) next[i][j] = 1;
		}
	}

	return next;
}

// Load pattern from file
world load_from_file(ifstream& file)
{
	world state;
	string line;
	int i = 0;
	while (getline(file, line))
	{
		state.push_back(vector<bool>());
		for (auto x : line)
		{
			state[i].push_back((x - '0'));
		}
		i++;
	}
	return state;
}

// Save pattern to file
void save_to_file(ofstream& file)
{
	for (auto row : board)
	{
		for (auto cell : row)
		{
			file << cell;
		}
		file << endl;
	}
}

void save_settings()
{
	ofstream file("settings.txt");
	file << bckgrnd << endl << color << endl << delay;
	file.close();
}

void load_settings()
{
	ifstream file("settings.txt");
	file >> bckgrnd >> color >> delay;
	SetConsoleTextAttribute(hConsole, color);
	file.close();
}

void background_type();
void change_color();
void change_speed();

void settings()
{
	system("cls");
	cout << " 1- Change background type\n"
		 << " 2- Change color\n"
		 << " 3- Change speed\n"
		 << " 4- Return\n\n"
		 << " Enter choice:  ";
	char choice;
	cin >> choice;
	switch (choice)
	{
		case '1':
		    	cin.clear();
			background_type();
			break;
		case '2':
		    	cin.clear();
			change_color();
			break;
		case '3':
		    	cin.clear();
			change_speed();
			break;
		case '4':
		    	cin.clear();
			break;
		default:
		    	cin.clear();
			break;
	}
}

void background_type()
{
	system("cls");
	cout << " 1- No background\n"
		 << " 2- Grid\n\n"
		 << " Enter your choice:  ";
	char choice;
	cin >> choice;
	switch(choice)
	{
		case '1':
			cin.clear();
			bckgrnd = false;
            		break;
		case '2':
		    	cin.clear();
		    	bckgrnd = true;
		    	break;
		default:
		    	cin.clear();
		    	break;
	}
	save_settings();
	settings();
}

void change_color()
{
	system("cls");
	cout << " 1- Red\n"
		 << " 2- Purple\n"
		 << " 3- Green\n"
		 << " 4- Grey\n"
		 << " 5- Light blue\n"
		 << " 6- White\n"
		 << " 7- Pink\n"
		 << " 8- Yellow\n"
		 << " 9- Dark blue\n\n"
		 << " Enter your choice:  ";
	char choice;
	cin >> choice;
	switch (choice)
	{
		case '1':
		    cin.clear();
			color = 4;
			break;
		case '2':
		    cin.clear();
			color = 5;
			break;
		case '3':
		    cin.clear();
			color = 2;
			break;
		case '4':
		    cin.clear();
			color = 8;
			break;
		case '5':
			cin.clear();
			color = 3;
			break;
		case '6':
			cin.clear();
			color = 7;
			break;
		case '7':
			cin.clear();
			color = 13;
			break;
		case '8':
			cin.clear();
			color = 6;
			break;
		case '9':
			cin.clear();
			color = 1;
			break;
		default:
		    cin.clear();
			break;
	}
	SetConsoleTextAttribute(hConsole, color);
	save_settings();
	settings();
}

void change_speed()
{
	system("cls");
	cout << " 1- Slow\n"
		 << " 2- Normal\n"
		 << " 3- Fast\n"
		 << " 4- Fastest\n\n"
		 << " Enter your choice:  ";
	char choice;
	cin >> choice;
	switch (choice)
	{
		case '1':
		    cin.clear();
			delay = 1000;
			break;
		case '2':
		    cin.clear();
			delay = 500;
			break;
		case '3':
		    cin.clear();
			delay = 200;
			break;
		case '4':
		    cin.clear();
			delay = 0;
		default:
		    cin.clear();
			break;
	}
	save_settings();
	settings();
}

void change_text_size(int width, int height)
{
	cfi.dwFontSize.X = width;		// Change font width
	cfi.dwFontSize.Y = height;		// Change font height
	SetCurrentConsoleFontEx(hConsole, FALSE, &cfi);
	GetWindowRect(console, &r);
	MoveWindow(console, r.left, r.top, 1300, 800, TRUE);	// Undo window size change from changing font size
}

int main()
{
	// Seed random number generator
	srand((unsigned)time(0));

	// Set console code page to UTF-8 so console knows how to interpret string data
	SetConsoleOutputCP(CP_UTF8);

	// Enable buffering to prevent chopping up UTF-8 byte sequences
	setvbuf(stdout, nullptr, _IOFBF, 1000);

	cfi.cbSize = sizeof(cfi);
	cfi.nFont = 0;
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_NORMAL;


	// Comment this if compiling on visual studio
	//wcscpy(cfi.FaceName, L"Courier New");

	// Uncomment this if compiling on visual studio
	wcscpy_s(cfi.FaceName, L"Courier New");

	load_settings();

	while (true)
	{
		change_text_size(10, 20);

		// Reconfirming window resizing to remove unexpected behaviour
		GetWindowRect(console, &r);
		MoveWindow(console, r.left, r.top, 1300, 800, TRUE);

		SetConsoleTextAttribute(hConsole, color);

		system("cls");
		string name;
		cout << " Type 'settings' to change configuration.\n";
		cout << " Type 'exit' to quit.\n";
		cout << " Type 'random' to generate a random pattern.\n";
		cout << " Type a pattern's name to generate it.\n";
		cout << " Check the Patterns folder for available patterns, inputting a pattern that doesn't exist will do nothing.\n";
		cout << " During pattern: <p> to pause\n                 <1-9> to change color, <0> to toggle rainbow\n"
             << "                 <s> to save starting state to a file (ends pattern)\n                 <e> to exit pattern\n\n\n ";
		cin >> name;

		if (name == "settings")
		{
			settings();
			continue;
		}

		else if (name == "exit")
        	{
			break;
        	}

		else if (name == "random")
		{
			int c;
			cout << " Enter chance of live cell in initial state:  ";
			cin >> c;
			board = random_state(62, 182, c);
			system("cls");
		}

		else
		{
			name += ".txt";
			name = "Patterns\\" + name;
			ifstream state(name);
			if (state)
			{
					board = load_from_file(state);
			}
			else
			{
				continue;
			}
			system("cls");
		}

		change_text_size(7, 12);

		// Reconfirming window resizing to remove unexpected behaviour
		GetWindowRect(console, &r);
		MoveWindow(console, r.left, r.top, 1300, 800, TRUE);
		CONSOLE_CURSOR_INFO cursorInfo;
		GetConsoleCursorInfo(hConsole, &cursorInfo);
		cursorInfo.dwSize = 1;
		cursorInfo.bVisible = FALSE;
		SetConsoleCursorInfo(hConsole, &cursorInfo);

		world next_board = next_state(board);
		print(board);
		cout << " Press <ENTER> to begin... ";
		string pause;
		cin.ignore();
		getline(cin, pause);
		system("cls");

		int rb_delay;
		while (true)
		{
			if (rainbow)
			{
			     rb_delay++;
			     if (rb_delay % 4 == 0)
                         SetConsoleTextAttribute(hConsole, ((rand() % 14)));
			}
			print(next_board, board);
			board = next_board;
			next_board = next_state(next_board);
			if (_kbhit())
			{
				char key = _getch();
				if (key == 'p' || key == 'P')
				{
					cout << " Paused, press <ENTER> to continue... ";
					string pause;
					getline(cin, pause);
				}
				else if (key == 's' || key == 'S')
				{
					system("cls");
					change_text_size(10, 20);

					// Reconfirming window resizing to remove unexpected behaviour
					GetWindowRect(console, &r);
					MoveWindow(console, r.left, r.top, 1300, 800, TRUE);
					CONSOLE_CURSOR_INFO cursorInfo;
					GetConsoleCursorInfo(hConsole, &cursorInfo);
					cursorInfo.dwSize = 1;
					cursorInfo.bVisible = FALSE;
					SetConsoleCursorInfo(hConsole, &cursorInfo);

					cout << " Save as: ";
					string name;
					cin >> name;
					name += ".txt";
					name = "Patterns\\" + name;
					ofstream state(name);
					save_to_file(state);
					break;
				}
				else if (key == 'e' || key == 'E')
				{
					break;
				}
				else if (key == '1')
				{
					rainbow ? (rainbow = !rainbow) : (rainbow = rainbow);
					SetConsoleTextAttribute(hConsole, 4);
				}
				else if (key == '2')
				{
					rainbow ? (rainbow = !rainbow) : (rainbow = rainbow);
					SetConsoleTextAttribute(hConsole, 5);
				}
				else if (key == '3')
				{
					rainbow ? (rainbow = !rainbow) : (rainbow = rainbow);
					SetConsoleTextAttribute(hConsole, 2);
				}
				else if (key == '4')
				{
					rainbow ? (rainbow = !rainbow) : (rainbow = rainbow);
					SetConsoleTextAttribute(hConsole, 8);
				}
				else if (key == '5')
				{
					rainbow ? (rainbow = !rainbow) : (rainbow = rainbow);
					SetConsoleTextAttribute(hConsole, 3);
				}
				else if (key == '6')
				{
					rainbow ? (rainbow = !rainbow) : (rainbow = rainbow);
					SetConsoleTextAttribute(hConsole, 7);
				}
				else if (key == '7')
				{
					rainbow ? (rainbow = !rainbow) : (rainbow = rainbow);
					SetConsoleTextAttribute(hConsole, 13);
				}
				else if (key == '8')
				{
					rainbow ? (rainbow = !rainbow) : (rainbow = rainbow);
					SetConsoleTextAttribute(hConsole, 6);
				}
				else if (key == '9')
				{
					rainbow ? (rainbow = !rainbow) : (rainbow = rainbow);
					SetConsoleTextAttribute(hConsole, 1);
				}
				else if (key == '0')
				{
				     rb_delay = 0;
					rainbow = !rainbow;
				}
			}
			Sleep(delay);
		}
	}
}
