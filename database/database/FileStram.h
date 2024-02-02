#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <string>
//	template <typename T>
struct table
{
	std::string name;// table name
	std::unordered_map<std::string, std::vector<std::string>> data;//string 1 table label, string 2 table´s data
};


struct db
{
	std::string databasename;
	std::string username;
	std::string password;
	std::vector<table> tables;
};

class FileStream
{
private:
	FILE* stream;
	const char* path_t;
public:
	FileStream(const char* path) : path_t(path) {};
	size_t __cdecl read(db& myDB);
	size_t __cdecl write(const db& myDB);
};

