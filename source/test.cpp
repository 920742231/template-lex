#include<string>
#include<iostream>
#include<algorithm>
#include"transtabl.hpp"
#include"lexanalyzer.hpp"
#include"transmachine.hpp"
int main() {

    using SmbEnt = TransTabl::SmbEnt;
    using TransM = TransMachine<TransTabl,int,SmbEnt>;
    
    SmbEnt smbent;
    std::string sname,dname,errname;

    std::cout << "Source File name : ";

    std::cin >> sname;

    LexAnalyzer<SmbEnt,TransM> lexanal(sname,TransTabl::Get());

    std::cin >> dname;
    std::cin >> errname;

    lexanal.OutputResult(dname,errname);
    /*
    smbent.first = 0;
    while(smbent.first != TransTabl::Over) {
        smbent = lexanal.GetToken();
        lexanal.PrintEnt(smbent);
        if(smbent.first == 1) delete smbent.second;
    }

   */
  /*
    std::for_each(lex_iterator<SmbEnt,TransM>(lexanal),
        lex_iterator<SmbEnt,TransM>(),
        [&lexanal](SmbEnt & smbent) {
            lexanal.PrintEnt(smbent);
            if(smbent.first == 1) delete smbent.second;
        }
    );
*/
    return 0;
}
