import os

def pai():
    # UPDATE HERE
    os.system("g++-13 -std=c++20 -Wall guess.cpp -o std")
    os.system("g++-13 -std=c++20 -Wall bf.cpp -o bf")

    tc = 0
    while True:
        os.system("python3 gen.py >1.in")
        os.system("time ./std <1.in >std.out")
        os.system("./bf <1.in >bf.out")
        if os.system("diff bf.out std.out"):
            print("WA")
            exit(0)
        else:
            tc += 1
            print("AC #", tc)

def clear():
    os.system("rm std bf std.out bf.out 1.in")
    

if __name__ == "__main__":
    pai()
    clear()