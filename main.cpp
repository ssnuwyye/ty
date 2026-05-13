#include "MainForm.h"
#include "DatabaseManager.h"
#include <Windows.h>
#include <locale>

using namespace System;
using namespace System::Windows::Forms;

[STAThread]
int main()
{
    std::locale::global(std::locale(""));

    Application::EnableVisualStyles();
    Application::SetCompatibleTextRenderingDefault(false);

    std::string connString = "host=localhost port=5432 dbname=tournament_manager user=postgres password=12345";

    DatabaseManager db(connString);

    if (!db.connect())
    {
        MessageBox::Show(L"Cannot connect to database!\nPlease check your PostgreSQL connection.",
            L"Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
        return 1;
    }

    Application::Run(gcnew MainForm(&db));

    return 0;
}