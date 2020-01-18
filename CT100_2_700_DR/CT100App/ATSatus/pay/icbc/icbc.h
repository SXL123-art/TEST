#ifndef ICBC_H
#define ICBC_H

using namespace std;

typedef struct bank_trade_request
{
    // 公共请求参数
    string url;
    string app_id;
    string msg_id;
    string timestamp;

    // 请求参数
    string attach;
    string mer_id;
    string store_code;
    string tporder_create_ip;
    string out_trade_no;
    string order_amt;
    string trade_date;
    string trade_time;

    string auth_code;

    // 退款接口参数
    string reject_no;
    string reject_amt;
}bankTradeRequest;

typedef struct bank_trade_response
{
    int return_code;
    //string return_code;
    string return_msg;
    string return_staus;
    string code;
    string msg;
    string qr_code;
    string tradeNo;
    string card_no;
    string total_amt;
    string point_amt;
    string ecoupon_amt;
    string mer_disc_amt;
    string coupon_amt;
    string bank_disc_amt;
    string payment_amt;
    string total_disc_amt;
}bankTradeResponse;

int bankTradePrecreate(bankTradeRequest* request, bankTradeResponse* response);			// 预下单
int bankTradePay(bankTradeRequest* request, bankTradeResponse* response);				//扫码
int bankTradeQuery(bankTradeRequest* request, bankTradeResponse* response);				// 查询
int bankTradeRefund(bankTradeRequest* request, bankTradeResponse* response);			// 退款
int bankTradeRefundQuery(bankTradeRequest* request, bankTradeResponse* response);		// 退款查询
int bankTradeCancel(bankTradeRequest* request, bankTradeResponse* response);

#endif // ICBC_H
