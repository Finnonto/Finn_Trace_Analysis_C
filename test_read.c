#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

struct stat st = {0};
int main(){

int result = mkdir("/mnt/c/Users/Lab108/Desktop/Finn_Trace_Analysis_C/output/1", 0777);

return 0;
}
