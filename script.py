import os


for ti in range(1,2):
    os.makedirs("/mnt/c/Users/Lab108/Desktop/1120/1126/clifford_cdf50_{0}-30".format(ti),exist_ok=True)
    #os.system("./bin/Finn_trace_analysis  /mnt/d/Download/MAWI2007/200701011400.dump.gz  1126/clifford_cdf50_{0}-30/Analysis_sec_30s_200701011400 30 {0}".format(ti))
    os.system("./bin/Finn_trace_analysis  /mnt/d/Download/MAWI2007/200702011400.dump.gz  1126/clifford_cdf100_{0}-30/Analysis_sec_30s_200702011400 30 {0}".format(ti))
    #os.system("./bin/Finn_trace_analysis  /mnt/d/Download/MAWI2007/200703011400.dump.gz  1126/clifford_cdf100_{0}-30/Analysis_sec_30s_200703011400 30 {0}".format(ti))
    #os.system("./bin/Finn_trace_analysis  /mnt/d/Download/MAWI2007/200704011400.dump.gz  1126/clifford_cdf100_{0}-30/Analysis_sec_30s_200704011400 30 {0}".format(ti))
    #os.system("./bin/Finn_trace_analysis  /mnt/d/Download/MAWI2007/200705011400.dump.gz  1126/clifford_cdf100_{0}-30/Analysis_sec_30s_200705011400 30 {0}".format(ti))
    #os.system("./bin/Finn_trace_analysis  /mnt/d/Download/MAWI2007/200706121400.dump.gz  1126/clifford_cdf100_{0}-30/Analysis_sec_30s_200706121400 30 {0}".format(ti))
    #os.system("./bin/Finn_trace_analysis  /mnt/d/Download/MAWI2007/200707011400.dump.gz  1126/clifford_cdf100_{0}-30/Analysis_sec_30s_200707011400 30 {0}".format(ti))
    #os.system("./bin/Finn_trace_analysis  /mnt/d/Download/MAWI2007/200708011400.dump.gz  1126/clifford_cdf100_{0}-30/Analysis_sec_30s_200708011400 30 {0}".format(ti))
    #os.system("./bin/Finn_trace_analysis  /mnt/d/Download/MAWI2007/200709011400.dump.gz  1126/clifford_cdf100_{0}-30/Analysis_sec_30s_200709011400 30 {0}".format(ti))
    #os.system("./bin/Finn_trace_analysis  /mnt/d/Download/MAWI2007/200710011400.dump.gz  1126/clifford_cdf100_{0}-30/Analysis_sec_30s_200710011400 30 {0}".format(ti))

