#include "Ji.hpp"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <array>
#include <cstdint>
#include <cstring>

using namespace Dualys;
using namespace std;

int Jah::init()
{

    const fs::path repo_dir = ".jah";
    const fs::path db_file = repo_dir / "ji.jah";
    const fs::path head_file = repo_dir / "HEAD";
    const fs::path refs_dir = repo_dir / "refs";
    const fs::path heads_dir = refs_dir / "heads";
    const string default_branch_ref = "ref: refs/heads/main";


    if (fs::exists(repo_dir)) {
        cerr << "Jah is already initialized" << endl;
        return EXIT_FAILURE;
    }

    try {
        fs::create_directory(repo_dir);
        fs::create_directory(refs_dir);
        fs::create_directory(heads_dir);
    } catch (const fs::filesystem_error& e) {
        cerr << e.what() << endl;
        return EXIT_FAILURE;
    }

    ofstream head_stream(head_file);
    if (!head_stream.is_open()) {
        cerr << "Failed to write the "<< head_file <<  " file" << endl;
        return EXIT_FAILURE;
    }
    head_stream << default_branch_ref << endl;
    head_stream.close();

    ofstream db_stream(db_file, ios::binary);
    if (!db_stream.is_open()) {
        cerr << "Failed to write the " << db_file  << " file" << endl;
        return EXIT_FAILURE;
    }

    db_stream.write(reinterpret_cast<const char*>(MAGIC.data()), MAGIC.size());

    db_stream.write(reinterpret_cast<const char*>(&VERSION), sizeof(VERSION));

    db_stream.close();

    if (!db_stream) { 
        cerr << "Sorry an error has been encontred on db file creation." << endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

bool Jah::isJahRepositoryValid() {
    ifstream file(JAH_DATABASE, ios::binary);
    array<uint8_t, 6> magic_from_file;

    file.read(reinterpret_cast<char*>(magic_from_file.data()), magic_from_file.size());

    if (!file) { 
        return false;
    }
    if (memcmp(magic_from_file.data(), MAGIC.data(), magic_from_file.size()) != 0) {
        cerr << "No detecting jah repository" << endl;
        return false;
    }
    return true;
}
