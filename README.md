# Finn_Trace_Analysis_C
implement trace analysis method based on libtrace in C  



If there is no output folder:
```
mkdir output
```
before you started,recommand you to make clean first.

## Usage: 
 
### Simulation:  
This method use zipf dbn to generate streams and calculate the entropy with sketch algorithm we choose, and use parameter [simulation] to enable this method.
#### usage:  
    
```
 $ Finn_Trace_Analysis_C simulation <options><parameter>
```
or you can use 
```
make sim 
```
to make simple test.
#### Options

`-l` : simutlate stream length [positive integer]  
`-r` :  range of dbn of zipf of simutlate stream [positive integer]    
`-k` :  k value of sketch entropy algorithm [positive integer]     
`-z` :  z parameter of zipf dbn   [positive float]  
`-t` :  simulation times  [positive integer]   
`-o` :  zipf dbn offset  [positive integer]  
`-a` :  choose specific sketch algorithm,need to provide amount of algorithm,then type algorthms. e.g [-a 2 Clifford PingLi]   
  there are 5 Algorithm  :
"exact",
"Clifford",
"Clifford_cdf",
"Clifford_HT",
"PingLi"  
`-it` :   amount of inverse cdf table of clifford  [positive integer max:10]    
`-Tbs` : size of CDF table [16384,32768,65536]
`-PA` : alpha of PingLi

### Trace:
We can calculate specific Trace.pcap file whether it's zipped or not, and use parameter <trace> to enable this method.

!!note this part is not finished yet.

```
$ Finn_Trace_Analysis_C trace file_path <options><parameter>
```


#### Options

`-t` : time interval of analysis(seconds) [positive integer]   
`-k` : k value of algorithm   [positive integer]  
`-a` : choose specific sketch algorithm  
`-it`:  amount of inverse cdf table of clifford  [positive integer max:10]    