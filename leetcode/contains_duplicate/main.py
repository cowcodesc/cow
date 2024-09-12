'''
Given an integer array nums, return true if any value appears at least twice in the array, and return false if every element is distinct.

Input: nums = [1,2,3,1]

Output: true

Explanation:

The element 1 occurs at the indices 0 and 3.
'''
class Solution:
    def containsDuplicate(self, nums: list[int]) -> bool:
        _nums = set(nums)
        if len(nums) != len(_nums):
            return True
        return False

    def _containsDuplicate(self, nums: list[int]) -> bool:
        _nums = {}
        for num in nums:
            if _nums.get(num, 0) != 0:
                return True
            _nums[num] = 1
        return False
