#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
using namespace std;

struct Product {
	int prod_id;
	string prod_name;
	int amount;
	Product * next;
	Product(int pid, string pname, int n, Product * next) 
		: prod_id(pid), prod_name(pname), amount(n), next(next) {}
	Product(int pid, string pname, int n)
		: prod_id(pid), prod_name(pname), amount(n), next(nullptr) {}
	Product()
		: prod_id(NULL), prod_name(""), amount(NULL), next(nullptr) {}
};

struct Customer{
	int cust_id;
	string cust_name;
	Customer * prev;
	Customer * next;
	Product * prod;
	Customer(int cid, string cname): cust_id(cid), cust_name(cname), prev(nullptr), next(nullptr), prod(nullptr){}
	Customer() :cust_id(NULL), prev(nullptr), next(nullptr), prod(nullptr) {}
};

Customer * insert_ordered(Customer * head, Customer * customer);
Customer * insert_product_ordered(Customer * customer, Product * product);

Customer * print_customer_data(Customer * head) {
	Customer * tmp = head;
	while(tmp != nullptr){
		cout << "ID: " << tmp->cust_id << " Name: " << tmp->cust_name << endl;
		cout << "Items in the cart: " << endl;
		Product * prd_tmp = tmp->prod;
		while (prd_tmp != nullptr) {
			cout << "Item ID: " << prd_tmp->prod_id << " Item name: " << prd_tmp->prod_name << ".  Amount: " << prd_tmp->amount << endl;
			prd_tmp = prd_tmp->next;
		}
		cout << endl;
		tmp = tmp->next;
	}
	return head;
}

bool is_all_digits(string input) {
	for (char c : input) {
		if (c < '0' || c > '9') {
			return false;
		}
	}
	return true;
}


void print_customer(Customer * cust) {
	Product * prd_tmp = cust->prod;
	cout << "ID: " << cust->cust_id << " Name: " << cust->cust_name << endl;
	while (prd_tmp != nullptr) {
		cout << "Item ID: " << prd_tmp->prod_id << " Item name: " << prd_tmp->prod_name << ".  Amount: " << prd_tmp->amount << endl;
		prd_tmp = prd_tmp->next;
	}
}

Customer * find_customer(Customer * head) {
	string line;
	Customer * c = head;
	cout << "Enter customer's ID or name and surname: ";
	cin.clear();
	//cin.ignore();
	getline(cin, line);
	bool found = false;
	if (is_all_digits(line)) {
		while (c->next && !found) {
			if (c->cust_id == stoi(line)) {
				print_customer(c);
				found = true;
			}
			c = c->next;
		}
		if (c->cust_id == stoi(line)) {//for the last node
			print_customer(c);
			found = true;
		}
	}
	else {
		while (c->next) {
			if (c->cust_name == line) {
				print_customer(c);
				found = true;
			}
			c = c->next;
		}
		if (c->cust_name == line) {// For the last node
			print_customer(c);
			found = true;
		}
	}
	if (!found) {
		cout << "CUSTOMER DOESN'T EXIST. GOING BACK TO MAIN MENU" << endl;
	}
	return head;
}

bool user_exists(Customer * head, int id) {
	Customer * cust = head;
	while (cust) {
		if (cust->cust_id == id) {
			return true;
		}
		cust = cust->next;
		if (cust == nullptr) {
			break;
		}
	}
	return false;
}

Customer * add_customer(Customer * head) {
	int id;
	string name;

	cout << "Enter an ID for the Customer: ";
	cin >> id;
	if (user_exists(head, id)) {
		cout << "Customer with ID: " << id << " already exists. Going back to main menu." << endl;
		return head;
	}
	else {
		cout << "Enter the customer's name and surname: ";
		cin.clear();
		cin.get();
		getline(cin, name);
		head = insert_ordered(head, new Customer(id, name));
		cout << "Customer with ID: " << id << " is inserted to the list." << endl;
	}
	return head;
}


Customer * delete_customer(Customer * head) {
	int id;
	string name;

	cout << "Enter an ID for the customer to be deleted: ";
	cin >> id;
	if (!user_exists(head, id)) {
		cout << "Customer with ID: " << id << " doesn't exists. Going back to main menu." << endl;
		return head;
	}
	else {
		Customer * cust = head;
		while (cust) {
			if (cust->cust_id == id) {
				if(cust->prev == nullptr){
					head = cust->next;
					cust->next->prev = nullptr;
				}
				else if (cust->next == nullptr) {
					cust->prev->next = nullptr;
				}
				else {
					cust->prev->next = cust->next;
					cust->next->prev = cust->prev;
				}
				if (cust->prod) {
					Product * tmp = cust->prod;
					Product * tmp2;
					while (tmp) {
						tmp2 = tmp;
						tmp = tmp->next;
						delete tmp2;
					}
				}
				delete cust;
				cout << "Customer is deleted succesfully." << endl;
				break;
			}
			cust = cust->next;
			if (cust == nullptr) {
				break;
			}
		}
	}
	return head;
}

bool product_exist(int id, Customer *c) {
	Product * prod = c->prod;
	while (prod) {
		if (prod->prod_id == id) {
			return true;
		}
		prod = prod->next;
		if (prod == nullptr) {
			break;
		}
	}
	return false;
}

Customer * add_product(Customer * head) {
	int id;
	string name;

	cout << "Enter an ID for the Customer to add product: ";
	cin >> id;
	if (!user_exists(head, id)) {
		cout << "Customer with ID: " << id << " doesn't exists. Going back to main menu." << endl;
		return head;
	}
	else {
		while (true) {
			string answer;
			cout << "Would you like to add item to cart(Y/y): ";
			cin >> answer;
			if (answer != "Y" && answer != "y") {
				cout << "Going back to main menu." << endl;
				break;
			}
			Customer * c = head;
			while (c->cust_id != id) {
				c = c->next;
			}
			int pid;
			cout << "ID of the item to add to cart: ";
			cin >> pid;
			if (product_exist(pid, c)) {
				int amount;
				cout << "Product already exists. How many would you like to add more: ";
				cin >> amount;
				Product * p = c->prod;
				while (p->prod_id != pid) {
					p = p->next;
				}
				p->amount += amount;
				cout << "Product's amount in the cart increased to: " << p->amount << endl;
			}
			else {
				string pname;
				int pamount;
				cout << "Name of the product: ";
				cin.clear();
				cin.get();
				getline(cin, pname);

				cout << "Amount: ";
				cin >> pamount;

				Product * prod_insert = new Product(pid, pname, pamount);
				c = insert_product_ordered(c, prod_insert);
			}
		}
	}
	return head;
}


Customer *delete_product(Customer * head) {
	int id;
	string name;

	cout << "Enter an ID for the Customer to delete product: ";
	cin >> id;
	if (!user_exists(head, id)) {
		cout << "Customer with ID: " << id << " doesn't exists. Going back to main menu." << endl;
		return head;
	}
	else {
		Customer * c = head;
		while (c->cust_id != id) {
			c = c->next;
		}
		int pid;
		cout << "ID of the item to delete from cart: ";
		cin >> pid;
		if (!product_exist(pid, c)) {
			cout << "Product doesn't exists. Going back to main menu.";
			return head;
		}
		else {
			Product * p = c->prod;
			Product * prev = c->prod;
			if (p->prod_id == pid) {
				c->prod = p->next;
				delete p;
			}
			else {
				p = p->next;
				while (p->prod_id != pid) {
					p = p->next;
					prev = prev->next;
				}
				prev->next = p->next;
				delete p;
			}
			cout << "Product has been deleted." << endl << "Current items in the cart: " << endl;
			print_customer(c);
		}
	}
	return head;
}


Customer * list_product_owners(Customer * head) {
	bool notfound = true;
	Customer * c = head;
	int pid;
	while (c->next) {
		c = c->next;
	}

	cout << "ID of the product to search buyers: ";
	cin >> pid;

	cout << "Buyers of the product with ID: " << pid << " is listed below: " << endl;
	while (c) {
		Product * p = c->prod;
		while (p) {
			if (p->prod_id == pid) {
				cout << "Customer ID: " << c->cust_id << " Customer name: " << c->cust_name << " and Amount: " << p->amount << endl;
				notfound = false;
			}
			p = p->next;
		}
		c = c->prev;
	}

	if (notfound) {
		cout << "There is no one buying the product with the given ID." << endl;
	}
	return head;
}


Customer * print_message(Customer * head) {
	cout << endl;
	cout << "***********************************************************************" << endl
		<< "**************** 0 - EXIT PROGRAM                          ************" << endl
		<< "**************** 1 - PRINT ALL CUSTOMER DATA               ************" << endl
		<< "**************** 2 - FIND A CUSTOMER                       ************" << endl
		<< "**************** 3 - ADD A CUSTOMER                        ************" << endl
		<< "**************** 4 - DELETE A CUSTOMER                     ************" << endl
		<< "**************** 5 - ADD A PRODUCT TO A CUSTOMER           ************" << endl
		<< "**************** 6 - DELETE A PRODUCT FROM A CUSTOMER      ************" << endl
		<< "**************** 7 - LIST THE BUYERS OF A PRODUCT          ************" << endl
		<< "***********************************************************************" << endl;
	cout << endl;
	int option;
	cout << "Pick an option from above (int number from 0 to 7): ";
	cin >> option;
	switch (option)
	{
	case 0:
		cout << "PROGRAM EXITING ... " << endl;
		system("pause");
		exit(0);
	case 1:
		head = print_customer_data(head);
		break;
	case 2:
		head = find_customer(head);
		break;
	case 3:
		head = add_customer(head);
		break;
	case 4:
		head = delete_customer(head);
		break;
	case 5:
		head = add_product(head);
		break;
	case 6:
		head = delete_product(head);
		break;
	case 7:
		head = list_product_owners(head);
		break;
	default:
		cout << "INVALID OPTION!!! Try again" << endl;
	}
	return head;
}

Customer * insert_ordered(Customer * head, Customer * customer) {
	if (head == nullptr) {
		head = customer;
	}
	else {
		Customer * cst = head;
		if (customer->cust_id < head->cust_id) {
			customer->next = cst;
			cst->prev = customer;
			head = customer;
		}
		else {
			while (cst->next != nullptr && customer->cust_id > cst->next->cust_id){
				cst = cst->next;
			}
			Customer * tmp_cst = cst->next;
			cst->next = customer;
			customer->prev = cst;
			customer->next = tmp_cst;
			if (tmp_cst != nullptr) {
				tmp_cst->prev = customer;
			}
		}
	}
	return head;
}


Customer * insert_product_ordered(Customer * customer, Product * product) {
	if (customer->prod == nullptr) {
		customer->prod = product;
	}
	else {
		Product * prd = customer->prod;
		if (product->prod_id < prd->prod_id) {
			product->next = prd;
			prd = product;
			customer->prod = prd;
		}
		else if (product->prod_id == prd->prod_id){
			prd->amount += product->amount;
		}
		else {
			while (prd->next != nullptr && product->prod_id >= prd->next->prod_id) {
					prd = prd->next;
			}
			if (product->prod_id == prd->prod_id) {
				prd->amount += product->amount;
			}
			else {
				Product * tmp_prd = prd->next;
				prd->next = product;
				product->next = tmp_prd;
			}
		}
	}
	return customer;
}

Customer * read_file(string filename, Customer * head) {
	ifstream ifs;
	ifs.open(filename);
	
	if (ifs.fail()) {
		cout << "Could not open the file " << filename << endl;
		exit(0);
	}

	string line;
	Customer * temp_customer = new Customer();
	while (getline(ifs, line)) {
		stringstream ss(line);
		string one_word;
		while (ss >> one_word) {
			Product * temp_product = new Product();
			if (one_word.at(0) == '#') {
				if (temp_customer->cust_id != NULL) {
					head = insert_ordered(head, temp_customer);
//					cout << temp_customer->cust_name << endl;
					temp_customer = new Customer();
				}
				temp_customer->cust_id = stoi(one_word.substr(1, one_word.length() - 2));
				ss >> one_word;
				string name = "";
				while (one_word.at(one_word.length() - 1) != ':') {
					if (name != "") {
						name += " ";
					}
					name += one_word;
					ss >> one_word;
				}
				name += " " + one_word.substr(0, one_word.length() - 1);
				temp_customer->cust_name = name;
			}
			else{
				temp_product->prod_id = stoi(one_word);
				ss >> one_word;
				while (one_word.at(0) != '('){
					if (temp_product->prod_name != "") {
						temp_product->prod_name += " ";
					}
					temp_product->prod_name += one_word;
					ss >> one_word;
				}
				temp_product->amount = stoi(one_word.substr(1, one_word.length()-2));
				temp_customer = insert_product_ordered(temp_customer, temp_product);
				temp_product = new Product();
			}
		}
	}
	head = insert_ordered(head, temp_customer);
	return head;
}


int main() {

	Customer * head = nullptr;

	head = read_file("input.txt", head);

	while (true) {
		head = print_message(head);
	}
	return 0;
}
