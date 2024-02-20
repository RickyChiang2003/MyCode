import numpy as np
import random
import matplotlib.pyplot as plt
import math

# const
N = 256
Ntest = 4096
runtime = 128
sigma1 = 0.6324555  # sqrt(0.4)
sigma2 = 0.7745966  # sqrt(0.6)
T = 500
eta = 0.1
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

def init_xytest(x,y):
    for i in range(Ntest):
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

def theta(s):
    return (1 / (1+math.exp(-s)))

def find_w2(x,y):
    w2 = np.zeros(3)
    for i in range(T):
        tmp = np.zeros(3)
        for j in range(N):
            tmp = tmp + theta(-1*y[j]*(x[j].dot(w2)))*(y[j]*x[j])
        tmp = tmp * eta / N
        w2 = w2 + tmp
    return w2

def find_Ein_01(w,x,y):
    tmp = (x@w)
    tmpEin = 0
    for i in range(N):
        tmpEin = tmpEin + (1 if tmp[i]*y[i] < 0 else 0)
    return ((tmpEin) / N)

def find_Eout_01(w,xtest,ytest):
    tmp = (xtest@w)
    tmpEout = 0
    for i in range(Ntest):
        tmpEout = tmpEout + (1 if tmp[i]*ytest[i] < 0 else 0)
    return ((tmpEout) / Ntest)
#

# main
x = np.zeros([N,3])
y = np.zeros(N)
xtest = np.zeros([Ntest,3])
ytest = np.zeros(Ntest)
Eout = np.zeros([2,runtime])  # 0 for A and 1 for B
for i in range(runtime):
    init_xy(x,y)
    init_xytest(xtest,ytest)
    # A
    w_LIN = find_w_LIN(x,y)
    Eout[0][i] = find_Eout_01(w_LIN,xtest,ytest)
    # B
    w2 = find_w2(x,y)
    Eout[1][i] = find_Eout_01(w2,xtest,ytest)
    #print("runtime: " + str(i))
#

#median
medianA = np.median(Eout[0])
print("median is",medianA)
medianB = np.median(Eout[1])
print("median is",medianB)
#

#scatter plot 
#size = [tmp*3 for tmp in Eout[1]]
#plt.scatter(Eout[0],Eout[1],s=size)
plt.scatter(Eout[0],Eout[1])
plt.xlabel("Eout(A(D))")
plt.ylabel("Eout(B(D))")
title = "ML hw3 P11 with median = (A:" + str(medianA) + ", B:" + str(medianB) + ")"
plt.title(title)
plt.show()