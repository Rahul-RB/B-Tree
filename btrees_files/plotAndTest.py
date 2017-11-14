import subprocess
from random import randint
import re


noOfTestCases = 20

t_values = [2**i for i in range(2, 10)]

def build_tree(t, records_num):
    print("\nCompiling for t="+str(t)+"....")
    p = subprocess.Popen(['make','clean'], stdin=subprocess.PIPE, stdout=subprocess.PIPE)
    p = subprocess.Popen(['make','t='+str(t)], stdin=subprocess.PIPE, stdout=subprocess.PIPE)
    out, err = p.communicate()

    print("Compiling done....")
    print("\nBuilding tree....")
    p = subprocess.Popen(['./run', '-b'], stdin=subprocess.PIPE, stdout=subprocess.PIPE)
    # print(str(records_num).encode('utf-8'))
    out, err = p.communicate(input = str(records_num).encode('utf-8'))
    print(out)
    # kk = float((re.findall("\d+\.\d+", str(out)))[0])
    # print(kk)
    # exit()
    return float((re.findall("\d+\.\d+", str(out)))[0])

def test_search(records_num,key):
    
    p = subprocess.Popen(['./run', '-s', str(key)], stdin=subprocess.PIPE, stdout=subprocess.PIPE)
    out, err = p.communicate(input = str(records_num).encode('utf-8'))
    print(out)
    # time = float((re.findall("\d+\.\d+", str(out)))[0])
    time = float((re.findall("\d+\.\d+", str(out)))[0])
    
    print(time)

    return(time)
    
    
if __name__ == "__main__":
    records_num = 1000000
    build_times = []
    avg_times = []
    search_items = [randint(1, 1000000) for i in range(10)]
    for i in t_values:
        build_times.append(build_tree(i, records_num))
        search_times = []
        for s in search_items:
            search_times.append(test_search(records_num,s))
        avg_times.append(sum(search_times)/len(search_times))

    print(t_values, build_times)
    plt.xlabel('T Values')
    plt.ylabel('Time in ms')
    plt.title('Tree Building')
    plt.plot(t_values, build_times)
    plt.show()
    plt.clf()

    plt.xlabel('T Values')
    plt.ylabel('Time in ms')
    plt.title('Tree Search For 10 Random Keys(Avg)')
    plt.plot(t_values, avg_times)
    plt.show()
    plt.clf()
