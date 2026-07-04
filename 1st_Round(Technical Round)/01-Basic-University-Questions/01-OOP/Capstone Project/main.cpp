#include<iostream>
#include<string>
#include <SQLiteCpp/SQLiteCpp.h>
using namespace std;

class UserDashboard {
    private:
        string borrowBookname;
        string currentDate;
        string ReturnDate;
        int latefee;
        string searchByBookName;
        string searchByAuthorName;
        int searchByprice;
        string searchByGenre;
    public:
        void setBorrowBook(){
            cin.ignore();
            cout<<"\n Enter book name for borrow \n";
            getline(cin,borrowBookname);
            cout<<" \n Enter current date \n ";
            getline(cin,currentDate);
            cout<<" \n Enter return(due) date \n ";
            getline(cin,ReturnDate);
            cout<<" \n Late fee after due date will be '500' \n ";
        }
        void getSearchBook(){
            cin.ignore();
            cout<<"\n Search book by BookName \n";
            getline(cin,searchByBookName);
            cout<<"\n Search book by authorName \n";
            getline(cin,searchByAuthorName);
            cout<<"\n Search book by Price \n";
            cin>>searchByprice;
            cout<<"\n Search book by Genre \n";
            getline(cin,searchByGenre);
        }
};

class AdminDashboard  {
    private:
        string username ;
        string isMembership;
        string borrowId;
        string contactDetail; 
        // setInsertBook() data members
        string booktitle;
        string ibanBook;
        string genreBook;
        double bookPrice = 0;
    public:
        void setSignUp(){
            cin.ignore();
            cout<<"\n Enter your Full UserName \n";
            getline(cin,username);
            cout<<"\n If you want to get memembership of library then input 'yes' or 'no'  \n";
            getline(cin,isMembership);
            cout<<"\n Enter your BorrowID \n";
            getline(cin,borrowId);
            cout<<"\n Enter your phoneNumber \n";
            getline(cin,contactDetail);
        }
        void setInsertBook(){
            cin.ignore();
            cout<<"\n Enter title of book \n";
            getline(cin,booktitle);
            cout<<"\n Enter IBAN of book \n";
            getline(cin,ibanBook);
            cout<<"\n Enter Genere of book \n";
            getline(cin,genreBook);
            cout<<"\n Enter price of book \n";
            cin>>bookPrice;
        }
        void setUpdateUser(){
            cin.ignore();
            cout<<"\n Enter your new UserName(updated) \n";
            getline(cin,username);
            cout<<"\n If you want to change memembership(updated) of library then input 'yes' or 'no'  \n";
            getline(cin,isMembership);
            cout<<"\n Enter your new phoneNumber(updated) \n";
            cin>>contactDetail;
        }
        void setUpdateBook(){
            cin.ignore();
            cout<<"\n Enter new title(updated) of book \n";
            cin>>booktitle;
            cout<<"\n Enter new Genere(updated) of book \n";
            cin>>genreBook;
            cout<<"\n Enter new Price(updated) of book  \n";
            cin>>bookPrice;
        }
};
class Admin : public AdminDashboard{
    private:
        string name = "Zain Ali";
        string password;
    public:
         void getPassword(){
            cout<<"\n Enter your password to acccess portal \n";
            cin>>password;
            if(password == "admin12345")
            {
                showDashboard();
                cout<<"Admin dashboard";
            }
            else{
                int count = 3;
                for (int i = 1; i <= count; count--) {
                    cout<<"\n Wrong Password, Try again "<<count<<" attempt left \n";
                    cin>>password;
                    if(password == "admin12345")
                    {
                        showDashboard();
                        cout<<"Admin dashboard";
                        break;
                       }
                }                       
            }
        }
        void showDashboard(){
            AdminDashboard admindashboard;
            int dashboardinput = 0;
            cout<<"Press 1 to 'SignUp User' \n";
            cout<<"Press 2 to 'Insert Book' \n";
            cout<<"Press 3 to 'Update User' \n";
            cout<<"Press 3 to 'Update Book' \n";
            cout<<"Press 5 to 'Go Back' \n";
            cin>>dashboardinput;
            int i = 1;
            while (i != 0) {
                if(dashboardinput == 1 || dashboardinput == 2 || dashboardinput == 3|| dashboardinput == 4 || dashboardinput == 5){
                if(dashboardinput == 1){
                cout<<"\n Signup Form";
                admindashboard.setSignUp();
                cout<<"Register user";
                i = 0;
            }else if(dashboardinput == 2){
                cout<<"\n Insert Book"; 
                admindashboard.setInsertBook();         
                i = 0;
            }else if(dashboardinput == 3){
                cout<<"\n Update User";
                admindashboard.setUpdateUser();
                i = 0;
            }else if(dashboardinput == 4){
                cout<<"\n Update Book";
                admindashboard.setUpdateBook();
                i = 0;
            }
            else{
                cout<<"\n Go Back";
                i = 0;
                cout<<"\n Go one step back";
            }
        }
        else{
           cout<<"\nPress any number 1, 2, 3, 4, 5 to choose option";
           cin>>dashboardinput;
        }
    }
    }
};
class User : public UserDashboard{
    private:
    string email;
    string password;
    public:
        void getEmail(){
            cout<<"\n Enter your Email \n";
            cin>>email;
            if(email == "user@gmail.com")
            {
                getUserPassword();
            }
            else{
                int count = 3;
                for (int i = 1; i <= count; count--) {
                    cout<<"\n Wrong Email, Try again "<<count<<" attempt left \n";
                    cin>>email;
                    if(email == "user@gmail.com")
                    {
                        getUserPassword();
                        break;
                    }
                }                       
            }
        }

        
        void getUserPassword(){
            cout<<"\n Enter your password \n";
            cin>>password;
            if(password == "user12345")
            {
                showUserDashboard();
            }
            else{
                int count = 3;
                for (int i = 1; i <= count; count--) {
                    cout<<"\n Wrong Password, Try again "<<count<<" attempt left \n";
                    cin>>password;
                    if(password == "user12345")
                    {
                        showUserDashboard();
                        cout<<"User dashboard";
                        break;
                    }
                }                       
            }
        
        }
        void showUserDashboard(){
            int dashboardinput = 0;
            UserDashboard userdashboard;
            cout<<"Press 1 to 'Borrow Book' \n";
            cout<<"Press 2 to 'Search Book' \n";
            cout<<"Press 3 to 'Go Back' \n";
            cin>>dashboardinput;
            int i = 1;
            while (i != 0) {
                if(dashboardinput == 1 || dashboardinput == 2 || dashboardinput == 3){
                        if(dashboardinput == 1){
                            cout<<"\n Borrow Form"; 
                            userdashboard.setBorrowBook();
                            i = 0;
                        }else if(dashboardinput == 2){
                            cout<<"\n Search Book";
                            userdashboard.getSearchBook();
                            i = 0;
                        }
                        else{
                            cout<<"\n Go Back";
                            i = 0;
                            cout<<"\n Go one step back";
                        }
                    }
                else{
                    cout<<"\nPress any number 1, 2, 3 to choose option";
                    cin>>dashboardinput;
                }
            }
        }
};

int main(){
    for(int i = 1; i < 5;i++){
        cout<<"---------------------------------------------------------------\n";
    }
    cout<<"--------<<<< Welcome to libraryManagementSystem >>>>-----------\n";
    for(int i = 1; i < 5;i++){
        cout<<"---------------------------------------------------------------\n";
    }
    int dashboardinput = 0;
    cout<<"Press 1 to 'Login as Admin' \n";
    cout<<"Press 2 to 'Login as User' \n";
    cout<<"Press 3 to 'Exit Program' \n";
    cin>>dashboardinput;
    int i = 1;
    while (i != 0) {
        if(dashboardinput == 1 || dashboardinput == 2 || dashboardinput == 3){
            if(dashboardinput == 1){
                cout<<"\n Welcome admin";
                Admin a;
                a.getPassword();
                i = 0;
            }else if(dashboardinput == 2){
                cout<<"\n Welcome user";
                User u;
                u.getEmail();
                i = 0;
            }else{
                cout<<"\n Exit program! GoodBye";
                i = 0;
                return 0;
            }
        }else{
           cout<<"\nPress any number 1, 2, 3 to choose option";
           cin>>dashboardinput;
        }
    }
}

// If memebership is yes than give user borrow id otherwise move on
// After succesfull registration don't close the program go one step back