#include <iostream>
#include <cstring>
#include <list>
#include <queue>

using namespace std;

class PrintRequest{
private:
    char* name;
    char* author;
    int amount;
public:
    int index;
    PrintRequest(){
        name = NULL;
        author = NULL;
        amount = 0;
    }

    PrintRequest(char* theName, char* theAuthor, int anAmount, int theIndex){
        try{
            name = new char[strlen(theName)+1];
            strcpy(name, theName);
        }
        catch(bad_alloc& ba){
            cerr << "Bad memory allocation for name found: " << ba.what() << endl;
        }
        try{
        author = new char[strlen(theAuthor)+1];
        strcpy(author, theAuthor);
        }
        catch(bad_alloc& ba){
            cerr << "Bad memory allocation for author found: " << ba.what() << endl;
        }
        amount = anAmount;
        index = theIndex;
    }
    ~PrintRequest(){
        delete[] name;
        delete[] author;
    }

    inline char* getName() const {return name;}
    inline char* getAuthor() const {return author;}
    inline int getAmount() const {return amount;}

    template <typename T>
    void printMember(T aMember) const{
        cout << aMember << endl;
    }
};

struct Comparator{
  bool operator()(const PrintRequest& l, const PrintRequest& r){
    return l.index>r.index;
  }
};

int main(){
    list<PrintRequest> inputList; //create a list of requests
    inputList.push_front(PrintRequest("Novel", "Tom", 10, inputList.size()));
    inputList.push_front(PrintRequest("Tale", "Jack", 20, inputList.size()));
    inputList.push_front(PrintRequest("Poem", "Fred", 30, inputList.size()));
    inputList.push_front(PrintRequest("Story", "Gordon", 40, inputList.size()));
    inputList.push_front(PrintRequest("Guide", "Jim", 50, inputList.size()));

    priority_queue<PrintRequest, vector<PrintRequest>, Comparator> aQueue;
    list<PrintRequest>::iterator l_it = inputList.begin(); //convert list into a priority queue
    while(l_it != inputList.end()){
        aQueue.push(*l_it); //compared by index in the list
        l_it++;
    }

    while(!aQueue.empty()){ //print queue using template function, deleting a queue
        aQueue.top().printMember(aQueue.top().getName());
        aQueue.top().printMember(aQueue.top().getAuthor());
        aQueue.top().printMember(aQueue.top().getAmount());
        cout << endl;
        aQueue.pop();
    }
    return 0;
}
