import cv2
import numpy as np
from keras.datasets import mnist
from keras import utils

# reference: https://steam.oxxostudio.tw/category/python/ai/ai-number-recognizer.html
(X_train, Y_train), (X_test, Y_test) = mnist.load_data()  # 載入訓練集

# 訓練集資料
X_train = X_train.reshape(X_train.shape[0],-1)  # 轉換資料形狀
X_train = X_train.astype('float32')/255         # 轉換資料型別
Y_train = Y_train.astype(np.float32)

# 測試集資料
X_test = X_test.reshape(X_test.shape[0],-1)     # 轉換資料形狀
X_test = X_test.astype('float32')/255           # 轉換資料型別
Y_test = Y_test.astype(np.float32)

maxacc = 0
for k in range(1,100):  # best k = 3
    x_train = X_train
    y_train = Y_train
    x_test = X_test
    y_test = Y_test
    knn=cv2.ml.KNearest_create()                    # 建立 KNN 訓練方法
    knn.setDefaultK(k)                              # 參數設定
    knn.setIsClassifier(True)
    knn.setAlgorithmType(cv2.ml.KNEAREST_BRUTE_FORCE)
    print('training k = ', k, ' ...')
    knn.train(X_train, cv2.ml.ROW_SAMPLE, Y_train)  # 開始訓練
    print('testing...')
    #X_test.shape here is (10000, 784)
    test_pre = knn.predict(X_test)                  # 讀取測試集並進行辨識
    test_ret = test_pre[1]
    test_ret = test_ret.reshape(-1,)
    test_sum = (test_ret == Y_test)
    acc = test_sum.mean()                           # 得到準確率
    print(acc)
    if acc > maxacc:
        maxacc = acc
        print('k = ', k, ' is better')
        knn.save('ftir_mnist_knn.xml')                       # 儲存訓練模型
print('k = ', k, ' is the best !!!!!')