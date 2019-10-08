
#include <iostream>
#include <cassert>
#include <map>
using namespace std;
#if !ACTIVATE_THIS_FOR_SEEING_THE_CORRECT_RESULT


//	Array(vector) je sekvencni kontejner reprezentujici pole, ktere muze menit svou velikost.
//	Vektory pouzivaji dynamicky alokovane pole pro ukladani jejich prvku.
//	Nekdy je potreba realokace, napr. pri zvetseni velikosti.
template<typename T>
class Array {
private:
	size_t size_of_array;
	T* data;
	size_t capacity_of_array;

	//	Inicializace konteineru
	void init() {
		size_of_array = 0;
		capacity_of_array = 10;
		data = 0; 
	}

	// Kopirovani dat do konteineru
	void copyFrom(const Array& other) {
		if (!other.data) {
			data = nullptr;
			return;
		}


		T* tmp_data = new T[other.capacity_of_array];
		for (size_t i = 0; i < other.size(); ++i)
		{
			tmp_data[i] = other[i];
		}
		data = tmp_data;
		size_of_array = other.size_of_array;
		capacity_of_array = other.capacity_of_array;

	}

	// Pomocna funkce pro Reserve a Resize
	void tmp_reserve(size_t size) {

		T* newData = new T[size];
		size_t newSize;
		if (size < size_of_array)
			newSize = size;
		else
			newSize = size_of_array;

		for (size_t i = 0; i < newSize; ++i)
			newData[i] = data[i];

		delete[] data;
		data = newData;

		capacity_of_array = size;


	}

public:

	// Konstruktor
	Array()
	{
		init();
	}

	// Destruktor
	~Array()
	{
		delete[] data;
	}

	// Copy konstruktor
	Array(const Array& other) {
		//cout << "Kopie" << endl;
		copyFrom(other);
	}

	// Move konstruktor
	Array(Array&& other) {
		capacity_of_array = other.capacity_of_array;
		size_of_array = other.size_of_array;
		data = other.data;
		other.data = nullptr;
	}

	// Copy assignment
	Array &operator=(const Array &other) {
		if (!other.data) {
			data = nullptr;
			return *this;
		}

		this->~Array();
		copyFrom(other);
		return *this;
	}

	// Move assignment 
	Array &operator=(Array &&other) {
		if (!other.data) {
			data = nullptr;
			return *this;
		}

		
		capacity_of_array = other.capacity_of_array;
		size_of_array = other.size_of_array;
		data = other.data;
		other.data = nullptr;
		return *this;
	}

	//	Swap
	//	Vymenuje obsah kontejneru obsahem other, ktery je jiny objekt stejneho typu. Velikosti mohou byt ruzne.
	//	
	void swap(Array<T> & other) {

		if (!other.data) {
				data = nullptr;
				return;
			}

		size_t tmp_size = size_of_array;
		size_t tmp_capacity = capacity_of_array;
		T* tmp_data = data;

		size_of_array = other.size_of_array;
		capacity_of_array = other.capacity_of_array;
		data = other.data;

		other.size_of_array = tmp_size;
		other.capacity_of_array = tmp_capacity;
		other.data = tmp_data;
	};

	// Vlozeni na konec konteineru. Potrebuje move konstruktor
	void push_back(const T& val)
	{
		if (size_of_array >= capacity_of_array)
			reserve(capacity_of_array * 2);
		data[size_of_array++] = val;
	}

	//Vlozeni na konec konteineru. Vyuziva args jako argumenty pro konstruktor. Nepotrebuje move konstruktor
	template<class ... Args>
	void emplace_back(Args && ... args)
	{
		if (size_of_array >= capacity_of_array)
			reserve(capacity_of_array * 2);
		data[size_of_array++] = move(T(std::forward<Args>(args) ...));
	}

	
	void pop_back()
	{
		if (size_of_array > 0)
			size_of_array--;
		if (size_of_array < capacity_of_array / 4) {
			tmp_reserve(capacity_of_array / 2);
		}
	}

	//	Vrati primou referenci na prvni element. (Na rozdil od Array::begin)
	T& front() const
	{
		return data[0];
	}

	//	Vrati primou referenci na posledni element. (Na rozdil od Array::end)
	T& back() const
	{
		return data[size_of_array - 1];
	}

	//	Capacity
	size_t capacity() const
	{
		return capacity_of_array;
	}

	//	Size
	size_t size() const
	{
		return size_of_array;
	}

	//	Odstrani vsechny prvky z vektoru (ktere jsou zniceny), na vystupu je kontejner o velikosti 0
	void clear()
	{
		this->~Array();
		init();
	}

	//	Zmena velikosti kontejneru. (Aby mohl obsahovat size prvku)
	//	Pokud velikost (size) je mensi nez velikost konteineru, pak obsah se snizi tak, ze odstrani zbytky (zustane jen size prvnich elementu)
	//	Pokud velikost je vetsi, pak obsah se rozsiri do velikosti size vlozenim novych prvku na konec. Vlozi se na konec defaulti hodnoty.
	void resize(size_t size)
	{
		tmp_reserve(size);
		for (auto i = size_of_array; i < size; ++i)
			push_back(T());
		size_of_array = size;

	}

	//	Zmena kapacity 
	//	Pokud je capacity vetsi nez aktualni vektorova kapacita, volani funckce zpusobi, ze kontejner realokuje svou kapacitu na "capacity" (nebo vetsi)
	//	Jinak, volani funkce nazpusobi realokace.
	//	Funkce nemeni velikost vektoru.
	void reserve(size_t capacity) {
		if (capacity > capacity_of_array)
			tmp_reserve(capacity);
	}

	//	Pozadavek, jestli vektor je prazdny
	bool empty() const {
		return !size_of_array;
	}

	void append(const Array &other) {
		for (auto i = 0; i < other.size(); i++)
			push_back(other[i]);
	}

	const T& operator[](const unsigned int index) const {
		return data[index];
	}

	T& operator[](const unsigned int index) {
		return data[index];
	}

	// Iteratory
	class iterator {
		T* curr;
	public:
		explicit iterator(T* curr) : curr(curr) {}


		iterator &operator++() {
			++curr;
			return *this;
		}

		T &operator*() const {
			return *curr;
		}

		T *operator->() const {
			return curr;
		}

		bool operator==(const iterator& other) const {
			return curr == other.curr;
		}

		bool operator!=(const iterator& other) const {
			return curr != other.curr;
		}

	};

public:
	//	Vrati iterator odkazujici na prvni prvek
	iterator begin()
	{
		return iterator(data);
	}

	//	Vrati iterator odkazujici na past-the-end prvek
	//	Jestli kontejner je prazdy, funkce vrati stejny prvek jako Array::begin
	iterator end()
	{
		return iterator(data + size());
	}


};
#endif

#if ACTIVATE_THIS_FOR_SEEING_THE_CORRECT_RESULT
#include <vector>
#define Array vector
#endif

template<typename T>
using Matrix = Array<Array<T>>;

// Matrix multiplication. The arrays are stored by columns. Passing in a
// non-rectangular array of arrays causes undefined behavior.
template<typename T>
Matrix<T> matrix_mult(const Matrix<T>& a, const Matrix<T>& b)
{
	if (a.empty() || b.empty()) return Matrix<T>();
	assert(a.size() == b.front().size());
	size_t h = a.front().size(), w = b.size(), d = a.size();

	Matrix<T> res;
	res.resize(w);
	for (size_t i = 0; i < w; ++i)
		res[i].resize(h); // resize should run default constructors,
	// i.e. assign zero elements to the matrix

	for (size_t i = 0; i < w; ++i)
		for (size_t j = 0; j < h; ++j)
			for (size_t k = 0; k < d; ++k)
				res[i][j] += a[k][j] * b[i][k];

	return res;
}

static constexpr size_t S = 100; //10 

int main()
{
	Matrix<float> a, b, c;
	// first, generate some data
	a.reserve(S);
	b.reserve(S);
	for (size_t i = 0; i < S; ++i) {
		Array<float> col;
		col.reserve(S);
		for (size_t j = 0; j < S; ++j)
			col.push_back(0.3 * i + 0.5 * j);
		a.push_back(col);
		col.pop_back();
		col.push_back(10 * i);
		b.push_back(col);
	}
	// permutations!
	a[5].swap(b[2]);
	b[3].swap(a[0]);

	// run the multiplication
	c = matrix_mult(a, b);

	// empty the matrices in various weird ways
	while (!a.empty()) a.pop_back();
	b.clear();

	// print out the content of 'c'
	for (size_t i = 0; i < S; ++i) {
		for (size_t j = 0; j < S; ++j) cout << c[i][j] << '\t';
		cout << endl;
	}
}
