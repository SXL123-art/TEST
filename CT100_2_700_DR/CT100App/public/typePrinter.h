#ifndef TYPE_PRINTER_H
#define TYPE_PRINTER_H


typedef enum tagENUMPRINT
{
	enString=0,
	enCenterString,
	enBigString,
	enLine,
	enNull,
	enBar,
	enQR,
	enImg,
	enLable,
	enLableString,
	enStartPrint
}enPrintType;

/*#define	MAXROWDATALEN		128
typedef struct tagPRINTER
{
	char  rowdata[MAXROWDATALEN];	//打印行数据
}stPrinter,*pstPrinter;

typedef struct tagPRINTER2
{
	wchar_t  rowdata[MAXROWDATALEN];	//打印行数据
}stPrinter2,*pstPrinter2;
*/

#define PRINT_MAXSTRLEN			128
typedef struct _tagSTRECEIPTData
{	
	enPrintType		type;
	char			str[PRINT_MAXSTRLEN];
}stReceiptData,*pstReceiptData;

/*typedef struct
{
	char Lable1[32];
	char Lable2[32];
	char Lable3[32];
	char Lable4[32];
	char Lable5[32];
	char Lable6[32];
	char Barcode[64];
	char QrCode[64];
} PrintLableData;*/

#endif

