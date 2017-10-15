#include "CVApplication.h"
#include "CVApplicationForm.h"

using namespace System;
using namespace System::Windows::Forms;

[STAThreadAttribute]

void main() {
    Application::EnableVisualStyles();
    Application::SetCompatibleTextRenderingDefault(false);
    ComputerVision::CVApplicationForm form;
    Application::Run(%form);
}