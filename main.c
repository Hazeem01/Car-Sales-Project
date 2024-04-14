#include <stdio.h>

#include <stdlib.h>

int main()
{
#define CAR_PRICE 8.5f
#define DISCOUNT_MIN_AGE 18
#define DISCOUNT_MAX_AGE 25
#define DISCOUNT_PERCENTAGE 0.20f
#define MAX_SALES 10
#define MENU_OPTION_BUY_CAR 'a'
#define MENU_OPTION_VIEW_SALES 'b'
#define MENU_OPTION_GIVE_FEEDBACK 'C'
#define MENU_OPTION_EXIT 'x'

    unsigned short carsAvailable = 20, carsNeeded, giveDiscount = 0, userAge = 0, carsSold = 0;
    float totalCost, totalSalesAmount = 0;
    char userResponse;
    float totalSales[MAX_SALES];
    int numberOfSales = 0;
    char customerNames[MAX_SALES][201];
    char feedbacks[200];


    do {
        system("cls");

        printf("Hello, Welcome to The Car Sales Company !\n\n");

        printf("Menu:\n");
        printf("a. Buy Car\n");
        printf("b. View Sales Stats\n");
        printf("c. Give Feedback\n");
        printf("x. Exit\n\n");

        printf("Please provide your response here: ");
        scanf(" %c", &userResponse);

        switch (userResponse) {
            case MENU_OPTION_BUY_CAR:
                printf("\n\n");
                if (carsAvailable > 0)
                {
                    printf("We currently have %hd cars available. Each of these cars cost %f GBP\n\n", carsAvailable, CAR_PRICE);
                } else {
                    printf("Sorry, we have no car left in the store\n\n");
                    break;
                }

                printf("What is your name?\nName: ");
                scanf(" %[^\n]", customerNames[numberOfSales]);

                printf("\nHow many cars would you be purchasing today ?\n");
                printf("Please provide your response here: ");
                scanf("%hd", &carsNeeded);

                if (carsNeeded > carsAvailable)
                {
                    printf("Sorry, but we only have %hd cars available.\n\n", carsAvailable);
                    break;
                }
                totalCost = carsNeeded * CAR_PRICE;

                printf("\n\nHow old are you ? \n");
                printf("Please provide your response here: ");
                scanf("%hd", &userAge);

                if ( userAge < 16)
                {
                    printf("\nYou need to be at least 16 years of age to get a car.... Kindly come back later");
                    break;
                } else if ( userAge >= DISCOUNT_MIN_AGE && userAge <= DISCOUNT_MAX_AGE)
                {
                    giveDiscount = 1;
                    totalCost *= (1 - DISCOUNT_PERCENTAGE);
                }

                switch (giveDiscount) {
                    case 0:
                        printf("\nYou did not qualify for the 18-25 discount :( \n\n");
                        break;
                    case 1:
                        printf("\nYaaaaay !!!.... You qualified for the automatic 18-25 discount from the shop! ;)\n\n");
                        break;
                    default:
                        printf("\nSorry, we can't determine if you qualified for any discount at this time\n\n");
                        break;
                }

                giveDiscount = 0;

                printf("Total cost of purchase would be %f GBP\n\n", totalCost);

                printf("Input \'a\' to continue with this purchase \nInput \'b\' to cancel this transaction\n");
                printf("Please provide your response here: ");
                scanf( " %c", &userResponse );

                switch (userResponse) {
                    case 'a':
                        carsAvailable -= carsNeeded;
                        totalSalesAmount += totalCost;
                        carsSold += carsNeeded;
                        totalSales[numberOfSales] = totalCost;
                        numberOfSales++;

                        printf("You have bought a total of %hd cars costing %f GBP.\n", carsNeeded, totalCost);

                        printf("We now have %hd cars left in the store\n\n", carsAvailable);
                        printf("Thanks for your purchase! Have a great day!\n");
                        break;

                    case 'b':
                        printf("Transaction terminated!\n\n");
                        printf("Thanks for stopping by today! Have a great day!");
                        break;

                    default:
                        printf("Please reply with only \'a\' or \'b\'... Transaction terminated :(");
                        break;
                }

            case MENU_OPTION_VIEW_SALES:

                totalSalesAmount = 0;

                for (int i = 0; i < numberOfSales; i++) {
                    printf("Sale Number: %d | Sale Amount: %f | Customer Name: %s\n", i, totalSales[i], customerNames[i]);

                    totalSalesAmount += totalSales[i];
                }

                printf("\n\n%hd cars have been sold with a total value of %f GBP.", carsSold, totalSalesAmount);
                break;

            case MENU_OPTION_GIVE_FEEDBACK:

                printf("Feedbacks:\n");
                printf("a. Give a feedback\n");
                printf("b. View feedbacks from other customers\n");
                break;

            case MENU_OPTION_EXIT:
                printf("\n\nThank you for visiting our store today. Have a pleasant day!");
                break;

            default:
                printf("\nPlease only provide inputs based on the options provided!");
                break;
        }

        if (userResponse == MENU_OPTION_EXIT) {
            printf("\n\nPress Enter to Exit...");
        }
        else {
            printf("\n\nPress Enter to return to the Menu...");
        }
        getchar();
        getchar();
    }  while (userResponse != MENU_OPTION_EXIT);

    system("cls");

    printf("\n\nThank you for visiting our store today. Have a pleasant day!");

    return  0;
}