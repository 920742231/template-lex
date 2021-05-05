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
    reinterpret_cast<std::string*>(t_atoi(symbol)));
    case EQ:    return SmbEnt(EQ,NULL);      // =
    case LE:    return SmbEnt(LE,NULL);      // <=
    case NE:    return SmbEnt(NE,NULL);      // <>
    case LT:    return SmbEnt(LT,NULL);      // <
    case GT:    return SmbEnt(GT,NULL);      // >
    case GE:    return SmbEnt(GE,NULL);      // >=
    case SUB:   return SmbEnt(SUB,NULL);     // -
    case MUL:   return SmbEnt(MUL,NULL);     // *
    case ASSIGN:return SmbEnt(ASSIGN,NULL);  // :=
    case LPAR:  return SmbEnt(LPAR,NULL);    // (
    case RPAR:  return SmbEnt(RPAR,NULL);    // )
    case SEM:   return SmbEnt(SEM,NULL);     // ;
    case Over:  return SmbEnt(Over,NULL);    //Over
    default:    break;
  }

  //否则,如果是保留字，则直接返回保留字编号
  iter = smbtabl.find(symbol);
  if(iter != smbtabl.end()) 
    return SmbEnt(iter->second,NULL);

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

//判断一个状态是否是异常状态
bool TransTabl::IsErrorSt(int st) const {
  //符号表中编号为Error的状态，表示有错误出现
  return st == Error;
}

/*
 *  有些状态会需要多读取一个字符才能识别，此时需要回
 *  退一个字符。
 */
bool TransTabl::RetractSt(int st) const {
  /*  需要回退的状态有四个，分别是
   *  1.标识符（包括保留字）
   *  2.整型数字
   *  3.<
   *  4.>
   */
  if(st==1 || st==3 || st==11 || st==12)
    return true;
  else return false;
}

//打印一个符号项
void TransTabl::PrintEnt(SmbEnt & ent) const {
  using namespace std;
  cout << setw(2) << ent.first << " : ";
  switch(ent.first) {
  case Over:      cout << "FILEEND\n";
                  break;
  case ID:        cout << *ent.second << "\n";
                  break;
  case INT:       cout << (long)ent.second << "\n";
                  break;
  case EQ:        cout << "=\n";
                  break;
  case LE:        cout << "<=\n";
                  break;
  case NE:        cout << "<>\n";
                  break;
  case LT:        cout << "<\n";
                  break;
  case GT:        cout << ">\n";
                  break;
  case GE:        cout << ">=\n";
                  break;
  case SUB:       cout << "-\n";
                  break;
  case MUL:       cout << "*\n";
                  break;
  case ASSIGN:    cout << ":=\n";
                  break;
  case LPAR:      cout << "(\n";
                  break;
  case RPAR:      cout << ")\n";
                  break;
  case SEM:       cout << ";\n";
                  break;
  case BEGIN:     cout << "BEGIN\n";
                  break;
  case END:       cout << "END\n";
                  break;
  case INTEGER:   cout << "INTEGER\n";
                  break;
  case IF:        cout << "IF\n";
                  break;
  case THEN:      cout << "THEN\n";
                  break;
  case ELSE:      cout << "ELSE\n";
                  break;
  case FUNCTION:  cout << "FUNCTION\n";
                  break;
  case READ:      cout << "READ\n";
                  break;
  case WRITE:     cout << "WRITE\n";
                  break;
  default:        break;
  }
}