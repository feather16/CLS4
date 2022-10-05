#include <string>
#include <vector>

using namespace std;

class Argument{
    public:
    string interpreter;
    string filename;
    string tree_filename;
    Argument(){}
    Argument(int, char*[]);
    private:
    vector<string> args;
    void throw_error(const vector<string>::iterator&, const string&) const;
    static bool is_option(const string&);
};

class ArgumentError: public exception{
    public:
    ArgumentError(const vector<string>&, const int, const string&);
    const char* what() const throw() override;
    private:
    string what_str;
};