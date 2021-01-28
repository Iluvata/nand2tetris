#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <bitset>


using namespace std;

struct ParseRes{
    int asmType;   // 0: A  1: C  2: L
    string symbol;
    string dest;
    string comp;
    string jump;
};

class Parser{
private:
    ifstream asmStream;
public:
    Parser(string asmFileName){
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
        if(str[0] == '@'){
            res.asmType = 0;
            res.symbol = str.substr(1);
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

int main(int argc, char* argv[])
{
    string filename;
    cin >> filename;
    string outputFileName = filename.substr(0, filename.find('.')) + ".hack";
    ofstream outFile(outputFileName);
    try {
        Parser infile(filename);
        ParseRes parsed;
        Code cres;
        while(infile.Parse(parsed)){
            // outFile << parsed.symbol << "\tdest:" << parsed.dest << "\tcomp:" << parsed.comp << "\tjump:" << parsed.jump << endl;
            if(parsed.asmType == 0){
                int i = stoi(parsed.symbol);
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


