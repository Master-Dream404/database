#include "FileStram.h"
size_t __cdecl FileStream::read(db& myDB) {
    std::ifstream inFile(path_t, std::ios::binary);
    if (inFile.is_open()) {
        // Read the string lengths and strings
        size_t dbnameLength, usernameLength, passwordLength;

        inFile.read(reinterpret_cast<char*>(&dbnameLength), sizeof(dbnameLength));
        myDB.databasename.resize(dbnameLength);
        inFile.read(const_cast<char*>(myDB.databasename.data()), dbnameLength);

        inFile.read(reinterpret_cast<char*>(&usernameLength), sizeof(usernameLength));
        myDB.username.resize(usernameLength);
        inFile.read(const_cast<char*>(myDB.username.data()), usernameLength);

        inFile.read(reinterpret_cast<char*>(&passwordLength), sizeof(passwordLength));
        myDB.password.resize(passwordLength);
        inFile.read(const_cast<char*>(myDB.password.data()), passwordLength);

        // Read the number of tables
        size_t numTables;
        inFile.read(reinterpret_cast<char*>(&numTables), sizeof(numTables));

        // Read each table
        std::vector<table> tempTables(numTables);
        for (auto& tbl : tempTables) {
            size_t tableNameLength;
            inFile.read(reinterpret_cast<char*>(&tableNameLength), sizeof(tableNameLength));
            tbl.name.resize(tableNameLength);
            inFile.read(const_cast<char*>(tbl.name.data()), tableNameLength);

            // Read the number of elements in the map
            size_t numElements;
            inFile.read(reinterpret_cast<char*>(&numElements), sizeof(numElements));

            // Read each key-value pair in the map
            for (size_t i = 0; i < numElements; ++i) {
                // Read the key length and key
                size_t keyLength;
                inFile.read(reinterpret_cast<char*>(&keyLength), sizeof(keyLength));
                std::string key(keyLength, '\0');
                inFile.read(const_cast<char*>(key.data()), keyLength);

                // Read the number of elements in the vector
                size_t numVectorElements;
                inFile.read(reinterpret_cast<char*>(&numVectorElements), sizeof(numVectorElements));

                // Read each element in the vector
                std::vector<std::string> value(numVectorElements);
                for (size_t j = 0; j < numVectorElements; ++j) {
                    size_t elementLength;
                    inFile.read(reinterpret_cast<char*>(&elementLength), sizeof(elementLength));
                    value[j].resize(elementLength);
                    inFile.read(const_cast<char*>(value[j].data()), elementLength);
                }

                // Store the key-value pair in the table's map
                tbl.data[key] = value;
            }
        }

        // Move the temporary tables to the target vector
        myDB.tables = std::move(tempTables);

        inFile.close();
    }
    else {
        std::cerr << "Unable to open the file for reading." << std::endl;
    }
}

size_t __cdecl FileStream::write(const db& myDB) {
    std::ofstream outFile(path_t, std::ios::binary);
    if (outFile.is_open()) {
        // Write the string lengths and strings
        size_t dbnameLength = myDB.databasename.size();
        outFile.write(reinterpret_cast<const char*>(&dbnameLength), sizeof(dbnameLength));
        outFile.write(myDB.databasename.c_str(), dbnameLength);

        size_t usernameLength = myDB.username.size();
        outFile.write(reinterpret_cast<const char*>(&usernameLength), sizeof(usernameLength));
        outFile.write(myDB.username.c_str(), usernameLength);

        size_t passwordLength = myDB.password.size();
        outFile.write(reinterpret_cast<const char*>(&passwordLength), sizeof(passwordLength));
        outFile.write(myDB.password.c_str(), passwordLength);

        // Write the number of tables
        size_t numTables = myDB.tables.size();
        outFile.write(reinterpret_cast<const char*>(&numTables), sizeof(numTables));

        // Write each table
        for (const auto& tbl : myDB.tables) {
            size_t tableNameLength = tbl.name.size();
            outFile.write(reinterpret_cast<const char*>(&tableNameLength), sizeof(tableNameLength));
            outFile.write(tbl.name.c_str(), tableNameLength);

            // Write the number of elements in the map
            size_t numElements = tbl.data.size();
            outFile.write(reinterpret_cast<const char*>(&numElements), sizeof(numElements));

            // Write each key-value pair in the map
            for (const auto& kv : tbl.data) {
                // Write the key length and key
                size_t keyLength = kv.first.size();
                outFile.write(reinterpret_cast<const char*>(&keyLength), sizeof(keyLength));
                outFile.write(kv.first.c_str(), keyLength);

                // Write the number of elements in the vector
                size_t numVectorElements = kv.second.size();
                outFile.write(reinterpret_cast<const char*>(&numVectorElements), sizeof(numVectorElements));

                // Write each element in the vector
                for (const auto& element : kv.second) {
                    size_t elementLength = element.size();
                    outFile.write(reinterpret_cast<const char*>(&elementLength), sizeof(elementLength));
                    outFile.write(element.c_str(), elementLength);
                }
            }
        }

        outFile.close();
    }
    else {
        std::cout << "Unable to open the file for writing." << std::endl;
    }
    return 1;
}