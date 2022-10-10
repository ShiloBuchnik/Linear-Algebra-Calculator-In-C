#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <windows.h>

// Sending out error message and cleaning buffer
static void invalidInput(void)
{
    printf("Invalid input, please try again: ");
    while (getchar() != '\n');
}

// Using Windows' API to enable color in program
int enableColor(void)
{
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE); // We need to get the windows handle of our stdout
    if (hOut == INVALID_HANDLE_VALUE) return -1; // If something went wrong - return -1

    DWORD dwMode = 0;
    if (!GetConsoleMode(hOut, &dwMode)) return -1; // checking the output mode, idk, anyway if something went wrong - return -1

    dwMode |= 0x0004; // ENABLE_VIRTUAL_TERMINAL_PROCESSING macro, this is what we tell to setConsoleMode() to enable ansi escape sequences
    if (!SetConsoleMode(hOut, dwMode)) return -1;
    // setting the console mode to enable ansi escape sequences, if something went wrong - return -1

    return 1; // Return 1 on success
}

void swap(double* num1, double* num2)
{
    double temp = *num1;
    *num1 = *num2;
    *num2 = temp;
}

bool areEqual(double a, double b){
    if (fabs(a - b) < 1E-10) return 1;
    else return 0;
}

/* This function takes an input and verify that it's a *positive* int; and if it's in the array 'arr'.
if we only want to check whether the input is a positive int, we can pass NULL as 'arr', and an arbitrary int as 'arrLen' */
void positiveIntVerify(int* input, int* arr, int arrLen)
{
    while (1)
    {
        while (scanf("%d", input) != 1 || *input < 1) invalidInput(); // We want it to be an int and a positive one

        if (arr == NULL) return;
        else // checking if input is in the array
        {
            for (int i = 0; i < arrLen; i++)
            {
                if (*input == arr[i]) return;
            }
            invalidInput();
        }
    }
}

/* For getting matrix elements & verifying them. Verification part is a tad spaghetti, I just wanted to verify perfectly */
void doubleVerify(int numRow, int numColumn, double* matrix, bool print)
{
    int i, j, symbol;
    bool flag = 0;
    double temp1, temp2;

    for (i = 0; i < numRow; i++)
    {
        if (print) printf("Enter row #%d\n", i+1);

        for (j = 0; j < numColumn; j++)
        {
            while (flag == 1 || scanf("%lf", &temp1) != 1)
            {
                invalidInput();
                flag = 0;
            }

            symbol = getchar();

            if (symbol == '/' || symbol == '\\')
            {
                if (scanf("%lf", &temp2) != 1)
                {
                    j--;
                    flag = 1;
                    continue;
                }
                *(matrix + i*numColumn + j) = temp1 / temp2;
            }

            else if (symbol == ' ' || symbol == '\n') *(matrix + i*numColumn + j) = temp1;

            else
            {
                j--;
                flag = 1;
                continue;
            }
        }
        if (print) printf("\n");
    }

    if (!print) printf("\n");
}

// This function gets number of rows and number of columns from user
void getRowAndColumnNum(int* numRow, int* numColumn)
{
    printf("Enter row size: "); // Number of arrays
    positiveIntVerify (numRow, NULL, 0);
    while (getchar() != '\n');
    printf("Enter column size: "); // Number of elements in array
    positiveIntVerify (numColumn, NULL, 0);
    while (getchar() != '\n');
    printf("\n");
}

void displayMatrix(int numRow, int numColumn, double* matrix)
{
    if (!matrix) return;

    for (int i = 0; i < numRow; i++)
    {
        for (int j = 0; j < numColumn; j++)
        {
            printf("%lf  ", *(matrix + i*numColumn + j));
        }
        printf("\n");
    }

    printf("\n");
}

// 'displaySolution' displays vectors in a slightly different way than 'displayMatrix'
void displaySolution(int size, double* solution) // For displaying the solution
{
    int i;

    printf("(");
    for (i = 0; i < size - 1; i++) printf("%lf, ", solution[i]);
    printf("%lf)\n\n", solution[i]); // I don't want a comma in the end :>
}

// Setting given square matrix to identity matrix
void setIdentityMatrix(int n, double* matrix)
{
    if (!matrix) return;

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (j == i) *(matrix + i*n + j) = 1;
            else *(matrix + i*n + j) = 0;
        }
    }
}

void matrixCopy(int numRow, int numColumn, double* srcMatrix, double* destMatrix)
{
    for (int i = 0; i < numRow; i++)
    {
        for (int j = 0; j < numColumn; j++)
        {
            *(destMatrix + i*(numColumn) + j) = *(srcMatrix + i*(numColumn) + j);
        }
    }
}

// This function swaps between two rows in a matrix
void switchRows(int row1, int row2, int numColumn, double* matrix)
{
    for (int j = 0; j < numColumn; j++) swap(matrix + row1*numColumn + j, matrix + row2*numColumn + j);
}

/* THIS FUNCTION ALLOCATES MEMORY FOR 'new_matrix'
This function takes a matrix and a vector, and inserts the vector in the column passed in 'column' argument,
without overwriting the column present there (it simply takes a step right, along with all the columns after it)

Remember the indices start from 0 when passing 'column' */
double* insertColumn(int numRow, int numColumn, int column, double* matrix, double* columnVector)
{
    if (column < 0 || numColumn < column) return NULL; // Out of bounds

    double* new_matrix = (double*) malloc(numRow * (numColumn + 1) * sizeof(double));

    for (int i = 0; i < numRow; i++)
    {
        for (int j = 0; j < numColumn + 1; j++)
        {
            if (j < column) *(new_matrix + i*(numColumn + 1) + j) = *(matrix + i*numColumn + j);
            else if (j == column) *(new_matrix + i*(numColumn + 1) + j) = *(columnVector + i);
            else *(new_matrix + i*(numColumn + 1) + j) = *(matrix + i*numColumn + j - 1);
            // After inserting the column to the new matrix; the original matrix is one step forward, so we even it out by reducing 1
        }
    }

    return new_matrix;
}

// Print introduction and get mode from user
int printOpeningAndGetNum(void)
{
    printf("Hello! This program offers: \n");
    printf("\x1b[92m1)\x1b[0m Various matrix-related calculations\n");
    printf("\x1b[91m2)\x1b[0m Square system of equations (number of variables = number of equations) calculator\n");
    printf("\x1b[94m3)\x1b[0m Matrix multiplication calculator\n\n");
    printf("Please input your choice[\x1b[92m1\x1b[0m/\x1b[91m2\x1b[0m/\x1b[94m3\x1b[0m]: ");

    int num, *arr = (int*) malloc(3 * sizeof(int));
    arr[0] = 1; arr[1] = 2; arr[2] = 3;

    positiveIntVerify(&num, arr, 3);
    free(arr);

    system("cls");

    return num;
}

// Printing instructions for user's choice of mode
// Did you know? Separating strings in 'printf' with whitespace - concatenates them
void printText(int num)
{
    switch (num)
    {
    case 1:
        printf("In this program you input a \x1b[96mmatrix\x1b[0m\n"
        "The output is the \x1b[92mreduced row echelon form\x1b[0m,\n"
        "as well as the \x1b[93mrank\x1b[0m, the \x1b[94minverse matrix\x1b[0m, the \x1b[91mdeterminant\x1b[0m and the \x1b[95madjoint matrix\x1b[0m\n"
        "Please input each row as a string of numbers, with spaces between them. For example: 0.3 1/2 5 8 23");
        break;
    case 2:
        printf("In this program you input a matrix of the variable coefficients (\x1b[31mA\x1b[0m) and a vector of the free coefficients (\x1b[34mb\x1b[0m),\n"
        "which represent the linear system of equations.\n"
        "In turn, the program solves the equation \x1b[31mA\x1b[0m\x1b[92mx\x1b[0m=\x1b[34mb\x1b[0m, and the output is solution vector \x1b[92mx\x1b[0m\n"
        "If there isn't a solution, the program uses Least Squares algorithm to provide a minimal point, and a \x1b[96mremainder\x1b[0m vector;\n"
        "Or notify that there isn't a single minimal point");
        break;
    case 3:
        printf("In this program you input two matrices: \x1b[91mmatrix 1\x1b[0m and \x1b[94mmatrix 2\x1b[0m\n"
        "The output is the \x1b[92mproduct\x1b[0m: \x1b[91mmatrix 1\x1b[0m * \x1b[94mmatrix 2\x1b[0m");
        break;
    }

    printf("\n\nThings to take heed of:\n"
        "1) If you type a non-number character in a row,\n"
        "the program would require you to retype every entry \x1b[4mfrom said non-number\x1b[0m to the end of the row\n"
        "2) If you wish to input a fraction, \x1b[4mdo not\x1b[0m include spaces before and after the fraction line\n"
        "This is valid: a/b\n"
        "This is not valid: a / b; a /b; a/ b\n"
        "3) This calculator is accurate up to 10 digits after the decimal\n"
        "4) Enjoy ^~^\n\n\n");
}


// Below are functions that were used in the code, and are no longer needed. Why do I keep them? idk why not

/*
This function takes a matrix and separate a column (listed as 'column') from it.
It returns a pointer to a new matrix without that column, and it stores that column in 'arrColumn'
double* separateColumn(int numRow, int numColumn, int column, double* matrix, double* arrColumn)
{
    if (column < 0 || column >= numColumn) return NULL;

    int i, j, x = 0;
    double *scalarMatrix = (double*) malloc(numRow * (numColumn - 1) * sizeof(double));

    for (i = 0; i < numRow; i++)
    {
        for (j = 0; j < numColumn; j++)
        {
            if (j == column) *(arrColumn + i) = *(matrix + i*numColumn + j);
            else
            {
                *(scalarMatrix + x) = *(matrix + i*numColumn + j);
                x++;
            }
        }
    }

    return scalarMatrix;
}


The function takes a matrix, a column vector and a number 'column'.
it swaps between the column vector and the column in the matrix whose number is stored in 'column'
void switchColumns(int numRow, int numColumn, int column, double* matrix, double* arrColumn)
{
    if (column < 0 || column >= numColumn) return;

    for (int i = 0; i < numRow; i++) swap(matrix + i*numColumn + column, arrColumn + i);
}
*/