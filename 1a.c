////
////  1a.c
////  2 Dynamic Allocation
////
////  Created by Amir Shavit on 11/11/2016.
////  Copyright © 2016 Amir Shavit. All rights reserved.
////
//// This program takes integer inputs from the user and sorts the pointers to them asc/desc
//// (Uses mege sort)
//
//#include <stdio.h>
//#include <stdlib.h>
//
//#define MALLOC_ERROR_CODE -1
//#define SIZE 100
//
//void printPointers(int ** pointers, int size);
//int** pointerSort(int* arr, unsigned int size, char ascend_flag);
//void mergeSort(int **pointers, unsigned int size, char ascend_flag);
//void mergeAsc(int **pointers, unsigned int size, unsigned int middle);
//void mergeDesc(int **pointers, unsigned int size, unsigned int middle);
//
///*
// Input:
//  10
// 1 7 4 9 -5 23 4 13 2 7
// 1
// */
//int main()
//{
//    unsigned int size, i;
//    int arr[SIZE];
//    int ** pointers;
//    int ascend_flag;
//    
//    printf("Please enter the number of items:\n");
//    scanf("%u", &size);
//    
//    for (i=0; i<size; i++)
//    {
//        scanf("%d", &arr[i]);
//    }
//    scanf("%d", &ascend_flag);
//    
//    pointers = pointerSort(arr, size, (char)ascend_flag);
//    printf("The sorted array:\n"); //Print the sorted array
//    printPointers(pointers, size);
//    free(pointers);
//    
//    return 0;
//}
//
//#pragma mark - Helpers
//
////int compareAsc(const void *a,const void *b) {
////    int *x = (int *) a;
////    int *y = (int *) b;
////    return *x - *y;
////}
////
////int compareDesc(const void *a,const void *b) {
////    int *x = (int *) a;
////    int *y = (int *) b;
////    return *y - *x;
////}
//
//#pragma mark - Declared functions
//
//int** pointerSort(int* arr, unsigned int size, char ascend_flag)
//{
//    // Create dynamic array and copy pointers
//    int **pointers = (int**)malloc(size * sizeof(int*));
//    if (!pointers)
//    {
//        printf("Allocation Error");
//        exit(MALLOC_ERROR_CODE);
//    }
//
//    for (int i = 0; i < size; ++i)
//    {
//        *(pointers + i) = arr + i;
//    }
//    
//    mergeSort(pointers, size, ascend_flag);
//    return pointers;
//}
//
//void mergeSort(int **pointers, unsigned int size, char ascend_flag)
//{
////    printPointers(pointers, size);
//    // If size is 0 or 1 the array is sorted already
//    if (size < 2)
//        return;
//    
//    // Recurssion - Sort both parts
//    unsigned int middle = size / 2;
//    mergeSort(pointers, middle, ascend_flag);
//    mergeSort(pointers + middle, size - middle, ascend_flag);
//    
//    // Merge 2 parts back according to ascending order
//    if (ascend_flag)
//    {
//        mergeAsc(pointers, size, middle);
//    }
//    else
//    {
//        mergeDesc(pointers, size, middle);
//    }
//}
//
//void mergeAsc(int **pointers, unsigned int size, unsigned int middle)
//{
//    int left = 0, right = middle, itr = 0;
//    int* temp[SIZE];
//    
//    // Compare next number in each part to find which is lower
//    while ((left < middle) && (right < size))
//    {
//        if (**(pointers + left) < **(pointers + right))
//        {
//            temp[itr] = *(pointers + left);
//            ++left;
//        }
//        else
//        {
//            temp[itr] = *(pointers + right);
//            ++right;
//        }
//        ++itr;
//    }
//    
//    // No no comparison between parts - Copy what is left in either parts
//    while (left < middle)
//    {
//        temp[itr] = *(pointers + left);
//        ++left;
//        ++itr;
//    }
//    
//    while (right < size)
//    {
//        temp[itr] = *(pointers + right);
//        ++right;
//        ++itr;
//    }
//    
//    // Copy temp back
//    for (itr = 0; itr < size; ++itr)
//    {
//        *(pointers + itr) = temp[itr];
//    }
//}
//
//void mergeDesc(int **pointers, unsigned int size, unsigned int middle)
//{
//    int left = 0, right = middle, itr = 0;
//    int* temp[SIZE];
//    
//    // Compare next number in each part to find which is higher
//    while ((left < middle) && (right < size))
//    {
//        if (**(pointers + left) > **(pointers + right))
//        {
//            temp[itr] = *(pointers + left);
//            ++left;
//        }
//        else
//        {
//            temp[itr] = *(pointers + right);
//            ++right;
//        }
//        ++itr;
//    }
//    
//    // No no comparison between parts - Copy what is left in either parts
//    while (left < middle)
//    {
//        temp[itr] = *(pointers + left);
//        ++left;
//        ++itr;
//    }
//    
//    while (right < size)
//    {
//        temp[itr] = *(pointers + right);
//        ++right;
//        ++itr;
//    }
//    
//    // Copy temp back
//    for (itr = 0; itr < size; ++itr)
//    {
//        *(pointers + itr) = temp[itr];
//    }
//}
//
//void printPointers(int ** pointers, int size)
//{
//    for (int i = 0; i < size; ++i)
//    {
//        printf("%d ", **(pointers + i));
//    }
//    printf("\n");
//}
//
