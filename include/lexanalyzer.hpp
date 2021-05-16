_Pragma("once");

#include"srcfhandler.hpp"
#include"functions.hpp"

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

    //打印符号条目信息
    void PrintEnt(Ent & ent) const {
      return transmachine.PrintEnt(ent);
    }

    void OutputResult(const std::string &,const std::string &);
    void OutputResult(const char * dy,const char * err) {
      return OutputResult(std::string(dy),std::string(err));
    }

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

//控制状态转换机进行状态转换，识别一个符号并输出符号项
template<class Ent,class Transm>
const Ent & LexAnalyzer<Ent,Transm>::GetToken() {
  
  //文件处理结束，输出结束标志项
  if(over) {
    last_entry = transmachine.OverEnt();
    return last_entry;
  };
  transmachine.Reset();
  //除去符号开头的空白字符
  srcfhandler.IgnoreSpace();

  //未达到文件尾且当前状态不是结束状态，读取字符进行状态转换
  while(!transmachine.IsFinalSt() && !srcfhandler.over())
    transmachine.StateTrans(srcfhandler.GetChar());
  
  
  if(transmachine.IsFinalSt()) {
    //如果达到异常状态，进行处理
    if(transmachine.IsErrorSt()) {
      last_entry = transmachine.ErrorEnt();
      if(transmachine.Retract()) srcfhandler.Retract();
    }
    else {
      if(transmachine.Retract()) srcfhandler.Retract();
      last_entry = transmachine.GetEnt();
    }
  }
  //file end
  else {
    if(transmachine.IsInitial()) {
      over = true;
      last_entry = transmachine.OverEnt();
    }
    else {
      if(!transmachine.IsFinalSt()) 
        transmachine.StateTrans(' ');
      if(transmachine.IsErrorSt())
        last_entry = transmachine.ErrorEnt();
      else {
        transmachine.Retract();
        last_entry = transmachine.GetEnt();
      }
    }
  }

  return last_entry;
}

template<class Ent,class Transm> class lex_iterator;

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


template<class Ent,class Transm>
void LexAnalyzer<Ent,Transm>::OutputResult(
  const std::string & dy,const std::string & err) {
  using namespace std;
  
  OFileStream dy_file(dy), err_file(err);
  if(!dy_file().is_open()) {
    cout << "Connot open file [ " << dy << " ]\n";
    return;
  }
  if(!err_file().is_open()) {
    cout << "Connot open file [ " << err << " ]\n";
    return;
  }

  while(last_entry.first != 29) {
      GetToken();
      if(transmachine.IsErrorSt(last_entry)) {
        err_file() << "__LINE__ " << srcfhandler.GetLines();
        err_file() << " \" " << transmachine.EntryString(last_entry);
        err_file() << " - " << transmachine.GetTStr() << " \"\n";
      }
      else {
        //cout << transmachine.EntryString(last_entry) << '\n';
        dy_file().write((const char *)&last_entry,sizeof(Ent));
      }
  }
  /*
  for_each(lex_iterator<Ent,Transm>(*this),
    lex_iterator<Ent,Transm>(),[&](Ent &) {
      GetToken();
      cout << last_entry.first << '\n';
      if(transmachine.IsErrorSt(last_entry)) {
        err_file() << "__LINE__ " << srcfhandler.GetLines();
        err_file() << " : " << transmachine.EntryString(last_entry);
        err_file() << " - " << transmachine.GetTStr() << '\n';
      }
      else dy_file().write((const char *)&last_entry,sizeof(Ent));
    });
*/
}


























#include<iterator>

template<typename Ent,
         typename Transm>
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

    bool operator != (lex_iterator<Ent,Transm> & iter) {
      return lexanalyzer != iter.LexAddress() \
        || current_ent != *iter;
    }
  
  private:
    Ent current_ent;
    lex_type * lexanalyzer;
};