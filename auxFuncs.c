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

/* This function takes an input and verify that it's a *positive* int; and that it's in the array 'arr'.
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

/* For getting matrix elements & verifying them. Verification part is a tad spaghetti, I just wanted to verify perfectly
We also pass a 'print' flag:
If it's 1, we ask for the row repeatedly. If it's 0, we don't */
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

// This function gets number of rows and number of columns from user, and stores it in 'numRow' and 'numColumn'
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

// This function gets size of vector from user, and stores it in 'size'
void getVectorSize(int* size)
{
    printf("Enter size: ");
    positiveIntVerify (size, NULL, 0);
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
            printf("%0.10lf  ", *(matrix + i*numColumn + j));
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

/* THIS FUNCTION MIGHT ALLOCATE MEMORY FOR 'new_matrix'
This function takes a matrix, a vector, and a flag.
If extend_or_overwrite == 'e', then the function inserts the vector in the index 'column', without overwriting,
and extends the columns after it (i.e. taking them all one step to the right).
If extend_or_overwrite == 'o', then the function simply insert the vector in index 'column', overwriting that column

Remember the indices start from 0 when passing 'column' */
double* insertColumn(int numRow, int numColumn, int column, double* matrix, double* columnVector, char extend_or_overwrite)
{
    if (extend_or_overwrite == 'o')
    {
        for (int i = 0; i < numRow; i++) *(matrix + i*numColumn + column) = *(columnVector + i);
        return NULL;
    }
    else // In this case extend_or_overwrite == 'e'
    {
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
}

/* THIS FUNCTION ALLOCATES MEMORY FOR 'vector'
this function takes a matrix and an 'rc' flag that can hold either 'r' for 'row' or 'c' for 'column'
If it holds 'r', it copies and returns the row in 'row_or_column' index
If it holds 'c', it copies and returns the column in 'row_or_column' index

remember that 'row_or_column' starts from 0, not 1 */
double* returnRowOrColumn(int numRow, int numColumn, int row_or_column, double* matrix, char rc)
{
    if (rc != 'r' && rc != 'c') return NULL;

    double* vector = NULL;

    if (rc == 'r')
    {
        vector = (double*) malloc(numColumn * sizeof(double));
        for (int j = 0; j < numColumn; j++) *(vector + j) = *(matrix + row_or_column*numColumn + j);
    }
    else // In this case rc == 'c'
    {
        vector = (double*) malloc(numRow * sizeof(double));
        for (int i = 0; i < numRow; i++) *(vector + i) = *(matrix + i*numColumn + row_or_column);
    }

    return vector;
}

/* This function takes a 'destMatrix' and its numColumn, truncates its rows and columns according to 'newNumRow' and 'newNumColumn',
and stores the truncated version in 'srcMatrix' */
void truncateMatrix(int numColumn, double* srcMatrix, int newNumRow, int newNumColumn, double* destMatrix)
{
    for (int i = 0; i < newNumRow; i++)
    {
        for (int j = 0; j < newNumColumn; j++) *(destMatrix + i*newNumColumn + j) = *(srcMatrix + i*numColumn + j);
    }
}

/* THIS FUNCTION ALLOCATES MEMORY FOR 'vector'
This function returns a standard vector, where every element is 0, except in index 'k', where it is 1 */
double* getStandardVector(int size, int k)
{
    if (size <= 0 || k < 0 || size <= k) return NULL;

    double* vector = (double*) malloc(size * sizeof(double));
    for (int i = 0; i < size; i++) *(vector + i) = (i == k ? 1 : 0);

    return vector;
}

/* The '%' operator in C is actually the remainder operator, not modulo operator
When both numbers are positive, it acts like the modulo operator, but when of them is negative, it does not.
For that reason we have this function to replace it */
int modulo(int n, int N)
{
    if (N <= 0) return -1;
    else return (n % N + N) % N;
}


// Print introduction and get mode from user
int printOpeningAndGetNum(void)
{
    printf("Hello! This program offers: \n");
    printf("\x1b[92m1)\x1b[0m Various matrix-related calculations\n");
    printf("\x1b[91m2)\x1b[0m Square system of equations (number of variables = number of equations) calculator\n");
    printf("\x1b[94m3)\x1b[0m Matrix multiplication calculator\n");
    printf("\x1b[95m4)\x1b[0m Linear and circular convolution calculator\n\n");
    printf("Please input your choice[\x1b[92m1\x1b[0m/\x1b[91m2\x1b[0m/\x1b[94m3\x1b[0m/\x1b[95m4\x1b[0m]: ");

    int num;
    int* arr = (int*) malloc(4 * sizeof(int));
    arr[0] = 1; arr[1] = 2; arr[2] = 3; arr[3] = 4;

    positiveIntVerify(&num, arr, 4);
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
        "It also provides several decompositions, such as \x1b[32mLU\x1b[0m, \x1b[33mLDV\x1b[0m and \x1b[31mQR\x1b[0m\n"
        "And at last, it provides the \x1b[36mFrobenius norm\x1b[0m\n"
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

    case 4:
        printf("In this program you input two vectors: \x1b[91mvector 1\x1b[0m and \x1b[94mvector 2\x1b[0m,\n"
        "and get their \x1b[92mlinear\x1b[0m and \x1b[32mcircular\x1b[0m convolution");
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