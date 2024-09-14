#include <vector>
#include <map>

namespace std;

/*
 * Given an array of integers nums and an integer target, return indices of the two numbers such that they add up to target.
 *
 * You may assume that each input would have exactly one solution, and you may not use the same element twice.
 *
 * You can return the answer in any order.
 *
 * Example 1:
 * Input: nums = [2,7,11,15], target = 9
 * Output: [0,1]
 * Explanation: Because nums[0] + nums[1] == 9, we return [0, 1].
*/

class Solution
{
    public:
    vector<int> twoSum(vector<int>& nums, int target)
    {
        map<int, int> dict;
        vector<int> output = {};
        for (size_t i = 0; i < nums.size(); ++i)
        {
            if (dict.find(target - nums[i]) != dict.end()) 
            {
                output.push_back(dict[(target - nums[i])]), output.push_back(i);
                return output;
            }
            dict[nums[i]] = i;
        }
        return output; 
    }
};
