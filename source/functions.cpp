#include<string>
#include<cstring>
#include<cstdlib>
#include<iostream>
void err_exit(const std::string & str) {
    std::cout << "wrong token - "<< str << '\n';
    exit(-1);
}

void err_open(const char * path) {
    std::cout << "Can not open file : " \
        << path << '\n';
    exit(-1);
}

bool is_space(char c) {
    return (c == ' ') || (c == '\n') \
    ||(c == '\r') || (c == '\t') || (!c);
}

long t_atol(const char * str) {
    int k,tmp,len;

    long value;
    
    
    k = 1;
    value = 0;
    len = strlen(str) - 1;
    for(int i=len;i >= 0;--i) {
        tmp = str[i] - 48;
        if(tmp < 0) return -1;
        value += tmp * k;
        k *= 10;
    }

    return value;
}

long t_atol(std::string const& str) {
    return t_atol(str.c_str());
}

bool is_letter(char c) {
    return (c >= 'A') && (c <= 'z');
}

bool is_number(char c) {
    return (c >= '0') && (c <= '9');
}