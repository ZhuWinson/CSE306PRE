#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

//Note: LEX02 has some help for files




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

    if (argc >= 2){

        //e.g -min field, -max field, -mean field
        char* field;
        int i = 1;
        while (i < argc-1){
            // -f
            char* ef = "-f";
            if ( strcmp(argv[i], ef) == 0){
                //keeps count of the fields
                int columns = 0;
                //keeps count of the rows
                int rows = 0;
                //char buffer to store the contents of the csv file
                char buffer[1024];

                while(fgets(buffer, 1024, file)){
                    rows++;

                    //we only care about row 1
                    if (rows == 1){
                        //split on commas
                        char* value = strtok(buffer, ", ");

                        //count the number of fields
                        while(value){
                            value = strtok(NULL, ", ");
                            columns++;
                        }
                    }
                }

                //print the number of fields
                printf("%d\n",columns);

                //close the file
                fclose(file);

                //exit
                return EXIT_SUCCESS;
            }

                // -h
            if (strcmp(argv[i], "-h") == 0) {

                //if user inputs the (the numeric positional option) of field instead of the (title of the corresponding field)
                //function should return EXIT_FAILURE
                //isdigit() checks if a string can be converted into an integer
                if (!isdigit(argv[i + 2]) || (i + 2 >= argc-1)){
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
                printf("%d\n", rows);
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
     //space for header fields
    char *headerFields[1024];
     //track field index
    int fieldIndex = -1;

    // Read the first line (header row)
    if (fgets(buffer, sizeof(buffer), file)) {
        int i = 0;
        char *value = strtok(buffer, ",\n");

        // compare fields
        while (value) {
            headerFields[i] = value;
            if (strcmp(headerFields[i], targetField) == 0) {
                fieldIndex = i;
            }
            i++;
            value = strtok(NULL, ",\n");
        }
    }

    // get the wanted fields values
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
