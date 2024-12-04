#include <bits/stdc++.h>
#include "myfdlibm.h"
#include <gmp.h>
#include <mpfr.h>
using namespace std;

ofstream ofs;

void RMSE(vector <double> &ieee, vector<double> &pos){
    long double sum1 = 0.0L, sum2 = 0.0L;
    int count = 0;
    for(int i = 0; i < 1000; i++){
        if(pos[i] == -1.0) continue;
        sum1 += ieee[i] * ieee[i];
        sum2 += pos[i] * pos[i];
        count++;
        ofs << i << " " << pos[i] * pos[i] << " " << sum2 << "\n\n";
    }
    cout << setw(10) << "Posit: " << sqrtl((sum2/count)) << "\n";
    cout << setw(10) << "IEEE754: " << sqrtl((sum1/count)) << "\n";
    ofs.close();
}

string Difference(string& num1, string& num2) {
    string result = "";
    
    int n1 = num1.length();
    int n2 = num2.length();
    if(n2 != n1 || num1[0] != num2[0]) return "-1.0";

    string str1 = num1;
    string str2 = num2;
    if(str1 < str2) str1.swap(str2);
    
    reverse(str1.begin(), str1.end());
    reverse(str2.begin(), str2.end());
    

    int carry = 0;

    for (int i = 0; i < n2; i++) {
        if(str1[i] == '.'){
            result.push_back('.');
            continue;
        }

        int sub = ((str1[i] - '0') - (str2[i] - '0') - carry);

        if (sub < 0) {
            sub += 10;
            carry = 1;
        } else {
            carry = 0;
        }

        result.push_back(sub + '0');
    }

    for (int i = n2; i < n1; i++) {
        int sub = ((str1[i] - '0') - carry);

        if (sub < 0) {
            sub += 10;
            carry = 1;
        } else {
            carry = 0;
        }

        result.push_back(sub + '0');
    }

    reverse(result.begin(), result.end());

    return result;
}

string MPFR(double input){
    mpfr_t x, result;
    char buffer[20];

    mpfr_init2(x, 256);
    mpfr_init2(result, 256);
    
    mpfr_set_d(x, input, MPFR_RNDN); 
    mpfr_acos(result, x, MPFR_RNDN);

    mpfr_sprintf(buffer, "%.18Rf", result);
    string num1(buffer);

    mpfr_clear(x);
    mpfr_clear(result);

    return num1;
}

string POSIT(double input){
    string num2;
    Posit64 ans = Posit_acos(input);
    ostringstream out;

    out << fixed << setprecision(21) << ans;
    num2 = out.str();
    int length = num2.length();
    size_t site = num2.find('.');
    
    int prec = length - site - 1;
    while(prec > 18){
        num2.pop_back();
        prec--;
    }
    out.str("");
    
    return num2;
}

string IEEE754(double input){
    string num3;
    double ans = acos(input);
    ostringstream out;
    out << fixed << setprecision(18) << ans;
    num3 = out.str();
    return num3;
}

void Run(mt19937 generator, double *interval){
    uniform_int_distribution<int>  site(0, 2);
    mpfr_t result;
    int target;
    double input;
    string num1, num2, num3;
    vector<double> IEEE, POS;
    ofs.open("output.txt");

    for(int i = 0; i < 1000; i++){
        target = site(generator);                             
        uniform_real_distribution<double> range(0, 1);  
        input = range(generator);

        num1 = MPFR(input);
        num2 = POSIT(input);
        num3 = IEEE754(input);

        if(num1[0] == '-'){
            num1.erase(0, 1);
            num2.erase(0, 1);
            num3.erase(0, 1);
        }

        ofs << i << "\n";
        ofs << input << "\n";
        ofs << setw(10) << "MPFR: " << num1 << "\n";
        ofs << setw(10) << "Posit: " << num2 << "\n";
        ofs << setw(10) << "IEEE754: " << num3 << "\n\n";
        
        double Posit_result = stod(Difference(num1, num2));
        double IEEE754_result = stod(Difference(num1, num3));
        IEEE.push_back(IEEE754_result);
        POS.push_back(Posit_result);
        
        //cout  << setw(10) << "Posit_D: " << fixed << setprecision(15) << Posit_result << "\n";
        //cout  << setw(10) << "IEEE_D: " << fixed << setprecision(15) << IEEE754_result << "\n\n";
    }
    RMSE(IEEE, POS);
}

int main(){
    string num1;
    random_device rd;
    mt19937 generator(rd());           
    double interval[11] = {0, 1, 10, 1e2, 1e3, 1e4, 1e5, 1e6, 1e7, 1e8, 1e9};       

    Run(generator, interval);
}
