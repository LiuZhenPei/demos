#include <iostream>
#include <cstdio>
#include <cassert>
#include <string>
#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/writer.h"
#include "rapidjson/reader.h"

using namespace std;
using namespace rapidjson;

const char *json = "{"
    "\"Str_val\":\"Hello World\","
    "\"Bool_val\":true,"
    "\"Null_val\":null,"
    "\"Int_val\":1024,"
    "\"Float_val\":3.1416,"
    "\"Array_val\":[1,2,3,4]"
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

    cout<<"\n###Query DOM###"<<endl;
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
    assert(doc["Bool_val"].IsTrue());
    cout<<"Bool_val = ";
    doc["Bool_val"].GetBool()?(puts("True")):(puts("False"));
    //query null
    cout<<"Null_val = ";
    doc["Null_val"].IsNull()?(puts("null")):(puts("?"));
    //query int
    assert(doc.HasMember("Int_val"));
    assert(doc["Int_val"].IsNumber());
    assert(doc["Int_val"].IsInt());
    cout<<"Int_val = "<<doc["Int_val"].GetInt()<<endl;
    //query float
    assert(doc.HasMember("Float_val"));
    assert(doc["Float_val"].IsNumber());
    assert(doc["Float_val"].IsFloat());
    cout<<"Float_val = "<<doc["Float_val"].GetFloat()<<endl;
    //query array
    assert(doc.HasMember("Array_val"));
    assert(doc["Array_val"].IsArray());
    const Value &arr = doc["Array_val"];
    for(SizeType i = 0;i < arr.Size();i++)
        cout<<"Array_val["<<i+1<<"] = "<<arr[i].GetInt()<<endl;
    //Query object
    static const char* kTypeNames[] =
            { "Null", "False", "True", "Object", "Array", "String", "Number" };
    Value::ConstMemberIterator iter;
    for(iter = doc.MemberBegin();iter < doc.MemberEnd();iter++)
        cout<<"Type of member "<<iter->name.GetString()<<" is "
            <<kTypeNames[iter->value.GetType()]<<endl;

    cout<<"\n###Modify DOM###"<<endl;
    //modify Str_val
    doc["Str_val"] = "Modify Str_val"; 
    assert(doc.HasMember("Str_val"));
    assert(doc["Str_val"].IsString());
    cout<<"Str_val = "<<doc["Str_val"].GetString()<<endl;
    cout<<"The length of Str_val = "<<doc["Str_val"].GetStringLength()<<endl;
    //add value to Array_val
    Value &a = doc["Array_val"];
    Document::AllocatorType &allocator = doc.GetAllocator();
    for(int i = 5;i <= 7;i++)
        a.PushBack(i,allocator);
    assert(doc.HasMember("Array_val"));
    assert(doc["Array_val"].IsArray());
    for(SizeType i = 0;i < a.Size();i++)
        cout<<"Array_val["<<i+1<<"] = "<<a[i].GetInt()<<endl;
    //add a string to DOM
    Value Name;
    char buf[10];
    int len = sprintf(buf,"%s","Allen");
    Name.SetString(buf,static_cast<SizeType>(len),doc.GetAllocator());
    doc.AddMember("Author",Name,doc.GetAllocator());

    cout<<"\n###Reformat JSON###"<<endl;
    StringBuffer sb;
    PrettyWriter<StringBuffer> Writer(sb);
    doc.Accept(Writer);
    puts(sb.GetString());
}
