#include "kNN.hpp"

/* TODO: You can implement methods, functions that support your data structures here.
 * */

//class InheritanceList
template<typename T>
InheritanceList<T>::InheritanceList() {
	this->head = NULL;
	this->tail = NULL;
	this->count = 0;
}

template<typename T>
Iterator<T> InheritanceList<T>::begin() {
	return Iterator<T>(this, true);
}

template<typename T>
Iterator<T> InheritanceList<T>::end() {
	return Iterator<T>(this, false);
}

template<typename T>
void InheritanceList<T>::push_back(T value) {
	Node<T>* newNode = new Node<T>(value);

	//insert
	if (this->head == NULL || this->tail == NULL) {
		this->head = newNode;
		this->tail = newNode;
	}
	else {
		this->tail->next = newNode;
		newNode->previous = this->tail;
		this->tail = newNode;
	}
	++this->count;
}

template<typename T>
void InheritanceList<T>::push_front(T value) {
	Node<T>* tmp = new Node<T>(value);
	if (this->head == NULL) {
		this->head = tmp;
		this->tail = tmp;
	}
	else {
		this->head->previous = tmp;
		tmp->next = this->head;
		this->head = tmp;
	}
	this->count++;
}

template<typename T>
void InheritanceList<T>::insert(int index, T value) {
	//invalid case
	if (index<0 || index > this->count) return;

	if (index == 0) {
		this->push_front(value);
		return;
	}
	else if (index == this->count) {
		this->push_back(value);
		return;
	}
	else {
		Node<T>* pre = this->head;
		for (int i = 1; i < index; i++) {
			pre = pre->next;
		}
		Node<T>* post = pre->next;

		//insert
		Node<T>* tmp = new Node<T>(value);
		pre->next = tmp;
		post->previous = tmp;
		tmp->previous = pre;
		tmp->next = post;
	}
	this->count++;
}

template<typename T>
void InheritanceList<T>::remove(int index) {
	//invalid case
	if (this->head == NULL || index < 0 || index >= this->count) return;

	if (index == 0) {
		Node<T>* tmp = this->head;
		if (this->count == 1) {
			this->head = NULL;
			this->tail = NULL;
		}
		else {
			Node<T>* tmp = this->head;
			this->head = this->head->next;
			this->head->previous = NULL;
		}
		delete tmp;
	}
	else if (index == this->count - 1) {
		Node<T>* tmp = this->tail;
		this->tail = this->tail->previous;
		this->tail->next = NULL;
		delete tmp;
	}
	else {
		Node<T>* tmp = this->head;
		for (int i = 0; i < index; i++) {
			tmp = tmp->next;
		}
		Node<T>* pre = tmp->previous;
		Node<T>* post = tmp->next;
		pre->next = post;
		post->previous = pre;
		delete tmp;
	}
	this->count--;
}

template<typename T>
T& InheritanceList<T>::get(int index) const {
	//invalid caserrent->data;
	if (index < 0 || index >= this->count) {
		throw std::out_of_range("get(): Out of range");
	}

	Node<T>* current = this->head;
	for (int i = 0; i < index; i++) {
		current = current->next;
	}
	return current->data;
}

template<typename T>
int InheritanceList<T>::length() const {
	return this->count;
}

template<typename T>
void InheritanceList<T>::clear() {
	while (this->head != NULL) {
		Node<T>* tmp = this->head;
		this->head = this->head->next;
		delete tmp;
	}
	this->tail = NULL;
	this->count = 0;
}

template<typename T>
void InheritanceList<T>::print() const {
	Node<T>* current = this->head;
	while (current != this->tail) {
		cout << current->data << " ";
		current = current->next;
	}
	cout << current->data;
}

template<typename T>
void InheritanceList<T>::reverse() {
	Node<T>* current = this->head;
	this->tail = this->head;
	while (current) {
		Node<T>* tmp = current->previous;
		current->previous = current->next;
		current->next = tmp;
		if (current->previous == NULL) {
			this->head = current;
		}
		current = current->previous;
	}
}


//class Dataset
Dataset::Dataset() {
	this->firstRow = new InheritanceList<string>();
	this->data = new InheritanceList<List<int>*>();
}

Dataset::~Dataset() {
	this->firstRow->clear();
	this->firstRow = NULL;
	for (int i = 0; i < this->data->length(); i++) {
		this->data->get(i)->clear();
	}
	this->data = NULL;
}

Dataset::Dataset(const Dataset& other) {
	this->firstRow = new InheritanceList<string>();
	/*for (int i = 0; i < other.firstRow->length(); i++) {
		this->firstRow->push_back(other.firstRow->get(i));
	}*/
	Iterator<string> it = other.firstRow->begin();
	for (; it != other.firstRow->end(); it++) {
		this->firstRow->push_back(*it);
	}
	this->data = new InheritanceList<List<int>*>();

	Iterator<List<int>*> data = other.data->begin();
	for (int i = 0; i < other.data->length(); i++) {
		List<int>* temp = new InheritanceList<int>;
		Iterator<int> it1 = (*data)->begin();
		for (; it1!=(*data)->end();it1++){
			temp->push_back(*it1);
		}
		this->data->push_back(temp);
		data++;
	}
}

Dataset& Dataset::operator=(const Dataset& other) {
	if (this != &other) { 
		//deep copy
		Iterator<string> it = other.firstRow->begin();
		for (; it != other.firstRow->end(); it++) {
			this->firstRow->push_back(*it);
		}

		Iterator<List<int>*> data = other.data->begin();
		for (int i = 0; i < other.data->length(); i++) {
			List<int>* temp = new InheritanceList<int>;
			Iterator<int> it1 = (*data)->begin();
			for (; it1 != (*data)->end(); it1++) {
				temp->push_back(*it1);
			}
			this->data->push_back(temp);
			data++;
		}
	}

	return *this;
}

bool Dataset::loadFromCSV(const char* fileName) {
	string FileName = string(fileName);
	ifstream file(FileName);

	//if cannot open the file
	if (!file.is_open()) return false;

	string cell, line;
	bool firstline = 1;
	while (getline(file, line)) {
		stringstream ss(line);
		//case Label
		if (firstline) {
			while (getline(ss, cell, ',')) {
				this->firstRow->push_back(cell);
			}
			firstline = 0;
		}
		else {
			List<int>* row = new InheritanceList<int>();
			while (getline(ss, cell, ',')) {
				row->push_back(stoi(cell));
			}
			this->data->push_back(row);
		}
	}
	file.close();
	return true;
}
 
void Dataset::printHead(int nRows, int nCols) const {
	int row = (nRows > this->data->length()) ? this->data->length() : nRows;
	int col = (nCols > this->firstRow->length()) ? this->firstRow->length() : nCols;

	Iterator<string> it = this->firstRow->begin();
	for (int i = 0; i < col; i++) {
		if (i == col - 1) {
			cout << *it;
		}
		else {
			cout << *it << " ";
		}
		it++;
	}	

	Iterator<List<int>*> it1 = this->data->begin();
	for (int i = 0; i < row; i++) {
		cout << endl;
		Iterator<int> it2 = (*it1)->begin();
		for (int j = 0; j < col; j++) {
			if (j == col - 1) {
				cout << (*it2);
			}
			else {
				cout << (*it2) << " ";
			}
			it2++;
		}
		it1++;
	}
}

void Dataset::printTail(int nRows, int nCols) const {
	int row = (nRows > this->data->length()) ? this->data->length() : nRows;
	int col = (nCols > this->firstRow->length()) ? this->firstRow->length() : nCols;

	Iterator<string> it = this->firstRow->begin();
	for (int i = 0; i< this->firstRow->length(); i++) {
		if (i > this->firstRow->length() - col-1) {
			if (i == this->firstRow->length()  -1) {
				cout << *it;
			}
			else {
				cout << *it << " ";
			}
		}
		it++;
	}

	Iterator<List<int>*> it1 = this->data->begin();
	for (int i = 0; i < this->data->length(); i++) {
		if (i >= this->data->length() - row) {
			cout << endl;
			Iterator<int> it2 = (*it1)->begin();
			for (int j = 0; j < this->firstRow->length(); j++) {
				if (j > this->firstRow->length() - col - 1) {
					if (j == this->firstRow->length() - 1) {
						cout << *it2;
					}
					else {
						cout << *it2 << " ";
					}
				}
				it2++;
			}
		}
		it1++;
	}
}

void Dataset::getShape(int& nRows, int& nCols) const {
	if (this->data->length() == 0 || this->firstRow->length() == 0) {
		throw std::out_of_range("get(): Out of range");
	}
	nRows = this->data->length();
	nCols = this->firstRow->length();
}

void Dataset::columns() const {
	this->firstRow->print();
}

bool Dataset::drop(int axis, int index, string columns){
	if (axis != 0 && axis != 1) return false;

	if (axis == 0) {
		if (index < 0 || index >= this->data->length()) return false;
		this->data->remove(index);
	}
	else {
		int index = 0;
		Iterator<string> it = this->firstRow->begin();
		for (; it != this->firstRow->end(); it++) {
			if (*it == columns) {
				break;
			}
			else {
				index++;
			}
		}
		if (it == this->firstRow->end()) return false;
		it.remove();

		Iterator<List<int>*> it1 = this->data->begin();
		for (; it1 != this->data->end(); it1++) {
			(*it1)->remove(index);
		}
	}
	return true;
}

Dataset Dataset::extract(int startRow, int endRow, int startCol, int endCol) const {
	endRow = (endRow == -1) ? this->data->length() - 1 : endRow;
	endCol = (endCol == -1) ? this->firstRow->length() - 1 : endCol;
	
	Dataset extractedDataset;

	Iterator<List<int>*> it = this->data->begin();
	for (int i = 0; i < startRow; i++) {
		it++;
	}
	for (int i = startRow; i <= endRow; i++) {

		List<int>* row = new InheritanceList<int>();
		/*for (int j = startCol; j <= endCol; j++) {
			row->push_back(this->data->get(i)->get(j));
		}*/
		Iterator<int> it1 = (*it)->begin();
		for (int k = 0; k < startCol; k++) {
			it1++;
		}
		for (int j = startCol; j <= endCol; j++) {
			row->push_back(*it1);
			it1++;
		}
		extractedDataset.data->push_back(row);
		it++;
	}
	Iterator<string> it2 = this->firstRow->begin();
	for (int i = 0; i < startCol; i++) {
		it2++;
	}
	for (int i = startCol; i <= endCol; i++) {
		extractedDataset.firstRow->push_back(*it2);
		it2++;
	}
	return extractedDataset;
}

List<List<int>*>* Dataset::getData() const {
	return this->data;
}

//more method
Dataset Dataset::setData(List<List<int>*>* data) {
	Iterator<List<int>*>it = data->begin();
	for (; it != data->end(); it++) {
		List<int>* row = new InheritanceList<int>();
		Iterator<int> it1 = (*it)->begin();
		for (; it1 != (*it)->end();it1++) {
			row->push_back(*it1);
		}
		this->data->push_back(row);
	}
	this->firstRow->push_back("label");
	return *this;
}

//class kNN
kNN::kNN(int k)	 {
	this->k = k;
} 

void kNN::fit(const Dataset& X_train, const Dataset& y_train){
	this->X_train = X_train;
	this->y_train = y_train;
}

double kNN::distance(List<int>* train, List<int>* test) {
	int sum = 0;
	Iterator<int> it1 = train->begin();
	Iterator<int> it2 = test->begin();
	//cout << "DISTANCE:" << (*it1) << " " << (*it2) << endl;
	for (; it1 != train->end(); it1++) {
		sum += ((*it1) - (*it2)) * ((*it1) - (*it2));
		it2++;
	}
	//cout << sum << endl;
	return sqrt(sum);
}

void kNN::SelectionSort(Pair* arr, int n) {
	for (int i = 0; i < n - 1; i++) {
		bool swaped = 0;
		Pair min = arr[i];
		int idx = 0;
		for (int j = i + 1; j < n; j++) {
			if (arr[j] < min) {
				min = arr[j];
				idx = j;
				swaped = 1;
			}
		}
		if (!swaped) return;
		swap(arr[i], arr[idx]);
	}
}

int kNN::MAX(int arr[]) {
	int m = arr[0];
	int tmp = 0;
	for (int i = 1; i < 10; i++) {
		if (arr[i] > m) {
			m = arr[i];
			tmp = i;
		}
	}
	return tmp;
}

Dataset kNN::predict(const Dataset& X_test){
	int rowTrain = this->X_train.getData()->length();
	int rowTest = X_test.getData()->length();
	int col = X_test.getData()->get(0)->length();

	List<List<int>*>* y = new InheritanceList<List<int>*>();
	Iterator<List<int>*> it = X_test.getData()->begin();
	for (int i = 0; i < rowTest; i++) {
		Pair* tmp = new Pair[rowTrain];
		Iterator<List<int>*> it1 = this->X_train.getData()->begin();
		Iterator<List<int>*> it2 = this->y_train.getData()->begin();
		for (int j = 0; j < rowTrain; j++) {
			Pair x;
			x.dist = this->distance((*it1), (*it));
			x.label = (*it2)->get(0);
			it2++;
			it1++;
			tmp[j] = x;
			//cout << x.dist << " " << x.label << endl;
		}
		SelectionSort(tmp, rowTrain);
		int arr[10] = { 0 };
		for (int i = 0; i < this->k; i++) {
			++arr[tmp[i].label];
		}
		List<int>* temp = new InheritanceList<int>();
		temp->push_back(MAX(arr));
		y->push_back(temp);
		tmp->~Pair();
		delete[] tmp;

		it++;
	}

	/*for (int i = 0; i < rowTest; i++) {
		Pair* tmp = new Pair[rowTrain];
		for (int j = 0; j < rowTrain; j++) {
			Pair x;
			x.dist = this->distance(this->X_train.getData()->get(j), X_test.getData()->get(i));
			x.label = y_train.getData()->get(j)->get(0);
			tmp[j] = x;
		}
		SelectionSort(tmp, rowTrain);
		int arr[10] = {0};
		for (int i = 0; i < this->k; i++) {
			++arr[tmp[i].label];
		}
		List<int>* temp = new InheritanceList<int>();
		temp->push_back(MAX(arr));
		y->push_back(temp);
		tmp->~Pair();
		delete[] tmp;
		it++;
	}*/
	Dataset y_pred;
	y_pred.setData(y);	
	return y_pred;



}

double kNN::score(const Dataset& y_test, const Dataset& y_pred){
	int count = 0;
	List<List<int>*>* picPred_y_test = y_test.getData();
	List<List<int>*>* picPred_y_pred = y_pred.getData();
	for (int i = 0; i < picPred_y_test->length(); i++) {
		if (picPred_y_test->get(i)->get(0) == picPred_y_pred->get(i)->get(0)) {
			++count;
		}
	}
	double accuracy = (double)count / (double)picPred_y_pred->length();
	for (int i = 0; i < picPred_y_test->length(); i++) {
		picPred_y_test->clear();
		picPred_y_pred->clear();
	}
	return accuracy;
}

//out of class
void train_test_split(Dataset& X, Dataset& y, double test_size,
	Dataset& X_train, Dataset& X_test, Dataset& y_train, Dataset& y_test){
	int row, dummy;
	X.getShape(row, dummy);
	int rowTest = floor((double)row * (1.0 - test_size));
	X_train = X.extract(0, rowTest - 1, 0, -1); 
	X_test = X.extract(rowTest, -1, 0, -1);   
	y_train = y.extract(0, rowTest - 1, 0, -1); 
	y_test = y.extract(rowTest, -1, 0, -1);
}