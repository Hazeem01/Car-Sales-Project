// required to enable use of scanf() and printf()
#include <stdio.h>
#include <stdlib.h> // needed for "system()" and the "errno" variable
// NOTE: if stdlib.h were to not be included, then errno.h would be required
#include <string.h>
#include <stdbool.h> // required for bool data type

/* Constants that can be used inside any Function */
#define DISCOUNT_MIN_AGE 18
#define DISCOUNT_MAX_AGE 25
#define DISCOUNT_PERCENTAGE 0.10f // this means 20%
#define MAX_SALES 10
#define CSV_FILE "data.csv"

// the below constants make the code more readable by
// improving the semantics (they give code more meaning)
#define MENU_OPTION_BUY_CARS 'a'
#define MENU_OPTION_VIEW_SALES 'b'
#define MENU_OPTION_VIEW_SALES_BETWEEN 'c'
#define MENU_OPTION_EXIT_APPLICATION 'x'

/* Variables that can be used inside any Function created below */
// don't need negative values, so unsigned short is most appropriate
unsigned short carsAvailable = 60;

// this will track how many ticket sales took place
// this will also be used as the index/position of each sale within the following arrays:
//		numberOfCarsPerSale, carModelPerSale, discountGivenPerSale, customerNames
unsigned short numberOfSales = 0;
// this array will hold the number of cars sold each sale
unsigned short numberOfCarsPerSale[MAX_SALES];
// this array will hold the type of the cars sold each sale; this relates to the ticket price
unsigned short carModelPerSale[MAX_SALES];
// this array will hold discount information about each sale
bool discountGivenPerSale[MAX_SALES];
// this array will hold the name of the customer for each sale, a maximum of 200 characters per name
char customerNames[MAX_SALES][201];

// this array will hold information about each type of ticket;
// the data is synchronised with the data within the carModels array
float carPrices[] = {4.0f, 8.5f, 3.5f};
// this array will hold the name of each type of ticket, for the user's benefit
// the data is synchronised with the data within the carPrices array
char carModels[][20] = {"Toyota Corolla", "Tesla Model X", "Honda Civic"};

// Files Variables & Constants
#define FILE_OPENED 0
#define FILE_CLOSED 1
#define FILE_ERROR 2

// will hold the file stream once opened
FILE *file;
unsigned char fileStatus = FILE_CLOSED;

/* Functions the program will be using */

void clearScreen()
{
    // this tells the Command Prompt (i.e. the System's output mechanism)
    // to do something to clear the screen/console
    // this is purely for the user's benefit, to keep the screen clear of clutter
    system("cls");
}

// see week 6 project DealingWithUserInput
void clearBuffer()
{
    while (getchar() != '\n')
        ;
}

void pauseProgram(char userChoice)
{
    // give the user a chance to read the previous output, allow them to continue when ready
    // customise the output depending on the user's choice
    if (userChoice == MENU_OPTION_EXIT_APPLICATION)
    {
        printf("\n\nPress Enter to Exit the Application");
    }
    else
    {
        printf("\n\nPress Enter to return to the Main Menu...");
    }

    getchar();
}

char getCharFromConsole(char *message)
{
    char userInput;
    // prompt user using the value of the given message
    printf("%s\n", message);
    // capture the character
    scanf(" %c", &userInput);
    clearBuffer();
    // return the captured value
    return userInput;
}

unsigned short getUnsignedShortFromConsole(char *message)
{
    // will be used to store the captured number
    unsigned short userInput;
    // prompt user using the value of the given message
    printf("%s\n", message);
    printf("Please provide your response here--> ");
    scanf("%hd", &userInput);
    clearBuffer();
    // finally, return/export the value so it can be used by whatever statement called this Function
    return userInput;
}

// *str will allow this function to establish a connection between this str variable
// and the one that will exist at the time/place where the function is executed, thus any direct
// modification of *str in here will actually also modify the existing string variable,
// as they will both be pointing to the same address in memory
void getStringFromConsole(char *message, char *str)
{
    // prompt user
    printf("%s\n", message);
    printf("Please provide your response here--> ");
    // as we're now working with the pointer str, we don't need & here
    scanf(" %[^\n]", str);
    clearBuffer();
}

void swapUnsignedShort(unsigned short *a, unsigned short *b)
{
    unsigned short temp;
    // copy the value found at the pointer a's address into the newly created temp variable
    temp = *a;
    // copy the value found at the pointer a's address into the address represented by the pointer b
    *a = *b;
    // copy the value of the temp variable to the pointer b's address
    *b = temp;
}

void swapBool(bool *a, bool *b)
{
    bool temp;
    // copy the value found at the pointer a's address into the newly created temp variable
    temp = *a;
    // copy the value found at the pointer a's address into the address represented by the pointer b
    *a = *b;
    // copy the value of the temp variable to the pointer b's address
    *b = temp;
}

/* START FILES FUNCTIONS */

// returns something that is not NULL if successful
FILE *createFile(char *fileName)
{
    // open the file for writing
    file = fopen(fileName, "w");
    // then close it immediately if successful
    if (file != NULL)
    {
        fclose(file);
    }
    // return the pointer so that the success may be checked
    return file;
}

// mode is one of: "w" for writing, "r" for reading, "a" for appending;
// appending means writing starting from the end of the file, will not touch existing contents;
// there are also other modes available, I encourage you to look these up
void openFile(char *fileName, char *mode)
{
    // attempt to open the file
    file = fopen(fileName, mode);

    // if file's value is NULL, then something happened...
    if (file == NULL)
    {

        // attempt to create it first and then check if it's still NULL, meaning
        //		that something still failed
        if (createFile(fileName) == NULL)
        {
            fileStatus = FILE_ERROR;

            // will produce the output: Error opening D:\data.csv: No such file of directory
            printf("Error opening %s: %s", fileName, strerror(errno));
        }
        else
        {
            // we managed to create the file ... so now ...
            // execute this function again with the same parameters
            //		because it should now succeed
            openFile(fileName, mode);
        }
    }
        // otherwise, all OK
    else
    {
        fileStatus = FILE_OPENED;
    }
}

// should be called once the operations on
//		the currently opened file have concluded
void closeFile()
{
    // only attempt to close the file if it's already open
    if (fileStatus == FILE_OPENED)
    {
        fclose(file);
        fileStatus = FILE_CLOSED;
    }
}

// will return an array of strings representing the
//		customer names stored in the file
void readDataFromFile()
{
    // will keep track of how many lines were read from the file
    int lineCounter = 0;

    // this is an infinite loop, we'll manually stop it once we reach the end of the file
    while (1)
    {

        unsigned short numberOfCarsPerSaleValue = 0, carModelPerSaleValue = 0, discountGivenPerSaleValue = 0;
        char customerNameValue[201];

        int scanResult = fscanf(
                file,											 // the file stream
                "%hd,%hd,%hd,%[^\n]",			 // the format of the line
                &numberOfCarsPerSaleValue, // the variables, one for each placeholder in the format above
                &carModelPerSaleValue,
                &discountGivenPerSaleValue,
                customerNameValue);

        // if we reached the end of the file
        if (scanResult == EOF)
        {
            // then, stop the loop
            break;
        }

        // add the bits of data that were read above into the correct arrays
        numberOfCarsPerSale[lineCounter] = numberOfCarsPerSaleValue;
        carModelPerSale[lineCounter] = carModelPerSaleValue;
        // also cast (convert) the discountGivenPerSaleValue from unsigned short to a bool type
        //		before putting it in the discountGivenPerSale array
        discountGivenPerSale[lineCounter] = (bool)discountGivenPerSaleValue;
        // need to use strcpy here because we're working with strings
        strcpy(customerNames[lineCounter], customerNameValue);

        // increment the lineCounter, ready for next line that might be read
        lineCounter++;
    }

    // make sure the numberOfSales variable is also aware of how many sales are available after the above operation
    numberOfSales = lineCounter;
}

void getDataFromFile()
{
    openFile(CSV_FILE, "r");

    if (fileStatus == FILE_OPENED)
    {
        readDataFromFile();
    }
    else if (fileStatus == FILE_ERROR)
    {
        printf("An error occurred while trying to read from %s.", CSV_FILE);
        // this function requires a char value, so we give it one
        pauseProgram('_');
    }

    closeFile();
}

void writeDataToFile()
{
    // 2,1,0,Raz
    // 3,0,1,Jack
    // ...
    // loop through every sale
    for (int i = 0; i < numberOfSales; i++)
    {
        // add a line to the file that looks like: "2,1,0,Raz"
        fprintf(file, "%hd,%hd,%hd,%s\n",
                numberOfCarsPerSale[i],
                carModelPerSale[i],
                discountGivenPerSale[i],
                customerNames[i]);
    }
}

void saveDataToFile()
{
    openFile(CSV_FILE, "w");

    if (fileStatus == FILE_OPENED)
    {
        writeDataToFile();
    }
    else if (fileStatus == FILE_ERROR)
    {
        printf("An error occurred while trying to write to %s.", CSV_FILE);
        // this function requires a char value, so we give it one
        pauseProgram('_');
    }

    closeFile();
}

/* END FILES FUNCTIONS */

void greetCustomer()
{
    // greet the user
    printf("Hello, Welcome to The Car Sales Company !\n\n");
}

void showMenu()
{
    // present the various actions the user can choose from
    printf("Menu:\n");
    printf("%c. Buy Car\n", MENU_OPTION_BUY_CARS);
    printf("%c. View Sales Stats\n", MENU_OPTION_VIEW_SALES);
    printf("%c. A subset of Sales Stats\n", MENU_OPTION_VIEW_SALES_BETWEEN);
    printf("%c. Exit\n\n", MENU_OPTION_EXIT_APPLICATION);
}

void menu_showCarModels()
{
    // calculate the number of Car Models by asking the carPrices
    // array about how many bytes it holds in memory and dividing that
    // result by the number of bytes a float data type holds in memory
    // the division result will be the number of Car Models (3 in this case)
    int numberOfCars = sizeof(carPrices) / sizeof(float);

    // show the user the types of cars
    printf("\nCar Models:\n");

    for (int i = 0; i < numberOfCars; i++)
    {
        printf("%d - %s\n", i, carModels[i]);
    }
}

float menu_applyDiscount(float currentPrice)
{
    // multiply the price value by the discount percentage
    // this is: 1 - DISCOUNT_PERCENTAGE (0.20) => 0.80 (or, 80%)
    // so, we calculate the discounted value by reducing it to 80% of its value
    return currentPrice * (1 - DISCOUNT_PERCENTAGE);
}

bool menu_isDiscountApplicable(unsigned short userAge)
{
    // check if we need to give a discount
    if (userAge >= DISCOUNT_MIN_AGE && userAge <= DISCOUNT_MAX_AGE)
    {
        // return the true value to indicate that discount is to be given
        return true;
    }
    // return the false value to indicate that no discount is to be given
    return false;
}

void menu_printDiscountOutcome(bool giveDiscount)
{
    // inform the user about the discount outcome
    // always a good idea to separate the calculation from the output messages, makes it easier to work with the code
    if (giveDiscount == true)
    {
        printf("\nYou qualified for a 20% discount !");
    }
    else
    {
        printf("\nSorry, you did not qualify for a discount :(");
    }
}

void menu_buyCars()
{
    printf("Buy Car\n\n");

    // check if we have at least 1 ticket available
    if (carsAvailable > 0)
    {
        printf("We currently have %hd cars available.\n\n", carsAvailable);
    }
    else
    {
        printf("Sorry, there is no car available in the store at this time");
        // this allows us to terminate/stop the Function early
        return;
    }

    /* Variables - these are only used inside menu_buyCars() Function and nowhere else */
    // don't need negative values; these are generally related to one sale, used when purchasing cars
    unsigned short carsNeeded, userAge, carModel;
    bool giveDiscount = false;
    float totalPrice;

    // get the "getStringFromConsole()" Function to directly store the customer's name
    // into the "customerNames" array at position "numberOfSales"
    //		=> we need the & operator to get the address of that array's position sent to the Function
    //			so that it can directly put the value at that address in memory
    getStringFromConsole("What is your name?", customerNames[numberOfSales]);

    carsNeeded = getUnsignedShortFromConsole("\nHow many cars would you be purchasing today ?");

    // check if we have enough cars
    // no "else" needed here
    if (carsAvailable < carsNeeded)
    {
        printf("Sorry, but we only have %hd cars available.\n\n", carsAvailable);
        // we will terminate the Function here
        return;
    }

    // also record the number of cars needed in the numberOfCarsPerSale array
    numberOfCarsPerSale[numberOfSales] = carsNeeded;

    // now, we'll determine what type of ticket the user wants; this will influence the price
    menu_showCarModels();

    // get the value representing the ticket type from the user
    carModel = getUnsignedShortFromConsole("\nPlease provide the car model you want from the list above");

    // also record the type of these cars in the carModelPerSale array
    carModelPerSale[numberOfSales] = carModel;

    // calculate total price for this sale by using the correct ticket type's price
    totalPrice = carsNeeded * carPrices[carModel];

    // update number of cars available by subtracting from it the amount of cars needed
    carsAvailable -= carsNeeded;

    // ask for the user's age
    userAge = getUnsignedShortFromConsole("How old are you ? ");

    if (userAge < 18) {
        printf("\nYou need to be at least 18 to purchase a car");
        return;
    }

    // calculate the value of giveDiscount by making use of this Function and the user's age
    giveDiscount = menu_isDiscountApplicable(userAge);

    // actually apply the discount if needed
    if (giveDiscount == true)
    {
        totalPrice = menu_applyDiscount(totalPrice);
    }

    // also record the outcome in the discountGivenPerSale array
    discountGivenPerSale[numberOfSales] = giveDiscount;

    // inform the user about the discount outcome
    menu_printDiscountOutcome(giveDiscount);

    char proceedWithTransactionResponse = getCharFromConsole("\nInput \'Y\' to continue with this purchase \nInput \'X\' to cancel this transaction ");

    if (proceedWithTransactionResponse == 'Y') {

        // present the outcome
        printf("\nYou have bought a total of %hd cars costing %f GBP.\n", carsNeeded, totalPrice);
        printf("We now have %hd cars left in the store\n\n", carsAvailable);
        printf("Thanks for your purchase! Have a great day!\n");

        // finally, add 1 to the numberOfSales counter
        numberOfSales++;
    }
    printf("Transaction Terminated!");
}

void sortArraysByNumberOfCarsSoldPerSale()
{

    // set up a loop that gives us an index "i" for accessing
    //		between the (first) and (second to last) positions that contain values
    for (int i = 0; i < numberOfSales - 1; i++)
    {

        // set up a loop the gives us an index "j" for accessing
        //		between the (first immediately after i) and (last) positions that contain values
        // this loop will execute fully, from start to finish,
        //		every time the above for loop begins a new iteration
        for (int j = i + 1; j < numberOfSales; j++)
        {

            // check if the value at position i is greater than the value at position j
            if (numberOfCarsPerSale[i] > numberOfCarsPerSale[j])
            {

                // if so, swap those two values in the numberOfCarsPerSale array
                swapUnsignedShort(&numberOfCarsPerSale[i], &numberOfCarsPerSale[j]);

                // also swap the two values at those same positions in the carModelPerSale array
                swapUnsignedShort(&carModelPerSale[i], &carModelPerSale[j]);

                // and in the discountGivenPerSale array
                swapBool(&discountGivenPerSale[i], &discountGivenPerSale[j]);

                // and lastly, do the same in the customerNames array

                // using a function to perform this swap would complicate this program a bit too much, but we
                //		can do the swap directly here quite easily
                char temp[201];
                // copy string from position i to the newly created temp variable
                strcpy(temp, customerNames[i]);
                // copy string from position j into position i
                strcpy(customerNames[i], customerNames[j]);
                // copy string from temp into position j
                strcpy(customerNames[j], temp);

            } // end of "if" statement

        } // end of second "for" loop

    } // end of first "for" loop
}

// specialised function that will display a row representing the sales data at a given array position
void printSalesDataAtPosition(int position)
{

    // here, we're using the value found at position i in the carModelPerSale array
    //	as a position for the carPrices array to find out the price of the type of ticket
    //	the customer purchased this sale
    int modelOfCar = carModelPerSale[position];
    // calculate the current sale's price
    float price = numberOfCarsPerSale[position] * carPrices[modelOfCar];

    // calculate the display text for the discount; this is for the user's benefit
    char discountGivenText[4];
    // if a discount was given, then...
    if (discountGivenPerSale[position] == true)
    {
        // set the display text to Yes
        strcpy(discountGivenText, "Yes");
        // and also modify the price
        price *= (1 - DISCOUNT_PERCENTAGE);
    }
    else
    {
        // set the display text to No
        strcpy(discountGivenText, "No");
    }

    // show the user the information related to each sale
    // I've broken down the "printf()" statement into several lines, so we
    //	can read it more easily, but it's still just one statement
    printf("Sale Index: %d | Sale Amount: %f | Car Model: %s | "
           "Price of Car: %f | Number of cars Sold: %hd | "
           "Discount Given: %s | Customer Name: %s\n",
            // please note that the above are three separate strings that will be glued together by the program
           position, price, carModels[modelOfCar],
           carPrices[modelOfCar], numberOfCarsPerSale[position],
           discountGivenText, customerNames[position]);
}

void menu_viewSales()
{
    // sort the arrays before showing the sales data
    sortArraysByNumberOfCarsSoldPerSale();

    /* Variables - these are only used inside menu_viewSales() Function and nowhere else */
    // these two will contain the sum the total sales price and total number of cars sold for all sales
    float totalSalesValue = 0;
    unsigned int carsSold = 0;

    printf("All Sales Data:\n\n");

    // set up a for loop that will execute the block of code as many times as
    // indicated by the numberOfSales variable's value
    for (int i = 0; i < numberOfSales; i++)
    {

        // here, we're using the value found at position i in the carModelPerSale array
        //	as a position for the carPrices array to find out the price of the type of ticket
        //	the customer purchased this sale
        int modelOfCar = carModelPerSale[i];
        // calculate the current sale's price
        float price = numberOfCarsPerSale[i] * carPrices[modelOfCar];

        // use this function to print the row of sales data at position i
        printSalesDataAtPosition(i);

        // also add up the total sales value and total number of cars sold
        totalSalesValue += price;
        carsSold += numberOfCarsPerSale[i];
    }

    // also display some overall statistics
    printf("We have sold a total of \n%hd cars valued to be %f GBP. We now have %hd cars unsold.\n",
           carsSold, totalSalesValue, carsAvailable);
}

void printSalesDataBasedOnCarsSoldBetweenMinMax(unsigned short min, unsigned short max)
{

    printf("Sales Data showing between %hd and %hd cars sold:\n\n", min, max);

    // searching through an array just means that you'll have a look that has an index
    //	variable starting with 0 and ending with the last position that contains a value
    //	in the array and then you'll have an if statement that check every value
    //		if a value is found to match a certain condition, it means that we found
    //		what we're looking for, so the code inside the {} will be executed as a response
    //		to us finding that value in the array

    // go through every position that will contain a value in our numberOfCarsPerSale array
    for (int i = 0; i < numberOfSales; i++)
    {

        // for every one of those positions, check if the value is between min and max
        if (min <= numberOfCarsPerSale[i] && numberOfCarsPerSale[i] <= max)
        {

            // print the sales data row at this position
            printSalesDataAtPosition(i);
        }
    }
}

void menu_viewSalesBetween()
{

    // sort the arrays before showing the sales data
    sortArraysByNumberOfCarsSoldPerSale();

    /* Variables - these are only used inside menu_viewSalesBetween() Function and nowhere else */
    unsigned short minCarsSold = getUnsignedShortFromConsole("What's the minimum number of cars sold you're interested in? Number = ");
    unsigned short maxCarsSold = getUnsignedShortFromConsole("What's the maximum number of cars sold you're interested in? Number = ");

    printSalesDataBasedOnCarsSoldBetweenMinMax(minCarsSold, maxCarsSold);
}

void menu_exit()
{
    printf("\n\nThank you for using our car sales application today. Have a pleasant day!");

    // ensure no data is lost, save it to the file
    saveDataToFile();
}

/* Starting Point of our Program */
int main()
{

    // see if there's any data in the file
    getDataFromFile();

    /* Variables - these are only used inside main() Function and nowhere else */
    // this will hold the user's choice when presented with the menu
    char userChoice;

    /* The rest of our Program */

    // loop/repeat the code between the immediately following curly brackets
    // for as long as the CONDITION found between the while()'s brackets
    // (see towards the end of void main() { ... }) evaluates to true; in our case,
    // we'll keep repeating for as long as the user does not choose to Exit
    // i.e. pushes 'x' when asked to choose their next action
    do
    {
        clearScreen();

        greetCustomer();
        showMenu();

        // the return value supplied by the Function once it finishes will be stored in userChoice
        userChoice = getCharFromConsole("Please provide your response here--> ");

        // for the user's benefit...
        clearScreen();

        // next, we check the user's choice and make a decision based on that
        switch (userChoice)
        {
            case MENU_OPTION_BUY_CARS:
                menu_buyCars();
                break;

            case MENU_OPTION_VIEW_SALES:
                menu_viewSales();
                break;

            case MENU_OPTION_VIEW_SALES_BETWEEN:
                menu_viewSalesBetween();
                break;

            case MENU_OPTION_EXIT_APPLICATION:
                menu_exit();
                break;
            default:
                printf("You have provided an unrecognized choice...\n");
                break;
        }

        // give the user a chance to read the previous output, allow them to continue when ready
        // this does not naturally exist in C
        pauseProgram(userChoice);

    } while (userChoice != MENU_OPTION_EXIT_APPLICATION);

    // again, keep the screen clear
    clearScreen();

    // a good sentence almost never fails to warm the heart...
    printf("\n\nEnjoy the rest of your day!\n\n");

    return 0;
}