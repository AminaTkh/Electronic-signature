#include <fstream>
#include <vector>
#include "help.hpp"

using namespace std;
int main(int argc, char**argv){
    argc++;
    Set ourset = s512;
    int indofprvk = 1;
        if (!strcmp(argv[1], "-s")){
            ourset = s256;
            indofprvk++;
        }
       
    ELC ourellipt(ourset);
    Private_key ourprivkey(ourellipt);
    ofstream filik;
    filik.open(argv[indofprvk]);
    write_privkey(filik, ourprivkey);
    return 0;
}





    
    
