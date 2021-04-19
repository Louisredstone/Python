tcl='{ aaa bb "cc c" {dd d} {ee}} '
stack=[]
word=""
words=[]
for i in tcl:
    if len(stack)>0 and stack[-1]=='"':
        if i == '"':
            stack.pop()
            words.append(word)
            word=""
        else:
            word+=i
    else:
        if i == "{":
            stack.append(i)
        elif i == "}":
            if len(stack)==0 or stack.pop() != "{":
                raise "Error"
            if word != "":
                words.append(word)
                word=""
        elif i == '"':
            stack.append(i)
        else:
            if i == " ":
                if word != "":
                    words.append(word)
                    word=""
            else:
                word+=i
if len(stack)!=0:
    raise "Error"

print(words)