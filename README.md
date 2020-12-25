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
    
```
 $ Finn_Trace_Analysis_C simulation [<option>]
```
  
#### Options

-l <vlaue>&emsp;&emsp;&emsp;              simutlate stream length,  
-r <vlaue>&emsp;&emsp;&emsp;            rmage of dbn of zipf of simutlate stream,   
-k <vlaue>&emsp;&emsp;&emsp;           k value of sketch entropy algorithm ,  
-z <vlaue>&emsp;&emsp;&emsp;           z parameter of zipf dbn,  
-t <vlaue>&emsp;&emsp;&emsp;           simulation times,  
-e <vlaue>&emsp;&emsp;&emsp;          error prob. threshold,  
-o <vlaue>&emsp;&emsp;&emsp;           zipf dbn offset,must be greater be positve integer,  
-a <vlaue>&emsp;&emsp;&emsp;           choose specific sketch algorithm,  
-it <vlaue>&emsp;&emsp;&emsp;           table amount of inverse cdf of clifford,  
-KLD&emsp;&emsp;&emsp;                enable to simulate another alogrithm and calculate the KLD of them,     
