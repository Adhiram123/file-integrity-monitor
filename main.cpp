#include <iostream>
#include <fstream>
#include <filesystem>
#include <openssl/sha.h>

using namespace std;
namespace fs = std::filesystem;

string sha256(const string &filePath) {

    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);

    ifstream file(filePath, ios::binary);

    char buffer[8192];

    while (file.good()) {
        file.read(buffer, sizeof(buffer));
        SHA256_Update(&sha256, buffer, file.gcount());
    }

    SHA256_Final(hash, &sha256);

    char output[65];
    for(int i = 0; i < 32; i++)
        sprintf(output + (i * 2), "%02x", hash[i]);

    return string(output);
}

int main() {

    string folder = "./";

    ofstream db("database.txt", ios::app);

    for (const auto &entry : fs::directory_iterator(folder)) {

        if (fs::is_regular_file(entry.path())) {

            string filePath = entry.path();

            string hash = sha256(filePath);

            cout << filePath << " : " << hash << endl;

            db << filePath << " " << hash << endl;
        }

    }

    db.close();

    cout << "Scan complete. Hashes stored in database.txt\n";

    return 0;
}
