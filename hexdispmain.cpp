#include <iostream>
#include <fstream>
#include <string>
#include <conio.h>
#include <iomanip>
#include <cstring>

using namespace std;

#define ROW_SIZE 16
#define COL_SIZE 16
#define LONG_SIZE sizeof(unsigned long)
 
int disp_proc(long pos, unsigned char buf[ROW_SIZE*COL_SIZE]);
unsigned long big_endian(unsigned long ul_local);

int main(int argc, char** argv) {
	string option1; 
	
	//�ж���û�д����ļ������� 
	if (argc >= 2){
		option1 = string(argv[1]);
	}
	else {
		cout << "Usage :\n";
		cout << argv[0] << " [filename]\n";
		cout << "\nInput the filename : ";
		cin >> option1;
	}
	
	////���ļ�����16�������, ���Խ������·�ҳ, һ�����ROW_SIZE*COL_SIZE���ֽ�
	
	//����ļ����Ƿ���ȷ 
	fstream if_hex(option1, ios_base::binary | ios_base::in | ios_base::out);
	if (not if_hex.is_open()){
		cerr << "The file can't be opened!!!\n";
		cerr << "Pls check the path and filename.\n";
		return 0;
	}
		
	//��ʼ�������, �޸����� 
	unsigned char data[ROW_SIZE*COL_SIZE];
	long pos = 0;
					
	while(true){
		if_hex.seekg(pos);
		if (if_hex.tellg() != -1){
			memset(data, 0x0, sizeof(data));
			if_hex.read((char *)data, sizeof(data));
			disp_proc(pos, data);					
		}			
	    cout << "\nPress the key 'j' 'k' 'g'  to browse; 'm' to modify; 'x' to exit ..." << endl;	
					
		switch (getch()) {
			case 'j':
				(pos > ROW_SIZE*COL_SIZE) ? pos -= ROW_SIZE*COL_SIZE : pos = 0;
				if_hex.clear();
				break;
			default:
			case 'k':
				if (if_hex.tellg() != -1) {
					pos = pos + ROW_SIZE*COL_SIZE; 
				}
				else {
					cout << "\n           ************ END OF FILE !!! ************ \n";
				}
				break;
				
			case 'g':
				cout << "\nInput the addr(hex) : ";
				cin >> hex >> pos;
				if_hex.clear();
				break;
			
			//�޸�ָ��λ�õ�����, �̶��޸ĳ���sizeof(unsigned long)				
			case 'm':			
				unsigned long ul_content;
				
				cout << "\nInput the addr(hex) : ";
				cin >> hex >> pos;
				
				cout << "\nInput the hex content(" << sizeof(ul_content) << " bytes) : ";
				cin >> hex >> ul_content;
				
				cout << "\nConfirm the pos : 0x" << pos;
				cout << "\nConfirm the content 0x" << hex << setw(LONG_SIZE*2) << right << setfill('0') << ul_content;
				
				//��д�л�ǰҪ���״̬, ���������д�������ɹ�
				if_hex.clear();  
				if_hex.seekp(pos);
				
				//��Ҫת����Bigendian��˴洢��ʽ 
				ul_content = big_endian(ul_content);
				if_hex.write(reinterpret_cast<char const*>(&ul_content), LONG_SIZE);
				break;
				
			case 'x':
			case 0x1b: // ESC
				return 0;
				break;
		} //end of switch
	} //end of while(true)

	return 0;
}

//��ʾһ�����ݿ� 
int disp_proc(long pos, unsigned char buf[ROW_SIZE*COL_SIZE]){
	int row, col;
	
	//��ʾһ�б���
	cout << "\n          " ;
	for (col = 0; col < COL_SIZE; col++)  cout << " " << setw(2) << setfill('0') << hex << uppercase << right << col;
	cout << "\n          " ;
	for (col = 0; col < COL_SIZE; col++)  cout << "---";
	
	//��ʾһ������ 
	for (row = 0; row < ROW_SIZE; row++){
		cout << "\n  " << hex << setw(6) << setfill('0') << uppercase << right << pos + row*COL_SIZE << "| ";
		for (col = 0; col < COL_SIZE; col++) 
			cout << " " << hex  << setw(2) << setfill('0') << uppercase << right << (unsigned int)buf[row*COL_SIZE + col];
		cout << "    ";
		for (col = 0; col < COL_SIZE; col++) 
			(buf[row*COL_SIZE + col] > 0x20 && buf[row*COL_SIZE + col] < 0x7f) ? cout << buf[row*COL_SIZE + col] : cout << ".";
	}
	return 0;	
}

//�ֽ���ת���ɴ�˸�ʽbigendian 
unsigned long big_endian(unsigned long ul_local){
	union {
		unsigned long ul_data;
		unsigned char uc_arr[LONG_SIZE];
	} stor;
	unsigned char uctemp;
	
	stor.ul_data = 0x1;
	if (stor.uc_arr[0] == 0x1) {
		//little endian case
		stor.ul_data = ul_local;
		for (int i = 0; i < (LONG_SIZE/2); i++){
			uctemp = stor.uc_arr[i];
			stor.uc_arr[i] = stor.uc_arr[LONG_SIZE - i - 1];
			stor.uc_arr[LONG_SIZE - i - 1] = uctemp;
		}
		return stor.ul_data;
	}
	else{
		//bit endian case
		return ul_local;
	}
}
