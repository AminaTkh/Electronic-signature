#include <fstream>
#include <vector>
#include "hash.hpp"
const ELC *signcurve;
hash_Block h_ctx;
uint512_t r;
uint512_t s;


uint512_t convert(ELC &criv, uint512_t &v){
    uint512_t t1 = addition(criv.e, criv.d, criv.p);
    uint512_t t = multiplication(t1, inverse(6, criv.p), criv.p );
    uint512_t s1 = substraction(criv.e, criv.d, criv.p);
    uint512_t s = multiplication(s1, inverse(4, criv.p), criv.p );
    uint512_t v1 = multiplication(addition(1,v,criv.p), s, criv.p);
    uint512_t v2 = multiplication(v1, inverse(substraction(1,v,criv.p), criv.p), criv.p);
    return addition(v2,t,criv.p);
}


using namespace std;
 int main (int argc, char **argv)
{
    Set ourset = s512;
    int indofpubk = 1;
        if (!strcmp(argv[1], "-s")){
            ourset = s256;
            indofpubk++;
        }
    ELC ourellipt(ourset);
    Private_key ourprivkey(ourellipt);
    uint512_t x1, y1;
    if (argc > 4) {
       cout<<("Incorrect command line arguments.");
    }
    ifstream pub_key_f;
    pub_key_f.open(argv[indofpubk]);
    if (!pub_key_f) {
          cout<<"File with public key not found.";
    }
    string pub_key_s_x;
    string pub_key_s_y;
    getline(pub_key_f, pub_key_s_x);
    getline(pub_key_f, pub_key_s_y);
    if (pub_key_s_x.size() != pub_key_s_y.size()) {
        cout<<"Incorrect public key.";
    }
    if (pub_key_s_x.size() == 2 * 64 && ourellipt.set == s512) {
        x1 = uint512_t(uint512_t::HEX, pub_key_s_x);
        y1 = uint512_t(uint512_t::HEX, pub_key_s_y);
    } else {
        if (pub_key_s_x.size() == 2 * 32 && ourellipt.set == s256) {
            x1 = extend(uint256_t(uint256_t::HEX, pub_key_s_x));
            y1 = extend(uint256_t(uint256_t::HEX, pub_key_s_y));
        } else {
            cout<<"Incorrect public key.";
        }
    }
    Public_key ourpubkey(Point(&ourellipt, x1, y1));
   // write_publkey(publcheck, ourpubkey);
    hash_Block hctx(hash_Block::h512, argv[indofpubk+1]);
    if (ourellipt.set == s256) 
        hctx.set_mode(hash_Block::h256);
    std::ifstream input(argv[indofpubk+2]);;
    if (!input) {
        cout<<"File with signature doesn't exist.";
    }
    std::string signature;
    std::getline(input, signature);
    if (signature.size() != 4 * 64 && signature.size() != 4 * 32) {
        cout<<"Incorrect signature format.";
    }
//        std::cout<<signature.size()<<"\n";

    size_t size = ourellipt.set == s256 ? 2 * 32 : 2 * 64;
    if (signature.size() == 4 * 64 && ourellipt.set == s512) {
        r = uint512_t(uint512_t::HEX, signature.substr(0, size));
        s = uint512_t(uint512_t::HEX, signature.substr(size, signature.size()));
    } else {
        if (signature.size() == 4 * 32 && ourellipt.set == s256) {
            r = extend(uint256_t(uint256_t::HEX, signature.substr(0, size)));
            s = extend(uint256_t(uint256_t::HEX, signature.substr(size, signature.size())));
        } else {
            cout<<"Incorrect signature format.";
        }
    }
//    std::cout<<"YA TUTA DA\n";
        if (r > ourellipt.q || s > ourellipt.q) {
        std::cout << "FAIL" << std::endl;
        return 0;
    }
    uint512_t hashh;
    while (!hctx.eof()) {
        hctx.update();
    }
    uint512_t result = hctx.finish();
    if (ourellipt.set == s256) {
        hashh = extend(result.split<256>()[1]);
    } else {
        hashh = result;
    }
    uint512_t e = hashh % ourellipt.q;
    uint512_t v = inverse(e, ourellipt.q);
    uint512_t z1 = multiplication(s, v, ourellipt.q);
    uint512_t z2 = opposite(multiplication(r, v, ourellipt.q), ourellipt.q);
    Point c = z1 * Point(&ourellipt) + z2 * ourpubkey.key;
    uint512_t cx = convert(ourellipt, c.y);
    if (cx % ourellipt.q == r) 
        std::cout << "SUCCESS\n";     
    else {
        std::cout << "FAIL\n";
    }     
    return 0;   
}


