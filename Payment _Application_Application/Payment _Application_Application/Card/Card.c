#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "Card.h"

#define MaxCardHolderName 24
#define MinCardHolderName 20
#define ExpiryDateStrLen  5
#define MaxCardPAN 19
#define MinCardPAN 16


EN_cardError_t getCardHolderName(ST_cardData_t* cardData)
{
    printf("Please enter the Card Holder's Name: ");

    scanf_s("%[^\n]s", &cardData->Card_Holder_Name, MaxCardHolderName + 1); // scanf_s prevents overflow 
    // Check on the Card Holder's Name format
    if (strlen(cardData->Card_Holder_Name) < MinCardHolderName || (cardData->Card_Holder_Name) == NULL)
        return WRONG_NAME;

    strcpy(Meow_2.Card_Holder_Name , cardData->Card_Holder_Name);


    printf("Hello, %s\n", cardData->Card_Holder_Name);

    return Card_OK;
}

EN_cardError_t getCardExpiryDate(ST_cardData_t* cardData)
{
    printf("Please enter the card Expiry Date in the format of MM/YY: ");

    // Using <ctype.h> to check on the Expiry Date format
    scanf_s("%s", &cardData->Card_Expiration_Date, ExpiryDateStrLen + 1);

    if (strlen(cardData->Card_Expiration_Date) != ExpiryDateStrLen || (cardData->Card_Expiration_Date) == NULL)
        return WRONG_EXP_DATE;

    if (!isdigit(cardData->Card_Expiration_Date[0]) || !isdigit(cardData->Card_Expiration_Date[1]))
        return WRONG_EXP_DATE;

    if ((cardData->Card_Expiration_Date[2]) != '/')
        return WRONG_EXP_DATE;

    if (!isdigit(cardData->Card_Expiration_Date[3]) || !isdigit(cardData->Card_Expiration_Date[4]))
        return WRONG_EXP_DATE;

    printf("The Expiry Date is: %s\n", cardData->Card_Expiration_Date);

    strcpy(Meow_2.Card_Expiration_Date, cardData->Card_Expiration_Date);

    return Card_OK;
}

EN_cardError_t getCardPAN(ST_cardData_t* cardData)
{
    printf("Please enter the Card PAN: ");

    scanf_s("%s", &cardData->Primary_Account_Number, MaxCardPAN + 1);

    // Check on the Card PAN format
    if (strlen(cardData->Primary_Account_Number) < MinCardPAN || (cardData->Primary_Account_Number) == NULL)
    {
        return WRONG_PAN;
    }
    printf("Card PAN number is: %s\n", cardData->Primary_Account_Number);

    strcpy(Meow_2.Primary_Account_Number, cardData->Primary_Account_Number);

    return Card_OK;
}