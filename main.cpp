#include <iostream>
#include "bparser.h"
#include "hparser.h"

using namespace std;

void print_indented( const std::string& str, ostream& os )
{
    const int IndentLevel = 3;
    int indent = 0;
    for ( auto c : str ) {
        if ( c == '(' ) {
            os << '\n';
            for ( int j=0; j<indent; ++j ) { os << ' '; }
            indent += IndentLevel;
        }
        else if ( c == ')' ) {
            indent -= IndentLevel;
        }
        os << c;
    }
    os << std::endl;
}


int main(int argc, char* argv[])
{
    // Process the command-line arguments, if any.
    // Usage: program [ option [ filename ] ]  (option -h or -b)
    bool use_bison = false;
    if ( argc >= 2 && string(argv[1]) == "-b" ) {
        use_bison = true;
    }
    string filename( "test.decaf" );
    if ( argc >= 3 ) {
        filename = argv[2];
    }

    // Open file with Decaf program, exit if error opening file.
    FILE* file = fopen( filename.c_str(), "r" );
    if ( file == nullptr ) {
        cerr << "Could not open input file '" << filename << "'." << endl;
        return -1;
    }

    // Instantiate the right parser.
    Parser* parser;
    if (use_bison) {
        parser = new BParser(file, false, false); // Change flags to true for debugging.
    }
    else {
        parser = new HParser(file, false, false);
    }

    // Parse and output the generated abstract syntax tree.
    cout << "====> PARSING FILE " << filename
         << " USING PARSER " << parser->get_name() << endl;
    parser->parse();
    cout << "====> AST" << endl;
    Node *ast = parser->get_AST();
    if ( ast != nullptr ) {
        print_indented( ast->str( ), std::cout );
    }

    // Clean up and return.
    fclose(file);
    delete parser;
    return 0;
}