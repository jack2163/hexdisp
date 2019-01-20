#include <iostream>
#include <fstream>
#include <string>
#include <conio.h>
#include <iomanip>
#include <cstring>

using namespace std;

#define ROW_SIZE 16
#define COL_SIZE 16
 
int disp_proc(long pos, unsigned char buf[ROW_SIZE*COL_SIZE]);

int main(int argc, char** argv) {
	string option1; 
	
	//判断有没有带入文件名参数 
	if (argc >= 2){
		option1 = string(argv[1]);
	}
	else {
		cout << "\nUsage : hexdisp [filename]";
		cout << "\n\nInput the filename : ";
		cin >> option1;
	}
	
	////对文件进行16进制浏览, 可以进行上下翻页, 一次浏览ROW_SIZE*COL_SIZE个字节
	
	//检查文件名是否正确 
//	ifstream if_hex(option1, ios_base::binary);
	fstream if_hex(option1, ios_base::binary | ios_base::in | ios_base::out);
	if (not if_hex.is_open()){
		cout << "\nThe file can't be opened!!!";
		cout << "\nPls check the path and filename.";
		return 0;
	}
//	if_hex.exceptions(std::ifstream::failbit);
		
	//开始进行浏览, 修改内容 
	unsigned char data[ROW_SIZE*COL_SIZE];
	long pos = 0;
	
	//修改内容时, 需要输入的内容 
//	long length = 0;
//	int  index = 0;
//	short buf[COL_SIZE];
//	char  confirm_modify;
					
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
					cout << "\n           ************ END OF FILE !!! ************" << endl;
				}
				break;
				
			case 'g':
				cout << "\nInput the addr(hex) : ";
				cin >> hex >> pos;
				if_hex.clear();
				break;
				
			case 'm':			
//				cout << "\nInput the addr(hex) : ";
//				cin >> hex >> pos;
//				cout << "\nInput the length(hex) : ";
//				cin >> dec >> length;
//				if (length > COL_SIZE) length = COL_SIZE;
//				cout << "\nInput the content : ";
//				for (index = 0; index < length; index++){
//					cin >> hex >> buf[index];
//					if (buf[index] > 0xFF) cout << "\nERROR : " <<  index+1 << " byte value > 0xFF";
//					break;
//				}
//				cout << "\nPls confirm the content : \n    ";
//				for (index = 0; index < length; index++) cout << " " << hex << buf[index];
//				cout << "\nDo you confirm to modify ? [Y/N] : ";
//				cin >> confirm_modify;
//				if (confirm_modify == 'Y' || confirm_modify == 'y'){
//					//修改指定位置的内容
//					if_hex.seekg(pos);
//					if_hex.write((char *)buf, length);
//					cout << "OK : have modified the content\n";
//				}
//				else{
//					//放弃修改内容
//					cout << "NOTE : canceled the modify content!\n";
//				} 
				unsigned long ul_content;
				cout << "\nInput the addr(hex) : ";
				cin >> hex >> pos;
				cout << "\nInput the hex content(" << sizeof(ul_content) << " bytes) : ";
				cin >> hex >> ul_content;
				
				cout << "\nConfirm the pos : 0x" << pos;
				cout << "\nConfirm the content 0x" << hex << setw(8) << right << setfill('0') << ul_content;
				
				if_hex.clear(); //读写切换前要清除状态, 否则后续的写操作不成功 
				if_hex.seekp(pos);
				if_hex.write(reinterpret_cast<char const*>(&ul_content), sizeof(ul_content));
				break;
				
			case 'x':
			case 0x1b: // ESC
				return 0;
				break;
		} //end of switch
	} //end of while(true)

	return 0;
}

//显示一块内容块 
int disp_proc(long pos, unsigned char buf[ROW_SIZE*COL_SIZE]){
	int row, col;
	
	//显示一行标题
	cout << "\n          " ;
	for (col = 0; col < COL_SIZE; col++)  cout << " " << setw(2) << setfill('0') << hex << uppercase << right << col;
	cout << "\n          " ;
	for (col = 0; col < COL_SIZE; col++)  cout << "---";
	
	//显示一块内容 
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
