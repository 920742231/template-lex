_Pragma("once");

#include<string>
void err_exit(const std::string &);
void err_open(const char *);
bool is_space(char);
bool is_letter(char);
bool is_number(char);
long t_atol(const char *);
long t_atol(const std::string &);