#include <bits/stdc++.h>
using namespace std; 

int level = 0;
vector<string> keywords; 
unordered_map<string, string> typelookup; 
bool CASE = 0; 

void judge(stringstream & ss); 

string removetypes(string s) {
    string ans = ""; 
    bool erasure = 0; 
    for (int i = 0; i < (int)s.size(); ++i) {
        if (s[i] == ',' || s[i] == ')') erasure = 0; 
        if (s[i] == ':') erasure = 1; 
        if (erasure) continue; 
        ans += s[i]; 
    }
    return ans; 
}

string str_transform(string s) {
    string ans = ""; 
    for (int i = 0; i < (int)s.size(); ++i) {
        if (s[i] == ',') {
            int l = i-1, r = i+1; 
            bool ok = 1; 
            while (l >= 0 && isdigit(s[l])) --l; 
            if (l >= 0) ok &= (s[l] == '['); 
            else ok = 0; 
            while (r < (int)s.size() && isdigit(s[r])) ++r; 
            if (r < (int)s.size()) ok &= (s[r] == ']'); 
            else ok = 0; 
            if (ok) {
                ans += "]["; 
                continue; 
            }
        }
        ans += s[i]; 
    }   
    return ans; 
}

string transform(string condition) {
    stringstream ss; ss<<condition; 
    string x;  
    vector<string> individual; 
    while (ss >> x) individual.push_back(x); 
    string ans = ""; 
    for (string& token : individual) {
        if (token == "=") token = "=="; 
        else if (token == "<>") token = "!="; 
        else if (token == "^") token = "**"; 
        else if (token == "TRUE") token = "True"; 
        else if (token == "FALSE") token = "False"; 
        else if (token == "AND") token = "and"; 
        else if (token == "OR") token = "or"; 
        ans += token + ' '; 
    }
    if (!ans.empty()) ans = ans.substr(0, (int)ans.size()-1); 
    return ans; 
}

void declare(stringstream& ss) {
    if (!CASE) cout << string(level, ' '); 
    string identifier = "", bin = "", ttype = "", t = "";  
    ss >> identifier >> bin >> ttype; 
    bool arr = 0; 
    if ((int)ttype.size() >= 5) {
        if (ttype.substr(0, 5) == "ARRAY") {
            // WHY WOULD YOU USE ANYTHING OTHER THAN LOWER BOUND OF 1???? 
            arr = 1; 
        }
    }
    if (arr) {
        // check for lower bound maybe? 
        if (ttype.back() == ',') {
            ss >> bin; 
            ttype += ' ' + bin; 
        }
        ss >> bin >> t; 
        string r = ""; 
        if (t == "INTEGER") r = "0"; 
        if (t == "REAL") r = "0.0";
        if (t == "CHAR") r = "\'\'";
        if (t == "STRING") r = "\"\"";
        if (t == "BOOLEAN") r = "False";
        
        if (count(ttype.begin(), ttype.end(), ',')) {
            // 2D 
            string ub1 = ttype.substr(ttype.find(':')+1, ttype.find(',') - ttype.find(':') - 1); 
            string temp = ttype; 
            reverse(temp.begin(), temp.end()); 
            string ub2 = ttype.substr((int)ttype.size() - temp.find(':'), ttype.find(']') - ((int)ttype.size() - temp.find(':') - 1) - 1); 
            cout << identifier << " = " << "[[" << r << "]*(" << ub2 << "+1)]*(" << ub1 << "+1)"; 
        } else {
            // 1D 
            string ub = ttype.substr(ttype.find(':')+1, ttype.find(']') - ttype.find(':') - 1); 
            cout << identifier << " = " << "[" << r << "]*(" << ub << "+1)"; 
        }
    } else {    
        string r = ""; 
        if (ttype == "INTEGER") r = "0"; 
        if (ttype == "REAL") r = "0.0";
        if (ttype == "CHAR") r = "\'\'";
        if (ttype == "STRING") r = "\"\"";
        if (ttype == "BOOLEAN") r = "False";
        cout << identifier << " = " << r; 
        typelookup[identifier] = ttype; 
    }
    if (!CASE) cout << '\n'; 
}

void output(stringstream& ss) {
    if (!CASE) cout << string(level, ' '); 
    cout << "print("; 
    string t = ""; 
    bool first = 1; 
    while (ss >> t) {
        if (!first) cout << ' ';
        if (t[0] != '\"') {
            t = str_transform(t); 
        }
        cout << t; 
        first = 0; 
    }
    cout << ", end = \"\")"; 
    if (!CASE) cout << '\n'; 
}

void input(stringstream& ss) {
    if (!CASE) cout << string(level, ' '); 
    string identifier = ""; 
    ss >> identifier; 
    identifier = str_transform(identifier); 
    string in = "input()"; 
    if (typelookup[identifier] == "INTEGER") in = "int(input())"; 
    if (typelookup[identifier] == "REAL") in = "float(input())";
    if (typelookup[identifier] == "BOOLEAN") in = "bool(input())"; 
    cout << identifier <<" = " << in; 
    if (!CASE) cout << '\n'; 
}

void iif(stringstream& ss) {
    cout << string(level, ' '); 
    string condition = "", x = ""; 
    while (ss >> x) condition += x + ' '; 
    if (!condition.empty()) condition = condition.substr(0, (int)condition.size()-1); 
    cout << "if " << str_transform(transform(condition)) << ":";
    level += 2;  
    cout << '\n'; 
}

void then(stringstream& ss) {
    level += 2; 
}

void eelse(stringstream& ss) {
    level -= 4; 
    cout << string(level, ' '); 
    cout << "else:";    
    level += 4; 
    cout << '\n'; 
}

void endif(stringstream& ss) {
    cout << string(level, ' '); 
    level -= 4; 
    cout << '\n'; 
}

void ccase(stringstream& ss) {
    cout << string(level, ' '); 
    string bin = "", identifier = ""; 
    ss >> bin >> identifier; 
    identifier = str_transform(identifier); 
    cout << "match " << identifier <<":"; 
    CASE = 1; 
    level += 4; 
    cout << '\n'; 
}

void case_handling(stringstream& ss) {
    string value = "", bin = "", statement = "", x = ""; 
    ss >> value; 
    if (value == "ENDCASE") {CASE = 0; level -= 4; return;}
    cout << string(level, ' '); 
    if (value != "OTHERWISE") {
        ss >> bin; 
        while (ss >> x) statement += x + ' '; 
        if (!statement.empty()) statement = statement.substr(0, (int)statement.size()-1); 
        cout << "case " << value << " : "; 
        stringstream ss; 
        while (ss << statement) 
        judge(ss); 
    } else {
        while (ss >> x) statement += x + ' '; 
        if (!statement.empty()) statement = statement.substr(0, (int)statement.size()-1); 
        cout << "case _ : ";
        stringstream ss;
        while (ss << statement)
        judge(ss); 
    }
    cout << '\n'; 
}

void ffor(stringstream& ss) {
    cout << string(level, ' '); 
    string identifier = ""; 
    ss >> identifier; 
    string bin = ""; 
    ss >> bin; 
    string value1 = "", value2 = "", x = ""; 
    while (ss >> x) {
        if (x == "TO") break; 
        value1 += x + ' '; 
    }
    if (!value1.empty()) value1 = value1.substr(0, (int)value1.size()-1); 
    while (ss >> x) {
        value2 += x + ' '; 
    }
    if (!value2.empty()) value2 = value2.substr( 0, (int)value2.size()-1); 
    cout << "for " << identifier << " in range(" << value1 << ", " << value2 << "+1):"; 
    level += 4; 
    cout << '\n'; 
}

void wwhile(stringstream& ss) {
    cout << string(level, ' '); 
    string condition = "", bin = "", x = ""; 
    while (ss >> x) condition += x + ' '; 
    if (!condition.empty()) condition = condition.substr(0, (int)condition.size()-3); 
    // work here 
    cout << "while " << str_transform(transform(condition)) << ":"; 
    level += 4; 
    cout << '\n'; 
}

void repeat(stringstream& ss) {
    cout << string(level, ' '); 
    cout << "while 1:"; 
    level += 4; 
    cout << '\n'; 
}

void until(stringstream& ss) {
    cout << string(level, ' '); 
    string condition = "", x = ""; 
    while (ss >> x) condition += x + ' '; 
    if (!condition.empty()) condition = condition.substr(0, (int)condition.size()-1); 
    cout << "if " << str_transform(transform(condition)) << ": break"; 
    level -= 4; 
    cout << '\n'; 
}

void nnext(stringstream& ss) {
    cout << string(level, ' '); 
    string bin = ""; 
    ss >> bin; 
    cout << '\n'; 
    level -= 4; 
}

void assignment(string& identifier, stringstream& ss) {
    if (!CASE) cout << string(level, ' '); 
    string bin = "", x = "", expression = ""; 
    while (ss >> x) {
        if (x == "<-") break; 
        identifier += x + ' '; 
    }
    // if (!identifier.empty()) identifier = identifier.substr(0, (int)identifier.size()-1); 
    while (ss >> x) expression += x + ' ';
    if (!expression.empty()) expression = expression.substr(0, (int)expression.size()-1); 
    expression = str_transform(transform(expression)); 
    identifier = str_transform(identifier); 
    cout << identifier << " = " << expression; 
    if (!CASE) cout << '\n'; 

}

void constant(stringstream& ss) {
    string keyword = ""; 
    ss >> keyword; 
    assignment(keyword, ss); 
}

void procedure(stringstream& ss) {
    if (!CASE) cout << string(level, ' '); 
    string identifier = "", x = ""; 
    while (ss >> x) identifier += x + ' '; 
    if (!identifier.empty()) identifier = identifier.substr(0, (int)identifier.size()-1); 
    identifier = removetypes(identifier);  
    if (count(identifier.begin(), identifier.end(), '(')) cout << "def " << identifier << ":"; 
    else cout << "def " << identifier << "():"; 
    level += 4; 
    if (!CASE) cout << '\n'; 
}

void ccall(stringstream& ss) {
    if (!CASE) cout << string(level, ' '); 
    string s = "", x = ""; 
    while (ss >> x) s += x + ' '; 
    if (!s.empty()) s = s.substr(0, (int)s.size()-1); 
    if (count(s.begin(), s.end(), '(')) cout << s; 
    else cout << s << "()"; 
    if (!CASE) cout << '\n'; 
}

void ffunction(stringstream& ss) {
    if (!CASE) cout << string(level, ' '); 
    string identifier = "", ttype = "", x = ""; 
    while (ss >> x) {
        if (x == "RETURNS") break; 
        identifier += x + ' '; 
    }
    if (!identifier.empty()) identifier = identifier.substr(0, (int)identifier.size()-1); 
    ss >> ttype; 
    identifier = removetypes(identifier); 
    if (count(identifier.begin(), identifier.end(), '(')) cout << "def " << identifier << ":"; 
    else cout << "def " << identifier << "():"; 
    level += 4; 
    if (!CASE) cout << '\n'; 
}

void rreturn(stringstream& ss) {
    if (!CASE) cout << string(level, ' '); 
    string s = "", x = ""; 
    while (ss >> x) s += x + ' '; 
    if (!s.empty()) s = s.substr(0, (int)s.size()-1); 
    cout << "return " << s; 
    if (!CASE) cout << '\n'; 
}

void judge(stringstream& ss) {
    string keyword; ss >> keyword; 
    if ((int)keyword.size() >= 2) {
        if (keyword.substr(0, 2) == "//") return; 
    }
    if (keyword.empty()) return; 
    if (keyword == "DECLARE") declare(ss); 
    else if (keyword == "OUTPUT") output(ss); 
    else if (keyword == "INPUT") input(ss);     
    else if (keyword == "IF") iif(ss); 
    else if (keyword == "THEN") then(ss); 
    else if (keyword == "ELSE") eelse(ss); 
    else if (keyword == "ENDIF") endif(ss); 
    else if (keyword == "CASE") ccase(ss); 
    else if (keyword == "FOR") ffor(ss); 
    else if (keyword == "WHILE") wwhile(ss); 
    else if (keyword == "REPEAT") repeat(ss); 
    else if (keyword == "UNTIL") until(ss); 
    else if (keyword == "NEXT" || keyword == "ENDWHILE" || keyword == "ENDPROCEDURE" || keyword == "ENDFUNCTION") nnext(ss); 
    else if (keyword == "CONSTANT") constant(ss); 
    else if (keyword == "PROCEDURE") procedure(ss); 
    else if (keyword == "FUNCTION") ffunction(ss); 
    else if (keyword == "CALL") ccall(ss); 
    else if (keyword == "RETURN") rreturn(ss); 
    else assignment(keyword, ss); 
}

int main() {
    freopen("main.ps", "r", stdin); 
    freopen("helper.py", "w", stdout); 
    cout << "def DIV(a, b): return a // b\n";
    cout << "def MOD(a, b): return a % b\n";
    cout << "def LENGTH(a): return len(a)\n";
    cout << "def LCASE(a): return a.lower()\n";
    cout << "def UCASE(a): return a.upper()\n"; 
    cout << "def SUBSTRING(x, a, b): return \"\".join([x[i] for i in range(a-1, a+b-1)])\n"; 
    string line = ""; 
    while (getline(cin, line)) {
        stringstream ss; 
        ss<<line; 
        if (CASE) {case_handling(ss); continue;}
        judge(ss); 
    }

    return 0; 
}