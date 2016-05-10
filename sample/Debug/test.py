# -*- coding: cp936 -*-
import commands
import time
import sys
import os

dic = ['!','"','#','$','%','&','\'','(',')','*','+',',','-','.','/','0','1',
'2','3','4','5','6','7','8','9',':',';','<','=','>','?','@','A','B','C','D',
'E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W',
'X','Y','Z','[','\\','_','`','a','b','c','d','e','f','g','h','i','j',
'k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z','{','|','}',
'~']

res_k = 8
known_pwd=""
for i in range(1,res_k + 1):
    for j in range(len(dic)):
        pwd = known_pwd + dic[j] + (res_k-len(known_pwd)-1)*'.'
        print pwd
        x=j
    known_pwd = known_pwd + dic[5]
