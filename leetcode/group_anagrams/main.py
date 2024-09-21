'''
Given an array of strings strs, group the  anagrams together. You can return the answer in any order.

Example 1:

Input: strs = ["eat","tea","tan","ate","nat","bat"]

Output: [["bat"],["nat","tan"],["ate","eat","tea"]]

Explanation:

There is no string in strs that can be rearranged to form "bat".
The strings "nat" and "tan" are anagrams as they can be rearranged to form each other.
The strings "ate", "eat", and "tea" are anagrams as they can be rearranged to form each other.
'''

class Solution:
    def isAnagram(s1, s2) -> bool:
        if len(s1) != len(s2):
            return False
        _s1, _s2 = {}, {}
        for c1, c2 in zip(s1, s2):
            _s1[c1] = _s1.get(c1, 0) + 1
            _s2[c2] = _s2.get(c1, 0) + 1
        return _s1 == _s2

    def groupAnagrams(self, strs: list[str]) -> list[list[str]]:
        _map = {}
        for s in strs:
            for k in _map.keys():
                if isAnagram(s, k):
                    _map[k].append(s)
                    continue
            if s not in _map:
                _map[s] = []
            _map[s].append(s)

        group = []
        for v in _map.values():
            group.append(v)
        return group
