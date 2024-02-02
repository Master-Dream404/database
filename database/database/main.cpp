#include <iostream>
#include "database.h"
int main()
{
	auto db = database();



	db.Create("dream", "123", "test");
	db.CreateTable("users");
	db.InsertTableData("users", "username", "MasterDream");
	db.InsertTableData("users", "password", "Dream2024");
	db.save();






	if (db.Open("dream", "123", "test"))
	{
		//std::cout << db.GetTableCombo("users", "username", "password");// error not working right now

		std::cout << db.GetTableData("users", "username") << std::endl;
		std::cout << db.GetTableData("users", "password") << std::endl;

		std::cout << db.FindTableData("users", "username", "Master") << std::endl;


	}
	else
	{
		std::cout << "Password error";
	}


	Sleep(9999);
}
