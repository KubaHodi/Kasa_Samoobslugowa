#include "Article.h"
#include "PrzykladoweDane.h"

using namespace System::Windows::Forms::DataVisualization::Charting;
using namespace System::Text;
using namespace System::Globalization;

#using <Microsoft.VisualBasic.dll>
#pragma once

namespace app {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace Microsoft::VisualBasic;
	using namespace System::Globalization;
	/// <summary>
	/// Podsumowanie informacji o cash
	/// </summary>
	
	public ref class CashForm : public System::Windows::Forms::Form
	{
	public:
		CashForm(void)
		{
			InitializeComponent();
			produkty = PrzykladoweDane::PobierzProdukty();
			koszyk = gcnew List<ArtykulSpozywczy^>();
			dane->AutoGenerateColumns = true;

			dtProdukty = ProduktyDoTabeli(produkty);
			dvProdukty = gcnew DataView(dtProdukty);

			kategorie->Items->Clear();
			kategorie->Items->Add("Wszystko");
			kategorie->Items->Add("Pieczywo");
			kategorie->Items->Add("Owoce");
			kategorie->Items->Add("Warzywa");
			kategorie->SelectedIndex = 0;

			kategorie->SelectedIndexChanged +=
				gcnew System::EventHandler(this, &CashForm::kategorie_SelectedIndexChanged);


			dane->AutoGenerateColumns = true;
			dane->DataSource = dvProdukty;

			dane->SelectionMode = DataGridViewSelectionMode::FullRowSelect;
			dane->MultiSelect = false;

			if (dane->Columns["Dodaj"] == nullptr)
			{
				auto col = gcnew System::Windows::Forms::DataGridViewButtonColumn();
				col->Name = "Dodaj";
				col->HeaderText = "";
				col->Text = "Dodaj";
				col->UseColumnTextForButtonValue = true;
				dane->Columns->Add(col);
			}
		}

	protected:
		/// <summary>
		/// Wyczyœæ wszystkie u¿ywane zasoby.
		/// </summary>
		~CashForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Button^ pieczywo;
	private: System::Windows::Forms::Button^ warzywa;
	private: System::Windows::Forms::Button^ owoce;
	private: System::Windows::Forms::Button^ podsumowanie;
	protected:




	private: System::Windows::Forms::DataVisualization::Charting::Chart^ chart1;
	private: System::Windows::Forms::ComboBox^ kategorie;
	private: System::Windows::Forms::PictureBox^ pictureBox2;
	protected:

	private:
		/// <summary>
		/// Wymagana zmienna projektanta.
		/// </summary>
		///
		DataTable^ dtProdukty;
	private: System::Windows::Forms::Label^ sumaLabel;


		   DataView^ dvProdukty;
		String^ JednostkaNaTekst(Jednostka j) {
			switch (j) {
			case Jednostka::Sztuka: return "szt.";
			case Jednostka::Kilogram: return "kg.";
			default: return "";
			}
		}

		double PobierzIloscDlaKg()
		{
			while (true)
			{
				String^ s = Interaction::InputBox(
					"Ile kg produktu chcesz dodaæ",
					"Dodaj produkt (kg)",
					"1.0",
					-1,
					-1
				);

				if (String::IsNullOrWhiteSpace(s))
					return -1.0;

				s = s->Replace(",", ".");

				double val;
				if (Double::TryParse(s, NumberStyles::Float, CultureInfo::InvariantCulture, val) && val > 0.0)
					return val;

				MessageBox::Show("Wpisz poprawn¹ liczbê > 0", "B³¹d");
			}
		}

		void OdswiezWykres()
		{
			chart1->Series->Clear();
			chart1->ChartAreas->Clear();
			chart1->Titles->Clear();

			auto area = gcnew System::Windows::Forms::DataVisualization::Charting::ChartArea("A");
			area->AxisX->Interval = 1;
			area->AxisY->Title = "Wartoœæ [z³]";
			area->AxisY->LabelStyle->Format = "0.00 z³";
			chart1->ChartAreas->Add(area);

			auto s = gcnew System::Windows::Forms::DataVisualization::Charting::Series("Wartoœæ koszyka");
			s->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Column;
			s->IsValueShownAsLabel = true;
			s->LabelFormat = "{0:0.00} z³";
			chart1->Series->Add(s);

			chart1->Titles->Add("Wartoœæ produktów");

			for each (System::Data::DataRow ^ r in dtProdukty->Rows)
			{
				String^ rodzaj = safe_cast<String^>(r["Rodzaj"]);
				double wartosc = safe_cast<double>(r["Wartosc"]);
				s->Points->AddXY(rodzaj, wartosc);
			}
		}

		double PoliczSume(DataView^ view)
		{
			double suma = 0.0;

			for each (DataRowView ^ rv in view)
			{
				Object^ v = rv["Wartosc"];
				if (v == nullptr || v == DBNull::Value) continue;

				suma += safe_cast<double>(v);
			}
			return suma;
		}

		void AktualizujSumeNaEkranie()
		{
			double suma = PoliczSume(gcnew DataView(dtProdukty));

			sumaLabel->Text = String::Format(CultureInfo::GetCultureInfo("pl-PL"),
				"Suma: {0:0.00} z³", suma);
		}

		System::ComponentModel::Container ^components;
	private: System::Windows::Forms::DataGridView^ dane;
		List<ArtykulSpozywczy^>^ produkty;
		DataTable^ ProduktyDoTabeli(List<ArtykulSpozywczy^>^ lista) {
			DataTable^ dt = gcnew DataTable();
			dt->Columns->Add("Rodzaj", String::typeid);
			dt->Columns->Add("Jednostka", String::typeid);
			dt->Columns->Add("Cena", double::typeid);
			dt->Columns->Add("Ilosc", double::typeid);
			dt->Columns->Add("Wartosc", double::typeid);
			dt->Columns->Add("Kategoria", String::typeid);

			for each (ArtykulSpozywczy ^ a in lista)
			{
				DataRow^ r = dt->NewRow();
				r["Rodzaj"] = a->Rodzaj;
				r["Jednostka"] = JednostkaNaTekst(a->Jedn);
				r["Cena"] = a->Cena;
				r["Ilosc"] = a->Ilosc;
				r["Wartosc"] = a->Wartosc();
				r["Kategoria"] = a->Kategoria;
				dt->Rows->Add(r);
			}
			return dt;
		}

		List<ArtykulSpozywczy^>^ koszyk;

		ArtykulSpozywczy^ ZnajdzPoRodzaju(String^ rodzaj)
		{
			for each (ArtykulSpozywczy ^ a in produkty)
				if (a->Rodzaj == rodzaj) return a;
			return nullptr;
		}

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Metoda wymagana do obs³ugi projektanta — nie nale¿y modyfikowaæ
		/// jej zawartoœci w edytorze kodu.
		/// </summary>
		

		void InitializeComponent(void)
		{
			System::Windows::Forms::DataVisualization::Charting::ChartArea^ chartArea1 = (gcnew System::Windows::Forms::DataVisualization::Charting::ChartArea());
			System::Windows::Forms::DataVisualization::Charting::Legend^ legend1 = (gcnew System::Windows::Forms::DataVisualization::Charting::Legend());
			System::Windows::Forms::DataVisualization::Charting::Series^ series1 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
			this->pieczywo = (gcnew System::Windows::Forms::Button());
			this->warzywa = (gcnew System::Windows::Forms::Button());
			this->owoce = (gcnew System::Windows::Forms::Button());
			this->podsumowanie = (gcnew System::Windows::Forms::Button());
			this->pictureBox2 = (gcnew System::Windows::Forms::PictureBox());
			this->dane = (gcnew System::Windows::Forms::DataGridView());
			this->chart1 = (gcnew System::Windows::Forms::DataVisualization::Charting::Chart());
			this->kategorie = (gcnew System::Windows::Forms::ComboBox());
			this->sumaLabel = (gcnew System::Windows::Forms::Label());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox2))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dane))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->chart1))->BeginInit();
			this->SuspendLayout();
			// 
			// pieczywo
			// 
			this->pieczywo->BackColor = System::Drawing::Color::DarkSlateGray;
			this->pieczywo->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 48, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(238)));
			this->pieczywo->ForeColor = System::Drawing::SystemColors::Control;
			this->pieczywo->Location = System::Drawing::Point(12, 119);
			this->pieczywo->Name = L"pieczywo";
			this->pieczywo->Size = System::Drawing::Size(350, 173);
			this->pieczywo->TabIndex = 3;
			this->pieczywo->Text = L"Pieczywo";
			this->pieczywo->UseVisualStyleBackColor = false;
			this->pieczywo->Click += gcnew System::EventHandler(this, &CashForm::button1_Click);
			// 
			// warzywa
			// 
			this->warzywa->BackColor = System::Drawing::Color::DarkSlateGray;
			this->warzywa->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 48, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(238)));
			this->warzywa->ForeColor = System::Drawing::SystemColors::Control;
			this->warzywa->Location = System::Drawing::Point(1542, 119);
			this->warzywa->Name = L"warzywa";
			this->warzywa->Size = System::Drawing::Size(350, 173);
			this->warzywa->TabIndex = 4;
			this->warzywa->Text = L"Warzywa";
			this->warzywa->UseVisualStyleBackColor = false;
			this->warzywa->Click += gcnew System::EventHandler(this, &CashForm::button2_Click);
			// 
			// owoce
			// 
			this->owoce->BackColor = System::Drawing::Color::DarkSlateGray;
			this->owoce->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Zoom;
			this->owoce->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 48, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(238)));
			this->owoce->ForeColor = System::Drawing::SystemColors::Control;
			this->owoce->Location = System::Drawing::Point(793, 119);
			this->owoce->Name = L"owoce";
			this->owoce->Size = System::Drawing::Size(350, 173);
			this->owoce->TabIndex = 5;
			this->owoce->Text = L"Owoce";
			this->owoce->UseVisualStyleBackColor = false;
			this->owoce->Click += gcnew System::EventHandler(this, &CashForm::button3_Click);
			// 
			// podsumowanie
			// 
			this->podsumowanie->BackColor = System::Drawing::Color::LightSeaGreen;
			this->podsumowanie->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 24, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(238)));
			this->podsumowanie->ForeColor = System::Drawing::Color::Snow;
			this->podsumowanie->Location = System::Drawing::Point(12, 915);
			this->podsumowanie->Name = L"podsumowanie";
			this->podsumowanie->Size = System::Drawing::Size(410, 93);
			this->podsumowanie->TabIndex = 6;
			this->podsumowanie->Text = L"PrzejdŸ do podsumowania";
			this->podsumowanie->UseVisualStyleBackColor = false;
			this->podsumowanie->Click += gcnew System::EventHandler(this, &CashForm::podsumowanie_Click);
			// 
			// pictureBox2
			// 
			this->pictureBox2->Location = System::Drawing::Point(1630, 787);
			this->pictureBox2->Name = L"pictureBox2";
			this->pictureBox2->Size = System::Drawing::Size(227, 221);
			this->pictureBox2->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
			this->pictureBox2->TabIndex = 7;
			this->pictureBox2->TabStop = false;
			// 
			// dane
			// 
			this->dane->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
			this->dane->Location = System::Drawing::Point(548, 390);
			this->dane->Name = L"dane";
			this->dane->Size = System::Drawing::Size(816, 343);
			this->dane->TabIndex = 8;
			this->dane->CellContentClick += gcnew System::Windows::Forms::DataGridViewCellEventHandler(this, &CashForm::dane_CellContentClick);
			// 
			// chart1
			// 
			chartArea1->Name = L"ChartArea1";
			this->chart1->ChartAreas->Add(chartArea1);
			legend1->Name = L"Legend1";
			this->chart1->Legends->Add(legend1);
			this->chart1->Location = System::Drawing::Point(1413, 390);
			this->chart1->Name = L"chart1";
			series1->ChartArea = L"ChartArea1";
			series1->Legend = L"Legend1";
			series1->Name = L"Cena w z³";
			this->chart1->Series->Add(series1);
			this->chart1->Size = System::Drawing::Size(479, 363);
			this->chart1->TabIndex = 9;
			this->chart1->Text = L"chart1";
			// 
			// kategorie
			// 
			this->kategorie->FormattingEnabled = true;
			this->kategorie->Location = System::Drawing::Point(793, 34);
			this->kategorie->Name = L"kategorie";
			this->kategorie->Size = System::Drawing::Size(350, 21);
			this->kategorie->TabIndex = 10;
			// 
			// sumaLabel
			// 
			this->sumaLabel->AutoSize = true;
			this->sumaLabel->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 36, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(238)));
			this->sumaLabel->ForeColor = System::Drawing::SystemColors::Control;
			this->sumaLabel->Location = System::Drawing::Point(452, 953);
			this->sumaLabel->Name = L"sumaLabel";
			this->sumaLabel->Size = System::Drawing::Size(304, 55);
			this->sumaLabel->TabIndex = 11;
			this->sumaLabel->Text = L"Suma: 0,00z³";
			// 
			// CashForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::Color::SeaGreen;
			this->ClientSize = System::Drawing::Size(1904, 1041);
			this->Controls->Add(this->sumaLabel);
			this->Controls->Add(this->kategorie);
			this->Controls->Add(this->chart1);
			this->Controls->Add(this->dane);
			this->Controls->Add(this->pictureBox2);
			this->Controls->Add(this->podsumowanie);
			this->Controls->Add(this->owoce);
			this->Controls->Add(this->warzywa);
			this->Controls->Add(this->pieczywo);
			this->Name = L"CashForm";
			this->Text = L"cash";
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox2))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dane))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->chart1))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void dane_CellContentClick(System::Object^ sender, System::Windows::Forms::DataGridViewCellEventArgs^ e) {	
		if (e->RowIndex < 0) return;

		if (dane->Columns[e->ColumnIndex]->Name == "Dodaj")
		{
			DataGridViewRow^ row = dane->Rows[e->RowIndex];

			String^ jedn = safe_cast<String^>(row->Cells["Jednostka"]->Value);
			double cena = safe_cast<double>(row->Cells["Cena"]->Value);
			double ilosc = safe_cast<double>(row->Cells["Ilosc"]->Value);

			double dodaj = 1.0;

			if (jedn == "kg." || jedn == "kg")
			{
				dodaj = PobierzIloscDlaKg();
				if (dodaj < 0.0) return;
			}

			ilosc += dodaj;

			row->Cells["Ilosc"]->Value = ilosc;
			row->Cells["Wartosc"]->Value = ilosc * cena;

			dane->Refresh();
			OdswiezWykres();
			AktualizujSumeNaEkranie();
		}
	}

	private: System::Void kategorie_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e)
	{
		 String^ kat = safe_cast<String^>(kategorie->SelectedItem);
	
		 if (kat == "Wszystko")
		 {
		  dvProdukty->RowFilter = "";
		 }
		 else
		 {
		  dvProdukty->RowFilter = String::Format("Kategoria = '{0}'", kat);
		 }
	}

	private: System::Void button1_Click(System::Object^ sender, System::EventArgs^ e) {
		String^ kat = safe_cast<String^>(pieczywo->Name);
		dvProdukty->RowFilter = String::Format("Kategoria = '{0}'", kat);
	}
	private: System::Void button3_Click(System::Object^ sender, System::EventArgs^ e) {
		String^ kat = safe_cast<String^>(owoce->Name);
		dvProdukty->RowFilter = String::Format("Kategoria = '{0}'", kat);
	}
	private: System::Void button2_Click(System::Object^ sender, System::EventArgs^ e) {
		String^ kat = safe_cast<String^>(warzywa->Name);
		dvProdukty->RowFilter = String::Format("Kategoria = '{0}'", kat);
	}
	private: System::Void podsumowanie_Click(System::Object^ sender, System::EventArgs^ e) {
		StringBuilder^ sb = gcnew StringBuilder();
		sb->AppendLine("PODSUMOWANIE ZAKUPÓW");
		sb->AppendLine("----------------------");

		double suma = 0.0;

		for each (DataRow ^ r in dtProdukty->Rows)
		{
			Object^ ilObj = r["Ilosc"];
			Object^ waObj = r["Wartosc"];
			if (ilObj == nullptr || waObj == nullptr) continue;
			if (ilObj == DBNull::Value || waObj == DBNull::Value) continue;

			double ilosc = safe_cast<double>(ilObj);
			double wartosc = safe_cast<double>(waObj);

			if (ilosc <= 0.0) continue;

			String^ rodzaj = safe_cast<String^>(r["Rodzaj"]);
			String^ jedn = safe_cast<String^>(r["Jednostka"]);
			double cena = safe_cast<double>(r["Cena"]);

			sb->AppendLine(String::Format(
				CultureInfo::GetCultureInfo("pl-PL"),
				"{0} — {1:0.##} {2} x {3:0.00} z³ = {4:0.00} z³",
				rodzaj, ilosc, jedn, cena, wartosc
			));

			suma += wartosc;
		}

		sb->AppendLine("----------------------");
		sb->AppendLine(String::Format(CultureInfo::GetCultureInfo("pl-PL"),
			"SUMA: {0:0.00} z³", suma));

		MessageBox::Show(sb->ToString(), "Podsumowanie", MessageBoxButtons::OK, MessageBoxIcon::Information);
	}
};
}
