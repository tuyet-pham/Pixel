/*
*	PROJECT 3 - 2100
*   Group : Tuyet Pham
*	Pixel project. 
*/

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <utility>
#include <set>
#include <iomanip>
using namespace std;


//Functions - More info below
string menu();
bool fileReader(fstream &, string);
vector<string> makeRow(string);
void displayPass(vector<vector<string>> & chart, int);
void change(vector<vector<string>> & chart);


int main() {

	//Make file,  
	fstream FILE;
	cout << endl;

	bool done = false;

	do {
		string doneAnswer;
		string answer = menu();						//Get answer from user
		fileReader(FILE, answer);					//Tossing both to fileReader
		


		cout << "\nWant to try another file ?(Y/N) : ";			//If user wants to continues
		cin >> doneAnswer;


		if (doneAnswer == "N" || doneAnswer == "n") {
			done = true;
		}
		else {
			FILE.close();								//Close file to be ready to read another file.
			cout << "Ok! I will continue. . . .\n";
		}

	}while(done == false);

	cout << "\n\n Thanks! Bye!\n";
	return 0;
}



/*
* _func_ : menu
* return : string
* task : Get the user's string for .txt file
*/
string menu() {

	string answer;

	cout << "\nPlease enter your filename with .txt at the end" << endl;
	cout << "> ";
	cin >> answer;
	return answer;
}




/*
* _func_ : fileReader
* param : fstream & , string
* return : bool
* task : Read in the file, and make the chart out of it & then toss the chart to the change funtion.
*/
bool fileReader(fstream & file, string filename)
{
	vector<vector<string>> chart;			//2D vector for the chart
	string temp;							//Temp string for each line of the chart

	file.open(filename);					//Return if doesn't open
	if (file.is_open() == false) {
		return false;
	}

	//Make each line into a row with makeRow() and push that row into chart vector<vector<string>>
	while (getline(file, temp)) {
		vector<string> row = makeRow(temp);
		chart.push_back(row);
	}

	//Toss the chart to be changed in change();
	change(chart);

	return true;
}




/*
* _func_ : makeRow
* param : string
* return : vector<string>
* task : This function will take in a string, that is the line of the file and chop it up by delimination of ',' 
*	It will then return a 1D vector
*/
vector<string> makeRow(string line)
{
	vector<string> row;					//Vector to return back
	istringstream stringd(line);		//To be able to use getline
	string temp;

	while (getline(stringd, temp, ',')) {		//Each line now will be each pixel
		
		if (temp == "1") {						//If the pixel is a one, change it to an x for easier config later.
			temp = "x";
		}
		row.push_back(temp);					//Pushing each pixel into vector
	}

	return row;
}




/*
* _func_ : change()
* param : vector<vector<string>> & chart
* task : It will change the chart given and then display it by calling the display funciton.
*/
void change(vector<vector<string>> & chart) {

	string numSection[104];						//My array of strings 'numbers'
	string * f = numSection;					//This will basically be an iterator to iterate through the array of string above as each has been used.
	int amount = 0;								//Hold the object number
	bool done = false;							
	set <pair<string, string>> connections;		//This holds the relationships between the connected numbers. the smaller is the key or the .first
												//This is used in the Second pass.

	//This loop makes the object counts string.
	for (int i = 0; i < 100; i++) {
		stringstream temp;
		temp << i + 1;
		numSection[i] = temp.str();
	}

	

	cout << "\n\nFirst Pass : \n";										
	for (int i = 0; i < chart.size(); i++) {			//FIRST PASS - changing the chart initially 
		for (int h = 0; h < chart[0].size(); h++) {
			
			//First row
			if (i == 0) {
				//First col
				if (h == 0) {
					if (chart[i][h] == "x") {			//If first pixel is x then set it as the first string in the vector
						chart[i][h] = (*f);	
						f++;							//Point to the next address/index of the array
					}
				}
				else {

					//Other col of first row
					if (chart[i][h] == "x"){					//If this pixel is and x 
						
						if (chart[i][h - 1] != "0") {			//If the pixel to the left is not 0 then current pixel = left pixel
							chart[i][h] = chart[i][h - 1];
						}
						else {						
							chart[i][h] = (*f);					//If the left is a zero then give it the value of the current pointer's position.
							f++;								//Point to the next address/index of the array	
						}
					}
				}
			}
			else if( i != 0) {				//Every other row	
				//first col of eveyother row
				if (h == 0) {
					if (chart[i][h] == "x") {
						if (chart[i - 1][h] != "0") {
							chart[i][h] = chart[i - 1][h];
						}
						else {
							chart[i][h] = (*f);
							f++;
						}
					}
				}
				else if (h != 0) {				//Every other col of every other row
					
					if (chart[i][h] == "x") {

						// If the 2 position is not zero . AKA if it has an object number
						if (chart[i][h - 1] != "0" && chart[i - 1][h] != "0") {
							
							//If they are the same
							if (chart[i][h - 1] == chart[i - 1][h]) {
								chart[i][h] = chart[i][h - 1];
							}

							//If they arent the same
							if (chart[i][h - 1] != chart[i - 1][h]) {
								//See which one is smaller
								if (stoi(chart[i][h - 1]) < stoi(chart[i - 1][h])) {
									chart[i][h] = chart[i][h - 1];

									//Make the connection
									auto temp = std::make_pair(chart[i][h - 1], chart[i - 1][h]);
									connections.insert(temp);

								}
								else {
									chart[i][h] = chart[i - 1][h];
									//Make the connection
									auto temp = std::make_pair(chart[i - 1][h], chart[i][h - 1]);
									connections.insert(temp);
								}
							}
						}
						else {			//If the top and left is not the same

							//If the left is not 0
							if (chart[i][h - 1] != "0") {
								chart[i][h] = chart[i][h - 1];
							}
							else if (chart[i][h - 1] == "0") {				//If the left is 0

								//check the top 
								if (chart[i - 1][h] == "0") {
									chart[i][h] = (*f);
									f++;
								}
								else {
									chart[i][h] = chart[i - 1][h];			//If the top is not zero.
								}
							}
						}
					}
				}
			}	
		}
	}


	amount = stoi(*f) - 1;								//Current position of the array minus one. 
	displayPass(chart, amount);							//Display the chart 


	cout << "\n\nSecond Pass : \n";
	while (done == false) {

		for (int i = 0; i < chart.size(); i++) {			//SECOND PASS - changing the connected numbers
			for (int j = 0; j < chart[0].size(); j++) {
				for (auto h : connections) {				//This is going through the connections sets and seeing if the current posiiton is 
															//equal to the .second value of the set then change it to the .first value. ( which is the smallest value ) 
					if (chart[i][j] == h.second) {
						chart[i][j] = h.first;
					}
				}
			}
		}

		done = true;

		//If there are any 'larger' values then change the graph until there are no more larger values.
		for (int i = 0; i < chart.size(); i++) {			
			for (int j = 0; j < chart[0].size(); j++) {
				for (auto h : connections) {					
					if (chart[i][j] == h.second) {
						done = false;
					}
				}
			}
		}
	}

	amount = amount - connections.size();				//Current position of the array minus the amount of connections. b/c we are getting rid of x amount of numbers.
	displayPass(chart, amount);							//Display the chart 
	
}




/*
* _func_ : displayPass
* param : vector<vector<string>> &, int
* task : Displays the changed chart and output the number of objects and see which one is the largest
*/
void displayPass(vector<vector<string>> & chart, int amount)
{

	for (int i = 0; i < chart.size(); i++) {
		for (int h = 0; h < chart[0].size(); h++) {
			cout << left << setw(4) << chart[i][h];
		}
		cout << endl;
	}

	cout << "\nNumber of Objects: " << amount << endl;
}