#pragma once
#include <vector>
#include <Windows.h>
#include <string>
#include "FileStram.h"
#define UIS const char* user, const char* pass, const char* databasename
class database
{
private:
	db m_db;
	const char* username;
	const char* password;
	const char* databaseN;
	void SaveDatabase();//test

public:
	//database(const char* user, const char* pass) : username(user), password(pass) {}
	database() {};
	void Create(UIS);
	bool Open(UIS);
	void CreateTable(const char* table_name);
	void InsertTableData(const char* table, const char* key, const char* key_data);
	std::string GetTableData(const char* table, const char* key);
	std::string GetTableValue(const char* table, const char* key, const char* key_value);
	std::string FindTableData(const char* table, const char* key, const char* data);
	std::string GetTableCombo(const char* table, const char* key1, const char* key2, const char* hint = NULL);
	void save();
};

