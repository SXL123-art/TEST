/*
��ӡ������
�������ܣ�
        void print_starts();  ��ӡ��ʼ
        void print_string(char * str); ���ӡ���������һ���ַ��� gbk��ʽ����к���
        void print_string_center(char * str); //ͬ�ϣ����Ǿ���
        void print_barcode(char * str,long kg, long pr, long to);��ӡһά�����룬������������д1,�������
        void print_qrcode(char * str);��ӡ��ά�룬���к��֣���gbk��ʽ
        void print_line(); ��ӡһ������
        void print_push(); ��ӡ��󣬶���һЩֽ������˺��
        void print_tireta();��ӡ����
�������ã�
        print_manager pm;
        pm.print_string_center("�����������ͨ");
        pm.print_tireta();
        pm.print_line();
        pm.print_string("׷����C��12345678901234567890");
        pm.print_barcode("d12345678901234567890",10l,20l,1l);
        pm.print_string("������D");
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
