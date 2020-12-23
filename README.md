# Finn_Trace_Analysis_C
implement trace analysis method based on libtrace in C  

```
mkdir bin
mkdir obj
mkdir output
```


## usage: 
 
### Simulation:
This method use zipf dbn to generate streams and calculate the entropy with sketch algorithm we choose, and use parameter <simulation> to enable this method
#### usage:
'''
$ Finn_Trace_Analysis_C simulation [<option>]
'''
####　Options 
-l <vlaue>       simutlate stream length,
-r <vlaue>       rmage of dbn of zipf of simutlate stream, 
-k <vlaue>       k value of sketch entropy algorithm ,
-z <vlaue>       z parameter of zipf dbn,
-t <vlaue>       simulation times,
-e <vlaue>       error prob. threshold,
-o <vlaue>       zipf dbn offset,must be greater be positve integer,
-a <vlaue>       choose specific sketch algorithm,
-i <vlaue>       table amount of inverse cdf of clifford,
-KLD             enable to simulate another alogrithm and calculate the KLD of them,
