import os
z = [0, 0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9]
length = [100000, 500000, 1000000]
for off in range(35,70):
    for l in length:
        for zeta in z :
            for r in range(1,10):
                
                os.system("./bin/Finn_trace_analysis simulation -l {0} -r {1}0000  -k 20 -z {2} -t 100  -o {3}".format(l,r,zeta,off))