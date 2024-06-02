#include <iostream>

const char STAGE_DATA[] = "\
   |   |   \n\
---|---|---\n\
   |   |   \n\
---|---|---\n\
   |   |   ";

const int STAGE_ROW{ 3 };
const int STAGE_COL{ 3 };

void Draw(const int stage[], int row, int col, const bool *isEmpty);
int CountConnect(const int *stage, int row, int col, const bool *isEmpty);

int main()
{

}

