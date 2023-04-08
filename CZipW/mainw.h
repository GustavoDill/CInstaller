#pragma once
#include <wx/wx.h>
#include <wx/listctrl.h>
#include "../CZipLib/czip.h"
#include "../CZipLib/czip_explorer.h"

class mainw : public wxFrame {

	wxImageList* img;
	wxListCtrl* list;
	czip czip;
	czip_explorer explorer;
	void list_sizec(wxSizeEvent& evt){ list->SetColumnWidth(0, wxLIST_AUTOSIZE_USEHEADER); evt.Skip(); }

	void OnOpen(wxCommandEvent& event) {
		wxFileDialog openFileDialog(this, _("Open file"), "", "", "CZip files (*.czip)|*.czip", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
		if (openFileDialog.ShowModal() == wxID_CANCEL)
			return;
		OpenFile(openFileDialog.GetPath().c_str());
	}
	void OnClose(wxCommandEvent& event) {
		CloseFile();
	}


	

	void OnRightClick(wxListEvent& event) {
		PopupMenu(contextMenu);
	}

	void OnExtractFile(wxCommandEvent& listItem) {
		// Your code to extract the file here
		long selItem = -1;
		selItem = list->GetNextItem(selItem, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
		if( selItem != -1)
			ExtractFile(selItem);
	}

	wxMenu* contextMenu;
public:
	mainw(const wxString& title, const wxSize size)
		: wxFrame(nullptr, -1, title, wxDefaultPosition, size)
		, czip(MODE_DECOMPRESS)
	{
		explorer.set_ref(&czip);
		wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
		list = new wxListCtrl(this, -1, wxPoint(0, 0), wxDefaultSize, wxLC_REPORT | wxLC_NO_HEADER);
		 img = new wxImageList(16, 16);
		wxBitmap folder;
		if (folder.LoadFile("M:\\linux-backup\\software\\ANGRYsearch-1.0.1\\resources\\mimeicons\\numix\\archive.png", wxBITMAP_TYPE_PNG) == false) {
			throw "Error loading image.";
		};
		wxImage image = folder.ConvertToImage();
		image.Rescale(16, 16);
		img->Add(wxBitmap(image));
		//img->Add(folder);
		list->AssignImageList(img, wxIMAGE_LIST_SMALL);
		Bind(wxEVT_SIZE, &mainw::list_sizec, this);
		list->AppendColumn("Name", wxLIST_FORMAT_CENTER);
		sizer->Add(list, 1, wxEXPAND);
		SetSizer(sizer);

		// Create the menu bar
		wxMenuBar* menuBar = new wxMenuBar;
		wxMenu* fileMenu = new wxMenu;
		fileMenu->Append(wxID_OPEN, "&Open\tCtrl-O", "Open a file");
		fileMenu->Append(wxID_CLOSE, "&Close\tCtrl-F", "Close the file");
		menuBar->Append(fileMenu, "&File");
		SetMenuBar(menuBar);

		// Bind the Open event
		Bind(wxEVT_MENU, &mainw::OnOpen, this, wxID_OPEN);
		Bind(wxEVT_MENU, &mainw::OnClose, this, wxID_CLOSE);


		contextMenu = new wxMenu;
		contextMenu->Append(wxID_ANY, wxT("Extract"), wxT("Extract file"));
		list->Connect(wxEVT_COMMAND_LIST_ITEM_RIGHT_CLICK, wxListEventHandler(mainw::OnRightClick), nullptr, this);

		Bind(wxEVT_MENU, &mainw::OnExtractFile, this, contextMenu->FindItemByPosition(0)->GetId());
	}
	void AddListItem(const wxString& item, int index = -1) {
		list->InsertItem((index == -1 ? list->GetItemCount() : index), item, 0);
	}
	void RemoveListItem(const int index) {
		list->DeleteItem(index);
	}
	void ExtractFile(const int fileIndex) {
		std::filesystem::path file_path(explorer[fileIndex].file);
		std::string ext = file_path.extension().string();
		std::string filter = (const char*)&ext.data()[1];
		filter += " files (*";
		filter += ext + ")|*" + ext;
		wxString file = SaveFile(filter.c_str(), explorer[fileIndex].file);
		if (file.empty()) return;

		if (explorer.extract(fileIndex, file.c_str()))
			wxMessageBox("File extracted!", "Extract file", wxICON_INFORMATION);
		else
			wxMessageBox("Failed", "Extract file", wxICON_ERROR);
	}
	wxString SaveFile(const char* filter, const char* defaultName = "") {
		wxFileDialog saveFileDialog(this, _("Save File"), "", defaultName,
									filter, wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
		if (saveFileDialog.ShowModal() == wxID_CANCEL)
			return wxEmptyString;
		return saveFileDialog.GetPath();
	}

	void ListFiles() {
		for (int i = 0; i < explorer.entries(); i++) {

			std::string item; item.reserve(explorer[i].header.name_length + explorer[i].header.path_length + 2);
			item += "\\";
			if (explorer[i].path != nullptr) {
				item += explorer[i].path;
				item += "\\";
			}
			item += explorer[i].file;
			AddListItem(item);


		}
	}
	void OpenFile(const char* path) {
		czip.open(path);
		explorer.update();
		ListFiles();
		SetTitle("CZipW - " + std::filesystem::path(path).filename().string());
	}
	void CloseFile() {
		czip.close();
		list->ClearAll();
	}

	bool Show(bool show = true) override {
		bool b = wxFrame::Show();
		Center();
		list->SetColumnWidth(0, wxLIST_AUTOSIZE_USEHEADER);
		return b;
	}
};

