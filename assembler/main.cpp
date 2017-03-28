#include<iostream>
#include<cstdlib>
#include<fstream>
#include<string>
#include<sstream>

using namespace std;

string motpot_file = "/home/amb/code/assembler/files/MOTPOT.txt";
string pot_file = "/home/amb/code/assembler/files/POT.txt";
string input_file = "/home/amb/code/assembler/files/input.txt";
string symbols_file = "/home/amb/code/assembler/files/SSTO.txt";
string getsymbols_file = "/home/amb/code/assembler/files/SGET.txt";
string regs_file = "/home/amb/code/assembler/files/registers.txt";
string p1_file = "/home/amb/code/assembler/files/p1.txt";
string p2_file = "/home/amb/code/assembler/files/p2.txt";
string pextra_file = "/home/amb/code/assembler/files/p1_1.txt";

struct regs
{
	int val;
};

int main(int argc, char** argv)
{
	string line;
	int addr = 0;
	int s = 0;
	char sep = ',';	
	char delim = ' ';
	int counter = 0;
	int ir = 0,br;

	struct regs *r;
	r = new regs[16];

	r[0].val = 0;

	ifstream motpotload, inputload, potload;
	fstream sload, regload, p1load, p2load, peload, sgetload; 
	
	motpotload.open(motpot_file.c_str());
	inputload.open(input_file.c_str());
	potload.open(pot_file.c_str());
	sload.open(symbols_file.c_str());
	sgetload.open(getsymbols_file.c_str());
	p1load.open(p1_file.c_str());	
	p2load.open(p2_file.c_str());
	peload.open(pextra_file.c_str());

	cout<<"The assembly language program is : "<<endl<<endl;

	while(getline(inputload, line))
	{
		cout<<line<<endl;
	}
	cout<<endl;

	cout<<"-----PASS 1-----"<<endl<<endl;

	inputload.clear();
        inputload.seekg(0, ios::beg);

	while(getline(inputload, line))
	{
		counter++;

		string label, op, operand1, operand2;
		string l1;
		string usg, balr;

		stringstream ss(line);

		getline(ss, label, delim);
		getline(ss, op, delim);
		getline(ss, operand1, sep);
		getline(ss, operand2);

		motpotload.clear();
		motpotload.seekg(0, ios::beg);
		
		while(getline(motpotload,l1))
		{
			string op1, sval, opcode, type;
	
			stringstream ss1(l1);

			getline(ss1, op1, delim);
			getline(ss1, sval, delim);
			getline(ss1, opcode, delim);
			getline(ss1, type);

			if(op == "USING")
                        {
                                br = atoi(operand2.c_str());
				r[br].val = addr;
				s = 0;
				break;
                        }
                        else if(op == "BALR")
                        {
				s = 0;
				break;
                        }
			else if(op == op1)
			{				
				s = atoi(sval.c_str());	
				
				p1load<<addr<<" "<<opcode<<" "<<operand1<<","<<operand2<<endl;
							
				break;
			}
			else if(op == "DC")
			{
				s = 4;
			}
			else if(op == "DS")
			{
				s = 4;
			}
			else
			{
				s = 0;
			}
		}

		cout<<addr<<" "<<line<<endl;

		peload<<addr<<" "<<line<<endl;
		
		addr = addr + s;

	}
	cout<<endl;

	peload.clear();
	peload.seekg(0, ios::beg);

	while(getline(peload, line))
	{
		stringstream ss2(line);
		char q = '\'';

		string a,label, op, operand1, operand2;

		getline(ss2, a, delim);
		getline(ss2, label, delim);
		getline(ss2, op, delim);
		getline(ss2, operand1, q);
		getline(ss2, operand2, q);

		if(op == "DC")
		{
			sload<<label.c_str()<<" "<<atoi(operand2.c_str())<<" "<<atoi(a.c_str())<<endl;
		}
		else if(op == "DS")
		{
			sload<<label.c_str()<<" "<<" "<<atoi(a.c_str())<<endl;			
		}
	}		

	cout<<"The base register is : R"<<br<<endl;
	cout<<"The value of the base register is : "<<r[br].val<<endl;

	cout<<"The index register is : R"<<ir<<endl;
	cout<<"The value of the index register is : "<<r[ir].val<<endl<<endl<<endl;
	

	cout<<"-----PASS 2-----"<<endl<<endl;

	p1load.clear();
	p1load.seekg(0, ios::beg);

	while(getline(p1load,line))
	{
		stringstream ss3(line);

		string a, opcode, operand1, operand2;
		string l2;

		getline(ss3, a, delim);
		getline(ss3, opcode, delim);
		getline(ss3, operand1, sep);
		getline(ss3, operand2);

		sload.clear();
		sload.seekg(0, ios::beg);

		while(getline(sload,l2))
		{
			stringstream ss4(l2);

			string symbol, val, offset;

			getline(ss4, symbol, delim);
			getline(ss4, val, delim);
			getline(ss4, offset);

			if(operand2 == symbol)
			{
				sgetload<<symbol<<" "<<val<<" "<<offset<<" "<<opcode<<" "<<a<<" "<<operand1<<endl;

			}
		}	
	}


	sgetload.clear();
	sgetload.seekg(0, ios::beg);

	while(getline(sgetload,line))
	{
		string symbol,val,offset,opc,ad, orand;

		stringstream ss5(line);

		getline(ss5, symbol, delim);
		getline(ss5, val, delim);
		getline(ss5, offset, delim);
		getline(ss5, opc, delim);
		getline(ss5, ad, delim);	
		getline(ss5, orand);

		p2load<<ad<<" "<<opc<<" "<<orand<<sep<<offset<<"("<<ir<<sep<<br<<")"<<endl;

		
	}

	p2load.clear();
	p2load.seekg(0, ios::beg);

	while(getline(p2load, line))
	{
		cout<<line<<endl;
	}
	cout<<endl;	
	
	return 0;
}
