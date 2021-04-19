#include <iostream>
#include <string>
#include <vector>
using namespace std;

void splitTclList(string name,vector<string>& list){
    string stack="";
    string word="";
    for(char ch : name){
        if(stack.size()>0 && stack.back()=='"'){//如果当前状态是引号内部
            if(ch=='"'){//如果又遇到一个'"'，说明这是右引号
                stack.pop_back();//旧的左引号弹出
                list.push_back(word);//list收集当前word
                word.clear();//当前word清空
            }
            else{//如果没有遇到引号
                word.push_back(ch);//将字符加入word
            }
        }
        else{//如果当前状态不在引号内部
            switch (ch)
            {
            case '{'://如果当前字符为左大括号，放入stack
                //拿到左大括号时，有可能是上一词的结尾
                if(!word.empty()){//如果word不为空
                    list.push_back(word);//list收集word
                    word.clear();//word清空
                }
                stack.push_back(ch);
                break;
            case '}'://如果当前字符为右大括号，尝试取出stack尾部的左大括号
                if(stack.empty() || stack.back()!='{'){
                    //如果stack为空，或者stack不为空但尾部不为左大括号，说明字符串存在语法错误
                    throw "Error";
                }
                stack.pop_back();//取出stack尾部的左大括号
                //拿到右大括号时有可能是上一词的结尾
                if(!word.empty()){//如果word不为空
                    list.push_back(word);//list收集word
                    word.clear();//word清空
                }
                break;
            case '"'://如果当前字符为引号，放入stack
                stack.push_back(ch);
                break;
            default://如果当前字符为一般字符
                if(ch==' '){//如果当前字符为空格，则有可能是上一词的结尾
                    if(!word.empty()){//如果word不为空
                        list.push_back(word);//list收集word
                        word.clear();//word清空
                    }
                }
                else
                {//当前字符为非空格的一般字符
                    word.push_back(ch);//将ch存入word
                }
                break;
            }
        }
    }
    if(!stack.empty()){
        //如果上述循环结束时stack不为空，意味着字符串存在语法错误。
        throw "Error";
    }
}

int main(){
    string str="{      aaa   bb \"cc c\"       {dd   d} {ee}} ";
    vector<string> words;
    splitTclList(str,words);

    cout<<"Hello World\n"<<endl;
    for(auto word:words){
        cout<<word<<endl;
    }
}