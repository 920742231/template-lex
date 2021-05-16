#include<string>
#include<iostream>
#include<algorithm>
#include"transtabl.hpp"
#include"lexanalyzer.hpp"
#include"transmachine.hpp"

using SmbEnt = TransTabl::SmbEnt;

int main() {

    using namespace std;
    using SmbEnt = TransTabl::SmbEnt;
    using TransM = TransMachine<TransTabl,int,SmbEnt>;
    using this_lexcal = LexAnalyzer<SmbEnt,TransM>;
    
    string sname;
    vector<SmbEnt> v;
    cout << "Source File name : ";

    cin >> sname;

    this_lexcal lexanal(sname,TransTabl::Get());

    lexanal.OutputResult("dyd.dyd","err.err");

    /*
    smbent.first = 0;
    while(smbent.first != TransTabl::Over) {
        smbent = lexanal.GetToken();
        lexanal.PrintEnt(smbent);
        if(smbent.first == 1) delete smbent.second;
    }

   */
  /*
    std::for_each(this_lexcal::lex_iterator(lexanal),
        this_lexcal::lex_iterator(),
        [&lexanal](SmbEnt & smbent) {
            std::cout << lexanal.EntryString(smbent) << '\n';
            if(smbent.first == 1) delete smbent.second;
        }
    );
*/
    return 0;
}
