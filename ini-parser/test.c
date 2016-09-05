
#include "dictionary.h"
#include "iniparser.h"

int main() {
    int i;
    int sec_count;

    dictionary * ini = NULL ;
    ini = iniparser_load("sample.ini");

    sec_count = iniparser_getnsec(ini);
    for (i = 0; i < sec_count; i++) {
        printf("%s\n", iniparser_getsecname(ini, i));
    }

    return 0;
}
