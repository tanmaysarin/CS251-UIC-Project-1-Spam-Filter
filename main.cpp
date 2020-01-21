//
// << Tanmay Sarin >>
// U. of Illinois, Chicago
// CS 251: Spring 2020
// 
// Project 1: Spam Filter -> Make a project to filter out 
// all the spam emails provided in the text files and 
// print out the non spam emails in a new text file.
// 
// More details on the project website ->
// 
// https://www.dropbox.com/sh/9egdp7si3bz7lxs/AACDfNJpLU_n9pYGAYN3Q9Oqa?dl=0&preview=project01.pdf
//

#include <iostream>
#include <fstream>
#include <string>

#include "ourvector.h"

using namespace std;

//Load command brings here-
ourvector<string> fileReader(string filename,                //name of the file to be loaded
                             ourvector<string>&  addresses){ //vector to store all the entries
    
    ifstream infile(filename);
    
    if (!infile.is_open()) {
        cout << "**Error, unable to open '" <<filename<<"'"<< endl;
        return addresses;
    }
    
    string str; //str to store the emails address
    int i = 0;  //Counter for the number of emails

    infile>>str;
    while(!infile.eof())
    {
        //Push back the spam email address
        addresses.push_back(str);
        i++;
        //Going to the next spam email address
        infile>>str;
    }
    
    infile.close();
    
    cout<<"Loading '"<<filename<<"'"<<endl;
    cout<<"# of spam entries: "<<i<<endl;
    
    return addresses;
}

//Display command brings here-
void display(ourvector<string>&  addresses){
     
     for (auto s : addresses)
         cout << s << endl;
}

//Perform binary search for the email entered, with the spamllist entries-
bool customBinarySearch(ourvector<string>&  addresses, string email)
{
    int mid = 0;
    int low = 0;  
    int high = addresses.size()-1;
    
    while(low<=high){
        mid = low + ((high - low)/2);
        string middle = addresses[mid];
        
        if(middle < email){
            low = mid +1;
        }
        else if(middle > email){
            high = mid -1;
        }
        else 
        {
            return true; //The email entered is a spam
        }
    }
    return false; // The email entered is not a spam
} 

//Function to get the domain and the username from the email entered by the user-
void parseEmailAddress(string email, string& username, string& domain)
{
    char c = '@';
    
    username = "";
    domain = "";
  
    int res = 0;
    int size = email.length();
    //Find the position of the @ symbol...
    for (int i=0;i<size;i++){
        // checking character in string 
        if (email[i] == c){
            res = i;      
        }
    }
    username = email.substr(0,res);
    domain = email.substr(res+1,size);
}

//Filter command brings here-
void filterCommand(string emailFile,//email file name entered by the user
                   string outPutFileName, //name of the output file
                   ourvector<string>& addresses, //vector that stores all the spam emails
                   string username,
                   string domain)
{
    string emailAddress, num, text;
    int emailCounter = 0;
    int spamCounter = 0;
    string newEmail;
    string newEmail2;
    
    //open output file
    ofstream outfile(outPutFileName);
    
    if(!outfile.good()){
        cout<<"Unable to open the output file"<<endl;
        return;
    }
    //open email file to read from
    ifstream infile(emailFile);
 
    if (!infile.is_open()) {
        cout << "**Error, unable to open '" <<emailFile<<"'"<< endl;
        return;
    }
        
    //Read the first line from the email file and store the number in num, email in emailAddress and subject in text
    infile>>num;
    infile>>emailAddress;
    getline(infile, text);
            
    while (!infile.eof())
    {
        //Parse the email  from the emailfile
        parseEmailAddress(emailAddress, username, domain);
            
        //Reformat the email to match the formatting of the spamlist
        newEmail = domain + ":" + username;
        newEmail2 = domain + ":*";
            
        //Check for spam
        if(customBinarySearch(addresses, newEmail) || customBinarySearch(addresses, newEmail2)){
            //if the email addresses read was a spam then do nothing
        }
        else{
            //if the email addresses read was not a spam then increment the counter and print out the line
            ++spamCounter;                
            outfile<<num<<" ";
            outfile<<emailAddress;
            outfile<<text; // Does this puts the text inside the output file as well?
            outfile<<endl;
        } 
        infile>>num;              //
        infile>>emailAddress;    //Reading the next entries...
        getline(infile, text);   //
                            
         emailCounter++; //counter
    }
    
    //Close the opened files
    infile.close();
    outfile.close();
    
    cout<<"# emails processed: "<<emailCounter<<endl;
    cout<<"# non-spam emails:  "<<spamCounter<<endl; 
}

int main(){
    
    string filename; //The name of the file to be loaded
    string username, domain, email;
    string userInput; //INput by the user
    string emailFile, outPutFileName; //Name of the email file and the output file, to be used during the filter function
    
    ourvector<string>  addresses; // Vector to store the spam addresses
    
    cout<<"** Welcome to spam filtering app **\n"<<endl;
    cout<<"Enter command or # to exit> ";
    cin>>userInput;
    
     //Start the menu-
    while (userInput != "#"){
        if(userInput == "display"){
            display(addresses);
        }
        else if(userInput == "check"){
            cin>>email;
            parseEmailAddress(email, username, domain);
            
            //Reformat the email to match the formatting of the spamlist entries
            string newEmail = domain + ":" + username;
            string newEmail2 = domain + ":*";
            
            //Function call to check whether the email address entered is a spam or not
            if(customBinarySearch(addresses, newEmail) || customBinarySearch(addresses, newEmail2)){
                cout<<email<<" is spam"<<endl;
            }else{
                cout<<email<<" is not spam"<<endl;
            }
        }
        else if(userInput == "filter"){
            cin>>emailFile;
            cin>>outPutFileName;
            
            //Call the filter command           
            filterCommand(emailFile, outPutFileName, addresses, username, domain);
        }
        else if(userInput == "load"){
            
            addresses.clear(); //Clearing out any list loaded previously
            
            cin>>filename;
            
            //Call the function to load the file
            fileReader(filename, addresses);
        }
        else{
            cout<<"**invalid command"<<endl;
        }
        
        //repeat:
        cout<<"\nEnter command or # to exit> ";
        cin>>userInput;
    }
    
    //Declaring the exit case-
    if (userInput == "#"){
        return 0;
    }
    
    return 0;
}