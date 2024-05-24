import os
from cyaron import *
from tqdm import tqdm

def make():
    data = IO(file_prefix="", data_id=1)
    n = randint(5, 10)
    data.input_writeln(n)
    
    return data

if __name__ == "__main__":
    # UPDATE HERE
    os.system("g++-13 -std=c++20 -Wall std.cpp -o std -O2")
    os.system("g++-13 -std=c++20 -Wall bf.cpp -o bf -O2")
    
    for _ in tqdm(range(100)):
        try:
            Compare.program("./bf", input=make(), std_program="./std") 
        except:
            print("WRONG ANSWER")
            os.system("rm std bf 1.out")
            exit(0)
            
    os.system("rm std bf 1.in 1.out")
    print("Accepted!")