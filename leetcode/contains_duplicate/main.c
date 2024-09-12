#include <stdio.h>
#include <stdlib.h>

/*
 * Given an integer array nums, return true if any value appears at least twice in the array, and return false if every element is distinct.
 *
 * Input: nums = [1,2,3,1]
 *
 * Output: true
 *
 * Explanation: The element 1 occurs at the indices 0 and 3.
 *
*/

int cmp(const void *a, const void *b) { return (*(int *)a - *(int *)b); }

bool containsDuplicate(int *nums, int numsSize)
{
    qsort(nums, (size_t)numsSize, sizeof(int), cmp);
    for (size_t i = 0; i < numsSize - 1; ++i)
        if (nums[i] == nums[i + 1]) return true;
    return false;
}
