# include <fstream>
# include <string>
# include <stdio.h>
# include <stdlib.h>
# include <ctime>
# include <sstream>
# include <iostream>
#include<complex>

# define PI 3.1415926
int const fs = 8000;
using namespace std;
double fre[] = { 697,770,852,941,1209,1336,1477,1633 };


double* Goertzel(complex<double> input[], int L)
{
	complex<double> unit;
	complex<double>* WN = new complex<double>[L];
	complex<double>* v = new complex<double>[L];
	complex<double> target[8];
	double* am = new double[8];
	double wk;
	int i, j, k;

	unit.real(cos(2 * PI / L));
	unit.imag(-sin(2 * PI / L));
	WN[0] = 1;
	for (i = 1; i < L; i++)	
		WN[i] = WN[i - 1] * unit;

	for (i = 0; i < 8; i++)
	{
		k = L * fre[i] / fs;
		wk = 2 * PI * fre[i] / fs;

		v[0] = input[0];
		v[1] = input[1] + 2 * cos(wk) * input[0];
		for (j = 2; j < L; j++)		
			v[j] = (input[j] + (2 * cos(wk) * v[j - 1])) - (v[j - 2]);
		target[i] = (v[L - 1] - ((WN[k] * v[L - 2])));
	}

	for (i = 0; i < 8; i++)	
		am[i] = pow(target[i].real(), 2) + pow(target[i].imag(), 2);

	return am;
}


char find_s(double a, double b)
{
	double freqs1[] = { 697,770,852,941 };
	double freqs2[] = { 1209,1336,1477,1633 };
	char symbol1[] = { '1','2','3','A','4','5','6','B','7','8','9','C','*','0','#','D' };

	if (a > b)
	{
		int temp = a;
		a = b;
		b = temp;
	}

	int x = 100;
	int y = 100;
	for (int i = 0; i < 4; i++)
	{
		if (abs(freqs1[i] - a) <= 10)
		{
			x = i;
			break;
		}
	}
	for (int i = 0; i < 4; i++)
	{
		if (abs(freqs2[i] - b) <= 10)
		{
			y = i;
			break;
		}
	}

	if (x == 100 || y == 100)
	{
		char error_char = 'x';
		return error_char;
	}

	char symbol = symbol1[x * 4 + y];
	return symbol;
}


int main()
{
	std::string* name = new std::string[10];
	name[0] = "data1081.txt";
	name[1] = "data1107.txt";
	name[2] = "data1140.txt";
	name[3] = "data1219.txt";
	name[4] = "data1234.txt";
	name[5] = "data1489.txt";
	name[6] = "data1507.txt";
	name[7] = "data1611.txt";
	name[8] = "data1942.txt";
	name[9] = "data1944.txt";
	double audio[10000];
	clock_t begin, end;
	int num, L;
	int i, j;
	int am;
	int max1;
	int max2;
	int pos1;
	int pos2;
	char output_symbol;
	for (i = 0; i < 10; i++)
	{
		num = 0;
		j = 0;
		max1 = 0;
		max2 = 0;
		pos1 = 0;
		pos2 = 0;
		std::string fileName = name[i];
		std::ifstream fstr(fileName.c_str());
		std::string lineStr;
		std::stringstream sstr;
		std::string seg_path;
		double data;
		memset(&audio, 0, sizeof(audio));

		while (std::getline(fstr, lineStr))
		{
			audio[j] = atof(lineStr.c_str());
			sstr >> seg_path >> data;
			sstr.clear();
			num++;
			j++;
		}

		begin = clock();

		// 补零
		int addzero = 0;
		for (j = 1; j < 20; j++)
		{
			if (pow(2, j) < num)			
				continue;
			else if (pow(2, j) == num)		
				break;
			else
			{
				addzero = pow(2, j) - num;
				break;
			}
		}
		L = num + addzero;

		complex<double>* input = new complex<double>[L];
		for (j = 0; j < L; j++)
		{
			if (j >= num)
				input[j] = 0;
			else
			{
				input[j].real(audio[j]);
				input[j].imag(0);
			}
		}
		double* output = Goertzel(input, L);

		//寻峰，识别
		for (j = 0; j < 8; j++)
		{
			am = output[j];
			if (am > max2)
			{
				if (am > max1)
				{
					max2 = max1;
					pos2 = pos1;
					max1 = am;
					pos1 = j;
				}
				else
				{
					max2 = am;
					pos2 = j;
				}
			}
		}

		output_symbol = find_s(fre[pos1], fre[pos2]);

		end = clock();

		cout << fileName << endl;
		cout << "数字: " << output_symbol << endl;
		cout << "耗时: " << (double)(end - begin) / CLOCKS_PER_SEC << "秒" << endl;
		cout << endl;
	}
	return 0;
}