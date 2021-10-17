#!/usr/bin/env python3
import csv
import matplotlib.pyplot as plt
import numpy as np
import math

path = ['csv_fix_data/multi_thread.csv', 'csv_fix_data/multi_process.csv']

for(i,path) in enumerate(path):
    datasize, num, time = [], [], []
    with open(path, newline='') as csvfile:
        rows = csv.reader(csvfile, delimiter=',')
        for row in rows:
            datasize.append(int(row[0]))
            num.append(int(row[1]))
            time.append(int(row[2]))
    plot_x, plot_y = [[] for i in range(16)], [[] for i in range(16)]
    datasize_plot_x = [[] for i in range(int(math.log(100000000,10)))]
    datasize_plot_y = [[] for i in range(int(math.log(100000000,10)))]
    for point in range(len(datasize)):
        plot_x[num[point]-1].append(datasize[point])
        plot_y[num[point]-1].append(time[point])

        datasize_plot_x[int(math.log(datasize[point],10))-1].append(num[point])
        datasize_plot_y[int(math.log(datasize[point],10))-1].append(time[point])

    for i in range(16): 
        plt.plot(plot_x[i],plot_y[i], '.-')

    plt.xscale("log", base=10)
    plt.grid()

    if i%2==0:
        plt.legend([str(i+1)+" thread" for i in range(16)])
    else:
        plt.legend([str(i+1)+" process" for i in range(16)])
    plt.xlabel("datasize")
    plt.ylabel("time")
    plt.title(path[13:])
    plt.savefig(path[13:]+".png")
    # plt.show()
    plt.clf()

    legend_list = []
    for k in range(int(math.log(100000000,10))):
        if (k>=2) and (k<8):
            legend_list.append("datasize=10^"+str(k+1))
            plt.plot(datasize_plot_x[k],datasize_plot_y[k], '.-')

    # plt.yscale("log", basex=2)
    plt.grid()
    plt.legend(legend_list)
    # plt.legend(["datasize=2^"+str(k+1) for k in range(int(math.log(100000000,2)),3)])
    if i%2==0:
        plt.xlabel("thread")
    else:
        plt.xlabel("process")

    plt.ylabel("time")
    plt.title("specify_datasize "+path[13:])
    plt.savefig("specify_datasize "+path[13:]+".png")
    plt.clf()
