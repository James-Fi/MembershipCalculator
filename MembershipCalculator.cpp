#include <iostream> // cout and cin
#include <iomanip>  // setprecision()
#include <string>   // Strings and stoi()
#include <vector>   // Vectors
#include <cctype>   // toupper()
#include <utility>  // Pairs

using namespace std;

struct MembershipTier {
	char key;
	double price;
	string description;
};

struct DiscountTier {
	int months;
	double discountRate;
};

double getMonthlyRate(const vector<MembershipTier>& mTier);
char validateCharacter(string errorMessage);
int getMonths(string prompt, int lowest, int highest);
int validateNumber();
pair <double, int> getDiscount(int months, const auto& dData);
pair <double, double> calculateTotalCost(double monthlyRate, double discountRate, int months);
bool getConfirmation(string prompt);

vector<MembershipTier> pricingData() {
	// Stores membership and pricing data.
	return {
		{'S', 24.50, "'S' for Standard."},
		{'P', 34.00, "'P' for Peak."},
		{'F', 52.75, "'F' for Family."}
	};
}

vector<DiscountTier> discountData() {
	// Stores required months and discount rates.
	return {
		{12, 0.15},
		{6, 0.10},
		{1, 0.0}
	};
}

int main() {
	// Initialises vectors containing Membership tiers and Monthly Rates.
	auto mTier = pricingData();
	auto dData = discountData();

	int quotesGiven = 0;
	double totalQuotedRevenue = 0.0;

	cout << fixed << setprecision(2);
	cout << "Fitlife Gym Membership Fee Calculator" << endl;

	do {
		string memberName;
		cout << endl << "Member Name: ";
		getline(cin, memberName);

		double monthlyRate = getMonthlyRate(mTier);
		int months = getMonths("Number of Months (1 to 24): ", 1, 24);

		auto [discountRate, discountPercent] = getDiscount(months, dData);
		bool discountApplied = (discountRate > 0.0);
		auto [totalCost, discountTotal] = calculateTotalCost(monthlyRate, discountRate, months);

		totalQuotedRevenue += totalCost;
		quotesGiven++;

		cout << endl << "Monthly Rate: \x9c" << monthlyRate << "." << endl;

		if (discountApplied) {
			cout << "Loyalty discount: " << discountPercent << "% - \x9c" << discountTotal << " saved." << endl;
		}
		else {
			cout << "No Loyalty discount at this duration." << endl;
		}
		cout << "Total for " << memberName << ": \x9c" << totalCost << endl;

	} while (getConfirmation("\nPrice another membership? Y/N: "));

	// Displays the final number of quotes and total revenue quoted.
	cout << endl << "Quotes given: " << quotesGiven << endl;
	cout << "Total revenue quoted: \x9c" << totalQuotedRevenue << "." << endl;
}

double getMonthlyRate(const vector<MembershipTier>& mTier) {
	string errorMessage = "Unrecognised membership tier, try again: ";
	cout << "Available membership tiers" << endl << endl;
	for (const auto& tier : mTier) {
		cout << tier.description << endl;
	}

	while (true) {
		cout << endl << "Member Tier: ";
		char input = validateCharacter(errorMessage);
		for (const auto& tier : mTier) {
			if (tier.key == input) {
				return tier.price;
			}
		}
		cout << errorMessage << endl;
	}
}

char validateCharacter(string errorMessage) {
	// Ensures input contains exactly one character and converts it to uppercase.
	string rawInput;
	while (true) {
		getline(cin, rawInput);
		if (rawInput.length() == 1) {
			char input = (char)toupper(rawInput[0]);
			return input;
		}
		else {
			cout << errorMessage;
		}
	}
}

int getMonths(string prompt, int lowest, int highest) {
	// Ensures the number of months is within the specified range.
	cout << prompt;
	while (true) {
		int input = validateNumber();
		if (input < lowest || input > highest) {
			cout << "Please enter a number of months between " << lowest << " and " << highest << ": ";
		}
		else {
			return input;
		}
	}
}

int validateNumber() {
	// Validates integer input and rejects values with non-numerics.
	while (true) {
		string rawInput;
		getline(cin, rawInput);
		try {
			size_t inputLength;
			int input;
			input = stoi(rawInput, &inputLength);

			// Checks the entire input was converted to an integer.
			if (!(inputLength == rawInput.length())) {
				cout << "Invalid Input, try again using only numbers: ";
			}
			else {
				return input;
			}
		}
		catch (...) {
			cout << "That is not a number, try again: ";
		}
	}
}

pair <double, int> getDiscount(int months, const auto& dData) {
	for (const auto x : dData) {
		if (months >= x.months) {
			return { x.discountRate, static_cast<int>(x.discountRate * 100) };
		}
	}
	return { 0.0, 0 };
}

pair <double, double> calculateTotalCost(double monthlyRate, double discountRate, int months) {
	double beforeDiscount = monthlyRate * months;
	double discountTotal = beforeDiscount * discountRate;

	return { (beforeDiscount - discountTotal), discountTotal };
}

bool getConfirmation(string prompt) {
	// Accepts Y/N to determine if another quote should be generated.
	cout << prompt;
	string errorMessage = "Invalid input, Y/N: ";

	while (true) {
		char input = validateCharacter(errorMessage);
		switch (input) {
		case 'Y':
			return true;
		case 'N':
			return false;
		default:
			cout << errorMessage;
		}
	}
}