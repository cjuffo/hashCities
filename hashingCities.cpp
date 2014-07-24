#include<iostream>
#include<ctime>
#include<iomanip>
#include<fstream>
#include<string>
#include<cstdlib>
#include<cmath>

using namespace std;

struct postal
{
	string state, name;
	double totalpop, lat, longi;

	void print()
	{
		cout << "\t\tCity:      " << "\t" << name << endl;
		cout << "\t\tState:     " << "\t" << state << endl;
		cout << "\t\tPopulation: " << "\t" << totalpop << endl;
		cout << "\t\tLatitude: " << "\t" << lat << endl;
		cout << "\t\tLongitude: " << "\t" << longi << endl << endl;
	}
};
struct LinkHash
{
	postal  data;
	LinkHash * next;
	int length;

	LinkHash()
	{ }

	LinkHash(string n, string s, double tp, double la, double lo, LinkHash * nx = NULL)
	{
		data.name = n;
		data.state = s;
		data.totalpop = tp;
		data.lat = la;
		data.longi = lo;
		next = nx;
	}
};



class HashMap
{
private:
	LinkHash ** table;
	int size_of;
	int num_of_links;
	double count;


public:
	HashMap()
	{
		size_of = 0;
		num_of_links = 0;
		count = 0;
		table = new LinkHash*[size_of];
		table[0] = NULL;

	}

	HashMap(int s)
	{
		size_of = s;
		num_of_links = 0;
		table = new LinkHash*[size_of];

		for (int i = 0; i<size_of; i++)
		{
			table[i] = NULL;
		}
	}

	int hash(string s)
	{
		char newchar;
		unsigned int hashval = 0;
		for (int i = 0; i < s.length(); i++)
		{
			newchar = s[i];
			hashval += (int)newchar;
			hashval *= 101;
		}
		return hashval%size_of;
	}

	void readStore(int size)
	{
		cout << "Reading file...." << endl;
		string filename = "data.txt";
		ifstream fin(filename.c_str());
		string line;

		while (!fin.eof())
		{
			getline(fin, line);
			if (fin.fail())
				break;

			int endofcity = line.find("  ");
			string name = line.substr(9, endofcity - 9);
			if (name == "Los Angeles") cout << "LA LA LA LA LA\n\n";
			unsigned int hashKey = hash(name);
			string state = line.substr(0, 2);
			int startofpop = line.find_first_not_of(" ", endofcity);
			double totalp = atoi(line.substr(startofpop, line.find_first_of(" ", startofpop) - startofpop).c_str());
			double lati = atof(line.substr(143, 8).c_str());
			double longit = atof(line.substr(153, 8).c_str());

			LinkHash * newLink = new LinkHash(name, state, totalp, lati, longit, NULL);

			if (table[hashKey] == NULL)
			{
				newLink->length++;
				table[hashKey] = newLink;
				num_of_links++;
			}
			else
			{
				LinkHash * newNext = new LinkHash;
				newNext = table[hashKey];

				while (newNext->next != NULL)
					newNext = newNext->next;
				newNext->next = newLink;
			}
			num_of_links++;
			table[hashKey]->length++;
		}
		fin.close();
	}

	void findAndPrint(string city)
	{

		int hashVal = hash(city);

		LinkHash * temp = new LinkHash;
		temp = table[hashVal];
		if (temp == NULL){
			cout << "Cannot locate this city. Please use proper capitalizing of city name such as Miami." << "\n\n";
			return ;
		}
		while ((temp->next != NULL) && (temp->data.name != city))
			temp = temp->next;

		if ((temp->next == NULL) && (temp->data.name != city))
			cout << "Cannot locate this city." << "\n\n";
		else{
			while ((temp->next != NULL)&& (temp->data.name == city)){
				temp->data.print();
				temp = temp->next;
			}
		}
	}

};

void main() {

	string search;
	int size = 32768;
	HashMap * info = new HashMap(size);
	info->readStore(size);
	cout << endl;

	while (true){
		cout << "\n\nPlease type the name of the city you wish to search for or EXIT to end program\n> ";
		getline(cin, search);
		if (search == "EXIT"){
			cout << "Terminating program ...";
			break;
		}
		cout << endl;
		info->findAndPrint(search);
	}


}