# -*- coding:utf-8 -*-
import pyperclip as ppc
import re
import time

while(True):
    # _ = input('(按 Enter 以开始处理剪贴板中的文字)')
    time.sleep(1)
    s = ppc.paste()
    print('处理前：')
    print(s)
    print('')
    s = s.replace('w.r.t','with respect to')
    s = re.sub(r'-\n','',s)
    s = re.sub(r'[\r\b]','',s)
    s = re.sub(r'[\n\t]',' ',s)
    # s = re.sub(r'[^A-Za-z0-9- .,!?(%)\[\]:;\'\"αβγ]','',s)
    s = re.sub(r'[ ]+',' ',s)
    ppc.copy(s)
    print('处理后：')
    print(ppc.paste())
    print('(内容已复制到剪贴板)')

