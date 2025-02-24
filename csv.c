#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

//Note: LEX02 has some help for files

//Globals for number of rows and columns in csv
int csvColumns = 0;
int csvRows = 0;

void call_h(FILE *file, char *targetField);

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
            return EXIT_FAILURE;
        }
    
    //Char buffer to store the contents of the csv file
    char buffer[1024];
    while(fgets(buffer, 1024, file))
    {
        csvRows++;
        //We only care about row 1
        if (csvRows == 1)
            {
            //Split on commas
            char* value = strtok(buffer, ",");
            //Count the number of fields
            while(value)
                {
                   value = strtok(NULL, ",");
                   csvColumns++;
                }
            }
    }
    //Create a 2D array that has all values from CSV
    char* csvFile[csvRows][csvColumns];
    int r = 0;
    rewind(file);
    //Copy in values
    while(fgets(buffer, 1024, file))
        {
            int c = 0;
            char* value = strtok(buffer, ",");
            while(value != NULL)
                {
                    csvFile[r][c] = value;
                    value = strtok(NULL, ",");
                    c++;
                }
            r++;
        }
    
    if (argc >= 2){

        //e.g -min field, -max field, -mean field
        char* field;
        int i = 1;
        while (i < argc-1){
            // -f
            char* ef = "-f";
            if (strcmp(argv[i], ef) == 0){
                //Print the number of fields
                printf("%d\n",csvColumns);

                //Close the file
                fclose(file);

                //Exit
                return EXIT_SUCCESS;
            }

                // -h
            if (strcmp(argv[i], "-h") == 0){
                //If user inputs the (the numeric positional option) of field instead of the (title of the corresponding field)
                //Function should return EXIT_FAILURE
                //isdigit() checks if a string can be converted into an integer
                if (!isdigit(atoi(argv[i + 2])) || (i + 2 >= argc-1)){
                    return EXIT_FAILURE;
                }

                call_h(file, argv[i + 1]); //(file & fieldname)
                fclose(file);
                return EXIT_SUCCESS;
            }

            // -r
            else if ( strcmp(argv[i], "-r") == 0){
                int rows = 0;
                char buffer[1024];

                while (fgets(buffer, 1024, file)){
                    rows++;
                }
                printf("%d\n", csvRows);
                return EXIT_SUCCESS;
            }

// -min
            else if (strcmp(argv[i], "-min") == 0)
                {
                    //Move i over to field and set field to the number
                    i++;
                    int field = atoi(argv[i]);
                    //Set a min and a flag to see if numeric data has been encountered
                    int min = INT_MAX;
                    int numeric = 0;
                    //Loop through specified field
                    for(int currentRow = 0; currentRow < csvRows; currentRow++)
                        {
                            int fieldValue = atoi(csvFile[currentRow][field]);
                            if(fieldValue < min)
                                {
                                    min = fieldValue;
                                }
                        }
                    printf("%d\n", min);
                }

            // -max
            else if (strcmp(argv[i], "-max") == 0)
                {
                    //Move i over to field and set field to the number
                    i++;
                    int field = atoi(argv[i]);
                    //Set a max and a flag to see if numeric data has been encountered
                    int max = INT_MIN;
                    int numeric = 0;
                    //Loop through specified field
                    for(int currentRow = 0; currentRow < csvRows; currentRow++)
                        {
                            int fieldValue = atoi(csvFile[currentRow][field]);
                            if(max < fieldValue)
                                {
                                    max = fieldValue;
                                }
                        }
                    printf("%d\n", max);
                }
            // -records field value 
            // for now this is WITHOUT -h
            //currently causing a seg fault somewhere!
            else if (strcmp(argv[i], "-records") == 0)
                {
                    // increment i to get the field, then again to get the value
                    i++;
                    int field = atoi(argv[i]);
                    printf("Field = %i", field);
                    i++;
                    char * value = argv[i];
                    printf("Value = %s", value);
                    // loop through field
                    // if this field has the value, print it
                    for(int currentRow = 0; currentRow < csvRows; currentRow++)
                    {
                        char * fieldValue = csvFile[currentRow][field];
                        if(value == fieldValue)
                            {
                                printf("%d\n", currentRow);
                            }
                    }
                    
                    //return
                    return EXIT_SUCCESS;
                }

            // temp response
            else
            {
                printf("Undefined\n");
                return EXIT_FAILURE;
            }

            i++;
            }

        }
}

//-h helper function

void call_h(FILE *file, char *targetField) {
    char buffer[1024];
    //Space for header fields
    char *headerFields[1024];
     //Track field index
    int fieldIndex = -1;

    //Read the first line (header row)
    if (fgets(buffer, sizeof(buffer), file)) {
        int i = 0;
        char *value = strtok(buffer, ",\n");

        //Compare fields
        while (value) {
            headerFields[i] = value;
            if (strcmp(headerFields[i], targetField) == 0) {
                fieldIndex = i;
            }
            i++;
            value = strtok(NULL, ",\n");
        }
    }

    //Get the wanted fields values
    if (fieldIndex != -1) {
        while (fgets(buffer, sizeof(buffer), file)) {
            char *value = strtok(buffer, ",\n");
            for (int i = 0; i < fieldIndex; i++) {
                value = strtok(NULL, ",\n");
            }
            if (value) {
                printf("%s\n", value);
            }
        }
    }
}
