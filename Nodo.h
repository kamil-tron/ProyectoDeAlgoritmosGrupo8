#pragma once.
template <class T>
class Nodo {
private:
	T elem_;
	Nodo<T>* sgte_;
public:
	explicit Nodo(const T& elem, Nodo<T>* sgte = nullptr)
		: elem_(elem), sgte_(sgte) {
	}
	~Nodo() = default;

	const T& get_Elem() const { return elem_; }
	void set_Elem(const T& elem) { elem_ = elem; }

	Nodo<T>* get_Sgte() const { return sgte_; }
	void set_Sgte(Nodo<T>* sgte) { sgte_ = sgte; }
};