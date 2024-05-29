#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define DISCOUNT_MIN_AGE 18
#define DISCOUNT_MAX_AGE 25
#define DISCOUNT_PERCENTAGE 0.10f
#define MAX_SALES 100
#define MAX_STOCK 3
#define CSV_FILE "data.csv"
#define MENU_OPTION_BUY_CARS 'a'
#define MENU_OPTION_VIEW_SALES 'b'
#define MENU_OPTION_FILTER_SALES_STAT 'c'
#define MENU_View_Available_CARS 'd'
#define MENU_OPTION_EXIT_APPLICATION 'x'
#define FILE_OPENED 0
#define FILE_CLOSED 1
#define FILE_ERROR 2

unsigned short carsAvailable = 60;
unsigned short numberOfSales = 0;
unsigned char fileStatus = FILE_CLOSED;

typedef struct {
    char customerNames[201];
    char customerFeedbacks[201];
    unsigned short numberOfCarsPerSale;
    unsigned short carModelPerSale;
    bool discountGivenPerSale;
} Sale;

Sale sales[MAX_SALES];

char carModels[][20] = {"Honda Civic", "Toyota Corolla", "Toyota Prius", "Tesla Model X"};
char carYearOfManufacture[][20] = {"2020", "2021","2022", "2023"};
float carPrices[] = {3.5f, 4.0f, 3.4f, 8.5f};
int numberOfModelsInStock[] = {30,10, 15, 5};

FILE *file;

void clearScreen()
{
    system("cls");
}

void clearBuffer()
{
    while (getchar() != '\n')
        ;
}

void pauseProgram(char userChoice)
{
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
    printf("%s\n", message);
    scanf(" %c", &userInput);
    clearBuffer();
    return userInput;
}

unsigned short getUnsignedShortFromConsole(char *message)
{
    unsigned short userInput;
    printf("%s\n", message);
    printf("Please provide your response here-->");
    while (1)
    {
        if (scanf("%hu", &userInput) != 1 || userInput < 0)
        {
            clearBuffer();
            printf("Invalid input.\n");
            printf("Please provide your response here-->");
        }
        else
        {
            clearBuffer();
            return userInput;
        }
    }
}

void getStringFromConsole(char *message, char *str)
{
    printf("%s\n", message);
    printf("Please provide your response here-->");
    scanf(" %50[^\n]", str);
    clearBuffer();
}

void swapUnsignedShort(unsigned short *a, unsigned short *b)
{
    unsigned short temp;
    temp = *a;
    *a = *b;
    *b = temp;
}

void swapBool(bool *a, bool *b)
{
    bool temp;
    temp = *a;
    *a = *b;
    *b = temp;
}

FILE *createFile(char *fileName)
{
    file = fopen(fileName, "w");
    if (file != NULL)
    {
        fclose(file);
    }
    return file;
}

void openFile(char *fileName, char *mode)
{
    file = fopen(fileName, mode);

    if (file == NULL)
    {

        if (createFile(fileName) == NULL)
        {
            fileStatus = FILE_ERROR;

            printf("Error opening %s: %s", fileName, strerror(errno));
        }
        else
        {
            openFile(fileName, mode);
        }
    }
    else
    {
        fileStatus = FILE_OPENED;
    }
}

void closeFile()
{
    if (fileStatus == FILE_OPENED)
    {
        fclose(file);
        fileStatus = FILE_CLOSED;
    }
}

void readDataFromFile()
{
    int lineCounter = 0;

        while (1)
        {

            unsigned short numberOfCarsPerSaleValue = 0, carModelPerSaleValue = 0, discountGivenPerSaleValue = 0;
            char customerNameValue[201];
            char customerFeedbackValue[201];

            int scanResult = fscanf(
                    file,
                    "%hd,%hd,%hd,%[^\n],%[^\n]",
                    &numberOfCarsPerSaleValue,
                    &carModelPerSaleValue,
                    &discountGivenPerSaleValue,
                    customerNameValue, customerFeedbackValue);

            if (scanResult == EOF)
            {
                break;
            }

            sales[lineCounter].numberOfCarsPerSale = numberOfCarsPerSaleValue;
            sales[lineCounter].carModelPerSale = carModelPerSaleValue;
            sales[lineCounter].discountGivenPerSale = (bool)discountGivenPerSaleValue;
            strcpy(sales[lineCounter].customerNames, customerNameValue);
            strcpy(sales[lineCounter].customerFeedbacks, customerFeedbackValue);

            lineCounter++;
        }

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
        pauseProgram('_');
    }

    closeFile();
}

void writeDataToFile()
{
        for (int i = 0; i < numberOfSales; i++)
        {
            fprintf(file, "%hd,%hd,%hd,%s,%s\n",
                    sales[i].numberOfCarsPerSale,
                    sales[i].carModelPerSale,
                    sales[i].discountGivenPerSale,
                    sales[i].customerNames,
                    sales[i].customerFeedbacks);
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
        pauseProgram('_');
    }

    closeFile();
}

void greetCustomer()
{
    printf("Hello, Welcome to The Car Sales Company !\n\n");
}

void showMenu()
{
    printf("Menu:\n");
    printf("%c. Buy Car\n", MENU_OPTION_BUY_CARS);
    printf("%c. View Sales Stats\n", MENU_OPTION_VIEW_SALES);
    printf("%c. Filter Through Sales Stats\n", MENU_OPTION_FILTER_SALES_STAT);
    printf("%c. View Cars Available For Sale\n", MENU_View_Available_CARS);
    printf("%c. Exit\n\n", MENU_OPTION_EXIT_APPLICATION);
}

void menu_showCarModels()
{
    int numberOfCars = sizeof(carPrices) / sizeof(float);

    printf("\n              <<<Available Car Models>>>\n");
    printf("--------------------------------------------------------\n");
    printf("N/A    Model Name      Year Of Manufacture  Availability\n");

    for (int i = 0; i < numberOfCars; i++)
    {
        printf("%-3d    %-15s  %-20s  %-d    \n", i, carModels[i], carYearOfManufacture[i], numberOfModelsInStock[i]);
    }

    printf("--------------------------------------------------------\n");
}

float menu_applyDiscount(float currentPrice)
{
    return currentPrice * (1 - DISCOUNT_PERCENTAGE);
}

bool menu_isDiscountApplicable(unsigned short userAge)
{
    if (userAge >= DISCOUNT_MIN_AGE && userAge <= DISCOUNT_MAX_AGE)
    {
        return true;
    }
    return false;
}

void menu_printDiscountOutcome(bool giveDiscount)
{
    if (giveDiscount == true)
    {
        printf("\nYou qualified for a 20 percent discount !");
    }
    else
    {
        printf("\nSorry, you did not qualify for a discount :(");
    }
}

void menu_give_Feedback()
{
    clearScreen();
    char userFeedback[50];

    printf("<---Give Us A Feedback--->\n\n");

    printf("\nYour Feedbacks Makes Us Better :) \n");

    getStringFromConsole("\nGive your feedback below...", userFeedback);
    strcpy(sales[numberOfSales - 1].customerFeedbacks, userFeedback);
    printf("Thank you for your feedback!\nWe have received your feedback...");
}

void menu_viewStock()
{
    clearScreen();
    printf("<---Buy Car--->\n\n");

    if (carsAvailable > 0)
    {
        printf("We currently have a total of %hd cars available.\n\n", carsAvailable);
    }
    else
    {
        printf("Sorry, there is no car available in the store at this time");
        return;
    }

    menu_showCarModels();
}

void menu_buyCars()
{
    clearScreen();
    printf("<---Buy Car--->\n\n");

    if (carsAvailable > 0)
    {
        printf("We currently have %hd cars available.\n\n", carsAvailable);
    }
    else
    {
        printf("Sorry, there is no car available in the store at this time");
        return;
    }

    unsigned short carsNeeded, userAge, carModel;
    bool giveDiscount = false;
    float totalPrice;
    char discountStatus[4];
    char customerName[50];

    getStringFromConsole("What is your name?", customerName);

    menu_showCarModels();

    carModel = getUnsignedShortFromConsole("\nPlease provide the car model you want from the list above");

    carsNeeded = getUnsignedShortFromConsole("\nHow many cars would you be purchasing today ?");

    if (numberOfModelsInStock[carModel] < carsNeeded)
    {
        printf("Sorry, but we only have %hd cars of this model in stock.\n\n", numberOfModelsInStock[carModel]);
        return;
    }


    totalPrice = carsNeeded * carPrices[carModel];

    userAge = getUnsignedShortFromConsole("\nHow old are you ? ");

    if (userAge < 18) {
        printf("\nYou need to be at least 18 to purchase a car");
        return;
    }

    giveDiscount = menu_isDiscountApplicable(userAge);

    if (giveDiscount == true)
    {
        totalPrice = menu_applyDiscount(totalPrice);
        strcpy(discountStatus, "Yes");
    } else {
        strcpy(discountStatus, "No");
    }

    menu_printDiscountOutcome(giveDiscount);
    printf("\nYour order detail is shown below:\n");
    printf("Total Cost Of Purchase: %f GBP (after any discount) | Model Of Car Purchased: %s | "
           "Unitary Cost Of Car: %f GBP | Number Of Cars Purchased: %hd | "
           "Discount Applicable: %s\n",
            totalPrice, carModels[carModel],
           carPrices[carModel], carsNeeded,
           discountStatus);

    char proceedWithTransactionResponse;

    while (true) {
        proceedWithTransactionResponse = getCharFromConsole("\nInput \'Y\' to continue with this purchase \nInput \'N\' to cancel this transaction ");
        if (proceedWithTransactionResponse == 'Y') {

            strcpy(sales[numberOfSales].customerNames, customerName);

            sales[numberOfSales].numberOfCarsPerSale = carsNeeded;

            sales[numberOfSales].carModelPerSale = carModel;

            sales[numberOfSales].discountGivenPerSale = giveDiscount;

            strcpy(sales[numberOfSales].customerFeedbacks, "");

            carsAvailable -= carsNeeded;
            numberOfModelsInStock[carModel] -= carsNeeded;

            printf("\nYou have bought a total of %hd cars costing %f GBP.\n", carsNeeded, totalPrice);
            printf("We now have %hd cars left in the store\n\n", carsAvailable);
            numberOfSales++;

            printf("Would you love to tell us how we performed ?\n");

            char provideFeedbackResponse;

            while (true) {
                provideFeedbackResponse = getCharFromConsole("\nInput \'Y\' to give feedback \nInput \'N\' to return to the main menu ");
                if (provideFeedbackResponse == 'Y') {
                    menu_give_Feedback();
                    printf("Thanks for your purchase! Have a great day!\n");
                    return;
                }
                else if (provideFeedbackResponse == 'N') {
                    printf("Thanks for your purchase! Have a great day!\n");
                    return;
                }
                else {
                    printf("You have provided an unrecognized response...\n");
                }
            };
        }
        else if (proceedWithTransactionResponse == 'N') {
            printf("Transaction Terminated!");
            return;
        }
        else {
            printf("You have provided an unrecognized response...\n");
        }
    };
}

void sortArraysByNumberOfCarsSoldPerSale()
{

    for (int i = 0; i < numberOfSales - 1; i++)
    {

        for (int j = i + 1; j < numberOfSales; j++)
        {

            if (sales[i].numberOfCarsPerSale > sales[j].numberOfCarsPerSale)
            {

                swapUnsignedShort(&sales[i].numberOfCarsPerSale, &sales[j].numberOfCarsPerSale);
                swapUnsignedShort(&sales[i].carModelPerSale, &sales[j].carModelPerSale);

                swapBool(&sales[i].discountGivenPerSale, &sales[j].discountGivenPerSale);

                char temp[201];
                strcpy(temp, sales[i].customerNames);
                strcpy(sales[i].customerNames, sales[j].customerNames);
                strcpy(sales[j].customerNames, temp);

            }

        }

    }
}

void printSalesDataAtPosition(int position)
{

    int modelOfCar = sales[position].carModelPerSale;
    float price = sales[position].numberOfCarsPerSale * carPrices[modelOfCar];

    char discountGivenText[4];
    if (sales[position].discountGivenPerSale == true)
    {
        strcpy(discountGivenText, "Yes");
        price *= (1 - DISCOUNT_PERCENTAGE);
    }
    else
    {
        strcpy(discountGivenText, "No");
    }

    printf("Sale Index: %-3d | Final Sale Amount (after any discount): %-10.2f | Car Model: %-15s | "
           "Unitary Price of Car: %-10.2f | Number of cars Sold: %-3hd | "
           "Discount Given: %-3s | Customer Name: %-s\n\n",
           position, price, carModels[modelOfCar],
           carPrices[modelOfCar], sales[position].numberOfCarsPerSale,
           discountGivenText, sales[position].customerNames);

}

void menu_viewSales()
{
    clearScreen();
    printf("<---View Sales Stats--->\n\n");
    sortArraysByNumberOfCarsSoldPerSale();

    float totalSalesValue = 0;
    unsigned int carsSold = 0;

    printf("\n                                              <<<All Sales Data>>>\n");
    printf("--------------------------------------------------------------------------------------------------------------------\n");

    for (int i = 0; i < numberOfSales; i++)
    {

        int modelOfCar = sales[i].carModelPerSale;
        float price = sales[i].numberOfCarsPerSale * carPrices[modelOfCar];

        printSalesDataAtPosition(i);

        totalSalesValue += price;
        carsSold += sales[i].numberOfCarsPerSale;
    }
    printf("--------------------------------------------------------------------------------------------------------------------\n");

    printf("We have sold a total of \n%hd cars valued to be %f GBP. We currently have %hd cars available for sale.\n",
           carsSold, totalSalesValue, carsAvailable);
}

void printSalesDataBasedOnCarsSoldBetweenMinMax(unsigned short min, unsigned short max)
{

    printf("Sales Data showing between %hd and %hd cars sold:\n\n", min, max);

    for (int i = 0; i < numberOfSales; i++)
    {

        if (min <= sales[i].numberOfCarsPerSale && sales[i].numberOfCarsPerSale <= max)
        {

            printSalesDataAtPosition(i);
        }
    }
}

void menu_viewSalesBetween()
{
    clearScreen();
    printf("<---Filter Through Sales Stats--->\n\n");
    sortArraysByNumberOfCarsSoldPerSale();

    unsigned short minCarsSold = getUnsignedShortFromConsole("What's the minimum number of cars sales you're interested in? ");
    unsigned short maxCarsSold = getUnsignedShortFromConsole("What's the maximum number of cars sold you're interested in? ");

    printSalesDataBasedOnCarsSoldBetweenMinMax(minCarsSold, maxCarsSold);
}

void menu_exit()
{
    clearScreen();
    printf("\n\nThank you for using our car sales application today. Have a pleasant day!");

    saveDataToFile();
}

int main()
{

    getDataFromFile();

    char userChoice;

    do
    {
        clearScreen();

        greetCustomer();
        showMenu();

        userChoice = getCharFromConsole("Please provide your response here-->");

        clearScreen();

        switch (userChoice)
        {
            case MENU_OPTION_BUY_CARS:
                menu_buyCars();
                break;

            case MENU_OPTION_VIEW_SALES:
                menu_viewSales();
                break;

            case MENU_OPTION_FILTER_SALES_STAT:
                menu_viewSalesBetween();
                break;

            case MENU_View_Available_CARS:
                menu_viewStock();
                break;

            case MENU_OPTION_EXIT_APPLICATION:
                menu_exit();
                break;
            default:
                printf("You have provided an unrecognized choice...\n");
                break;
        }
        pauseProgram(userChoice);

    } while (userChoice != MENU_OPTION_EXIT_APPLICATION);

    clearScreen();

    printf("\n\nEnjoy the rest of your day!\n\n");

    return 0;
}