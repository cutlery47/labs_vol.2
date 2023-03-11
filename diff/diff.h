using namespace std;

// int terminal_width = 211;
// int terminal_height = 57;
int terminal_width = 121;
int terminal_height = 30;
fstream file;
const char* separator_line = "========================================================================================================================";

void HideCursor() {  // makes the cursor invisible
    CONSOLE_CURSOR_INFO cursor;  
	cursor.bVisible = FALSE;    
	cursor.dwSize = sizeof(cursor);    
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);    
	SetConsoleCursorInfo(handle, &cursor);
}

void SetFullscrn() {  // sets terminal to fullscreen
    keybd_event(VK_MENU,0x38,0,0);  
    keybd_event(VK_RETURN,0x1c,0,0);
    keybd_event(VK_RETURN,0x1c,KEYEVENTF_KEYUP,0);
    keybd_event(VK_MENU,0x38,KEYEVENTF_KEYUP,0);
}

void SetCursor(int x, int y) { // sets the cursor in a particular position
    COORD coord;
    coord.X = x;
    coord.Y = y;

    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);

}

void PrintCenteredText(const char* string, int y, int multiplier) { // makes the text centered
    int i = 0, len = 0;
    while (string[i] != '\0') { // calculating string length
        len += 1;
        ++i;
    }

    if (multiplier > 0) {
        SetCursor(terminal_width - (terminal_width /(2 * multiplier)) - (len / 2), y); // centering text
    } else if (multiplier < 0) {
        SetCursor(-terminal_width /(2 * multiplier) - (len / 2), y); //yep
    }
    

    for (int i = 0; i < len; ++i) {
        cout << string[i];
    }

    cout << endl;
}

void Confirm(int y) {
    short confirm = 0;
    PrintCenteredText("Нажмите Enter, чтобы продолжить...", y, 1);
    while (confirm != 13) {
        confirm = getch();
    }
}

int InputCheck (int argc, char** argv) {
    if (argc < 3) {
        PrintCenteredText("Недостаточное число аргументов...", 0, 1);
        Confirm(1);
        return 1;
    }
}

void color(int x) {
    if (x >= 0 && x <= 15)
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), x);
    else
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);  
} 

char* ReverseString(const char* string, int len) {
    char* rev_string = new char[len];

    for (int i = 0; i < len; ++i) {
        rev_string[i] = string[len - i - 1];
    }

    return rev_string;
}

char* ReverseString(char* string, int len) {
    char* rev_string = new char[len];

    for (int i = 0; i < len; ++i) {
        rev_string[i] = string[len - i - 1];
        // cout << rev_string[i];
    }

    return rev_string;
}

int GetFileLen(char* file_name, int len) {
    char el;
    file.open(file_name, fstream::in);

    while (file.get(el)) {
        ++len;
    }

    file.close();

    return len;
}

char* FileToString(char* file_name, char* string) {
    char el;
    int cntr = 0;
    file.open(file_name, fstream::in);

    while (file.get(el)) {
        string[cntr] = el;
        ++cntr;
    }

    file.close();

    return string;
}

int** LCS_Matrix(const char* string_1, const char* string_2, int height, int width) {
    // the matrix consists of amounts of elements, which are common
    // in both the first and the second string

    int** LCS_Matrix = new int*[height + 1];    //
                                                // initializing an array dynamically
    for (int i = 0; i < height + 1; ++i) {      //
        LCS_Matrix[i] = new int[width + 1];     //
    }

    for (int i = 0; i < height + 1; ++i) {     // filling the array with zero's
        for (int j = 0; j < width + 1; ++j) {
            LCS_Matrix[i][j] = 0;
        }
    }

    for (int i = 1; i <= height; ++i) {    //filling the array with numbers according to the LCS algorithm
        for (int j = 1; j <= width; ++j) {
            if (string_1[i - 1] == string_2[j - 1]) {
                LCS_Matrix[i][j] = LCS_Matrix[i - 1][j - 1] + 1;
            } else {
                if (LCS_Matrix[i - 1][j] > LCS_Matrix[i][j - 1]) {
                    LCS_Matrix[i][j] = LCS_Matrix[i - 1][j];
                } else {
                    LCS_Matrix[i][j] = LCS_Matrix[i][j - 1];
                }
            }
        }
    }

    return LCS_Matrix;
}

char* LCS_String(const char* string_1, const char* string_2, int** matrix, int size, int i, int j) {
    int cnt = 1;
    char* reversed_string = new char[size];

    for (int i = 0; i < size + 1; ++i) {
        reversed_string[i] = 0;
    }

    while (i > 0 && j > 0) {
        if (string_1[i - 1] == string_2[j - 1]) {
            reversed_string[size - cnt] = string_1[i - 1];
            cnt++;
            i--;
            j--;
        } else {
            if (matrix[i - 1][j] > matrix[i][j - 1]) {
                i--;
            } else {
                j--;
            }
        }
    }

    // reversed_string = ReverseString(reversed_string);
    return reversed_string;
}

void HighlightedPrint(char* common, const char* string, int string_size, int common_size, int line, int begin, int end) {
    int i = 0, j = 0;
    int cntr = 0;
    SetCursor(begin, line);

    while ((i < string_size) && (j < common_size)) {
        if (string[i] == '\n' || (begin + cntr >= end)) {
            if (common[j] == '\n') {
                ++j;
            }
            cntr = 0;
            line += 1;
            SetCursor(begin, line);
        } else {
            if (common[j] == string[i]) {
                color(10);
                cout << common[j];
                color(7);
                ++j;
            } else {
                color(12);
                cout << string[i];
                color(7);
            }
            ++cntr;
        }
        ++i;    
    }

    while (i < string_size) {
        SetCursor(begin + cntr, line);
        if (begin + cntr >= end) {
            cntr = 1;
            ++line;
            SetCursor(begin, line);
        } else {
            ++cntr;
        }
        color(12);
        cout << string[i];
        color(7);
        ++i;
    }
}

void MainMenu(char* file_name_1, char* file_name_2, char* string_1, char* string_2, char* common, int string_1_size, int string_2_size, int common_size) { // main menu interface
    int current_line = 0;
    while (current_line < terminal_height) {

        if (current_line == 0 || current_line == 2 || current_line == 5 || current_line == terminal_height - 2) {
            cout << separator_line << endl;
        } else if (current_line == 1) {
            PrintCenteredText("Добро пожаловать в Beyond Compare 2", current_line, 1);
        } else if ((current_line > 2 && current_line < 5) || (current_line > 5 && current_line < terminal_height - 3)) {
            if (current_line == 3) {
                PrintCenteredText("Файл №1", current_line, -2);
                PrintCenteredText("Файл №2", current_line, 2);
            } else if (current_line == 4) {
                PrintCenteredText(file_name_1, current_line, -2);
                PrintCenteredText(file_name_2, current_line, 2);
            } else if (current_line == 6) {
                HighlightedPrint(common, string_1, string_1_size, common_size, current_line, 0, 60);
                SetCursor(terminal_width/2, current_line);
                HighlightedPrint(common, string_2, string_2_size, common_size, current_line, terminal_width / 2 + 3, terminal_width - 1);
            }
            PrintCenteredText("|||", current_line, 1);
            
        } else if (current_line == terminal_height - 1) {
            Confirm(current_line);
        }
        

        current_line++;
    }
}