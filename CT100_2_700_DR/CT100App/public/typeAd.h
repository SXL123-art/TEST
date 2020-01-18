#ifndef TYPEAD_H
#define TYPEAD_H

#define kilo		1000      //
//称重显示相关
typedef struct
{
    long kg,kgtara;
    char stabok ;
    char zerook ;
    char taraok ;
    char overok ;
}adc_ret;
//称重相关
typedef struct adc_init_param
{
    int gl; // local g value :db store
    int gd; // destination g value :db store
    int unload; //unload internal adc code :db store
    int load;   // load internal adc code :db store
    int full_arrange; //full arragne,3kg,6kg,15kg,30kg,60kg,150kg,300kg,600kg,1000kg :db store
    int cal_arrange;
    int is_dual; //sigial or dual arrange :db store
    int e; //db store
    int e1;
}adc_param;
//#define 	MINADINTERVAL		300	//这个值最小值
#define 	MINADINTERVAL		150	//这个值最小值


#define 	AD_KOIL			100

#define 	ADSTABE_INIT_TIMES	25
#endif // TYPEAD_H
