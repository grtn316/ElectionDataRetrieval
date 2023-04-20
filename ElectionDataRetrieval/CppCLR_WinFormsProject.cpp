#include "pch.h"
#include <unordered_map>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include<tuple>
#include <string>

using namespace System;
using namespace std;

// int main(array<System::String ^> ^args)
// {
//    return 0;
// }

#include "Form1.h"

using namespace System::Windows::Forms;

class ElectionUtil {
private:
	vector<vector<string>> data; //raw CSV data
	unordered_map<string, vector<tuple <string, string, string, string, string>>> candidateData; //name > party, county, state, numVotes, percVotes
	vector<string> candidates; //list of candidates that can be used to pass into map
public:
	void readElectionData() {
		ifstream file("data/election.csv");

		string line, field;
		while (getline(file, line)) {
			vector<string> row;
			stringstream ss(line);

			while (getline(ss, field, ',')) {

				// Handle quoted fields
				if (field.front() == '"' && field.back() == '"') {
					field = field.substr(1, field.size() - 2);
				}

				row.push_back(field);
			}

			data.push_back(row);
		}

		//Row 1 = Headers cols 4,5,6 split for candidate name

		//Col 1 = County
		//Col 2 = State
		//Col 3 = State Abv

		//Col 4 = Num Votes
		//Col 5 = Party
		//Col 6 = Percent Votes

		int countyPos = 0;
		int statePos = 1;
		//int numVotesPos = 3;
		//int partyPos = 4;
		//int percentVotesPos = 5;


		for (int y = 3; y < data[0].size(); y += 3) { //y == candidate start column
			vector<tuple <string, string, string, string, string>> votes;
			string candidateName = data[0][y];

			//get candidate name from row 1 by splitting numVotesPos field.
			int pos1 = candidateName.find('.');
			int pos2 = candidateName.find('.', pos1 + 1);
			candidateName = candidateName.substr(pos1 + 1, pos2 - pos1 - 1);

			for (int x = 1; x < data.size(); x++) { //x == rowPos

				//get candidate details for a county
				tuple <string, string, string, string, string> tup(data[x][y + 1], data[x][countyPos], data[x][statePos], data[x][y], data[x][y + 2]);
				votes.push_back(tup);
			}

			candidates.push_back(candidateName);
			candidateData[candidateName] = votes;

		}

	}

};

[STAThread]
int main()
{

	//Code will execute before UI is loaded and waits for user interaction

	//load data
	ElectionUtil election;
	election.readElectionData();

	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	Application::Run(gcnew CppCLRWinFormsProject::Form1());

	//code does not execute here; Any code that needs to respond to UI interaction should be handled via an event.

	return 0;
}