// COMSC-210 | Lab 30 | Diksha Tara Natesan
// IDE used: Vim/Terminal

//Include necessary headers for file handling, data structures, etc.
#include <iostream>
#include <fstream>
#include <map>
#include <array>
#include <list>
#include <string>
using namespace std;

//index 0: menu items, index 1: customers, index 2: profits
using BAKERY = array<list<string>, 3>;

//Define a function to simulate renovations (15% likelihood)
//Parameters: key-value pair of a bakery location
void renovation(pair<const string, BAKERY> &bakery);

//Define a function to simulate a rival bakery opening/closing (30% likelihood)
//Parameters: key-value pair of a bakery location
void rival(pair<const string, BAKERY> &bakery);

//Define a function to simulate an ingredient being out of stock (25% likelihood)
//Parameters: key-value pair of a bakery location
void outOfStock(pair<const string, BAKERY> &bakery);

//Define a function to simulate an ingredient that was out of stock last quarter being back in stock
//Parameters: key-value pair of a bakery location
void backInStock(pair<const string, BAKERY> &bakery);

//Define a function that chooses one of the events to occur - with somewhat realistic likelihoods of any event occuring.
//Parameters: map of bakery locations
void event(map<string, array<list<string>, 3>> &bakeries);

//Define a function that displays all the map information
//Parameters: map of bakery locations, int specifying the quarter it is printing
void display(map<string, array<list<string>, 3>> &bakeries, int qtr);

//Define main function
int main(){
	cout << "\n\t*** Tara's Bakes Simulation ***\n\n";

	//Initialize a map to store bakery location information each associated with an array of lists for number of customers per week of the quarter, menu items for the quarter, and the profits of each week in the quarter
	map<string, BAKERY> bakeries;
	//populate w empty locations
	bakeries["SFO"] = BAKERY{};
	bakeries["CHI"] = BAKERY{};
	bakeries["NYC"] = BAKERY{};
	bakeries["PHL"] = BAKERY{};

	//Open an external file to read initial data about bakery locations and populate the map
	ifstream fin("bakerydata.txt");
	//If file does not open, print an error and exit
	if (!fin) {
		cerr << "Error: could not open bakerydata.txt\n";
		return 1;
	}
	string city;
	string item;
	string customers;
	string profit;
	//Read data from file and populate map
	//For each line, extract bakery location and its corresponding categorical data
        //Insert data into the appropriate category's list
        //Close the file
	while (fin >> city >> item >> customers >> profit) {
		bakeries[city][0].push_back(item);
		bakeries[city][1].push_back(customers);
		bakeries[city][2].push_back(profit);
	}
	fin.close();
	int qtr = 0;
	display(bakeries, qtr);
	//Begin a time-based simulation for quarterly changes
	for (int i = 0; i < 28; i++){
	//For 28 yearly quarters as the time intervals (7 year projection)
		qtr += 1;
	//Iterate through each bakery location in the map
	//For each location, simulate changes
	//25% chance of a menu item being removed for a quarter - ensure it gets brought back for the next quarter along with sales returning to normal
			event(bakeries);
	//Print the changes for this quarter, "{menu item} out of stock decreased {customers and/or profits} in {bakery location}"
	//Call the function that chooses an event to occur
	//The events can impact multiple locations or one location
	//Wait briefly to simulate the passage of time between quarters
		display(bakeries, qtr);
	}
	cout << "\n*** End of Tara's Bakes Simulation (wireframe) ***\n";
	//End of main function
}

//dummy functions
void renovation(pair<const string, BAKERY> &bakery){
	cout << "[renovation()] Renovations for Tara's Bakes at {city} location\n";
}
void rival(pair<const string, BAKERY> &bakery){
	list<string> &cust = bakery.second[1];
	list<string> updated;
	string holder;
	
	for (string c : cust) {
        	int customer = stoi(c);
        	customer = (customer * 0.75);  		//decrease by 25%
		holder = customer + "";
		updated.push_back(holder);
    	}
	
	cout << "[rival()] Rival bakery opens in " << bakery.first << endl;
}
void outOfStock(pair<const string, BAKERY> &bakery){

	int prob = rand() % bakery.second[0].size();
	string item;
	int count = -1;	
	for (string i : bakery.second[0])
		count += 1;
		if (count == prob){
			item = i;
			break;
		}
	}
	bakery.second[0].remove(item);
 
	cout << item << " is unavailable at Tara's Bakes in " << bakery.first << " location\n";
}
void backInStock(pair<const string, BAKERY> &bakery){
	cout << "[backInStock()] {menu item} is now restocked at Tara's Bakes in {city} location\n";
}
void event(map<string, array<list<string>, 3>> &bakeries){
	bool event = false;
	int prob = rand() % 100 + 1;  // returns random number 1-100
	for (auto &pair : bakeries){
		if (prob <= 25){
			outOfStock(pair);
			event = true;
		}
		prob = rand() % 100 + 1;
		if (prob <= 15){
			renovation(pair);
       	 		event = true;
        	}
		prob = rand() % 100 + 1;
	       	if (prob <= 30){
			rival(pair);
        	        event = true;
	        }
	}

	if (!event)
		cout << "\nNo significant events occurred this quarter.\n";
}

void display(map<string, array<list<string>, 3>> &bakeries, int qtr){
	
	if (qtr != 0){
		int year = ((qtr-1)/4) + 1;
		qtr %=4;
		cout << "Q" << qtr << " Bakery Data:\n";
	}else
		cout << "Starting Bakery Data:\n";
	for(const auto &[city, data]:bakeries){
		cout << '\t' << city << " Location:\n";
		cout << "\tMenu Items\t\tCustomers\t\tProfits\n";
		//loop here so each list gets fully printed
		auto itItem = data[0].begin();
       		auto itCust = data[1].begin();
        	auto itProf = data[2].begin();
		
		while (itItem != data[0].end()){
			cout << '\t' << *itItem << "\t\t\t" << *itCust << "\t\t\t" << *itProf << endl;
			++itItem;
            		++itCust;
            		++itProf;
		}
		cout << '\n';
	}

}

