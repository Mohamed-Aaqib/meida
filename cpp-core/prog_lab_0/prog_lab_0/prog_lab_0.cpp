#include <iostream>
#include <vector>
#include <iostream>
using namespace std;

vector<pair<int, char>> enumerate(vector<char>& mainL, int startIdx = 0, int steps = 1) {

	int finalEnd = 0;
	vector<pair<int, char>> vec;


	int mainIndx = 0;
	int i = startIdx;

	while (mainIndx < mainL.size()) {

		vec.push_back({ i,mainL[mainIndx] });
		i = i + steps;
		mainIndx++;
	}

	return vec;

}


int main()
{
	
	vector<char> letters = { 'a','b','c' };

    // 3 Cases 


    vector<pair<int, char>> v1 = enumerate(letters);
    cout << "enumerate(list): ";
    for (pair<int, char> p : v1)
        cout << "(" << p.first << ", " << p.second << ") ";
    cout << endl;

    vector<pair<int, char>> v2 = enumerate(letters, 10);
    cout << "enumerate(list, 10): ";
    for (pair<int, char> p : v2)
        cout << "(" << p.first << ", " << p.second << ") ";
    cout << endl;

    vector<pair<int, char>> v3 = enumerate(letters, 3, 2);
    cout << "enumerate(list, 3, 2): ";
    for (pair<int, char> p : v3)
        cout << "(" << p.first << ", " << p.second << ") ";
    cout << endl;


	return 0;


}

