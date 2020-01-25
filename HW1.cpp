#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include <sstream>
#include <list>
#include <istream>
#include <fstream>
#include <set>
#include <map>
using namespace std;

int convert(string& ASnetwork)
{
	int x = 0;
	try
	{
		x = stoi(ASnetwork); //string to an int the word ex: 1239
		return x;
	}
	catch (exception& ex)
	{

		ex.what();
		return 0;
	}
}

string find_token_at_index(string s, string delim, size_t index) //finds the "|"
{
	size_t pos = 0;
	size_t n = 0; //counts delimiter instances  
	string token;
	//while the delimiter is found and counter is less than 6 (index)
	while (((pos = s.find(delim)) != string::npos) && (n < index)) {
		token = s.substr(0, pos);
		s.erase(0, pos + delim.length());
		n++;
	}
	return s.substr(0, s.find_first_of(delim)); //returns 7th line
}


int main()
{
	int AS = 0;
	int Temporary = 0;
	map<int, set<int>>Networks; //one int for the AS; set for neighbors.
	string LINE; //find ASPATH
	string s;
	int previous = 0;
	string delimiter = "|";

	while (getline(cin, s)) //start reading into s, reads a line
	{
		bool firstword = true; //detect if it is the first word.
		LINE = find_token_at_index(s, delimiter, 6); //eg: 1239 2914 174 11096 2553
		istringstream iss(LINE);//read word by word
		string word; // has the word
		while (iss >> word)
		{ //this loop will read word by word
			AS = convert(word); //converts the word to a int and stores it in AS. eg: 1239
			if (AS == 0) { continue; }
			if (firstword == true)
			{
				//check the map if the AS# exists as the key, if not, insert it as new
				if (Networks.find(AS) == Networks.end()) //doesn't exist
				{
					//Networks.insert(make_pair(AS, set<int>())); //this would work too instead of the (if) statement
					Networks[AS] = set<int>(); //create empty set
					previous = AS; //1239 becomes  the past
					firstword = false;
					continue;
				}
				else
				{
					previous = AS; //1239 becomes  the PAST
					firstword = false;
					continue;
				}
			}
			if (Networks.find(AS) == Networks.end()) //if AS# cannot be found 1239 not found
			{
				Networks.insert(make_pair(AS, set<int>())); //create the AS#'s set and put previous node in it
				Networks[AS].insert(previous);
				Networks[previous].insert(AS);
				previous = AS;
			}
			else
			{
				Networks[AS].insert(previous); //inserts the previous node if it does exist
				Networks[previous].insert(AS);
				previous = AS;
			}
		}
	}
	vector<int>TopTenAS;

	int count = 0;
	int MAX = 0;
	int GOAT = 0; //greatest of all AS Technology
	for (int i = 0; i < 10; i++) //go through the loop ten times. if AS is already existing in the vector, do not add it to the vector
	{
		for (auto mapIt = begin(Networks); mapIt != end(Networks); ++mapIt) // this prints out all AS#'s and neighbors.
		{
			if (find(TopTenAS.begin(), TopTenAS.end(), mapIt->first) != TopTenAS.end())
				continue; //if element is found in vector, continue

			count = mapIt->second.size();

			if (find(TopTenAS.begin(), TopTenAS.end(), mapIt->first) == TopTenAS.end() && count > MAX) //if element isn't found in the vector and count > MAX
			{
				count = mapIt->second.size();
				MAX = count;
				GOAT = mapIt->first; //holds maximum AS
			}
		}

		TopTenAS.push_back(GOAT); //insert the max AS into the vector. //must check the vector for this AS#'s!
		MAX = 0;
	}
	
	for (int element : TopTenAS)
	{
		bool flag = true;
		auto it = Networks.find(element);
		if (it != Networks.end())
			cout << (it->first) << " " << it->second.size() << endl;
		for (auto it2 = it->second.begin(); it2 != it->second.end(); ++it2)
			if (flag == true)
			   {
			      cout << *it2;
			      flag = false;
			   }
			else
			    {
			      cout << "|" << *it2;
		            }

		cout << endl;
	}

	return 0;
}
