//
//  2.c
//  2 Dynamic Allocation
//
//  Created by Amir Shavit on 11/11/2016.
//  Copyright © 2016 Amir Shavit. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define _CRT_SECURE_NO_WARNINGS
#define MALLOC_ERROR_CODE -1

#define MAX_MONOM_STRING_SIZE 100

typedef struct monom
{
    int coefficient;	//המקדם
    int power;		// החזקה
} Monom;

Monom* getPolynom(unsigned int *size);
void printPolySum(Monom *polynom1, const unsigned int polynom1Size, Monom *polynom2, const unsigned int polynom2Size);
void printPolyMul(Monom *polynom1, const unsigned int polynom1Size, Monom *polynom2, const unsigned int polynom2Size);

int main2()
{
    Monom *polynom1, *polynom2;             // The input polynoms
    unsigned int polynom1Size,polynom2Size; // The size of each polynom
    
    printf("Please enter the first polynom:\n");
    polynom1 = getPolynom(&polynom1Size);   // get polynom 1
    
    printf("Please enter the second polynom:\n");
    polynom2 = getPolynom(&polynom2Size);   // get polynom 2
    
    printf("The multiplication of the polynoms is:\n"); // print the multiplication
    printPolyMul(polynom1, polynom1Size, polynom2, polynom2Size);
    printf("\n");
    
    printf("The sum of the polynoms is:\n"); // print the sum
    printPolySum(polynom1, polynom1Size, polynom2, polynom2Size);
    printf("\n");
    
    free(polynom1); // releasing all memory allocations
    free(polynom2);
    
    return 0;
}

#pragma mark - Helper Functions

void mergeMonoms(Monom *monoms, unsigned int middle, unsigned int *size1, unsigned int *size2)
{
    Monom temp[*size1 + *size2];
    int left = 0, leftEnd = *size1;
    int right = middle, rightEnd = right + *size2;
    int i = 0;
    
    while ((left < leftEnd) && (right < rightEnd))
    {
        // Merge monoms of same power
        if ((monoms+left)->power == (monoms+right)->power)
        {
            // Skip if coeff zero each other
            int newCoeff = (monoms+left)->coefficient + (monoms+right)->coefficient;
            if (!newCoeff)
                break;
            
            (temp+i)->power = (monoms+left)->power;
            (temp+i)->coefficient = newCoeff;
            ++left;
            ++right;
        }
        else if ((monoms+left)->power > (monoms+right)->power)
        {
            (temp+i)->power = (monoms+left)->power;
            (temp+i)->coefficient = (monoms+left)->coefficient;
            ++left;
        }
        else
        {
            (temp+i)->power = (monoms+right)->power;
            (temp+i)->coefficient = (monoms+right)->coefficient;
            ++right;
        }
        ++i;
    }
    
    // Copy what is left
    while (left < leftEnd)
    {
        (temp+i)->power = (monoms+left)->power;
        (temp+i)->coefficient = (monoms+left)->coefficient;
        ++left;
        ++i;
    }
    
    while (right < rightEnd)
    {
        (temp+i)->power = (monoms+right)->power;
        (temp+i)->coefficient = (monoms+right)->coefficient;
        ++right;
        ++i;
    }
    
    // Reduce size returned for next recursive call
    *size1 = i;
    *size2 = 0;
    
    // Copy temp back
    for (i = 0; i < *size1; ++i)
    {
        *(monoms+i) = temp[i];
    }
}

void mergeSortMonoms(Monom *monoms, unsigned int *size)
{
    if (*size < 2)
        return;
    
    const unsigned int keepMiddle = *size / 2;
    unsigned int size1 = keepMiddle;
    unsigned int size2 = *size - keepMiddle;
    mergeSortMonoms(monoms, &size1);
    mergeSortMonoms(monoms + keepMiddle, &size2);

    mergeMonoms(monoms, keepMiddle, &size1, &size2);
    *size = size1 + size2;
}

void printPoly(Monom *polynom, const unsigned int polynomSize)
{
    int isFirst = 1;
    for (int i = 0; i < polynomSize; ++i)
    {
        Monom curr = *(polynom + i);
        
        if (isFirst)
        {
            // Skip + sign for fisrt monom
            isFirst = 0;
            printf("%d", curr.coefficient);
        }
        else
        {
            // Add plus sign for any monom except first and only for positive coeff.
            if (curr.coefficient > 0)
            {
                printf(" + %d", curr.coefficient);
            }
            else
            {
                printf(" - %d", -curr.coefficient);
            }
        }
        
        // Print power if any
        if (curr.power >= 2)
        {
            printf("x^%d", curr.power);
        }
        else if (curr.power == 1)
        {
            printf("x");
        }
    }
    
    printf("\n");
}

#pragma mark - Declared Functions

Monom* getPolynom(unsigned int *size)
{
    char numbers[MAX_MONOM_STRING_SIZE];
    gets(numbers);
    
    Monom* monoms = (Monom*)malloc(MAX_MONOM_STRING_SIZE * sizeof(Monom));
    if (!monoms)
    {
        printf("Allocation Error");
        exit(MALLOC_ERROR_CODE);
    }
    
    const char delimiters[8] = " \n\t";
    char *currNumber = strtok(numbers, delimiters);

    *size = 0;
    int coef, power;
    while (currNumber)
    {
        // Ignore monoms with 0 coeff.
        sscanf(currNumber, "%d", &coef);
        currNumber = strtok(NULL, delimiters);
        sscanf(currNumber, "%d", &power);
        currNumber = strtok(NULL, delimiters);

        if (coef)
        {
            (monoms + *size)->coefficient = coef;
            (monoms + *size)->power = power;
            *size += 1;
        }
    }

    mergeSortMonoms(monoms, size);
    
    // Reduce size
    monoms = realloc(monoms, *size * sizeof(Monom));
    
    return monoms;
}

void printPolySum(Monom *polynom1, const unsigned int polynom1Size, Monom *polynom2, const unsigned int polynom2Size)
{
    // Init a new array of monoms to hold both monoms and add them
    unsigned int size = polynom1Size + polynom2Size;
    Monom *united = (Monom*)malloc(sizeof(Monom) * size);
    if (!united)
    {
        printf("Allocation Error");
        exit(MALLOC_ERROR_CODE);
    }

    // Copy all monoms to temp array
    int i;
    for (i = 0; i < polynom1Size; ++i)
    {
        *(united + i) = *(polynom1 + i);
    }
    for (int j = 0; j < polynom2Size; ++i, ++j)
    {
        *(united + i) = *(polynom2 + j);
    }
    
    // merge and sort - it will also add same powers
    mergeSortMonoms(united, &size);
    
    printPoly(united, size);
    free(united);
}

void printPolyMul(Monom *polynom1, const unsigned int polynom1Size, Monom *polynom2, const unsigned int polynom2Size)
{
    // Init a new array of monoms to hold both monoms and add them
    unsigned int size = polynom1Size * polynom2Size;
    Monom *united = (Monom*)malloc(sizeof(Monom) * size);
    if (!united)
    {
        printf("Allocation Error");
        exit(MALLOC_ERROR_CODE);
    }
    
    // Multply every pair into temp array
    int i, j, pos = 0;
    for (i = 0; i < polynom1Size; ++i)
    {
        for (j = 0; j < polynom2Size; ++j, ++pos)
        {
            (united + pos)->coefficient = (polynom1 + i)->coefficient * (polynom2 + j)->coefficient;
            (united + pos)->power = (polynom1 + i)->power + (polynom2 + j)->power;
        }
    }

    // merge and sort - it will also add same powers
    mergeSortMonoms(united, &size);
    
    printPoly(united, size);
    free(united);
}

#pragma mark - Test functions

void testPrintMonom()
{
    Monom arr[4];
    arr[0].coefficient = 8;
    arr[0].power = 4;
    
    arr[1].coefficient = 7;
    arr[1].power = 3;
    
    arr[2].coefficient = -5;
    arr[2].power = 1;
    
    arr[3].coefficient = -8;
    arr[3].power = 0;
    
    printPoly(arr, 4);
    printf("8x^4 + 7x^3 - 5x - 8 Expected\n");
    
}

/*
 Test 1:
 
 Input:
 2   4   7   3
 -5   1   6   4
 
 Output:
 Please enter the first polynom:
 Please enter the second polynom:
 The multiplication of the polynoms is:
 12x^8 + 42x^7 - 10x^5 - 35x^4
 The sum of the polynoms is:
 8x^4 + 7x^3 - 5x
 
 */

/*
 Test 2:
 
 Input:
 2   4   -5   1   0   6   6   4   -8   0   7   3
 2   4   -5   1   0   6   6   4   -8   0   7   3
 
 Output:
 Please enter the first polynom:
 Please enter the second polynom:
 The multiplication of the polynoms is:
 64x^8 + 112x^7 + 49x^6 - 80x^5 - 198x^4 - 112x^3 + 25x^2 + 80x + 64
 The sum of the polynoms is:
 16x^4 + 14x^3 - 10x - 16
 
 */
