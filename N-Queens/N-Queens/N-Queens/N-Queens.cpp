#include <iostream>
#include <vector>
#include <cstring>
#include <algorithm>
#include <ctime>

using namespace std;

int N;

const int MAX = 10000;

int initialStateOfQueens[MAX];

int numberOfQueensRows[MAX];
int numberOfQueensFirstDiagonal[MAX * 2];
int numberOfQueensSecondDiagonal[MAX * 2];

char solutionChessBoard[MAX][MAX];

vector<int> v;

int steps;

void removeQueen(int rows, int cols)
{
    numberOfQueensRows[rows]--;
    numberOfQueensFirstDiagonal[rows - cols + N]--;
    numberOfQueensSecondDiagonal[rows + cols]--;
}

void addQueen(int rows, int cols)
{
    numberOfQueensRows[rows]++;
    numberOfQueensFirstDiagonal[rows - cols + N]++;
    numberOfQueensSecondDiagonal[rows + cols]++;
}

void getFirstConfiguration()
{
    random_shuffle(v.begin(), v.end());

    for (int i = 0; i < N; ++i)
    {
        initialStateOfQueens[i] = v[i];
    }
}

int getConfilictsCount(int rows, int cols)
{

    int conflictsCount = numberOfQueensRows[rows] + numberOfQueensFirstDiagonal[rows - cols + N] + numberOfQueensSecondDiagonal[rows + cols];
    
    if (initialStateOfQueens[cols] == rows)
        conflictsCount -= 3;

    return conflictsCount;

}

int getRowWithMinConflicts(int cols)
{
    int minConflicts = MAX;
    int rowWithMinCOnflicts[MAX];

    for (int rows = 0; rows < N; rows++)
    {
        rowWithMinCOnflicts[rows] = getConfilictsCount(rows, cols);
        
        if (rowWithMinCOnflicts[rows] < minConflicts)
            minConflicts = rowWithMinCOnflicts[rows];
    }

    vector<int> currentRow;

    for (int rows = 0; rows < N; rows++)
    {
        if (rowWithMinCOnflicts[rows] == minConflicts)
        {
            currentRow.push_back(rows);
        }
    }

    return currentRow[rand() % currentRow.size()];
}

int getColumnWithMaxConflicts()
{
    int maxConflicts = -1;

    for (int cols = 0; cols < N; cols++)
    {

        int currentConflicts = getConfilictsCount(initialStateOfQueens[cols], cols);
        
        if (currentConflicts > maxConflicts)
            maxConflicts = currentConflicts;

    }

    vector<int> currentCol;

    for (int cols = 0; cols < N; cols++)
    {
        if (getConfilictsCount(initialStateOfQueens[cols], cols) == maxConflicts)
        {
            currentCol.push_back(cols);
        }
    }

    return currentCol[rand() % currentCol.size()];

}

void initConflicts()
{
    memset(numberOfQueensRows, 0, sizeof(numberOfQueensRows));
    memset(numberOfQueensFirstDiagonal, 0, sizeof(numberOfQueensFirstDiagonal));
    memset(numberOfQueensSecondDiagonal, 0, sizeof(numberOfQueensSecondDiagonal));

    for (int cols = 0; cols < N; cols++)
        addQueen(initialStateOfQueens[cols], cols);
}

int getZeroConflicts()
{
    int zeroConflicts = 0;
    
    for (int cols = 0; cols < N; ++cols)
    {
        zeroConflicts += (getConfilictsCount(initialStateOfQueens[cols], cols) == 0);
    }

    return zeroConflicts;
}

void printChessBoard()
{
    memset(solutionChessBoard, '.', sizeof(solutionChessBoard));

    for (int cols = 0; cols < N; cols++)
    {
        solutionChessBoard[initialStateOfQueens[cols]][cols] = '*';
    }

    for (int rows = 0; rows < N; rows++)
    {
        for (int cols = 0; cols < N; cols++)
        {
           cout << solutionChessBoard[rows][cols] << " ";
        }

        cout << endl;
    }
}

void NQueens()
{

    time_t start = clock();
    
    for (int i = 0; i < N; ++i)
    {
        v.push_back(i);
    }

    int MAX_ITER = 3 * N;

    while (true)
    {

        getFirstConfiguration();

        initConflicts();

        for (int i = 0; i <= MAX_ITER && getZeroConflicts() != N; ++i)
        {

            int maxConflictedCol = getColumnWithMaxConflicts();
            removeQueen(initialStateOfQueens[maxConflictedCol], maxConflictedCol);
            initialStateOfQueens[maxConflictedCol] = -1;

            initialStateOfQueens[maxConflictedCol] = getRowWithMinConflicts(maxConflictedCol);
            addQueen(initialStateOfQueens[maxConflictedCol], maxConflictedCol);

            steps++;

        }

        if (getZeroConflicts() == N)
        {
            break;
        }

    }

    time_t end = clock();
    cout << (end - start) / (double) CLOCKS_PER_SEC << endl;
    
    printChessBoard();

}

int main()
{
    cout << "IMPLEMENTING N QUEENS" << endl;
    cout << "Please enter the number of queens: " << endl;
    
    cin >> N;
    
    NQueens();

}

