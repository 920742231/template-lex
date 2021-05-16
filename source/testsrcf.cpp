#include"srcfhandler.hpp"

int main() {
    using namespace std;
    string path;
    cin >> path;
    SrcFHandler<6> f(path);
    char c = 0;
    do {
        c = f.GetChar();
        cout << c;
        if(c == '\n')
        cout << " __LINE__" << f.GetLines() << '\n';
        
    }
    while(c);
}