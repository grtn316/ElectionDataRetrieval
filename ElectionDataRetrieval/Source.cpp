#include <wx/wx.h>
#include <wx/grid.h>
#include <wx/choice.h>
#include <wx/button.h>
#include <wx/statbmp.h>
#include <chrono>
#include <thread>
#include "ElectionUtil.cpp"
#include "BTree.h"
#include "MaxHeap.h"

using namespace std;


class MyFrame : public wxFrame
{
private: 
    vector<string> candidates;
    unordered_map<string, vector<tuple <string, string, string, string, string>>> candidateData; //name > party, county, state, numVotes, percVotes
public:
    MyFrame() : wxFrame(nullptr, wxID_ANY, "Election Data Retrieval", wxPoint(30, 30), wxSize(928, 569))
    {

        //Load data
        ElectionUtil util = ElectionUtil();
        util.readElectionData();

        candidates = util.getCandidates();
        candidateData = util.getCandidateData();

        // Initialize all image handlers
        wxInitAllImageHandlers();       

        wxStaticText* dropDownLabel = new wxStaticText(this, wxID_ANY, "Search by Candidate Name:", wxPoint(281, 15), wxSize(257,33), wxALIGN_CENTER);

        wxChoice* dropDown = new wxChoice(this, wxID_ANY, wxPoint(281, 46), wxSize(257, 33));

        UpdateChoiceList(dropDown, candidates);

        dropDown->SetSelection(0);

        // Create the search button
        wxButton* searchBtn = new wxButton(this, wxID_ANY, "Search", wxPoint(572, 43), wxSize(110, 39));

        wxStaticText* gridLabel = new wxStaticText(this, wxID_ANY, "Top 10 Counties by Votes", wxPoint(258, 120), wxSize(567, 33), wxALIGN_CENTER);

        // Create the table with 4 columns
        wxGrid* grid = new wxGrid(this, wxID_ANY, wxPoint(258, 150), wxSize(600, 279));
        grid->CreateGrid(10, 4);
        //table->SetColLabelValue(0, "Row Position");
        grid->SetColLabelValue(0, "County");
        grid->SetColLabelValue(1, "State");
        grid->SetColLabelValue(2, "Votes");
        grid->SetColLabelValue(3, "% of Votes");

        //Adjust column width
        int tableWidth = grid->GetSize().GetWidth() - grid->GetRowLabelSize();
        int numCols = grid->GetNumberCols();
        int colWidth = tableWidth / numCols;
        for (int i = 0; i < numCols; i++) {
            grid->SetColSize(i, colWidth);
        }

        //table->AutoSizeColumns();

        
        wxStaticText* candidateLabel = new wxStaticText(this, wxID_ANY, "", wxPoint(31, 130), wxSize(194, 15), wxALIGN_CENTER);
        wxStaticText* partyLabel = new wxStaticText(this, wxID_ANY, "", wxPoint(31, 150), wxSize(194, 15), wxALIGN_CENTER);
        

        // Load and resize the image
        wxString imagePath = wxT("images/uncommitted.jpg");
        wxImage image(imagePath, wxBITMAP_TYPE_JPEG);
        wxSize size(194, 211);
        wxImage resizedImage = image.Rescale(size.GetWidth(), size.GetHeight());

        // Create the picture with the resized image
        wxBitmap bmp(resizedImage);
        wxStaticBitmap* candidatePicture = new wxStaticBitmap(this, wxID_ANY, bmp, wxPoint(31, 170), size);

        wxStaticText* retrievalLabel = new wxStaticText(this, wxID_ANY, "Time to Retrieve Data", wxPoint(31, 400), wxSize(194, 15), wxALIGN_CENTER);
        
        wxStaticText* bTreeLabel = new wxStaticText(this, wxID_ANY, "B Tree:", wxPoint(31, 430), wxSize(100, 15), wxALIGN_LEFT);
        wxStaticText* maxHeapLabel = new wxStaticText(this, wxID_ANY, "Max Heap:", wxPoint(31, 450), wxSize(100, 15), wxALIGN_LEFT);

        wxStaticText* bTreeResultLabel = new wxStaticText(this, wxID_ANY, "0.96 s", wxPoint(180, 430), wxSize(40, 15), wxALIGN_RIGHT);
        wxStaticText* maxHeapResultLabel = new wxStaticText(this, wxID_ANY, "0.96 s", wxPoint(180, 450), wxSize(40, 15), wxALIGN_RIGHT);

        /*EVENT HANDLERS*/
        searchBtn->Bind(wxEVT_BUTTON, [this, dropDown, grid, candidatePicture, bTreeResultLabel, maxHeapResultLabel, candidateLabel, partyLabel](wxCommandEvent& event) {
            OnSearchButtonClicked(event, dropDown, grid, candidatePicture, bTreeResultLabel, maxHeapResultLabel, candidateLabel, partyLabel);
            });

        //Trigger initial selection
        wxCommandEvent event(wxEVT_BUTTON, searchBtn->GetId());
        wxPostEvent(searchBtn, event);
    }

    void UpdateChoiceList(wxChoice* choice, const std::vector<std::string>& items) {
        // Clear the current items in the dropdown
        choice->Clear();
        
        // Add the new items to the dropdown
        for (const auto& item : items) {
            choice->Append(item);
        }

        // Set the selection to the first item in the list
        choice->SetSelection(0);
    }

    void UpdateImage(wxStaticBitmap* bitmap, string candidateName) {
        // Load and resize the image
        
        //Remove space from canidate name so that image is found
        candidateName.erase(remove(candidateName.begin(), candidateName.end(), ' '), candidateName.end());
        wxString imagePath = wxT("images/" + candidateName + ".jpg");
        wxImage image(imagePath, wxBITMAP_TYPE_JPEG);
        wxSize size(194, 211);
        wxImage resizedImage = image.Rescale(size.GetWidth(), size.GetHeight());

        bitmap->SetBitmap(resizedImage);
    }

    vector<tuple<string, string, int, float>> bTreeSearch(string candidateName) {
        //Election Data unordered map: candidateData
        //Candidate list: candidates

        vector <tuple <string, string, string, string, string>> data = candidateData[candidateName];
        BTree b(3);
        for (int i = 0; i < data.size(); i++)
        {
            // insert(county, state, (int)votes, (float)percVotes)
            b.Insert(get<1>(data[i]), get<2>(data[i]), stoi(get<3>(data[i])), stof(get<4>(data[i])));
        }

        vector<tuple<string, string, int, float>> gridData = b.TraverseTopTen();

        return gridData;
    }

    vector<tuple<string, string, int, float>> maxHeapSearch(string candidateName) {
        //Election Data unordered map: candidateData
        //Candidate list: candidates

        vector<tuple<string, string, int, float>> gridData;

        vector<tuple <string, string, string, string, string>> data = candidateData[candidateName];
        MaxHeap mh = MaxHeap(5000);
        for (tuple <string, string, string, string, string> t : data)
        {
            mh.insert(get<1>(t), get<2>(t), stoi(get<3>(t)), stof(get<4>(t)));
        }
        for (int i = 0; i < 10; ++i)
        {
            gridData.push_back(make_tuple(mh.getMaxCounty(), mh.getMaxState(), mh.getMaxVotes(), mh.getMaxPercent()));
            mh.popMax();
        }

        return gridData;
    }

    void UpdateGrid(wxGrid* grid, string candidateName, wxStaticText* bTreeResultLabel, wxStaticText* maxHeapResultLabel) {
        vector<tuple<string, string, int, float>> gridData;
        //Go fetch data to display in table

        auto start = std::chrono::high_resolution_clock::now();
        gridData = bTreeSearch(candidateName); // Replace with the name of the function you want to time
        auto end = std::chrono::high_resolution_clock::now();

        auto bTreeduration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() / 1000.0;

        start = std::chrono::high_resolution_clock::now();
        gridData = maxHeapSearch(candidateName); // Replace with the name of the function you want to time
        end = std::chrono::high_resolution_clock::now();

        auto maxHeapduration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() / 1000.0;

        //Update grid
        grid->ClearGrid();
        for (int i = 0; i < 10; i++) {
            tuple<string, string, int, float> tup = gridData[i];

            grid->SetCellValue(i, 0, get<0>(tup));
            grid->SetCellValue(i, 1, get<1>(tup));
            grid->SetCellValue(i, 2, to_string(get<2>(tup)));
            grid->SetCellValue(i, 3, to_string(get<3>(tup)));
        }

        bTreeResultLabel->SetLabel(std::to_string(std::stod(to_string(bTreeduration).substr(0, to_string(bTreeduration).find(".") + 5))) + " s"); //converts to 4 decimal places
        maxHeapResultLabel->SetLabel(std::to_string(std::stod(to_string(maxHeapduration).substr(0, to_string(maxHeapduration).find(".") + 5))) + " s"); //converts to 4 decimal places

    }

    void OnSearchButtonClicked(wxCommandEvent& event, wxChoice* choice, wxGrid* grid, wxStaticBitmap* bitmap, wxStaticText* bTreeResultLabel, wxStaticText* maxHeapResultLabel, wxStaticText* candidateLabel, wxStaticText* partyLabel) {
        // Get the current selection from the dropdown
        wxString selection = choice->GetStringSelection();
        //int selection = choice->GetSelection();

        //Update Grid with selected candidate vote data
        UpdateGrid(grid, selection.ToStdString(), bTreeResultLabel, maxHeapResultLabel);

        candidateLabel->SetLabel(selection.ToStdString());

        // Access the first tuple in the vector for the supplied candidate
        tuple<string, string, string, string, string> firstTuple = candidateData[selection.ToStdString()].at(0);

        partyLabel->SetLabel(get<0>(firstTuple));

        //Update selected candidate image
        UpdateImage(bitmap, selection.ToStdString());

        grid->SetColLabelValue(3, "% of " + get<0>(firstTuple) +" Votes");
    }

};

class MyApp : public wxApp
{
public:
    virtual bool OnInit()
    {     
        MyFrame* frame = new MyFrame();
        frame->Show();
        return true;
    }
};

wxIMPLEMENT_APP(MyApp);
