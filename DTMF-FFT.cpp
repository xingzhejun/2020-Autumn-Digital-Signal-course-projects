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


//以下为第一次作业的DIT部分
class Complex
{
public:
	Complex(double r = 0, double i = 0) :r(r), i(i) {};
	~Complex() {};
	Complex& operator =(const Complex& c)
	{
		this->r = c.r;
		this->i = c.i;		return *this;
	}
	Complex operator +(Complex& c)
	{
		return Complex(this->r + c.r, this->i + c.i);
	}
	Complex operator -(Complex& c)
	{
		return Complex(this->r - c.r, this->i - c.i);
	}
	Complex operator *(Complex& c)
	{
		return Complex(this->r * c.r - this->i * c.i, this->i * c.r + this->r * c.i);
	}
	bool operator ==(const Complex& c)
	{
		return this->r == c.r && this->i == c.i;
	}
	friend ostream& operator <<(ostream& out, Complex& c)
	{
		out << c.r << " + j( " << c.i << " )" << endl;
		return out;
	}
	friend istream& operator >>(istream& in, Complex& c)
	{
		in >> c.r >> c.i;
		return in;
	}
	double getr() { return r; }
	double geti() { return i; }
	double r;
	double i;
};

Complex polar_c(double rou, double angle)
{
	return Complex(rou * cos(angle), rou * sin(angle));
}


complex<double>* DIT(complex<double>input[], int L)
{
	int i, j, k, n;
	int N;
	for (N = 1;; N++)//增加部分
	{
		if ((1 << N == L))
			break;
	}
	Complex w, temp1, temp2;
	Complex* x = new Complex[L];
	for (i = 0; i < L; i++)
	{
		x[i].r = input[i].real();
		x[i].i = input[i].imag();
	}

	//逆序计算
	int* rev = new int[L];
	rev[0] = 0;
	for (i = 0; i < L; i++)
		rev[i] = rev[i / 2] / 2 + (i & 1) * (1 << (N - 1));

	//DIT		
	Complex* X_dit = new Complex[L];
	for (i = 0; i < L; i++)
		X_dit[i] = x[i];
	for (i = 0; i < L; i++)
	{
		if (i < rev[i])
		{
			temp1 = X_dit[i];
			X_dit[i] = X_dit[rev[i]];
			X_dit[rev[i]] = temp1;
		}
	}
	for (i = 0; i < N; i++)
	{
		for (j = 0; j < L; j = j + (1 << (i + 1)))
		{
			w = Complex(1.0, 0);
			for (k = j; k < j + (1 << i); k++)
			{
				temp1 = X_dit[k + (1 << i)];
				temp1 = w * temp1;
				X_dit[k + (1 << i)] = X_dit[k] - temp1;
				X_dit[k] = X_dit[k] + temp1;
				temp2 = polar_c(1.0, -2 * PI / (2 << i));
				w = w * temp2;
			}
		}
	}
	complex<double>* x_dit = new complex<double>[L];
	for (i = 0; i < L; i++)
	{
		x_dit[i] = X_dit[i].r + 1i;
		x_dit[i].imag(X_dit[i].i);
	}
	return x_dit;
}


// 主函数，新写
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
	int i, j;
	int num, L;
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
		complex<double>* output = new complex<double>[L];
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
		output = DIT(input, L);//调用DIT

		//寻峰，识别
		for (j = 0; j < L / 2; j++)
		{
			am = pow(output[j].real(), 2) + pow(output[j].imag(), 2);
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
		double step = double(fs) / (L - 1);
		double* x = new double[L];
		for (j = 0; j < L; j++)
			x[j] = j * step;
		output_symbol = find_s(x[pos1], x[pos2]);
		end = clock();

		cout << fileName << endl;
		cout << "数字: " << output_symbol << endl;
		cout << "耗时: " << (double)(end - begin) / CLOCKS_PER_SEC << "秒" << endl;
		cout << endl;
	}
	return 0;
}




