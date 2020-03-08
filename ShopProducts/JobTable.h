#pragma once

using namespace System;
using namespace System::Collections::Generic;
using namespace System::ComponentModel;
using namespace System::IO;
using namespace System::Windows::Forms;

public ref class JobTable
{
private:
	static List<String^>^ listDTableTxt;
	static String^ textFile;
public:
	static int prodId;
	static int departId;

	JobTable() {
		prodId = GetProdId();
		departId = GetDepartId();
	}

	static int SaveAllId() {
		StreamWriter^ pwriter = gcnew StreamWriter(L"ShopProducts/IdTables");
		try
		{
			pwriter->WriteLine("DepartTable|" + departId);
			pwriter->WriteLine("ProdTable|" + prodId);
			pwriter->Close();
			return 0;
		}
		catch (System::Exception^ e)
		{
			pwriter->Close();
			return -1;
		}
	}

	static int GetDepartId() {
		array<String^>^ str;
		StreamReader^ reader = gcnew StreamReader(L"ShopProducts/IdTables");
		try
		{
			do
			{
				str = reader->ReadLine()->Split((char)'|');
				if (str[0] == System::Convert::ToString("DepartTable")) {
					reader->Close();
					return System::Convert::ToInt32(str[1]);
				}
			} while (reader->Peek() != -1);
			reader->Close();
		}
		catch (System::Exception^ e)
		{
			reader->Close();
			return 0;
		}
	}
	static int GetProdId() {
		array<String^>^ str;
		StreamReader^ reader = gcnew StreamReader(L"ShopProducts/IdTables");
		try
		{
			do
			{
				str = reader->ReadLine()->Split((char)'|');
				if (str[0] == System::Convert::ToString("ProdTable")) {
					reader->Close();
					return System::Convert::ToInt32(str[1]);
				}
			} while (reader->Peek() != -1);
			reader->Close();
		}
		catch (System::Exception^ e)
		{
			reader->Close();
			return 0;
		}
	}

	static int WriteRowsList(List<String^>^ listData, String ^ name)
	{
		StreamWriter^ pwriter = gcnew StreamWriter(name);
		try
		{
			for each (String^ var in listData)
			{
				pwriter->WriteLine(var);
			}
			pwriter->Close();
			return 0;
		}
		catch (System::Exception^ e)
		{
			pwriter->Close();
			return -1;
		}
	}
	static List<String^>^ ReadRwos(String^ name) {
		listDTableTxt = gcnew List<String^>();
		StreamReader^ reader = gcnew StreamReader(name);
		try
		{
			do
			{
				listDTableTxt->Add(reader->ReadLine());
			} while (reader->Peek() != -1);
			reader->Close();
			return listDTableTxt;
		}
		catch (System::Exception^ e)
		{
			reader->Close();
			return listDTableTxt = nullptr;
		}
	}
	static int AddNewRowsList(List<String^>^ listData, String^ name) {
		StreamWriter^ pwriter = gcnew StreamWriter(name, true);
		try
		{
			for each (String^ var in listData)
			{
				pwriter->WriteLine(var);
			}
			pwriter->Close();
			return 0;
		}
		catch (System::Exception^ e)
		{
			pwriter->Close();
			return -1;
		}
	}
	static int AddNewRow(String^ Data, String^ name) {
		StreamWriter^ pwriter = gcnew StreamWriter(name, true);
		try
		{
			pwriter->WriteLine(Data);
			pwriter->Close();
			return 0;
		}
		catch (System::Exception^ e)
		{
			pwriter->Close();
			return -1;
		}
	}
	static List<String^>^ Search(String^ nameSearch, String^ nameTable) {
		listDTableTxt = gcnew List<String^>();
		StreamReader^ reader = gcnew StreamReader(nameTable);
		try
		{
			do
			{
				textFile = String::Concat(reader->ReadLine());
				if (textFile->Contains(nameSearch) == true)
					listDTableTxt->Add(textFile);
			} while (reader->Peek() != -1);
			reader->Close();
			return listDTableTxt;
		}
		catch (System::Exception^ e)
		{
			reader->Close();
			return listDTableTxt = nullptr;
		}
	}
	static List<String^>^ DeleteRow(String^ strDelete, String^ nameTable) {
		listDTableTxt = gcnew List<String^>();
		StreamReader^ reader = gcnew StreamReader(nameTable);
		try
		{
			do
			{
				textFile = String::Concat(reader->ReadLine());
				if (textFile->Contains(strDelete) != true)
					listDTableTxt->Add(textFile);
			} while (reader->Peek() != -1);
			reader->Close();
			WriteRowsList(listDTableTxt, nameTable);
			return listDTableTxt;
		}
		catch (System::Exception^ e)
		{
			reader->Close();
			return listDTableTxt = nullptr;
		}
	}
	static List<String^>^ DeleteRow(int IdRow, String^ nameTable) {
		listDTableTxt = gcnew List<String^>();
		StreamReader^ reader = gcnew StreamReader(nameTable);
		try
		{
			array<String^>^ str;
			do
			{
				textFile = String::Concat(reader->ReadLine());
				str = textFile->Split((char)'|');
				if (str[0]->Contains(Convert::ToString(IdRow)) != true)
					listDTableTxt->Add(textFile);
			} while (reader->Peek() != -1);
			reader->Close();
			WriteRowsList(listDTableTxt, nameTable);
			return listDTableTxt;
		}
		catch (System::Exception^ e)
		{
			reader->Close();
			return listDTableTxt = nullptr;
		}
	}

	static void UpdateComboBoxProd(System::Windows::Forms::ComboBox^ comboObject) 
	{
		array<String^>^ str;
		comboObject->Items->Clear();
		List<String^>^ list = ReadRwos(L"ShopProducts/DepartTable");
		comboObject->Items->Add(L"Все отделы");
		comboObject->SelectedItem = L"Все отделы";
		for each (String^ item in list)
		{
			str = item->Split((char)'|');
			comboObject->Items->Add(str[1]);
		}
	}

	static void UpdateDataGridViewProd(System::Windows::Forms::DataGridView^ dataGrid, String ^ nameDep)
	{
		List<String^>^ list;
		if (nameDep->Contains(L"Все отделы") != true)
		{
			list = SearchListProdDeparId(SearchDepart(nameDep));
		}
		else
		{
			list = ReadRwos(L"ShopProducts/ProdTable");
		}
		dataGrid->Rows->Clear();
		int index = 0;

		for each (String^ carrDesc in list)
		{
			array<String^>^ str;
			dataGrid->Rows->Add();
			str = carrDesc->Split((char)'|');
			dataGrid->Rows[index]->Cells[0]->Value = SearchDepartId(Convert::ToInt32(str[6]));
			dataGrid->Rows[index]->Cells[1]->Value = str[1];
			dataGrid->Rows[index]->Cells[2]->Value = str[2];
			dataGrid->Rows[index]->Cells[3]->Value = str[3];
			dataGrid->Rows[index]->Cells[4]->Value = str[4];
			dataGrid->Rows[index]->Cells[5]->Value = str[5];
			dataGrid->Rows[index]->Cells[6]->Value = str[0];
			index++;
		}
	}

	static List<String^>^ SearchListProdDeparId(int DeparId)
	{
		array<String^>^ str;
		List<String^>^ returnList = gcnew List<String^>();
		List<String^>^ list = ReadRwos(L"ShopProducts/ProdTable");
		for each (String^ item in list)
		{
			str = item->Split((char)'|');
			if (str[6] == Convert::ToString(DeparId)) {
				returnList->Add(item);
			}
		}
		return returnList;
	}

	static int SearchDepart(String^ nameSearch)
	{
		array<String^>^ str;
		List<String^>^ list = ReadRwos(L"ShopProducts/DepartTable");
		for each (String^ item in list)
		{
			str = item->Split((char)'|');
			if (str[1] == nameSearch) {
				return Convert::ToInt32(str[0]);
			}
		}
	}

	static String^ SearchProdId(int id)
	{
		array<String^>^ str;
		List<String^>^ list = ReadRwos(L"ShopProducts/ProdTable");
		for each (String^ item in list)
		{
			str = item->Split((char)'|');
			if (str[0] == Convert::ToString(id)) {
				return item;
			}
		}
	}

	static String^ SearchDepartId(int id)
	{
		array<String^>^ str;
		List<String^>^ list = ReadRwos(L"ShopProducts/DepartTable");
		for each (String^ item in list)
		{
			str = item->Split((char)'|');
			if (Convert::ToInt32(str[0]) == id) {
				return str[1];
			}
		}
	}
	static void SearchProduct(System::Windows::Forms::DataGridViewSelectedRowCollection^ row)
	{
		DeleteRow(Convert::ToInt32(row[0]->Cells[6]->Value), L"ShopProducts/ProdTable");
	}

	static void DeletDepar(String^ nameDep) 
	{
		int depId = SearchDepart(nameDep);
		DeleteRowsDepId(depId, L"ShopProducts/ProdTable");
		DeleteRow(depId, L"ShopProducts/DepartTable");

	}

	static List<String^>^ DeleteRowsDepId(int IdRow, String^ nameTable) {
		listDTableTxt = gcnew List<String^>();
		StreamReader^ reader = gcnew StreamReader(nameTable);
		try
		{
			array<String^>^ str;
			do
			{
				textFile = String::Concat(reader->ReadLine());
				str = textFile->Split((char)'|');
				if (str[6]->Contains(Convert::ToString(IdRow)) != true)
					listDTableTxt->Add(textFile);
			} while (reader->Peek() != -1);
			reader->Close();
			WriteRowsList(listDTableTxt, nameTable);
			return listDTableTxt;
		}
		catch (System::Exception^ e)
		{
			reader->Close();
			return listDTableTxt = nullptr;
		}
	}
};
