#pragma once
using namespace std;

string readFileIntoString(ifstream& input_file) {
      
//      if (!input_file.is_open()) {
//            cerr << "Could not open the file - '"
//            << path << "'" << endl;
//            exit(EXIT_FAILURE);
//      }
      return string((std::istreambuf_iterator<char>(input_file)), std::istreambuf_iterator<char>());
}
