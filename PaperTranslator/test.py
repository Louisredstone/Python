from Py4Js import *

def google_translate(content):
    '''实现谷歌的翻译'''
    js = Py4Js()
    tk = js.getTk(content)
