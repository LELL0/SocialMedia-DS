//This Program was written and tested on Xcode 7.0

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
using namespace std;

struct Individual;

struct Friend
{
    Individual *self;
    Friend * next;
    int agepoint=0;
};

struct Individual
{
    string FirstName, LastName, username;
    int Age;
    string gender;
    string University;
    Individual *next;
    Individual *previous;
    Friend *myFriends;
};

struct World
{
    Individual *Head, *Tail;
};

Individual *Find_Individual (World w, Individual i)
{
    Individual *cur=w.Head;
    while (cur!=NULL)
    {
        if (cur->FirstName==i.FirstName && cur->LastName==i.LastName && cur->Age==i.Age && cur->gender==i.gender && cur->University==i.University)
            return cur;
        cur=cur->next;
    }
    
    return NULL;
}

bool IsEmpty(World w){
    return (w.Head==NULL);
}

void InitializeWorld (World &w)
{
    w.Head=NULL;
    w.Tail=NULL;
}

Individual* InsertAtHead (World &w, Individual i)
{
    Individual* tmp;
    tmp = new Individual;
    
    if (tmp==NULL)
        exit(1);
    
    tmp->FirstName=i.FirstName;
    tmp->LastName=i.LastName;
    tmp->Age=i.Age;
    tmp->gender=i.gender;
    tmp->University=i.University;
    tmp->myFriends=i.myFriends;
    tmp->username="NULL";
    tmp->next=NULL;
    tmp->previous=NULL;
    
    if(IsEmpty(w))
    {
        w.Head=tmp;
        w.Tail=tmp;
        return tmp;
    }
    
    else
    {
        tmp->next=w.Head;
        w.Head->previous=tmp;
        w.Head=tmp;
        return tmp;
    }
}

bool IsaFriend (Individual* i1, Individual* i2)
{
    if (i1==NULL || i2==NULL)
    {
        return false;
    }
    Friend *cur= i1->myFriends;
    while (cur!=NULL)
    {
        if(cur->self==i2)
            return true;
        cur=cur->next;
    }
    return false;
}

void Create_Relationship (Individual *&i1, Individual *&i2)
{
    
    Friend  *tmp1, *tmp2;
    tmp1 = new Friend;
    tmp2 = new Friend;
    
    tmp1->next=NULL;
    tmp1->self=NULL;
    
    tmp2->next=NULL;
    tmp2->self=NULL;
    
    tmp1->self=i2;
    tmp2->self=i1;
    
    if (i1->myFriends==NULL)
        i1->myFriends=tmp1;
    
    else
    {
        tmp1->next=i1->myFriends;
        i1->myFriends=tmp1;
    }
    
    if (i2->myFriends==NULL)
        i2->myFriends=tmp2;
    
    else
    {
        tmp2->next=i2->myFriends;
        i2->myFriends=tmp2;
    }
}

void Delete_Friend (Individual* to_delete, Friend* &f)
{
    Friend *cur, *prev;
    if (f==NULL)
        return;
    
    if(f->self==to_delete)
    {
        cur=f->next;
        delete f;
        f=cur;
    }
    
    cur=f;
    prev=NULL;
    
    while(cur!=NULL && cur->self!=to_delete)
    {
        prev=cur;
        cur=cur->next;
    }
    
    if(cur==NULL)
        return;
    
    prev->next=cur->next;
    delete cur;
}

void Delete_Friendship (Individual *&i1, Individual *&i2)
{
    Delete_Friend(i1, i2->myFriends);
    Delete_Friend(i2, i1->myFriends);
}


void Delete_Individual (World &w, Individual *i)
{
    
    while(i->myFriends!=NULL)
    {
        Individual* tmp=i->myFriends->self;
        Delete_Friendship(i, tmp);
    }
    
    
    if (i==w.Head)
    {
        i=w.Head->next;
        delete w.Head;
        w.Head=i;
        w.Head->previous=NULL;
        return;
    }
    
    else if (i==w.Tail)
    {
        i=w.Tail->previous;
        delete w.Tail;
        w.Tail=i;
        w.Tail->next=NULL;
        return;
    }
    
    else
    {
        i->previous->next=i->next;
        i->next->previous=i->previous;
        delete i;
    }
    
}

void InsertFriendAtHead (Friend *&head, Individual *f)
{
    Friend* tmp = new Friend;
    tmp->self = f;
    tmp->next = NULL;
    
    if (head == NULL)
        head = tmp;
    
    else
    {
        tmp->next = head;
        head = tmp;
    }
}

Friend* Friend_Exist (Individual* i1, Individual* i2)
{
    Friend* Mutual=new Friend;
    Mutual=NULL;
    
    Friend *cur1= i1->myFriends;
    
    while (cur1!=NULL)
    {
        Friend *cur2= i2->myFriends;
        while (cur2!=NULL)
        {
            if (cur1->self==cur2->self)
            {
                InsertFriendAtHead(Mutual, cur1->self);
                break;
            }
            cur2=cur2->next;
        }
        cur1=cur1->next;
    }
    return Mutual;
}

void InsertFromFile (World &w)
{
    ifstream file;
    file.open("World.txt");
    if (!file.is_open())
        return;
    string line;
    while (getline(file, line))
    {
        stringstream s(line);
        Individual tmp1, tmp2;
        getline(s,tmp1.FirstName,'#');
        getline(s,tmp1.LastName,'#');
        string s1;
        getline(s,s1,'#');
        tmp1.Age= stoi(s1);
        getline(s,tmp1.gender,'#');
        getline(s,tmp1.University,',');
        getline(s,tmp2.FirstName,'#');
        getline(s,tmp2.LastName,'#');
        string s2;
        getline(s,s2,'#');
        tmp2.Age=stoi(s2);
        getline(s,tmp2.gender,'#');
        getline(s,tmp2.University);
        
        Individual *search1 =Find_Individual(w, tmp1);
        Individual *search2 =Find_Individual(w, tmp2);
        
        if (search1==NULL)
            search1=InsertAtHead(w, tmp1);
        
        if (search2==NULL)
            search2=InsertAtHead(w, tmp2);
        
        if (IsaFriend(search1, search2)==false)
            Create_Relationship(search1, search2);
    }
    file.close();
}

void SavetoFile (World w)
{
    cout<<"Caution: Saving deletes friendships, if you wish to continue load data from file again."<<endl;
    ofstream file;
    file.open("World.txt");
    Individual* curi;
    curi=w.Head;
    while(curi!=NULL)
    {
        while (curi->myFriends!=NULL)
        {
            file<<curi->FirstName<<"#"<<curi->LastName<<"#"<<curi->Age<<"#"<<curi->gender<<"#"<<curi->University<<','<<curi->myFriends->self->FirstName<<"#"<<curi->myFriends->self->LastName<<"#"<<curi->myFriends->self->Age<<"#"<<curi->myFriends->self->gender<<"#"<<curi->myFriends->self->University<<endl;
            Delete_Friendship(curi, curi->myFriends->self);
        }
        curi=curi->next;
    }
}



Friend * InsertSorted(Friend* &head, Individual* self, int point)
{
    Friend *tmp = new Friend;
    tmp->self=self;
    tmp->agepoint=point;
    tmp->next=NULL;
    if (head==NULL)
        return tmp;
    if (tmp->agepoint<=head->agepoint)
    {
        tmp->next=head;
        head=tmp;
        return head;
    }
    Friend *cur= head;
    while (cur->next!=NULL && cur->next->agepoint<point)
    {
        cur=cur->next;
    }
    tmp->next=cur->next;
    cur->next=tmp;
    return head;
}

Friend *Sugg (World w, Individual*i)       //Age Difference and Same University
{
    Individual* cur = w.Head;
    Friend* Suggested =NULL;
    int point;
    while (cur!=NULL)
    {
        point=0;
        if (!IsaFriend(i, cur) && cur!=i && cur->University==i->University)
        {
            point=point+abs(cur->Age-i->Age);
            Suggested=InsertSorted(Suggested, cur, point);
        }
        cur=cur->next;
    }
    return Suggested;
}

//Xcode
void displayfriend (Friend *head)
{
    if (head==NULL)
    {
        cout<<"No available friends."<<endl;
        return;
    }
    else
    {
        Friend* cur=head;
        cout <<"Friends:"<<endl;
        while (cur!=NULL)
        {
            cout<<"\t\tName: "<<cur->self->FirstName <<" "<<cur->self->LastName<<endl;
            cur=cur->next;
        }
        cout<<endl;
    }
}


//Changed to work on visual studio
//void displayfriend (Friend *head)
//{
//    cout <<"Friends:"<<endl;
//    if (head==NULL)
//    {
//        return;
//    }
//    Friend* cur=head;
//    while (cur!=NULL)
//    {
//        Individual* tmp=cur->self;
//        cout<<"\t\tName: "<<tmp->FirstName <<" "<<tmp->LastName<<endl;
//        cur=cur->next;
//    }
//    cout<<endl;
//}

void displayIndividual(Individual *i,bool friends)
{
    cout <<"FirstName: "<<i->FirstName<<endl;
    cout <<"LastName: "<<i->LastName<<endl;
    cout <<"Age: "<<i->Age<<endl;
    cout <<"Gender: "<<i->gender<<endl;
    cout <<"University: "<<i->University<<endl;
    cout<<"Username: "<<i->username<<endl;
    if (friends==true && i->myFriends!= NULL)
        displayfriend(i->myFriends);
}

void displayworld (World w, bool friends)
{
    Individual *cur=w.Head;
    int c=1;
    while (cur!=NULL)
    {
        cout<<c<<endl;
        c++;
        displayIndividual(cur, friends);
        cur=cur->next;
    }
}

int username_is_unique (Individual* i)
{
    int c=1;
    Individual *cur=i->previous;
    while (cur!=NULL)
    {
        if (i->FirstName==cur->FirstName && i->LastName==cur->LastName)
            c++;
        cur=cur->previous;
    }
    return c;
}

void addUsername (World &w)
{
    Individual* cur=w.Head;
    while (cur!=NULL)
    {
        cur->username=cur->FirstName+"."+cur->LastName+"."+to_string(username_is_unique(cur));
        cur=cur->next;
    }
}

Individual InputIndividual()
{
    Individual cur;
    cout<<"FirstName: ";
    cin>>cur.FirstName;
    cout<<"LastName: ";
    cin>>cur.LastName;
    cout<<"Age: ";
    cin>>cur.Age;
    while (cur.Age<=0 || cur.Age>110)
    {
        cout<<"Invalid entry. Please enter an age between 1 and 111"<<endl;
        cin>>cur.Age;
    }
    cout <<"Gender: ";
    cin>>cur.gender;
    cout <<"University: ";
    cin>>cur.University;
    cout<<endl;
    cur.myFriends=NULL;
    return cur;
}

void DisplayInput (World w)
{
    string FName, LName;
    cout <<"Enter first Name to search for: ";
    getline(cin, FName);
    cout <<"Enter last name to search for: ";
    getline(cin, LName);
    while (w.Head!=NULL)
    {
        if (FName==w.Head->FirstName && LName==w.Head->LastName)
            displayIndividual(w.Head, false);
        w.Head=w.Head->next;
    }
}

Individual* Find_by_username (World w)
{
    string username;
    cout<<"Enter corresponding username: ";
    getline(cin,username);
    while (w.Head!=NULL)
    {
        if (username==w.Head->username)
            return w.Head;
        w.Head=w.Head->next;
    }
    return NULL;
}

Individual* Username_Search(World w)
{
    string username;
    cout<<"Enter corresponding username: ";
    cin.ignore();
    getline(cin,username);
    while (w.Head!=NULL)
    {
        if (w.Head->username==username)
            return w.Head;
        w.Head=w.Head->next;
    }
    return NULL;
}

Individual* Name_Search(World w)
{
    Individual* tempArr[50];
    int counter = 0;
    Individual* cur = w.Head;
    
    string FName, LName;
    cout<<"Enter first name to search for: ";
    cin.ignore();
    getline(cin, FName);
    cout<<"Enter last name to search for: ";
    getline(cin, LName);
    while(cur != NULL)
    {
        if (cur->FirstName == FName && cur->LastName == LName)
        {
            tempArr[counter] = cur;
            counter++;
        }
        cur = cur->next;
    }
    
    if (counter == 0)
        return NULL;
    
    if (counter == 1)
        return tempArr[0];
    
    for (int i = 0; i < counter; i++)
    {
        cout<<endl;
        cout <<"FirstName: "<<tempArr[i]->FirstName<<endl;
        cout <<"LastName: "<<tempArr[i]->LastName<<endl;
        cout <<"Age: "<<tempArr[i]->Age<<endl;
        cout <<"Gender: "<<tempArr[i]->gender<<endl;
        cout <<"University: "<<tempArr[i]->University<<endl;
        cout<<"Username: "<<tempArr[i]->username<<endl;
    }
    
    Individual* to_return=NULL;
    int n;
    do
    {
        cout << endl <<"Enter the corresponding username extension for "<<FName <<" and "<<LName<<" : ";
        cin>>n;
        for (int i = 0; i < counter; i++)
            if (tempArr[i]->username==(FName+"."+LName+"."+to_string(n)))
                to_return=tempArr[i];
    }
    while (to_return==NULL);
    return to_return;
}

Individual* Searching (World w)
{
    char c1;
    cout <<"Do you know the username directly?"<<endl;
    cout<<"Enter 'y' for yes or 'n' for no: "<<endl;
    cin>>c1;
    while (c1!='Y' && c1!='y' && c1!='N' && c1!='n')
    {
        cout<<"Invalid entry please try again: ";
        cin>>c1;
    }
    if (c1=='y' || c1=='Y')
        return Username_Search(w);
    else
        return Name_Search(w);
}


void menu ()
{
    cout<<"----------------------To exit press 0---------------------"<<endl;
    cout<<"-----------------To load data from file press 1-----------"<<endl;
    cout<<"-----------To display the entire world press 2------------"<<endl;
    cout<<"--------To add a new individual manually press 3----------"<<endl;
    cout<<"----To display information about an individual press 4----"<<endl;
    cout<<"-------------To delete an individual press 5--------------"<<endl;
    cout<<"---------To display suggested friends press 6-------------"<<endl;
    cout<<"-To display mutual friends between two individuals press 7"<<endl;
    cout<<"-To create a relationship between two individuals press 8-"<<endl;
    cout<<"-To delete a relationship between two individuals press 9-"<<endl;
    cout<<"-----To display an individual's friend list press 10------"<<endl;
    cout<<"------------------To save world press 11------------------"<<endl;
    cout <<endl;
}

int main()
{
    World w;
    InitializeWorld(w);
    InsertFromFile(w);
    addUsername(w);
    cout<<"Initializing... Extracting Data from 'World.txt'..."<<endl;
    if(w.Head==NULL)
        cout<<"Could not retrieve data from file. Try Again"<<endl;
    else
    {
        int c;
        cout<<"Success!"<<endl;
        displayworld(w, true);
        char c2;
        do
        {
            Individual *i, *i1, *i2;
            menu();
            cin>>c;
            if (c==1)
                InsertFromFile(w);
            else if (c==2)
            {
                cout<<"Would you like to display each individual's friend list also?"<<endl;
                cout<<"Enter 'y' for yes or 'n' for no: "<<endl;
                char c1;
                cin>>c1;
                while (c1!='Y' && c1!='y' && c1!='N' && c1!='n')
                {
                    cout<<"Invalid entry please try again: ";
                    cin>>c1;
                }
                if (c1=='y' || c1=='Y')
                    displayworld(w, true);
                else
                    displayworld(w, false);
            }
            else if(c==3)
            {
                cout<<"Please input corresponding individual charachteristics: "<<endl;
                Individual i1=InputIndividual();
                InsertAtHead(w, i1);
                addUsername(w);
                cout<<"Would you like to add a friend to this individual?"<<endl;
                cout<<"Enter 'y' for yes or 'n' for no: "<<endl;
                char c1;
                cin>>c1;
                while (c1!='Y' && c1!='y' && c1!='N' && c1!='n')
                {
                    cout<<"Invalid entry please try again: ";
                    cin>>c1;
                }
                if (c1=='y' || c1=='Y')
                {
                    Individual *i2 =Searching(w);
                    if (i2==NULL)
                        cout<<"Individual not found"<<endl;
                    else
                    {
                        Create_Relationship(w.Head, i2);
                        displayIndividual(w.Head, true);
                        displayIndividual(i2, true);
                    }
                }
            }
            
            else if(c==4)
            {
                i=Searching(w);
                if (i==NULL)
                    cout<<"Individual not found"<<endl;
                else
                    displayIndividual(i, true);
            }
            
            else if (c==5)
            {
                i = Searching(w);
                if (i==NULL)
                    cout<<"Individial not found.";
                else
                {
                    Delete_Individual(w, i);
                    cout<<"Success!";
                }
            }
            
            else if (c==6)
            {
                i = Searching(w);
                if (i==NULL)
                    cout<<"Individial not found.";
                else
                {
                    cout<<"Suggested friends based on age difference and in " <<i->University<<":"<<endl;
                    Friend* sugg1= Sugg(w, i);
                    displayfriend(sugg1);
                }
            }
            
            else if (c==7)
            {
                cout<<"First individual: "<<endl;
                i1=Searching(w);
                if (i1==NULL)
                    cout<<"Individial not found.";
                else
                {
                    cout<<"Second individual: "<<endl;
                    i2 = Searching(w);
                    if (i2==NULL)
                        cout<<"Individial not found.";
                    if (i2==i1)
                        cout<<"You entered the same individual.";
                    if (i1!=NULL && i2!=NULL && i1!=i2)
                    {
                        Friend* Mutual=Friend_Exist(i1, i2);
                        displayfriend(Mutual);
                    }
                }
            }
            else if (c==8)
            {
                cout<<"First individual: "<<endl;
                i1=Searching(w);
                if (i1==NULL)
                    cout<<"Individial not found.";
                else
                {
                    cout<<"Second individual: "<<endl;
                    i2 = Searching(w);
                    if (i2==NULL)
                        cout<<"Individial not found.";
                    if (i2==i1)
                        cout<<"You entered the same individual.";
                    if (i1!=NULL && i2!=NULL && i1!=i2)
                        Create_Relationship(i1, i2);
                }
            }
            
            else if (c==9)
            {
                cout<<"First individual: "<<endl;
                i1=Searching(w);
                if (i1==NULL)
                    cout<<"Individial not found.";
                else
                {
                    cout<<"Second individual: "<<endl;
                    i2 = Searching(w);
                    if (i2==NULL)
                        cout<<"Individial not found.";
                    if (i2==i1)
                        cout<<"You entered the same individual.";
                    if (i1!=NULL && i2!=NULL && i1!=i2)
                    {
                        if (IsaFriend(i1, i2)==true)
                            Delete_Friendship(i1, i2);
                        else
                            cout<<"They are not friends."<<endl;
                    }
                }
            }
            
            else if (c==10)
            {
                i=Searching(w);
                if (i==NULL)
                    cout<<"Individial not found.";
                else
                    displayfriend(i->myFriends);
            }
            
            else if (c==11)
            {
                SavetoFile(w);
            }
            else if(c==0)
                break;
            else
                cout<<"Invalid number.";
            
            cout<<endl;
            cout<<"Would you like to do another function?"<<endl;
            cout<<"Enter 'y' for yes or 'n' for no: "<<endl;
            
            cin>>c2;
            while (c2!='Y' && c2=='y' && c2=='n' && c2=='N')
            {
                cout<<"Invalid entry please try again: ";
                cin>>c2;
            }
            
        } while (c2=='Y' || c2=='y');
        cout<<endl;
        return 0;
    }
}
