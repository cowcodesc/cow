'''
Given two strings s and t, return true if t is an anagram of s, and false otherwise.

Example 1:

Input: s = "anagram", t = "nagaram"

Output: true
'''

class Solution:
    def isAnagram(self, s: str, t: str) -> bool:
        if len(s) != len(t):
            return False

        s_map, t_map = {}, {}
        for _s, _t in zip(s, t):
            s_map[_s] = s_map.get(_s, 0) + 1
            t_map[_t] = t_map.get(_t, 0) + 1

        # for k, _s in s_map.items():
            # if t_map.get(k, 0) != _s:
                # return False
        # return True
        return t_map == s_map
