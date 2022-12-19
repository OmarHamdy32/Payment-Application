#ifndef CARD_H
#define CARD_H

typedef unsigned char       uint8_t;
typedef signed char         int8_t;
typedef unsigned short      uint16_t;
typedef signed short	    int16_t;
typedef unsigned int		uint32_t;
typedef signed int			int32_t;
typedef unsigned long long  uint64_t;
typedef signed long long    int64_t;


typedef struct ST_cardData_t
{
	uint8_t Card_Holder_Name[25];
	uint8_t Primary_Account_Number[20];
	uint8_t Card_Expiration_Date[6];
}ST_cardData_t;

ST_cardData_t Meow_2;


typedef enum EN_cardError_t
{
	Card_OK,		//0
	WRONG_NAME,		//1
	WRONG_EXP_DATE, //2
	WRONG_PAN		//3
}EN_cardError_t;

EN_cardError_t getCardHolderName(ST_cardData_t* cardData);
EN_cardError_t getCardExpiryDate(ST_cardData_t* cardData);
EN_cardError_t getCardPAN(ST_cardData_t* cardData);


#endif // !CARD_H
