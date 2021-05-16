#include<iomanip>
#include<iostream>
#include"functions.hpp"
#include"transtabl.hpp"
/*
 *  静态Get函数提供状态转换表的单例引用，转换表无
 *  状态且不可修改。
 */
const TransTabl & TransTabl::Get() {
  static TransTabl transtabl;
  return transtabl;
}

/*
 *  使用状态标号和符号字符串构建符号项，主要是对于
 *  关键字的处理，如begin、end等。
 */
TransTabl::SmbEnt TransTabl::GetSmbEnt(int st,
  const std::string & symbol) const {
  
  decltype(smbtabl.end()) iter;

  //当前符号是一个操作符或界符，直接返回符号编号
  switch(st) {
    case INT:   return SmbEnt(INT,
      (std::string *)(t_atol(symbol)));
    case EQ:    return SmbEnt(EQ,nullptr);      // =
    case LE:    return SmbEnt(LE,nullptr);      // <=
    case NE:    return SmbEnt(NE,nullptr);      // <>
    case LT:    return SmbEnt(LT,nullptr);      // <
    case GT:    return SmbEnt(GT,nullptr);      // >
    case GE:    return SmbEnt(GE,nullptr);      // >=
    case SUB:   return SmbEnt(SUB,nullptr);     // -
    case MUL:   return SmbEnt(MUL,nullptr);     // *
    case ASSIGN:return SmbEnt(ASSIGN,nullptr);  // :=
    case LPAR:  return SmbEnt(LPAR,nullptr);    // (
    case RPAR:  return SmbEnt(RPAR,nullptr);    // )
    case SEM:   return SmbEnt(SEM,nullptr);     // ;
    case Over:  return SmbEnt(Over,nullptr);    //Over
    case Error: return SmbEnt(Error,nullptr);   //Error
    default:    break;
  }

  //否则,如果是保留字，则直接返回保留字编号
  iter = smbtabl.find(symbol);
  if(iter != smbtabl.end()) 
    return SmbEnt(iter->second,nullptr);

  //否则，则表示此符号是一个标识符
  return SmbEnt(st,new std::string(symbol));
}

/*
 *  字符的类别，便于使用状态表进行转换,字符类型与
 *  编号关系如下
 */
int TransTabl::CharType(char c) const {
  if(is_letter(c)) return 1;
  if(is_number(c)) return 2;
  if(is_space(c)) return 12;
  switch(c) {
    case '=': return 3;
    case '<': return 4;
    case '>': return 5;
    case '-': return 6;
    case '*': return 7;
    case ':': return 8;
    case '(': return 9;
    case ')': return 10;
    case ';': return 11;
    default : return 13;
  }
}

//判断一个状态是否是终止状态，终止状态对应符号被识别
bool TransTabl::IsFinalSt(int st) const {
  //终止状态为奇数编号或大于10的编号
  return st > 10 || st % 2;
}

//判断状态是否是一个异常状态
bool 
TransTabl::IsErrorSt(int st,int len) const {
  return len > Tokenlens || st == Overflow || \
    st == IllegalC || st == Notmatch;
}

//  输出异常提示信息，否则返回string {0}表示正常
TransTabl::SmbEnt 
TransTabl::GetErrorEnt(int st,int len) const {
  using namespace std;
  
  //标识符长度溢出
  if(len > Tokenlens) return make_pair(Overflow,new \
    string {"Overflow of declaration length(limit 256)"});
  switch (st) {
    //非法字符
    case IllegalC:
      return make_pair(IllegalC,new string {"Illegal character"});
    //冒号不匹配
    case Notmatch:
      return make_pair(Notmatch,new string {R"(Not matching for single ':')"});
    case Overflow:
       return make_pair(Overflow,new \
        string {"Overflow of declaration length(limit 256)"});
    //非异常状态
    default:
      return make_pair(0,reinterpret_cast<string *>(0));
  }
}

/*
 *  有些状态会需要多读取一个字符才能识别，此时需要回
 *  退一个字符。
 */
bool TransTabl::RetractSt(int st) const {
  /*  需要回退的状态有五个，分别是
   *  1.标识符（包括保留字）
   *  2.整型数字
   *  3.<
   *  4.>
   *  5.:[c]
   */
  if(st==1 || st==3 || st==11 || st==12 || st == Notmatch)
    return true;
  else return false;
}

//打印一个符号项
void TransTabl::PrintEnt(SmbEnt & ent) const {
  using namespace std;
  cout << EntryString(ent) << '\n';
}

#include<sstream>

std::string 
TransTabl::EntryString(const SmbEnt & ent) const {
  using namespace std;
  ostringstream outs;

  outs << setw(2) << ent.first << " : ";
  switch(ent.first) {
  case ID:        outs << *ent.second;
                  break;
  case INT:       outs << (long)ent.second;
                  break;
  case EQ:        outs << "=";
                  break;
  case LE:        outs << "<=";
                  break;
  case NE:        outs << "<>";
                  break;
  case LT:        outs << "<";
                  break;
  case GT:        outs << ">";
                  break;
  case GE:        outs << ">=";
                  break;
  case SUB:       outs << "-";
                  break;
  case MUL:       outs << "*";
                  break;
  case ASSIGN:    outs << ":=";
                  break;
  case LPAR:      outs << "(";
                  break;
  case RPAR:      outs << ")";
                  break;
  case SEM:       outs << ";";
                  break;
  case BEGIN:     outs << "BEGIN";
                  break;
  case END:       outs << "END";
                  break;
  case INTEGER:   outs << "INTEGER";
                  break;
  case IF:        outs << "IF";
                  break;
  case THEN:      outs << "THEN";
                  break;
  case ELSE:      outs << "ELSE";
                  break;
  case FUNCTION:  outs << "FUNCTION";
                  break;
  case READ:      outs << "READ";
                  break;
  case WRITE:     outs << "WRITE";
                  break;
  case Over:      outs << "EOF";
                  break;
  case Error:
  case IllegalC:
  case Notmatch:
  case Overflow:  outs << *ent.second;
  default:        break;
  }

  return outs.str();
}