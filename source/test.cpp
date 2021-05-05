#include<string>
#include<iostream>
#include"transtabl.hpp"
#include"lexanalyzer.hpp"
#include"srcfhandler.hpp"
#include"transmachine.hpp"
int main() {

    using SmbEnt = TransTabl::SmbEnt;
    using TransM = TransMachine<TransTabl,int,SmbEnt>;
    
    SmbEnt smbent;
    std::string sname;

    std::cout << "Source File name : ";

    std::cin >> sname;

    LexAnalyzer<SmbEnt,TransM> lexanal(sname,TransTabl::Get());

    smbent.first = 0;
    while(smbent.first != TransTabl::Over) {
        smbent = lexanal.GetToken();
        lexanal.PrintEnt(smbent);
        if(smbent.first == 1) delete smbent.second;
    }

    return 0;
}
