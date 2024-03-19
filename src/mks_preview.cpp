#include "../include/mks_preview.h"

bool gimage_is_showed = false;
bool simage_is_showed = false;

// std::vector<std::string> gimage;
// std::vector<std::string> simage;

// std::string str_gimage = "";

// std::vector<std::string> gimage_temp;
// std::vector<std::string> simage_temp;

/*
void generate_gimage(std::string filename) {
    gimage.clear();
    // gimage_temp.clear();
    std::string path = "/home/mks/gcode_files" + filename;
    std::ifstream data(path.c_str());
    std::string strline;
    while (getline(data, strline)) {
        bool exists = strline.find(";gimage:") == std::string::npos;
        if (!exists) {
            std::cout << strline.substr(8) + "\n" << std::endl;
            gimage.push_back(strline.substr(8));
            // str_gimage += strline.substr(8);
        }
        // std::cout << str_gimage << std::endl;
    }
    data.close();
}
*/

/*
void generate_simage(std::string filename) {
    simage.clear();
    // simage_temp.clear();
    std::string path = "/home/mks/gcode_files" + filename;
    std::ifstream data(path.c_str());
    std::string strline;
    while (getline(data, strline)) {
        bool exists = strline.find(";simage:") == std::string::npos;
        if (!exists) {
            std::cout << strline.substr(8) + "\n" << std::endl;
            // simage.push_back(strline.substr(8));
        }
    }
    data.close();
    // for (int i = 0; i < simage.size(); i++) {
    //     std::cout << simage[i] << "\n" << std::endl;
    // }
}
*/


