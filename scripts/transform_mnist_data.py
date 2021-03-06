#!/usr/bin/python3
import numpy as np

train_x = np.load('../data/mnist_train_X.npy')
train_y = np.load('../data/mnist_train_y.npy')
test_x = np.load('../data/mnist_test_X.npy')
test_y = np.load('../data/mnist_test_y.npy')


with open('../data/mnist_train.bench', 'w') as f:
    for i in range(len(train_x)):
        for j in range(len(train_x[i])):
            f.write('{} '.format(train_x[i][j]))
        f.write('{}\n'.format(train_y[i][0]))

with open('../data/mnist_test.bench', 'w') as f:
    for i in range(len(test_x)):
        for j in range(len(test_x[i])):
            f.write('{} '.format(test_x[i][j]))
        f.write('{}\n'.format(test_y[i][0]))
    
