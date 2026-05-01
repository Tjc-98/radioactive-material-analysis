/*
    The application is to store information and do calculations on radioactive materials.
    Important note! the layout of the stored radioactive material in the text file should 
    look like the following:
        name1 massNumber1 half-Life1(Seconds)
        name2 massNumber2 half-Life2(Seconds)
        ...
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXREGISTERS 1000
#define MAXNAMELENGTH 20
#define DEFAULTFILENAME "RadioactiveMaterialData.txt"
#define MAXFILENAMELENGTH 500

// Represnets the radioactive material with its properties.
struct RadioactiveMaterial {
    char name[MAXNAMELENGTH];
    int massNumber;
    double halfLifeTime; // in seconds
};

// Print out the functionality that the application offers for the user.
void interfaceMenu() {
    printf("Choose one of the numbers related to the options below!\n");
    printf("(1) Register a new radioactive material(s).\n");
    printf("(2) Show the stored radioactive materials.\n");
    printf("(3) Sort the stored radioactive materials.\n");
    printf("(4) Search for a radioactive material.\n");
    printf("(5) Change infomation for a radioactive material.\n");
    printf("(6) Delete a stored radioactive material.\n");
    printf("(7) Show the menu.\n");
    printf("(0) Quit the App.\n");
}

// Get the file name from the user.
void getUserFilename(char *filename) {
    printf("Enter the name of the file contaning the radioactive material infomation followed by (.txt).\n");
    printf("In case there is not saved data before choose a name to save the data into followed by (.txt).\n");
    printf("Note! Enter (def) for using the default file name. Default name is (RadioactiveMaterialData.txt)\n");
    printf("Enter the name Here: ");
    scanf("%[^\n]%*c", filename);
    printf("\n");
}

// Get user input for the name.
char *getUserNameInput() {
    static char name[MAXNAMELENGTH];
    printf("Enter any character combinations (20 at most): ");
    scanf("%20s", name);
    printf("\n");
    return name;
}

// Get user input for the mass number
int getUserMassNumberInput() {
    int number;
    printf("Enter the mass number: ");
    scanf("%d", &number);
    printf("\n");
    return number;
}

// Get user input for the half-time minimum number
double getUserMinNumberInput() {
    double minNumber;
    printf("Enter the minimum half-time value: ");
    scanf("%lE", &minNumber);
    printf("\n");
    return minNumber;
}

// Get user input for the half-time maximum number
double getUserMaxNumberInput() {
    double maxNumber;
    printf("Enter the maximum half-time value: ");
    scanf("%lE", &maxNumber);
    printf("\n");
    return maxNumber;
}

// Read the data of the radioactive material from the application.
void readDataFromFile(char filename[], struct RadioactiveMaterial *RadioactiveMaterials, int *currentSize) {
    FILE *file;
    int bufferLength = 255;
    char buffer[bufferLength];

    // Check the name that the user chosen
    if (strcmp(filename, "def") == 0) { // in case the user choose the default name
        file = fopen(DEFAULTFILENAME, "r");
    } else {
        file = fopen(filename, "r");
    }
    
    // if the file exists we take the data from it
    if (file != NULL) {
        // Read lines in the text
        while(fgets(buffer, bufferLength, file) && *currentSize < MAXREGISTERS) {
            struct RadioactiveMaterial tempRadioactiveMaterial;
            char *ptr = strtok(buffer, " ");
            strcpy(tempRadioactiveMaterial.name, ptr);
		    ptr = strtok(NULL, " ");
            tempRadioactiveMaterial.massNumber = atoi(ptr);
	        ptr = strtok(NULL, " ");
            tempRadioactiveMaterial.halfLifeTime = atof(ptr);

            RadioactiveMaterials[*currentSize] = tempRadioactiveMaterial;
            *currentSize = *currentSize + 1;
        }
        fclose(file);
    }
}

// Save the temporary information of the radioactive material to a text file.
void saveDataToFile(char filename[], struct RadioactiveMaterial *RadioactiveMaterials, int currentSize) {
    FILE *file;
    int bufferLength = 255;
    char buffer[bufferLength];
    char str[100];

    // Check the name that the user chosen
    // Note that the file is created automaticaly in case it does not exists
    if (strcmp(filename, "def") == 0) { // in case the user choose the default name
        file = fopen(DEFAULTFILENAME, "w");
    } else {
        file = fopen(filename, "w");
    }

    for(int i = 0; i < currentSize; i++) {
        strcpy(buffer, ""); // Reset the content every line
        strcat(buffer, RadioactiveMaterials[i].name);
        strcat(buffer, " ");
        sprintf(str,"%d",RadioactiveMaterials[i].massNumber);
        strcat(buffer, str);
        strcat(buffer, " ");
        sprintf(str,"%E",RadioactiveMaterials[i].halfLifeTime);
        strcat(buffer, str);
        if(i != (currentSize - 1)) {
            strcat(buffer, "\n");
        }
        fputs(buffer, file);
    }
    fclose(file);
}

// Show the infomation about the stored radioactive material in a pleasing manner.
void showStoredRadioactiveMaterials(struct RadioactiveMaterial RadioactiveMaterials[], int currentSize) {
    printf("Name\t\t\tMass Number\tHalf-Time(s)\n");
    printf("----------------------------------------------------\n");

    for (int i = 0; i < currentSize; i++) {
        printf("%s\t\t\t%d\t\t%.4E\n", RadioactiveMaterials[i].name, RadioactiveMaterials[i].massNumber, RadioactiveMaterials[i].halfLifeTime);
    }
    
    printf("\n");
}

// Convert the half-time from any unit (s, m, h, d, y) to seconds.
double convertHalfTimeRepresentationToNumber(char representation[]) {
    double number;
    
    char *ptr = strtok(representation, " ");
    number = atof(ptr);

    ptr = strtok(NULL, "");
    if (strcmp(ptr, "y") == 0) {
        number = number * 365 * 24 * 60 * 60;
    } else if (strcmp(ptr, "d") == 0) {
        number = number * 24 * 60 * 60;
    } else if (strcmp(ptr, "h") == 0) {
        number = number * 60 * 60;
    } else if (strcmp(ptr, "m") == 0) {
        number = number * 60;
    }

    return number;
}

// Check how many materials that match the passed name.
int checkNameQuantityForRadioactiveMaterials(struct RadioactiveMaterial RadioactiveMaterials[], 
int currentSize, char name[]) {
    int count = 0;
    for(int i = 0; i < currentSize; i++) {
        if (strstr(RadioactiveMaterials[i].name, name)) {
            count++;
        }
    }
    return count;
}

// Get the indexes for the radioactive materials matching the passed name.
void getNameSearchIndexesForRadioactiveMaterials(struct RadioactiveMaterial RadioactiveMaterials[], 
int currentSize, char name[], int *MatchedRadioactiveMaterialsIndexes) {
    int j = 0;
    for (int i = 0; i < currentSize; i++) {
        if (strstr(RadioactiveMaterials[i].name, name)) {
            MatchedRadioactiveMaterialsIndexes[j] = i;
            j++;
        }
    }
}

// Check how many instances are existent that match the passed mass number.
int checkMassNumberQuantityForRadioactiveMaterials(struct RadioactiveMaterial RadioactiveMaterials[], 
int currentSize, int number) {
    int count = 0;
    for(int i = 0; i < currentSize; i++) {
        if (RadioactiveMaterials[i].massNumber == number) {
            count++;
        }
    }
    return count;
}

// Get the indexes for the radioactive materials matching the passed mass number.
void getMassNumberSearchIndexesForRadioactiveMaterials(struct RadioactiveMaterial RadioactiveMaterials[], 
int currentSize, int number, int *MatchedRadioactiveMaterialsIndexes) {
    int j = 0;
    for (int i = 0; i < currentSize; i++) {
        if (RadioactiveMaterials[i].massNumber == number) {
            MatchedRadioactiveMaterialsIndexes[j] = i;
            j++;
        }
    }
}

// Check how many instances are existent that match the passed interval of the half-time.
int checkHalfTimeQuantityForRadioactiveMaterials(struct RadioactiveMaterial RadioactiveMaterials[], 
int currentSize, double minNumber, double maxNumber) {
    int count = 0;
    for(int i = 0; i < currentSize; i++) {
        if (RadioactiveMaterials[i].halfLifeTime >= minNumber && RadioactiveMaterials[i].halfLifeTime <= maxNumber) {
            count++;
        }
    }
    return count;
}

// Get the indexes for the radioactive materials matching the passed interval of the half-time.
void getHalfTimeSearchIndexesForRadioactiveMaterials(struct RadioactiveMaterial RadioactiveMaterials[], 
int currentSize, double minNumber, double maxNumber, int *MatchedRadioactiveMaterialsIndexes) {
    int j = 0;
    for (int i = 0; i < currentSize; i++) {
        if (RadioactiveMaterials[i].halfLifeTime >= minNumber && RadioactiveMaterials[i].halfLifeTime <= maxNumber) {
            MatchedRadioactiveMaterialsIndexes[j] = i;
            j++;
        }
    }
}

// Get the information of the selected radioactive materials passing their indices.
void getInfoOfRadioactiveMaterialsWithIndexes(struct RadioactiveMaterial RadioactiveMaterials[], 
int indexes[], int indexesCount, struct RadioactiveMaterial *Materials) {
    for(int i = 0; i < indexesCount; i++) {
        Materials[i] = RadioactiveMaterials[indexes[i]];
    }
}

// Change the half-time value for a specific material.
void changeHalfTime(struct RadioactiveMaterial *RadioactiveMaterials, int index) {
    double halftime;
    char halftimeRep[17]; // 17 total = 15 decimals, 1 space and 1 character
    
    printf("Enter the new half-time in (s, m, h, d, y)(Ex. 10e+18 h): ");
    scanf(" %[^\n]%*c", halftimeRep);
    halftime = convertHalfTimeRepresentationToNumber(halftimeRep);

    RadioactiveMaterials[index].halfLifeTime = halftime;
    printf("\n");
}

// Delete the specified material index from the data.
void deleteMaterial(struct RadioactiveMaterial *RadioactiveMaterials, int *currentSize, int index) {
    for (int i = index; i < *currentSize - 1; i++) {
        RadioactiveMaterials[i] = RadioactiveMaterials[i + 1];
    }
    *currentSize = *currentSize - 1;
}

// Run the search method for the name.
void runNameSearch(struct RadioactiveMaterial RadioactiveMaterials[], int currentSize, char name[]) {
    int resSize = checkNameQuantityForRadioactiveMaterials(RadioactiveMaterials, currentSize, name);
    int resInd[resSize];
    getNameSearchIndexesForRadioactiveMaterials(RadioactiveMaterials, currentSize, name, resInd);
    struct RadioactiveMaterial Res[resSize];
    getInfoOfRadioactiveMaterialsWithIndexes(RadioactiveMaterials, resInd, resSize, Res);
    showStoredRadioactiveMaterials(Res, resSize);
}

// Run the search method for the mass number.
void runMassNumberSearch(struct RadioactiveMaterial RadioactiveMaterials[], int currentSize, int number) {
    int resSize = checkMassNumberQuantityForRadioactiveMaterials(RadioactiveMaterials, currentSize, number);
    int resInd[resSize];
    getMassNumberSearchIndexesForRadioactiveMaterials(RadioactiveMaterials, currentSize, number, resInd);
    struct RadioactiveMaterial Res[resSize];
    getInfoOfRadioactiveMaterialsWithIndexes(RadioactiveMaterials, resInd, resSize, Res);
    showStoredRadioactiveMaterials(Res, resSize);
}

// Run the search method for the half-time.
void runHalfTimeSearch(struct RadioactiveMaterial RadioactiveMaterials[], int currentSize, 
double minNumber, double maxNumber) {
    int resSize = checkHalfTimeQuantityForRadioactiveMaterials(RadioactiveMaterials, currentSize, minNumber, maxNumber);
    int resInd[resSize];
    getHalfTimeSearchIndexesForRadioactiveMaterials(RadioactiveMaterials, currentSize, minNumber, maxNumber, resInd);
    struct RadioactiveMaterial Res[resSize];
    getInfoOfRadioactiveMaterialsWithIndexes(RadioactiveMaterials, resInd, resSize, Res);
    showStoredRadioactiveMaterials(Res, resSize);
}

// Get the specifc index of one material related to searched name.
int getSpecificNameSearchIndex(struct RadioactiveMaterial *RadioactiveMaterials, int currentSize) {
    int resSize;
    char *name = getUserNameInput();
    runNameSearch(RadioactiveMaterials, currentSize, name);
    resSize = checkNameQuantityForRadioactiveMaterials(RadioactiveMaterials, currentSize, name);
    if (resSize > 1 || resSize == 0) {
        return -1;
    }
    int resInd[resSize];
    getNameSearchIndexesForRadioactiveMaterials(RadioactiveMaterials, currentSize, name, resInd);
    return resInd[0];
}

// Get the specific index of one material related to searched mass number.
int getSpecificMassNumberSearchIndex(struct RadioactiveMaterial *RadioactiveMaterials, int currentSize) {
    int resSize;
    int number = getUserMassNumberInput();
    runMassNumberSearch(RadioactiveMaterials, currentSize, number);
    resSize = checkMassNumberQuantityForRadioactiveMaterials(RadioactiveMaterials, currentSize, number);
    if (resSize > 1 || resSize == 0) {
        return -1;
    }
    int resInd[resSize];
    getMassNumberSearchIndexesForRadioactiveMaterials(RadioactiveMaterials, currentSize, number, resInd);
    return resInd[0];
}

// Get the specific index of one material related to searched half-time.
int getSpecificHafTimeSearchIndex(struct RadioactiveMaterial *RadioactiveMaterials, int currentSize) {
    int resSize;
    double minNumber = getUserMinNumberInput();
    double maxNumber = getUserMaxNumberInput();
    runHalfTimeSearch(RadioactiveMaterials, currentSize, minNumber, maxNumber);
    resSize = checkHalfTimeQuantityForRadioactiveMaterials(RadioactiveMaterials, currentSize, minNumber, maxNumber);
    if (resSize > 1 || resSize == 0) {
        return -1;
    }
    int resInd[resSize];
    getHalfTimeSearchIndexesForRadioactiveMaterials(RadioactiveMaterials, currentSize, minNumber, maxNumber, resInd);
    return resInd[0];
}

// Sort the stored radioactive materials using bubblesort algorithm.
// Reference: https://www.geeksforgeeks.org/bubble-sort/
void sortStoredRadioactiveMaterials(struct RadioactiveMaterial *RadioactiveMaterials, int currentSize) {
    int choice;
    struct RadioactiveMaterial currentMaterial;
    int counter = 0;

    printf("Enter what the sorting should follow:\n");
    printf("(1) name, (2) mass number, (3) half-time\n");
    printf("Enter the number here: ");
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            for (int i = 0; i < currentSize; i++) {
                for(int j = 0; j < (currentSize - i - 1); j++) {
                    if(strcmp(RadioactiveMaterials[j].name, RadioactiveMaterials[j + 1].name) > 0) {
                        currentMaterial = RadioactiveMaterials[j];
                        RadioactiveMaterials[j] = RadioactiveMaterials[j + 1];
                        RadioactiveMaterials[j + 1] = currentMaterial;
                    }
                }
            }
            break;
        case 2:
            for (int i = 0; i < currentSize; i++) {
                for(int j = 0; j < (currentSize - i - 1); j++) {
                    if(RadioactiveMaterials[j].massNumber > RadioactiveMaterials[j + 1].massNumber) {
                        currentMaterial = RadioactiveMaterials[j];
                        RadioactiveMaterials[j] = RadioactiveMaterials[j + 1];
                        RadioactiveMaterials[j + 1] = currentMaterial;
                    }
                }
            }
            break;
        case 3:
            for (int i = 0; i < currentSize; i++) {
                for(int j = 0; j < (currentSize - i - 1); j++) {
                    if(RadioactiveMaterials[j].halfLifeTime > RadioactiveMaterials[j + 1].halfLifeTime) {
                        currentMaterial = RadioactiveMaterials[j];
                        RadioactiveMaterials[j] = RadioactiveMaterials[j + 1];
                        RadioactiveMaterials[j + 1] = currentMaterial;
                    }
                }
            }
            break;
    }
    printf("The radioactive materials have been sorted!\n");
    printf("\n");
}

// Menu for searching into the stored radioactive material with different options of searching.
void searchForRadioactiveMaterialMenu(struct RadioactiveMaterial RadioactiveMaterials[], int currentSize) {
    printf("What are you looking to search for regarding stored radioactive materials?\n");
    int choice;
    while(1) {
        printf("(1) Name.\n");
        printf("(2) Mass number.\n");
        printf("(3) Half-time.\n");
        printf("(4) Return to main menu.\n");
        printf("Enter number related to desired option: ");
        scanf("%d", &choice); // Problem for letting characters slip of characters
        printf("\n");

        switch (choice) {
            case 1: {
                char *name = getUserNameInput();
                runNameSearch(RadioactiveMaterials, currentSize, name);
                break;
            }
            case 2: {
                int number = getUserMassNumberInput();
                runMassNumberSearch(RadioactiveMaterials, currentSize, number);
                break;
            }
            case 3: {
                double minNumber = getUserMinNumberInput();
                double maxNumber = getUserMaxNumberInput();
                runHalfTimeSearch(RadioactiveMaterials, currentSize, minNumber, maxNumber);
                break;
            }
            case 4:
                return;
            default:
                printf("Please enter a valid option from the menu.\n");
                break;
        }
    }
}

// Register a new radioactive material and store its data.
void registerNewRadioactiveMaterial(struct RadioactiveMaterial *RadioactiveMaterials, int *currentSize) {
    char name[MAXNAMELENGTH];
    int massNumber;
    double halftime;
    char halftimeRep[17]; // 17 total = 15 decimals, 1 space and 1 character

    while (1) {
        printf("Register a new radioactive material.\n");
        if((*currentSize + 1) >= MAXREGISTERS) {
            printf("Unfortunately unable to add more radioactive material because the data is full (%d entries to be exact).\n", *currentSize);
            return;
        }

        printf("Enter name (20 characters most)(q to quit adding): ");
        scanf("%20s", name);
        
        if (strcmp(name, "q") == 0) {
            return;
        }

        massNumber = getUserMassNumberInput();

        // Search if the material is already existent in the data
        int searchResSize = checkNameQuantityForRadioactiveMaterials(RadioactiveMaterials, *currentSize, name);
        int searchResInd[searchResSize];
        getNameSearchIndexesForRadioactiveMaterials(RadioactiveMaterials, *currentSize, name, searchResInd);
        
        int check = 0;
        // Check the user and search results
        for (int i = 0; i < searchResSize; i++) {
            if(RadioactiveMaterials[searchResInd[i]].massNumber == massNumber) {
                printf("The material is already added!\n");
                check = 1;
                break;
            }
        }

        // If the material was already added then start over the register
        if (check) {
            continue;
        }

        printf("Enter the half-time in (s, m, h, d, y)(Ex. 10e+18 h): ");
        scanf(" %[^\n]%*c", halftimeRep);
        halftime = convertHalfTimeRepresentationToNumber(halftimeRep);
        
        struct RadioactiveMaterial NewRadioactiveMaterial;
        strcpy(NewRadioactiveMaterial.name, name);
        NewRadioactiveMaterial.massNumber = massNumber;
        NewRadioactiveMaterial.halfLifeTime = halftime;

        RadioactiveMaterials[*currentSize] = NewRadioactiveMaterial;
        *currentSize = *currentSize + 1;
        printf("The new meterial is added!\n");
        printf("\n");
    }
}

// Change the half-time information about a radioactive material.
void changeRadioactiveMaterialInfo(struct RadioactiveMaterial *RadioactiveMaterials, int currentSize) {
    int choice;
    int index;

    printf("Change the half-time of a radioactive material.\n");
    while(1) {
        printf("Enter method to search for a metrial.\n");
        printf("(1) name, (2) mass number, (3) halftime: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1: 
                index = getSpecificNameSearchIndex(RadioactiveMaterials, currentSize);
                if (index == -1) {
                    printf("More or less than one of the material was found!\n");
                    break;
                }
                changeHalfTime(RadioactiveMaterials, index);
                return;
            case 2:
                index = getSpecificMassNumberSearchIndex(RadioactiveMaterials, currentSize);
                if (index == -1) {
                    printf("More or less than one of the material was found!\n");
                    break;
                }
                changeHalfTime(RadioactiveMaterials, index);
                return;
            case 3:
                index = getSpecificHafTimeSearchIndex(RadioactiveMaterials, currentSize);
                if (index == -1) {
                    printf("More or less than one of the material was found!\n");
                    break;
                }
                changeHalfTime(RadioactiveMaterials, index);
                return;
            default:
                printf("Invalid number!\n");
                break;
        }
    }
}

// Delete a radioactive material from the stored data.
void deleteRadioactiveMaterial(struct RadioactiveMaterial *RadioactiveMaterials, int *currentSize) {
    int choice;
    int index;

    printf("Delete a radioactive material.\n");
    while(1) {
        printf("Enter method to search for a metrial.\n");
        printf("(1) name, (2) mass number, (3) halftime: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1: 
                index = getSpecificNameSearchIndex(RadioactiveMaterials, *currentSize);
                if (index == -1) {
                    printf("More or less than one of the material was found!\n");
                    break;
                }
                deleteMaterial(RadioactiveMaterials, currentSize, index);
                return;
            case 2:
                index = getSpecificMassNumberSearchIndex(RadioactiveMaterials, *currentSize);
                if (index == -1) {
                    printf("More or less than one of the material was found!\n");
                    break;
                }
                deleteMaterial(RadioactiveMaterials, currentSize, index);
                return;
            case 3:
                index = getSpecificHafTimeSearchIndex(RadioactiveMaterials, *currentSize);
                if (index == -1) {
                    printf("More or less than one of the material was found!\n");
                    break;
                }
                deleteMaterial(RadioactiveMaterials, currentSize,index);
                return;
            default:
                printf("Invalid number!\n");
                break;
        }
    }
}

// The function responsible to initialize the whole program and manage the correct call for the other functions.
int main() {
    struct RadioactiveMaterial RadioactiveMaterials[MAXREGISTERS];
    char filename[MAXFILENAMELENGTH];
    int currentSize = 0;
    
    printf("********************************\n");
    printf("****Radioactive Material App****\n");
    printf("********************************\n");
    getUserFilename(filename);
    readDataFromFile(filename, RadioactiveMaterials, &currentSize);
    interfaceMenu();
    int choice;

    while(1) {
        printf("\nchoose an option ([7] to show the menu again): ");
        scanf("%d", &choice);
        printf("\n");
        switch (choice) {
            case 0:
                saveDataToFile(filename, RadioactiveMaterials, currentSize);
                printf("Thank you for using the application!\n");
                printf("****Quiting****");
                return 0;
            case 1:
                registerNewRadioactiveMaterial(RadioactiveMaterials, &currentSize);
                break;
            case 2:
                showStoredRadioactiveMaterials(RadioactiveMaterials, currentSize);
                break;
            case 3:
                sortStoredRadioactiveMaterials(RadioactiveMaterials, currentSize);
                break;
            case 4:
                searchForRadioactiveMaterialMenu(RadioactiveMaterials, currentSize);
                break;
            case 5:
                changeRadioactiveMaterialInfo(RadioactiveMaterials, currentSize);
                break;
            case 6:
                deleteRadioactiveMaterial(RadioactiveMaterials, &currentSize);
                break;
            case 7:
                interfaceMenu();
                break;
            default:
                printf("Please enter a valid number related to one of the options!\n");
        }
    }

    return 0;
}