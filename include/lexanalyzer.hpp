_Pragma("once");

#include"srcfhandler.hpp"
#include"functions.hpp"

#include<vector>
#include<iterator>

template<class Ent,
         class Transm>
class LexAnalyzer {

  public:
    //使用源文件名创建文件处理器，使用状态装换表创建状态转换机
    template<class Tab>
    LexAnalyzer(std::string & path,const Tab & trans_tb) 
    : transmachine(trans_tb), srcfhandler(path) {}

    //词法分析器不支持拷贝与赋值复制
    LexAnalyzer(const LexAnalyzer &) = delete;

    LexAnalyzer & operator = (const LexAnalyzer &) = delete;

    //源文件是否处理结束
    bool Isover() const {return over;}

    //获取下一个符号
    const Ent & GetToken();

    static std::string EntryString(const Ent & ent) {
      return Transm::EntryString(ent);
    }

    //对源文件进行完整的词法分析，二元式输出到指定文件，错误词法输出到指定文件
    bool OutputResult(const std::string &,const std::string &);
    
    bool OutputResult(const char * dy,const char * err) {
      return OutputResult(std::string(dy),std::string(err));
    }

  //词法分析器的迭代器，提供使用迭代器推动词法分析的能力
  class lex_iterator 
  : public std::iterator<std::input_iterator_tag,Ent> {

    using lex_type = LexAnalyzer<Ent,Transm>;
    public:
  
      //  空构造函数，生成迭代器的结束位置
      lex_iterator() 
      : lexanalyzer(0),current_ent(Ent()) {}

      //  生成指定词法分析器的迭代器
      lex_iterator(lex_type & lexanal)
      : lexanalyzer(&lexanal) {
        current_ent = lexanalyzer->GetToken();
      }

      //
      lex_iterator & operator ++ () {
        if(lexanalyzer == 0) {
          current_ent = Ent();
          return *this;
        }
        current_ent = lexanalyzer->GetToken();
        if(lexanalyzer->Isover()) 
          lexanalyzer = 0;
        return *this;
      }

      lex_iterator & operator ++ (int) {
        decltype(*this) tmp = this;
        ++*this;
        return this;
      }
  
      Ent & operator * () {
        return current_ent;
      }

      const lex_type * LexAddress () {
        return lexanalyzer;
      }

      bool operator != (lex_iterator & iter) {
        return lexanalyzer != iter.LexAddress() \
          || current_ent != *iter;
      }
  
    private:
      Ent current_ent;
      lex_type * lexanalyzer;
  };


  private:

    //上一个状态的二元式
    Ent last_entry;

    //状态转换机
    Transm transmachine;

    //源文件处理器
    SrcFHandler<> srcfhandler;

    //标志文件读取结束
    bool over {false};

};

//控制状态转换机进行状态转换，识别一个符号并输出二元式
template<class Ent,class Transm>
const Ent & LexAnalyzer<Ent,Transm>::GetToken() {

  //文件处理结束，输出结束标志项
  if(over) {
    last_entry = transmachine.OverEnt();
    return last_entry;
  };

  //分析前重置状态转换机
  transmachine.Reset();

  //除去符号开头的空白字符
  srcfhandler.IgnoreSpace();

  //未达到文件尾且当前状态不是结束状态，读取字符进行状态转换
  while(!transmachine.IsFinalSt() && !srcfhandler.over())
    transmachine.StateTrans(srcfhandler.GetChar());
  
  //当前状态是一个终止状态，则可以输出二元式，或者产生了异常，输出异常
  //状态的二元式。
  if(transmachine.IsFinalSt()) {
    //如果达到异常状态，进行处理
    if(transmachine.IsErrorSt()) {
      last_entry = transmachine.ErrorEnt();
      if(transmachine.Retract()) srcfhandler.Retract();
    }
    //否则是正常终止状态
    else {
      if(transmachine.Retract()) srcfhandler.Retract();
      last_entry = transmachine.GetEnt();
    }
  }
  
  //循环结束于源文件读取结束
  else {
    //状态机处于初始状态，说明源文件一分析结束
    if(transmachine.IsInitial()) {
      over = true;
      last_entry = transmachine.OverEnt();
      return last_entry;
    }
    //否则，若当前状态不是终止状态，输入一个空格保证状态完成转换
    if(!transmachine.IsFinalSt()) 
      transmachine.StateTrans(' ');
    //转换后达到异常状态，说明当前符号不是词法接收的符号
    if(transmachine.IsErrorSt())
      last_entry = transmachine.ErrorEnt();
    //转换达到正常结束状态，表示符号被接收，回退最后加入的空格
    else {
      transmachine.Retract();
      last_entry = transmachine.GetEnt();
    }
  }

  return last_entry;
}


/*  简单的文件流资源类，利用RAII机制获取文件流，避免复杂的
 *  错误处理。
 */
class OFileStream {
  public:
    OFileStream() = default;
    OFileStream(const std::string & path) 
    : file_stream(path,std::ios::out) {}
    ~OFileStream() {
      if(file_stream.is_open()) file_stream.close();
    }
    std::fstream & operator () () { return file_stream; }

  private:
    std::fstream file_stream;
};


/*  输出词法分析结果到指定文件中。  */
template<class Ent,class Transm>
bool LexAnalyzer<Ent,Transm>::OutputResult(
  const std::string & dy,const std::string & err) {
  using namespace std;
  
  //dyd_file 保存二元式的文件，err_file 保存错误信息的文件
  OFileStream dyd_file(dy), err_file(err);
  //任一文件打开失败则不能继续进行，函数退出
  if(!dyd_file().is_open()) {
    cout << "Connot open file [ " << dy << " ]\n";
    return false;
  }
  if(!err_file().is_open()) {
    cout << "Connot open file [ " << err << " ]\n";
    return false;
  }

  //表示异常的二元式输出字符串信息到err_file中，
  //正常的二元式输出二元式二进制数据到dyd_file中
  while(last_entry.first != transmachine.Over) {
    GetToken();
    if(transmachine.IsErrorSt(last_entry)) {
      err_file() << "***LINE:" << srcfhandler.GetLines();
      err_file() << " (\" " << transmachine.EntryString(last_entry);
      err_file() << " - " << transmachine.GetTStr() << " \")\n";
    }
    else dyd_file() << EntryString(last_entry) << '\n';

  }

  return true;

}