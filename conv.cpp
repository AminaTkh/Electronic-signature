#include <fstream>
#include <vector>
#include "Block.hpp"
#include "help.hpp"

using namespace std;
int main (int argc, char **argv){
    Set ourset = s512;
    vector <string>filenames;
    int indofprvk = 1;
        if (!strcmp(argv[1], "-s")) {
            ourset = s256;
            indofprvk++;
        }
    ELC ourellipt (ourset);
    ofstream filik;
    uint512_t priv_key_num;
    if (argc > 4) {
        cout<<("Incorrect command line arguments.");
    }
    ifstream priv_key_f;
    priv_key_f.open(argv[indofprvk]);
    if (!priv_key_f) {
        cout<<("File with private key not found.");
    }
    string priv_key_s;
    getline(priv_key_f, priv_key_s);
    if (priv_key_s.size() == 2 * 32 && ourellipt.set == s256) {
        priv_key_num = extend(uint256_t(uint256_t::HEX, priv_key_s));
    } else {
        if (priv_key_s.size() == 2 * 64 && ourellipt.set == s512) {
            priv_key_num = uint512_t(uint512_t::HEX, priv_key_s);
        } else {
            cout<<("Incorrect private key.");
        }
    }
    Private_key ourprivkey(ourellipt);
    ourprivkey.get() = priv_key_num;
    filik.open(argv[indofprvk+1]);
    Public_key outpubkey(ourellipt, ourprivkey);
    write_publkey(filik,outpubkey);
    return 0;
}