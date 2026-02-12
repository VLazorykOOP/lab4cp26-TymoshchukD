#pragma once

namespace Library {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace MySql::Data::MySqlClient;
	using namespace System::Data;


	/// <summary>
	/// Сводка для MyForm
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:


		MyForm(void)
		{
			InitializeComponent();

			String^ connStr = "server=localhost;user=root;password=root;database=library_db;";
			conn = gcnew MySqlConnection(connStr);

			LoadBooks();
		}


		

	protected:
		/// <summary>
		/// Освободить все используемые ресурсы.
		/// </summary>
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::DataGridView^ dataGridView1;
	private: System::Windows::Forms::Button^ btnAdd;
	private: System::Windows::Forms::Button^ btnDelete;
	private: System::Windows::Forms::Button^ btnRefresh;
	private: System::Windows::Forms::Button^ btnExit;

	protected:





	private:
		MySqlConnection^ conn;

		void LoadBooks()
		{
			try
			{
				dataGridView1->DataSource = nullptr;   // <-- важливо

				conn->Open();
				MySqlDataAdapter^ da =
					gcnew MySqlDataAdapter("SELECT * FROM books WHERE is_deleted = 0", conn);

				DataTable^ dt = gcnew DataTable();
				da->Fill(dt);
				conn->Close();

				dataGridView1->AutoGenerateColumns = true;
				dataGridView1->DataSource = dt;
			}
			catch (Exception^ ex)
			{
				MessageBox::Show(ex->Message, "Помилка");
				if (conn->State == ConnectionState::Open) conn->Close();
			}
		}

		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Требуемый метод для поддержки конструктора — не изменяйте 
		/// содержимое этого метода с помощью редактора кода.
		/// </summary>
		void InitializeComponent(void)
		{
			this->dataGridView1 = (gcnew System::Windows::Forms::DataGridView());
			this->btnAdd = (gcnew System::Windows::Forms::Button());
			this->btnDelete = (gcnew System::Windows::Forms::Button());
			this->btnRefresh = (gcnew System::Windows::Forms::Button());
			this->btnExit = (gcnew System::Windows::Forms::Button());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataGridView1))->BeginInit();
			this->SuspendLayout();
			// 
			// dataGridView1
			// 
			this->dataGridView1->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
			this->dataGridView1->Location = System::Drawing::Point(37, 37);
			this->dataGridView1->Name = L"dataGridView1";
			this->dataGridView1->RowHeadersWidth = 51;
			this->dataGridView1->RowTemplate->Height = 24;
			this->dataGridView1->Size = System::Drawing::Size(558, 198);
			this->dataGridView1->TabIndex = 0;
			// 
			// btnAdd
			// 
			this->btnAdd->Location = System::Drawing::Point(37, 277);
			this->btnAdd->Name = L"btnAdd";
			this->btnAdd->Size = System::Drawing::Size(75, 23);
			this->btnAdd->TabIndex = 1;
			this->btnAdd->Text = L"Додати";
			this->btnAdd->UseVisualStyleBackColor = true;
			this->btnAdd->Click += gcnew System::EventHandler(this, &MyForm::btnAdd_Click);
			// 
			// btnDelete
			// 
			this->btnDelete->Location = System::Drawing::Point(507, 277);
			this->btnDelete->Name = L"btnDelete";
			this->btnDelete->Size = System::Drawing::Size(88, 23);
			this->btnDelete->TabIndex = 2;
			this->btnDelete->Text = L"Видалити";
			this->btnDelete->UseVisualStyleBackColor = true;
			this->btnDelete->Click += gcnew System::EventHandler(this, &MyForm::btnDelete_Click);
			// 
			// btnRefresh
			// 
			this->btnRefresh->Location = System::Drawing::Point(190, 277);
			this->btnRefresh->Name = L"btnRefresh";
			this->btnRefresh->Size = System::Drawing::Size(75, 23);
			this->btnRefresh->TabIndex = 3;
			this->btnRefresh->Text = L"Оновити";
			this->btnRefresh->UseVisualStyleBackColor = true;
			this->btnRefresh->Click += gcnew System::EventHandler(this, &MyForm::btnRefresh_Click);
			// 
			// btnExit
			// 
			this->btnExit->Location = System::Drawing::Point(356, 277);
			this->btnExit->Name = L"btnExit";
			this->btnExit->Size = System::Drawing::Size(75, 23);
			this->btnExit->TabIndex = 4;
			this->btnExit->Text = L"Вихід";
			this->btnExit->UseVisualStyleBackColor = true;
			this->btnExit->Click += gcnew System::EventHandler(this, &MyForm::btnExit_Click);
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(638, 371);
			this->Controls->Add(this->btnExit);
			this->Controls->Add(this->btnRefresh);
			this->Controls->Add(this->btnDelete);
			this->Controls->Add(this->btnAdd);
			this->Controls->Add(this->dataGridView1);
			this->Name = L"MyForm";
			this->Text = L"MyForm";
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataGridView1))->EndInit();
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void btnRefresh_Click(System::Object^ sender, System::EventArgs^ e) {
		LoadBooks();

	}
private: System::Void btnExit_Click(System::Object^ sender, System::EventArgs^ e) {
	this->Close();

}
private: System::Void btnDelete_Click(System::Object^ sender, System::EventArgs^ e) {
	if (dataGridView1->SelectedRows->Count == 0)
	{
		MessageBox::Show("Вибери рядок для видалення");
		return;
	}

	int id = Convert::ToInt32(dataGridView1->SelectedRows[0]->Cells["id"]->Value);

	try
	{
		conn->Open();
		MySqlCommand^ cmd = gcnew MySqlCommand(
			"UPDATE books SET is_deleted = 1 WHERE id=@id", conn);
		cmd->Parameters->AddWithValue("@id", id);
		cmd->ExecuteNonQuery();

		conn->Close();
		LoadBooks();
	}
	catch (Exception^ ex)
	{
		MessageBox::Show(ex->Message);
		if (conn->State == ConnectionState::Open) conn->Close();
	}

}
private: System::Void btnAdd_Click(System::Object^ sender, System::EventArgs^ e) {
	try
	{
		conn->Open();
		MySqlCommand^ cmd = gcnew MySqlCommand(
			"INSERT INTO books(author,title) VALUES('Test','TestBook')", conn);
		cmd->ExecuteNonQuery();
		conn->Close();
		LoadBooks();
	}
	catch (Exception^ ex)
	{
		MessageBox::Show(ex->Message);
		if (conn->State == ConnectionState::Open) conn->Close();
	}

}
};
}
