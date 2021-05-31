#include <iostream>
using namespace std;

string lookahead;
long stringsub;

string getToken();
void MatchToken(string expected);

void ParseS();
void ParseE();
void ParseR();
void ParseT();
void ParseP();
void ParseD();

string getToken() {
    char inchar;
    inchar = getchar();
//    cout << "inchar>" << inchar << "<inchar" << endl;
    stringsub += 1;
    switch(inchar) {
        case '+': {
            return "plus";
        }
        case '-': {
            return "minus";
        }
        case '*': {
            return "times";
        }
        case '/': {
            return "slash";
        }
        case '(': {
            return "lparen";
        }
        case ')': {
            return "rparen";
        }
        case '\n': {
            return "END";
        }
        case ' ': {
            return getToken();
        }
        default: {
            if (inchar >= 48 && inchar <= 57) {
                long value = 0;
//                cout << inchar << ": " << long(inchar - 48) << endl;
                value = long(inchar - 48);
                inchar = getchar();
                stringsub += 1;
                while (inchar) {
                    if (inchar >= 48 && inchar <= 57) {
                        value = value * 10 + long(inchar - 48);
                        inchar = getchar();
                        stringsub += 1;
                    }
                    else if (inchar == '+' || inchar == '-' || inchar == '*' || inchar == '/' || inchar == '(' || inchar == ')' || inchar == '\n' || inchar == ' ') {
                        ungetc(inchar, stdin);
                        stringsub -= 1;
//                        cout << to_string(value) << endl;
                        return "number";
                    } else {
                        cout << "[error@" + to_string(stringsub) + "] Illegal coming char after number." << endl;
                        exit(0);
                    }
                }
            } else if (inchar >= 'A' && inchar <= 'z') {
                string identname;
                identname += inchar;
                inchar = getchar();
                stringsub += 1;
                while (inchar) {
                    if ((inchar >= 48 && inchar <= 57) || (inchar >= 'A' && inchar <= 'z')) {
                        identname += inchar;
                        inchar = getchar();
                        stringsub += 1;
                    }
                    else if (inchar == '+' || inchar == '-' || inchar == '*' || inchar == '/' || inchar == '(' || inchar == ')' || inchar == '\n' || inchar == ' ') {
                        ungetc(inchar, stdin);
                        stringsub -= 1;
//                        cout << identname << endl;
                        return "ident";
                    } else {
                        cout << "[error@" + to_string(stringsub) + "] Illegal coming char after number." << endl;
                        exit(0);
                    }
                }
            } else {
                cout << "[error@" + to_string(stringsub) + "] Illegal coming char." << endl;
                exit(0);
            }
        }
    }
}

void MatchToken(string expected) {
    if (lookahead != expected) {
        cout << "[error@" + to_string(stringsub) + "] Illegal coming phase'." << endl;
        exit(0);
    } else {
        lookahead = getToken();
    }
}

void ParseS() {
    cout << "S -> E" << endl;
    ParseE();
    cout << "[Result] Accepted." << endl;
}

void ParseE() {
    cout << "E -> TR" << endl;
    ParseT();
    ParseR();
}

void ParseR() {
    if (lookahead == "plus") {
        cout << "R -> +TR" << endl;
        MatchToken("plus");
        ParseT();
        ParseR();
    } else if (lookahead == "minus") {
        cout << "R -> -TR" << endl;
        MatchToken("minus");
        ParseT();
        ParseR();
    } else if (lookahead != "plus" && lookahead != "minus") {
        cout << "R -> epsilon" << endl;
    }
}

void ParseT() {
    cout << "T -> DP" << endl;
    ParseD();
    ParseP();
}

void ParseP() {
    if (lookahead == "times") {
        cout << "P -> *DP" << endl;
        MatchToken("times");
        ParseD();
        ParseP();
    } else if (lookahead == "slash") {
        cout << "P -> /DP" << endl;
        MatchToken("slash");
        ParseD();
        ParseP();
    } else if (lookahead != "times" && lookahead != "slash") {
        cout << "P -> epsilon" << endl;
    }
}

void ParseD() {
    if (lookahead == "lparen") {
        cout << "D -> (E)" << endl;
        MatchToken("lparen");
        ParseE();
        if (lookahead == "rparen") {
            MatchToken("rparen");
        } else {
            cout << "[error@" + to_string(stringsub) + "] Detected '(E', the coming phase should be ')'." << endl;
            exit(0);
        }
    } else if (lookahead == "ident") {
        cout << "D -> ident" << endl;
        MatchToken("ident");
    } else if (lookahead == "number") {
        cout << "D -> number" << endl;
        MatchToken("number");
    } else {
        cout << "[error@" + to_string(stringsub) + "] For D, the coming phase should be an ident or a number." << endl;
        exit(0);
    }
}


int main() {
    stringsub = 0;
    lookahead = getToken();
    ParseS();
    return 0;
}