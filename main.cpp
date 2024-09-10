// Ana Paola Oviedo Salgado | A01738145
// This code sorts the dates of a .txt file
// And asks the user for input for a range of dates, the output displays it
// NOTE: it must be run with this format: g++ -std=c++11 main.cpp -> ./a.out 
#include <iostream> 
#include <fstream>
#include <sstream>
#include <vector>
using namespace std; 

//stucture of the atributes of entries
struct LogEntry {
    int month;
    int day;
    string time;
    string ip_address;
    string failure_reason; 
};

//makes string of month into integer
int Month(string month){
    if (month == "Jan") return 1;
    if (month == "Feb") return 2;
    if (month == "Mar") return 3;
    if (month == "Apr") return 4;
    if (month == "May") return 5;
    if (month == "Jun") return 6;
    if (month == "Jul") return 7;
    if (month == "Aug") return 8;
    if (month == "Sep") return 9;
    if (month == "Oct") return 10;
    if (month == "Nov") return 11;
    if (month == "Dec") return 12;
    return 0; //for invallid input
}

//function to read data and make it a vector -> logEntries
void readData(vector<LogEntry>& logEntries){
    ifstream infile("bitacora.txt"); 
    string line;

    while (getline(infile, line)) {
    istringstream iss(line);
    string month, time, ip_address, failure_reason;
    int day;
    iss >> month >> day >> time >> ip_address;
    int month_integer = Month(month);
    getline(iss, failure_reason);  // for the failure reason

    LogEntry log = {month_integer, day, time, ip_address, failure_reason};
    logEntries.push_back(log); //acomodates the vector 
    }
}

//function to split and merge vector 
void mergeLog(vector<LogEntry>& logEntries, int start, int mid, int end) {
    int n1 = mid - start + 1; // left half
    int n2 = end - mid;       // right half

    // vectors for both halves 
    vector<LogEntry> left(n1);
    vector<LogEntry> right(n2);

    for (int i = 0; i < n1; i++){
        left[i] = logEntries[start + i]; // adding 
    }
    for (int j = 0; j < n2; j++){
        right[j] = logEntries[mid + 1 + j]; //adding 
    }

    int i = 0, j = 0, k = start;

    while (i < n1 && j < n2) {
        if (left[i].month < right[j].month || 
            (left[i].month == right[j].month && left[i].day < right[j].day) ||
            (left[i].month == right[j].month && left[i].day == right[j].day && left[i].time < right[j].time)) {
            logEntries[k] = left[i];
            i++;
        } else {
            logEntries[k] = right[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        logEntries[k] = left[i];
        i++;
        k++;
    }

    while (j < n2) {
        logEntries[k] = right[j];
        j++;
        k++;
    }
}

//function to go merging recursively, mergeLog called 
//& unnecesary copies that could be created 
void mergeSort(vector<LogEntry>&logEntries, int start, int end){
        if(start<end){
            int mid = start + (end - start) / 2;

            mergeSort(logEntries, start, mid);
            mergeSort(logEntries, mid + 1, end);

            mergeLog(logEntries, start, mid,end);
        }
}

// function for the user to introduce the range of months and dates to search
void getDates(int& startMonth, int& startDay, int& endMonth, int& endDay){
    cout<<"Please introduce the START MONTH of your search: "<<endl;
    cin>>startMonth;

    cout<<"Please introduce the START DAY of your search: "<<endl;
    cin>>startDay;

    cout<<"Please introduce the END MONTH of your search: "<<endl;
    cin>>endMonth;

    cout<<"Please introduce the END Day of your search: "<<endl;
    cin>>endDay;
}

//function to search through the vector the dates that the user introduced 
void searchDates(const vector<LogEntry>& logEntries, int startMonth, int startDay, int endMonth, int endDay) {
    bool found = false;
    for (size_t i = 0; i < logEntries.size(); ++i) {
        if (logEntries[i].month > startMonth || 
            (logEntries[i].month == startMonth && logEntries[i].day >= startDay)) {

            if (logEntries[i].month < endMonth || 
                (logEntries[i].month == endMonth && logEntries[i].day <= endDay)) {

                // Display the log entry
                cout << "Month: " << logEntries[i].month << " "
                     << "Day: " << logEntries[i].day << " "
                     << "Time: " << logEntries[i].time << " "
                     << "IP Address: " << logEntries[i].ip_address << " "
                     << "Failure Reason: " << logEntries[i].failure_reason << endl;
           
                found = true; 
            }
        }
    }

    if (!found) { // in case the date is non existent in the .txt
        cout << "No information found for the specified date range" << endl;
    }
}

//function to create the sorted file (.txt)
void newTXT(const vector<LogEntry>& logEntries, const string& filename) {
    ofstream outfile(filename);  

    if (!outfile) {
        cout << "ERROR opening file." << endl;
        return;
    }

    for (const auto& entry : logEntries) {
        outfile << "Month: " << entry.month << " "
                << "Day: " << entry.day << " "
                << "Time: " << entry.time << " "
                << "IP Address: " << entry.ip_address << " "
                << "Failure Reason: " << entry.failure_reason << endl;
    }

    outfile.close();
    cout << "sorted information saved into: " << filename << endl;
}

int main() {
    //1. reads data
    vector<LogEntry> logEntries;
    readData(logEntries);

    //sorts information
    mergeSort(logEntries, 0, logEntries.size() - 1);

    //save sorted data into file
    newTXT(logEntries, "sorted.txt");

    //reseting the code 
    int startMonth, startDay, endMonth, endDay;
    getDates(startMonth, startDay, endMonth, endDay);
    searchDates(logEntries, startMonth, startDay, endMonth, endDay);

    return 0;
}

 