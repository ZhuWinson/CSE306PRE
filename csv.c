#include <stdio.h>
#include <stdlib.h>

//Note: LEX02 has some help for files

int main(int argc, char * argv[])
{
    
/*OPTIONAL flags:
-f
-r
-h
-max field
-min field
-mean field
-records field value
*/
    
    //Create file and open it for reading (last argument)
    FILE * file = NULL;
    file = fopen(argv[argc-1], "r");
    //If file is still NULL there was an error
    if (file == NULL)
        {
            printf("Error opening file\n");
        }
}
