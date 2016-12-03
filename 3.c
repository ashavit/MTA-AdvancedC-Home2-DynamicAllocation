//
//  3.c
//  2 Dynamic Allocation
//
//  Created by Amir Shavit on 11/11/2016.
//  Copyright © 2016 Amir Shavit. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MALLOC_ERROR_CODE -1
#define MAX_LINE_LENGTH 100

void history();

int main()
{
    history();  // Start the history() function
    return 0;
}

#pragma mark - Helper Functions

int MAX(int a, int b)
{
    if (a > b)
        return a;
    else
        return b;
}

int MIN(int a, int b)
{
    if (a < b)
        return a;
    else
        return b;
}

void initHistoryArray(char ***lines, unsigned int size)
{
    *lines = (char**)malloc(size * sizeof(char*));
    if (*lines == NULL)
    {
        printf("Error allocating history array");
        exit(MALLOC_ERROR_CODE);
    }
}

void reduceOldestLine(char **lines, unsigned int maxSize)
{
    // Free oldest line
    free(lines[0]);
    
    // Override its' spot
    for (int i = 1; i < maxSize; ++i)
    {
        lines[i-1] = lines[i];
    }
}

void storeLine(char **lines, char* newLine, unsigned int *counter, unsigned int maxSize)
{
    // Make sure line has length
    if (!strlen(newLine))
        return;
    
    int pos = *counter;
    if (*counter >= maxSize)
    {
        // Remove last line and swap down
        reduceOldestLine(lines, maxSize);
        pos = maxSize - 1;
    }
    
    lines[pos] = newLine;
    ++(*counter);
}

#pragma mark ! Command functions

void restoreLine(char **lines, unsigned int lineNum ,unsigned int *counter, unsigned int maxSize)
{
    // Make sure line exists in history
    if (lineNum > *counter || (*counter >= maxSize && lineNum < *counter - maxSize + 1))
    {
        printf("Could not restore line number %d, currently lines %d-%d are in the storage\n", lineNum, MAX(*counter - maxSize + 1, 1), *counter);
        return;
    }
    
    unsigned int pos = (*counter > maxSize ? maxSize - (*counter - lineNum) - 1 : lineNum - 1);
    char *newLine = strdup(lines[pos]);
    storeLine(lines, newLine, counter, maxSize);
    printf("%s\n", newLine);
}

void restoreLastLineNumber(char **lines, unsigned int *counter, unsigned int maxSize)
{
    char line[MAX_LINE_LENGTH];
    sprintf(line, "Number of last line is %d", *counter);
    
    char *newLine = strdup(line);
    storeLine(lines, newLine, counter, maxSize);
    printf("%s\n", newLine);
}

void restoreLineWithPrefix(char **lines, char* prefix, unsigned int *counter, unsigned int maxSize)
{
    int found = 0;
    int pos = MIN(maxSize - 1, *counter - 1);
    char* ptr;
    
    while (pos >= 0 && !found)
    {
        ptr = strstr(lines[pos], prefix);
        if (ptr == lines[pos])
        {
            found = 1;
            break;
        }
        --pos;
    }
    
    // Duplicate if found
    if (found)
    {
        char *newLine = strdup(lines[pos]);
        storeLine(lines, newLine, counter, maxSize);
        printf("%s\n", newLine);
    }
    else
    {
        // Print error if not found
        printf("Prefix '%s' not found\n", prefix);
    }
    
}

void restoreLineNotContaining(char **lines, char* partial, unsigned int *counter, unsigned int maxSize)
{
    int found = 1;
    int pos = MIN(maxSize - 1, *counter - 1);
    char* ptr;
    
    while (pos >= 0 && found)
    {
        ptr = strstr(lines[pos], partial);
        if (!ptr)
        {
            found = 0;
            break;
        }
        --pos;
    }
    
    // Duplicate if found
    if (!found)
    {
        char *newLine = strdup(lines[pos]);
        storeLine(lines, newLine, counter, maxSize);
        printf("%s\n", newLine);
    }
    else
    {
        // Print error if not found
        printf("String '%s' was found in all lines\n", partial);
    }
    
}

void printHistory(char **lines, unsigned int counter, unsigned int maxSize)
{
    int par = (counter < maxSize ? 1 : counter - maxSize + 1);
    for (int i = 0; i < maxSize && par <= counter; ++i, ++par)
    {
        printf("%d. %s\n", par, lines[i]);
    }
}

// Return 1 to continue program, 0 to stop
int runCommand(char **lines, char* command, unsigned int *counter, unsigned int maxSize)
{
    int result = 1;
    
    char* comm = command + 1;
    if (strcmp(comm, "quit") == 0)
    {
        printf("Thanks for using the history application, bye bye\n");
        result = 0;
    }
    else if (strcmp(comm, "print") == 0)
    {
        printHistory(lines, *counter, maxSize);
    }
    else if (strcmp(comm, "L") == 0)
    {
        restoreLastLineNumber(lines, counter, maxSize);
    }
    else if (strcmp(comm, "!") == 0)
    {
        restoreLine(lines, *counter, counter, maxSize);
    }
    else if (comm[0] >= '1' && comm[0] <= '9') // Check if the command is a line number
    {
        int lineNum;
        sscanf(comm, "%d", &lineNum);
        restoreLine(lines, lineNum, counter, maxSize);
    }
    else if (comm[0] == '~')
    {
        restoreLineNotContaining(lines, comm + 1, counter, maxSize);
    }
    else
    {
        restoreLineWithPrefix(lines, comm, counter, maxSize);
    }
    
    return result;
}

void freeHistory(char **history, unsigned int counter, unsigned int size)
{
    unsigned stop = MIN(counter, size);
    for (int i = stop - 1; i >= 0; --i)
    {
        free(history[i]);
    }
    
    free(history);
}

#pragma mark - Decalred Functions

void history()
{
    int size;
    printf("Please enter the history storage size\n");
    scanf("%d", &size);
    printf("The storage size was set to %d\n", size);
    
    char **history;
    initHistoryArray(&history, size);
    
    char input[MAX_LINE_LENGTH];
    unsigned int count = 0;
    int inProgress = 1;
    
    while (inProgress)
    {
        gets(input);
        
        if (input[0] == '!')
        {
            inProgress = runCommand(history, input, &count, size);
        }
        else
        {
            char *newLine = strdup(input);
            storeLine(history, newLine, &count, size);
        }
    }
    
    freeHistory(history, count, size);
}

/*
Test 1: Input:
 7
 some text
 !!
 !quit

Output:
 Please enter the history storage size
 The storage size was set to 7
 some text
 Thanks for using the history application, bye bye
*/

/* Test 2: Input: 
 5
 some text
 some more text
 and more text
 !2
 hello world
 !!
 !!
 !2
 !print
 !~hello
 !some
 !L
 !quit

Output:
 Please enter the history storage size >
 The storage size was set to 5
 some more text
 hello world
 hello world
 Could not restore line number 2, currently lines 3-7 are in the storage
 3. and more text
 4. some more text
 5. hello world
 6. hello world
 7. hello world
 some more text
 some more text
 Number of last line is 9
 Thanks for using the history application, bye bye
*/
