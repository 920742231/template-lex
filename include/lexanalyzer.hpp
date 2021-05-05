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
    bool Isover() const {return srcfhandler.over();}

    //获取下一个符号
    Ent GetToken();

    //打印符号条目信息
    void PrintEnt(Ent & ent) const {
      return transmachine.PrintEnt(ent);
    }

  private:

    //状态转换机
    Transm transmachine;

    //源文件处理器
    SrcFHandler<> srcfhandler;

    //标志文件读取结束
    bool over {false};

};

//控制状态转换机进行状态转换，识别一个符号并输出符号项
template<class Ent,class Transm>
Ent LexAnalyzer<Ent,Transm>::GetToken() {
  
  Ent ent;

  //文件处理结束，输出结束标志项
  if(over) return transmachine.OverEnt();

  //除去符号开头的空白字符
  srcfhandler.IgnoreSpace();

  //未达到文件尾且当前状态不是结束状态，读取字符进行状态转换
  while(!transmachine.IsFinalSt() && \
  !srcfhandler.over() && !transmachine.IsErrorSt())
    transmachine.StateTrans(srcfhandler.GetChar());
  
  //如果达到异常状态，进行处理
  if(transmachine.IsErrorSt())
    err_exit(transmachine.GetTStr());

  //如果达到终止状态，输出符号项
  if(transmachine.IsFinalSt()) {
    //是否需要回退字符
    if(transmachine.Retract())
      srcfhandler.Retract();
    
    ent = transmachine.GetEnt();
    //重置状态转换机
    transmachine.Reset();
  }

  //文件读取结束，无额外字符进行状态转换
  else if(srcfhandler.over()) {
    //文件处理结束,设置标志
    over = true;
    if(transmachine.IsInitial())
      return transmachine.OverEnt();
    //传入一个空格进行状态转换，保证状态转换完成
    transmachine.StateTrans(' ');
    //如果达到终止状态，输出符号项
    if(transmachine.IsFinalSt())
      ent = transmachine.GetEnt();
    //否则，表示出现了错误
    err_exit(transmachine.GetTStr());
  }

  return ent;
}