/*
 * =====================================================================================
 *
 *       Filename:  t1.c
 *
 *        Version:  1.0
 *        Created:  2017年04月19日 23时14分53秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "cJSON.h"

char *Create_json();

int main(int argc, const char *argv[]){
    printf("\n************************\n");
    printf("******Create json*******");
    printf("\n************************\n");
    char *str = Create_json();
    printf("\n*************************\n");
    printf("********Read json********");
    printf("\n*************************\n");

    return 0;
}

char *Create_json(){
    cJSON *root, *sub1, *sub2;

    root = cJSON_CreateObject();
    sub1 = cJSON_CreateObject();
    sub2 = cJSON_CreateObject();

    assert(root);
    assert(sub1);
    assert(sub2);

    cJSON_AddStringToObject(root,"Country","China");
    cJSON_AddItemToObject(root,"Persion_Information",sub1);
    cJSON_AddStringToObject(sub1,"name","XiaoMing");
    cJSON_AddNumberToObject(sub1,"age",18);
    cJSON_AddFalseToObject(sub1,"Married");
    cJSON_AddItemToObject(sub1,"body",sub2);
    cJSON_AddNumberToObject(sub2,"Hight",170);
    cJSON_AddNumberToObject(sub2,"Wight",58);

    char *s = cJSON_Print(root);
    assert(s);
    printf("%s\n",s);

    return s;
}
