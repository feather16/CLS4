#include <string>
#include <vector>

using namespace std;

class Argument{
    public:
    string interpreter;
    string filename;
    string tree_filename;
    Argument(int, char*[]);
    private:
    vector<string> args;
};