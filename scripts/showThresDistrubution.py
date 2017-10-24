#!/usr/bin/python3

import sys
import math
import matplotlib.pyplot as plt

if __name__ == '__main__':
    
    step_size = 1
    tlf_filename = sys.argv[1]
    total = [0] * 1000

    with open(tlf_filename, 'r') as f:
        for line in f:
            tokens = line.split(' ')
            if tokens[0] == '.thres':
                info = f.readline().split(' ')
                thres_val = math.ceil(float(info[-1]))
                total[thres_val // step_size] += 1
    plt.plot(total)
    plt.show()
                

