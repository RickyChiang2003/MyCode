import numpy as np
import random
import matplotlib.pyplot as plt


# const
N = 256
Ntest = 4096
runtime = 128
sigma1 = 0.6324555
sigma2 = 0.7745966
#

# function
def init_xy(x,y):
    for i in range(N):
        y[i] = (1 if random.randint(-1,2) > 0 else -1)
        x[i][0] = 1
        if y[i] == 1:
            x[i][1] = random.gauss(mu = 3, sigma = sigma1)
            x[i][2] = random.gauss(mu = 2, sigma = sigma1)
        if y[i] == -1:
            x[i][1] = random.gauss(mu = 5, sigma = sigma2)
            x[i][2] = random.gauss(mu = 0, sigma = sigma2)

def find_w_LIN(x,y):
    return (np.linalg.inv((np.transpose(x)) @ x) @ (np.transpose(x)) @ y)

def find_Ein_sqr(w,x,y):
    tmp = x@w - y
    tmpEin = 0
    for i in tmp:
        tmpEin = tmpEin + i*i
    return ((tmpEin) / N)
#

# main
x = np.zeros([N,3])
y = np.zeros(N)
Ein = np.zeros(runtime)
for i in range(runtime):
    init_xy(x,y)
    w_LIN = find_w_LIN(x,y)
    Ein[i] = find_Ein_sqr(w_LIN,x,y)
#

#median
median = np.median(Ein)
print("median is",median)
#

# histogram
plt.hist(Ein,10)
plt.ylabel("count")
plt.xlabel("Ein")
title = "ML hw3 P9 with median = " + str(median)
plt.title(title)
plt.show()
