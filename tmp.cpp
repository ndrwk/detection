#include<iostream> 
#include<thread> 
#include <chrono>
#include <vector>
#include <mutex>
using namespace std;

class Foo {
	const int elementsCount = 10;
public:

	Foo()
	{
		cout << "Constr_" << endl;
	}

	~Foo()
	{
		cout << "Destr_" << endl;
	}

	void func(int a)
	{
		thread::id id = this_thread::get_id();
		cout << "func #" << id << " started" << endl;
		this_thread::sleep_for(chrono::seconds(10));
		cout << "#" << id << "ended" << endl;
	}

	void push(vector<int> &arr, mutex& m_arr, mutex& m_out) {
		cout << "push#" << this_thread::get_id() << " started" << endl;
		int num;
		for (int i = 0; i < elementsCount; i++) {
			m_arr.lock();
			num = rand();
			arr.push_back(num);
			m_arr.unlock();
			m_out.lock();
			cout << "Push " << num << "\n";
			m_out.unlock();
		}
	}

	void pop(vector<int> &arr, mutex& m_arr, mutex& m_out) {
		cout << "pop#" << this_thread::get_id() << " started" << endl;
		int i = 0, num;
		while (i < elementsCount) {
			m_arr.lock();
			if (arr.size() > 0) {
				num = arr.back();
				arr.pop_back();
				m_out.lock();
				cout << "Pop " << num << "\n";
				m_out.unlock();
				i++;
			}
			m_arr.unlock();
		}
	}

};

int tmp() {
	const int c = 5;
	thread::id id;
	Foo obj, obj_pop, obj_push;
	thread thread2(&Foo::func, &obj, c);
	id = thread2.get_id();
	cout << "from main id=#" << this_thread::get_id() << endl;
	//            this_thread::sleep_for(chrono::seconds(1));
	mutex m_arr, m_out;
	vector<int> vec;
	thread push_thread(&Foo::push, &obj_push, ref(vec), ref(m_arr), ref(m_out));
	thread pop_thread(&Foo::pop, &obj_pop, ref(vec), ref(m_arr), ref(m_out));
	if (push_thread.joinable())
	{
		cout << "#" << push_thread.get_id() << " stop" << endl;
		push_thread.join();
	}
	if (pop_thread.joinable())
	{
		cout << "#" << pop_thread.get_id() << " stop" << endl;
		pop_thread.join();
	}
	if (thread2.joinable())
	{
		thread2.join();
		cout << "from main end" << endl;
	}

	return 0;
}
