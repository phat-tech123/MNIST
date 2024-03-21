#include "main.hpp"

/* TODO: Please design your data structure carefully so that you can work with the given dataset
 *       in this assignment. The below structures are just some suggestions.
 */
template<typename T>
class Iterator;
template<typename T>
class List {
public:
    virtual ~List() = default;
    virtual void push_back(T value) = 0;
    virtual void push_front(T value) = 0;
    virtual void insert(int index, T value) = 0;
    virtual void remove(int index) = 0;
    virtual T& get(int index) const = 0;
    virtual int length() const = 0;
    virtual void clear() = 0;
    virtual void print() const = 0;
    virtual void reverse() = 0;
    virtual Iterator<T> begin() = 0;
    virtual Iterator<T> end() = 0;
};
template<typename T>
class InheritanceList;

template<typename T>
class Node {
public:
    T data;
    Node* next;
    Node* previous;
    friend class InheritanceList<T>;
public:
    Node()
    {
        this->previous = NULL;
        this->next = NULL;
    }

    Node(const T& data)
    {
        this->data = data;
        this->previous = NULL;
        this->next = NULL;
    }
};

template<typename T>
class Iterator
{
private:
    InheritanceList<T>* pList;
    Node<T>* current;
    int index; // is the index of current in pList
    friend class Node<T>;
    friend class InheritanceList<T>;
public:
    Iterator(InheritanceList<T>* pList, bool begin) {
        this->pList = pList;
        if (begin) {
            if (pList->head == NULL) {
                this->current = NULL;
                this->index = -1;
            }
            else {
                this->current = pList->head;
                this->index = 0;
            }
        }
        else {
            this->current = NULL;
            if (pList->head == NULL) {
                this->index = 0;
            }
            else {
                this->index = pList->count;
            }
        }
    }
    Iterator& operator=(const Iterator& iterator) {
        this->pList = iterator.pList;
        this->current = iterator.current;
        this->index = iterator.index;
        return (*this);
    }
    void set(const T& e) {
        if (this->current != NULL) {
            this->current->data = e;
        }
        else {
            throw std::out_of_range("Segmentation fault!");
        }
    }
    T& operator*() {
        if (this->current == NULL) {
            throw std::out_of_range("Segmentation fault!");
        }
        else {
            return this->current->data;
        }
    }
    bool operator!=(const Iterator<T>& iterator) {
        return (this->current != iterator.current) && (this->index != iterator.index);
    }
    bool operator==(const Iterator<T>& iterator) {
        return (this->current == iterator.current) && (this->index == iterator.index);
    }
    void remove() {
        if (this->current == this->pList->head && this->current == this->pList->tail) {
            this->pList->head = NULL;
            this->pList->tail = NULL;
            this->index = -1;
            this->current = NULL;
            this->pList->count--;
        }
        else if (this->current == this->pList->head) {
            this->pList->head = this->pList->head->next;
            this->current = this->pList->head->previous;
            this->index = -1;
            this->pList->count--;
        }
        else if (this->current == this->pList->tail) {
            this->pList->tail = this->pList->tail->previous;
            this->index = -1;
            delete this->current;
            this->current = this->pList->tail;
            this->pList->count--;
        }
        else {
            Node<T>* tmp = current->previous;
            current->previous->next = current->next;
            current->next->previous = current->previous;
            delete current;
            this->index -= 1;
            current = tmp;
            this->pList->count--;
        }
    }

    // Prefix ++ overload
    Iterator& operator++() {
        if (this->index == -1) {
            this->index++;
            this->current = this->pList->head;
        }
        else {
            this->current = this->current->next;
            this->index++;
        }
        return *this;
    }

    // Postfix ++ overload
    Iterator operator++(int) {
        Iterator it = (*this);
        if (this->index == -1) {
            this->current = this->pList->head;
            this->index += 1;
        }
        else {
            this->current = this->current->next;
            this->index += 1;
        }
        return it;
    }
};

template<typename T>
class InheritanceList :public List<T> {
protected:
    Node<T>* head;
    Node<T>* tail;
    int count;
    friend class Iterator<T>;
public:
    InheritanceList();
    void push_back(T value) override;
    void push_front(T value) override;
    void insert(int index, T value) override;
    void remove(int index) override;
    T& get(int index) const override;
    int length() const override;
    void clear() override;
    void print() const override;
    void reverse() override;
    Iterator<T> begin() override;
    Iterator<T> end() override;
};



class Dataset {
private:
    List<List<int>*>* data;
    //You may need to define more
    List<string>* firstRow;
    friend class Iterator<List<int>*>;
public:
    Dataset();
    ~Dataset();
    Dataset(const Dataset& other);
    Dataset& operator=(const Dataset& other);
    bool loadFromCSV(const char* fileName);
    void printHead(int nRows = 5, int nCols = 5) const;
    void printTail(int nRows = 5, int nCols = 5) const;
    void getShape(int& nRows, int& nCols) const;
    void columns() const;
    bool drop(int axis = 0, int index = 0, std::string columns = "");
    Dataset extract(int startRow = 0, int endRow = -1, int startCol = 0, int endCol = -1) const;
    //more method
    List<List<int>*>* getData() const;
    Dataset setData(List<List<int>*>* data);
};


struct Pair {   
    double dist;
    int label;
    bool operator < (const Pair& other) {
        return dist < other.dist;
    }
    ~Pair() {
        dist = 0;
        label = 0;
    }
};

class kNN {
private:
    int k;
    //You may need to define more  
    Dataset X_train;
    Dataset y_train;
public:
    kNN(int k = 5);
    void fit(const Dataset& X_train, const Dataset& y_train);
    Dataset predict(const Dataset& X_test);
    double score(const Dataset& y_test, const Dataset& y_pred);
    //more method
    double distance(List<int>* train, List<int>* test);
    void SelectionSort(Pair* arr, int n);
    int MAX(int arr[]);
};


void train_test_split(Dataset& X, Dataset& y, double test_size, 
                        Dataset& X_train, Dataset& X_test, Dataset& y_train, Dataset& y_test);

// Please add more or modify as needed