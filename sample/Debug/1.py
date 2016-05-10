# -*- coding: cp936 -*-
import commands
import time
import sys
import os

pin_pos = "C:\\Users\\wangchong\\Desktop\\pin-2.14-71313-msvc10-windows\\pin.exe"
my_pin_dll_pos = "C:\\Users\\wangchong\\Desktop\\pin-2.14-71313-msvc10-windows\\source\\tools\\MyPinTool\\Debug\\MyPinTool.dll"
file_pos = "C:\\Users\\wangchong\\Desktop\\sample\\Debug\\sample.exe"
in_file_pos = "C:\\Users\\wangchong\\Desktop\\sample\\Debug\\mid_in.txt"
out_file_pos = "C:\\Users\\wangchong\\Desktop\\sample\\Debug\\mid_out.txt"
result = "C:\\Users\\wangchong\\Desktop\\sample\\Debug\\result.txt"

cmd = pin_pos + " -t " + my_pin_dll_pos + " -- " + file_pos + " <" + in_file_pos
print cmd

pwd_len=10 #长度
pwd = ""
max_ins = 0xffffffff
res_k = 0

for i in range(1,pwd_len+1):
    pwd = pwd + 'a'

    in_object = open(in_file_pos,"w")
    in_object.write(pwd)
    in_object.close()

    #print cmd
    os.system(cmd)

    time.sleep(3)
    out_object = open(out_file_pos,"r")
    context = out_object.read()
    print context
    out_object.close()

    res_object = open(result,"a")
    res_object.write("len:" + str(len(pwd)) + "\n" + context + "\n\n")
    res_object.close()


    insnum = int(context.split()[7])
    if(i==1):
        max_ins = insnum
        res_k = i;
    else:
        if(max_ins<insnum):
            max_ins = insnum
            res_k=i
    print res_k
        
print "The len of flag is :" + str(res_k)

dic = ['!','"','#','$','%','&','\'','(',')','*','+',',','-','.','/','0','1',
'2','3','4','5','6','7','8','9',':',';','<','=','>','?','@','A','B','C','D',
'E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W',
'X','Y','Z','[','\\','_','`','a','b','c','d','e','f','g','h','i','j',
'k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z','{','|','}',
'~']

brute_x=''
known_pwd=""
for i in range(1,res_k + 1):
    for j in range(len(dic)):
        pwd = known_pwd + dic[j] + (res_k-len(known_pwd)-1)*'.'
        #print pwd

        in_object = open(in_file_pos,"w")
        in_object.write(pwd)
        in_object.close()

        os.system(cmd)

        time.sleep(3)
        out_object = open(out_file_pos,"r")
        context = out_object.read()
        #print context
        out_object.close()

        res_object = open(result,"a")
        res_object.write("pwd:" + (pwd) + "\n" + context + "\n\n")
        res_object.close()


        insnum = int(context.split()[7])
        if(j==0):
            max_ins = insnum
            brute_x = j;
        else:
            if(max_ins<insnum):
                max_ins = insnum
                brute_x = j
    print known_pwd
    known_pwd = known_pwd + dic[brute_x]
print known_pwd































        
        
    


