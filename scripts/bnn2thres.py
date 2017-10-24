#!/usr/bin/python3

import sys
import time
import math
import numpy as np

class ThresGate():
    def __init__(self, Id, weights, thres_val, fanins, fanouts):
        self.Id = Id
        self.weights = weights
        self.thres_val = thres_val
        self.fanins = fanins
        self.fanouts = fanouts
        self.out = None

    def getOutput(self, inputs):
        assert len(inputs) == len(self.weights)
        val = 0
        for i in range(len(inputs)):
            val += inputs[i] * self.weights[i]
        if val >= self.thres_val:
            return 1
        return -1

def sign(x):
    if x >= 0:
        return 1
    return -1

def testNetwork(threslogics, all_thresgate):
    test_X = np.load(sys.argv[3])
    test_y = np.load(sys.argv[4])
    test_X = 2 * test_X - 1; # normalize to [-1, 1]
    result = []
    cnt = 1
    for i in range(len(test_X)):
        print('Input number {}'.format(cnt))
        cnt += 1
        # First layer
        outs = []
        print('    Testing Layer 0')
        for gate in threslogics[0]:
            #print(len(Xdata), len(gate.weights))
            gate.out = gate.getOutput(test_X[i])
            outs.append(gate.out)
        
        for l in range(1, len(threslogics)):
            print('    Testing Layer {}'.format(l))
            inputs = outs[:]
            outs[:] = []
            for gate in threslogics[l]:
                #print(len(inputs), len(gate.weights))
                gate.out = gate.getOutput(inputs)
                outs.append(gate.out)
        
        idx = None
        if outs.count(1) == 1:
            idx = outs.index(1)
        else:
            idx = -1
        print('Result: {}, Ans: {}'.format(idx, test_y[i][0]))

def writeNetwork(filename, threslogics, input_shape, output_shape):
    with open(filename, 'w') as f:
        f.write('##################################################\n')
        f.write('# Benchmark generate from BNN mnist params       #\n')
        f.write('# Author: Hao Chen ({})    #\n'.format(time.ctime()))
        f.write('##################################################\n\n')

        f.write('.model {}\n\n'.format(filename))
        
        f.write('.inputs')
        for i in range(input_shape):
            f.write(' i{}'.format(i))
        f.write('\n')
        
        f.write('.outputs')
        for i in range(output_shape):
            f.write(' o{}'.format(i))
        f.write('\n\n')

        for i in range(len(threslogics)):
            for gate in threslogics[i]:

                f.write('.thres')
                #f.write('-in')
                for fanin in gate.fanins:
                    f.write(' {}'.format(fanin))
                f.write(' t{}\n'.format(gate.Id))
                
                #f.write('-out')
                #for fanout in gate.fanouts:
                #    f.write(' {}'.format(fanout))
                #f.write('\n')

                #f.write('-weight')
                for weight in gate.weights:
                    f.write('{} '.format(weight))
                f.write('{}\n\n'.format(gate.thres_val))

                #f.write('-thres_val {}\n\n'.format(gate.thres_val))
        for i in range(output_shape):
            f.write('.po t{} o{}\n'.format(threslogics[len(threslogics) - 1][i].Id, i));

def transform_MNIST_dense():
    print('Loading BNN parameters.....')
    bnn_params_filename = sys.argv[1]
    out_filename = sys.argv[2]

    bnn_params = np.load(bnn_params_filename)
    input_shape  = 28 * 28 #len(bnn_params['arr_0'])
    output_shape = 10 * 1  #len(bnn_params['arr_{}'].format(len(bnn_params) - 1))
    n_hiddens    = 3       #len(bnn_params) / 6 - 1
    n_units      = 4096    #len(bnn_params['arr_0'][0])
    
    
    all_thresgate = []
    threslogics = []
    thresId = 0
    print('Parsing BNN to Threslogic Network.....')
    for l in range(n_hiddens + 1):
        # [W, bias, beta, gamma, mean, inv_std]
        W       = np.transpose(bnn_params['arr_{}'.format(6 * l)])
        bias    = np.transpose(bnn_params['arr_{}'.format(6 * l + 1)])
        beta    = np.transpose(bnn_params['arr_{}'.format(6 * l + 2)])
        gamma   = np.transpose(bnn_params['arr_{}'.format(6 * l + 3)])
        mean    = np.transpose(bnn_params['arr_{}'.format(6 * l + 4)])
        inv_std = np.transpose(bnn_params['arr_{}'.format(6 * l + 5)])
        
        fanins = []
        fanouts = []
        if l == 0:
            assert len(W) == n_units and len(W[0]) == input_shape
            fanins  = ['i{}'.format(i) for i in range(input_shape)]
            fanouts = ['t{}'.format(i) for i in range(n_units)]
        elif 0 < l < n_hiddens:
            assert len(W) == n_units and len(W[0]) == n_units
            fanins  = ['t{}'.format(i) for i in range((l - 1) * n_units, l * n_units)]
            fanouts = ['t{}'.format(i) for i in range(l * n_units - 1, (l + 1) * n_units)]
        else:
            assert len(W) == output_shape and len(W[0]) == n_units
            fanins  = ['t{}'.format(i) for i in range((l - 1) * n_units - 1, l * n_units)]
            fanouts = ['o{}'.format(i) for i in range(output_shape)]

        thres = []
        for i in range(len(W)):
            thres_val = mean[i] - (beta[i] / (gamma[i] * inv_std[i]))
            if l > 0:
                thres_val = math.ceil(thres_val)
            thres_gate = None
            if gamma[i] * inv_std[i] < 0:
                thres_gate = ThresGate(thresId, [-int(w) for w in W[i]], thres_val, fanins, fanouts)
            else:
                thres_gate = ThresGate(thresId, [int(w) for w in W[i]], thres_val, fanins, fanouts)
            #thres_gate = ThresGate(thresId, [int(w) for w in W[i]], thres_val, fanins, fanouts)
            thres.append(thres_gate)
            all_thresgate.append(thres_gate)
            thresId += 1
        threslogics.append(thres)
    
    print('n_thres_layer: {}'.format(len(threslogics)))
    print('n_thres: ', end = '')
    for l in range(len(threslogics)):
        if l < len(threslogics) - 1:
            print('{}'.format(len(threslogics[l])), end = ',')
        else:
            print(len(threslogics[l]))
    
    #print('Testing network.....')
    #testNetwork(threslogics, all_thresgate)
    
    print('Writing network.....')
    writeNetwork(out_filename, threslogics, input_shape, output_shape)

if __name__ == '__main__':
    transform_MNIST_dense()
