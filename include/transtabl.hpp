_Pragma("once");

#include<map>
#include<string>
#include<utility>

class TransTabl {

  public:

    using SmbEnt = std::pair<int,std::string *>;
    //
    static const TransTabl & Get();

    //禁止拷贝与赋值，实际上状态转换表只需要一个实例
    TransTabl(const TransTabl &) = delete;

    TransTabl & operator = (const TransTabl &) = delete;

    //提供操作符[]以便访问转换表元素
    const char * operator [] (int i) const {
      return trans_tb[i];
    }

    //使用状态和符号构造符号项
    SmbEnt GetSmbEnt(int,const std::string &) const;

    //
    SmbEnt GetErrorEnt(int,int) const;

    //字符的类别，便于使用状态表进行转换
    int CharType(char) const;

    //判断一个状态是否是终止状态
    bool IsFinalSt(int) const;

    //判断一个状态是否是异常状态,
    bool IsErrorSt(int,int) const;

    bool IsErrorSt(const SmbEnt & ent) const {
      return IsErrorSt(ent.first,0);
    }

    //需要回退的状态
    bool RetractSt(int) const;  
    
    //二元式转换为字符串
    static std::string EntryString(const SmbEnt & ent);

    enum {
      Init = 0,
      ID = 1,         //ID
      INT = 3,        //integer value
      EQ = 5,         //=
      LE = 7,         //<=
      NE = 9,         //<>
      LT = 11,        //<
      GT,             //>     12
      GE,             //>=    13
      SUB,            //-     14
      MUL,            //*     15
      ASSIGN,         //:=    16
      LPAR,           //(     17
      RPAR,           //)     18
      SEM,            //;     19
      BEGIN,          //begin 20
      END,            //end   21
      INTEGER,        //integer22
      IF,             //if     23
      THEN,           //then   24
      ELSE,           //else   25
      FUNCTION,       //function26
      READ,           //read    27
      WRITE,          //write   28
      Over,
      Error
    };

  private:

    //禁止通过构造函数实例化
    TransTabl() = default;

    //标识符长度限制256字符
    enum {
      //状态数
      Statsnum = 22,
      //转换表列数  
      Columns = 14,
      //转换表行数
      Rows = 11,
      //非法字符错误
      IllegalC = 31,
      //冒号不匹配错误
      Notmatch = 32,
      //标识符长度溢出错误
      Overflow = 33,
      //标识符长度限制
      Tokenlens = 16
    };

    /*  实验要求词法的状态装换表，用二维度数组表示
     *  例如trans_tb[i][j]表示在状态i时接收j类
     *  字符所转变的下一状态
     */
    const char trans_tb[Rows][Columns] {\
      //state 0  - 初始状态，等待接收字符
      { 0, 2, 4, 5, 6, 8,14,15,10,17,18,19, 0,IllegalC},
      //state 1  - ID符号的终止状态
      { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,IllegalC},
      //state 2  - ID符号的中间状态
      { 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,IllegalC},
      //state 3  - INT符号的终止状态
      { 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,IllegalC},
      //state 4  - INT符号的中间状态
      { 4, 3, 4, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,IllegalC},
      //state 5  - '='的终止状态
      { 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,IllegalC},
      //state 6  - 接收到字符'<'
      { 6,11,11, 7,11, 9,11,11,11,11,11,11,11,IllegalC},
      //state 7  - '<='的终止状态
      { 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,IllegalC},
      //state 8  - 接收到字符'>'
      { 8,12,12,13,12,12,12,12,12,12,12,12,12,IllegalC},
      //state 9  - '<>'的终止状态
      { 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,IllegalC},
       //state 10 - 接收到字符':'
      {10,32,32,16,32,32,32,32,32,32,32,32,32,IllegalC}};
      //其余状态没有状态转变，因此不在表中出现

    //保留字与其编号对应
    const std::map<std::string,int> smbtabl {
      {"begin",BEGIN},
      {"end",END},
      {"integer",INTEGER},
      {"if",IF},
      {"then",THEN},
      {"else",ELSE},
      {"function",FUNCTION},
      {"read",READ},
      {"write",WRITE}
    };

};
