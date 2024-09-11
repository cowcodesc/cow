#include <stdio.h>
#include "german_strings.h"

int main(void)
{
    german_string *str = new_string("Hello World");
    printf("%s\n", string(str));
    german_string *_str = new_string("HelLo World");
    printf("%s\n", string(_str));

    printf("on comparing: %s\n", ( string_cmp(str, _str) == 0 ? "both the strings are same" : "both the strings are different" ));

    delete(str);
    delete(_str);
}
