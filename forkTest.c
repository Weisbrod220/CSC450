#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/shm.h>

int* mergesort(int left_array[], int right_array[], int left_size, int right_size)
{
    int z;
    static int final_array[1000], i = 0, j = 0, k=0;
    while(i<left_size && j< right_size) {
		if(left_array[i]  < right_array[j]) final_array[k++] = left_array[i++];
		else final_array[k++] = right_array[j++];
	}
    while(i < left_size) final_array[k++] = left_array[i++];
	while(j < right_size) final_array[k++] = right_array[j++];
    return final_array;
}

int* sort(int array[], int length)
{
    static int sorted_array[2];
    static int sorted_array_one[1];
    if(length == 2)
    {
        if(array[0] < array[1])
        {
            return array;
        }
        else
        {
            sorted_array[0] = array[1];
            sorted_array[1] = array[0];
            return sorted_array;
        }
    }
    sorted_array_one[0] = array[0];
    return sorted_array_one;
}

int* sorting(int array[], int length)
{
    int middle = (length / 2) + (length % 2), nbytes;
    int lpipe[2], rpipe[2], larray[middle], rarray[length - middle], larrayread[middle], rarrayread[length - middle], i;
    pid_t left_child, right_child;
   
    pipe(lpipe);
    pipe(rpipe);
    
    if((left_child = fork()) == 0)
    {
        // left_child
        close(lpipe[0]);
        for(i = 0; i < middle; i ++)
        {
            larray[i] = array[i];
        }
        if(middle > 2)
        {           
            int* mergedSortedArray = sorting(larray, middle);
            write(lpipe[1], mergedSortedArray, sizeof(larray));
            exit(0);
        }
        else
        {
            int* sorted = sort(larray, middle);

            write(lpipe[1], sorted, sizeof(larray));
            exit(0);
        }
    }
    else
    {
        if((right_child = fork()) == 0)
        {
            // right child
            close(rpipe[0]);
            for(i = middle; i < length; i++)
            {
                rarray[i - middle] = array[i];
            }
            int n = length - middle;
            if(n > 2)
            {
                int* mergedSortedArray =  sorting(rarray, n);
                write(rpipe[1], mergedSortedArray, sizeof(rarray));
                exit(0);
            }
            else
            {
                int* sorted = sort(rarray, n);
                write(rpipe[1], sorted, sizeof(rarray));
                exit(0);
            }
        }
        else
        {
            // parent
            close(lpipe[1]);
            close(rpipe[1]);
            
            read(lpipe[0], larrayread, sizeof(larray));
            int lbyteLength = middle ;
            
            read(rpipe[0], rarrayread, sizeof(rarray));
            int rbyteLength = length - middle;
            int* returnArray = mergesort(larrayread, rarrayread, lbyteLength, rbyteLength);

            // sort 
            return(returnArray);
        }
    }
}


int main(int argc, char *argv[])
{
    int unsorted[10], i;

    for(i = 0; i < 10; i++)
    {
        unsorted[i] = 9 - i;
    }
    
    int* sorted_array = sorting(unsorted, 10);
    for(i = 0; i < 10; i++)
    {
        printf("%d\n",sorted_array[i]);
    }

    return(0);
}