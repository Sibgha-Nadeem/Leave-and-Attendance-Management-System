#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <ctime>
#include <vector>
#include <sstream>
#include <set>
#include <iomanip>
using namespace std;


// Interface class defining the blueprint for a Worker class
class IWorker
{
    // Interface for the Worker class, providing pure virtual functions
public:
    // Retrieves the unique ID of the worker
    virtual string getID() = 0;

    // Retrieves the name of the worker
    virtual string getName() = 0;

    // Retrieves the position or role of the worker (e.g., Employee, Manager)
    virtual string getPosition() = 0;

    // Retrieves the worker's password (Note: storing passwords as plain text is a bad practice in real systems)
    virtual string getPassword() = 0;

    // Retrieves the worker's clock-in time
    virtual string getInTime() = 0;

    // Retrieves the worker's clock-out time
    virtual string getOutTime() = 0;

    // Sets the worker's clock-in time
    virtual void setInTime(string inTime) = 0;

    // Sets the worker's clock-out time
    virtual void setOutTime(string outTime) = 0;
};
// Concrete implementation of the IWorker interface
class Worker : public IWorker
{
    // Protected members are accessible to derived classes
protected:
    string ID;         // Unique identifier for the worker
    string name;       // Name of the worker
    string position;   // Position or role of the worker
    string password;   // Password for the worker's account (Plain text storage is not secure)
    string inTime;     // Clock-in time
    string outTime;    // Clock-out time

public:
    // Default and parameterized constructor
    // Initializes the worker with default or specified values
    Worker(string name = "", string id = "", string pass = "", string pos = "Employee")
        : name(name), ID(id), password(pass), position(pos) {}

    // Overrides the pure virtual functions from the IWorker interface

    // Returns the worker's ID
    string getID() override { return ID; }

    // Returns the worker's name
    string getName() override { return name; }

    // Returns the worker's position or role
    string getPosition() override { return position; }

    // Returns the worker's password
    string getPassword() override { return password; }

    // Returns the worker's clock-in time
    string getInTime() override { return inTime; }

    // Returns the worker's clock-out time
    string getOutTime() override { return outTime; }

    // Sets the worker's clock-in time
    void setInTime(string inTime) override { this->inTime = inTime; }

    // Sets the worker's clock-out time
    void setOutTime(string outTime) override { this->outTime = outTime; }
};//class to handle read,write,append functionality of files
//Static functions allow you to call them without creating an instance of the class, making them ideal for utility operations
class FileOperationsHandler
{
public:
    static vector<string> readfromthefile(const string& filename)
    {
        ifstream file(filename);
        vector<string> lines;

        if (!file.is_open())//check if file open.
        {
            cout << "\033[33m--- Error: Could not open file. ---/033[0m" << filename << endl;
            return lines;
        }

        string line;
        //line by line read.
        while (getline(file, line))
        {
            lines.push_back(line);
        }
        file.close();
        return lines;
    }
    static void writetothefile(const string& filename, const vector<string>& lines)
    {
        ofstream file(filename, ios::trunc);
        // trunc to overwrite
        if (!file.is_open())
        //check if open
        {
            cout << "\033[33m--- Error: Could not open file. ---\033[0m" << filename << endl;
            return;
        }

        for (const auto& line : lines)
        //writing to file line by line.
        {
            file << line << endl;
        }
        file.close();
    }
    static void appenddataonthefile(const string& filename, const vector<string>& lines)
    {
        ofstream file(filename, ios::app);
        // app to append
        if (!file.is_open())
        //check if open.
        {
            cout << "\033[33m--- Error: Could not open file ---\033[0m" << filename << endl;
            return;
        }

        for (const auto& line : lines)
        //writing to file line by line.
        {
            file << line << endl;
        }
        file.close();
    }
    static vector<string> parseLine(const vector<string>& line, char delimiter = '/')
    {
        vector<string> arr;
        string combinedLine;
        for (size_t i = 0; i < line.size(); ++i)
        {
            combinedLine += line[i];
            if (i < line.size() - 1)
            {
                combinedLine += delimiter;
            }
        }
        stringstream ss(combinedLine);  
        // Use the vector converted into string because stringstream works on string, not vector.
        string t;
        while (getline(ss, t, delimiter))
        {
            arr.push_back(t);
        }
        return arr;
    }
    //sample string like name/ID/Password/Position
    static string formatLine(const vector<string>& tokens, char delimiter = '/')
    {
        ostringstream ofile;
        for (size_t i = 0; i < tokens.size(); ++i)
        {
            ofile << tokens[i];
            if (i < tokens.size() - 1)
            {
                ofile << delimiter;
                //putting our data after every '/'
            }
        }
        return ofile.str();
    }
};
class UserData 
//class for user id and name when entering.
{
private:
    string employeeID;
    string Employeename;

public:
    void setEmployeeID(const string& id)
    //set id
    {
        employeeID = id;
    }
    void setName(const string& empName)
    //set name 
    {
        Employeename = empName;
    }
    string getEmployeeID() const
    //getting the id from user
    {
        return employeeID;
    }
    string getName() const
    //getting name from user.
    {
        return Employeename;
    }
};
static UserData currentUserLogin;
// Encapsulated object for UserData class.
//the role class is for giving the roles to the worker like supervisor,director,employee and guard done by Composition.
class Role 
{
public:
    virtual void showDashboard(vector<Worker>& employees) = 0; 
    // Polymorphism
    virtual ~Role() = default;
    // Virtual destructor for cleanup
};

//Factory Method Pattern for Worker
//done dynamically. this helps in a way that instead of manually entering a new kind of worker we could just automatically make a new worker dunamically.
class WorkerFactory
{
public:
    static Worker* createWorker(const string& name, const string& id, const string& password, const string& role)
    //dynamic function having dynamically assign object to each kind of worker.
    {
        return new Worker(name, id, password, role);
    }
};

//Observer Pattern
class Observer
{
public:
    //virtuak function to be overridden
    //Observer is the Employee to be notified when leave approved/rejected
    virtual void update(const string& leaveStatus) = 0;
};
class Subject
{
public:
    //subject is to notify user about leave status changes
    virtual void addObserver(Observer* observer) = 0;//adding observer
    virtual void removeObserver(Observer* observer) = 0;//removing observer 
    virtual void notify() = 0;//to notify the user about the change made.
    virtual ~Subject() = default;//destructor
};

class Leave;        //forward declaration because of circular dependency

//State Pattern
class LeaveStatusState 
{
public:
    LeaveStatusState() {} //constructor
    virtual void approvetheLeave(Leave* leave) = 0; 
    //virtual function for a case where leave is approved.
    virtual void rejecttheLeave(Leave* leave) = 0;
    //virtual function for a case in which a leave is rejected.
    virtual string getLeaveStatus() const = 0; 
    //virtual function for getting the leave status from the file/when user enters or the defaulkt to change it to either approve or reject.
    virtual ~LeaveStatusState() = default; 
    //destructor
};
class PendingState : public LeaveStatusState 
//for handling the pending state.
{
public:
    void approvetheLeave(Leave* leave) override;
    //function decleration. override to change the data for this class also because of virtual.
    void rejecttheLeave(Leave* leave) override;
    //function decleration. override to change the data for this class also because of virtual.
    string getLeaveStatus() const override 
    //function defination+ decleration, return pending as default is pending when user gives a leave. override to change the data for this class also because of virtual.
    {
        return "Pending";
    }
};
class ApprovedState : public LeaveStatusState 
{
public:
    void approvetheLeave(Leave* leave) override
    //in case leave is approved a message is displayed.
    {
        cout << "\033[33mLeave is already in ApprovedState.\033[0m" << endl;
    }

    void rejecttheLeave(Leave* leave) override 
    //display a message if the leave is made to be approved if it has already been rejected.
    {
        cout << "\033[33mCannot reject an approved leave.\033[0m" << endl;
    }

    string getLeaveStatus() const override 
    //status will be changed to approved from pending because the leave has been approved by supervisor or director.
    {
        return "Approved";
    }
};
class RejectedState : public LeaveStatusState 
{
public:
    void approvetheLeave(Leave* leave) override 
    //display a message if the leave is made to be approved if it has already been rejected.
    {
        cout << "\033[33mCannot approve a rejected leave.\033[0m" << endl;
    }

    void rejecttheLeave(Leave* leave) override 
    //in case leave is rejected a message is displayed.
    {
        cout << "\033[33mLeave is already in RejectedState.\033[0m" << endl;
    }

	string getLeaveStatus() const override  
    //status will been changed to approved from pending because the leave has been approved by supervisor or director.
	{
        return "Rejected";
    }
};

//interface for Leave
class ILeave
{
public:
    //pure virtual functions for abstract class for interface
    virtual string getLeaveType() const = 0;
    virtual int getLeaveId() const = 0;
    virtual string getStartDate() const = 0;
    virtual string getEndDate() const = 0;
    virtual string getReason() const = 0;
    virtual string getApprovalStatus() const = 0;
    virtual string getAddress() const = 0;
    virtual string getApplicationDate() const = 0;
    virtual void display() const = 0;
};
//defrault leave class containing of all the leave data members and is the parent class to several Leave Types.
class Leave : public ILeave, public Subject 
{
   //Leave inheirts from Interface class and Subject class (for implementing Observer Pattern)
protected:
//protected so that it can used by the children so that public can use it to but for the class itself it is private.
    string leave_type;
    int leave_id;
    string start_date, end_date, reason, approval_status, address, application_date;
    vector<Observer*> observers;//observer object
    LeaveStatusState* presentState = nullptr;// defualt nullptr 
    virtual void displayDetails() const   // Shared display logic for all leave types
    {
        cout << "\033[33m-----EMPLOYEE LEAVE RECORD-----\033[0m" << endl;
        cout << "\033[33mLeave Type: \033[0m" << leave_type << endl;
        cout << "\033[33mEmployee ID: \033[0m" << leave_id << endl;
        cout << "\033[33mStart Date: \033[0m" << start_date << endl;
        cout << "\033[33mEnd Date: \033[0m" << end_date << endl;
        cout << "\033[33mReason: \033[0m" << reason << endl;
        cout << "\033[33mApproval Status: \033[0m" << approval_status << endl;
        cout << "\033[33mAddress: \033[0m" << address << endl;
        cout << "\033[33mApplication Date: \033[0m" << application_date << endl;
        cout << "\033[33m--------------------------------" << endl;
    }
public:
    //constructor
    Leave(int id, const string& start, const string& end, const string& reason, const string& approval,const string& address, const string& application_date): leave_id(id), start_date(start), end_date(end), reason(reason),approval_status(approval), address(address), application_date(application_date)
    {
        if (approval_status == "Pending")
            presentState = new PendingState();
        else if (approval_status == "Approved")
            presentState = new ApprovedState();
        else if (approval_status == "Rejected")
            presentState = new RejectedState();
    }
    //overriding functions
    int getLeaveId() const override { return leave_id; }
    string getStartDate() const override { return start_date; }
    string getEndDate() const override { return end_date; }
    string getReason() const override { return reason; }
    string getApprovalStatus() const override { return approval_status; }
    string getAddress() const override { return address; }
    string getApplicationDate() const override { return application_date; }
    string getLeaveType() const override { return leave_type; }
    void display() const override
    {
        displayDetails();        
        // Called the shared display logic
    }
    // Subject interface methods (for observer Pattern)
    void addObserver(Observer* observer) override
    {
        //adds an observer into vector
        observers.push_back(observer);
    }
    void removeObserver(Observer* observer) override 
    {
        //removes an observer from vector
        observers.erase(remove(observers.begin(), observers.end(), observer), observers.end());
    }
    void notify() override 
    {
        //sends/notifies the observer if change occured
        for (Observer* observer : observers) 
        {
            observer->update(approval_status);
        }
    }
    void setApprovalStatus(const string& status) 
    {
        //updates the status of approval accordingly and then notifies
        approval_status = status;
        notify();
    }
    void setState(LeaveStatusState* state) 
    {
        //changes state of object to the one sent in parameter
        if (presentState)
            delete presentState;
        presentState = state;
    }
    void approvetheLeave() 
    {
        // for approval of leave
        presentState->approvetheLeave(this);
        notify(); // Notifying observers of the status change
    }
    void rejecttheLeave()
    {
        //for rejecting leave
        presentState->rejecttheLeave(this);
        notify(); // Notifying observers of the status change
    }
    ~Leave() 
    {
        delete presentState; // Free the present state
    }
};
void PendingState::approvetheLeave(Leave* leave)// function defination because of circular dependency.
{
    leave->setApprovalStatus("Approved");
    leave->setState(new ApprovedState());
    cout << "\033[33mLeave approved and transitioned to ApprovedState.\033[0m" << endl;
}
void PendingState::rejecttheLeave(Leave* leave) 
//function defination because of circular dependency.
{
    leave->setApprovalStatus("Rejected");
    leave->setState(new RejectedState());
    cout << "\033[33mLeave rejected and transitioned to RejectedState.\033[0m" << endl;
}
//Employee class for employees that can see attendance and apply/see leaves.
class Employee
{
    Worker worker;  // Composition: Employee "has-a" Worker instead of inheritance
public:
    //default + parameterized constructor
    Employee(string id = "", string name = "", string pass = "") : worker(id, name, pass, "Employee")
    {}
};
//Guard class which can save and mark attendance. Guard must be on duty.
class Guard
{
private:
    Worker worker;  // Composition: Guard "has-a" Worker instead of inheritance
    int gateLocation;
    string shift;
    //temporary vector to store the EmployeeID, Time, Status before saving into file
    vector<string> attendanceRecords;
    //gets the current time and date for marking attendance
    string getCurrentTimestamp() const
    {
        time_t now = time(0);
        struct tm ltm;
        localtime_s(&ltm, &now);
        char buffer[20];
        strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &ltm);
        return string(buffer);
    }
public:
    //default + parameterized constructor; assuming gate location is 1 and morning shift
    Guard(string name = "", string id = "", string pass = "") : worker(name, id, pass, "Guard")
    {
        gateLocation = 1;
        shift = "M";
    }
    //stores the empID, time, status into the temporary vector
    void markEmpAttendance(Worker* emp, const string& In_Out)
    {
        string timestamp = getCurrentTimestamp();
        string record = timestamp + "/" + emp->getID() + "/" + In_Out;
        attendanceRecords.push_back(record);
        //initialize the sign-in or sign-out time of the employee object
        if (In_Out == "Sign-In") 
        {
            emp->setInTime(timestamp);
        }
        else if (In_Out == "Sign-Out") 
        {
            emp->setOutTime(timestamp);
        }
        cout << "\033[33mAttendance recorded for Employee \033[0m" << emp->getID() << endl;
    }
    //opens file to save the temporary vector into Attendance file
    void saveAttendanceToFile(const string& filename)
    {
        FileOperationsHandler::appenddataonthefile(filename, attendanceRecords);
        //call from another class to make sure it handles only 1 functionality.
        cout << "\033[33mAttendance record added\033[0m" << endl;
        attendanceRecords.clear();
    }
};
//Supervisor class which is used to approve and reject the leaves and if necessary send the long earned leaves to the Director.
class Supervisor : public Observer
{
    //inherits from Observer because of Observer Pattern
private:
    Worker worker;  // Composition: Supervisor "has-a" Worker
    int no_of_leaves_approved; //Leaves approve
    int no_of_leaves_rejected; //leaves rejected
    // For reading leave record file
    vector<string> SupervisorreadLeaveRecords(const string& filename)
    {
        return FileOperationsHandler::readfromthefile(filename); 
        //call from another class to make sure it handles only 1 functionality.
    }
    void SupporvisorUpdatesLeaveRecords(const string& filename, const vector<string>& leaveRecords)
    {
        //for updating leave records
        FileOperationsHandler::writetothefile(filename, leaveRecords); 
        //call from another class to make sure it handles only 1 functionality.
    }
    static void EarnedforwardToDirector(const string& leaveRecord, const string& filename) 
    //forward to director in case long earned leaves because they cannot approve it themseleves.
    {
        // To forward the leave to director; saves the leave into the director's file
        ofstream directorFile("DirectorApproval.txt", ios::app); //append as we dont want to rewrite everything but change what is required.
        if (!directorFile) //check
        {
            cerr << "\033[33m!!!!!! Error opening file !!!!!!\033[0m" << endl;
            return;
        }
        directorFile << leaveRecord << endl;//written to director file "DirectorApproval.txt"
        directorFile.close();
        cout << "\033[33m----Long Earned Leave forwarded to Director for further approval.----\033[0m" << endl; 
        //messge to tell supervisor that they have to and have forwarded the leave to director.
    }
    int date_difference_in_day(const string& start_date, const string& end_date)  //calculates the difference in days.
    {
        tm stt_tm = {};
        tm endd_tm = {};
        istringstream ss_start(start_date); // Convert start_date string to tm as istringstream can only use string 
        ss_start >> get_time(&stt_tm, "%Y-%m-%d");//time pattern 
        if (ss_start.fail()) //in case of fail in change.
        {
            cerr << "Invalid start date format: " << start_date << endl;
            return -1; // Error handling
        }
        istringstream ss_end(end_date); // Convert end_date string to tm as istringstream can only use string 
        ss_end >> get_time(&endd_tm, "%Y-%m-%d");//time pattern 
        if (ss_end.fail()) //in case of fail in change.
        {
            cerr << "Invalid end date format: " << end_date << endl;
            return -1; // Error handling
        }
        time_t beginning_time = mktime(&stt_tm); // Converting tm -> time_t
        time_t finish_time = mktime(&endd_tm); // Convert tm -> time_t
        if (beginning_time == -1 || finish_time == -1) //check
        {
            cerr << "Error converting dates to time_t." << endl;
            return -1; 
        }
        const int sec_per_day = 60 * 60 * 24;//seconds in a day (min* sec*hours)
        return static_cast<int>((finish_time - beginning_time) / sec_per_day); //returing the diff of days 
    }
public:
    // Constructor
    Supervisor(string name = "", string id = "", string pass = "") : worker(name, id, pass, "Supervisor")
    {
        no_of_leaves_approved = 0;
        no_of_leaves_rejected = 0;
    }
    int getApprovedLeavesCount() const // get for no_of_leaves_approved
    {
        return no_of_leaves_approved;
    }
    void setApprovedLeavesCount(int count)// set  for no_of_leaves_approved
    {
        no_of_leaves_approved = count;
    }
    int getRejectedLeavesCount() const // get for no_of_leaves_rejected
    {
        return no_of_leaves_rejected;
    }
	void setRejectedLeavesCount(int count)// set for no_of_leaves_rejected
    {
        no_of_leaves_rejected = count;
    }
    void update(const string& leaveStatus) override
    {
        //for Observer pattern to notify employee of leave status change
        cout << "\033[33mSupervisor notified of leave status change:\033[0m " << leaveStatus << endl;
    }
    void approveRejectLeave(const string& filename)
    {
        vector<string> leaveRecords = SupervisorreadLeaveRecords(filename);
        vector<string> updatedRecords;
        bool foundPendingLeave = false;  // bool Tracker for if any pending leave is found
        bool fileUpdated = false;        // bool Trackeer for if the file is updated
        for (const string& record : leaveRecords)// going through all the enteries in the leave records file to approve and reject pending leaves.
        {
            istringstream ss(record);
            vector<string> arr;
            string token;

            while (getline(ss, token, '/')) //line by line. 
            {
				arr.push_back(token);
            }
            if (arr.size() < 8)
            {
                updatedRecords.push_back(record);
                continue;
            }
            //from file 
            string leaveStatus = arr[4]; // Pending/Approved/Rejected
            string leaveType = arr[5];   // leave types like casuak  etc.
            string leaveStartDate = arr[1]; // starting date entered by user.
            string leaveEndDate = arr[2];
            string leaveReason = arr[3];

            if (leaveStatus == "Pending") 
            {
                foundPendingLeave = true; // telling that a pending leave was found
                int leaveDuration = date_difference_in_day(leaveStartDate, leaveEndDate);
                cout << endl;
                cout << "\033[33mLeave Request Details:\033[0m" << endl;
                cout << "\033[33mEmployee ID: \033[0m" << arr[0] << endl;
                cout << "\033[33mStart Date: \033[0m" << leaveStartDate << endl;
                cout << "\033[33mEnd Date: \033[0m" << leaveEndDate << endl;
                cout << "\033[33mReason:\033[0m " << leaveReason << endl;
                cout << "\033[33mType: \033[0m" << leaveType << endl;
                cout << "\033[33mDuration: \033[0m" << leaveDuration << " \033[33mdays\033[0m" << endl;
                if (leaveType == "EarnedLeave" && leaveDuration > 4) // Checking if the leave is an Earned Leave and needs to be forwarded to Director
                {
                    cout << "\033[33mThis is a long Earned Leave and requires Director approval.\033[0m" << endl;
                    EarnedforwardToDirector(record, filename); // Forwarding the leave to the Director
                    continue; // Skip and continue to the next record
                }
                cout << "\033[35mEnter 'A' to Approve or 'R' to Reject:\033[0m ";
                char choice;
                cin >> choice;
                Leave leave(stoi(arr[0]), leaveStartDate, leaveEndDate, leaveReason, arr[4], "", arr[7]); //object 
                leave.addObserver(this); // Attaching the supervisor as an observer
                if (choice == 'A' || choice == 'a')//approved 
                {
                    leave.approvetheLeave();  // Approve the leave
					arr[4] = "Approved";   // Updating the leave status
                    cout << "\033[35mEnter approval date (YYYY-MM-DD):\033[0m ";
                    string approvalDate;
                    cin >> approvalDate;//get approval date by user 
					arr.push_back(approvalDate); // add the user given approval date
                    setApprovedLeavesCount(getApprovedLeavesCount() + 1);
                    fileUpdated = true; // Indicating  that the file needs to be updated so that if no need than the file is not emptied.
                }
                else if (choice == 'R' || choice == 'r') // reject 
                {
                    leave.rejecttheLeave();  // Rejecting the leave
					arr[4] = "Rejected";  // Updating the leave status
                    cout << "\033[35mEnter rejection date (YYYY-MM-DD):\033[0m ";
                    string rejectionDate;//get rejecting date 
                    cin >> rejectionDate;
					arr.push_back(rejectionDate); // Adding rejection date
                    setRejectedLeavesCount(getRejectedLeavesCount() + 1);
                    fileUpdated = true; // Indicating  that the file needs to be updated
                }
                else //in case anything else is entered by user 
                {
                    
                    cout << "\033[33mInvalid entry, try again\033[0m" << endl;
                    continue;
                }
                // Adding  the updated record back to updatedRecords
                string updatedRecord;
                for (size_t i = 0; i < arr.size(); ++i)
                {
                    updatedRecord += arr[i]; //pushes back into the file
                    if (i < arr.size() - 1) updatedRecord += "/";
                }
                updatedRecords.push_back(updatedRecord);
            }
        }
        if (!foundPendingLeave) //show this message once if no pending leaves were found
        {
           
            cout << "\033[33m--- No Pending Leaves for approval/rejection.---\033[0m" << endl;
        }
        if (fileUpdated)  //If any leave was approved/rejected we are updating the file
        {
            SupporvisorUpdatesLeaveRecords(filename, updatedRecords);
            //pattern : EmployeeId/StartDate/EndDate/Reason/Status/Type/Location/ApplicationDate/Approval/RejectionDate 
            cout << "\n\n\033[33m---Leave processing completed.---\033[0m" << endl;
        }
        else // display if nothing more to reject or approve.
        {
            cout << "\033[33m--- No changes made to the leave records. File remains unchanged. ---\033[0m" << endl;
        }
    }
};
//class Director which can do most of worker/Employee functionality but majorly handles the approve rejection of long Earned Leaves.
class Director : public Observer
{
private:
    Worker worker;  // Composition: Director "has-a" Worker instead of inheritance
    int no_of_leaves_approved;
    int no_of_leaves_rejected;
    vector<string> readLeaveRecords(const string& filename) 
    {
        //reads its leaves from the "DirectorApproval.txt" file
		return FileOperationsHandler::readfromthefile(filename); //call from another class to make sure it handles only 1 functionality.
    }
    void updateLeaveRecords(const string& filename, const vector<string>& leaveRecords) 
    {
        //trunc means to trunicate
		FileOperationsHandler::writetothefile(filename, leaveRecords); //call from another class to make sure it handles only 1 functionality.
	}
    void updateLeaveRecordsInMainFile(const string& mainFilename, const string& approvalFilename)
    {
        //updates the original LeaveRecords.txt file to the changes made
        vector<string> mainLeaveRecords = readLeaveRecords(mainFilename);
        vector<string> approvalLeaveRecords = readLeaveRecords(approvalFilename);
        vector<string> updatedMainRecords;

        for (const string& mainRecord : mainLeaveRecords) 
        {
            //all done to save the changes made by director into the LeaveRecords.txt file
            bool found = false;
            for (const string& approvalRecord : approvalLeaveRecords)
            {
                vector<string> mainTokens;
                vector<string> approvalTokens;
                istringstream mainSS(mainRecord);
                istringstream approvalSS(approvalRecord);
                string token;

                while (getline(mainSS, token, '/'))  //line by line push in file 
                {
                    mainTokens.push_back(token);
                }
                while (getline(approvalSS, token, '/'))  //line by line 
                {
                    approvalTokens.push_back(token);
                }

                if (mainTokens[0] == approvalTokens[0] && mainTokens[1] == approvalTokens[1] && mainTokens[2] == approvalTokens[2]) //check if same record than update it with the one in DirectorApproval that the director jsut approved/rejected.
                {
                    updatedMainRecords.push_back(approvalRecord);
                    found = true;
                    break;
                }
            }
            if (!found)//check 
            {
                updatedMainRecords.push_back(mainRecord);
            }
        }
        updateLeaveRecords(mainFilename, updatedMainRecords);//update using main files.
		//pattern : EmployeeId/StartDate/EndDate/Reason/Status/Type/Location/ApplicationDate/Approval/RejectionDate 
    }
    int date_difference_in_day(const string& start_date, const string& end_date) 
    {
		tm stt_tm = {};
		tm endd_tm = {};
		istringstream ss_start(start_date); // Convert start_date string to tm as istringstream can only use string 
		ss_start >> get_time(&stt_tm, "%Y-%m-%d");//time pattern 
		if (ss_start.fail()) //in case of fail in change.
		{
			cerr << "Invalid start date format: " << start_date << endl;
			return -1; // Error handling
		}
		istringstream ss_end(end_date); // Convert end_date string to tm as istringstream can only use string 
		ss_end >> get_time(&endd_tm, "%Y-%m-%d");//time pattern 
		if (ss_end.fail()) //in case of fail in change.
		{
			cerr << "Invalid end date format: " << end_date << endl;
			return -1; // Error handling
		}
		time_t beginning_time = mktime(&stt_tm); // Converting tm -> time_t
		time_t finish_time = mktime(&endd_tm); // Convert tm -> time_t
		if (beginning_time == -1 || finish_time == -1) //check
		{
			cerr << "Error converting dates to time_t." << endl;
			return -1;
		}
		const int sec_per_day = 60 * 60 * 24;//seconds in a day (min* sec*hours)
		return static_cast<int>((finish_time - beginning_time) / sec_per_day); //returing the diff of days 
    }
public:
    //constructor
    Director(string id = "", string name = "", string pass = "") : worker(id, name, pass, "Director")
    {
        no_of_leaves_approved = 0;
        no_of_leaves_rejected = 0;
    }
    // Getter for no_of_leaves_approved
    int getApprovedLeavesCount() const
    {
        return no_of_leaves_approved;
    }
    // Setter for no_of_leaves_approved
    void setApprovedLeavesCount(int count) 
    {
        no_of_leaves_approved = count;
    }
    // Getter for no_of_leaves_rejected
    int getRejectedLeavesCount() const 
    {
        return no_of_leaves_rejected;
    }
    // Setter for no_of_leaves_rejected
    void setRejectedLeavesCount(int count)
    {
        no_of_leaves_rejected = count;
    }
    // Implement the update method from Observer interface
    void update(const string& leaveStatus) override
    {
        cout << "\033[33mDirector notified of leave status change:\033[0m " << leaveStatus << endl;
    }
    void approve_reject_leave(const string& filename)
    {
        //for rejecting/approving the leave by director
        vector<string> leaveRecords = readLeaveRecords(filename);
        vector<string> updatedRecords;
        bool foundPendingLeave = false;  // Track if any pending leave is found
        bool fileUpdated = false;        // Track if the file is updated

        for (const string& record : leaveRecords) 
        {
            istringstream ss(record);
            vector<string> tokens;
            string token;

            while (getline(ss, token, '/'))
            {
                tokens.push_back(token);
            }
            if (tokens.size() < 8) 
            {
                updatedRecords.push_back(record);
                continue;
            }
            string leaveStatus = tokens[4]; // Pending/Approved/Rejected
            string leaveStartDate = tokens[1];
            string leaveEndDate = tokens[2];
            string leaveReason = tokens[3];

            if (leaveStatus == "Pending") 
            {
                foundPendingLeave = true; // Mark that a pending leave was found
                int leaveDuration = date_difference_in_day(leaveStartDate, leaveEndDate);
                cout << endl;
                cout << "\033[33mLeave Request Details:\033[0m" << endl;
                cout << "\033[33mEmployee ID:\033[0m " << tokens[0] << endl;
                cout << "\033[33mStart Date: \033[0m" << leaveStartDate << endl;
                cout << "\033[33mEnd Date: \033[0m" << leaveEndDate << endl;
                cout << "\033[33mReason:\033[0m " << leaveReason << endl;
                cout << "\033[33mDuration:\033[0m " << leaveDuration << " \033[33mdays\033[0m" << endl;

                Leave leave(stoi(tokens[0]), leaveStartDate, leaveEndDate, leaveReason, tokens[4], "", tokens[7]);
                leave.addObserver(this); // Attach the director as an observer

                cout << "\033[35mEnter 'A' to Approve or 'R' to Reject:\033[0m ";
                char choice;
                cin >> choice;

                // Handle Approval or Rejection
                if (choice == 'A' || choice == 'a')
                {
                    leave.approvetheLeave();  // Approve the leave
                    tokens[4] = "Approved";   // Update the leave status
                    setApprovedLeavesCount(getApprovedLeavesCount() + 1);
                    fileUpdated = true;       // Indicate that the file needs to be updated
                }
                else if (choice == 'R' || choice == 'r') 
                {
                    leave.rejecttheLeave();  // Reject the leave
                    tokens[4] = "Rejected";  // Update the leave status
                    setRejectedLeavesCount(getRejectedLeavesCount() + 1);
                    fileUpdated = true;       // Indicate that the file needs to be updated
                }
                else 
                {
                    cout << "\033[33m------Invalid entry, try again-------\033[0m" << endl;
                    continue;
                }
                // Add the updated record back to updatedRecords
                string updatedRecord;
                for (size_t i = 0; i < tokens.size(); ++i)
                {
                    updatedRecord += tokens[i];
                    if (i < tokens.size() - 1) updatedRecord += "/";
                }
                updatedRecords.push_back(updatedRecord);
            }
        }
        // After processing all records, show this message once if no pending leaves were found
        if (!foundPendingLeave) {
            cout << "\033[33m--- No Pending Leaves for approval/rejection.---\033[0m" << endl;
        }

        // If any leave was modified (approved/rejected), update the file
        if (fileUpdated) {
            updateLeaveRecords(filename, updatedRecords);  // Update DirectorApproval.txt
            cout << "\n\033[33mLeave processing completed.\033[0m" << endl;

            // Now, update the main LeaveRecords.txt file to reflect the changes
            updateLeaveRecordsInMainFile("LeaveRecords.txt", "DirectorApproval.txt"); // Sync the records
        }
        else {
            cout << "\033[33m--- No changes made to the leave records. File remains unchanged. ---\033[0m" << endl;
        }
    }
};
//class for the leave type Casual.
class CasualLeave : public Leave 
{
public:
//default + parameterized constructor.
    CasualLeave(int id, const string& start, const string& end, const string& reason,
        const string& approval, const string& address, const string& application_date)
        : Leave(id, start, end, reason, approval, address, application_date) {
        leave_type = "CasualLeave";
    }

    void displayDetails() const override //ovveride due to virtual
    {
        Leave::displayDetails();  // Use shared logic
        cout << "\033[33mNote: Casual Leave allows up to 4 days at a time without prior approval.\033[0m\n";
    }
};
//class for leave type Official
class OfficialLeave : public Leave
{
public:
	//default + parameterized constructor.
    OfficialLeave(int id, const string& start, const string& end, const string& reason,
        const string& approval, const string& address, const string& application_date)
        : Leave(id, start, end, reason, approval, address, application_date) {
        leave_type = "OfficialLeave";
    }
    //ovveride due to virtual
    void displayDetails() const override {
        Leave::displayDetails(); // Call the base class's shared display logic
        cout << "\033[33mNote: Official Leave is granted for work-related travel.\033[0m\n";
    }
};
//class for leave type Unpaid
class UnpaidLeave : public Leave
{
public:
//default + parameterized constructor.
    UnpaidLeave(int id, const string& start, const string& end, const string& reason,
        const string& approval, const string& address, const string& application_date)
        : Leave(id, start, end, reason, approval, address, application_date) 
        {
        leave_type = "UnpaidLeave";
    }
    //ovveride due to virtual
    void displayDetails() const override {
        Leave::displayDetails(); // Call the base class's shared display logic
        cout << "\033[33mNote: Unpaid Leave requires prior approval and results in no salary during the leave period.\033[0m\n";
    }
};
// class for leave type Earned 
class EarnedLeave : public Leave {
public:
	//default + parameterized constructor.
    EarnedLeave(int id, const string& start, const string& end, const string& reason,
        const string& approval, const string& address, const string& application_date)
        : Leave(id, start, end, reason, approval, address, application_date) {
        leave_type = "EarnedLeave";
    }
     //ovveride due to virtual, will be callled for this class only if its obj calls, 
    void displayDetails() const override {
        Leave::displayDetails();  // Use shared logic
        cout << "\033[33mNote: Earned Leave requires prior approval for leaves exceeding 4 days.\033[0m\n";
    }
};
//class for Leave types made by using factory pattern. 
//done dynamically. this helps in a way that instead of manually entering a new kind of type of leave we could just automatically make a new worker dunamically.
class LeaveTypesFactory
{
public:
    static Leave* createLeave(const string& leaveType, int id, const string& start, const string& end, const string& reason, const string& approval, const string& address, const string& applicationDate)
    {
        if (leaveType == "CasualLeave")
            return new CasualLeave(id, start, end, reason, approval, address, applicationDate);
        else if (leaveType == "EarnedLeave")
            return new EarnedLeave(id, start, end, reason, approval, address, applicationDate);
        else if (leaveType == "OfficialLeave")
            return new OfficialLeave(id, start, end, reason, approval, address, applicationDate);
        else if (leaveType == "UnpaidLeave")
            return new UnpaidLeave(id, start, end, reason, approval, address, applicationDate);
        else
            throw invalid_argument("Invalid leave type");
    }
};
//class for handling Reports Genertion and display 
class ReportManager
{
public:
    static void generateEmpReport(vector<Worker>& employees, const string& inputfile, const string& outputfile) //to display all employee reports 
    {
        ifstream file(inputfile);
        if (!file.is_open())//check 
        {
            cerr << "Error: Could not open " << inputfile << endl;
            return;
        }
        //traversing through all the file
        for (size_t i = 0; i < employees.size(); ++i)
        {
            Worker emp = employees[i];
            string in, out;
            float presence = 0;
            string line;

            while (getline(file, line))//line by line 
            {
                stringstream ss(line);//only handles string 
                string timing, id, status;
                out.clear();

                getline(ss, timing, '/');//get timings of current 
                getline(ss, id, '/');// the id 
                getline(ss, status, '/');//if presenf or not 

                if (id == emp.getID())//if id is same as that entered
                {
                    if (status == "Sign-In")//in case of sign in by guard of an employee 
                        in = timing;
                    if (status == "Sign-Out")// in case df sign out by the guard of an employee 
                        out = timing;

                    if (!in.empty() && !out.empty())  //if already sign-in & sign-out 
                    {
                        presence += calculateTimeDifference(in, out);//get the current time.
                        in.clear();// now be prepeared for new employee 
                        out.clear();
                    }
                }
            }
            float perPresence = (presence / 40) * 100;
            saveReporttoFile(emp.getID(), presence, perPresence, outputfile);//add to file and display 
            file.clear();
            file.seekg(0); // Reset the file for the next employee
        }
        file.close();
        cout << "\033[33m---Employee report generated successfully.---\033[0m" << endl;//display message for the guard to know on console
    }
    static void displayEmpReport(vector<Worker>& employees, const string& reportFile)
    {
        //first opens the EmpReport file, extracts only th employees that have less than 80% attendance
        vector<string> line = FileOperationsHandler::readfromthefile(reportFile);

        // Check if the file is empty by trying to read the first character
        if (line.empty())
        {
            // File is empty
            cout << "\033[33m-------Generate report first---------\033[0m" << endl;
        }
        else
        {
            line = FileOperationsHandler::parseLine(line);//line by line 
            for (int i = 0; i < line.size(); i = i + 4)
            {
                string empID, att, perAtt, date;
                float attendance, perAttendance;

                empID = line[i];
                att = line[i + 1];
                perAtt = line[i + 2];
                date = line[i + 3];

                // Convert string to float
                attendance = stof(att);
                perAttendance = stof(perAtt);

                for (Worker& emp : employees)
                {
                    if (emp.getID() == empID)
                    {
                        // Output employee details (ID, Name, Position)
                        cout << "\033[33mEmployee ID: \033[0m" << emp.getID() << endl;
                        cout << "\033[33mName: \033[0m" << emp.getName() << endl;
                        cout << "\033[33mPosition: \033[0m" << emp.getPosition() << endl;
                        cout << "\033[33mAttendance:\033[0m " << attendance << " \033[33mhours\033[0m" << endl;
                        cout << "\033[33mAttendance Percentage:\033[0m " << perAttendance << "\033[33m%\033[0m" << endl;
                        cout << "\033[33mDate report was generated: \033[0m" << date << endl << endl;
                        break;
                    }
                }
            }
        }
    }
    static void showLess80Attendance(vector<Worker>& employees, const string& reportFile)
    {
        //first opens the EmpReport file, extracts only th employees that have less than 80% attendance
        vector<string>line = FileOperationsHandler::readfromthefile(reportFile);
        // Checks if the file is empty by trying to read the first character
        if (line.empty())
        {
            // File is empty
            cout << "\033[33m-----Generate report first----\033[0m" << endl;
        }
        else
        {
            line = FileOperationsHandler::parseLine(line);//line by line 
            for (int i = 0; i < line.size(); i = i + 4)
            {
                string empID, att, perAtt, date;
                float attendance, perAttendance;

                empID = line[i];
                att = line[i + 1];
                perAtt = line[i + 2];
                date = line[i + 3];

                // Convert string to float
                attendance = stof(att);
                perAttendance = stof(perAtt);//percantage of attendance 

                // If attendance is less than 80%, find the employee details
                if (perAttendance < 80.0)
                {
                    for (Worker& emp : employees)
                    {
                        if (emp.getID() == empID)
                        {
                            // Output employee details (ID, Name, Position)
                            cout << "\033[33mEmployee ID:\033[0m " << emp.getID() << endl;
                            cout << "\033[33mName: \033[0m" << emp.getName() << endl;
                            cout << "\033[33mPosition: \033[0m" << emp.getPosition() << endl;
                            cout << "\033[33mAttendance: \033[0m" << attendance << " \033[33mhours\033[0m" << endl;
                            cout << "\033[33mAttendance Percentage:\033[0m " << perAttendance << "\033[33m%\033[0m" << endl;
                            cout << "\033[33mDate report was generated: \033[0m" << date << endl << endl;
                            break;
                        }
                    }
                }
            }
        }
    }
    static float calculateTimeDifference(const string& time1, const string& time2)//to see what time they sign-in and what time they signed out. the employeee must complete a time limit per week 
    {
        int hour1, minute1, second1;
        int hour2, minute2, second2;

        sscanf_s(time1.c_str(), "%*d-%*d-%*d %d:%d:%d", &hour1, &minute1, &second1);//for hour.minute and seconds all 
        sscanf_s(time2.c_str(), "%*d-%*d-%*d %d:%d:%d", &hour2, &minute2, &second2);

        float totalSeconds1 = hour1 * 3600 + minute1 * 60 + second1;//getting in seconds
        float totalSeconds2 = hour2 * 3600 + minute2 * 60 + second2;//getting in seconds 

        return (totalSeconds2 - totalSeconds1) / 3600.0;//return the time differnece 
    }
    static void saveReporttoFile(const string& empID, float attendance, float perAttendance, const string& filename)//saving the sign in sign out and employee data to the employee file 
    {
        ofstream file(filename, ios::out | ios::app);//to remove the old data and to append the file .
        if (!file.is_open())//check if file can be opened.
        {
            cout << "\033[33m----Error: Unable to open file!---\033[0m" << endl;
            return;
        }

        // Get the current time
        time_t now = time(0);
        tm localtm;//location 
        localtime_s(&localtm, &now);
        char dateBuffer[80];
        strftime(dateBuffer, sizeof(dateBuffer), "%Y-%m-%d", &localtm);

        file << empID << "/" << attendance << "/" << perAttendance << "/" << dateBuffer << endl;//adding to file the employee ID,attendance(sign in and sign out) and percentage of attendance 
        file.close();
        cout << "\033[33m-----Report saved for Employee---\033[0m" << empID << endl;//to display to the user that the change has been made. 
    }
    static void display_leave_counts(const string& filename, const string& employeeID)// display the employee counts of how many leaves have been taken. 
    {
        cout << "\n\033[33m---Employee: \033[0m" << currentUserLogin.getName() << "\033[33m---\033[0m" << endl;//employee name.
        ifstream file(filename);

        if (!file.is_open()) //check for file.
        {
            cout << "\n\033[33m --- Error opening leave counts file! ---\033[0m" << endl;
            return;
        }

        string line;
        bool found = false;
        while (getline(file, line)) //linr by line 
        {
            stringstream ss(line);
            string id;
            int casualLeave, earnedLeave, officialLeave, unpaidLeave;//leave types variables 

            getline(ss, id, '/');
            if (id == employeeID) 
            {
                ss >> casualLeave;
                ss.ignore(1); // Ignore the '/'
                ss >> earnedLeave;
                ss.ignore(1); // Ignore the '/'
                ss >> officialLeave;
                ss.ignore(1); // Ignore the '/'
                ss >> unpaidLeave;
                //display the leave counts 
                cout << "\033[33mEmployeeID:\033[0m " << id << endl;
                cout << "\033[33mCasual Leave Count:\033[0m " << casualLeave << endl;
                cout << "\033[33mEarned Leave Count:\033[0m " << earnedLeave << endl;
                cout << "\033[33mOfficial Leave Count: \033[0m" << officialLeave << endl;
                cout << "\033[33mUnpaid Leave Count:\033[0m " << unpaidLeave << endl;
                cout << "\033[33m-----------------------------\033[0m" << endl;
                found = true;
                break;//skip and continue 
            }
        }

        if (!found) {//in case not found 
            cout << "\n\033[33m--- No leave counts found for Employee ID: \033[0m" << employeeID << "\033[33m---\033[0m" << endl;
        }

        file.close();//close file 
    }
    static void display_entire_leave_records(const vector<Leave*>& leave_records, const string& leave_counts_file)// to display the entire leave records of the user. 
    {
        bool Ofound = false;//bool to check if found 
        for (size_t i = 0; i < leave_records.size(); ++i) 
        {
            if (to_string(leave_records[i]->getLeaveId()) == currentUserLogin.getEmployeeID())//check if the id is same in file and what is given by 
            {
                leave_records[i]->display();//call the display.
                Ofound = true;
            }
        }

        if (!Ofound) 
        {//if not found 
            cout << "\n\033[33m--- No leave records found for Employee ID:\033[0m " << currentUserLogin.getEmployeeID() << " ---" << endl;
        }
        cout << "\n\033[33m --- No of Leaves taken is---\033[0m\n ";
        display_leave_counts(leave_counts_file, currentUserLogin.getEmployeeID());// call to display the user 
    }
    static void display_outstanding_leave_counts(const string& filename, const string& employeeID)// check to display outstanding leave counts as report 
    {
        ifstream file(filename);

        if (!file.is_open())//if file can be opened 
        {
            cout << "\n\033[33m --- Error opening leave counts file! ---\033[0m" << endl;
            return;
        }
        const int MaxCasualLeave = 21;//varaibles storing the max Leaves counts will be used for comaprison 
        const int MaxEarnedLeave = 15;
        const int MaxOfficialLeave = 10;
        const int MaxUnpaidLeave = 30;
        string line;
        bool leaveFound = false;//bool to keep count 
        while (getline(file, line)) //line by line 
        {
            stringstream ss(line);
            string id;
            int casualLeave, earnedLeave, officialLeave, unpaidLeave;

            getline(ss, id, '/');
            if (id == employeeID) {
                ss >> casualLeave;
                ss.ignore(1); // Ignore the '/'
                ss >> earnedLeave;
                ss.ignore(1); // Ignore the '/'
                ss >> officialLeave;
                ss.ignore(1); // Ignore the '/'
                ss >> unpaidLeave;
                // to see the no of leaves left for the user to take 
                int outstandingCasualLeave = MaxCasualLeave - casualLeave;
                int outstandingEarnedLeave = MaxEarnedLeave - earnedLeave;
                int outstandingOfficialLeave = MaxOfficialLeave - officialLeave;
                int outstandingUnpaidLeave = MaxUnpaidLeave - unpaidLeave;
                //display the no of levaes left to the user. 
                cout << "\033[33mEmployeeID:\033[0m " << id << endl;
                cout << "\033[33mOutstanding Casual Leave:\033[0m " << outstandingCasualLeave << endl;
                cout << "\033[33mOutstanding Earned Leave:\033[0m " << outstandingEarnedLeave << endl;
                cout << "\033[33mOutstanding Official Leave:\033[0m" << outstandingOfficialLeave << endl;
                cout << "\033[33mOutstanding Unpaid Leave:\033[0m " << outstandingUnpaidLeave << endl;
                cout << "\033[33m-----------------------------\033[0m" << endl;
                leaveFound = true;//change the variable. 
                break;//skip 
            }
        }

        if (!leaveFound)//if not found 
        {
            cout << "\n\033[33m--- No outsanding leaves found for Employee ID:\033[0m " << employeeID << "\033[33m\033[0m" << endl;
        }

        file.close();//close file 
    }
};

//Command Pattern
class ICommand
{
    //interface for the command pattern
public:
    virtual void execute() = 0;
    virtual ~ICommand() = default;
};
class LeaveActionInvoker
{
    //invoker lclass meaning triggers the execute function in interface
private:
    ICommand* command;
public:
    //setter
    void setCommand(ICommand* cmd)
    {
        command = cmd;
    }
    void executeCommand()
    {
        if (command) 
        {
            command->execute();
        }
    }
};
class LeaveManager; // Forward declaration because of circular dependency
class SubmitLeaveCommand : public ICommand
{
    //inherits from interface
private:
    LeaveManager& leaveManager;
    vector<Leave*>& leaveRecords;
    vector<vector<int>>& leaveCounts;
public:
    //constructor
    SubmitLeaveCommand(LeaveManager& lm, vector<Leave*>& lr, vector<vector<int>>& lc) : leaveManager(lm), leaveRecords(lr), leaveCounts(lc)
    {}
    void execute() override;
};
class LeaveManager
{
public:
    static bool is_valid_date(const string& date)
    {
        //checks if entered date is valid or not
        int year, month, day;
        char dash1, dash2;
        stringstream ss(date);
        ss >> year >> dash1 >> month >> dash2 >> day;
        if (dash1 != '-' || dash2 != '-' || month < 1 || month > 12 || day < 1 || day > 31)
        {
            return false;
        }
        if ((month == 2 && day > 28) || (month == 4 || month == 6 || month == 9 || month == 11) && day > 30)
        {
            return false;
        }

        return true;
    }
    static bool is_end_date_after_start_date(const string& start_date, const string& end_date)
    {
        //checks if end date entered is after the start date entered
        return end_date >= start_date;
    }
    static int date_difference_in_days(const string& start_date, const string& end_date)
    {
        //calculates the leave duration
        tm stt_tm = {};
        tm endd_tm = {};

        // Convert start_date string to tm
        istringstream ss_start(start_date);
        ss_start >> get_time(&stt_tm, "%Y-%m-%d");
        if (ss_start.fail())
        {
            cerr << "Invalid start date format: " << start_date << endl;
            return -1; // Error handling
        }

        // Convert end_date string to tm
        istringstream ss_end(end_date);
        ss_end >> get_time(&endd_tm, "%Y-%m-%d");
        if (ss_end.fail()) {
            cerr << "Invalid end date format: " << end_date << endl;
            return -1; // Error handling
        }

        // Convert tm to time_t
        time_t beginning_time = mktime(&stt_tm);
        time_t finish_time = mktime(&endd_tm);

        if (beginning_time == -1 || finish_time == -1) {
            cerr << "Error converting dates to time_t." << endl;
            return -1; // Error handling
        }
        const int sec_per_day = 60 * 60 * 24;
        return static_cast<int>((finish_time - beginning_time) / sec_per_day);
    }
    //pattern : EmployeeId/StartDate/EndDate/Reason/Status/Type/Location/ApplicationDate
    static vector<Leave*> read_leave_records(const string& filename)
    {
        // Read leave records from a file
        set<string> unique_records;
        vector<Leave*> leave_records;

        vector<string> lines = FileOperationsHandler::readfromthefile(filename); // Use FileHandler
        for (const auto& line : lines)
        {
            if (unique_records.find(line) != unique_records.end())
                continue;

            unique_records.insert(line);
            vector<string> tokens = FileOperationsHandler::parseLine({ line }); // Pass the line as a vector of one string
            if (tokens.size() < 8)
                continue;

            int id = stoi(tokens[0]);
            const string& start_date = tokens[1];
            const string& end_date = tokens[2];
            const string& reason = tokens[3];
            const string& approval_status = tokens[4];
            const string& leave_type = tokens[5];
            const string& address = tokens[6];
            const string& application_date = tokens[7];

            //creates new object for the type of leave being sent by worker
            Leave* new_leave = nullptr;
            if (leave_type == "UnpaidLeave")
                new_leave = new UnpaidLeave(id, start_date, end_date, reason, approval_status, address, application_date);
            else if (leave_type == "CasualLeave")
                new_leave = new CasualLeave(id, start_date, end_date, reason, approval_status, address, application_date);
            else if (leave_type == "EarnedLeave")
                new_leave = new EarnedLeave(id, start_date, end_date, reason, approval_status, address, application_date);
            else if (leave_type == "OfficialLeave")
                new_leave = new OfficialLeave(id, start_date, end_date, reason, approval_status, address, application_date);
            if (new_leave)
                leave_records.push_back(new_leave);
        }
        return leave_records;
    }
    // Save leave records to a file
    //file format: EmployeeID/CasualLeaveCount/EarnedLeaveCount/OfficialLeaveCount/UnpaidLeaveCount
    static void save_leave_records(const string& filename, const vector<Leave*>& leave_records) 
    {
        set<string> unique_records;
        vector<string> lines;

        for (const auto& leave : leave_records) 
        {
            //for formatting the line by separating different entites using the delimtier "/"
            string record = FileOperationsHandler::formatLine(
                {
                to_string(leave->getLeaveId()),
                leave->getStartDate(),
                leave->getEndDate(),
                leave->getReason(),
                leave->getApprovalStatus(),
                leave->getLeaveType(),
                leave->getAddress(),
                leave->getApplicationDate()
                });

            if (unique_records.find(record) == unique_records.end()) {
                unique_records.insert(record);
                lines.push_back(record);
            }
        }

        FileOperationsHandler::writetothefile(filename, lines); // Use FileHandler for writing
        cout << "\n\033[33m--- Leave records saved to file. ---\033[0m" << endl;
    }
    // Read leave counts from a file
    static vector<vector<int>> read_leave_counts(const string& filename) 
    {
        //reads the leave counts file using file handling
        vector<vector<int>> leave_counts;
        ifstream file(filename);

        if (!file.is_open()) 
        {
            cout << "\n\033[33m--- Error opening leave counts file! ---\033[0m" << endl;
            return leave_counts;
        }
        string line;
        while (getline(file, line)) 
        {
            stringstream ss(line);
            string emp_id, casual, earned, official, unpaid;
            getline(ss, emp_id, '/');
            getline(ss, casual, '/');
            getline(ss, earned, '/');
            getline(ss, official, '/');
            getline(ss, unpaid, '/');

            //try-catch exception handling to avoid bugs
            try 
            {
                leave_counts.push_back({ stoi(emp_id), stoi(casual), stoi(earned), stoi(official), stoi(unpaid) });
            }
            catch (const invalid_argument&) 
            {
                cout << "\033[33mInvalid argument in line:\033[0m " << line << endl;
            }
            catch (const out_of_range&) 
            {
                cout << "\033[33mOut of range in line:\033[0m " << line << endl;
            }
        }

        file.close();
        return leave_counts;
    }
    // Save leave counts to a file
    static void saveLeaveCounts(const string& filename, const vector<vector<int>>& leave_counts) 
    {
        //saves the leavecounts into the file
        ofstream file(filename);
        if (!file.is_open())
        {
            cout << "\n\033[33m--- Error opening leave counts file! ---\033[0m" << endl;
            return;
        }
        //formatting the line by using delimiter
        for (const auto& entry : leave_counts) 
        {
            file << entry[0] << "/" << entry[1] << "/" << entry[2] << "/" << entry[3] << "/" << entry[4] << endl;
        }

        file.close();
        cout << "\n\033[33m-----------------------------\033[0m\n" << endl;
        cout << "\n\033[33m--- Leave counts saved to file. ---\033[0m" << endl;
    }
    // Update leave count for an employee
    static void updateLeaveCount(vector<vector<int>>& leave_counts, int emp_id, const string& leave_type) 
    {
        //leave counts gets incremented accordingly
        bool found = false;
        for (auto& entry : leave_counts)
        {
            if (entry[0] == emp_id)
            {
                found = true;
                if (leave_type == "CasualLeave")
                    entry[1]++;
                else if (leave_type == "EarnedLeave")
                    entry[2]++;
                else if (leave_type == "OfficialLeave")
                    entry[3]++;
                else if (leave_type == "UnpaidLeave")
                    entry[4]++;
                break;
            }
        }

        if (!found) 
        {
            vector<int> new_entry = { emp_id, 0, 0, 0, 0 };
            if (leave_type == "CasualLeave")
                new_entry[1] = 1;
            else if (leave_type == "EarnedLeave")
                new_entry[2] = 1;
            else if (leave_type == "OfficialLeave")
                new_entry[3] = 1;
            else if (leave_type == "UnpaidLeave")
                new_entry[4] = 1;

            leave_counts.push_back(new_entry);
        }
    }
    static void add_leave_record(vector<Leave*>& leave_records, vector<vector<int>>& leave_counts)
    {
        //entering all leave details
        string start_date, end_date, reason, leave_type, address, application_date;
        cout << "\033[35mEnter Start Date (YYYY-MM-DD): \033[0m";
        cin >> start_date;
        cout << "\033[35mEnter End Date (YYYY-MM-DD):\033[0m ";
        cin >> end_date;
        cout << "\033[35mEnter Reason for Leave:\033[0m";
        cin.ignore();
        getline(cin, reason);
        cout << "\033[35m Enter Leave Type (Casual, Earned, Official, Unpaid):\033[0m ";
        cin >> leave_type;
        cout << "\033[35m Enter Address: \033[0m";
        cin.ignore();
        getline(cin, address);
        cout << "\033[35m Enter Application Date (YYYY-MM-DD):\033[0m ";
        cin >> application_date;
        cout << endl;
        if (leave_type != "Casual" && leave_type != "Earned" && leave_type != "Official" && leave_type != "Unpaid")
        {
            cout << "\033[33mInvalid leave type entered. Please choose one of the following: Casual, Earned, Official, Unpaid\033[0m." << endl;
            return;  // Return early if the input is invalid
        }
        Leave* new_leave = LeaveTypesFactory::createLeave(leave_type + "Leave", stoi(currentUserLogin.getEmployeeID()),start_date, end_date, reason, "Pending", address, application_date);
        leave_records.push_back(new_leave);
        save_leave_records("LeaveRecords.txt", leave_records);
        updateLeaveCount(leave_counts, stoi(currentUserLogin.getEmployeeID()), leave_type + "Leave");
        saveLeaveCounts("LeavesCount.txt", leave_counts);

        cout << "\033[33mLeave request added successfully!\033[0m" << endl;
        cout << "\n\033[33m---------------------------------------\033[0m\n" << endl;
    }
    static void display_leave_records(const vector<Leave*>& leave_records)
    {
        //displays the leaves of the employee
        cout << "\n\033[33m---Employee:\033[0m " << currentUserLogin.getName() << "\033[33m---\033[0m" << endl;
        if (leave_records.empty())
        {
            cout << "\033[33mNo leave records found.\033[0m" << endl;
            return;
        }

        for (size_t i = 0; i < leave_records.size(); ++i)
        {
            leave_records[i]->display(); // Polymorphic call
        }
    }
    static void leave_menu(vector<Worker>& employees)
    {
        // Filenames for leave records and counts
        string leave_records_filename = "LeaveRecords.txt";
        string leave_counts_filename = "LeavesCount.txt";

        // Read leave records and counts
        vector<Leave*> leave_records = LeaveManager::read_leave_records(leave_records_filename);
        vector<vector<int>> leave_counts = LeaveManager::read_leave_counts(leave_counts_filename);

        // Setup the Command Pattern
        LeaveManager leaveManager;
        LeaveActionInvoker invoker;
        SubmitLeaveCommand submitCommand(leaveManager, leave_records, leave_counts);

        int choice;
        do {
            //menu is displayed
            cout << "\n\033[33m--- Leave Portal ---\033[0m\n";
            cout << "\033[33m1. See Complete Leaves Report\033[0m\n";
            cout << "\033[33m2. Apply for new Leave\033[0m\n";
            cout << "\033[33m3. See Outsanding Leaves Report\033[0m\n";
            cout << "\033[33m4. Back\033[0m\n";
            cout << "\033[35m Enter your choice:\033[0m";
            cin >> choice;
            switch (choice)
            {
            case 1:
                ReportManager::display_entire_leave_records(leave_records, leave_counts_filename);
                break;
            case 2:
                cout << "\033[33m--------------------------------------------------------------------------------------------\033[0m" << endl;
                cout << "\033[33m--- Employees can apply for the following leaves: ---\033[0m" << endl;
                cout << "\033[33m-> Casual Leave (up to 15 days / year, max 4 days at a time, no prior approval needed)\033[0m";
                cout << "\n\033[33m-> Earned Leave (up to 21 days/year, prior approval required for more than 4 days)\033[0m";
                cout << "\n\033[33m-> Official Leave (for work-related travel, full-day credit will be awarded)\033[0m";
                cout << "\n\033[33m-> Unpaid Leave (for extended periods, prior approval required, no salary given during leave)\033[0m";
                cout << "\n\033[33m--- Select the leave type, fill in details, and submit your application ---\033[0m" << endl;
                cout << "\033[33m---------------------------------------------------------------------------------------------\033[0m" << endl;
                invoker.setCommand(&submitCommand); // Use the SubmitLeaveCommand for leave submission
                invoker.executeCommand();
                break;
            case 3:
                ReportManager::display_outstanding_leave_counts(leave_counts_filename, currentUserLogin.getEmployeeID());
                break;
            case 4:
                cout << "\033[33m----Back... -----\033[0m" << endl;
                break;
            default:
                cout << "\033[33mInvalid choice! Please try again.\033[0m\n";
            }
        } while (choice != 4);
        for (size_t i = 0; i < leave_records.size(); ++i)
        {
            delete leave_records[i];
        }
    }
};
void SubmitLeaveCommand::execute()
{
    leaveManager.add_leave_record(leaveRecords, leaveCounts);
}
class ApproveLeaveCommand : public ICommand
{
private:
    Supervisor& supervisor;
    string filename;
public:
    ApproveLeaveCommand(Supervisor& sv, const string& fn) : supervisor(sv), filename(fn)
    {}
    void execute() override
    {
        supervisor.approveRejectLeave(filename);
    }
};
class RejectLeaveCommand : public ICommand
{
private:
    Supervisor& supervisor;
    string filename;
public:
    RejectLeaveCommand(Supervisor& sv, const string& fn) : supervisor(sv), filename(fn) {}
    void execute() override
    {
        supervisor.approveRejectLeave(filename);
    }
};

class AttendanceManager
{
public:
    static void CheckAttendance(const string& filename)
    {
        vector<string> line;
        //reading from file
        line = FileOperationsHandler::readfromthefile(filename);

        bool found = false;
        line = FileOperationsHandler::parseLine(line);
        for (int i = 0; i < line.size(); i = i + 3)
        {
            string timing, id, status;
            timing = line[i];
            id = line[i + 1];
            status = line[i + 2];

            if (id == currentUserLogin.getEmployeeID())
            {
                found = true;
                if (status == "Sign-In")
                {
                    cout << "\033[33mSign-in Time: \033[0m" << timing << endl;
                }
                if (status == "Sign-Out")
                {
                    cout << "\033[33mSign-out Time:\033[0m " << timing << endl;
                }
                cout << endl;
            }
        }
        if (!found)
        {
            cout << "\033[33m--- No attendance record found for Employee ID: \033[0m" << currentUserLogin.getEmployeeID() << "\033[33m---\033[0m" << endl;
        }
    }
};
class LoginManager
{
public:
    static bool validSignIn(string empID, Worker* emp, vector<Worker>& employees)
    {
        for (size_t i = 0; i < employees.size(); ++i)
        {
            if (employees[i].getID() == empID)
            {
                //if emp has previously signed-in and signed-out, it needs to be updated
                if (!employees[i].getOutTime().empty() && !employees[i].getInTime().empty())
                {
                    employees[i].setInTime("");
                    employees[i].setOutTime("");
                    *emp = move(employees[i]);
                    return true;
                }
                // if emp not signed-in and signed-out
                else if (employees[i].getInTime().empty() && employees[i].getOutTime().empty())
                {
                    *emp = move(employees[i]);
                    return true;
                }
                else if (employees[i].getInTime().empty() && !employees[i].getOutTime().empty())
                {
                    cout << "\n\033[33m--- Employee hasn't signed-in but has signed-out ---\n\n" << endl;
                }
                else if (!employees[i].getInTime().empty() && employees[i].getOutTime().empty())
                {
                    cout << "\n\033[33m--- Employee has signed-in already ---\033[0m" << endl;
                }
                cout << "\n\033[33m--- Employee has not signed out yet ---\033[0m\n" << endl;
                cout << "\n\033[33m---------------------------------------\033[0m\n" << endl;
                return false;
            }
        }
        return false;
    }
    static bool validSignOut(string empID, Worker* emp, vector<Worker>& employees)
    {
        for (size_t i = 0; i < employees.size(); ++i)
        {
            if (employees[i].getID() == empID)
            {
                //if emp signed-in but hasn't signed-out yet
                if (employees[i].getOutTime().empty() && !employees[i].getInTime().empty())
                {
                    *emp = move(employees[i]);
                    return true;
                }
                else if (!employees[i].getOutTime().empty())
                {
                    cout << "\n\033[33m--- Employee has already signed-out--- \033[0m" << endl;
                }
                else if (employees[i].getOutTime().empty() && employees[i].getInTime().empty())
                {
                    cout << "\n\033[33m--- Employee hasn't signed-in yet---\033[0m " << endl;
                }
                return false;
            }
        }
        return false;
    }
    static bool loginVerification(string un, string p, vector<Worker>& employees);
};

class GuardFunctionalityHandler
{
    //has the guard menu
public:
    static void guardMenu(Guard& guard, vector<Worker>& employees)
    {
        LeaveManager leaveManager;
        LeaveActionInvoker invoker;

        // Read leave records and counts
        vector<Leave*> leaveRecords = LeaveManager::read_leave_records("LeaveRecords.txt");
        vector<vector<int>> leaveCounts = LeaveManager::read_leave_counts("LeavesCount.txt");

        // Create command objects
        SubmitLeaveCommand submitCommand(leaveManager, leaveRecords, leaveCounts);

        int menu;
        bool status = true;

        while (status)
        {
            //displays the menu
            cout << "\n\n\n\033[33m--- Guard Dashboard ---\033[0m" << endl;
            cout << "\033[33m1) Mark Sign-in Attendance\033[0m" << endl;
            cout << "\033[33m2) Mark Sign-out Attendance\033[0m" << endl;
            cout << "\033[33m3) Leave Portal\033[0m" << endl;
            cout << "\033[33m4) Check Attendance\033[0m" << endl;
            cout << "\033[33m5) Generate Employee Report\033[0m" << endl;
            cout << "\033[33m6) Display Employee Report\033[0m" << endl;
            cout << "\033[33m7) Show less than 80% Attendance Employees\033[0m" << endl;
            cout << "\033[33m8) Exit\033[0m" << endl;
            cout << "\033[35mEnter your choice (1-8):\033[0m ";
            cin >> menu;

            switch (menu)
            {
            case 1:
                handleSignIn(guard, employees);
                break;
            case 2:
                handleSignOut(guard, employees);
                break;
            case 3:
                LeaveManager::leave_menu(employees);
                break;
            case 4:
                AttendanceManager::CheckAttendance("Attendance.txt");
                break;
            case 5:
                ReportManager::generateEmpReport(employees, "Attendance.txt", "EmployeeReport.txt");
                break;
            case 6:
                ReportManager::displayEmpReport(employees, "EmployeeReport.txt");
            case 7:
                ReportManager::showLess80Attendance(employees, "EmployeeReport.txt");
                break;
            case 8:
                cout << "\n\033[33mExiting Guard Menu...\033[0m\n";
                status = false;
                break;
            default:
                cout << "\033[33mInvalid input. Please enter a number between 1 and 7.\033[0m" << endl;
                break;
            }
        }
        for (Leave* leave : leaveRecords)
        {
            delete leave;
        }
    }
private:
    static void handleSignIn(Guard& guard, vector<Worker>& employees)
    {
        //if emp hasnt singed in yet or hasnt signed out yet then dont make him sign in.
        string empID;
        cout << "\033[35m Enter Employee ID:\033[0m ";
        cin >> empID;

        size_t empIndex = findEmployee(employees, empID);
        if (empIndex != employees.size() && LoginManager::validSignIn(empID, &employees[empIndex], employees))
        {
            //marks and saves the attendance into the file
            guard.markEmpAttendance(&employees[empIndex], "Sign-In");
            guard.saveAttendanceToFile("Attendance.txt");
        }
    }
    static void handleSignOut(Guard& guard, vector<Worker>& employees)
    {
        //if emp hasnt signed in yet or signing out twice
        string empID;
        cout << "\033[35m Enter Employee ID: \033[0m";
        cin >> empID;

        size_t empIndex = findEmployee(employees, empID);
        if (empIndex != employees.size() && LoginManager::validSignOut(empID, &employees[empIndex], employees))
        {
            //marks and saves the attendance to the file
            guard.markEmpAttendance(&employees[empIndex], "Sign-Out");
            guard.saveAttendanceToFile("Attendance.txt");
        }
    }
    static size_t findEmployee(vector<Worker>& employees, const string& empID)
    {
        //validates if the emp id entered by the guard for marking attendace exists or not
        for (size_t i = 0; i < employees.size(); ++i)
        {
            if (employees[i].getID() == empID)
                return i;
        }
        return employees.size();
    }
};
class EmployeeFunctionalityHandler
{
public:
    static void EmployeeMenu(vector<Worker>& employees)
    {
        //emp menu 
        LeaveManager leaveManager;
        LeaveActionInvoker invoker;

        // Read leave records and counts
        vector<Leave*> leaveRecords = LeaveManager::read_leave_records("LeaveRecords.txt");
        vector<vector<int>> leaveCounts = LeaveManager::read_leave_counts("LeavesCount.txt");

        // Create command objects
        SubmitLeaveCommand submitCommand(leaveManager, leaveRecords, leaveCounts);

        int menu = 0;
        bool status = true;

        while (status)
        {
            //displays the emp menu
            cout << "\n\033[33m--- Employee Dashboard ---\033[0m" << endl;
            cout << "\033[33m1) Leave Portal\033[0m" << endl;
            cout << "\033[33m2) Check Attendance\033[0m" << endl;
            cout << "\033[33m3) Generate Employee Report\033[0m" << endl;
            cout << "\033[33m4) Display Employee Report\033[0m" << endl;
            cout << "\033[33m5) Show less than 80% Attendance Employees\033[0m" << endl;
            cout << "\033[33m6) Exit\033[0m" << endl;
            cout << "\033[35m Enter your choice (1-6):\033[0m ";

            if (cin >> menu)
            {
                if (menu == 1)
                {
                    LeaveManager::leave_menu(employees);
                }
                else if (menu == 2)
                {
                    AttendanceManager::CheckAttendance("Attendance.txt");
                }
                else if (menu == 3)
                {
                    //generate report for all employees
                    ReportManager::generateEmpReport(employees, "Attendance.txt", "EmployeeReport.txt");
                }
                else if (menu == 4)
                {
                    ReportManager::displayEmpReport(employees, "EmployeeReport.txt");
                }
                else if (menu == 5)
                {
                    ReportManager::showLess80Attendance(employees, "EmployeeReport.txt");
                }
                else if (menu == 6)
                {
                    cout << "\033[33mExiting program...\033[0m" << endl;
                    exit(0);
                    status = false;
                }
                else
                {
                    cout << "\033[33mInvalid Option entered. Please choose between 1 and 6.\033[0m" << endl;
                }
            }
            else
            {
                cout << "\033[33mInvalid input. Please enter a number between 1 and 6.\033[0m" << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        }
        // Cleanup leave records
        for (Leave* leave : leaveRecords)
        {
            delete leave;
        }
    }
};
class SupervisorFunctionalityHandler
{
public:
    static void SupervisorMenu(Supervisor& supervisor, const string& leaveRecordsFile, vector<Worker>& employees)
    {
        //menu for supervisor
        LeaveManager leaveManager;
        LeaveActionInvoker invoker;

        // Read leave records and counts
        vector<Leave*> leaveRecords = LeaveManager::read_leave_records(leaveRecordsFile);
        vector<vector<int>> leaveCounts = LeaveManager::read_leave_counts("LeavesCount.txt");

        // Create command objects
        SubmitLeaveCommand submitCommand(leaveManager, leaveRecords, leaveCounts);
        ApproveLeaveCommand approveCommand(supervisor, leaveRecordsFile);

        int menu;
        bool status = true;
        while (status)
        {
            //displaying the supervisor menu
            cout << "\n\033[33m--- Supervisor Dashboard ---\033[0m" << endl;
            cout << "\033[33m1) Approve/Disapprove Pending Leaves\033[0m" << endl;
            cout << "\033[33m2) Leave Portal\033[0m" << endl;
            cout << "\033[33m3) Check Attendance\033[0m" << endl;
            cout << "\033[33m4) Generate Employee Report\033[0m" << endl;
            cout << "\033[33m5) Display Employee Report\033[0m" << endl;
            cout << "\033[33m6) Show less than 80% Attendance Employees\033[0m" << endl;
            cout << "\033[33m7) Exit\033[0m" << endl;
            cout << "\033[35m Enter your choice (1-7):\033[0m ";
            cin >> menu;

            if (menu == 1)
            {
                cout << "\n\033[33m--- Approve/Disapprove Pending Leaves ---\033[0m" << endl;
                invoker.setCommand(&approveCommand); // Approve or reject leaves
                invoker.executeCommand();
                break;
            }
            else if (menu == 2)
            {
                LeaveManager::leave_menu(employees);
            }
            else if (menu == 3)
            {
                AttendanceManager::CheckAttendance("Attendance.txt");
            }
            else if (menu == 4)
            {
                //generate report for all employees
                ReportManager::generateEmpReport(employees, "Attendance.txt", "EmployeeReport.txt");
            }
            else if (menu == 5)
            {
                ReportManager::displayEmpReport(employees, "EmployeeReport.txt");
            }
            else if (menu == 6)
            {
                ReportManager::showLess80Attendance(employees, "EmployeeReport.txt");
            }
            else if (menu == 7)
            {
                cout << "\n\033[33m!!!--- Exiting Supervisor Menu... ---!!!\033[0m" << endl;
                exit(0);
                status = false;
            }
            else
            {
                //if user enters anything other than 1-7 (can be a char)
                cout << "\033[33mInvalid input. Please enter a number between 1 and 7.\033[0m" << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        }
        // Cleanup leave records
        for (Leave* leave : leaveRecords)
        {
            delete leave;
        }
    }
};
class DirectorFunctionalityHandler
{
public:
    static void DirectorMenu(Director& director, const string& leaveRecordsFile, vector<Worker>& employees)
    {
        //dirctor menu
        int menu;
        bool status = true;

        while (true)
        {
            //displays the menu
            cout << "\n\033[33m--- Director Dashboard ---\033[0m" << endl;
            cout << "\033[33m1) Approve/Reject Pending Leaves\033[0m" << endl;
            cout << "\033[33m2) Leave Portal\033[0m" << endl;
            cout << "\033[33m3) Check Attendance\033[0m" << endl;
            cout << "\033[33m4) Generate Employee Report\033[0m" << endl;
            cout << "\033[33m5) Display Employee Report\033[0m" << endl;
            cout << "\033[33m6) Show less than 80% Attendance Employees\033[0m" << endl;
            cout << "\033[33m7) Exit\033[0m" << endl;
            cout << "\033[35m Enter your choice (1-7):\033[0m ";
            cin >> menu;

            if (menu == 1)
            {
                cout << "\n\033[33m--- Approve/Reject Pending Leaves ---\033[0m" << endl;
                director.approve_reject_leave(leaveRecordsFile);
                break;
            }
            else if (menu == 2)
            {
                LeaveManager::leave_menu(employees);
            }
            else if (menu == 3)
            {
                AttendanceManager::CheckAttendance("Attendance.txt");
            }
            else if (menu == 4)
            {
                //generate report for all employees
                ReportManager::generateEmpReport(employees, "Attendance.txt", "EmployeeReport.txt");
            }
            else if (menu == 5)
            {
                ReportManager::displayEmpReport(employees, "EmployeeReport.txt");
            }
            else if (menu == 6)
            {
                ReportManager::showLess80Attendance(employees, "EmployeeReport.txt");
            }
            else if (menu == 7)
            {
                cout << "\n\033[33m!!!--- Exiting Director Menu... ---!!!\033[0m" << endl;
                exit(0);
                status = false;
            }
            else
            {
                //when user enters anything other than 1-7 (can be char)
                cout << "\033[33mInvalid input. Please enter a number between 1 and 7.\033[0m" << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        }
    }
};
class GuardRole : public Role
{
    //inherits from role class
    Employee* employee;
public:
    //overrides the dashboard in role
    void showDashboard(vector<Worker>& employees) override
    {
        Guard guard;
        GuardFunctionalityHandler::guardMenu(guard, employees);
    }
};
class SupervisorRole : public Role
{
    Employee* employee;  // Pointer to an Employee
public:
    //constructor
    SupervisorRole(Employee* emp) : employee(emp)
    {}
    //dashboard overrides
    void showDashboard(vector<Worker>& employees) override
    {
        Supervisor sv;
        SupervisorFunctionalityHandler::SupervisorMenu(sv, "LeaveRecords.txt", employees);
    }
};
class DirectorRole : public Role
{
    Employee* employee;
public:
    //dashboard gets override to show specific menu
    void showDashboard(vector<Worker>& employees) override
    {
        Director dr;
        DirectorFunctionalityHandler::DirectorMenu(dr, "DirectorApproval.txt", employees);
    }
};
class EmployeeRole : public Role
{
    Employee* employee;     // Link to an Employee object
public:
    //constructor
    EmployeeRole(Employee* emp) : employee(emp)
    {}
    //dashboard overriden to show specific menu
    void showDashboard(vector<Worker>& employees) override
    {
        Employee emp;
        EmployeeFunctionalityHandler::EmployeeMenu(employees);
    }
};
class Menu
{
    //has all the menus for all the worker types
    //calls the specific handler menu according to the position
public:
    static void GuardMenu(Guard& guard, vector<Worker>& employees)
    {
        GuardFunctionalityHandler::guardMenu(guard, employees);
    }
    static void EmployeeMenu(vector<Worker>& employees) 
    {
        EmployeeFunctionalityHandler::EmployeeMenu(employees);
    }
    static void SupervisorMenu(Supervisor& supervisor, const string& leaveRecordsFile, vector<Worker>& employees)
    {
        SupervisorFunctionalityHandler::SupervisorMenu(supervisor, leaveRecordsFile, employees);
    }
    static void DirectorMenu(Director& director, const string& leaveRecordsFile, vector<Worker>& employees) 
    {
        DirectorFunctionalityHandler::DirectorMenu(director, leaveRecordsFile, employees);
    }
};

//Template Method Pattern
class LeaveProcessing 
{
    //abstract class
public:
    //checks for prior conditions for applying leave e.g checks remaining leave
    void processLeave(const string& employeeID)
    {
        gatherDetails(employeeID);
        calculateLeaveDuration();
        if (checkBalance()) 
        {
            approveLeave();
        }
        else 
        {
            rejectLeave();
        }
        finalize();
    }
protected:
    virtual void gatherDetails(const string& employeeID)
    {
        //only displays message of gathering details
        cout << "\033[33mGathering leave details for Employee ID:\033[0m " << employeeID << endl;
    }
    virtual void calculateLeaveDuration()
    {
        cout << "\033[33mCalculating leave duration...\033[0m" << endl;
    }
    virtual bool checkBalance() = 0;
    virtual void approveLeave() 
    {
        cout << "\033[33mLeave approved!\033[0m" << endl;
    }
    virtual void rejectLeave() 
    {
        cout << "\033[33mLeave rejected\033[0m "<< endl;
    }
    virtual void finalize()
    {
        cout << "\033[33mFinalizing leave request process.\033[0m" << endl;
    }
};
class CasualLeaveProcessing : public LeaveProcessing 
{
    //for casual leave processing
protected:
    bool checkBalance() override
    {
        cout << "\033[33mChecking casual leave balance...\033[0m" << endl;
        return true;
    }
};
class UnpaidLeaveProcessing : public LeaveProcessing
{//for UnpaidLeave
protected:
    bool checkBalance() override
    {
        cout << "\033[33mChecking sick leave balance...\033[0m" << endl;
        return false; // Simulated insufficient balance
    }
};
class EarnedLeaveProcessing : public LeaveProcessing 
{
    //inherits from leaveProcessing
protected:
    bool checkBalance() override 
    {
        cout << "\033[33mChecking earned leave balance...\033[0m" << endl;
        return true;
    }
    void approveLeave() override 
    {
        cout << "\033[33mEarned leave approved by Supervisor.\033[0m" << endl;
    }
    void rejectLeave() override
    {
        cout << "\033[33mEarned leave rejected \033[0m" << endl;
    }

};
class OfficialLeaveProcessing : public LeaveProcessing
{
protected:
    void gatherDetails(const string& employeeID) override {
        cout << "\033[33mGathering details for official leave for Employee ID:\033[0m " << employeeID << endl;
    }
    bool checkBalance() override {
        cout << "\033[33mNo balance deduction for official leave.\033[0m" << endl;
        return true; // Official leave doesn't rely on balance
    }
    void approveLeave() override {
        cout << "\033[33mOfficial leave approved by Department Head.\033[0m" << endl;
    }
    void rejectLeave() override 
    {
        cout << "\033[33mOfficial leave rejected\033[0m "<< endl;
    }
};

// for circular dependency between 2 classes
bool LoginManager::loginVerification(string un, string p, vector<Worker>& employees)
{
    //verifies the username and password entered is correct or not
    for (size_t i = 0; i < employees.size(); ++i)
    {
        if (employees[i].getID() == un && employees[i].getPassword() == p)
        {
            currentUserLogin.setEmployeeID(employees[i].getID()); // Set session ID
            currentUserLogin.setName(employees[i].getName());     // Set session name

            cout << "\n\033[33m---WELCOME:\033[0m " << currentUserLogin.getName() << "---" << endl;

            if (employees[i].getPosition() == "Guard")
            {
                //calls the guard menu
                Guard guard;
                GuardFunctionalityHandler::guardMenu(guard, employees);
            }
            else if (employees[i].getPosition() == "Employee")
            {
                //calls the emp menu
                Employee emp;
                EmployeeFunctionalityHandler::EmployeeMenu(employees);
            }
            else if (employees[i].getPosition() == "Supervisor") 
            {
                //calls the supervisor menu
                Supervisor supervisor;
                string leaveRecordsFile = "LeaveRecords.txt";
                SupervisorFunctionalityHandler::SupervisorMenu(supervisor, leaveRecordsFile, employees);
            }
            else if (employees[i].getPosition() == "Director") 
            { //calls the director menu
                Director director;
                string leaveRecordsFile = "DirectorApproval.txt";
                DirectorFunctionalityHandler::DirectorMenu(director, leaveRecordsFile, employees);
            }
            return true;
        }
    }
    return false;
}


int main()
{
    // Load employee data from the file
    vector<Worker> employees;
    vector<string> line;
    //used for reading, parsing in file
    line = FileOperationsHandler::readfromthefile("Employees.txt");
    line = FileOperationsHandler::parseLine(line);

    // Read employee data into the vector (assuming format: Name/UserID/Password/Position)
    for (int i = 0; i < line.size(); i = i + 4)
    {
        //creates objects of employees in the Employees.txt file
        employees.push_back(Worker(line[i], line[i + 1], line[i + 2], line[i + 3]));
    }

    // Login process
    string username, password;
    bool status = true;

    // Start login verification
    while (status)
    {
        cout << "\033[35mEnter username:\033[0m";
        cin >> username;
        cout << "\033[35mEnter password:\033[0m ";
        cin >> password;

        if (LoginManager::loginVerification(username, password, employees))
        {
            status = false;  // Successful login
        }
        else
        {
            cout << "\033[33m--- Login failed, try again ---\033[0m" << endl;
        }
    }

    // After successful login, navigate to the correct menu based on employee's position
    //calls the specific menu based on the position
    if (!currentUserLogin.getEmployeeID().empty())
    {
        for (size_t i = 0; i < employees.size(); ++i)
        {
            if (employees[i].getID() == currentUserLogin.getEmployeeID())
            {
                if (employees[i].getPosition() == "Guard")
                {
                    Guard guard;
                    GuardFunctionalityHandler::guardMenu(guard, employees);
                }
                else if (employees[i].getPosition() == "Supervisor")
                {
                    Supervisor supervisor;
                    SupervisorFunctionalityHandler::SupervisorMenu(supervisor, "LeaveRecords.txt", employees);
                }
                else if (employees[i].getPosition() == "Employee")
                {
                    Employee emp;
                    EmployeeFunctionalityHandler::EmployeeMenu(employees);
                }
                else if (employees[i].getPosition() == "Director")
                {
                    Director director;
                    DirectorFunctionalityHandler::DirectorMenu(director, "DirectorApproval.txt", employees);
                }
                break;
            }
        }
    }
}