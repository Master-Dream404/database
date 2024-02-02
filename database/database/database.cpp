#include "database.h"
#include <m_includes/filesystem/filesystem.hpp>
#include <m_includes/XorStr/XR.hpp>
#include <string>
#include <iostream>

void database::SaveDatabase()
{
	const char* database_path = (XR("D:\\Program Files (x86)\\CidiaDatabase\\database"));
	std::string buffer = (std::string(database_path).append(XR("\\")).append("test.cidia"));
	if (!fs::exists(buffer))
		return;

	FileStream(buffer.c_str()).write(m_db);

}


void database::Create(UIS)
{
	m_db.username = user;
	m_db.password = pass;
	const char* database_path = (XR("D:\\Program Files (x86)\\CidiaDatabase\\database"));

	if (!fs::exists(database_path))
	{
		fs::create_directories(database_path);
		std::string fixbuff = databasename;
		if (std::string(databasename).length() > 6) {
			size_t buff = (std::string(databasename).length() - 6);

			const char* buffer{};
			for (int x = 0; x < 6; x++)
			{
				buffer += databasename[buff + x];
			}
			if (buffer != XR(".cidia"))
			{
				fixbuff.append(XR(".cidia"));
			}
		}
		else
		{
			fixbuff.append(XR(".cidia"));
		}

		m_db.databasename = databasename;

		databaseN = fixbuff.c_str();


		std::ofstream database_file;
		database_file.open(std::string(database_path).append(XR("\\")).append(fixbuff));

		database_file.close();

		std::cout << "database was created" << std::endl;
	}
	else
	{
		std::cout << XR("database already exist.") << std::endl;
	}
}

bool database::Open(UIS)
{
	const char* database_path = (XR("D:\\Program Files (x86)\\CidiaDatabase\\database"));
	std::string buffer = (std::string(database_path).append(XR("\\")).append(databasename).append(XR(".cidia")));
	if (!fs::exists(buffer))
		return false;
	FileStream(buffer.c_str()).read(m_db);
	if (!m_db.username.empty())
	{
		if (m_db.username == user && m_db.password == pass)
			return true;
		else {
			db _th;
			m_db = _th;
			return false;
		}
	}
	return false;

}

void database::CreateTable(const char* table_name) {
	table _this;
	_this.name = table_name;
	m_db.tables.push_back(_this);
}

void database::InsertTableData(const char* table_name, const char* table_data_key, const char* table_data) {
	
	auto it = std::find_if(m_db.tables.begin(), m_db.tables.end(),
		[table_name](const table& t) { return t.name == table_name; });

	if (it != m_db.tables.end()) {
		size_t index = std::distance(m_db.tables.begin(), it);
		m_db.tables[index].data[table_data_key].push_back(table_data);
	}
	else {
		// Table not found, you may want to handle this case accordingly.
		// For now, the function returns without doing anything.
		return;
	}
	
}

std::string database::GetTableData(const char* table_name, const char* key)
{
	auto it = std::find_if(m_db.tables.begin(), m_db.tables.end(),
		[table_name](const table& t) { return t.name == table_name; });

	if (it != m_db.tables.end()) {
		size_t index = std::distance(m_db.tables.begin(), it);
		auto x = m_db.tables[index].data.find(key);

		if (x != m_db.tables[index].data.end())
		{
			if (!x->second.empty())
			{
				std::string data;
				for (const auto& value : x->second)
				{
					//std::cout << value.c_str() << std::endl;
					data.append(value).append("\n");
				}
				return data;
			}
			else
			{
				return x->second.empty() ? "None" : x->second[0].c_str();
			}
		}
		else
		{
			return "None";
		}
	}
	else
	{
		return "None";
	}
}

std::string database::GetTableValue(const char* table_name, const char* key, const char* key_value) {
	auto it = std::find_if(m_db.tables.begin(), m_db.tables.end(),
		[table_name](const table& t) { return t.name == table_name; });

	if (it != m_db.tables.end()) {
		size_t index = std::distance(m_db.tables.begin(), it);
		auto x = m_db.tables[index].data.find(key);

		if (x != m_db.tables[index].data.end())
		{
			if (!x->second.empty())
			{
				std::string data;
				for (const auto& value : x->second)
				{
					if (value == key_value)
						data = value;
				}
				if (!data.empty())
					return data;
				else
					return "None";
			}
			else
			{
				return x->second.empty() ? "None" : x->second[0].c_str();
			}
		}
		else
		{
			return "None";
		}
	}
	else
	{
		return "None";
	}
}

std::string database::FindTableData(const char* table_name, const char* key, const char* data) {
	auto it = std::find_if(m_db.tables.begin(), m_db.tables.end(),
		[table_name](const table& t) { return t.name == table_name; });

	if (it != m_db.tables.end()) {
		size_t index = std::distance(m_db.tables.begin(), it);
		auto x = m_db.tables[index].data.find(key);

		if (x != m_db.tables[index].data.end())
		{
			if (!x->second.empty())
			{
				std::string data_t;
				for (const auto& value : x->second)
				{
					if (value.find(data) != std::string::npos)
						data_t = value;
				}
				if (!data_t.empty())
					return data_t;
				else
					return "None";
			}
			else
			{
				return x->second.empty() ? "None" : x->second[0].c_str();
			}
		}
		else
		{
			return "None";
		}
	}
	else
	{
		return "None";
	}
}

std::string database::GetTableCombo(const char* table_name, const char* key1, const char* key2, const char* hint) {
	if (hint != NULL)
	{
		auto it = std::find_if(m_db.tables.begin(), m_db.tables.end(),
			[table_name](const table& t) { return t.name == table_name; });

		if (it != m_db.tables.end()) {
			size_t index = std::distance(m_db.tables.begin(), it);
			auto x = m_db.tables[index].data.find(key1);
			auto x2 = m_db.tables[index].data.find(key2);

			if (x != m_db.tables[index].data.end())
			{
				if (!x->second.empty())
				{
					std::vector<std::string> data1;
					std::vector<std::string> data2;
					std::string data;

					for (const auto& value : x->second)
					{
						data1.push_back(value);
					}
					for (const auto& value : x2->second)
					{
						data2.push_back(value);
					}
					for (int i = 0; i < data1.size(); i++)
					{
						data += data1[i] + " - " + data2[i] + "\n";
					}

					if (!data.empty())
						return data;
					else
						return "None";
				}
				else
				{
					return x->second.empty() ? "None" : x->second[0].c_str();
				}
			}
			else
			{
				return "None";
			}
		}
		else
		{
			return "None";
		}
	}
	else
	{
		auto it = std::find_if(m_db.tables.begin(), m_db.tables.end(),
			[table_name](const table& t) { return t.name == table_name; });

		if (it != m_db.tables.end()) {
			size_t index = std::distance(m_db.tables.begin(), it);
			auto x = m_db.tables[index].data.find(key1);
			auto x2 = m_db.tables[index].data.find(key2);

			if (x != m_db.tables[index].data.end())
			{
				if (!x->second.empty())
				{
					std::string data;
					int c = 0;
					bool found = false;
					for (const auto& value : x->second)
					{
						if (!found)
						{
							if (value.find(hint) != std::string::npos)
							{
								data.append(value);
								found = true;
							}
							else
								c += 1;
						}
					}

					if (found)
					{
						data.append(" - ").append(x2->second[c]);
					}
					else
					{
						c = 0;
						for (const auto& value : x2->second)
						{
							if (!found)
							{
								if (value.find(hint) != std::string::npos)
								{
									data.append(value);
									found = true;
								}
								else
									c += 1;
							}
						}
					}

					if (found)
					{
						data.append(" - ").append(x->second[c]);
					}
					else
					{
						data = "None";
					}

					if (!data.empty())
						return data;
					else
						return "None";
				}
				else
				{
					return x->second.empty() ? "None" : x->second[0].c_str();
				}
			}
			else
			{
				return "None";
			}
		}
		else
		{
			return "None";
		}
	}
}

void database::save() {
	this->SaveDatabase();
}