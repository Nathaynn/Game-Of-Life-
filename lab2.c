#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define TABLE_WIDTH 8l

struct MyTable *initRandomTable();
void printTable(struct MyTable *table);
int determineValue(int currValue, int counter);
struct MyTable *stepTable(struct MyTable *table) ;
void cleanTable(struct MyTable *table);

struct MyTable {
    int* topGhost;
    int* botGhost;
    int* leftGhost;
    int* rightGhost;
    int* center;
    int ne;
    int nw;
    int se;
    int sw;
};

int main(int argc, char* argv[]) {    
    struct MyTable *table = initRandomTable();
    printTable(table);

    struct MyTable *newTable = stepTable(table);
    printTable(newTable);

    cleanTable(newTable);
    cleanTable(table);

    return 0;
}

/* init a table that is used for the game of life */
struct MyTable *initRandomTable() {

    // initialize space within arrays
    int* top = (int *)malloc(sizeof(int) * (TABLE_WIDTH - 2));
    int* bot = (int *)malloc(sizeof(int) * (TABLE_WIDTH - 2));
    int* left = (int *)malloc(sizeof(int) * (TABLE_WIDTH - 2));
    int* right = (int *)malloc(sizeof(int) * (TABLE_WIDTH - 2));
    int* cent = (int *)malloc(sizeof(int) * ((TABLE_WIDTH - 2) * (TABLE_WIDTH-2)));
    
    
    // fill in ghosts
    srand(time(0));
    for (int i = 0; i < (TABLE_WIDTH - 2); i++) {
        top[i] = rand() % 2;
        bot[i] = rand() % 2;
        left[i] = rand() % 2;
        right[i] = rand() % 2;
    }

    // fill in center
    sleep(1);
    srand(time(0));
    for (int i = 0; i < ((TABLE_WIDTH - 2) * (TABLE_WIDTH - 2)); i++) {
        cent[i] = rand() % 2;
    }
    
    // create table
    struct MyTable *table = (struct MyTable *)malloc(sizeof(struct MyTable)); 
    table->topGhost = top;
    table->botGhost = bot;
    table->leftGhost = left;
    table->rightGhost = right;
    table->center = cent;

    sleep(1);
    srand(time(0));

    table->ne = rand() % 2;
    table->nw = rand() % 2;
    table->se = rand() % 2;
    table->sw = rand() % 2; 
    
    return table;
}

/* print the table that is used for the simulation. This is mainly used for
 * debugging. */
void printTable(struct MyTable *table) {
    
    // top row
    printf("\n%d  ", table->nw);
    for (int i = 0; i < (TABLE_WIDTH - 2); i++) {
        printf("%d  ", table->topGhost[i]);
    }
    printf("%d\n", table->ne);

    // everything in the middle
    for (int i = 0; i < (TABLE_WIDTH - 2); i++) {
        printf("%d  ", table->leftGhost[i]);
        for (int j = 0; j < (TABLE_WIDTH - 2); j++) {
            printf("%d  ", table->center[((TABLE_WIDTH - 2) * i) + j]);
        }
        printf("%d\n", table->rightGhost[i]);
    }
    
    // bot row
    printf("%d  ", table->sw);
    for (int i = 0; i < (TABLE_WIDTH - 2); i++) {
        printf("%d  ", table->botGhost[i]);
    }
    printf("%d\n", table->se);
}

/* take a step through the current table and make all updates to the new table.
 * Since we're planning to parallize this later, we don't  want to have updates
 * inteferring with future updates to the table. */
struct MyTable *stepTable(struct MyTable *table) {

    struct MyTable *newTable = (struct MyTable *)malloc(sizeof(struct MyTable));
    newTable->topGhost = (int *)malloc(sizeof(int) * (TABLE_WIDTH - 2));
    newTable->botGhost = (int *)malloc(sizeof(int) * (TABLE_WIDTH - 2));
    newTable->leftGhost = (int *)malloc(sizeof(int) * (TABLE_WIDTH - 2));
    newTable->rightGhost = (int *)malloc(sizeof(int) * (TABLE_WIDTH - 2));
    newTable->center = (int *)malloc(sizeof(int) * ((TABLE_WIDTH - 2) * (TABLE_WIDTH-2)));
    
    // note to future self: pretend table is a 20x20 array, and use the col and
    // row to determine what block to focus on

    // determine nw
    int counter = 0;
    
    if (table->topGhost[0]) {
        counter++;
    }
    if (table->leftGhost[0]) {
        counter++;
    }
    if (table->center[0]) {
        counter++;
    }

    newTable->nw = determineValue(table->nw, counter);

    // determine ne
    counter = 0;
    
    if (table->topGhost[TABLE_WIDTH - 3]) {
        counter++;
    }
    if (table->rightGhost[0]) {
        counter++;
    }
    if (table->center[TABLE_WIDTH - 3]) {
        counter++;
    }
    newTable->ne = determineValue(table->ne, counter);

    // determine sw
    counter = 0;
    
    if (table->leftGhost[TABLE_WIDTH - 3]) {
        counter++;
    }
    if (table->botGhost[0]) {
        counter++;
    }
    if (table->center[(TABLE_WIDTH - 2)  * (TABLE_WIDTH - 3)]) {
        counter++;
    }
    
    newTable->sw = determineValue(table->sw, counter);

    // determine se
    counter = 0;
    
    if (table->rightGhost[TABLE_WIDTH - 3]) {
        counter++;
    }
    if (table->botGhost[TABLE_WIDTH - 3]) {
        counter++;
    }
    if (table->center[(TABLE_WIDTH - 2) * (TABLE_WIDTH - 2) - 1]) {
        counter++;
    }
    
    newTable->se = determineValue(table->se, counter);

    // determine top
    for (int i = 0; i < (TABLE_WIDTH - 2); i++) {
        counter = 0;
        if (i == 0) {
            if (table->nw) {
                counter++;
            } 
            if (table->leftGhost[0]) {
                counter++;
            }
            if (table->center[0]) {
                counter++;
            }
            if (table->topGhost[1]) {
                counter++;
            }
            if (table->center[1]) {
                counter++;
            }
        } else {
            if (table->topGhost[i - 1]) {
                counter++;
            }
            if (i != (TABLE_WIDTH - 3)) {
                if (table->topGhost[i + 1]) {
                    counter++;
                }
                if (table->center[i+1]) {
                    counter++;
                }
            } else {
                if (table->ne) {
                    counter++;
                } 
                if (table->rightGhost[0]) {
                    counter++;
                }
            }
            if (table->center[i-1]) {
                counter++;
            }
            if (table->center[i]) {
                counter++;
            }
        }
        newTable->topGhost[i] = determineValue(table->topGhost[i], counter);
    }

    // determine bot
    for (int i = 0; i < (TABLE_WIDTH - 2); i++) {
        counter = 0;
        if (i == 0) {
            if (table->sw) {
                counter++;
            } 
            if (table->leftGhost[TABLE_WIDTH - 3]) {
                counter++;
            }
            if (table->center[(TABLE_WIDTH - 2) * (TABLE_WIDTH - 3)]) {
                counter++;
            }
            if (table->botGhost[1]) {
                counter++;
            }
            if (table->center[(TABLE_WIDTH - 2) * (TABLE_WIDTH - 3) + 1]) {
                counter++;
            }
        } else {
            if (table->botGhost[i - 1]) {
                counter++;
            }
            if (i != (TABLE_WIDTH - 3)) {
                if (table->botGhost[i + 1]) {
                    counter++;
                }
                if (table->center[(TABLE_WIDTH - 2) * (TABLE_WIDTH - 3) + i + 1]) {
                    counter++;
                }
            } else {
                if (table->se) {
                    counter++;
                } 
                if (table->rightGhost[TABLE_WIDTH - 3]) {
                    counter++;
                }
            }
            if (table->center[(TABLE_WIDTH - 2) * (TABLE_WIDTH - 3) + i - 1]) {
                counter++;
            }
            if (table->center[(TABLE_WIDTH - 2) * (TABLE_WIDTH - 3) + i]) {
                counter++;
            }
        }
        newTable->botGhost[i] = determineValue(table->botGhost[i], counter);
    }

    // determine left
    for (int i = 0; i < TABLE_WIDTH - 2; i++) {
        counter = 0;
        if (i == 0) {
            if (table->nw) {
                counter++;
            }
            if (table->topGhost[0]) {
                counter++;
            }
            if (table->center[0]) {
                counter++;
            }
            if (table->center[TABLE_WIDTH - 2]) {
                counter++;
            }
            if (table->leftGhost[1]) {
                counter++;
            }
        } else {
            if (table->leftGhost[i-1]) {
                counter++;
            }
            if (table->center[(i-1) * (TABLE_WIDTH - 2)]) {
                counter++;
            }
            if (table->center[i * (TABLE_WIDTH - 2)]) {
                counter++;
            }
            if (i != (TABLE_WIDTH - 3)) {
                if (table->center[(i+1) * (TABLE_WIDTH - 2)]) {
                    counter++;
                }  
                if (table->leftGhost[i+1]) {
                    counter++;
                } 
            } else {
                if (table->botGhost[0]) {
                    counter++;
                }
                if (table->sw) {
                    counter++;
                }
            }
        }
        newTable->leftGhost[i] = determineValue(table->leftGhost[i], counter);
    }

    // determine right
    for (int i = 0; i < TABLE_WIDTH - 2; i++) {
        counter = 0;
        if (i == 0) {
            if (table->ne) {
                counter++;
            }
            if (table->topGhost[TABLE_WIDTH - 3]) {
                counter++;
            }
            if (table->center[TABLE_WIDTH - 3]) {
                counter++;
            }
            if (table->center[(TABLE_WIDTH - 2) + (TABLE_WIDTH - 3)]) {
                counter++;
            }
            if (table->rightGhost[1]) {
                counter++;
            }
        } else {
            if (table->rightGhost[i-1]) {
                counter++;
            }
            if (table->center[((i-1) * (TABLE_WIDTH - 2)) + (TABLE_WIDTH - 3)]) {
                counter++;
            }
            if (table->center[(i * (TABLE_WIDTH - 2)) + (TABLE_WIDTH - 3)]) {
                counter++;
            }
            if (i != (TABLE_WIDTH - 3)) {
                if (table->center[((i+1) * (TABLE_WIDTH - 2)) + (TABLE_WIDTH - 3)]) {
                    counter++;
                }  
                if (table->rightGhost[i+1]) {
                    counter++;
                } 
            } else {
                if (table->botGhost[TABLE_WIDTH - 3]) {
                    counter++;
                }
                if (table->se) {
                    counter++;
                }
            }
        }
        newTable->rightGhost[i] = determineValue(table->rightGhost[i], counter);
    }

    // determine center
    for (int i = 0; i < (TABLE_WIDTH - 2); i++) {
        for (int j = 0; j < (TABLE_WIDTH - 2); j++) {
            counter = 0;

            // handle the top/bot edges of the center: (tl, t, tr) and (bl, b, br)
            if (i == 0) {
                if (table->topGhost[j]) {
                    counter++;  
                }
                if (j > 0) {
                    if (table->topGhost[j-1]) {
                        counter++;
                    }
                }
                if (j < TABLE_WIDTH - 3) {
                    if (table->topGhost[j+1]) {
                        counter++;
                    }
                }
                if (j == 0) {
                    if (table->nw) {
                        counter++;
                    }
                }
                if (j == (TABLE_WIDTH - 3)){
                    if (table->ne) {
                        counter++;
                    }
                }
            } else if (i == (TABLE_WIDTH - 3)) {
                if (table->botGhost[j]) {
                    counter++;  
                }
                if (j > 0) {
                    if (table->botGhost[j-1]) {
                        counter++;
                    } 
                }
                if (j < TABLE_WIDTH - 3) {
                    if (table->botGhost[j+1]) {
                        counter++;
                    }
                }
                if (j == 0) {
                    if (table->sw) {
                        counter++;
                    }
                }
                if (j == (TABLE_WIDTH - 3)){
                    if (table->se) {
                        counter++;
                    }
                }
            }

            // handle left/right edges of center: (l, tl, bl) and (r, tr, br)
            if (j == 0) {
                if (table->leftGhost[i]) {
                    counter++;
                }
                if (i > 0) {
                    if (table->leftGhost[i-1]) {
                        counter++;
                    }
                }
                if (i < TABLE_WIDTH - 3) {
                    if (table->leftGhost[i+1]) {
                        counter++;
                    }
                }
            } else if (j == (TABLE_WIDTH - 3)) {
                if (table->rightGhost[i]) {
                    counter++;
                }
                if (i > 0) {
                    if (table->rightGhost[i-1]) {
                        counter++;
                    }
                }
                if (i < TABLE_WIDTH - 3) {
                    if (table->rightGhost[i+1]) {
                        counter++;
                    }
                }
            }

            // handle any center index for: bl, b, br
            // row below current index j: ((i+1) * (TABLE_WIDTH - 2))
            if (i != (TABLE_WIDTH - 3)) {
                if (j > 0) {
                    if (table->center[((i+1) * (TABLE_WIDTH - 2)) + j - 1]) {
                        counter++;
                    }
                }
                if (j < TABLE_WIDTH - 3) {
                    if (table->center[((i+1) * (TABLE_WIDTH - 2)) + j + 1]) {
                        counter++;
                    }
                }
                if (table->center[((i+1) * (TABLE_WIDTH - 2)) + j]) {
                    counter++;
                }
            
            }

            // handle any center index for: tl, t, tr
            // row above current index j: ((i-1) * (TABLE_WIDTH - 2))
            if (i != 0) {
                if (j > 0) {
                    if (table->center[((i-1) * (TABLE_WIDTH - 2)) + j - 1]) {
                        counter++;
                    }
                }
                if (j < TABLE_WIDTH - 3) {
                    if (table->center[((i-1) * (TABLE_WIDTH - 2)) + j + 1]) {
                        counter++;
                    }
                }
                if (table->center[((i-1) * (TABLE_WIDTH - 2)) + j]) {
                    counter++;
                }
            }

            // handle any center index for: l, r
            // current index: ((i * (TABLE_WIDTH - 2)) + j)  
            if (j > 0) {
                if (table->center[((i * (TABLE_WIDTH - 2)) + j) - 1]) {
                    counter++;
                }
            }

            if (j < (TABLE_WIDTH - 3)) {
                if (table->center[((i * (TABLE_WIDTH - 2)) + j) + 1]) {
                    counter++;
                }
            }
            newTable->center[(i * (TABLE_WIDTH - 2)) + j] = determineValue(table->center[(i * (TABLE_WIDTH - 2)) + j], counter);
        }
    }


    return newTable;
}

int determineValue(int currValue, int counter) {
    if ((counter < 2) || (currValue && (counter > 3))) {
        return 0;
    } else if ((currValue && (counter == 2 || counter == 3)) ||
    ((currValue == 0) && (counter == 3))) {
        return 1;
    } else {
        return 0;
    }
}

void cleanTable(struct MyTable *table) {
    free(table->topGhost);
    free(table->botGhost);
    free(table->leftGhost);
    free(table->rightGhost);
    free(table->center);
    free(table);
}
