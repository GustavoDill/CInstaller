#include <wx/wx.h>
#include "mainw.h"
#include <filesystem>


void app_main(int argc, wxCmdLineArgsArray& argv) {
	wxInitAllImageHandlers();
	mainw* main = new mainw("CZipW", wxSize(1000, 720));
	main->Show();
	if (argc > 1) {
		if (std::filesystem::exists(argv[1].c_str().AsChar())) {
			main->OpenFile(argv[1].c_str().AsChar());
		}
	}

}

class App : public wxApp {
public:
	bool OnInit() override {

		app_main(argc, argv);

		return true;
	}
};

wxIMPLEMENT_APP(App);