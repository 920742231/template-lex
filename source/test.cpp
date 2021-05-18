#include<regex>
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
    
    regex pattern("([^.]+)[.]pas$");
    string dyd {"$1.dyd"} , err {"$1.err"};
    string path;
    
    cout << "Source File name : ";
    cin >> path;

    if(regex_match(path,pattern)) {
        this_lexcal lexanal(path,TransTabl::Get());
        lexanal.OutputResult(regex_replace(path,pattern,dyd),
            regex_replace(path,pattern,err));
    }
    else 
        cout << R"(Illegal file name [")" << path << R"("])" << '\n';

    return 0;
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
}
