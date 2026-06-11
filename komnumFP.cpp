#include <iostream>
#include <string>
#include <cmath>
#include <iomanip>
#include <algorithm>
#include <stdexcept>

using namespace std;

class MathParser {
    string expr;
    size_t pos;
    double x_val;

    double parsePrimary() {
        if (pos >= expr.length()) return 0;
        char ch = expr[pos];
        
        if (ch == 'x' || ch == 'X') {
            pos++;
            return x_val;
        }
        if (ch == '(') {
            pos++;
            double res = parseAddSub();
            if (pos < expr.length() && expr[pos] == ')') pos++;
            return res;
        }
        
        size_t start = pos;
        while (pos < expr.length() && (isdigit(expr[pos]) || expr[pos] == '.')) pos++;
        if (start == pos) throw runtime_error("Error");
        return stod(expr.substr(start, pos - start));
    }

    double parseExponent() {
        double left = parsePrimary();
        while (pos < expr.length() && expr[pos] == '^') {
            pos++;
            double right = parseUnary();
            left = pow(left, right);
        }
        return left;
    }

    double parseUnary() {
        if (pos >= expr.length()) return 0;
        if (expr[pos] == '+') {
            pos++;
            return parseUnary();
        }
        if (expr[pos] == '-') {
            pos++;
            return -parseUnary();
        }
        return parseExponent();
    }

    double parseMulDiv() {
        double left = parseUnary();
        while (pos < expr.length() && (expr[pos] == '*' || expr[pos] == '/')) {
            char op = expr[pos++];
            double right = parseUnary();
            if (op == '*') left *= right;
            else {
                if (right == 0) left = NAN;
                else left /= right;
            }
        }
        return left;
    }

    double parseAddSub() {
        double left = parseMulDiv();
        while (pos < expr.length() && (expr[pos] == '+' || expr[pos] == '-')) {
            char op = expr[pos++];
            double right = parseMulDiv();
            if (op == '+') left += right;
            else left -= right;
        }
        return left;
    }

public:
    double evaluate(string expression, double x) {
        expr = expression;
        expr.erase(remove_if(expr.begin(), expr.end(), ::isspace), expr.end());
        pos = 0;
        x_val = x;
        return parseAddSub();
    }
};

int main() {
    MathParser parser;
    string rumus_g;
    double x0, x_true;
    int max_iter;

    cout << "=============================================================================" << endl;
    cout << "                 PANDUAN INPUT RUMUS METODE ITERASI SATU TITIK               " << endl;
    cout << "=============================================================================" << endl;
    cout << " 1. MASUKKAN FUNGSI g(x), BUKAN f(x)." << endl;
    cout << "    Contoh: x^2 + 5x + 6 = 0 diubah menjadi x = (-x^2 - 6)/5" << endl;
    cout << "    Maka yang diketik di terminal adalah: (-x^2 - 6)/5" << endl;
    cout << " 2. Wajib gunakan tanda '*' untuk perkalian (Contoh: 10*x^2, BUKAN 10x^2)" << endl;
    cout << " 3. Wajib gunakan tanda '^' untuk pangkat    (Contoh: x^3 atau x^2)" << endl;
    cout << " 4. Gunakan kurung '()' untuk memisahkan pembilang dan penyebut pada pecahan" << endl;
    cout << "=============================================================================" << endl;
    cout << "Masukkan rumus g(x): ";
    getline(cin, rumus_g);
    
    cout << "Masukkan nilai x0 [tebakan awal]: ";
    cin >> x0;
    
    cout << "Masukkan nilai x sebenarnya [ketik 0 jika tidak ada]: ";
    cin >> x_true;
    
    cout << "Masukkan batas maksimum iterasi: ";
    cin >> max_iter;

    cout << "\n=============================================================================" << endl;
    cout << " " << setw(7) << "Iterasi" 
         << " | " << setw(14) << "x_i" 
         << " | " << setw(14) << "x_(i+1)" 
         << " | " << setw(14) << "Et (%)" 
         << " | " << setw(14) << "Ea (%)" << endl;
    cout << "=============================================================================" << endl;

    double x_i = x0;
    double x_next, Et, Ea;

    cout << fixed << setprecision(2);

    for (int i = 1; i <= max_iter; i++) {
        try {
            x_next = parser.evaluate(rumus_g, x_i);
        } catch (...) {
            cout << "Gagal menghitung. Pastikan penulisan rumus benar." << endl;
            return 1;
        }

        if (isnan(x_next) || isinf(x_next)) {
            cout << " " << setw(7) << i 
                 << " | " << setw(14) << x_i 
                 << " | " << setw(14) << "Divergen/NaN" 
                 << " | " << setw(14) << "-" 
                 << " | " << setw(14) << "-" << endl;
            break;
        }

        if (x_true != 0) {
            Et = abs((x_true - x_next) / x_true) * 100.0;
        } else {
            Et = 0;
        }

        if (x_next != 0) {
            Ea = abs((x_next - x_i) / x_next) * 100.0;
        } else {
            Ea = 0;
        }

        cout << " " << setw(7) << i 
             << " | " << setw(14) << x_i 
             << " | " << setw(14) << x_next 
             << " | " << setw(14) << Et 
             << " | " << setw(14) << Ea << endl;

        x_i = x_next;
    }
    
    cout << "=============================================================================" << endl;

    return 0;
}