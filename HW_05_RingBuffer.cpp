#include <iostream>
#include <memory>


template <typename T>
class RingBuffer {
	T* buf_ = nullptr;
	size_t size_ = 10; // размер буфера
	size_t count_ = 0; // количество элементов в буфере
	size_t it_ = 0; // индекс €чейки, следующей за последней записанной
public:
	RingBuffer() {
		buf_ = new T[size_];
	}
	RingBuffer(const size_t size) :
		size_(size)
	{
		buf_ = new T[size_];
	}
	RingBuffer(T buf[], const size_t count) :
		count_(count)
	{
		buf_ = new T[size_];
		for (int i = 0; i < count_; ++i) {
			buf_[i] = buf[i];
			++it_; 
		}
	}
	RingBuffer(const RingBuffer& obj):
		size_(obj.size_),
		count_(obj.count)
	{
		buf_ = new T[size_];
		for (int i = 0; i < obj.size_; ++i) {
			buf_[i] = obj.buf_[i];
			++it_; 
		}
	}
	RingBuffer(RingBuffer&& obj):
		size_(obj.size_),
		count_(obj.count_)
	{
		buf_ = new T[size_];
		for (int i = 0; i < obj.count_; ++i) {
			buf_[i] = obj.buf_[i];
			++it_; 
		}
		obj.buf_ = nullptr;
		obj.count_ = 0;
	}
	~RingBuffer() {
		delete[] buf_;
	}

	size_t get_size() const {
		return size_;
	}
	size_t get_count() const {
		return count_;
	}
	size_t get_it() const {
		return it_;
	}
	size_t resize(size_t newSize) {
		if (newSize > size_ && count_ == size_) {
			int tmp = size_;
			size_ = newSize;
			T* new_buf = new T[size_];
			for (int i = 0; i < count_; ++i) {
				new_buf[i] = buf_[i];
			}
			delete[] buf_;
			buf_ = new_buf;
			it_ = tmp;
			return size_;
		}
		size_ = newSize;
		return size_;
	}
	
	void add(T newElement) {
		
		if (count_ < size_ && it_ < size_) {
			T* it = buf_ + count_;
			*it = newElement;
			++count_;
			++it_;
		}
		else if (count_ == size_ && it_ == size_) {
			T* it = buf_;
			*it = newElement;
			it_ = 0;
			++it_;
		}
		else if (count_ == size_ && it_ < size_) {
			T* it = buf_ + it_;
			*it = newElement;
			++it_;
		}
	}
	
	T del(size_t index) {
		if (count_ == 0) {
			return 0; 
		}
		for (int i = index; i < count_ - 1; ++i) {
			buf_[i] = buf_[i + 1];
		}
		count_ = count_ - 1;
		--it_;
		return buf_[index - 1];
	}
	T del_end() {
		if (count_ == 0) {
			return 0; 
		}
		count_ = count_ - 1;
		--it_;
		return buf_[count_];
	}

	T iter(size_t index) const {
		if (index < 0 || index >= count_) {
			return 0; 
		}
		return buf_[index];
	}

	void print() const {
		for (int i = 0; i < count_; ++i) {
			std::cout << buf_[i] << ' ';
		}
	}
};


template<typename T>
bool check_error(RingBuffer<T>& b, const int b_size, const int b_count) {
	if (b.get_size() != b_size || b.get_count() != b_count) {
		std::cout << "Error" << std::endl;
		return false;
	}
	std::cout << "OK" << std::endl;
	return true;
}


int main() {

	std::cout << "Test for constructors, add- and del- methods: " << std::endl;
	RingBuffer<int> b;
	b.add(1);
	check_error(b, 10, 1);
	
	RingBuffer<int> b0(30);
	check_error(b0, 30, 0);
	
	const int SIZE = 5;
	int arr[SIZE] = { 0, 1, 2, 3, 4 };
	RingBuffer<int> b1(arr, SIZE);
	check_error(b1, 10, 5);
	
	for (int i = 0; i < 5; ++i) {
		b1.add(i + 5);
	}
	check_error(b1, 10, 10);
		
	b1.resize(20);
	check_error(b1, 20, 10);
	
	b1.print();
	std::cout << std::endl;
	std::cout << b1.del(1) << std::endl;
	std::cout << b1.del_end() << std::endl;
	check_error(b1, 20, 8);

	std::cout << b1.iter(b1.get_count() - 1) << std::endl;
	std::cout << std::endl;
	
	std::cout << "Test for ring add elements: " << std::endl;
	b1.print();
	std::cout << std::endl;
	for (int i = 0; i < 13; ++i) {
		b1.add(i);
	}
	b1.print();
	std::cout << std::endl;

	for (int i = 0; i < 13; ++i) {
		b1.add(1);
	}
	b1.print();
	std::cout << std::endl;

	system("pause");
	return 0;
}