//Copyright 2019 <yurygavrilov47>
#include "FTPfiles.h"

FTPfiles::FTPfiles(const path &pathToDir) {
    map<string, pair <unsigned int, string>> accounts;
    std::cout << std::endl;
    mypath = pathToDir.string();
    pz = mypath.length()+1;
    for (const directory_entry& obj : recursive_directory_iterator(pathToDir)) {
        FTPfiles::analyse(obj.path(), accounts);
    }
    FTPfiles::printAccountsInfo(pathToDir, accounts);
}

string FTPfiles::getLatestDate(const string &date1, const string &date2) {
    unsigned int d1 = stoi(date1);
    unsigned int d2 = stoi(date2);
    if (d1 >= d2)
        return date1;
    else
        return date2;
}

void FTPfiles::printFinFile(const path &p,
                  map<string, pair <unsigned int, string>>& accounts) {
    file file;
    file.name = p.stem().string();
    file.balance = file.name.substr(0, 8);
    file.number = file.name.substr(8, 8);
    file.separator = file.name.substr(16, 1);
    file.date = file.name.substr(17, 8);
    file.type = p.extension().string();
    if (file.balance == "balance_" && stoi(file.number) && file.separator == "_"
    && stoi(file.date) && file.type == ".txt"
    && file.name.substr(25, 4) != ".old") {
        std::cout << p.parent_path().string().substr(pz) + " " +
        p.filename().string() + '\n';
        if (accounts.find(file.number) == accounts.end()) {
            accounts[file.number] = pair<int, string>(1, file.date);
        } else {
            accounts[file.number].first++;
            accounts[file.number].second =
                    getLatestDate(file.date, accounts[file.number].second);
        }
    }
}

void FTPfiles::printAccountsInfo(const path &path_to_dir,
                       map<string, pair <unsigned int, string>>& accounts) {
    for (const directory_entry& obj :
    recursive_directory_iterator(path_to_dir)) {
        if (is_regular_file(obj.path())) {
            file file;
            file.name = obj.path().stem().string();
            file.balance = file.name.substr(0, 8);
            file.number = file.name.substr(8, 8);
            file.separator = file.name.substr(16, 1);
            file.date = file.name.substr(17, 8);
            file.type = obj.path().extension().string();
            if (file.balance == "balance_" && stoi(file.number)
            && file.separator == "_"
            && stoi(file.date) && file.type == ".txt" && file.date ==
            accounts[file.number].second) {
                std::cout << "broker:" <<
                obj.path().string().substr(pz, obj.path().string().length()
                -obj.path().filename().string().length()-pz-1)
                          << " account:" +
                                  file.number + " files:" <<
                                  accounts[file.number].first <<
                          " lastdate:" + accounts[file.number].second + '\n';
            }
        }
    }
}

void FTPfiles::analyse(const path& p,
             map<string, pair <unsigned int, string>>& accounts) {
    if (exists(p)) {
        if (is_regular_file(p)) {
            printFinFile(p, accounts);
        } else if (is_directory(p)) {
            std::cout << p.string().substr(pz) <<
                      " - директория, содержащая:\n";
        } else if (is_symlink(p)) {
            analyse(read_symlink(p), accounts);
        } else {
            std::cout << p <<
                      "существует, но не является директорией "
                      "или нужным файлом\n";
        }
    } else {
        std::cout << p << "не существует\n";
    }
}
