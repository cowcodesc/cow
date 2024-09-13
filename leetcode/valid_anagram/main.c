#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Given two strings s and t, return true if t is an anagram of s, and false otherwise.
 *
 * Example 1:
 *
 * Input: s = "anagram", t = "nagaram"
 *
 * Output: true
*/

int cmp(const void *a, const void *b) { return *(char *)a - *(char *)b; }

bool isAnagram(char *s, char *t)
{
    size_t s_len = strlen(s), t_len = strlen(t);

    if (s_len != t_len) return false;

    qsort(s, s_len, sizeof(char), cmp);
    qsort(t, t_len, sizeof(char), cmp);

    for (size_t i = 0; i < s_len; ++i)
        if (s[i] != t[i]) return false;
    return true;
}

bool isAnagramBetter(char *s, char *t)
{
    int map[26] = { 0 };
    size_t s_len = strlen(s), t_len = strlen(t);

    if (s_len != t_len) return false;

    for (size_t i = 0; i < s_len; ++i)
        map[s[i] - 'a']++, map[t[i] - 'a']--;

    for (size_t k = 0; k < 26; ++k)
        if (map[k] != 0) return false;
    return true;
}
