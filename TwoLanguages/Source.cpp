#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "DynamicStrQueue.h"
#include "DynamicIntStack.h"
#include "DynamicStrStack.h"
#include "strutils.h"

using namespace std;

int special_regs[5] = {0};// namely CSA CSB CSC CSP CST
int general_regs[20] = {0}; // namely CSG0 CSG1 CSG2 ... CSG19
string variables[20] = {""}; // to store the variable names defined by the user. (e.g. var1, var2)
DynamicStrQueue instruction_queue; // to store the instructions in the queue
int def_counter = 0; // counting the def statements so that general reg - variable match holds
DynamicIntStack memory_stack; // needed for debugger
int instr_counter = 1; // counter for printing the instruction counts in print remaining instructions


void clear_regs_vars() {
	def_counter = 0;
	for (int &i : special_regs) {
		i = 0;
	}
	for (int &i : general_regs) {
		i = 0;
	}
	for (string &i : variables) {
		i = "";
	}
	instruction_queue.clear();
	memory_stack.clear();
}

bool var_exists(string var_name) {
	for (string var : variables) {
		if (var_name == var) {
			return true;
		}
	}
	return false;
}

string get_regname(string var_name) {
	int ctr = 0;
	if (var_exists(var_name)) {
		for (string var : variables) {
			if (var_name == var) {
				break;
			}
			ctr++;
		}
		return("CSG" + itoa(ctr));
	}
	else {
		return var_name; //it must be integer value not a variable name.
	}
}

int get_regval(string var_name) {
	if (var_exists(var_name)) {
		int ctr = 0;
		for (string var : variables) {
			if (var_name == var) {
				break;
			}
			ctr++;
		}
		return(general_regs[ctr]);
	}
	else {
		stoi(var_name); //then it must be an integer value
	}
}

int & get_register(string reg_name) {
	if (reg_name.at(0) == 'C') {
		if (reg_name.substr(0, 3) == "CSG") {
			return general_regs[stoi(reg_name.substr(3, reg_name.length()))];
		}
		else {
			char c = reg_name.at(2);
			switch (c) {
			case 'A':
				return special_regs[0];
				break;
			case 'B':
				return special_regs[1];
				break;
			case 'C':
				return special_regs[2];
				break;
			case 'P':
				return special_regs[3];
				break;
			case 'T':
				return special_regs[4];
				break;
			default:
				break;
			}
		}
	}
}

int get_regval_with_regname(string reg_name) {
	if (reg_name.at(0) == 'C') {
		if (reg_name.substr(0, 3) == "CSG") {
			return general_regs[stoi(reg_name.substr(3, reg_name.length()))];
		}
		else {
			char c = reg_name.at(2);
			switch (c) {
			case 'A':
				return special_regs[0];
				break;
			case 'B':
				return special_regs[1];
				break;
			case 'C':
				return special_regs[2];
				break;
			case 'P':
				return special_regs[3];
				break;
			case 'T':
				return special_regs[4];
				break;
			default:
				break;
			}
		}
	}
	else {
		return stoi(reg_name);//considering it is not a register but just an int value
	}
}

int count_chars(string input, char to_count) {
	int result = 0;
	for (char c : input) {
		if (c == to_count) {
			result++;
		}
	}
	return result;
}

string handle_calculation_infix(string calculation) {
	// 	a7 = ( ( ( a3 * ( a1 + a2 ) ) - a4 ) / ( a5 % a6 ) )
	DynamicStringStack operand_stack, operator_stack;
	istringstream iss(calculation);
	string temp, lhs, rhs, op, instruction = "", result_reg;
	while (iss >> temp) {
		if (temp != "(" && temp != ")" && temp != "+" && temp != "-" && temp != "*" && temp != "/" && temp != "%") {
			operand_stack.push(temp);
		}
		else if (temp == "+" || temp == "-" || temp == "*" || temp == "/" || temp == "%") {
			operator_stack.push(temp);
		}
		else if (temp == ")") {
			operand_stack.pop(rhs);
			operand_stack.pop(lhs);
			operator_stack.pop(op);
			if (rhs == "CSP" && lhs == "CSP") {
				instruction_queue.enqueue("pop");
				instruction_queue.enqueue("mov CST CSP");
				instruction_queue.enqueue("pop");
				lhs = "CSP";
				rhs = "CST";
			}
			else if (rhs == "CSP") {
				instruction_queue.enqueue("pop");
			}
			else if (lhs == "CSP") {
				instruction_queue.enqueue("pop");
			}
			if (op == "+") {
				instruction = "add " + get_regname(lhs) + " " + get_regname(rhs);
				instruction_queue.enqueue(instruction);
				instruction_queue.enqueue("push CSA");
			}
			else if (op == "-") {
				instruction = "sub " + get_regname(lhs) + " " + get_regname(rhs);
				instruction_queue.enqueue(instruction);
				instruction_queue.enqueue("push CSA");
			}
			else if (op == "*") {
				instruction = "mul " + get_regname(lhs) + " " + get_regname(rhs);
				instruction_queue.enqueue(instruction);
				instruction_queue.enqueue("push CSA");
			}
			else if (op == "/") {
				instruction = "div " + get_regname(lhs) + " " + get_regname(rhs);
				instruction_queue.enqueue(instruction);
				instruction_queue.enqueue("push CSB");
			}
			else if (op == "%") {
				instruction = "div " + get_regname(lhs) + " " + get_regname(rhs);
				instruction_queue.enqueue(instruction);
				instruction_queue.enqueue("push CSC");
			}
			operand_stack.push("CSP");
		}
	}
	instruction_queue.enqueue("pop");
	return "CSP";
}


bool transform_source() {//filename will be correctly given.
	instr_counter = 1;
	clear_regs_vars();

	int reg_counter = 0;
	string filename, line;
	cout << "Please give me the input filename: ";
	cin >> filename;
	cin.ignore();
	ifstream ifs;
	ifs.open(filename);
	if (ifs.fail()) {
		return false;
	}
	while (getline(ifs, line)) {
		istringstream iss(line);
		string first_word;
		iss >> first_word;
		if(first_word == "int") {
			string var_name, temp, val;
			iss >> var_name;
			iss >> temp; // to store the "="
			variables[reg_counter] = var_name; //mapping regs to vars
	//		instruction_queue.enqueue("def " + var_name);
			if (line.find("=") != string::npos) {
				
				if (count_chars(line, ' ') > 3) {
					// right side of the initialization includes calculations. int var1 = 5 6 +
					string calculation = line.substr(line.find("=")+2, line.length());
					string result_calc = handle_calculation_infix(calculation);
					instruction_queue.enqueue("mov CSG" + itoa(reg_counter) + " " + result_calc); //mov CSG0 4 or mov CSG4 CSG2
					reg_counter++;

				}
				else {
					//definition with initialization like: int var1 = 12 or int var7 = var2
					iss >> val;
					instruction_queue.enqueue("mov CSG" + itoa(reg_counter) + " " + get_regname(val)); //mov CSG0 4 or mov CSG4 CSG2
					reg_counter++;
				}
			}
			else {
				//only definition like: int var1 or int var2
				reg_counter++;
			}
		}
		else if (first_word == "print") { // print ( 4 ) or print ( var1 ) or print ( 4 8 + )
			string to_print;
			iss >> to_print;
			instruction_queue.enqueue("push " + get_regname(to_print));
			instruction_queue.enqueue("print");

		}
		else if (first_word == "return") { // ends the loop
			instruction_queue.enqueue("ret");
			break;
		}
		else if (line.find("=") != string::npos) {
			// line without a definition but with a assignment like: var1 = var2 or var1 = ( 4 8 + )  or var1 = ( var2 6 - )
			if (line.find("(") == string::npos) {
				// no calculation needed
				string temp, val;
				iss >> temp >> val;
				instruction_queue.enqueue("mov " + get_regname(first_word) + " " + get_regname(val));
			}
			else {
				// calculation needed
				string calculation = line.substr(line.find("=") + 2, line.length());
				string result_calc = handle_calculation_infix(calculation);
				instruction_queue.enqueue("mov " + get_regname(first_word) + " " + result_calc); //mov CSG0 4 or mov CSG4 CSG2
			}
		}
	}
	return true;
}

void run_one_instruction() {
	if (!instruction_queue.isEmpty()) {
		string instruction, tmp;
		instruction_queue.dequeue(instruction);
		istringstream iss(instruction);
		iss >> tmp;
		if (tmp == "push") {
			iss >> tmp;
			if (tmp.substr(0, 2) == "CS") {
				//then it is a register
				if (tmp.substr(0, 3) == "CSG") {
					//then it is a general use register
					int count = stoi(tmp.substr(3, tmp.length()));
					memory_stack.push(general_regs[count]);
				}
				else {
					//it is a special register
					if (tmp == "CSA") {
						memory_stack.push(special_regs[0]);
					}
					else if (tmp == "CSB") {
						memory_stack.push(special_regs[1]);
					}
					else if (tmp == "CSC") {
						memory_stack.push(special_regs[2]);
					}
					else if (tmp == "CSP") {
						memory_stack.push(special_regs[3]);
					}
					else if (tmp == "CST") {
						memory_stack.push(special_regs[4]);
					}
				}
			}
			else {
				//it is an integer value
				memory_stack.push(stoi(tmp));
			}
		}
		else if (tmp == "pop") {
			memory_stack.pop(special_regs[3]);
		}
		else if (tmp == "mov") {
			string param1, param2;
			//param1 is absulately register
			//param2 can be value or a register
			iss >> param1 >> param2;
			get_register(param1) = get_regval_with_regname(param2);
		}
		else if (tmp == "add") {
			string param1, param2;
			iss >> param1 >> param2;
			get_register("CSA") = get_regval_with_regname(param1) + get_regval_with_regname(param2);
		}
		else if (tmp == "sub") {
			string param1, param2;
			iss >> param1 >> param2;
			get_register("CSA") = get_regval_with_regname(param1) - get_regval_with_regname(param2);
		}
		else if (tmp == "mul") {
			string param1, param2;
			iss >> param1 >> param2;
			get_register("CSA") = get_regval_with_regname(param1) * get_regval_with_regname(param2);
		}
		else if (tmp == "div") {
			string param1, param2;
			iss >> param1 >> param2;
			get_register("CSB") = get_regval_with_regname(param1) / get_regval_with_regname(param2);
			get_register("CSC") = get_regval_with_regname(param1) % get_regval_with_regname(param2);
		}
		else if (tmp == "print") {
			int to_print;
			memory_stack.pop(to_print);
			cout << "Print Output: " << to_print << endl;
		}
		else if (tmp == "ret") {
			//it does nothing
		}
	/*	else if (tmp == "def") {
			string var_name;
			iss >> var_name;
			variables[def_counter] = var_name;
			def_counter++;
		}
	*/
	}
	instr_counter++;
}

void run_until_end() {
	if (!instruction_queue.isEmpty()) {
		while (!instruction_queue.isEmpty()) {
			run_one_instruction();
		}
	}
	else {
		cout << "There is no instruction left." << endl;
	}
}


void print_stack() {
	if (memory_stack.isEmpty()) {
		cout << endl << "Currently stack is empty." << endl;
	}
	else {
		IntStackNode * temp_node = memory_stack.GetTopPointer();
		cout << endl << "  Current Stack  " << endl << endl;
		while (temp_node) {
			cout << "\t" << temp_node->value << endl;
			temp_node = temp_node->next;
		}
	}
}


void print_registers() {
	cout << endl << "	  SPECIAL REGISTERS" << endl << endl;
	cout << "		 CSA: " << special_regs[0] << endl;
	cout << "		 CSB: " << special_regs[1] << endl;
	cout << "		 CSC: " << special_regs[2] << endl;
	cout << "		 CSP: " << special_regs[3] << endl;
	cout << "		 CST: " << special_regs[4] << endl;

	cout << endl << "	GENERAL USE REGISTERS" << endl << endl;
	for (int i = 0; i < 20; i++) {
		cout << "		 CSG" << i << ": " << general_regs[i] << endl;
	}
}


void print_next_instruction() {
	if (!instruction_queue.isEmpty()) {
		string next_instruction = instruction_queue.GetFront()->value;
		cout << endl << "Next Instruction is: " << next_instruction << endl;
	}
	else {
		cout << "There is no instruction left." << endl;
	}
}


void print_remaining_instructions(int instr_counter) {
	if (instruction_queue.isEmpty()) {
		cout << endl << "There is no instruction left." << endl;
	}
	else {
		cout << endl << "******** INSTRUCTIONS ******** " << endl;
		StrQueueNode * temp = instruction_queue.GetFront();
		while (temp) {
			cout << "[" << instr_counter << "] " << temp->value << endl;
			temp = temp->next;
			instr_counter++;
		}
	}
}



void print_variables() {
	cout << endl << "******** DEFINED VARIABLES ******** " << endl;
	for (int i = 0; i < 20; i++) {
		if (variables[i] != "") {
			cout << "		 " << variables[i] << ": " << general_regs[i] << endl;
		}
	}
}


int main() {
	bool input_OK = false;
	while (true) {
		cout << endl;
		cout << "***********************************************************************" << endl
		  	 << "**************** 0 - EXIT PROGRAM                          ************" << endl
			 << "**************** 1 - GIVE INPUT FILE                       ************" << endl
			 << "**************** 2 - RUN UNTIL THE END                     ************" << endl
			 << "**************** 3 - RUN ONE INSTRUCTION                   ************" << endl
			 << "**************** 4 - PRINT CURRENT STACK                   ************" << endl
			 << "**************** 5 - PRINT REGISTER VALUES                 ************" << endl
			 << "**************** 6 - PRINT NEXT INSTRUCTION                ************" << endl
			 << "**************** 7 - PRINT REMAINING INSTRUCTIONS          ************" << endl
			 << "**************** 8 - PRINT DEFINED VARIABLES               ************" << endl
			 << "***********************************************************************" << endl;
		cout << endl;
		int option;
		cout << "Pick an option from above (int number from 0 to 8): ";
		cin >> option;
		switch (option)
		{
		case 0:
			cout << "PROGRAM EXITING ... " << endl;
			system("pause");
			exit(0);
		case 1:
			if (transform_source()) {
				input_OK = true;
			}
			else {
				cout << "There is a problem with the input file." << endl;
			}
			break;
		case 2:
			if (input_OK) {
				run_until_end();
			}
			else {
				cout << "First give an input file." << endl;
			}
			break;
		case 3:
			if (input_OK) {
				if (instruction_queue.GetFront()) {
					string next_instruction = instruction_queue.GetFront()->value;
					cout << endl << "Executed instruction: " << next_instruction << endl;
				}
				run_one_instruction();
				print_next_instruction();
			}
			else {
				cout << "First give an input file." << endl;
			}
			break;
		case 4:
			if (input_OK) {
				print_stack();
			}
			else {
				cout << "First give an input file." << endl;
			}
			break;
		case 5:
			if (input_OK) {
				print_registers();
			}
			else {
				cout << "First give an input file." << endl;
			}
			break;
		case 6:
			if (input_OK) {
				print_next_instruction();
			}
			else {
				cout << "First give an input file." << endl;
			}
			break;
		case 7:
			if (input_OK) {
				print_remaining_instructions(instr_counter);
			}
			else {
				cout << "First give an input file." << endl;
			}
			break;
		case 8:
			if (input_OK) {
				print_variables();
			}
			else {
				cout << "First give an input file." << endl;
			}
			break;
		default:
			cout << "INVALID OPTION!!! Try again" << endl;
		}
	}

	return 0;
