from DPF import *

def foo(lst):
    result=''
    for i in lst:
        result+=str(i)
    return int(result)

if __name__ == '__main__':
    num = int(input('please input a number'))
    stack=[num]
    # while(True):
    for i in range(5):
        print(num)
        lst=DPF(num)
        print(lst)
        num=foo(lst)
        if(num in stack):
            break
        else:
            stack.append(num)
    print(stack)