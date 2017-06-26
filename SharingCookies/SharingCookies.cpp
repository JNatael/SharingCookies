// problem_c.cpp : Defines the entry point for the console application.
#include <iostream>
#include <vector>  //Vector inclusion
#include <algorithm> //Max

using namespace std;

/*
This function reads a line of int values into a vector function and returns that
*/
vector<int> readlineofints(int start, int end) {
	// Input money line
	vector<int> linevalues;
	for (int j = start; j < end; j++) {
		int val;
		cin >> val;
		linevalues.push_back(val);
	}

	return linevalues;
}

//Calculate cookies in this bowl and return cookie count
long long cookie_calculation(int i, int p, int q, int r, int s) {
	long long cookies = ((long long) ((long long)i*(long long)p + (long long)q) % (long long)r) + (long long)s;
	return cookies;
}


//Binary search through passed in vector
long int binary_search_in_vector_by_index(long int _low, long int _high, const vector<long long> &_search_vector,long long _search_value) {
	
	//Set the midpoint
	long int _mid = (_low + _high) / 2;

	//Iterate until converge
	while(_high - _low > 1){
		if (_search_vector[_mid] == _search_value) { //If we've found the search value
			return _mid; //Return
		}else if(_search_vector[_mid]<_search_value){ //If we're too high, bring up low and update mid
			_low = _mid;
			_mid = (_low + _high) / 2;
		}
		else { //If we're too low, bring down high and update mid
			_high = _mid;
			_mid = (_low + _high) / 2;
		}
	}

	if (_high > _search_vector.size() - 1) { _high--; } //If _high is bigger than the vector, reduce it

	//If we made it this far, return whichever value is closer
	if (abs(_search_value - _search_vector[_low]) < abs(_search_value - _search_vector[_high])) {
		return _low;
	}
	else {
		return _high;
	}
}

long long check_bowl_shift(int shift1, int shift2, vector<long long> &bowls,long long RunningSum, long int a, long int b) {
	//Check to see if Tom's take could be smaller with one shift left
	long long Option1 = bowls[a +shift1];
	long long Option2 = bowls[b + shift2] - bowls[a + shift1];
	long long Option3 = RunningSum - bowls[b + shift2];
	long long potential_take = max(Option1, Option2); //Set to max of first two options
	potential_take = max(potential_take, Option3); //Set to max of previous max and third option
	return potential_take;
}

long long sequential_shift_check(int aplus, int aminus, int bplus, int bminus, vector<long long> &bowls, long long RunningSum, long int a, long int b,long long Toms_Take) {
	Toms_Take = min(Toms_Take, check_bowl_shift(aminus, bminus, bowls, RunningSum, a, b)); //Subtract from both
	Toms_Take = min(Toms_Take, check_bowl_shift(aminus, 0, bowls, RunningSum, a, b)); //Subtract/nothing
	Toms_Take = min(Toms_Take, check_bowl_shift(aminus, bplus, bowls, RunningSum, a, b)); //Subtract/add
	Toms_Take = min(Toms_Take, check_bowl_shift(0, bminus, bowls, RunningSum, a, b)); //Nothing/subtract
	Toms_Take = min(Toms_Take, check_bowl_shift(0, bplus, bowls, RunningSum, a, b)); //Nothing/add
	Toms_Take = min(Toms_Take, check_bowl_shift(aplus, bminus, bowls, RunningSum, a, b)); //Add/subtract
	Toms_Take = min(Toms_Take, check_bowl_shift(aplus, 0, bowls, RunningSum, a, b)); //Add/nothing
	Toms_Take = min(Toms_Take, check_bowl_shift(aplus, bplus, bowls, RunningSum, a, b)); //Add to both
	return Toms_Take;
}

int main() {
	std::ios_base::sync_with_stdio(false);

	//get test case count
	int t;
	std::cin >> t;

	//loop over all the test cases
	for (int i = 1; i <= t; i++) {
		// Input parameters line
		vector<int> parameters = readlineofints(0, 5);

		vector<long long> bowls;
		bowls.push_back(0);
		long long RunningSum = 0;
		//Iterate over all the bowls, populating them
		for (int j = 1; j <= parameters[0]; j++) {
			RunningSum += cookie_calculation(j, parameters[1], parameters[2], parameters[3], parameters[4]);
			bowls.push_back(RunningSum);
		}

		//Search for a at the 1/3 point
		long int a = binary_search_in_vector_by_index(1, bowls.size(), bowls, RunningSum / 3);
		
		//Search for b at the midpoint of the remaining space
		long int b = binary_search_in_vector_by_index(a, bowls.size(), bowls, (RunningSum + bowls[min(a, (long int) bowls.size()-1)])/2);

		//Deal with exceptions in which b is too big due to limited bowls
		
		if (b >= bowls.size()-1) {
			b = bowls.size() - 1;
			if (a >= bowls.size()-1) { //If a is too big
				if (bowls.size() == 2) { //If there's only one bowl, set it equal to b
					a = bowls.size() - 1;
				}
				else {
					a = bowls.size() - 2; //If there's more than one bowl (presumably 2) set it one less than b
				}
			}
		}
			long long Toms_Take;
			//Tom takes the max of the bowls to a, the bowls from a+1 to b, or the bowls from b+1 on. This numbering is one off from the sheet
			//but shouldn't make a difference in the result
			long long Option1 = bowls[a];
			long long Option2 = bowls[b] - bowls[a];
			long long Option3 = RunningSum - bowls[b];
			Toms_Take = max(Option1,Option2); //Set to max of first two options
			Toms_Take = max(Toms_Take,Option3); //Set to max of previous max and third option

			//If the array is big enough for a shift, check to see if that's better by shifting one in each direction
			int aminus =0, aplus = 0, bminus=0, bplus=0;
			if (a > 1) { aminus = -1; }
			if (b<bowls.size()-1) { bplus = 1; }
			if (b > 1) { bminus = -1; }
			if (a<bowls.size()-1) { aplus = 1; }
			Toms_Take = sequential_shift_check(aplus, aminus, bplus, bminus, bowls, RunningSum, a, b, Toms_Take);/*
			if (a > 2) { aminus = -2; }
			if (b<bowls.size()-2) { bplus = 2; }
			if (b > 2) { bminus = -2; }
			if (a<bowls.size()-2) { aplus = 2; }
			Toms_Take = sequential_shift_check(aplus, aminus, bplus, bminus, bowls, RunningSum, a, b, Toms_Take);
			Toms_Take = sequential_shift_check(aplus/2, aminus, bplus, bminus, bowls, RunningSum, a, b, Toms_Take);
			Toms_Take = sequential_shift_check(aplus, aminus/2, bplus, bminus, bowls, RunningSum, a, b, Toms_Take);
			Toms_Take = sequential_shift_check(aplus, aminus, bplus/2, bminus, bowls, RunningSum, a, b, Toms_Take);
			Toms_Take = sequential_shift_check(aplus, aminus, bplus, bminus/2, bowls, RunningSum, a, b, Toms_Take);*/

		//write output
		std::cout << "Case #" << i << ": " << RunningSum - Toms_Take << std::endl;
	}

}
