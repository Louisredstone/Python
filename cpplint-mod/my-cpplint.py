import re

def removeMultipleLinesComments(content):
    state=''
    result=''
    for i in range(len(content)):  # index of str
        c=content[i]
        if state=='':
            if(c=='/'):
                state='/'
            else:
                result+=c   # state = " "
        elif state=='/':
            if(c=='*'):
                state='/*'
            else:
                state=''
                result+='/'+c      # state = "/"
        elif state=='/*':
            if(c=='*'):
                state='/**'
        elif state=='/**':
            if(c=='/'):
                state=''
            elif(c!='*'):
                state='/*'
    return result

def removeSingleLineComments(lines):
    for i in range(len(lines)):
        # print(lines[i])
        # print(re.search('//',lines[i]))
        reResult=re.search('//',lines[i])  # re.search(pattern, string, flags=0)
        print (reResult)
        if(reResult!=None):
            singleLineCommentIndex=reResult.span()[0]
            lines[i]=lines[i][0:singleLineCommentIndex]
     
def removeEmptyLines(lines):
    i=0
    while i<len(lines):
        reResult=re.match(r'^[\s]*$',lines[i])   
        if(reResult==None):
            i+=1
        else:
            del lines[i]

def lint(filename):
    with open(filename,'r',encoding='gbk') as file:
        content=file.read()  # return str
        lines=removeMultipleLinesComments(content).split('\n')
        removeSingleLineComments(lines)
        removeEmptyLines(lines)
        #for line in lines:
          #  print(line)

testFile='GameScenes.h'

# lint(testFile)

s0="\s"
print(s0)
