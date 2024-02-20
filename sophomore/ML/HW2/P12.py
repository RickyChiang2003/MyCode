import numpy as np
import random
import matplotlib.pyplot as plt


# const
N = 8
runtime = 2000


def reset_Data_theta_y(Data,theta,y):
    # init Data
    Data[0] = -1
    for i in range(1,N+1):
        Data[i] = random.uniform(-1,1)
    iszero = 0
    for i in range(N+1):
        for j in range(N+1):
            if (i != j and Data[i] == Data[j]):
                Data[j] = 0
                iszero = 1
    while iszero == 1:
        for i in range(N+1):
            if (Data[i] == 0):
                Data[i] = random.uniform(-1,1)
        iszero = 0
        for i in range(N+1):
            for j in range(N+1):
                if (i != j and Data[i] == Data[j]):
                    Data[j] = 0
                    iszero = 1
    Data = np.sort(Data)

    # init theta
    for i in range(1,N):
        theta[i] = (Data[i]+Data[i+1])/2
    theta[N] = -1

    # init y
    for i in range(N+1):
        noise = random.uniform(0, 10)
        y[i] = np.sign(Data[i])
        if(noise <= 1):
            y[i] = -y[i]


# main
Data = np.zeros((N+1))
theta = np.zeros((N+1))
y = np.zeros((N+1))

Ein = np.zeros((runtime+1))
Eout = np.zeros((runtime+1))
Eout_Ein = np.zeros((runtime+1))
for cnt in range(runtime+1):
    reset_Data_theta_y(Data,theta,y)
    s_in = (1 if random.uniform(-1,1) > 0 else -1)
    theta_in = theta[random.randint(1,N)]
    Ein[cnt] = 0
    for x in range(1,N+1):
        h = s_in * (-1 if Data[x] <= theta_in else 1)
        if (y[x] != h):
            Ein[cnt] += 1
    Ein[cnt] /= N
    s_out = s_in
    theta_out = theta_in
    Eout[cnt] = 0.5 - 0.4*s_out + 0.4*s_out*theta_out*np.sign(theta_out)
    Eout_Ein[cnt] = Eout[cnt] - Ein[cnt]


#median
median = np.median(Eout_Ein)
print("median is",median)


#scatter plot 
size = [tmp*3 for tmp in Eout]
plt.scatter(Ein,Eout,s=size)
plt.xlabel("Ein")
plt.ylabel("Eout")
title = "ML hw2 P12 with median = " + str(median)
plt.title(title)
plt.show()
