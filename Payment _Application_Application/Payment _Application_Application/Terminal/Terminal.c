#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_WARNINGS

#include <stdio.h>
#include "Terminal.h"
#include <time.h>
#include <string.h>


#define Card_Year_Tens  3
#define Card_Year_Ones  4
#define Date_Year_Tens  8
#define Date_Year_Ones  9

#define Card_Mon_Tens  0
#define Card_Mon_Ones  1
#define Date_Mon_Tens  3
#define Date_Mon_Ones  4

#define Max_Amount 1000;

// DD/MM/YYYY format defines
#define _D      0
#define D_      1
#define DD_YY   2
#define _M      3
#define M_      4
#define MM_YYYY 5
#define _YYY    6
#define Y_YY    7
#define YY_Y    8
#define YYY_    9


EN_terminalError_t getTransactionDate(ST_terminalData_t* termData)
{
    uint8_t Curr_Day[3];
    uint8_t Curr_Mon[3];
    uint8_t Curr_Year[5];

    // Get System Date

    time_t Date_Time = time(NULL);                  // time_t is a long long typedef in time.h
    struct tm currDate = *localtime(&Date_Time);    // cuurDate is a struct that contains all the system Date and time info, 

    //sprintf sends formated output to a string
    sprintf(Curr_Day, "%d", currDate.tm_mday);           // Get Day from system and store it in Curr_Day
    sprintf(Curr_Mon, "%d", currDate.tm_mon + 1);        // Get Month from system and store it in Curr_Mon 
    sprintf(Curr_Year, "%d", currDate.tm_year + 1900);   //Get Year from system and store it in Curr_Year

    // Should be stored in this format - > DD/MM/YYYY

    // Storing DD
    if ((currDate.tm_mday) < 10)
    {
        (termData->Transaction_Date[_D]) = '0';
        (termData->Transaction_Date[D_]) = Curr_Day[0];
    }
    else {
        (termData->Transaction_Date[_D]) = Curr_Day[0];
        (termData->Transaction_Date[D_]) = Curr_Day[1];
    }

    (termData->Transaction_Date[DD_YY]) = '/';

    // Storing MM
    if ((currDate.tm_mon + 1) < 10)
    {
        (termData->Transaction_Date[_M]) = '0';
        (termData->Transaction_Date[M_]) = Curr_Mon[0];

    }
    else {
        (termData->Transaction_Date[_M]) = Curr_Mon[0];
        (termData->Transaction_Date[M_]) = Curr_Mon[1];
    }

    // Storing YYYY
    (termData->Transaction_Date[MM_YYYY]) = '/';
    (termData->Transaction_Date[_YYY]) = Curr_Year[0];
    (termData->Transaction_Date[Y_YY]) = Curr_Year[1];
    (termData->Transaction_Date[YY_Y]) = Curr_Year[2];
    (termData->Transaction_Date[YYY_]) = Curr_Year[3];
    (termData->Transaction_Date[10]) = '\0';

    if (termData->Transaction_Date == NULL)
    {
        return WRONG_DATE;
    }
    printf("Date:%s\n", termData->Transaction_Date);

    return TERMINAL_OK;
}

EN_terminalError_t isCardExpired(ST_cardData_t cardData, ST_terminalData_t termData)
{
    // cardExpirationDate MM/YY      transactionDate DD/MM/YYYY

    if ((cardData.Card_Expiration_Date[Card_Year_Tens]) < (termData.Transaction_Date[Date_Year_Tens])) // Year tens digit check        MM/_Y <> DD/MM/_Y
        return EXPIRED_CARD;

    if ((cardData.Card_Expiration_Date[Card_Year_Tens]) == (termData.Transaction_Date[Date_Year_Tens]))
    {
        if ((cardData.Card_Expiration_Date[Card_Year_Ones]) < (termData.Transaction_Date[Date_Year_Ones])) // Year ones digit check   MM/Y_ <> DD/MM/Y_
            return EXPIRED_CARD;

        if ((cardData.Card_Expiration_Date[Card_Year_Ones]) == (termData.Transaction_Date[Date_Year_Ones]))
        {
            if ((cardData.Card_Expiration_Date[Card_Mon_Tens]) < (termData.Transaction_Date[Date_Mon_Tens])) // Month tens check       _M/YY <> DD/_M/YY
                return EXPIRED_CARD;

            if ((cardData.Card_Expiration_Date[Card_Mon_Tens]) == (termData.Transaction_Date[Date_Mon_Tens]))
            {
                if ((cardData.Card_Expiration_Date[Card_Mon_Ones]) < (termData.Transaction_Date[Date_Mon_Ones])) // Month ones check   M_/YY <> DD/M_/YY
                    return EXPIRED_CARD;
            }
        }
    }
    printf("Your Card is NOT Expired\n");
    return TERMINAL_OK;
}

EN_terminalError_t isValidCardPAN(ST_cardData_t* cardData)
{
    /* Check PAN validity with luhn algorithm:
     * Drop the rightmost digit (checksum)
     * Starting from the right most digit double the vaule of every other digit
     * sum the digits and that sum%10 should be 0
     */

    uint16_t sum = 0;
    uint16_t PAN_length;
    uint16_t Count = 1;
    uint16_t PAN_digits[20] = { 0 };

    PAN_length = strlen(cardData->Primary_Account_Number);    // Get PAN length

    for (uint16_t i = PAN_length - 1; i > 0; i--)
    {
        if (Count % 2 == 0)                          // Double every other digit
        {
            uint16_t temp = (cardData->Primary_Account_Number[i] - '0') * 2;

            if ((temp / 10) > 0)                        // If the double value is a 2 digit number add them together
            {
                temp = (temp % 10) + 1;
                PAN_digits[i] = temp;
            }
            else
            {
                PAN_digits[i] = temp;
            }
        }
        else
        {
            PAN_digits[i] = (cardData->Primary_Account_Number[i] - '0');
        }
        Count++;
    }

    // The same as the above loop but for the first number ( index = 0 )
    if (Count % 2 == 0)
    {
        uint16_t temp = (cardData->Primary_Account_Number[0] - '0') * 2;;

        if ((temp / 10) > 0)
        {
            temp = (temp % 10) + 1;
            PAN_digits[0] = temp;
        }
        else
        {
            PAN_digits[0] = temp;
        }
    }
    else
    {
        PAN_digits[0] = (cardData->Primary_Account_Number[0] - '0');
    }

    // Sum all the digits
    for (uint16_t i = 0; i < PAN_length; i++)
    {
        sum += PAN_digits[i];
    }


    // Check for Validity
    if (sum % 10 == 0)
    {
        printf("Valid card\n");
        return TERMINAL_OK;
    }
    else
        return INVALID_CARD;

}

EN_terminalError_t getTransactionAmount(ST_terminalData_t* termData)
{
    // Get Transaction Amount and it should be > than 0

    printf("Please enter the Transaction Amount: ");

    scanf("%f", &termData->Transaction_Amount);
    Meow = termData->Transaction_Amount;
    if ((termData->Transaction_Amount) <= 0)
        return INVALID_AMOUNT;

    printf("Your Transaction Amount is: %.2f\n", termData->Transaction_Amount);
    return TERMINAL_OK;
}

EN_terminalError_t isBelowMaxAmount(ST_terminalData_t* termData)
{
    // Checks if the Transactions Amount is below the Max Amount

    if ((termData->Transaction_Amount) > (termData->Max_Transaction_Amount))
        return EXCEED_MAX_AMOUNT;

    return TERMINAL_OK;
}

EN_terminalError_t setMaxAmount(ST_terminalData_t* termData)
{
    // Sets Max Amount 

    termData->Max_Transaction_Amount = Max_Amount;
    Meow_1 = Max_Amount;
    printf("Max Transaction Amount is : %.2f\n", termData->Max_Transaction_Amount);

    if ((termData->Max_Transaction_Amount) <= 0)
        return INVALID_MAX_AMOUNT;

    return TERMINAL_OK;
}