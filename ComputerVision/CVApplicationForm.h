#pragma once

#include "ComputerVisionProcessor.h"
#include "Helper.h"

namespace ComputerVision {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

    ComputerVisionProcessor* processor;

	public ref class CVApplicationForm : public System::Windows::Forms::Form
	{
	public:
        Bitmap^ image;

    private: System::Windows::Forms::PictureBox^  imageOutput;
    public:

    public:
        String^ filename;

		CVApplicationForm(void)
		{
			InitializeComponent();
		}

	protected:
		~CVApplicationForm()
		{
			if (components)
			{
				delete components;
			}
		}
    private: System::Windows::Forms::MenuStrip^  menuStrip1;
    protected:
    private: System::Windows::Forms::ToolStripMenuItem^  fileToolStripMenuItem;
    private: System::Windows::Forms::ToolStripMenuItem^  openToolStripMenuItem;
    private: System::Windows::Forms::ToolStripMenuItem^  effectsToolStripMenuItem;
    private: System::Windows::Forms::ToolStripMenuItem^  grayscaleToolStripMenuItem;
    private: System::Windows::Forms::ToolStripMenuItem^  cannyToolStripMenuItem;

	private:
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		void InitializeComponent(void)
		{
            this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
            this->fileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->openToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->effectsToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->grayscaleToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->cannyToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->imageOutput = (gcnew System::Windows::Forms::PictureBox());
            this->menuStrip1->SuspendLayout();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->imageOutput))->BeginInit();
            this->SuspendLayout();
            // 
            // menuStrip1
            // 
            this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {
                this->fileToolStripMenuItem,
                    this->effectsToolStripMenuItem
            });
            this->menuStrip1->Location = System::Drawing::Point(0, 0);
            this->menuStrip1->Name = L"menuStrip1";
            this->menuStrip1->Size = System::Drawing::Size(750, 24);
            this->menuStrip1->TabIndex = 0;
            this->menuStrip1->Text = L"menuStrip1";
            // 
            // fileToolStripMenuItem
            // 
            this->fileToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) { this->openToolStripMenuItem });
            this->fileToolStripMenuItem->Name = L"fileToolStripMenuItem";
            this->fileToolStripMenuItem->Size = System::Drawing::Size(37, 20);
            this->fileToolStripMenuItem->Text = L"File";
            // 
            // openToolStripMenuItem
            // 
            this->openToolStripMenuItem->Name = L"openToolStripMenuItem";
            this->openToolStripMenuItem->Size = System::Drawing::Size(103, 22);
            this->openToolStripMenuItem->Text = L"Open";
            this->openToolStripMenuItem->Click += gcnew System::EventHandler(this, &CVApplicationForm::openToolStripMenuItem_Click);
            // 
            // effectsToolStripMenuItem
            // 
            this->effectsToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {
                this->grayscaleToolStripMenuItem,
                    this->cannyToolStripMenuItem
            });
            this->effectsToolStripMenuItem->Name = L"effectsToolStripMenuItem";
            this->effectsToolStripMenuItem->Size = System::Drawing::Size(54, 20);
            this->effectsToolStripMenuItem->Text = L"Effects";
            // 
            // grayscaleToolStripMenuItem
            // 
            this->grayscaleToolStripMenuItem->Name = L"grayscaleToolStripMenuItem";
            this->grayscaleToolStripMenuItem->Size = System::Drawing::Size(152, 22);
            this->grayscaleToolStripMenuItem->Text = L"Grayscale";
            this->grayscaleToolStripMenuItem->Click += gcnew System::EventHandler(this, &CVApplicationForm::grayscaleToolStripMenuItem_Click);
            // 
            // cannyToolStripMenuItem
            // 
            this->cannyToolStripMenuItem->Name = L"cannyToolStripMenuItem";
            this->cannyToolStripMenuItem->Size = System::Drawing::Size(152, 22);
            this->cannyToolStripMenuItem->Text = L"Canny";
            this->cannyToolStripMenuItem->Click += gcnew System::EventHandler(this, &CVApplicationForm::cannyToolStripMenuItem_Click);
            // 
            // imageOutput
            // 
            this->imageOutput->Location = System::Drawing::Point(13, 28);
            this->imageOutput->Name = L"imageOutput";
            this->imageOutput->Size = System::Drawing::Size(725, 435);
            this->imageOutput->TabIndex = 1;
            this->imageOutput->TabStop = false;
            // 
            // CVApplicationForm
            // 
            this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = System::Drawing::Size(750, 475);
            this->Controls->Add(this->imageOutput);
            this->Controls->Add(this->menuStrip1);
            this->MainMenuStrip = this->menuStrip1;
            this->Name = L"CVApplicationForm";
            this->Text = L"CVApplicationForm";
            this->Load += gcnew System::EventHandler(this, &CVApplicationForm::CVApplicationForm_Load);
            this->menuStrip1->ResumeLayout(false);
            this->menuStrip1->PerformLayout();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->imageOutput))->EndInit();
            this->ResumeLayout(false);
            this->PerformLayout();

        }
#pragma endregion
    private: System::Void CVApplicationForm_Load(System::Object^  sender, System::EventArgs^  e) {
        image = gcnew Bitmap(1, 1);
        imageOutput->Image = gcnew Bitmap(image);
        imageOutput->Refresh();
    }
    private: System::Void openToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
        OpenFileDialog^ dialog = gcnew OpenFileDialog();
        dialog->Filter = "Image Files | *.bmp; *.jpg; *.png; All Files | (*.*) | *.*";

        if (dialog->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
            image = gcnew Bitmap(dialog->FileName);
            filename = dialog->FileName;
            //SetWindowSize();
            processor = new ComputerVisionProcessor(SystemToStl(filename));
            imageOutput->Image = gcnew Bitmap(image);
            imageOutput->Refresh();
        }
    }
private: System::Void grayscaleToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
    
}
private: System::Void cannyToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {

}
};
}
