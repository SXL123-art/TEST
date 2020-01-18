#include "Ad.h"
#include <stdlib.h>
#include <stdio.h>

CAdc::CAdc()
{
    stabok = 0;
    zerook = 0;
    taraok = 0;
    overok = 0;
    kgtara = 0;
    kgzero = 0;

    kg =0;
    kg1=0;
    kgzero=0;
    kgtara=0;
    MAXKG=0;
    MAX_KG1=0;
    MAX_E=0;MAX_E1=0;MAX_CAL=0;MAX_DIV=0;MAX_KG=0;MAX_TARA=0;MAX_ZERO=0;
    MAX_AUTOZERO=0;MAX_AUTOZERODIS=0;MAX_ON=0;MINKG=0;

    gl=0; // local g value :db
    gd=0; // destination g value :db
    unload=0; //unload internal adc code :db
    load=0;   // load internal adc code :db
    full_arrange=0; //full arragne,3kg,6kg,15kg,30kg,60kg,150kg,300kg,600kg,1000kg :db
    cal_arrange=0;
    is_dual=0; //sigial or dual arrange :db
    e=0; //db
    e1=0;

    cala=0.0f;calb=0.0f;x=0.0f;
}

int CAdc::getkg(byte key)
{
    static byte stabcount;
    uint8 s;//stable
    uint8 z;//zero
    unsigned long Ad_Raw_Val = 0;
    Ad_Raw_Val = AD_Val_Handle();

	if(Ad_Raw_Val == 0 || Ad_Raw_Val== 5999){
        return 0;
    }

	//cout<<Ad_Raw_Val<<endl;
    kg = (long)(Ad_Raw_Val * cala - calb);
    //cout<<"kg*cals -calb"<<kg<<endl;
    
    if(labs(kg - kg1) <= MAX_AUTOZERODIS)
    {
        stabcount++;
    }
    else
    {
        stabcount = 0;
        s = 0;
        stabok = 0;
    }
	// 苏州智能秤，需要根据稳定状态来自动保存交易数据，所以将稳定状态时间判断缩短到5，修改之前是20
        if(stabcount > 5)
	// if(stabcount > 20)//10ms(脟脭录枚陆脟脟脿陆脙掳拢)*20*stable(defulat:5) = 1 sec
    {
        stabok = 1;
    }
	
    if(stabcount > 4)
    {
        s = 1;
        //stabcount = 200;
    }

	kg1 = kg;
    if (kg > (MAX_KG + kgzero)){
		overok = 1;
    }else{
        overok = 0;
    }
#if 0	
    if(key == 1) //key zero
    {
	// printf("taraok:--%d--%d--%d--%d\n", taraok, stabok, labs(kg), MAX_ZERO);
        //if(taraok) return 0;
        if((labs(kg) < MAX_ZERO) && stabok) {
			kgzero = kg;
			printf("kgzero:%d\n",kgzero);
			return 1;
		}else{
			return 0;
		}
    }
#else
	if(key == 1) //key zero
    {
        //if(taraok) return 0;
        if((labs(kg) < MAX_ZERO) && stabok) {
#if AD_GATE_ZERO_HAVETATA
			if(kgtara>0){	
				kgzero = kg-kgtara;
			}else{
				kgzero = kg;
			}
#else
			kgzero = kg;
#endif
			printf("kgzero:%d\n",kgzero);
			return 1;
		}else{
			return 0;
		}
    }
#endif
    //---- autozero
    if(labs(kg) < MAX_ZERO)
    {
        if((labs(kg - kgzero) <= MAX_AUTOZERO) && (s)){
			kgzero = kg1;
			//printf("autozero kgzero:%d\n",kgzero);
        }
    }
	
    //----
    //cout<<"kg:"<<kg<<"kgzero:"<<kgzero<<endl;
    kg = kg - kgzero;
    z = (labs(kg) <= (MAX_AUTOZERODIS * 4));//KYK tare solve contidion extension
    zerook = z | (labs(kg - kgtara) <= MAX_AUTOZERODIS);
    taraok = !(labs(kgtara) <= MAX_AUTOZERO);

    //----
    if(key == 2)// key tara
    {
        if((labs(kg) <= MAX_TARA) && stabok)
        {
#if AD_GATE_TARA_NEGATIVE
			if((kg+kgtara)<0){
				return 0;
			}
#else			      
            if((kg < kgtara) && (!zerook)){
		   		return 0;
           	}
#endif			
            kgtara = adc_read_weight(kg);//kg is internal ad, return is external weight

            if(z){ 
				kgtara = 0;
			}
        }else{
			return 0;
        }
    }

    kg -= kgtara;
    kg = adc_read_weight(kg);
    kg /= MAX_DIV;
    return 1;
}

int CAdc::getInternalCode()
{
    unsigned long Ad_Raw_Val = AD_Val_Handle();
    //cout<<Ad_Raw_Val<<endl;
    return Ad_Raw_Val;
}

long CAdc::adc_read_weight(long internal_weight)
{

    long weight;

    weight = internal_weight;
    if(is_dual)
    {
        if(labs(weight) < MAX_KG1)
        {
            weight = roundl(weight, MAX_E);
            //CURE = MAX_E/MAX_DIV;
        }
        if(labs(weight) >= MAX_KG1)
        {
            weight = roundl(weight, MAX_E1);
            //CURE = MAX_E1/MAX_DIV;
        }
    }
    else
    {
        weight = roundl(weight, MAX_E);
        //CURE = MAX_E/MAX_DIV;
    }
    return weight;
}
long CAdc::roundl(long x, long y)
{
    char b = 0;
    if( x < 0 ) { x = -x; b = 1; }
    x = ( x / y + ( ( ( x % y ) * 2 ) >= ( ( y * 2 ) >> 1 ) ) ) * y;
    return b ? -x : x;
}
adc_param CAdc::adc_set_default()
{
    gl = 97940;
    gd = 97940;
    unload = 9000;
    load = 20000;
    full_arrange=30;
    cal_arrange = 0;
    e = 5;
    e = 10;
    adc_param adcp;
    adcp.gl = gl;
    adcp.gd = gd;
    adcp.unload = unload;
    adcp.load = load;
    adcp.full_arrange = full_arrange;
    adcp.cal_arrange = cal_arrange;
    adcp.is_dual = is_dual;
    adcp.e = e;
    adcp.e1 = e1;
    return adcp;
}
/*
void CAdc::adc_set_default(SQLiteWrapper * sqlite)
{
    gl = 97940;
    gd = 97940;
    unload = 9000;
    load = 20000;
    full_arrange=30;
    e = 5;
    point_position = 3;
    password = 13579;
    round = 10;

}
*/
void CAdc::load_param(adc_param  adcparam)
{
//ctor
    adcinit();
    //cout<<"init"<<endl;
    ////////////////////////
    gl = adcparam.gl;
    gd = adcparam.gd;
    unload = adcparam.unload;
    load = adcparam.load;
    full_arrange = adcparam.full_arrange;
    cal_arrange = adcparam.cal_arrange;
    is_dual = adcparam.is_dual;
    e = adcparam.e;
    e1 = adcparam.e1;
    //cout<<gl<<endl;
    //cout<<unload<<endl;
    //cout<<load<<endl;
    //cout<<full_arrange<<endl;
    //cout<<e<<endl;
    long full_count = 60000;
    switch(full_arrange)
    {
	case 1000:
            MAX_KG1 = 1600000;
            full_count = 4000000;
	     break;
	case 600:
            MAX_KG1 = 960000;
            full_count = 2400000;
	     break;
	case 300:
            MAX_KG1 = 480000;
            full_count = 1200000;
	     break;
        case 150:
            MAX_KG1 = 240000;
            full_count = 600000;
            break;
        case 60:
            MAX_KG1 = 30000;
            break;
        case 30:
            MAX_KG1 = 30000;
            break;
        case 20:
            MAX_KG1 = 30000;
            break;
        case 15:
            MAX_KG1 = 24000;
            break;
        case 10:
            MAX_KG1 = 24000;
            break;
        case 6:
            MAX_KG1 = 30000;
            break;
        case 3:
            MAX_KG1 = 30000;
            break;
    }
    MAX_KG = full_arrange * 1000;
    MAX_DIV = full_count / MAX_KG;
    //cout<<"max kg:"<<MAX_KG<<endl;
    //cout<<"max div:"<<MAX_DIV<<endl;

    MAX_E 	= e * MAX_DIV;
    //cout<<MAX_E<<endl;
    if(is_dual)
    {
        MAX_E1 = e1 * MAX_DIV;
    }
    else
    {
        MAX_E1 = e * MAX_DIV;
    }
    MAX_KG = (full_count + 9 * MAX_E1);

    //MAX_KG1 = 30000;
    MAX_TARA = (MAX_KG1 - MAX_E1);

    MAX_ZERO = full_count / 10lu;
    MAX_AUTOZERO = (MAX_E / 2);
    //MAX_AUTOZERODIS = (MAX_E / 4);
    MAX_AUTOZERODIS = (MAX_E / 8);
    MINKG = 0;
    MAX_TARA = full_count;

    if(cal_arrange > 0 && full_arrange % cal_arrange  == 0)
        load = (load-unload)*full_arrange/cal_arrange + unload;
    //cout<<load<<endl;
    cala = (float)full_count / (load - unload);
    calb = cala * unload;
    x = ((float)gl) / ((float)gd);
    cala *= x;
    calb *= x;
    //cout<<cala<<endl;
    //cout<<calb<<endl;
}

/*
void CAdc::load_param(SQLiteWrapper * sqlite)
{

    SQLiteStatement* stmt=sqlite->Statement("select * from scale");
    if(stmt->NextRow())
    {
    // std::cout<<stmt->ValueInt(0)<<" - "<<stmt->ValueFloat(1)<<std::endl;
    unload=stmt->ValueInt(1);
    load=stmt->ValueInt(2);
    gl=stmt->ValueInt(3);
    gd=stmt->ValueInt(4);
    full_arrange=stmt->ValueInt(5);
    is_dual=stmt->ValueInt(6);
    e=stmt->ValueInt(7);
    round=stmt->ValueInt(8);
    password=stmt->ValueInt(9);

    }
    delete(stmt);
}
*/
adc_param CAdc::save_param()
{
    adc_param adcp;
    adcp.gl = gl;
    adcp.gd = gd;
    adcp.unload = unload;
    adcp.load = load;
    adcp.full_arrange = full_arrange;
    adcp.cal_arrange = cal_arrange;
    adcp.e = e;
    adcp.e1 = e1;
    return adcp;
}
/*
void CAdc::save_param(SQLiteWrapper * sqlite)
{
    SQLiteStatement* stmt=sqlite->Statement("update scale set upload=? ,load=? ,gl=?,gd=?,full_arrange=?,is_dual=?,e=?,round=?,password=?");
    stmt->Bind(0,unload);
    stmt->Bind(1,load);
    stmt->Bind(2,gl);
    stmt->Bind(3,gd);
    stmt->Bind(4,full_arrange);
    stmt->Bind(5,is_dual);
    stmt->Bind(6,e);
    stmt->Bind(7,round);
    stmt->Bind(8,password);
    stmt->Execute();
    delete(stmt);
}
*/

CAdc::~CAdc()
{
    //dtor
}
