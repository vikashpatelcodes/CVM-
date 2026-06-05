#include <iostream>
#include <fstream>
#include <sstream>
#include "lexer.h"
#include "parser.h"
#include "compiler.h"
#include "vm.h"

using namespace std;

void run(const string& source, bool debug = false) {
    try {
        Lexer lexer(source);
        auto tokens = lexer.tokenize();

        Parser parser(move(tokens));
        auto ast = parser.parse();

        Compiler compiler;
        auto bytecode = compiler.compile(ast.get());

        if (debug) {
            cout << "=== BYTECODE ===\n";
            compiler.printBytecode(bytecode);
            cout << "=== OUTPUT ===\n";
        }

        VM vm;
        vm.execute(bytecode);

    } catch (exception& e) {
        cerr << "Error: " << e.what() << "\n";
    }
}

void runREPL() {
    cout << "CVM++ REPL\n";
    cout << "Commands: 'exit' to quit | 'debug' to toggle debug | 'reset' to clear variables\n\n";

    bool debug = false;
    string line;
    string session = "";
    int prevOutputCount = 0;

    while (true) {
        cout << ">> ";
        getline(cin, line);

        if (line == "exit") break;

        if (line == "debug") {
            debug = !debug;
            cout << "Debug " << (debug ? "ON" : "OFF") << "\n";
            continue;
        }

        if (line == "reset") {
            session = "";
            prevOutputCount = 0;
            cout << "Session cleared.\n";
            continue;
        }

        if (line.empty()) continue;

        string attempted = session + line + "\n";

        streambuf* oldBuf = cout.rdbuf();
        ostringstream captured;
        cout.rdbuf(captured.rdbuf());

        streambuf* oldErr = cerr.rdbuf();
        ostringstream capturedErr;
        cerr.rdbuf(capturedErr.rdbuf());

        run(attempted, false);

        cout.rdbuf(oldBuf);
        cerr.rdbuf(oldErr);

        string errOutput = capturedErr.str();

        if (!errOutput.empty()) {
            cout << errOutput;
            continue;
        }

        session = attempted;

        string allOutput = captured.str();
        vector<string> lines;
        istringstream ss(allOutput);
        string l;
        while (getline(ss, l)) lines.push_back(l);

        for (int i = prevOutputCount; i < (int)lines.size(); i++)
            cout << lines[i] << "\n";

        prevOutputCount = lines.size();
    }
}

int main(int argc, char* argv[]) {
    if (argc == 1) {
        runREPL();
    }
    else if (argc == 2) {
        string filename = argv[1];
        ifstream file(filename);
        if (!file) { cerr << "Cannot open file: " << filename << "\n"; return 1; }
        stringstream ss;
        ss << file.rdbuf();
        run(ss.str());
    }
    else if (argc == 3 && string(argv[1]) == "debug") {
        ifstream file(argv[2]);
        if (!file) { cerr << "Cannot open file: " << argv[2] << "\n"; return 1; }
        stringstream ss;
        ss << file.rdbuf();
        run(ss.str(), true);
    }
    else {
        cerr << "Usage:\n";
        cerr << "  ./cvm                  - REPL mode\n";
        cerr << "  ./cvm script.cvm       - run a file\n";
        cerr << "  ./cvm debug file.cvm - debug mode\n";
    }

    return 0;
}