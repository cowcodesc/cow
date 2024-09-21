#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#ifndef GERMAN_STRING_H

#define GERMAN_STRING_H

#define SHORT_STR_LEN 12
#define LONG_STR_PREFIX_LEN 4

/*
 * Short String Representation
 * | 32 bit | --96 bit = 12 chars-- |
 * |  len   |        content        |
 *
 * Long String Representation
 * | 32 bit | --32 bit = 4 chars-- | 64 bit |
 * |  len   |        prefix        |   ptr  |
*/

struct _short_string {
    uint32_t len;
    char str[SHORT_STR_LEN];
};

struct _long_string {
    uint32_t len;
    char prefix[LONG_STR_PREFIX_LEN];
    char *ptr;
};

typedef union german_string
{
    struct _short_string s_str;
    struct _long_string  l_str;
} german_string;

german_string *new_string(const char *str)
{
    uint32_t slen = (uint32_t)strlen(str);

    german_string *s = NULL;
    s = (german_string *)malloc(sizeof(german_string));
    s->s_str.len = slen;

    if (slen > SHORT_STR_LEN)
    {
        memcpy(s->l_str.prefix, str, LONG_STR_PREFIX_LEN);
        s->l_str.ptr = strndup(str, slen);

        return s;
    }

    memcpy(s->s_str.str, str, SHORT_STR_LEN);
    return s;
}

char *string(german_string *str)
{
    if (str->s_str.len > SHORT_STR_LEN)
        return str->l_str.ptr;
    return str->s_str.str;
}

int string_cmp(german_string *str1, german_string *str2)
{
    if (str1->s_str.len != str2->s_str.len) return -1;

    int cmp = memcmp(str1->l_str.prefix, str2->l_str.prefix, LONG_STR_PREFIX_LEN);
    if (cmp != 0) return cmp;

    if (str1->s_str.len > SHORT_STR_LEN)
        return strncmp(str1->l_str.ptr, str2->l_str.ptr, str1->l_str.len);
    return strncmp(str1->s_str.str, str2->s_str.str, str1->s_str.len);
}

void delete(german_string *str)
{
    if (str->s_str.len > SHORT_STR_LEN)
        free(str->l_str.ptr);
    free(str);
}

#endif
