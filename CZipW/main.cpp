#include <wx/wx.h>
#include "mainw.h"


void app_main() {
	wxInitAllImageHandlers();
	mainw* main = new mainw("CZipW", wxSize(1000, 720));
	main->Show();

}

class App : public wxApp {
public:
	bool OnInit() override {

		app_main();

		return true;
	}
};

wxIMPLEMENT_APP(App);