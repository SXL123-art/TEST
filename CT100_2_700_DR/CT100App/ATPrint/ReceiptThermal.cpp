#include "ReceiptThermal.h"
#include "print_manager.h"

print_manager pm;


void* PrintReceiptThermal(void* p)
{		
    pthread_mutex_t mutex= ((CReceiptThermal*)p)->m_mutex;
    CDList<stReceiptData>* pReceiptBuf =((CReceiptThermal*)p)->m_pReceiptBuf; ;

    stReceiptData data;
    int		  len;
    bool	  bFormatOk=true;
    char     strTemp[128] ={0};

    unsigned int	timeouts=1000*1000;

    while(1)
    {
        usleep(timeouts);
        pthread_mutex_lock(&mutex);

        if(pReceiptBuf->GetCount() > 0)
        {
            timeouts = 10;
            data = pReceiptBuf->GetAt(0);
            switch(data.type)
            {
                case enString:
                    printf("string code!\n");
                    pm.print_string(data.str);
                    break;

                case enCenterString:
                    pm.print_string_center(data.str);
                    break;

                case enBigString:
                    //printf("enBigString : %s\n", data.str);
                    pm.print_string_bigsize(data.str);
                    break;

                case enLine:
                    // 打印一行线
                    pm.print_line();
                    break;

                case enNull:
                    // 打印空的一行，其实就是换行
                    pm.print_tireta();
                    break;

                case enBar:
                        printf("bar code!\n");
                        bFormatOk = true;
                        //打印1微码，第一个字符必须为字符d
                        len = strlen(data.str);
                        for(int i=0;i<len;i++){
                                if( ('0'<= data.str[i]) && (data.str[i]<='9')){

                                }else{
                                        printf("bar code string is error!\n");;
                                        bFormatOk = false;
                                        break;
                                }
                        }
                        if(bFormatOk){
                            sprintf(strTemp,"%s",data.str);
                            memset(data.str,0,PRINT_MAXSTRLEN);
                            data.str[0]='d';
                                memcpy(data.str+1, strTemp, len);

                                printf("dta:%s\n",data.str);
                                pm.print_barcode(data.str,0,0,0);
                        }
                        break;

                case enQR:
                        printf("qr code---!\n");
                        pm.print_qrcode(data.str);
                        break;

                case enImg:
                        printf("img code---!\n");
                        pm.Print_img();
                        break;

                case enStartPrint:
                        printf("realy begin print!\n");
                        pm.print_push();
                        pm.print_push();
                        pm.print_starts();
                        break;
            }

            pReceiptBuf->RemoveHead();
        }
        else
        {
                //没有打印内容，调整循环频率
                timeouts = 1000*1000;		//mx28
        }

        pthread_mutex_unlock(&mutex);
    }
}

CReceiptThermal::CReceiptThermal(void)
{
	pthread_mutex_init(&m_mutex, NULL);
	
	m_pReceiptBuf  = new CDList<stReceiptData>;
	
	pthread_t idPrintReceipt;
	
	pthread_create(&idPrintReceipt, NULL, PrintReceiptThermal, this);
}

CReceiptThermal::~CReceiptThermal(void)
{

}

void CReceiptThermal::init(void)
{
    printf("--------------CReceiptThermal initialize!\n");
    pm.init();
    int i;
    for(i = 0; i < 1; i++)
    {
        pm.print_string("");
        pm.print_push();
        pm.print_starts();
    }
}
int CReceiptThermal::SaveQrcode(char *str)
{
    pm.save_qrcode(str);
    return 0;
}

void CReceiptThermal::add(stReceiptData data)
{
    pthread_mutex_lock(&m_mutex);
    m_pReceiptBuf->AddTail(data);
    pthread_mutex_unlock(&m_mutex);
}

int CReceiptThermal::Print(void)
{
    stReceiptData data;

    data.type = enStartPrint;

    add(data);

    return 0;
}
	
int CReceiptThermal::PrintPaper(int nCow)
{
    for(int i=0; i<nCow; i++)
    {
        pm.print_string("");
        pm.print_push();
        pm.print_starts();
    }

    return 0;
}
	
