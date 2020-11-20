#include <iostream>
#include <string>

using namespace std;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Human
{
public: 
	Human(){};
	Human(string Name, int Age): name(Name), age(Age){} // Has two member variables: name and age.
	virtual ~Human(){};
	virtual void printInfo(); // Has a function named: printInfo, which prints the name, age information of a human.

protected:
	string name;
	int age;
};

void Human::printInfo(){
	cout << "Human informations" << endl << "Name: " << name << endl << "Age: " << age << endl;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Muggle: public Human
{
public:
	Muggle(){};
	Muggle(string Name, int Age, string Job): Human(Name, Age){ 
		job = Job;
	}
	virtual ~Muggle(){};
	virtual void printInfo();

// Has a member variable: job.
private:
	string job;
};

// Has printInfo function implemented to print job alongside name and age.
void Muggle::printInfo(){
	cout << "Muggle Informations" << endl << "Name: " << name << endl << "Age: " << age << endl << "Job: " << job << endl;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Wizardkind: public Human
{
public:
	Wizardkind(){};
	Wizardkind(string Name, int Age, string Wand): Human(Name, Age){
		wand = Wand;
	}
	virtual ~Wizardkind(){};
	virtual void printInfo();
	void doSpell(string spell);

// Has a member variable: wand.
protected:
	string wand;
};

// Has printInfo function implemented to print wand alongside name and age.
void Wizardkind::printInfo(){
	cout << "Name: " << name << endl << "Age: " << age << endl << "Wand: " << wand << endl;
}


// Has another function, called doSpell which takes a string parameter which is the spell to cast.
void Wizardkind::doSpell(string spell){
	cout << name << " used spell: " << spell << endl;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Student: public Wizardkind
{
public:
	Student(){};
	Student(string Name, int Age, string Wand, string Pet, string House): Wizardkind(Name, Age, Wand){
		pet = Pet;
		houseName = House;
	}
	virtual void printInfo();
	void feedPet();

// Has two more member variables: pet, houseName.
private:
	string pet;
	string houseName;
};

// Has another function called feedPet, which prints a corresponding message.
void Student::feedPet(){
	cout << name << " fed " << pet << endl;
}

// Has printInfo function implemented to print pet and houseName alongside name, age and wand.
void Student::printInfo(){
	cout << "Student Wizardkind Informations" << endl;
	cout << "Name: " << name << endl << "Age: " << age << endl << "House: " << houseName << endl << "Wand: " << wand << endl << "Pet: " << pet << endl;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class GrownUp: public Wizardkind{
public:
	GrownUp(){};
	GrownUp(string Name, int Age, string asa, string Job): Wizardkind(Name, Age, asa){
		job = Job;
	}
	virtual void printInfo();

// Has one additional member variable: job.
private:
	string job;
};

// Has printInfo function implemented to print job alongside name, age and wand.
void GrownUp::printInfo(){
	cout << "Grownup Wizarkind Informations" << endl;
	Wizardkind::printInfo();
	cout << "Job: " << job << endl;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main(){

	Muggle vernon("Vernon Dursley", 50, "Director at Grunnings");
	vernon.printInfo();
	cout << endl;

	Student Harry("Harry Potter", 16, "Phoenix Feather", "Hedwig", "Gryffindor");
	Harry.printInfo();
	cout << endl;

	GrownUp Dumbledore("Albus Dumbledore", 110, "Elder Wand", "Headmaster");
	Dumbledore.printInfo();
	cout << endl;

	Dumbledore.doSpell("Expecto Patronum");
	cout << endl;

	Harry.doSpell("Expelliarmus");
	cout << endl;

	Harry.feedPet();
	cout << endl;

	Student Ginny("Ginny Weasley", 15, "Yew wood", "Arnold", "Gryffindor");
	GrownUp Snape("Severus Snape", 35, "Dragon Heartstring", "Potion Master");
	Student Hermione("Hermione Granger", 16, "Vine", "Crookshanks", "Gryffindor");

	Human hArray[6];

	hArray[0] = vernon;
	hArray[1] = Harry;
	hArray[2] = Dumbledore;
	hArray[3] = Ginny;
	hArray[4] = Snape;
	hArray[5] = Hermione;

	Human * hPtr;

	for (int i = 0; i < 6; i++) {
		cout << endl;
		hPtr = &hArray[i];
		hPtr->printInfo();
	}

	cout << endl;
	system("pause");
	return 0;
}
