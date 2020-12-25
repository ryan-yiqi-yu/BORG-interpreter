
#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
using namespace std;

class hashtable{ //class for the hashtable used in the program
private: //privately accessible elements of hashtable class
    struct node{ //in order to utilize chaining as a collision resolution technique, nodes are used to make a linked list at the beginning of each hashtable entry
        string name; //the name of the person is stored in each node
        int value, scopelvl; //the value associated with each person and the scope level of each person are stored in the node
        node *next = NULL; //the next linked node pointer is defaulted to NULL initially
    };
    const int size=100; //constant int value for the size of the hashtable
    node ** array=new node *[size]; //declaring a dynamically allocated array of pointers. These are the buckets of the hashtable.
    int scope=0; //the scope of the hashtable is initially at 0, but can increase or decrease based on BORG commands
    node *find =NULL; //find node pointer is used for traversing the hashtable and searching for names and values
    
public: //publicly accessible elements of hashtable class; functions of the class
    hashtable (){ //hashtable class constructor. This function initializes all the pointers in the array to NULL
        for(int j = 0; j<size; j++){//traversing through entire pointer array
            array[j]=NULL;//initializing to NULL
        }
    }
    int hash(string newname){ // hash function to calculate the hash value of a key--what index a key will be hashed to
        stringstream ss(newname);//takes the name/key passed into the hash function and converts it to a string stream in order to find individual ASCII values of each char
        char x; //creating char variable to be utilized in calculation
        int indexplusone=1, hashvalue=0; // hashvalue is initialized to 0, and the postion of each char is initialized at 1 to start
        while(ss>>x){//for every character in the full name string, the following while-suite will execute
            hashvalue = hashvalue+(indexplusone * int(x)); //the un-modded hashvalue is the total sum of each char's ASCII value multiplied to the char's position. this equation finds it
            indexplusone++; //at the end of the while loop, the postion number increases in order to account for next char's postion
        }
        hashvalue=hashvalue%size; //hash value is the above sum mod the size of the hashtable--100
        return hashvalue;//hash function returns the final hash value
    }
    void queue(string newname, int data, int hashvalue){ //queue function is used to queue the linked lists connected to the buckets of the hashtable
        node *ptr = array[hashvalue]; //node pointer starts at the base of the bucket at the hashvalue
        node *newnode = new node;//a new node and a pointer pointing to it are created, which will contain all the data
        newnode->scopelvl=scope;//assigning a specific node to a scope level, in order to keep track of scopes in the hashtable
        newnode->value=data;//assigning a person's data to the value in the node
        newnode->name=newname;//storing the person's name/key in the new node
        if (ptr==NULL){//if pointer in a bucket of the hashtable is NULL, the pointer will be directed to the new node
            array[hashvalue]=newnode; //the pointer in the bucket is set to the new node
        }
        else{ //else suite will be executed if the bucket already has nodes attached to it, collision resolution is handled in this suite
            while(ptr->next!=NULL){//traversing the linked list until the next pointer is NULL, meaning that its at the end of the list
                ptr = ptr->next; //when the next pointer is not NULL, the while loop will keep traversing to the next respective pointer
            }
            ptr->next=newnode; //when the next node is found to be NULL, the while loop will stop and the next node will be directed to the new node, essentially ading a node to the linked list
        }
    }
    void hashinsert(string newname, int value){//function hashes a name/key and value to the hashtable, utilizing seperate chaining as the collision resolution technique
        int hashvalue = hash(newname);//calling the hash function to obtain the hash value
        queue(newname, value, hashvalue); //adding the name/key to the hashtable
    }
    void newscope(){//when START keyword is used, the scope value will increment
        ++scope;//increment
    }
    void closescope(){//when FINISH keyword is used, this closescope function is used in order to destroy nodes and decrement the scope level
        for(int k =0;k<size;k++){//traversing the pointer array
            node *ptr = array[k];//pointer variable ptr is set to the specific ptr in the array index
            while(true){//this while loop traverses the linked list attached to each index of the array
                if(ptr==NULL){//if the ptr is NULL, the scope will close without deleting any nodes
                    break;//breaks out the while loop
                }
                else if(ptr!=NULL && ptr->scopelvl==scope){ //for nodes where the scope level is equal to the scope being deleted, the node is destroyed
                    ptr->name="DELETED";//deleted name indicates that the node was destroyed
                    ptr->value=0;//the value will also be deleted, removing the original value
                    break;//while loop is broken
                }
                else { //if NULL and specific scope level hasnt been found, the ptr will traverse to the next pointer in the list
                    ptr=ptr->next;//traversing to the next pointer
                }
            }
        }
        scope=scope-1;//scope level value is decremented
    }
    int search(string name){ //function used to search for a specific key/name in the hashtable
        int val=hash(name);//hash value of the given name/key is calculated by calling the hash function
        if (val<0){ //for when a negative hash value is returned, an error has occured and the search function will retunr a 0, meaning that the search was unsuccessful
            return 0;//search unsuccessful
        }
        node *ptr = array[val];//a ptr is set to the pointer in the hashed bucket of the hashtable
        while (true){ //while loop traverses the linked list attached to each bucket
            if (ptr==NULL){ //if ptr is NULL, it means that the specific name was not found in the hashtable
                find=NULL;//the find pointer will be set to NULL
                return 0;//search unsuccessful
                break;//while loop is broken
            }
            else if(ptr->name==name){//if a pointer pointing to a node in the linked list has a name equivalent to the one being searched, the following suite will execute
                find=ptr;//the find pointer will be set to the found pointer
                return 1;//search successful
                break;//while loop broken
            }
            else if(ptr->next!=NULL){//if the next link in the linked list is not NULL, the following suite will execute, traversing further
                ptr=ptr->next;//pointer traverses to the next node
            }
            else if (ptr->next==NULL){//if the next node is NULL and none of the names beforehand has matched, that means that the search was unsuccessful
                find=NULL;//the find pointer will be set to NULL
                return 0;//search unsuccessful
                break;//while loop is broken
            }
        }
    }
    void print(string name){//print function
        if (search(name)==1){//if search is successful, the following will execute
            cout<<find->name<<" IS "<<find->value<<endl; //the name/key and value will print
        }
        else{//if search was unsuccessful, the following will execute
            cout<<name<<" IS UNDEFINED"<<endl;//the name/key will be printed to be undefined as it is not in the hashtable
        }
    }
    void changevalue(string name, int newvalue){//for when the = operator is used, the value of an existing name/key will be changed via this function
        search(name);//search function will be utilized and the find pointer will be utilized
        find->value=newvalue;//the value in the find pointer's node will be set to the new value
    }
    void increment(string name){//or when the ++ operator is used, the value of an existing name/key will be incremented via this function
        search(name);//search function will be utilized and the find pointer will be utilized
        int y = find->value;
        find->value=++y;//incrementing the value in the node
    }
    void decrement(string name){//or when the -- operator is used, the value of an existing name/key will be decremented via this function
        search(name);//search function will be utilized and the find pointer will be utilized
        int y = find->value;
        find->value=--y;//decrementing the value in the node
    }
    int multiply(string name, int num){//or when the * operator is used, the value of an existing name/key will be multiplied by a given number via this function
        search(name);//search function will be utilized and the find pointer will be utilized
        int ans = (find->value)*num; //finding the existing value multiplied by a given number
        //cout<<find->name<<" * "<<num<<" IS "<<ans<<endl;
        return ans;
    }
    int add(string name, int num){//or when the + operator is used, the value of an existing name/key will be added to by a given number via this function
        search(name);//search function will be utilized and the find pointer will be utilized
        int ans = (find->value)+num;//finding the existing value added to by a given number
        return ans;
    }
    int subtract(string name, int num){//or when the - operator is used, the value of an existing name/key will be subtracted by a given number via this function
        search(name);//search function will be utilized and the find pointer will be utilized
        int ans = (find->value)-num;//finding the existing value subtracted from by a given number
        return ans;
    }
    int divide(string name, int num){//or when the / operator is used, the value of an existing name/key will be divided by a given number via this function
        search(name);//search function will be utilized and the find pointer will be utilized
        int ans = (find->value)/num;//finding the existing value divided by a given number
        return ans;
    }
    int mod(string name, int num){//or when the % operator is used, the value of an existing name/key will be modded by a given number via this function
        search(name);//search function will be utilized and the find pointer will be utilized
        int ans = (find->value)%num;//finding the existing value modded by a given number
        return ans;
    }
    int exp(string name, int num){//or when the ^ operator is used, the value of an existing name/key will be put in the power of the given number via this function
        search(name);//search function will be utilized and the find pointer will be utilized
        int ans = (find->value)^(num);//finding the existing value exponentiated by a given number
        return ans;
    }
    void printinfo(){ //function called when only the PRINT keyword is used
        cout<<find->name<<" IS "<<find->value<<endl;//print statement
    }
};
class readBORG{ //class to interpret the BORG language
private:
    hashtable test;//private element of the class, an object of the hashtable class
public:
    void input(string *array, int num){ //this function takes in text files written in BORG and reads them
        for(int h = 0; h<num; h++){ //for loop will iterate over hte number of text files passed in
            cout<<"OUTPUT FOR "<<array[h]<<": "<<endl<<endl;
            ifstream borgcommands; //creating object borgcommands which will read the text files
            borgcommands.open(array[h]);//opening the text file passed in
            for(string line; getline(borgcommands, line); ){ //reads the text file line by line
                command(line);//a single line will be passed into the command function
            }
            borgcommands.close(); //closing the text file
            cout<<endl<<endl<<endl<<endl;
        }
    }
void command(string line){//command function taking in a single line at a time
    stringstream ss(line);//creating a stringstream object containing the line
    string word, word2, word3, word4;
    ss>>word>>word2>>word3>>word4;//the stringstream object is inputted into 4 word variables
    if (word=="START"){//if the first world variable is START, the following suite will execute
        test.newscope();//a new scope is created in the hashtable
    }
    else if(word=="FINISH"){//if the first world variable is FINISH, the following suite will execute
        test.closescope();//a scope is decreased in the hashtable
    }
    else if(word=="VAR"){//if the first world variable is VAR, the following suite will execute
        int num = stoi(word4); //according to the required syntax, word4 should be the number value associated with the person. This statement converts the string to an integer
        test.hashinsert(word2, num);//calling the function inserts the name and value into tha hashtable
    }
    else if (word=="COM"){//if the first word is COM, the rest of the line will be ignored
        return;//the function will stop here
    }
    else if (word2=="="){//if the second word is =, then the value of an existing name/key is being changed
        int num = stoi(word3);//will be changed to the 3rd word according to the required syntax of BORG
        test.changevalue(word, num);//change value function is called with the name to be changed and the value to be changed to
    }
    else if (word=="PRINT"){ //when the first word is PRINT, the following suite will execute
        if (word3=="*"){//if the 3rd word is *, the number stored in the hashtable will be printed multipled by the given number
            int num = stoi(word4);//the 4th word will be converted to an integer
            cout<<word2<<" * "<<num<<" IS "<<test.multiply(word2, num)<<endl;//the name and number will be passed into the multiply function and printed
        }
        else if (word3=="/"){//if the 3rd word is /, the number stored in the hashtable will be printed divided by the given number
            int num = stoi(word4);//the 4th word will be converted to an integer
            cout<<word2<<" / "<<num<<" IS "<<test.divide(word2, num)<<endl;//the name and number will be passed into the divide function and printed
        }
        else if(word3=="%"){//if the 3rd word is %, the number stored in the hashtable will be printed modded by the given number
            int num = stoi(word4);//the 4th word will be converted to an integer
            cout<<word2<<" % "<<num<<" IS "<<test.mod(word2, num)<<endl;//the name and number will be passed into the mod function and printed
        }
        else if(word3=="^"){//if the 3rd word is ^, the number stored in the hashtable will be printed exponentiated by the given number
            int num = stoi(word4);//the 4th word will be converted to an integer
            cout<<word2<<" ^ "<<num<<" IS "<<test.exp(word2, num)<<endl;//the name and number will be passed into the exponent function and printed
        }
        else if(word3=="+"){//if the 3rd word is +, the number stored in the hashtable will be printed added by the given number
            int num = stoi(word4);//the 4th word will be converted to an integer
            cout<<word2<<" + "<<num<<" IS "<<test.add(word2, num)<<endl;//the name and number will be passed into the exponent function and printed
        }
        else if(word3=="-"){//if the 3rd word is -, the number stored in the hashtable will be printed subtracted by the given number
            int num = stoi(word4);//the 4th word will be converted to an integer
            cout<<word2<<" - "<<num<<" IS "<<test.subtract(word2, num)<<endl;//the name and number will be passed into the exponent function and printed
        }
        else{ //if none of the above are true, the print function will be called
            test.print(word2);//passing in the name into the print function
        }
    }
    else if(word2=="++"){//if the second word is ++, the value associated with the name will be incremented via the increment function
        test.increment(word);//increment function is called passing in the name
    }
    else if (word2=="--"){//if the second word is --, the value associated with the name will be incremented via the increment function
        test.decrement(word);//decrement function is called passing in the name
    }
   ss.str("");//at the end of the function, the stringstream is cleared
}
};
int main(){//int main used to test code
    string array[2]= {"BORGdefault.txt", "BORGcustom.txt"};//code will be tested with these two text files written in BORG
    int numoffiles = 2;
    readBORG test1; //readBORG object is created
    test1.input(array, numoffiles);//the text files are passed into the input function of readBORG
    }

/*
 OUTPUT(lines 250 to 295):
 
 
 
 OUTPUT FOR BORGdefault.txt:

 BORAMIR IS 25
 LEGOLAS IS 101
 GANDALF IS UNDEFINED
 BORAMIR * 2 IS 52
 GANDALF IS 49
 BORAMIR IS 26
 GANDALF IS UNDEFINED
 LEGOLAS IS 1000
 LEGOLAS IS 1000
 LEGOLAS IS 999




 OUTPUT FOR BORGcustom.txt:

 BAM IS 25
 GOD IS 66
 ARCHIE IS 101
 ARCHIE % 3 IS 2
 GAN IS UNDEFINED
 BAM * 2 IS 52
 DOG / 7 IS 60
 DOG IS 419
 BAM IS 26
 DOG IS UNDEFINED
 GOD IS 23
 JOSHUA IS UNDEFINED
 GAN IS 5
 BAM IS 26
 GAN IS UNDEFINED
 CONAN IS 8
 ARCHIE IS 1000
 ARCHIE IS 1000
 JAMES IS UNDEFINED
 ARCHIE IS 999
 CONAN IS 9
 GOD * 8 IS 184
 ARCHIE IS 36




 Program ended with exit code: 0
 */


