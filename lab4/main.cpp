#include <iostream>
#include <string>
#include <list>
#include <queue>

#define MAX_AMOUNT 100

using namespace std;

class PrintRequest{
private:
    string name;
    string author;
    int amount;
public:
    int index;
    PrintRequest(){
        name = "";
        author = "";
        amount = 0;
    }

    PrintRequest(string theName, string theAuthor, int anAmount, int theIndex){
        name = theName;
        author = theAuthor;
        try{
            if(anAmount > MAX_AMOUNT) throw 1;
            amount = anAmount;
        }
        catch(int error){
            cerr << "Error 1: amount is too big!" << endl;
        }
        index = theIndex;
    }

    PrintRequest(const PrintRequest &inputRequest){
        name = inputRequest.getName();
        author = inputRequest.getAuthor();
        try{
            if(inputRequest.getAmount() > MAX_AMOUNT) throw 1;
            amount = inputRequest.getAmount();
        }
        catch(int error){
            cerr << "Error 1: amount is too big!" << endl;
        }
        index = inputRequest.index;
    }

    inline string getName() const {return name;}
    inline string getAuthor() const {return author;}
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
    list<PrintRequest> inputList(0); //create a list of requests
    inputList.push_front(PrintRequest("Novel", "Tom", 10, inputList.size()));
    inputList.push_front(PrintRequest("Tale", "Jack", 20, inputList.size()));
    inputList.push_front(PrintRequest("Poem", "Fred", 30, inputList.size()));
    inputList.push_front(PrintRequest("Story", "Gordon", 40, inputList.size()));
    inputList.push_front(PrintRequest("Guide", "Jim", 50, inputList.size()));

    priority_queue<PrintRequest, vector<PrintRequest>, Comparator> aQueue;
    queue<PrintRequest> anotherQueue;

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
