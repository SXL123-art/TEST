/*
打印控制类
方法介绍：
        void print_starts();  打印开始
        void print_string(char * str); 向打印缓存中添加一行字符串 gbk格式如果有汉字
        void print_string_center(char * str); //同上，但是居中
        void print_barcode(char * str,long kg, long pr, long to);打印一维条形码，后三个参数填写1,不用理会
        void print_qrcode(char * str);打印二维码，如有汉字，请gbk格式
        void print_line(); 打印一条横线
        void print_push(); 打印最后，多走一些纸，方便撕下
        void print_tireta();打印空行
方法调用：
        print_manager pm;
        pm.print_string_center("济南市肉菜流通");
        pm.print_tireta();
        pm.print_line();
        pm.print_string("追溯码C：12345678901234567890");
        pm.print_barcode("d12345678901234567890",10l,20l,1l);
        pm.print_string("遇见薄D");
        pm.print_qrcode("12345678901234567890");
*/

#ifndef PRINT_MANAGER_H
#define PRINT_MANAGER_H

using namespace std;

class print_manager
{
public:
	print_manager();
	virtual ~print_manager();
	void print_starts();
	void print_string(char * str);
	void print_string_center(char * str);
	void print_string_bigsize(char * str);
	void print_barcode(char * str,long kg, long pr, long to);
	void print_qrcode(char * str);
	void save_qrcode(char * str);
	void Print_img(void);
	void print_line();
	void print_push();
	void print_tireta();
	int init();

	long print_noPaper();
	long print_temp();

	// Test by DQ
	int TestPrint(char *str);
	
protected:
private:
};

#endif // PRINT_MANAGER_H
