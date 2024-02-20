import numpy as np
import random
import matplotlib.pyplot as plt

# Const
DIM = 12
N = 256
init_x0 = 11.26
max_time = 5*N
repeat_time = 1000
scale = 11.26


data = np.empty((N,DIM+2))
for i in range(N):
    strs = input()
    strs = strs.split()
    data[i][0] = init_x0
    for j in range(1,DIM+2):
        data[i][j] = float(strs[j-1])*scale

test = np.zeros(repeat_time)
for cnt in range(repeat_time):
    w = np.zeros(13)
    ac_cnt = 0
    change_cnt = 0
    while ac_cnt <= max_time:
        idx = random.randint(0,N-1)
        y = data[idx][DIM+1]
        x = data[idx][0:DIM+1]
        if(y * (x.dot(w)) <= 0):
            w = w + (y*x)
            ac_cnt = 0
            change_cnt += 1
        ac_cnt += 1
    test[cnt] = change_cnt

test = np.sort(test)

# file write
'''
fp = open("P10.txt","a")
for c in test:
    fp.write(str(c)+"\n")
fp.close()
'''

# median
median = np.median(test)
print("median is ",median)


# histogram
plt.hist(test,20)
plt.xlabel("change times")
plt.ylabel("count")
plt.title("ML hw1 P10 change times")

plt.show()

