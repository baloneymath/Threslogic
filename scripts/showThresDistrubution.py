#!/usr/bin/python3

import sys
import math
from mpl_toolkits.mplot3d import Axes3D
import matplotlib.pyplot as plt
from matplotlib import cm
from matplotlib.ticker import LinearLocator, FormatStrFormatter
import numpy as np

if __name__ == '__main__':
    '''
    step_size = 1
    tlf_filename = sys.argv[1]
    total = [0] * 1600
    with open(tlf_filename, 'r') as f:
        for line in f:
            tokens = line.split(' ')
            if tokens[0] == '.thres':
                info = f.readline().split(' ')
                thres_val = math.ceil(float(info[-1]))
                total[thres_val // step_size + 700] += 1
    plt.plot([x for x in range(-700, 900)], total)
    plt.show()
    '''
    tlf_filename = sys.argv[1]

    fig = plt.figure()
    ax = fig.add_subplot(111, projection = '3d')

    # Make data
    X = []
    Y = []
    Z = []
    Z2 = []
    Z_dict = {}
    Z2_dict = {}

    with open(tlf_filename, 'r') as f:
        for line in f:
            tokens = line.split(' ')
            if tokens[0] == '.thres':
                info = f.readline().split(' ')
                thres_val = int(np.ceil(float(info[-1])))
                num_1s = sum([int(x) for x in info if x == '1']) / (len(info) - 1)
                X.append(thres_val)
                Y.append(num_1s)
                if (thres_val, num_1s) in Z_dict.keys():
                    Z_dict[(thres_val, num_1s)] += 1
                else:
                    Z_dict[(thres_val, num_1s)] = 0
    
    for i in range(len(X)):
        Z.append(Z_dict[(X[i], Y[i])])
    
    ax.scatter(X, Y, Z, c = 'r', marker = 'o')

    ax.set_xlabel('Threshold Value')
    ax.set_ylabel('Ratio of 1s')
    ax.set_zlabel('Number of gates')

    plt.show()
