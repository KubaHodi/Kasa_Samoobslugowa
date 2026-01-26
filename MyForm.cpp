#include "form.h"
#include "author_form.h"

using namespace System;
using namespace System::Windows::Forms;
void main(array<String^>^ args) {
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	app::MyForm form;
	Application::Run(% form);
}