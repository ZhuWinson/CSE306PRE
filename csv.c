#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <math.h>

//Note: COMPILE WITH -lm flag

//Globals for number of rows and columns in csv
int csvColumns = 0;
int csvRows = 0;

//void call_h(FILE *file, char *targetField);
int call_h(char* csvFile[][csvColumns], char* flag, char* field, char* recordValue, char* fieldname[]);

int call_min(char* csvFile[][csvColumns], int fieldIndex);
int call_max(char* csvFile[][csvColumns], int fieldIndex);
int call_mean(char* csvFile[][csvColumns], int fieldIndex);
int call_records(char* csvFile[][csvColumns], int fieldIndex, char* Value);


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
                    //Check if we are in quotes
                    int inQuotes = 0;
                    if(value[0] == '"')
                        {
                            inQuotes = 1;
                            //Give space for a summation of strings
                            csvFile[r][c] = (char*)malloc(sizeof(char)*80);
                            //Append all the pieces together and exit when the closing " is found
                            while(inQuotes == 1)
                                {
                                    int length = strlen(value);
                                    if(value[length-1] == '"' || (value[length-2] == '"' && value[length-1] == '\n'))
                                        {
                                            inQuotes = 0;
                                        }
                                    char* piece = (char*)malloc(sizeof(char)*strlen(value)+1);
                                    strcpy(piece, value);
                                    strcat(csvFile[r][c],piece);
                                    value = strtok(NULL, ",");
                                }
                        }
                    else
                        {
                            char* copy = (char*)malloc(sizeof(char)*strlen(value)+1);
                            strcpy(copy, value);
                            csvFile[r][c] = copy;
                            value = strtok(NULL, ",");
                        }
                    c++;
                }
            r++;
        }

    char* fieldNames[csvColumns];
    for (int i = 0; i < csvColumns; i++)
        {
            fieldNames[i] = csvFile[0][i];
        }

    if (argc >= 2){

        //e.g -min field, -max field, -mean field
        char* field;
        int i = 1;
        int h_called = 0;
        while (i < argc-1){
            // -f
            char* ef = "-f";
            if (strcmp(argv[i], ef) == 0){
                //Print the number of fields
                printf("%d\n",csvColumns);

                //Close the file
                fclose(file);

                i++;

            }

                // -h
            else if (strcmp(argv[i], "-h") == 0){
                //If user inputs the (the numeric positional option) of field instead of the (title of the corresponding field)
                //Function should return EXIT_FAILURE
                //isdigit() checks if a string can be converted into an integer
                // if (!isdigit(atoi(argv[i + 2])) || (i + 2 >= argc-1)){
                //     return EXIT_FAILURE;
                // }

                // call_h(file, argv[i + 1]); //(file & fieldname)
                // fclose(file);

                h_called = 1;

                if (isdigit(atoi(argv[i + 2])) || (i + 2 >= argc-1)){
                    return EXIT_FAILURE;
                }

                int result = 0;

                if (strcmp(argv[i+1], "-r") == 0)
                {
                    result = call_h(csvFile, argv[i+1], "all", "none", fieldNames);

                    // i  i+1  i+2
                    //-h  -r   -f
                    //if it's records then don't try to continue
                    if(strcmp(argv[i+2], "-records")==0){return EXIT_SUCCESS;}
                    i+=2;
                }
                else{
                    //printf("enter\n");
                    result = call_h(csvFile, argv[i+1], argv[i+2], argv[i+3], fieldNames);
                    //printf("exit\n");

                    // i    i+1   i+2   i+3
                    //-h  -mean  field  -f

                    //if it's records then don't try to continue
                    if(strcmp(argv[i+1], "-records")==0){return EXIT_SUCCESS;}
                    i+=3;
                }

                if (result == EXIT_FAILURE){
                    return EXIT_FAILURE;
                }
                //return EXIT_SUCCESS;
            }

            // -r
            else if ( strcmp(argv[i], "-r") == 0){
                if (h_called == 1)
                {
                    printf("%d\n", csvRows+1);
                }
                else{
                    printf("%d\n", csvRows);
                }
                i++;
            }

            // -min
            else if (strcmp(argv[i], "-min") == 0)
                {
                    int result = 0;
                    if(h_called == 1)
                    {
                        result = call_h(csvFile, argv[i], argv[i+1], "none", fieldNames);
                    }
                    else{
                        result = call_min(csvFile, atoi(argv[i+1]));
                    }

                   if (result == EXIT_FAILURE)
                   {
                    return EXIT_FAILURE;
                   }
                   // this should move from the -min flag index to the next flag index
                   //  i  i+1  i+2
                   //-min  0   -f
                   i+=2;
                }
            // -max
            else if (strcmp(argv[i], "-max") == 0)
                {
                    int result = 0;
                    if(h_called == 1)
                    {
                        result = call_h(csvFile, argv[i], argv[i+1], "none", fieldNames);
                    }
                    else{
                        result = call_max(csvFile, atoi(argv[i+1]));
                    }


                    if (result == EXIT_FAILURE)
                    {
                        return EXIT_FAILURE;
                    }
                    // this should move from the -min flag index to the next flag index
                    //  i  i+1  i+2
                    //-max  0   -f
                    i+=2;
                }
             // -mean
             else if (strcmp(argv[i], "-mean") == 0)
                {
                    int result = 0;
                    if(h_called == 1)
                    {
                        result = call_h(csvFile, argv[i], argv[i+1], "none", fieldNames);
                    }
                    else{
                         result = call_mean(csvFile, atoi(argv[i+1]));
                    }

                    if (result == EXIT_FAILURE)
                    {
                        return EXIT_FAILURE;
                    }
                    // this should move from the -min flag index to the next flag index
                    //  i  i+1  i+2
                    //-mean  0   -f
                    i+=2;
                }
            // -records field value

            else if (strcmp(argv[i], "-records") == 0)
                {
                    
                    int result = 0;
                    if (h_called == 1)
                    {
                        //printf("argv[i+2 = %s]\n", argv[i+2]);
                        result = call_h(csvFile, argv[i], argv[i+1], argv[i+2], fieldNames);
                    }
                    else{
                        result = call_records(csvFile, atoi(argv[i+1]), argv[i+2]);
                    }

                    if (result == EXIT_FAILURE){
                        return EXIT_FAILURE;
                    }

                    //     i        i+1    i+2     i+3
                    // -records    field  value     -f
                    i += 3;

                    //return
                    //return EXIT_SUCCESS;
                }

            // temp response

            //i++;
            }

        }

        return EXIT_SUCCESS;
}

//-h helper function

// void call_h(FILE *file, char *targetField) {
//     char buffer[1024];
//     //Space for header fields
//     char *headerFields[1024];
//      //Track field index
//     int fieldIndex = -1;

//     //Read the first line (header row)
//     if (fgets(buffer, sizeof(buffer), file)) {
//         int i = 0;
//         char *value = strtok(buffer, ",\n");

//         //Compare fields
//         while (value) {
//             headerFields[i] = value;
//             if (strcmp(headerFields[i], targetField) == 0) {
//                 fieldIndex = i;
//             }
//             i++;
//             value = strtok(NULL, ",\n");
//         }
//     }

//     //Get the wanted fields values
//     if (fieldIndex != -1) {
//         while (fgets(buffer, sizeof(buffer), file)) {
//             char *value = strtok(buffer, ",\n");
//             for (int i = 0; i < fieldIndex; i++) {
//                 value = strtok(NULL, ",\n");
//             }
//             if (value) {
//                 printf("%s\n", value);
//             }
//         }
//     }
// }

int call_h(char* csvFile[][csvColumns], char* flag, char* field, char* recordValue, char* fieldname[])
{
    //traverse the fieldName list to find the index of the that matches the field
    //argument and use that index to call the other helper functions
    for (int i = 0; i < csvColumns; i++)
    {
        // "Value", aka the last field has a \r\n slapped onto the back of it
        // With some funky C pointer magic it can be removed
        // But only the last feild has this issue, so we'll need to make sure we only do that if we're at the last field
        
        char * f_name;

        // if this is the last field, fix the string
        if(i == (csvColumns-1)){
            f_name = fieldname[i];
            // Find the tab character and replace it with a null terminator
            for (int i = 0; f_name[i] != '\0'; i++) {
                if (f_name[i] == '\r') {
                    f_name[i] = '\0';
                    break;
                }
            }
        }
        // if not, leave it alone
        else{
            f_name = fieldname[i];
        }
        
        if (strcmp(fieldname[i],  field) == 0)
        {
            if (strcmp(flag, "-min") == 0)
            {
                return call_min(csvFile, i);
            }
            else if (strcmp(flag, "-max") == 0)
            {
                return call_max(csvFile, i);
            }
            else if (strcmp(flag, "-mean") == 0)
            {
                return call_mean(csvFile, i);
            }
            else if (strcmp(flag, "-r") == 0)
            {
                printf("%d\n", csvRows+1);
                return EXIT_SUCCESS;
            }else if (strcmp(flag, "-records") == 0)
            {
                //printf("index = %d\n", i);
                //printf("record value = %f\n", atof(recordValue));
                return call_records(csvFile, i, recordValue);
            }
        }
    }

    // if the for loop is exited it means none of the conditionals caught, which means there is some kind of issue
    return EXIT_FAILURE;
}

int call_min(char* csvFile[][csvColumns], int field)
{
     //int field = atoi(fieldIndex);
     //Set a min and a flag to see if numeric data has been encountered
     double min = INT_MAX;
     int numeric = 0;
     //Loop through specified field
     for(int currentRow = 0; currentRow < csvRows; currentRow++)
         {
             char* currentValue = csvFile[currentRow][field];
             double numValue = atof(currentValue);
             //Check if it is a number
             if(numValue == 0 && currentValue[0] != '0')
                 {
                     continue;
                 }
             else
                 {
                     //Set flag for number has been seen
                     numeric = 1;
                     if(numValue < min)
                         {
                             min = numValue;
                         }
                 }
         }
     if(numeric == 1)
         {
             printf("%.2f\n", min);
             return EXIT_SUCCESS;
         }
     return EXIT_FAILURE;
}

int call_max(char* csvFile[][csvColumns], int field)
{
    //int field = atoi(fieldIndex);
    //Set a max and a flag to see if numeric data has been encountered
    double max = INT_MIN;
    int numeric = 0;
    //Loop through specified field
    for(int currentRow = 0; currentRow < csvRows; currentRow++)
        {
            char* currentValue = csvFile[currentRow][field];
            double numValue = atof(currentValue);
            //Check if it is a number
            if(numValue == 0 && currentValue[0] != '0')
                {
                    continue;
                }
            else
                {
                    //Set flag for number has been seen
                    numeric = 1;
                    if(numValue > max)
                        {
                            max = numValue;
                        }
                }
        }
    if(numeric == 1)
        {
            printf("%.2f\n", max);
            return EXIT_SUCCESS;
        }
    return EXIT_FAILURE;
}

int call_mean(char* csvFile[][csvColumns], int field)
{
      //int field = atoi(fieldIndex);
      //Set a sum and a flag to see if numeric data has been encountered
      double sum = 0;
      int counter = 0;
      int numeric = 0;
      //Loop through specified field
      for(int currentRow = 0; currentRow < csvRows; currentRow++)
          {
              char* currentValue = csvFile[currentRow][field];
              double numValue = atof(currentValue);
              //Check if it is a number
              if(numValue == 0 && currentValue[0] != '0')
                  {
                      continue;
                  }
              else
                  {
                      //Set flag for number has been seen
                      numeric = 1;
                      sum += numValue;
                      counter++;
                  }
          }
      double result = round((sum/counter)*100)/100;
      if(numeric == 1)
          {
              printf("%.2f\n", result);
              return EXIT_SUCCESS;
          }
      return EXIT_FAILURE;
}

int call_records(char* csvFile[][csvColumns], int field, char* Value)
{
     float value = atof(Value);
     //printf("Value = %f\n", value);
     // loop through field
     // if this field has the value, print it
     for(int currentRow = 0; currentRow < csvRows; currentRow++)
     {
         float fieldValue = atof(csvFile[currentRow][field]);

         if(value == fieldValue)
             {

                 //printf("%s\n", csvFile[currentRow][field]);
                 for(int curr_column = 0; curr_column < csvColumns; curr_column++){
                     printf("%s", csvFile[currentRow][curr_column]);
                     if(curr_column != csvColumns -1){printf(",");}
                 }
                 printf("\r"); // handout whats us to have \r instead of \n ig
             }
     }
     return EXIT_SUCCESS;
}
