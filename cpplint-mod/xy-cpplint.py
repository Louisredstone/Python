def RemoveMultiComments(contents):
    state=""
    result0=""
    for i in len(contents):
        if(state==""):
            
        elif(state)


def RemoveSingleComments():
    pass

def RemoveSpaceLine():
    pass

def main():
    with open(filename,'r+',encoding-'gbk') as file:   # ??''""
        contents=file.read()
        lines=RemoveMultiComments(contents)
        Lines_withoutcomt=RemoveSingleComments(lines)
        result=RemoveSpaceLine(Lines_withoutcomt)
        print(result)



if __name__ == "__main__":
        main()


