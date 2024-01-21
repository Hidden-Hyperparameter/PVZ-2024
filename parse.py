with open("debug.log","rb") as f:
    bytes=f.read()
st = bytes.decode()
lines = []
a=0
b=0
while(b<len(st)):
    # print(f"b = {b}")
    if(st[b]=='\n'):
        lines.append(st[a:b-1])
        # print(f"lines append {st[a:b-1]}")
        a=b+1
    b=b+1
lines.append(st[a:b-1])
# print(f"len is {len(lines)}")
# exit(1)
cord=[]
for line in lines:
    newline=""
    for c in line:
        if((('0'<=c)&(c<='9')) |(('a'<=c)&(c<='z'))| (c==',')):
            newline+=c
    # print(f"newline is {newline}")
    if(newline[:len("userclick")]=="userclick"):
        # print(f"find true line {newline}")
        pos=newline.find(',')
        x=newline[len("userclick"):pos]
        y=newline[pos+1:]
        if(not [x,y] in cord):
            cord.append([x,y])
print(cord)