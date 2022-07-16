import os

stdName = "A"
bfName = "B"
dirName = "pai"

os.system("g++ -std=c++20 -Wall {0:}.cpp -o std".format(stdName))
os.system("g++ -std=c++20 -Wall {0:}.cpp -o bf".format(bfName))

os.system("mkdir {0:}".format(dirName))
os.system("mv std {0:}".format(dirName))
os.system("mv bf {0:}".format(dirName))

tc = 0
while True:
    os.system("python gen.py > ./{0:}/in.in".format(dirName))
    os.system("time ./{0:}/std < ./{0:}/in.in > ./{0:}/std.out".format(dirName))
    os.system("./{0:}/bf < ./{0:}/in.in > ./{0:}/bf.out".format(dirName))
    if os.system("diff ./{0:}/bf.out ./{0:}/std.out".format(dirName)):
        print("WA")
        exit(0)
    else:
        tc += 1
        print("AC #", tc)
    
