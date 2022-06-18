/*
main.cpp
2022-06-17
Collin Abraham

A short program that demonstrates a few of the major changes that came with the C++ 2011 standard 

*/

#include <algorithm>
#include <vector>
#include <iostream> 
#include <memory>

using namespace std;

/* cpp11 - ranged for loops
* simplfied the ability to traverse an entire container
* previous to this (cpp98) you'd usually create an iterator (or const_interator)
* set to the container.begin(), and loop until container.end()
* and perform whatever functionality you wanted on those elements
*/
template<typename VEC_TYPE>
void modify_vector(vector<VEC_TYPE>& v) {
	for (auto& x : v)
		x = rand() % 10;	// modify each element to be a random value that is modulo 10
	
}

/* fills a vector with 100 elements, each element contains an increasing ++i ... 0 - 99 */
template<typename VEC_TYPE>
void fill_vector(vector<VEC_TYPE>& v, const int& vecSize) {
	for (int i = 0; i < vecSize; ++i)
		v.push_back(i); 
}

/* cpp11 - auto
*  cpp11 - lambdas
* 
 Auto allows the compiler to decide the type of something when you are sure that
 you don't really care about what type is returned. Varying opinions on using auto
 as of today but it make generic programming super valuable.

 Lambdas are small functions that usually do a simple comparison to return a boolean
 value.. in this function it is used to create a constant iterator to the first occurence
 of an even number

 This function reads in a generic container type and runs std::find_if.. it will loop
 through the container and try to find a match which is an even number, based on the lambda predicate passed to it 
 */

template<typename VEC_TYPE>
void find_if_example(const vector<VEC_TYPE>& v) {
	const auto ptr = find_if(v.begin(), v.end(),
		[](const VEC_TYPE& n) { return (n % 2) == 0; }	// capture the element of same generic type of vector, named n
	);

	if (ptr != v.end())		// the pointer found something and did not reach the end of the container 
		cout << "First even number found was: " << *ptr << endl;
	else
		cout << "Container did not contain any even numbers" << endl; 
}

/* helper function overloaded to demonstrate variadic functions demonstrated in output_values() */
void output_values() {
	cout << "This is the final function call.. I am an empty function" << endl; 
}

/* cpp11 - variadic templates
* Variadic templates allows a useful application of generic programming whereby we can pass a hypothetical infinite number
* of arguments to a function. Initiated by using three dots ... 
*/
template <typename T, typename... theTypes> 
void output_values(const T& stuff_to_print, theTypes... finalValue) {
	
	cout << stuff_to_print << endl; 

	output_values(finalValue...);
}


/* cpp11 - unique_ptr
* an object that creates a pointer to an object 
* when this pointer goes out of scope it destroys the object
* ownership must be transferred to another unique pointer 
*/
void unique_pointer_example() {
	
	struct A {
		A() { cout << "A Created" << endl; };	// constructor and destructor print to screen when made or destroyed 
		~A() { cout << "A Destroyed" << endl; }; 

		virtual void to_string() { cout << "I am an A" << endl; };
	};

	struct B : A {
		B() { cout << "B:A Created" << endl; };
		~B() { cout << "B:A Destoyed" << endl; };

		void to_string() override { cout << "I am a B" << endl; }; // polymorphically override the to_string 
	};

	/* The following 4 lines of code will:
	1) create a new A and announce it within default constructor
	2) state its type with to_string
	3) create a new B:A by first creating an A, then a B:A
	4) state its type with to_string
	5) B:A is destroyed first, then the abstracted A
	6) original A is destroyed 
	*/
	unique_ptr<A> ptrA = make_unique<A>();
	ptrA->to_string();

	unique_ptr<B> ptrB = make_unique<B>();
	ptrB->to_string();

	ptrA = nullptr;	// explicitly destroy both pointers and objects .. cpp11 feature 
	ptrB = nullptr; 

	cout << endl << "Run-time polymorphism and dynamic dispatch: " << endl; 
	unique_ptr<A> ptrC = make_unique<B>(); //  creates a derived resource
	ptrC->to_string(); 
}

/* cpp11 - shared_ptr
* an object that creates a pointer to an object
* when this pointer goes out of scope it destroys the object
* ownership must be transferred to another unique pointer
*/
void shared_pointer_example() {

	struct A {
		string member;

		A() { cout << "A Created" << endl; member = "A"; };	
		~A() { cout << "A Destroyed" << endl; };

		virtual void to_string() { cout << "I am an A" << endl; };
	};

	struct B : A {
		B() { cout << "B:A Created" << endl; member = "B"; };
		~B() { cout << "B:A Destoyed" << endl; };

		void to_string() override { cout << "I am a B" << endl; }; // polymorphically override the to_string 
	};

	shared_ptr<A> ptrA = make_shared<A>();
	shared_ptr<A> ptrB = ptrA;

	cout << "ptrA points to " << ptrA.get() << endl; // both pointers point to the same obj 
	cout << "ptrB points to " << ptrB.get() << endl;

	cout << endl;
	cout << "ptrA member: " << ptrA->member << endl;
	cout << "ptrB member: " << ptrB->member << endl;

	cout << endl << "ptrA usecount is: " << ptrA.use_count() << endl; 

	ptrA = nullptr; 
	ptrB = nullptr; 


	// pointer casting 
	cout << endl << "---- Pointer casting: \n";
	auto basePtr = make_shared<A>();
	auto derivedPtr = make_shared<B>();

	cout << endl << "basePtr: "; basePtr->to_string();
	basePtr = static_pointer_cast<A>(derivedPtr);
	cout << endl << "basePtr went up class hierarchy! ";
	cout << endl << "basePtr: "; basePtr->to_string();

	// go down a class hierarchy 
	auto downCastPtr = dynamic_pointer_cast<B>(basePtr);
	cout << endl << "downCastPtr: "; downCastPtr->to_string();

	cout << endl << "How many pointers point to the Derived class (B)? " << derivedPtr.use_count() << endl;

}

int main() {

#define POINTER_EXAMPLES true

	// create a vector of ints, fill it with 0-19 incrementally, then modify the vector 
	vector<int> workHorse;
	fill_vector(workHorse, 20); 
	modify_vector(workHorse);

	// show the contents of the vector
	cout << "What is in the vector? " << endl; 
	for (auto x : workHorse)
		cout << x << " ";
	cout << endl; 

	// locate the first even number found 
	find_if_example(workHorse);

	// variadic function example
	// in practice you wouldn't really want to do this iterator dereferencing and stating + 1, + 2 .. but just for demonstration purposes to show it's possible to do it 
	cout << endl << "We'll ask the variadic function to access the values from index 0, ascending" << endl;
	const auto arbiraryValue = workHorse.begin(); 

	output_values( *arbiraryValue
		, *(arbiraryValue + 1)
		, *(arbiraryValue + 2)
		, *(arbiraryValue + 3)
		, *(arbiraryValue + 4)
	);

#if POINTER_EXAMPLES
	// demo of unique pointers
	cout << endl << "---- Unique pointers: " << endl; 
	unique_pointer_example();

	// demo of shared pointers
	cout << endl << "---- Shared pointers: " << endl;
	shared_pointer_example();
#endif
}