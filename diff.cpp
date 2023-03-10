#include <iostream>
#include <conio.h>
#include <windows.h>
#include <fstream>
#include "diff.h"
using namespace std;

int main(int argc, char* argv[]) {
    system("cls");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    HideCursor();

    char el;
    int cntr = 0;
    int length_1 = 0, length_2 = 0;
    fstream file;

    length_1 = GetFileLen(argv[1], length_1);
    length_2 = GetFileLen(argv[2], length_2);

    char* string_1 = new char[length_1 + 1];
    char* string_2 = new char[length_2 + 1];

    string_1 = FileToString(argv[1], string_1);
    string_2 = FileToString(argv[2], string_2);

    int **matrix = LCS_Matrix(string_1, string_2, length_1, length_2);
    int result_len = matrix[length_1][length_2];
    char* result = LCS_String(string_1, string_2, matrix, result_len, length_1, length_2);
    // cout << result; 
    // getch()

    MainMenu(argv[1], argv[2], string_1, string_2, result, length_1, length_2, result_len);
    // for (int i = 0; i < length_1; ++i) {
    //     for (int j = 0; j < length_2; ++j) {
    //         cout << matrix[i][j] << " ";
    //     }
    //     cout << "\n";
    // }

    // cout << result;

    // getch();
    system("cls");
    return 0;
}