#! /usr/bin/python
# Decompose prime factors
import math
def DPF(num,lst=[]):
    # print('*'*30)
    isPrime = True
    i = 2
    square = int(math.sqrt(num)) + 1
    while i <= square:
        if num % i == 0:
            lst.append(i)
            isPrime = False
            DPF(int(num / i),lst)
            i += 1
            break
        i += 1
    if isPrime:
        lst.append(num)
    return lst

def DPF2(num):
    result=[]
    flag=True
    while(flag):
        i=2
        

if __name__ == '__main__':
    for i in range(3):
        number = int(input("Enter a number: "))
        print(DPF2(number))