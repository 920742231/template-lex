_Pragma("once");

#include<iostream>
#include<string>
#include<fstream>

template<int num = 1>
class SrcFHandler {

  public:

    SrcFHandler(char * const);

    SrcFHandler(std::string const&);

    ~SrcFHandler();

    SrcFHandler(const SrcFHandler &) = delete;

    SrcFHandler & operator = (const SrcFHandler &) = delete;

    //回退一个字符
    void Retract();

    //输出缓冲区第一个字符
    char GetChar();

    //是否达到文件尾，包括缓冲区也为空
    bool over() const;

    //忽略空白字符
    void IgnoreSpace();

    //char operator++();

    //char operator++(int);

  private:

    //缓冲区大小
    enum {BufSize = 1 << 12};

    //标记文件是否读完
    bool isend;

    //使用缓冲区数量
    int bufnums {1 << num};

    //标记当前使用缓冲区
    int curbuf;

    //标记当前缓冲区读取位置
    int bufpos;

    //缓冲区保存字符数
    int buflen[1 << num];

    //源文件流，读取文件内容
    std::fstream srcfile;

    //缓冲区，由于空间可能较多，所以动态分配
    char * databuf[1 << num];

    //缓冲区是否为空
    bool BufEnd(int) const;

    //读取文件内容到缓冲区
    void ReadToBuf(int);

};

#include"functions.hpp"

//文件流打开文件，失败则退出。
template<int num>
SrcFHandler<num>::SrcFHandler(char * const path)
:isend(false),curbuf(0),bufpos(0),srcfile(path){
    //缓冲区大小为0，缓冲区空间暂不分配
    for(int i = 0;i < bufnums;++i) {
        buflen[i] = 0;
        databuf[i] = nullptr;
    }
}
template<int num>
SrcFHandler<num>::SrcFHandler(std::string const & path)
:isend(false),curbuf(0),bufpos(0),srcfile(path){
    //缓冲区大小为0，缓冲区空间暂不分配
    for(int i = 0;i < bufnums;++i) {
        buflen[i] = 0;
        databuf[i] = nullptr;
    }
}

//释放new分配的缓冲区内存
template<int num>
SrcFHandler<num>::~SrcFHandler() {
    for(int i = 0;i < bufnums;++i) 
        if(databuf[i]) delete [] databuf[i];
}

//缓冲区是否为空
template<int num>
bool SrcFHandler<num>::BufEnd(int i) const {
    //不是当前使用的缓冲区，缓冲内容是否为0
    if(i != curbuf) return !buflen[i];
    //当前缓冲，缓冲是否为空，或到达缓冲区尾
    return !buflen[i] || buflen[i] == bufpos;
}

//读取文件内容到缓冲区
template<int num>
void SrcFHandler<num>::ReadToBuf(int i) {
    
    //文件结束，放弃读取
    if(isend) return;
    
    //若缓冲区为分配空间，先分配
    if(databuf[i] == nullptr) 
        databuf[i] = new char[BufSize];
    
    //分配失败则放弃读取文件
    if(databuf[i] == nullptr) return;
    
    //读取文件内容到缓冲区中
    srcfile.read(databuf[i],BufSize);
    //设置缓冲区内容长度
    buflen[i] = srcfile.gcount();
    //读取长度小于需求长度，则文件达到文件尾
    if(buflen[i] != BufSize) isend = true;

    //

}

//回退一个字符，缓冲区读取指针减一
template<int num>
void SrcFHandler<num>::Retract() {
    if(bufpos) --bufpos;
}

//输出缓冲区第一个字符
template<int num>
char SrcFHandler<num>::GetChar() {
    //检测是否有内容可读
    if(over()) return 0;
    //当前缓冲区读取到尾，则读文件到缓冲区
    if(bufpos == buflen[curbuf]) {
        //读文件到缓冲
        ReadToBuf(curbuf);
        //从下一个缓冲中读取字符
        bufpos = 0;
        curbuf = (curbuf + 1) % bufnums;
        return GetChar();
    }
    else {
        //否则可直接读取字符
        return databuf[curbuf][bufpos++];
    }
}

//是否达到文件尾，包括缓冲区也为空
template<int num>
bool SrcFHandler<num>::over() const {
    //文件未读取完全，不空
    if(!isend) return false;
    //缓冲区不空
    for(int i = 0;i < bufnums;++i) 
        if(!BufEnd(i)) return false;
    //否则，源文件无额外内容可读
    return true;
}

//忽略空白字符
template<int num>
void SrcFHandler<num>::IgnoreSpace() {
    char c = ' ';
    //直到读取一个非空字符或文件读取结束
    while(!over() && is_space(c)) 
        c = GetChar();
    //读取到非空字符，缓冲区指针回退一个字符
    if(!is_space(c)) --bufpos;
    //否则文件读取结束，无需采取行动
}