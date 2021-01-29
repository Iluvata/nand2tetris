#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <bitset>
#include <map>


using namespace std;

map<string, int> symbolTable;

struct ParseRes{
    int asmType;   // 0: A  1: C  2: L
    int n;         // line number
    string symbol;
    string dest;
    string comp;
    string jump;
};

class Parser{
private:
    ifstream asmStream;
    int n;
public:
    Parser(string asmFileName){
        n = 0;
        asmStream.open(asmFileName);
        if(!asmStream.is_open())
            throw "No such file!";
    }
    ~Parser(){
        asmStream.close();
    }

    bool Parse(ParseRes& res){
        // get processing line
        string str;
        do{
            if(!getline(asmStream, str))
                return false;

            // remove space and comments
            str.erase(remove_if(str.begin(), str.end(), ::isspace), str.end());
            string::size_type position = str.find("//");
            if(position != str.npos){
                str = str.substr(0, position);
            }
        }while(str.empty());
        res.n = n;
        if(str[0] == '@'){
            res.asmType = 0;
            res.symbol = str.substr(1);
            n++;
        }
        else if(str[0] == '('){
            res.asmType = 2;
            res.symbol = str.substr(1, str.size() - 2);
        }
        else{
            res.asmType = 1;
            string::size_type position = str.find("=");
            if(position != str.npos){
                res.dest = str.substr(0, position);
                str = str.substr(position + 1);
            }
            else{
                res.dest = "";
            }
            position = str.find(";");
            if(position != str.npos){
                res.comp = str.substr(0, position);
                res.jump = str.substr(position + 1);
            }
            else{
                res.comp = str;
                res.jump = "";
            }
            n++;
        }
        return true;
    }
};

class Code{
public:
    string dbits, cbits, jbits;
    void dest(string d){
        dbits = "000";
        if(d.find("M") != d.npos)
            dbits[2] = '1';
        if(d.find("D") != d.npos)
            dbits[1] = '1';
        if(d.find("A") != d.npos)
            dbits[0] = '1';
    }
    void comp(string c){
        cbits = "0000000";
        if(c == "0")
            cbits = "0101010";
        else if(c == "1")
            cbits = "0111111";
        else if(c == "-1")
            cbits = "0111010";
        else if(c == "D")
            cbits = "0001100";
        else if(c == "A")
            cbits = "0110000";
        else if(c == "M")
            cbits = "1110000";
        else if(c == "!D")
            cbits = "0001101";
        else if(c == "!A")
            cbits = "0110001";
        else if(c == "!M")
            cbits = "1110001";
        else if(c == "-D")
            cbits = "0001111";
        else if(c == "-A")
            cbits = "0110011";
        else if(c == "-M")
            cbits = "1110011";
        else if(c == "D+1")
            cbits = "0011111";
        else if(c == "A+1")
            cbits = "0110111";
        else if(c == "M+1")
            cbits = "1110111";
        else if(c == "D-1")
            cbits = "0001110";
        else if(c == "A-1")
            cbits = "0110010";
        else if(c == "M-1")
            cbits = "1110010";
        else if(c == "D+A")
            cbits = "0000010";
        else if(c == "D+M")
            cbits = "1000010";
        else if(c == "D-A")
            cbits = "0010011";
        else if(c == "D-M")
            cbits = "1010011";
        else if(c == "A-D")
            cbits = "0000111";
        else if(c == "M-D")
            cbits = "1000111";
        else if(c == "D&A")
            cbits = "0000000";
        else if(c == "D&M")
            cbits = "1000000";
        else if(c == "D|A")
            cbits = "0010101";
        else if(c == "D|M")
            cbits = "1010101";
    }
    void jump(string j){
        jbits = "000";
        if(j == "JGT")
            jbits = "001";
        else if(j == "JEQ")
            jbits = "010";
        else if(j == "JGE")
            jbits = "011";
        else if(j == "JLT")
            jbits = "100";
        else if(j == "JNE")
            jbits = "101";
        else if(j == "JLE")
            jbits = "110";
        else if(j == "JMP")
            jbits = "111";
    }

};

int main()
{
    // predefined lables
    symbolTable["SP"] = 0;
    symbolTable["LCL"] = 1;
    symbolTable["ARG"] = 2;
    symbolTable["THIS"] = 3;
    symbolTable["THAT"] = 4;
    symbolTable["SCREEN"] = 16384;
    symbolTable["KBD"] = 24576;
    for(int i = 0; i < 16; ++i){
        symbolTable["R" + to_string(i)] = i;
    }



    string filename;
    cin >> filename;

    try {
        Parser pass1(filename);
        ParseRes parsed;
        Code cres;


        while(pass1.Parse(parsed)){
            // cout << parsed.asmType << "\t" << parsed.n << endl;
            if(parsed.asmType == 2){
                symbolTable[parsed.symbol] = parsed.n;
            }
        }
    } catch (const char* msg) {
        cerr << msg << endl;
    }

    // map<string, int> look = symbolTable;


    string outputFileName = filename.substr(0, filename.find('.')) + ".hack";
    ofstream outFile(outputFileName);
    try {
        Parser pass2(filename);
        ParseRes parsed;
        Code cres;
        int n = 16;
        while(pass2.Parse(parsed)){
            if(parsed.asmType == 0){
                int i;
                try {
                    i = stoi(parsed.symbol);
                } catch (...) {
                    if(symbolTable.count(parsed.symbol)){
                        i = symbolTable[parsed.symbol];
                    }
                    else{
                        symbolTable[parsed.symbol] = n;
                        i = n;
                        n++;
                    }
                }
                outFile << (bitset<16>)i << endl;
            }
            else if(parsed.asmType == 1){
                cres.dest(parsed.dest);
                cres.comp(parsed.comp);
                cres.jump(parsed.jump);
                outFile << "111" + cres.cbits + cres.dbits + cres.jbits << endl;
            }

        }
    } catch (const char* msg) {
        cerr << msg << endl;
    }

    return 0;
}


