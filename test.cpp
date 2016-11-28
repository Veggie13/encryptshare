#include <fstream>

#include "EncryptShare_iostream.h"

using namespace std;


int main()
{
    list<string> password;
    password.push_back("superawesome");

    // Write a small encrypted file
    {
        ofstream    key1("../../data/key1.bin", ios_base::binary | ios_base::out),
                    key2("../../data/key2.bin", ios_base::binary | ios_base::out);
        vector<ostream*> keys(2);
        keys[0] = &key1;
        keys[1] = &key2;

        Encrypt_ostream filter(keys, password);

        filter << "Hello World" << endl;
        filter.flush();
    }

    // Read it back
    {
        ifstream    key1("../../data/key1.bin", ios_base::binary | ios_base::in),
                    key2("../../data/key2.bin", ios_base::binary | ios_base::in);
        vector<istream*> keys(2);
        keys[0] = &key1;
        keys[1] = &key2;

        Encrypt_istream filter(keys, password);

        string input;
        getline(filter, input, '\n');
        cout << input << endl;
    }

    return 0;
}
