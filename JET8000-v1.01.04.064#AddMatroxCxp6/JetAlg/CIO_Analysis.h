#include <string>
#include <iostream>
#include <fstream>

using namespace std;

namespace JET {
    namespace tkt {
        class CIO_Analysis {
        public:
            //load a float image(.FImg)
            static bool Load_FloatImage(std::string filename, int& w, int& h, int& nc, float*& data) {
                bool status = false;
                fstream fs = fstream(filename, ios_base::in | ios_base::binary);
                if (fs.is_open()) {
                    fs.read((char*)&w, sizeof(w));
                    fs.read((char*)&h, sizeof(h));
                    fs.read((char*)&nc, sizeof(nc));
                    if (!data) {
                        data = new float[w * h * nc];
                    }
                    int loc_data_beg = fs.tellg();
                    fs.seekg(0, std::ios::end);
                    int loc_data_end = fs.tellg();
                    fs.seekg(loc_data_beg, std::ios::beg);
                    int len_data = loc_data_end - loc_data_beg;
                    fs.read((char*)data, w * h * nc * sizeof(float));
                    status = true;
                }
                return status;
            }
            //save a float image(.FImg)
            static bool Save_FloatImage(string filename, const int& w, const int& h, const int& nc, const float* data) {
                bool status = false;
                fstream fs = fstream(filename, ios::out | ios::binary);
                if (fs.is_open()) {
                    fs.write((char*)&w, sizeof(w));
                    fs.write((char*)&h, sizeof(h));
                    fs.write((char*)&nc, sizeof(nc));
                    fs.write((char*)data, w * h * nc * sizeof(float));
                    fs.close();
                    status = true;
                }
                return status;
            }
        };
    }
}