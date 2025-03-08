#include "Main.h"
#include "RAM.h"
#include "Cache.h"
#include "CacheLine.h"
#include "AddressManager.h"
#include "CacheSet.h"
#include <msclr/marshal_cppstd.h>
#include <vector>
#include <string>
#include <bitset>

using namespace System;
using namespace System::Windows::Forms;

int cache_hits;
int cache_misses;
double cache_hit_miss_ratio;

static int globalAccessCounter = 0;


void cachesim::Main::button_generate_system_Click(System::Object^ sender, System::EventArgs^ e) {


    cache_hits = 0;
    cache_misses = 0;
    cache_hit_miss_ratio = 0;
    textBox_cache_hits->Text = "0";
    textBox_cache_misses->Text = "0";
    textBox_cache_ratio->Text = "0";

    try {
        addressWidth = Convert::ToInt32(textbox_address_width->Text);
        if (addressWidth < 1 || addressWidth > 16) {
            MessageBox::Show("Address width must be between 1 and 16.", "Invalid Input");
            return;
        }

        int cacheSize = Convert::ToInt32(comboBox_cacheSize->SelectedItem->ToString());
        int blockSize = Convert::ToInt32(comboBox_block_line_size->SelectedItem->ToString());
        String^ associativityText = comboBox_associativity->SelectedItem->ToString();
        int associativity = Char::IsDigit(associativityText[0])
            ? Convert::ToInt32(associativityText->Substring(0, 1))
            : cacheSize / blockSize;

        //parsam replacement policy-ul
        String^ replacementPolicyText = comboBox_replacement->SelectedItem->ToString();
        if (replacementPolicyText == "LRU") {
            replacementPolicy->setPolicy(0);
        }
        else if (replacementPolicyText == "LFU") {
            replacementPolicy->setPolicy(1);
        }
        else if (replacementPolicyText == "Random") {
            replacementPolicy->setPolicy(2);
        }
        else {
            MessageBox::Show("Invalid replacement policy selected.", "Error");
            return;
        }

        //parsam write policy-ul
        System::String^ selectedWritePolicy = comboBox_writePolicy->SelectedItem->ToString();
        if (selectedWritePolicy == "Write back") {
            writePolicy->setPolicy(0);
        }
        else {
            writePolicy->setPolicy(1);
        }

        addressManager = new AddressManager(addressWidth, cacheSize, blockSize, associativity);

        ram = new RAM(addressWidth);
        ram->generate();

        ramVisualizationScrollPanel->Controls->Clear();
        std::vector<unsigned char> ramData = ram->getMemory();
        int numBytesPerRow = 8;
        int lineHeight = 30;
        int boxWidth = 50;
        
        int addressLabelWidth = 60; 
        for (int row = 0; row < (int)ramData.size() / numBytesPerRow; ++row) {
            System::Windows::Forms::Panel^ rowPanel = gcnew System::Windows::Forms::Panel();
            rowPanel->Location = System::Drawing::Point(10, row * lineHeight);
            
            int addressLabelWidth = 60;
            rowPanel->Size = System::Drawing::Size(addressLabelWidth + numBytesPerRow * boxWidth, lineHeight);
            rowPanel->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;

            int startAddress = row * numBytesPerRow;

            System::Windows::Forms::Label^ addressLabel = gcnew System::Windows::Forms::Label();
            addressLabel->Location = System::Drawing::Point(0, 0);
            addressLabel->Size = System::Drawing::Size(addressLabelWidth, lineHeight);
            addressLabel->Text = "0x" + Convert::ToString(startAddress, 16)->ToUpper();
            addressLabel->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
            addressLabel->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
            addressLabel->BackColor = System::Drawing::Color::Plum;
            rowPanel->Controls->Add(addressLabel);

      
            for (int col = 0; col < numBytesPerRow; ++col) {
                System::Windows::Forms::Label^ byteLabel = gcnew System::Windows::Forms::Label();
                byteLabel->Location = System::Drawing::Point(addressLabelWidth + col * boxWidth, 0);
                byteLabel->Size = System::Drawing::Size(boxWidth, lineHeight);
                byteLabel->Text = "0x" + Convert::ToString(ramData[startAddress + col], 16)->ToUpper();
                byteLabel->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
                byteLabel->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
                rowPanel->Controls->Add(byteLabel);
            }

            ramVisualizationScrollPanel->Controls->Add(rowPanel);
        }

        ramVisualizationScrollPanel->AutoScrollMinSize = System::Drawing::Size(
            ramVisualizationScrollPanel->Width,
            ((int)ramData.size() / numBytesPerRow) * lineHeight
        );

        int numCacheSets = (cacheSize / blockSize) / associativity;
        std::vector<CacheSet> cacheSets;
        for (int i = 0; i < numCacheSets; ++i) {
            CacheSet cacheSet(associativity, blockSize);
            cacheSets.push_back(cacheSet);
        }
        if (cache) {
            delete cache;
        }
        cache = new Cache(cacheSets);
        cacheVisualizationScrollPanel->Controls->Clear();

        int cacheLineHeight = 30;
        int cacheBoxWidth = 50;
        int cacheSpacing = 5;

        int totalColumns = 1 + blockSize + 3;
        int lineWidth = totalColumns * cacheBoxWidth;

        array<System::Drawing::Color>^ alternateColors = gcnew array<System::Drawing::Color>(2) {
            System::Drawing::Color::LightGray,
                System::Drawing::Color::White
        };

        for (int setIdx = 0; setIdx < numCacheSets; ++setIdx) {
            for (int lineIdx = 0; lineIdx < associativity; ++lineIdx) {
                int row = setIdx * associativity + lineIdx;
                int yOffset = row * (cacheLineHeight + cacheSpacing);

                System::Windows::Forms::Panel^ linePanel = gcnew System::Windows::Forms::Panel();
                linePanel->Location = System::Drawing::Point(10, yOffset);
                linePanel->Size = System::Drawing::Size((blockSize + 4) * cacheBoxWidth, cacheLineHeight);
                linePanel->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
                linePanel->BackColor = System::Drawing::Color::White;

                // INDEX label
                System::Windows::Forms::Label^ lineIndexLabel = gcnew System::Windows::Forms::Label();
                lineIndexLabel->Location = System::Drawing::Point(0, 0);
                lineIndexLabel->Size = System::Drawing::Size(cacheBoxWidth, cacheLineHeight);
                lineIndexLabel->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
                lineIndexLabel->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
                lineIndexLabel->BackColor = System::Drawing::Color::Plum;
                // Font normal 
                lineIndexLabel->Font = gcnew System::Drawing::Font(lineIndexLabel->Font->FontFamily, lineIndexLabel->Font->Size, System::Drawing::FontStyle::Regular);

                if (lineIdx == 0) {
                    lineIndexLabel->Text = "SET " + setIdx.ToString() + "\n" + row.ToString();
                }
                else {
                    // Restul liniilor: doar indexul liniei
                    lineIndexLabel->Text = row.ToString();
                }

                linePanel->Controls->Add(lineIndexLabel);

                // DATE
                for (int blockIdx = 0; blockIdx < blockSize; ++blockIdx) {
                    System::Windows::Forms::Label^ blockLabel = gcnew System::Windows::Forms::Label();
                    blockLabel->Location = System::Drawing::Point((blockIdx + 1) * cacheBoxWidth, 0);
                    blockLabel->Size = System::Drawing::Size(cacheBoxWidth, cacheLineHeight);
                    blockLabel->Text = "-";
                    blockLabel->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
                    blockLabel->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
                    linePanel->Controls->Add(blockLabel);
                }

                // VALID
                System::Windows::Forms::Label^ validLabel = gcnew System::Windows::Forms::Label();
                validLabel->Location = System::Drawing::Point((blockSize + 1) * cacheBoxWidth, 0);
                validLabel->Size = System::Drawing::Size(cacheBoxWidth, cacheLineHeight);
                validLabel->Text = "0";
                validLabel->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
                validLabel->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
                linePanel->Controls->Add(validLabel);

                // DIRTY
                System::Windows::Forms::Label^ dirtyLabel = gcnew System::Windows::Forms::Label();
                dirtyLabel->Location = System::Drawing::Point((blockSize + 2) * cacheBoxWidth, 0);
                dirtyLabel->Size = System::Drawing::Size(cacheBoxWidth, cacheLineHeight);
                dirtyLabel->Text = "0";
                dirtyLabel->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
                dirtyLabel->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
                linePanel->Controls->Add(dirtyLabel);

                // TAG
                System::Windows::Forms::Label^ tagLabel = gcnew System::Windows::Forms::Label();
                tagLabel->Location = System::Drawing::Point((blockSize + 3) * cacheBoxWidth, 0);
                tagLabel->Size = System::Drawing::Size(cacheBoxWidth, cacheLineHeight);
                tagLabel->Text = "--";
                tagLabel->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
                tagLabel->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
                linePanel->Controls->Add(tagLabel);

                cacheVisualizationScrollPanel->Controls->Add(linePanel);
            }
        }
        int headerHeight = 30;
        if (this->cacheHeaderPanel != nullptr) {
            delete this->cacheHeaderPanel;
        }

        this->cacheHeaderPanel = gcnew System::Windows::Forms::Panel();
        this->cacheHeaderPanel->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
        this->cacheHeaderPanel->Location = System::Drawing::Point(
            cacheVisualizationScrollPanel->Location.X,
            cacheVisualizationScrollPanel->Location.Y - headerHeight
        );
        this->cacheHeaderPanel->Size = System::Drawing::Size(cacheVisualizationScrollPanel->Width, headerHeight);
        this->Controls->Add(this->cacheHeaderPanel);

        int offset = 10;
        System::Windows::Forms::Label^ vLabel = gcnew System::Windows::Forms::Label();
        vLabel->Location = System::Drawing::Point((blockSize + 1) * cacheBoxWidth + offset, 0);
        vLabel->Size = System::Drawing::Size(cacheBoxWidth, headerHeight);
        vLabel->Text = "V";
        vLabel->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
        vLabel->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
        vLabel->BackColor = System::Drawing::Color::Plum;
        this->cacheHeaderPanel->Controls->Add(vLabel);

        System::Windows::Forms::Label^ dLabel = gcnew System::Windows::Forms::Label();
        dLabel->Location = System::Drawing::Point((blockSize + 2) * cacheBoxWidth + offset, 0);
        dLabel->Size = System::Drawing::Size(cacheBoxWidth, headerHeight);
        dLabel->Text = "D";
        dLabel->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
        dLabel->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
        dLabel->BackColor = System::Drawing::Color::Plum;
        this->cacheHeaderPanel->Controls->Add(dLabel);

        System::Windows::Forms::Label^ tLabel = gcnew System::Windows::Forms::Label();
        tLabel->Location = System::Drawing::Point((blockSize + 3) * cacheBoxWidth + offset, 0);
        tLabel->Size = System::Drawing::Size(cacheBoxWidth, headerHeight);
        tLabel->Text = "T";
        tLabel->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
        tLabel->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
        tLabel->BackColor = System::Drawing::Color::Plum;
        this->cacheHeaderPanel->Controls->Add(tLabel);


        cacheVisualizationScrollPanel->AutoScrollMinSize = System::Drawing::Size(
            cacheVisualizationScrollPanel->Width,
            (numCacheSets * associativity) * (cacheLineHeight + cacheSpacing)
        );


    }
    catch (FormatException^) {
        MessageBox::Show("Please enter valid values for all fields.", "Error");
    }
    catch (Exception^ ex) {
        MessageBox::Show("An error occurred: " + ex->Message, "Error");
    }
}




void cachesim::Main::button_read_Click(System::Object^ sender, System::EventArgs^ e) {
    try {

        if (lastHighlightedCacheByte != nullptr) {
            lastHighlightedCacheByte->BackColor = System::Drawing::Color::White;
            lastHighlightedCacheByte = nullptr;
        }
        if (lastHighlightedRAMByte != nullptr) {
            lastHighlightedRAMByte->BackColor = System::Drawing::Color::White;
            lastHighlightedRAMByte = nullptr;
        }
        for each (System::Windows::Forms::Label ^ lbl in lastWritebackRAMLabels) {
            lbl->BackColor = System::Drawing::Color::White;
        }
        lastWritebackRAMLabels->Clear();

        // Parsez adresa
        System::String^ addressText = textBox_address->Text;
        if (addressText->Length == 0) {
            MessageBox::Show("Please enter a valid hexadecimal address.", "Error");
            return;
        }

        std::string addressString = msclr::interop::marshal_as<std::string>(addressText);
        int address;
        {
            std::stringstream ss;
            ss << std::hex << addressString;
            ss >> address;
            if (ss.fail()) {
                MessageBox::Show("Invalid hexadecimal address.", "Error");
                return;
            }
        }

        if (addressManager == nullptr) {
            MessageBox::Show("AddressManager not initialized.", "Error");
            return;
        }
        if (cache == nullptr) {
            MessageBox::Show("Cache not initialized.", "Error");
            return;
        }

        std::bitset<32> binaryAddress(address);
        std::string binaryAddressString = binaryAddress.to_string().substr(32 - addressWidth);
        addressManager->getTagIndexOffset_ofAddress(binaryAddressString);
        int tag = addressManager->getTag();
        int index = addressManager->getIndex();
        int offset = addressManager->getOffset();

        std::vector<CacheSet>& cacheSets = cache->getCacheSets();
        if (index < 0 || index >= (int)cacheSets.size()) {
            MessageBox::Show("Index out of bounds.", "Error");
            return;
        }

        CacheSet& cacheSet = cacheSets[index];
        std::vector<CacheLine>& cacheLines = cacheSet.getCacheLines();

        CacheLine* targetLine = nullptr;
        bool cacheHit = false;

        for (CacheLine& line : cacheLines) {
            if (line.isValid() && line.getTag() == tag) {
                cacheHit = true;
                targetLine = &line;
                targetLine->setAccessedLineCount(targetLine->getAccessedLineCount() + 1);
                targetLine->setLeastRecentlyused(globalAccessCounter++);
                break;
            }
        }

        int blockSize = addressManager->getBlockSize();

        if (!cacheHit) {
            cache_misses++;
            textBox_cache_misses->Text = cache_misses.ToString();

            int RAM_block_toCopy_index = address / blockSize;
            int startIndex = RAM_block_toCopy_index * blockSize;
            const std::vector<unsigned char>& constRamMemory = ram->getMemory();
           

            if (startIndex < 0 || startIndex + blockSize >(int)constRamMemory.size()) {
                MessageBox::Show("Invalid RAM block index.", "Error");
                return;
            }

            std::vector<unsigned char> blockData;
            for (int i = 0; i < blockSize; i++) {
                blockData.push_back(constRamMemory[startIndex + i]);
            }

            targetLine = nullptr;
            for (CacheLine& line : cacheLines) {
                if (!line.isValid()) {
                    targetLine = &line;
                    break;
                }
            }

            if (targetLine == nullptr) {
                int victimIndex = replacementPolicy->replaceLineIndex(cacheLines);
                CacheLine* victimLine = &cacheLines[victimIndex];

                // Write-back daca victimLine are dirtyBit=1
                if (victimLine->isDirty()) {
                    int offsetWidth = addressManager->getOffsetWidth();
                    int indexWidth = addressManager->getIndexWidth();

                    int blockStartAddress = (victimLine->getTag() << (indexWidth + offsetWidth)) + (index << offsetWidth);
                    // Avem nevoie de scriere în RAM
                    std::vector<unsigned char>& ramMemory = const_cast<std::vector<unsigned char>&>(ram->getMemoryRef());
                    std::vector<unsigned char>& victimData = victimLine->getCacheLineData();

                    for (int i = 0; i < blockSize; i++) {
                        ramMemory[blockStartAddress + i] = victimData[i];
                    }

                    int numBytesPerRow = 8;
                    for (int i = 0; i < blockSize; i++) {
                        int ramAddress = blockStartAddress + i;
                        int ramRow = ramAddress / numBytesPerRow;
                        int ramOff = ramAddress % numBytesPerRow;

                        if (ramRow >= 0 && ramRow < ramVisualizationScrollPanel->Controls->Count) {
                            System::Windows::Forms::Panel^ ramLinePanel = (System::Windows::Forms::Panel^)ramVisualizationScrollPanel->Controls[ramRow];
                            int dataControlIndexRAM = ramOff + 1;
                            if (dataControlIndexRAM >= 0 && dataControlIndexRAM < ramLinePanel->Controls->Count) {
                                System::Windows::Forms::Label^ ramByteLabel = (System::Windows::Forms::Label^)ramLinePanel->Controls[dataControlIndexRAM];
                                ramByteLabel->Text = "0x" + Convert::ToString(victimData[i], 16)->ToUpper();
                                ramByteLabel->BackColor = System::Drawing::Color::LightBlue;
                                lastWritebackRAMLabels->Add(ramByteLabel);
                            }
                        }
                    }

                    victimLine->setDirty(false);
                }

                victimLine->setCacheLineData(blockData);
                victimLine->setTag(tag);
                victimLine->setValid(true);
                victimLine->setAccessedLineCount(1);
                targetLine = victimLine;
            }
            else {
              
                targetLine->setCacheLineData(blockData);
                targetLine->setTag(tag);
                targetLine->setValid(true);
                targetLine->setAccessedLineCount(1);
            }

            int lineIdx = index * (int)cacheLines.size() + (targetLine - &cacheLines[0]);
            if (lineIdx < 0 || lineIdx >= cacheVisualizationScrollPanel->Controls->Count) {
                MessageBox::Show("Line index out of bounds.", "Error");
                return;
            }

            System::Windows::Forms::Panel^ linePanel = (System::Windows::Forms::Panel^)cacheVisualizationScrollPanel->Controls[lineIdx];
            for (int i = 1; i < linePanel->Controls->Count - 3; ++i) {
                System::Windows::Forms::Label^ blockLabel = (System::Windows::Forms::Label^)linePanel->Controls[i];
                int blockIndex = i - 1;
                blockLabel->BackColor = System::Drawing::Color::LightYellow;
                blockLabel->Text = "0x" + Convert::ToString(targetLine->getCacheLineData()[blockIndex], 16)->ToUpper();
            }

        }
        else {
            cache_hits++;
            textBox_cache_hits->Text = cache_hits.ToString();
        }

        // Acum avem targetLine cu datele, luam dataAtOffset
        unsigned char dataAtOffset = targetLine->getCacheLineData()[offset];

        int lineIdx2 = index * (int)cacheLines.size() + (targetLine - &cacheLines[0]);
        if (lineIdx2 < 0 || lineIdx2 >= cacheVisualizationScrollPanel->Controls->Count) {
            MessageBox::Show("Line index out of bounds.", "Error");
            return;
        }

        System::Windows::Forms::Panel^ linePanel2 = (System::Windows::Forms::Panel^)cacheVisualizationScrollPanel->Controls[lineIdx2];

        System::Windows::Forms::Label^ cacheByteLabel = nullptr;
        for (int i = 1; i < linePanel2->Controls->Count - 3; ++i) {
            Label^ blockLabel = (Label^)linePanel2->Controls[i];
            int blockIndex = i - 1;
            if (blockIndex == offset) {
                blockLabel->BackColor = System::Drawing::Color::LightGreen;
                blockLabel->Text = "0x" + Convert::ToString(dataAtOffset, 16)->ToUpper();
                cacheByteLabel = blockLabel; // memoram label-ul din cache pentru highlight
            }
        }

        // Actualizeaza valid/dirty/tag
        System::Windows::Forms::Label^ validLabel = static_cast<System::Windows::Forms::Label^>(
            linePanel2->Controls[linePanel2->Controls->Count - 3]);
        validLabel->Text = targetLine->isValid() ? "1" : "0";

        System::Windows::Forms::Label^ dirtyLabel = static_cast<System::Windows::Forms::Label^>(
            linePanel2->Controls[linePanel2->Controls->Count - 2]);
        dirtyLabel->Text = targetLine->isDirty() ? "1" : "0";

        System::Windows::Forms::Label^ tagLabel = static_cast<System::Windows::Forms::Label^>(
            linePanel2->Controls[linePanel2->Controls->Count - 1]);
        tagLabel->Text = Convert::ToString(targetLine->getTag());

        // Evidentiem in RAM byte-ul accesat
        int numBytesPerRow = 8;
        int ramRow = address / numBytesPerRow;
        int ramOff = address % numBytesPerRow;
        System::Windows::Forms::Label^ ramByteLabel = nullptr;
        if (ramRow >= 0 && ramRow < ramVisualizationScrollPanel->Controls->Count) {
            System::Windows::Forms::Panel^ ramLinePanel = (System::Windows::Forms::Panel^)ramVisualizationScrollPanel->Controls[ramRow];
            int dataControlIndexRAM = ramOff + 1;
            if (dataControlIndexRAM >= 0 && dataControlIndexRAM < ramLinePanel->Controls->Count) {
                ramByteLabel = (System::Windows::Forms::Label^)ramLinePanel->Controls[dataControlIndexRAM];
                ramByteLabel->BackColor = System::Drawing::Color::LightGreen;
            }
        }

        if (cacheByteLabel != nullptr) {
            lastHighlightedCacheByte = cacheByteLabel;
        }
        if (ramByteLabel != nullptr) {
            lastHighlightedRAMByte = ramByteLabel;
        }

        cache_hit_miss_ratio = (cache_hits + cache_misses > 0)
            ? (double)cache_hits / (cache_hits + cache_misses)
            : 0;
        textBox_cache_ratio->Text = cache_hit_miss_ratio.ToString("F2");

    }
    catch (std::exception& ex) {
        MessageBox::Show(gcnew System::String(ex.what()), "Error");
    }
    catch (System::Exception^ ex) {
        MessageBox::Show("An error occurred: " + ex->Message, "Error");
    }
}


void cachesim::Main::button_write_Click(System::Object^ sender, System::EventArgs^ e) {
    try {
        // Stergem highlight-uri anterioare
        if (lastHighlightedCacheByte != nullptr) {
            lastHighlightedCacheByte->BackColor = System::Drawing::Color::White;
            lastHighlightedCacheByte = nullptr;
        }
        if (lastHighlightedRAMByte != nullptr) {
            lastHighlightedRAMByte->BackColor = System::Drawing::Color::White;
            lastHighlightedRAMByte = nullptr;
        }
        for each (System::Windows::Forms::Label ^ lbl in lastWritebackRAMLabels) {
            lbl->BackColor = System::Drawing::Color::White;
        }
        lastWritebackRAMLabels->Clear();

        System::String^ addressText = textBox_address->Text;
        if (addressText->Length == 0) {
            MessageBox::Show("Please enter a valid hexadecimal address.", "Error");
            return;
        }

        std::string addressString = msclr::interop::marshal_as<std::string>(addressText);
        int address;
        {
            std::stringstream ss;
            ss << std::hex << addressString;
            ss >> address;
            if (ss.fail()) {
                MessageBox::Show("Invalid hexadecimal address.", "Error");
                return;
            }
        }

        System::String^ dataText = textBox_dataToWrite->Text;
        if (dataText->Length == 0) {
            MessageBox::Show("Please enter a valid hex byte to write.", "Error");
            return;
        }

        unsigned int byteToWrite;
        {
            std::stringstream ss;
            ss << std::hex << msclr::interop::marshal_as<std::string>(dataText);
            ss >> byteToWrite;
            if (ss.fail() || byteToWrite > 0xFF) {
                MessageBox::Show("Invalid byte. Must be between 00 and FF hex.", "Error");
                return;
            }
        }
        unsigned char byteVal = (unsigned char)byteToWrite;

        if (addressManager == nullptr) {
            MessageBox::Show("AddressManager not initialized.", "Error");
            return;
        }
        if (cache == nullptr) {
            MessageBox::Show("Cache not initialized.", "Error");
            return;
        }

        std::bitset<32> binaryAddress(address);
        std::string binaryAddressString = binaryAddress.to_string().substr(32 - addressWidth);
        addressManager->getTagIndexOffset_ofAddress(binaryAddressString);
        int tag = addressManager->getTag();
        int index = addressManager->getIndex();
        int offset = addressManager->getOffset();

        std::vector<CacheSet>& cacheSets = cache->getCacheSets();
        if (index < 0 || index >= (int)cacheSets.size()) {
            MessageBox::Show("Index out of bounds.", "Error");
            return;
        }

        CacheSet& cacheSet = cacheSets[index];
        std::vector<CacheLine>& cacheLines = cacheSet.getCacheLines();

        CacheLine* targetLine = nullptr;
        bool cacheHit = false;

        for (CacheLine& line : cacheLines) {
            if (line.isValid() && line.getTag() == tag) {
                cacheHit = true;
                targetLine = &line;
                targetLine->setAccessedLineCount(globalAccessCounter++);
                break;
            }
        }

        int blockSize = addressManager->getBlockSize();

        if (!cacheHit) {
            cache_misses++;
            textBox_cache_misses->Text = cache_misses.ToString();

            int RAM_block_toCopy_index = address / blockSize;
            int startIndex = RAM_block_toCopy_index * blockSize;
            const std::vector<unsigned char>& constRamMemory = ram->getMemory();

            if (startIndex < 0 || startIndex + blockSize >(int)constRamMemory.size()) {
                MessageBox::Show("Invalid RAM block index.", "Error");
                return;
            }

            std::vector<unsigned char> blockData;
            for (int i = 0; i < blockSize; i++) {
                blockData.push_back(constRamMemory[startIndex + i]);
            }

            targetLine = nullptr;
            for (CacheLine& line : cacheLines) {
                if (!line.isValid()) {
                    targetLine = &line;
                    break;
                }
            }

            if (targetLine == nullptr) {
                int victimIndex = replacementPolicy->replaceLineIndex(cacheLines);
                CacheLine* victimLine = &cacheLines[victimIndex];

                // Write-back daca victimLine are dirtyBit =1
                if (victimLine->isDirty()) {
                    int offsetWidth = addressManager->getOffsetWidth();
                    int indexWidth = addressManager->getIndexWidth();

                    int blockStartAddress = (victimLine->getTag() << (indexWidth + offsetWidth)) + (index << offsetWidth);

                    std::vector<unsigned char>& ramMemory = const_cast<std::vector<unsigned char>&>(ram->getMemoryRef());
                    std::vector<unsigned char>& victimData = victimLine->getCacheLineData();

                    for (int i = 0; i < blockSize; i++) {
                        ramMemory[blockStartAddress + i] = victimData[i];
                    }

                    int numBytesPerRow = 8;
                    for (int i = 0; i < blockSize; i++) {
                        int ramAddress = blockStartAddress + i;
                        int ramRow = ramAddress / numBytesPerRow;
                        int ramOff = ramAddress % numBytesPerRow;
                        if (ramRow >= 0 && ramRow < ramVisualizationScrollPanel->Controls->Count) {
                            System::Windows::Forms::Panel^ ramLinePanel = (System::Windows::Forms::Panel^)ramVisualizationScrollPanel->Controls[ramRow];
                            int dataControlIndexRAM = ramOff + 1;
                            if (dataControlIndexRAM >= 0 && dataControlIndexRAM < ramLinePanel->Controls->Count) {
                                System::Windows::Forms::Label^ ramByteLabel = (System::Windows::Forms::Label^)ramLinePanel->Controls[dataControlIndexRAM];
                                ramByteLabel->Text = "0x" + Convert::ToString(victimData[i], 16)->ToUpper();
                                ramByteLabel->BackColor = System::Drawing::Color::LightBlue;
                                lastWritebackRAMLabels->Add(ramByteLabel);
                            }
                        }
                    }

                    victimLine->setDirty(false);
                }

                victimLine->setCacheLineData(blockData);
                victimLine->setTag(tag);
                victimLine->setValid(true);
                victimLine->setAccessedLineCount(1);
                targetLine = victimLine;
            }
            else {
                targetLine->setCacheLineData(blockData);
                targetLine->setTag(tag);
                targetLine->setValid(true);
                targetLine->setAccessedLineCount(1);
            }

            int lineIdx = index * (int)cacheLines.size() + (targetLine - &cacheLines[0]);
            if (lineIdx < 0 || lineIdx >= cacheVisualizationScrollPanel->Controls->Count) {
                MessageBox::Show("Line index out of bounds during MISS.", "Error");
                return;
            }

            System::Windows::Forms::Panel^ linePanel = (System::Windows::Forms::Panel^)cacheVisualizationScrollPanel->Controls[lineIdx];
            for (int i = 1; i < linePanel->Controls->Count - 3; ++i) {
                System::Windows::Forms::Label^ blockLabel = (System::Windows::Forms::Label^)linePanel->Controls[i];
                int blockIndex = i - 1;
                blockLabel->BackColor = System::Drawing::Color::LightYellow;
                blockLabel->Text = "0x" + Convert::ToString(targetLine->getCacheLineData()[blockIndex], 16)->ToUpper();
            }

        }
        else {
            cache_hits++;
            textBox_cache_hits->Text = cache_hits.ToString();
        }

        // Acum scriem byte-ul in cache (Write Back)
        if (writePolicy->getPolicy() == 0) {
            if (offset < 0 || offset >= blockSize) {
                MessageBox::Show("Offset out of range.", "Error");
                return;
            }

            std::vector<unsigned char>& lineData = targetLine->getCacheLineData();
            lineData[offset] = byteVal;
            targetLine->setDirty(true);

            int lineIdx2 = index * (int)cacheLines.size() + (targetLine - &cacheLines[0]);
            if (lineIdx2 < 0 || lineIdx2 >= cacheVisualizationScrollPanel->Controls->Count) {
                MessageBox::Show("Line index out of bounds during WRITE.", "Error");
                return;
            }

            System::Windows::Forms::Panel^ linePanel2 = (System::Windows::Forms::Panel^)cacheVisualizationScrollPanel->Controls[lineIdx2];
            int dataControlIndex = offset + 1;
            if (dataControlIndex < 0 || dataControlIndex >= linePanel2->Controls->Count - 3) {
                MessageBox::Show("Data control index out of range.", "Error");
                return;
            }

            System::Windows::Forms::Label^ blockLabelToWrite = (System::Windows::Forms::Label^)linePanel2->Controls[dataControlIndex];
            blockLabelToWrite->Text = "0x" + Convert::ToString(byteVal, 16)->ToUpper();
            blockLabelToWrite->BackColor = System::Drawing::Color::LightGreen;
            lastHighlightedCacheByte = blockLabelToWrite;

            int dirtyIndex = (blockSize + 2);
            System::Windows::Forms::Label^ dLabel = (System::Windows::Forms::Label^)linePanel2->Controls[dirtyIndex];
            dLabel->Text = "1";

            // Evidentiem in RAM acelasi byte
            int numBytesPerRow = 8;
            int ramRow = address / numBytesPerRow;
            int ramOff = address % numBytesPerRow;
            if (ramRow >= 0 && ramRow < ramVisualizationScrollPanel->Controls->Count) {
                System::Windows::Forms::Panel^ ramLinePanel = (System::Windows::Forms::Panel^)ramVisualizationScrollPanel->Controls[ramRow];
                int dataControlIndexRAM = ramOff + 1;
                if (dataControlIndexRAM >= 0 && dataControlIndexRAM < ramLinePanel->Controls->Count) {
                    System::Windows::Forms::Label^ ramByteLabel = (System::Windows::Forms::Label^)ramLinePanel->Controls[dataControlIndexRAM];
                    ramByteLabel->BackColor = System::Drawing::Color::LightGreen;
                    lastHighlightedRAMByte = ramByteLabel;
                }
            }

            // Actualizeaza valid/dirty/tag
            System::Windows::Forms::Label^ validLabel = static_cast<System::Windows::Forms::Label^>(
                linePanel2->Controls[linePanel2->Controls->Count - 3]);
            validLabel->Text = targetLine->isValid() ? "1" : "0";

            System::Windows::Forms::Label^ dirtyLabel = static_cast<System::Windows::Forms::Label^>(
                linePanel2->Controls[linePanel2->Controls->Count - 2]);
            dirtyLabel->Text = targetLine->isDirty() ? "1" : "0";

            System::Windows::Forms::Label^ tagLabel = static_cast<System::Windows::Forms::Label^>(
                linePanel2->Controls[linePanel2->Controls->Count - 1]);
            tagLabel->Text = Convert::ToString(targetLine->getTag());

        }
        else {
            // WRITE THROUGH (policy == 1)
            int blockSize = addressManager->getBlockSize();
            if (offset < 0 || offset >= blockSize) {
                MessageBox::Show("Offset out of range.", "Error");
                return;
            }

            // Scriem in cache
            std::vector<unsigned char>& lineData = targetLine->getCacheLineData();
            lineData[offset] = byteVal;
            // NU setam dirty bit
            targetLine->setDirty(false);

            int lineIdx2 = index * (int)cacheLines.size() + (targetLine - &cacheLines[0]);
            if (lineIdx2 < 0 || lineIdx2 >= cacheVisualizationScrollPanel->Controls->Count) {
                MessageBox::Show("Error: Line index out of bounds during WRITE THOUGH.", "Error");
                return;
            }

            System::Windows::Forms::Panel^ linePanel2 = (System::Windows::Forms::Panel^)cacheVisualizationScrollPanel->Controls[lineIdx2];
            int dataControlIndex = offset + 1;
            if (dataControlIndex < 0 || dataControlIndex >= linePanel2->Controls->Count - 3) {
                MessageBox::Show("Data control index out of range (Write Through).", "Error");
                return;
            }

            System::Windows::Forms::Label^ blockLabelToWrite = (System::Windows::Forms::Label^)linePanel2->Controls[dataControlIndex];
            blockLabelToWrite->Text = "0x" + Convert::ToString(byteVal, 16)->ToUpper();
            blockLabelToWrite->BackColor = System::Drawing::Color::LightGreen;
            lastHighlightedCacheByte = blockLabelToWrite;

            // Scriem direct in RAM
            std::vector<unsigned char>& ramMemory = ram->getMemoryRef();
            ramMemory[address] = byteVal;

            int numBytesPerRow = 8;
            int ramRow = address / numBytesPerRow;
            int ramOff = address % numBytesPerRow;
            if (ramRow >= 0 && ramRow < ramVisualizationScrollPanel->Controls->Count) {
                System::Windows::Forms::Panel^ ramLinePanel = (System::Windows::Forms::Panel^)ramVisualizationScrollPanel->Controls[ramRow];
                int dataControlIndexRAM = ramOff + 1;
                if (dataControlIndexRAM >= 0 && dataControlIndexRAM < ramLinePanel->Controls->Count) {
                    System::Windows::Forms::Label^ ramByteLabel = (System::Windows::Forms::Label^)ramLinePanel->Controls[dataControlIndexRAM];
                    ramByteLabel->Text = "0x" + Convert::ToString(byteVal, 16)->ToUpper();
                    ramByteLabel->BackColor = System::Drawing::Color::LightBlue;
                    lastWritebackRAMLabels->Add(ramByteLabel);
                    lastHighlightedRAMByte = ramByteLabel;
                }
            }

           
            System::Windows::Forms::Label^ validLabel = static_cast<System::Windows::Forms::Label^>(
                linePanel2->Controls[linePanel2->Controls->Count - 3]);
            validLabel->Text = targetLine->isValid() ? "1" : "0";

            System::Windows::Forms::Label^ dirtyLabel = static_cast<System::Windows::Forms::Label^>(
                linePanel2->Controls[linePanel2->Controls->Count - 2]);
            dirtyLabel->Text = "0"; 

            System::Windows::Forms::Label^ tagLabel = static_cast<System::Windows::Forms::Label^>(
                linePanel2->Controls[linePanel2->Controls->Count - 1]);
            tagLabel->Text = Convert::ToString(targetLine->getTag());

            cache_hit_miss_ratio = (cache_hits + cache_misses > 0)
                ? (double)cache_hits / (cache_hits + cache_misses)
                : 0;
            textBox_cache_ratio->Text = cache_hit_miss_ratio.ToString("F2");
        }

        cache_hit_miss_ratio = (cache_hits + cache_misses > 0)
            ? (double)cache_hits / (cache_hits + cache_misses)
            : 0;
        textBox_cache_ratio->Text = cache_hit_miss_ratio.ToString("F2");

    }
    catch (std::exception& ex) {
        MessageBox::Show(gcnew System::String(ex.what()), "Error");
    }
    catch (System::Exception^ ex) {
        MessageBox::Show("An error occurred: " + ex->Message, "Error");
    }
}


int main(array<System::String^>^ args) {
    srand((unsigned)time(NULL));
    Application::EnableVisualStyles();
    Application::SetCompatibleTextRenderingDefault(false);
    cachesim::Main form;
    Application::Run(% form);
    return 0;
}