_Pragma("once");

#include<string>

template<typename Tab,
         typename St,
         typename Ent>
class TransMachine {

  public:

    using SmbEnt = std::pair<int,std::string *>;

    //使用状态转换表构造转换机实例
    TransMachine(const Tab & trans_tb);

    //删除拷贝与复制操作，转换机不支持复制
    TransMachine(const TransMachine &) = delete;

    TransMachine & operator = (const TransMachine &) = delete;


  public:

    //读取字符，转化到下一状态
    void StateTrans(char);

    //获取当前状态
    const St & GetCurSt() const {return cur_state;}

    //获取当前状态的字符串
    const std::string & GetTStr() const {return cur_str;}

    //当前状态是否是一个终止状态
    bool IsFinalSt() const {
      return trans_tb.IsFinalSt(cur_state);
    }

    //当前状态是否是一个异常状态
    bool IsErrorSt() const {
      return trans_tb.IsErrorSt(cur_state,length);
    }

    bool IsInitial() const {
      return cur_state == trans_tb.Init;
    }

    bool IsErrorSt(const Ent & ent) const {
      return trans_tb.IsErrorSt(ent);
    }
    //重置状态与字符串
    void Reset();

    //回退一个字符
    bool Retract();

    //状态与读取的字符串转换为符号项
    Ent GetEnt() const {
      return trans_tb.GetSmbEnt(cur_state,cur_str);
    }
    //结束标志项，表示状态转换结束
    Ent OverEnt() const {
      return trans_tb.GetSmbEnt(Tab::Over,cur_str);
    }
    Ent ErrorEnt() const {
      return trans_tb.GetErrorEnt(cur_state,length);
    }
    //打印一个符号项
    void PrintEnt(Ent & ent) const {
      return trans_tb.PrintEnt(ent);
    }
    
    std::string EntryString(const Ent & ent) const {
      return trans_tb.EntryString(ent);
    }
  private:

    //当前读取字符串长度
    int length;

    //当前状态
    St cur_state;

    //状态转换表
    const Tab & trans_tb;

    //当前读取的字符串
    std::string cur_str;

};

/*  模板参数分别为状态转换表类型，状态类型，符号项类型
 *  使用状态转换表的Init状态初始化转换的状态，读取的字
 *  串为空。
 */
template<class Tab,class St,class Ent>
TransMachine<Tab,St,Ent>::TransMachine(const Tab & tb)
: length(0),trans_tb(tb),cur_state(Tab::Init) {}

//  读取字符，进行状态转换。
template<class Tab,class St,class Ent>
void TransMachine<Tab,St,Ent>::StateTrans(char c) {
  cur_str.push_back(c);
  ++length;
  cur_state = trans_tb[cur_state][trans_tb.CharType(c)];
}

/*  重置状态转换机，当前状态设置为转换表的初始状态，
 *  清空读取的字符串。
 */
template<class Tab,class St,class Ent>
void TransMachine<Tab,St,Ent>::Reset() {
  cur_str.clear();
  length = 0;
  cur_state = trans_tb.Init;
}

//回退字符
template<class Tab,class St,class Ent>
bool TransMachine<Tab,St,Ent>::Retract() {
  if(!trans_tb.RetractSt(cur_state))
    return false;
  //否则，说明需要回退一个字符
  cur_str.pop_back();
  return true;
}
