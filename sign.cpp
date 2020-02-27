#include <fstream>
#include <vector>
#include "hash.hpp"



hash_Block h_ctx;
uint512_t r;
uint512_t s;
extern "C" int rand_bytes (uint8_t*, int);

uint512_t randss (const ELC &prm)
{
    uint512_t seq;
    uint8_t seq_arr[UINT512_SIZE] = {0};
    int arr_size = prm.set == s512 ? UINT512_SIZE : UINT256_SIZE;
    do {
        rand_bytes(seq_arr, arr_size);
        seq = uint512_t(seq_arr, UINT512_SIZE);
        std::cout << "  randss :   "<<seq<<"\n\n";
    } while (seq == (uint512_t)0 || (seq > prm.q));
    return seq;
}

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
int main (int argc, char **argv){
    Set ourset = s512;
    vector <string>filenames;
    int indofprvk = 1;
        if (!strcmp(argv[1], "-s")) {
            ourset = s256;
            indofprvk++;
            }
    ELC signcurve (ourset);
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
    if (priv_key_s.size() == 2 * 32 && signcurve.set == s256) {
        priv_key_num = extend(uint256_t(uint256_t::HEX, priv_key_s));
    } else {
        if (priv_key_s.size() == 2 * 64 && signcurve.set == s512) {
            priv_key_num = uint512_t(uint512_t::HEX, priv_key_s);
        } else {
            cout<<("Incorrect private key.");
        }
    }
    Private_key ourprivkey(signcurve,priv_key_num);
  //  ourprivkey.get() = priv_key_num;

    hash_Block hctx(hash_Block::h512, argv[indofprvk+1]);
    if (signcurve.set == s256) 
        hctx.set_mode(hash_Block::h256);
    std::ofstream output(argv[indofprvk+2]);
    while (!hctx.eof()) {
        hctx.update();
    }
    uint512_t result = hctx.finish();
    uint512_t hashh;

    if (signcurve.set == s256) {
        hashh = extend(result.split<256>()[1]);
    } else {
        hashh = result;
    }

    uint512_t e = hashh % signcurve.q;
    if (e == (uint512_t)0) {
        e = 1;
    }
    uint512_t k;
    cout << "      HASH    :      "<<hashh <<"\n\n" << "       e:      "<<e <<"\n\n";
    do {


        k = randss(signcurve);
       cout << "      K :      "<<k <<"\n";
        cout<<k<<"\n";
        Point c = k * Point(&signcurve);
        uint512_t cx = convert(signcurve, c.y);
        r = cx % signcurve.q;
    } while (r == (uint512_t)0);
  //  std::cout << "\n privatniy kluch \n " << ourprivkey.key <<"\n";
    s = addition(multiplication(r, ourprivkey.key, signcurve.q), multiplication(k, e, signcurve.q), signcurve.q);

    if (signcurve.set == s256) {
        output << r.split<256>()[0] << s.split<256>()[0] << std::endl;
    } else {
        output << r << s << std::endl;
    }
    return 0;
}
