#include "RAM.h"
#include "Cache.h"
#include "CacheLine.h"
#include "AddressManager.h"
#include "ReplacementPolicy.h"
#include "WritePolicy.h"
#pragma once

namespace cachesim {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	public ref class Main : public System::Windows::Forms::Form {
	public:
		Main(void) {
			InitializeComponent();

			// Initialize ComboBoxes
			comboBox_cacheSize->Items->Add("8");
			comboBox_cacheSize->Items->Add("16");
			comboBox_cacheSize->Items->Add("32");
			comboBox_cacheSize->Items->Add("64");
			comboBox_cacheSize->Items->Add("128");
			comboBox_cacheSize->Items->Add("256");

			comboBox_block_line_size->Items->Add("2");
			comboBox_block_line_size->Items->Add("4");
			comboBox_block_line_size->Items->Add("8");

			comboBox_associativity->Items->Add("1 (Direct Mapping)");
			comboBox_associativity->Items->Add("2 (2-ways Associative Mapping)");
			comboBox_associativity->Items->Add("4 (4-ways Associative Mapping)");
			comboBox_associativity->Items->Add("nbOfCacheLines (Fully Associative Mapping)");

			
			button_generate_system->Click += gcnew System::EventHandler(this, &Main::button_generate_system_Click);
			button_read->Click += gcnew System::EventHandler(this, &Main::button_read_Click);
			button_write->Click += gcnew System::EventHandler(this, &Main::button_write_Click);


			cacheSetPanels = gcnew System::Collections::Generic::List<System::Windows::Forms::Panel^>();


			textBox_cache_hits = gcnew System::Windows::Forms::TextBox();
			textBox_cache_hits->Location = System::Drawing::Point(780, 20); 
			textBox_cache_hits->Size = System::Drawing::Size(100, 20); 
			this->Controls->Add(textBox_cache_hits);

			textBox_cache_misses = gcnew System::Windows::Forms::TextBox();
			textBox_cache_misses->Location = System::Drawing::Point(780, 45); 
			textBox_cache_misses->Size = System::Drawing::Size(100, 20);
			this->Controls->Add(textBox_cache_misses);

			textBox_cache_ratio = gcnew System::Windows::Forms::TextBox();
			textBox_cache_ratio->Location = System::Drawing::Point(780, 75); 
			textBox_cache_ratio->Size = System::Drawing::Size(100, 20);
			this->Controls->Add(textBox_cache_ratio);

			comboBox_replacement->Items->Add("LRU");
			comboBox_replacement->Items->Add("LFU");
			comboBox_replacement->Items->Add("Random");
			replacementPolicy = new ReplacementPolicy();

			comboBox_writePolicy->Items->Add("Write back");
			comboBox_writePolicy->Items->Add("Write through");
			writePolicy = new WritePolicy();

		}

	protected:
		~Main() {
			if (components) {
				delete components;
			}
		}

	private:
		RAM* ram;
		AddressManager* addressManager;
	private: System::Windows::Forms::Label^ label_cache_hits;
	private: System::Windows::Forms::Label^ label_cache_miss;


	private: System::Windows::Forms::Label^ label_ratio;
	private: System::Windows::Forms::ComboBox^ comboBox_replacement;

	private: System::Windows::Forms::Label^ label3;
	private: System::Windows::Forms::Label^ label_writePolicy;
	private: System::Windows::Forms::ComboBox^ comboBox_writePolicy;
	private: System::Windows::Forms::Label^ label_dataToWrite;
	private: System::Windows::Forms::Label^ label4;
	private: System::Windows::Forms::TextBox^ textBox_dataToWrite;
	private: System::Windows::Forms::Button^ button_write;
	private: System::Windows::Forms::Label^ label_cacheMemoryTitle;
	private: System::Windows::Forms::Label^ label_RAMMemoryTitle;



		   Cache* cache;
			 System::Void button_generate_system_Click(System::Object^ sender, System::EventArgs^ e);
			 System::Void button_read_Click(System::Object^ sender, System::EventArgs^ e);
			 int addressWidth;
			 System::Windows::Forms::Panel^ cacheVisualizationScrollPanel;
			 System::Collections::Generic::List<System::Windows::Forms::Panel^>^ cacheSetPanels;
			 System::Windows::Forms::Panel^ ramVisualizationScrollPanel;
			 int associativity;
			 ReplacementPolicy* replacementPolicy;
			 WritePolicy* writePolicy;
			 System::Collections::Generic::List<System::Windows::Forms::Label^>^ lastWritebackRAMLabels = gcnew System::Collections::Generic::List<System::Windows::Forms::Label^>();

	private:
		System::Windows::Forms::Panel^ cacheHeaderPanel;

	private: System::Windows::Forms::TextBox^ textBox_cache_hits;
	private: System::Windows::Forms::TextBox^ textBox_cache_misses;
	private: System::Windows::Forms::TextBox^ textBox_cache_ratio;
	private:
		System::Windows::Forms::Label^ lastHighlightedCacheByte = nullptr;
		System::Windows::Forms::Label^ lastHighlightedRAMByte = nullptr;




	private:
		System::Windows::Forms::TextBox^ textBox_address;
		System::Windows::Forms::Label^ label1;
		System::Windows::Forms::Label^ label2;
		System::Windows::Forms::Button^ button_read;
		System::Windows::Forms::Label^ label_cache_size;
		System::Windows::Forms::Label^ label_block_line_size;
		System::Windows::Forms::Label^ label_address_width;
		System::Windows::Forms::Label^ label_associativity;

		System::Windows::Forms::ComboBox^ comboBox_cacheSize;
		System::Windows::Forms::ComboBox^ comboBox_block_line_size;
		System::Windows::Forms::TextBox^ textbox_address_width;
		System::Windows::Forms::ComboBox^ comboBox_associativity;
		System::Windows::Forms::Button^ button_generate_system;

		System::ComponentModel::Container^ components;

#pragma region Windows Form Designer generated code
		void InitializeComponent(void) {
			this->label_cache_size = (gcnew System::Windows::Forms::Label());
			this->label_block_line_size = (gcnew System::Windows::Forms::Label());
			this->label_address_width = (gcnew System::Windows::Forms::Label());
			this->label_associativity = (gcnew System::Windows::Forms::Label());
			this->comboBox_cacheSize = (gcnew System::Windows::Forms::ComboBox());
			this->comboBox_block_line_size = (gcnew System::Windows::Forms::ComboBox());
			this->textbox_address_width = (gcnew System::Windows::Forms::TextBox());
			this->comboBox_associativity = (gcnew System::Windows::Forms::ComboBox());
			this->button_generate_system = (gcnew System::Windows::Forms::Button());
			this->textBox_address = (gcnew System::Windows::Forms::TextBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->button_read = (gcnew System::Windows::Forms::Button());
			this->cacheVisualizationScrollPanel = (gcnew System::Windows::Forms::Panel());
			this->ramVisualizationScrollPanel = (gcnew System::Windows::Forms::Panel());
			this->label_cache_hits = (gcnew System::Windows::Forms::Label());
			this->label_cache_miss = (gcnew System::Windows::Forms::Label());
			this->label_ratio = (gcnew System::Windows::Forms::Label());
			this->comboBox_replacement = (gcnew System::Windows::Forms::ComboBox());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->label_writePolicy = (gcnew System::Windows::Forms::Label());
			this->comboBox_writePolicy = (gcnew System::Windows::Forms::ComboBox());
			this->label_dataToWrite = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->textBox_dataToWrite = (gcnew System::Windows::Forms::TextBox());
			this->button_write = (gcnew System::Windows::Forms::Button());
			this->label_cacheMemoryTitle = (gcnew System::Windows::Forms::Label());
			this->label_RAMMemoryTitle = (gcnew System::Windows::Forms::Label());
			this->SuspendLayout();
			// 
			// label_cache_size
			// 
			this->label_cache_size->AutoSize = true;
			this->label_cache_size->Location = System::Drawing::Point(4, 15);
			this->label_cache_size->Name = L"label_cache_size";
			this->label_cache_size->Size = System::Drawing::Size(203, 25);
			this->label_cache_size->TabIndex = 0;
			this->label_cache_size->Text = L"Cache size (bytes) :";
			// 
			// label_block_line_size
			// 
			this->label_block_line_size->AutoSize = true;
			this->label_block_line_size->Location = System::Drawing::Point(4, 40);
			this->label_block_line_size->Name = L"label_block_line_size";
			this->label_block_line_size->Size = System::Drawing::Size(241, 25);
			this->label_block_line_size->TabIndex = 1;
			this->label_block_line_size->Text = L"Block/Line size (bytes) :";
			// 
			// label_address_width
			// 
			this->label_address_width->AutoSize = true;
			this->label_address_width->Location = System::Drawing::Point(4, 65);
			this->label_address_width->Name = L"label_address_width";
			this->label_address_width->Size = System::Drawing::Size(213, 25);
			this->label_address_width->TabIndex = 2;
			this->label_address_width->Text = L"Address width (bits) :";
			// 
			// label_associativity
			// 
			this->label_associativity->AutoSize = true;
			this->label_associativity->Location = System::Drawing::Point(272, 18);
			this->label_associativity->Name = L"label_associativity";
			this->label_associativity->Size = System::Drawing::Size(144, 25);
			this->label_associativity->TabIndex = 3;
			this->label_associativity->Text = L"Associativity :";
			// 
			// comboBox_cacheSize
			// 
			this->comboBox_cacheSize->FormattingEnabled = true;
			this->comboBox_cacheSize->Location = System::Drawing::Point(135, 15);
			this->comboBox_cacheSize->Name = L"comboBox_cacheSize";
			this->comboBox_cacheSize->Size = System::Drawing::Size(121, 33);
			this->comboBox_cacheSize->TabIndex = 4;
			// 
			// comboBox_block_line_size
			// 
			this->comboBox_block_line_size->FormattingEnabled = true;
			this->comboBox_block_line_size->Location = System::Drawing::Point(135, 40);
			this->comboBox_block_line_size->Name = L"comboBox_block_line_size";
			this->comboBox_block_line_size->Size = System::Drawing::Size(121, 33);
			this->comboBox_block_line_size->TabIndex = 5;
			// 
			// textbox_address_width
			// 
			this->textbox_address_width->Location = System::Drawing::Point(135, 65);
			this->textbox_address_width->Name = L"textbox_address_width";
			this->textbox_address_width->Size = System::Drawing::Size(121, 31);
			this->textbox_address_width->TabIndex = 6;
			// 
			// comboBox_associativity
			// 
			this->comboBox_associativity->FormattingEnabled = true;
			this->comboBox_associativity->Location = System::Drawing::Point(377, 12);
			this->comboBox_associativity->Name = L"comboBox_associativity";
			this->comboBox_associativity->Size = System::Drawing::Size(210, 33);
			this->comboBox_associativity->TabIndex = 7;
			// 
			// button_generate_system
			// 
			this->button_generate_system->Location = System::Drawing::Point(9, 116);
			this->button_generate_system->Name = L"button_generate_system";
			this->button_generate_system->Size = System::Drawing::Size(75, 23);
			this->button_generate_system->TabIndex = 8;
			this->button_generate_system->Text = L"GENERATE";
			this->button_generate_system->UseVisualStyleBackColor = true;
			// 
			// textBox_address
			// 
			this->textBox_address->Location = System::Drawing::Point(34, 158);
			this->textBox_address->Name = L"textBox_address";
			this->textBox_address->Size = System::Drawing::Size(105, 31);
			this->textBox_address->TabIndex = 9;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(10, 136);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(184, 25);
			this->label1->TabIndex = 10;
			this->label1->Text = L"Introduce address";
			this->label1->Click += gcnew System::EventHandler(this, &Main::label1_Click);
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(12, 161);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(38, 25);
			this->label2->TabIndex = 11;
			this->label2->Text = L"0X";
			// 
			// button_read
			// 
			this->button_read->Location = System::Drawing::Point(64, 186);
			this->button_read->Name = L"button_read";
			this->button_read->Size = System::Drawing::Size(75, 23);
			this->button_read->TabIndex = 12;
			this->button_read->Text = L"READ";
			this->button_read->UseVisualStyleBackColor = true;
			this->button_read->Click += gcnew System::EventHandler(this, &Main::button_read_Click_1);
			// 
			// cacheVisualizationScrollPanel
			// 
			this->cacheVisualizationScrollPanel->AutoScroll = true;
			this->cacheVisualizationScrollPanel->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->cacheVisualizationScrollPanel->Location = System::Drawing::Point(9, 265);
			this->cacheVisualizationScrollPanel->Name = L"cacheVisualizationScrollPanel";
			this->cacheVisualizationScrollPanel->Size = System::Drawing::Size(500, 400);
			this->cacheVisualizationScrollPanel->TabIndex = 0;
			// 
			// ramVisualizationScrollPanel
			// 
			this->ramVisualizationScrollPanel->AutoScroll = true;
			this->ramVisualizationScrollPanel->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->ramVisualizationScrollPanel->Location = System::Drawing::Point(532, 265);
			this->ramVisualizationScrollPanel->Name = L"ramVisualizationScrollPanel";
			this->ramVisualizationScrollPanel->Size = System::Drawing::Size(600, 400);
			this->ramVisualizationScrollPanel->TabIndex = 1;
			// 
			// label_cache_hits
			// 
			this->label_cache_hits->AutoSize = true;
			this->label_cache_hits->Location = System::Drawing::Point(690, 23);
			this->label_cache_hits->Name = L"label_cache_hits";
			this->label_cache_hits->Size = System::Drawing::Size(144, 25);
			this->label_cache_hits->TabIndex = 13;
			this->label_cache_hits->Text = L"CACHE HITS:";
			// 
			// label_cache_miss
			// 
			this->label_cache_miss->AutoSize = true;
			this->label_cache_miss->Location = System::Drawing::Point(690, 48);
			this->label_cache_miss->Name = L"label_cache_miss";
			this->label_cache_miss->Size = System::Drawing::Size(176, 25);
			this->label_cache_miss->TabIndex = 14;
			this->label_cache_miss->Text = L"CACHE MISSES:";
			this->label_cache_miss->Click += gcnew System::EventHandler(this, &Main::label_cache_miss_Click);
			// 
			// label_ratio
			// 
			this->label_ratio->AutoSize = true;
			this->label_ratio->Location = System::Drawing::Point(690, 73);
			this->label_ratio->Name = L"label_ratio";
			this->label_ratio->RightToLeft = System::Windows::Forms::RightToLeft::No;
			this->label_ratio->Size = System::Drawing::Size(81, 25);
			this->label_ratio->TabIndex = 17;
			this->label_ratio->Text = L"RATIO:";
			// 
			// comboBox_replacement
			// 
			this->comboBox_replacement->FormattingEnabled = true;
			this->comboBox_replacement->Location = System::Drawing::Point(377, 35);
			this->comboBox_replacement->Name = L"comboBox_replacement";
			this->comboBox_replacement->Size = System::Drawing::Size(121, 33);
			this->comboBox_replacement->TabIndex = 18;
			this->comboBox_replacement->SelectedIndexChanged += gcnew System::EventHandler(this, &Main::comboBox_replacement_SelectedIndexChanged);
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(272, 43);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(208, 25);
			this->label3->TabIndex = 19;
			this->label3->Text = L"Replacement Policy:";
			this->label3->Click += gcnew System::EventHandler(this, &Main::label3_Click);
			// 
			// label_writePolicy
			// 
			this->label_writePolicy->AutoSize = true;
			this->label_writePolicy->Location = System::Drawing::Point(272, 68);
			this->label_writePolicy->Name = L"label_writePolicy";
			this->label_writePolicy->Size = System::Drawing::Size(138, 25);
			this->label_writePolicy->TabIndex = 20;
			this->label_writePolicy->Text = L"Write Policy :";
			// 
			// comboBox_writePolicy
			// 
			this->comboBox_writePolicy->FormattingEnabled = true;
			this->comboBox_writePolicy->Location = System::Drawing::Point(377, 63);
			this->comboBox_writePolicy->Name = L"comboBox_writePolicy";
			this->comboBox_writePolicy->Size = System::Drawing::Size(121, 33);
			this->comboBox_writePolicy->TabIndex = 21;
			this->comboBox_writePolicy->SelectedIndexChanged += gcnew System::EventHandler(this, &Main::comboBox_writePolicy_SelectedIndexChanged);
			// 
			// label_dataToWrite
			// 
			this->label_dataToWrite->AutoSize = true;
			this->label_dataToWrite->Location = System::Drawing::Point(233, 136);
			this->label_dataToWrite->Name = L"label_dataToWrite";
			this->label_dataToWrite->Size = System::Drawing::Size(235, 25);
			this->label_dataToWrite->TabIndex = 22;
			this->label_dataToWrite->Text = L"Introduce byte to write :";
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(233, 158);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(38, 25);
			this->label4->TabIndex = 23;
			this->label4->Text = L"0X";
			// 
			// textBox_dataToWrite
			// 
			this->textBox_dataToWrite->Location = System::Drawing::Point(252, 155);
			this->textBox_dataToWrite->Name = L"textBox_dataToWrite";
			this->textBox_dataToWrite->Size = System::Drawing::Size(100, 31);
			this->textBox_dataToWrite->TabIndex = 24;
			// 
			// button_write
			// 
			this->button_write->Location = System::Drawing::Point(277, 186);
			this->button_write->Name = L"button_write";
			this->button_write->Size = System::Drawing::Size(75, 23);
			this->button_write->TabIndex = 25;
			this->button_write->Text = L"WRITE";
			this->button_write->UseVisualStyleBackColor = true;
			// 
			// label_cacheMemoryTitle
			// 
			this->label_cacheMemoryTitle->AutoSize = true;
			this->label_cacheMemoryTitle->Font = (gcnew System::Drawing::Font(L"Times New Roman", 10.125F, static_cast<System::Drawing::FontStyle>(((System::Drawing::FontStyle::Bold | System::Drawing::FontStyle::Italic)
				| System::Drawing::FontStyle::Underline)), System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->label_cacheMemoryTitle->Location = System::Drawing::Point(3, 231);
			this->label_cacheMemoryTitle->Name = L"label_cacheMemoryTitle";
			this->label_cacheMemoryTitle->Size = System::Drawing::Size(234, 31);
			this->label_cacheMemoryTitle->TabIndex = 26;
			this->label_cacheMemoryTitle->Text = L"CACHE MEMORY";
			// 
			// label_RAMMemoryTitle
			// 
			this->label_RAMMemoryTitle->AutoSize = true;
			this->label_RAMMemoryTitle->Font = (gcnew System::Drawing::Font(L"Times New Roman", 10.125F, static_cast<System::Drawing::FontStyle>(((System::Drawing::FontStyle::Bold | System::Drawing::FontStyle::Italic)
				| System::Drawing::FontStyle::Underline)), System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->label_RAMMemoryTitle->Location = System::Drawing::Point(717, 231);
			this->label_RAMMemoryTitle->Name = L"label_RAMMemoryTitle";
			this->label_RAMMemoryTitle->Size = System::Drawing::Size(201, 31);
			this->label_RAMMemoryTitle->TabIndex = 27;
			this->label_RAMMemoryTitle->Text = L"RAM MEMORY";
			// 
			// Main
			// 
			this->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(192)), static_cast<System::Int32>(static_cast<System::Byte>(192)),
				static_cast<System::Int32>(static_cast<System::Byte>(255)));
			this->ClientSize = System::Drawing::Size(1018, 659);
			this->Controls->Add(this->label_RAMMemoryTitle);
			this->Controls->Add(this->label_cacheMemoryTitle);
			this->Controls->Add(this->button_write);
			this->Controls->Add(this->textBox_dataToWrite);
			this->Controls->Add(this->label4);
			this->Controls->Add(this->label_dataToWrite);
			this->Controls->Add(this->comboBox_writePolicy);
			this->Controls->Add(this->label_writePolicy);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->comboBox_replacement);
			this->Controls->Add(this->label_ratio);
			this->Controls->Add(this->label_cache_miss);
			this->Controls->Add(this->label_cache_hits);
			this->Controls->Add(this->cacheVisualizationScrollPanel);
			this->Controls->Add(this->ramVisualizationScrollPanel);
			this->Controls->Add(this->button_read);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->textBox_address);
			this->Controls->Add(this->button_generate_system);
			this->Controls->Add(this->comboBox_associativity);
			this->Controls->Add(this->textbox_address_width);
			this->Controls->Add(this->comboBox_block_line_size);
			this->Controls->Add(this->comboBox_cacheSize);
			this->Controls->Add(this->label_associativity);
			this->Controls->Add(this->label_address_width);
			this->Controls->Add(this->label_block_line_size);
			this->Controls->Add(this->label_cache_size);
			this->Name = L"Main";
			this->Text = L"Main";
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void label_cache_miss_Click(System::Object^ sender, System::EventArgs^ e) {
	}
	private: System::Void label3_Click(System::Object^ sender, System::EventArgs^ e) {
	}
	private: System::Void comboBox_replacement_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e) {
	}

	private: System::Void label1_Click(System::Object^ sender, System::EventArgs^ e) {
	}
	private: System::Void comboBox_writePolicy_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e) {
	}
	private: System::Void button_read_Click_1(System::Object^ sender, System::EventArgs^ e) {
	}
	private: System::Void button_write_Click(System::Object^ sender, System::EventArgs^ e);


};
}
