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
            cerr << "Error 1: amount is too big in request <<" << theName << ">>" << endl;
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
            cerr << "Error 1: amount is too big in request <<" << inputRequest.getName() << ">>" << endl;
        }
        index = inputRequest.index;
    }

    inline string getName() const {return name;}
    inline string getAuthor() const {return author;}
    inline int getAmount() const {return amount;}
};

class PrintMember{
public:
    template <typename T>
    void operator ()(const T input){
        cout << input << endl;
    }
}PrintMember;


struct Comparator{
  bool operator()(const PrintRequest& l, const PrintRequest& r){
    return l.index>r.index;
  }
};

template <typename type>
void print(type& input){
    while(!input.empty()){ //print queue using template function, (I don't know why), deleting a queue
        PrintMember(input.top().getName());
        PrintMember(input.top().getAuthor());
        PrintMember(input.top().getAmount());
        cout << endl;
        input.pop();
    }
}

int main(){
    list<PrintRequest> inputList(0); //create a list of requests
    inputList.push_front(PrintRequest("Novel", "Tom", 10, inputList.size()));
    inputList.push_front(PrintRequest("Tale", "Jack", 20, inputList.size()));
    inputList.push_front(PrintRequest("Poem", "Fred", 300, inputList.size()));
    inputList.push_front(PrintRequest("Story", "Gordon", 40, inputList.size()));
    inputList.push_front(PrintRequest("Guide", "Jim", 50, inputList.size()));

    priority_queue<PrintRequest, deque<PrintRequest>, Comparator> aQueue;

    list<PrintRequest>::iterator l_it = inputList.begin(); //convert list into a priority queue
    while(l_it != inputList.end()){
        aQueue.push(*l_it); //compared by index in the list
        l_it++;
    }

    print(aQueue);

    return 0;
}
