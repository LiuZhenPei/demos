#include <iostream>
#include <cstdio>
#include <cassert>
#include <string>
#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"

using namespace std;
using namespace rapidjson;

const char *json = "{"
    "\"Str_val\":\"Hello World\","
    "\"Bool_val\":true,"
    "\"Null_val\":null,"
    "\"Int_val\":1024,"
    "\"Float_val\":3.1416,"
    "\"Array_val\":[1,2,3,4,5]"
"}";

void basic_usage(const char *data);

int main(){
    cout<<"******************************"<<endl;
    cout<<"******Basic Usage Of DOM******"<<endl;
    cout<<"******************************"<<endl;
    basic_usage(json);
    return 0;
}


void basic_usage(const char *data){
    Document doc;
    //parser json
    doc.Parse(data);
    //query object
    assert(doc.IsObject());
    //query string
    assert(doc.HasMember("Str_val"));
    assert(doc["Str_val"].IsString());
    cout<<"Str_val = "<<doc["Str_val"].GetString()<<endl;
    cout<<"The length of Str_val = "<<doc["Str_val"].GetStringLength()<<endl;
    //query bool
    assert(doc.HasMember("Bool_val"));
    assert(doc["Bool_val"].IsBool());
    cout<<"Bool_val = ";
    doc["Bool_val"].GetBool()?(puts("True")):(puts("False"));
    //query null
    cout<<"Null_val = ";
    doc["Null_val"].IsNull()?(puts("null")):(puts("?"));

}
