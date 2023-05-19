/**
 * implement a container like std::map
 */
#ifndef SJTU_MAP_HPP
#define SJTU_MAP_HPP

#include <functional>
#include <cstddef>
#include "utility.hpp"
#include "exceptions.hpp"

namespace sjtu {

	template<
		class Key,
		class T,
		class Compare = std::less<Key>
	>
	class map {
	public:
		/**
		 * the internal type of data.
		 * it should have a default constructor, a copy constructor.
		 * You can use sjtu::map as value_type by typedef.
		 */
		typedef pair<const Key, T> value_type;

		struct knot {
			value_type value;
			int height = 1;
			knot* father = nullptr;
			knot* lc = nullptr;
			knot* rc = nullptr;
			knot(){}

			knot(knot* ff, knot* ll, knot* rr, value_type vv, int hh):value(vv){
				father = ff, lc = ll, rc = rr, height = hh;
			}
		};

		knot* head = nullptr;
		int len = 0;

		/**
		 * see BidirectionalIterator at CppReference for help.
		 *
		 * if there is anything wrong throw invalid_iterator.
		 *     like it = map.begin(); --it;
		 *       or it = map.end(); ++end();
		 */
		class const_iterator;

		class iterator {
		private:
			/**
			 * TODO add data members
			 *   just add whatever you want.
			 */

		public:
			knot* ptr= nullptr;
			map<Key, T, Compare>* ori= nullptr;
			// The following code is written for the C++ type_traits library.
			// Type traits is a C++ feature for describing certain properties of a type.
			// For instance, for an iterator, iterator::value_type is the type that the
			// iterator points to.
			// STL algorithms and containers may use these type_traits (e.g. the following
			// typedef) to work properly.
			// See these websites for more information:
			// https://en.cppreference.com/w/cpp/header/type_traits
			// About value_type: https://blog.csdn.net/u014299153/article/details/72419713
			// About iterator_category: https://en.cppreference.com/w/cpp/iterator
			using difference_type = std::ptrdiff_t;
			using value_type = map::value_type;
			using pointer = value_type*;
			using reference = value_type&;
			using iterator_category = std::output_iterator_tag;
			// If you are interested in type_traits, toy_traits_test provides a place to
			// practice. But the method used in that test is old and rarely used, so you
			// may explore on your own.
			// Notice: you may add some code in here and class const_iterator and namespace sjtu to implement toy_traits_test,
			// this part is only for bonus.
			iterator(){
			}
			iterator(knot* pp, map<Key, T, Compare>* oo){
				ptr = pp,ori=oo;
			}
			iterator(const iterator& other){
				ptr = other.ptr;
				ori = other.ori;
			}
			iterator(const const_iterator& other){
				ptr = other.ptr;
				ori = other.ori;
			}
			iterator& operator =(const const_iterator & other){
				ptr = other.ptr;
				ori = other.ori;
				return *this;
			}
			iterator& operator =(const iterator & other){
				ptr = other.ptr;
				ori = other.ori;
				return *this;
			}
			/**
			 * TODO iter++
			 */
			iterator operator ++(int){
				if(ptr == nullptr)throw invalid_iterator();
				knot* temp = ptr;
				if(ptr->rc!= nullptr){
					ptr = ptr->rc;
					while(ptr->lc!= nullptr)ptr = ptr->lc;
				}
				else{
					knot* x=ptr;
					ptr = ptr->father;
					while (ptr!= nullptr and x==ptr->rc){
						x = ptr;
						ptr = ptr->father;
					}
				}
				return iterator(temp,ori);
			}

			/**
			 * TODO ++iter
			 */
			iterator& operator ++(){
				if(ptr == nullptr and ori)throw invalid_iterator();
				if(ptr->rc!= nullptr){
					ptr = ptr->rc;
					while(ptr->lc!= nullptr)ptr = ptr->lc;
				}
				else{
					knot* x=ptr;
					ptr = ptr->father;
					while (ptr!= nullptr and x==ptr->rc){
						x = ptr;
						ptr = ptr->father;
					}
				}
				return *this;
			}

			/**
			 * TODO iter--
			 */
			iterator operator --(int){
				knot* temp = ptr;
				if(ptr== nullptr){
					ptr = ori->head;
					if(ptr == nullptr)throw invalid_iterator();
					while (ptr->rc)ptr=ptr->rc;
				}
				else if(ptr->lc!= nullptr){
					knot* ans = ptr->lc;
					while(ans->rc!= nullptr)ans = ans->rc;
					ptr = ans;
				}
				else{
					knot* y = ptr->father, *x=ptr;
					while (y!= nullptr and x==y->lc){
						x = y;
						y = y->father;
					}
					if(y== nullptr)throw invalid_iterator();
					ptr = y;
				}
				return iterator(temp,ori);
			}

			/**
			 * TODO --iter
			 */
			iterator& operator --(){
				if(ptr== nullptr){
					ptr = ori->head;
					if(ptr == nullptr)throw invalid_iterator();
					while (ptr->rc)ptr=ptr->rc;
				}
				else if(ptr->lc!= nullptr){
					knot* ans = ptr->lc;
					while(ans->rc!= nullptr)ans = ans->rc;
					ptr = ans;
				}
				else{
					knot* y = ptr->father, *x=ptr;
					while (y!= nullptr and x==y->lc){
						x = y;
						y = y->father;
					}
					if(y== nullptr)throw invalid_iterator();
					ptr = y;
				}
				return *this;
			}

			/**
			 * a operator to check whether two iterators are same (pointing to the same memory).
			 */
			value_type& operator *() const{return (ptr->value);}

			bool operator ==(const iterator& rhs) const{return ptr == rhs.ptr and ori == rhs.ori;}

			bool operator ==(const const_iterator& rhs) const{return ptr == rhs.ptr and ori == rhs.ori;}

			/**
			 * some other operator for iterator.
			 */
			bool operator !=(const iterator& rhs) const{return ptr != rhs.ptr or ori != rhs.ori;}

			bool operator !=(const const_iterator& rhs) const{return ptr != rhs.ptr or ori != rhs.ori;}

			/**
			 * for the support of it->first.
			 * See <http://kelvinh.github.io/blog/2013/11/20/overloading-of-member-access-operator-dash-greater-than-symbol-in-cpp/> for help.
			 */
			value_type* operator ->() const noexcept{return &(ptr->value);}
		};

		class const_iterator {
			// it should has similar member method as iterator.
			//  and it should be able to construct from an iterator.
		private:
			// data members.

		public:
			using difference_type = std::ptrdiff_t;
			using value_type = map::value_type;
			using pointer = value_type*;
			using reference = value_type&;
			using iterator_category = std::input_iterator_tag;
			const_iterator& operator =(const const_iterator & other){
				ptr = other.ptr;
				ori = other.ori;
				return *this;
			}
			const_iterator& operator =(const iterator & other){
				ptr = other.ptr;
				ori = other.ori;
				return *this;
			}
			knot* ptr= nullptr;
			const map<Key, T, Compare>* ori= nullptr;
			const_iterator(){
			}
			const_iterator(knot* pp,const map<Key, T, Compare>* oo){
				ptr = pp,ori=oo;
			}
			const_iterator(const const_iterator& other){
				ptr = other.ptr;
				ori = other.ori;
			}
			const_iterator(const iterator& other){
				ptr = other.ptr;
				ori = other.ori;
			}
			const_iterator operator ++(int){
				if(ptr == nullptr)throw invalid_iterator();
				knot* temp = ptr;
				if(ptr->rc!= nullptr){
					ptr = ptr->rc;
					while(ptr->lc!= nullptr)ptr = ptr->lc;
				}
				else{
					knot*x=ptr;
					ptr=ptr->father;
					while (ptr!= nullptr and x==ptr->rc){
						x = ptr;
						ptr = ptr->father;
					}
				}
				return const_iterator(temp,ori);
			}

			/**
			 * TODO ++iter
			 */
			const_iterator& operator ++(){
				if(ptr == nullptr)throw invalid_iterator();
				if(ptr->rc!= nullptr){
					knot* ans = ptr->rc;
					while(ans->lc!= nullptr)ans = ans->lc;
					ptr = ans;
				}
				else{
					knot* y = ptr->father,* x=ptr;
					while (y!= nullptr and x==y->rc){
						x = y;
						y = y->father;
					}
					ptr = y;
				}
				return *this;
			}

			/**
			 * TODO iter--
			 */
			const_iterator operator --(int){
				knot* temp = ptr;
				if(ptr == nullptr){
					ptr = ori->head;
					if(ptr== nullptr)throw invalid_iterator();
					while (ptr->rc)ptr = ptr->rc;
				}
				else if(ptr->lc!= nullptr){
					knot* ans = ptr->lc;
					while(ans->rc!= nullptr)ans = ans->rc;
					ptr = ans;
				}
				else{
					knot* y = ptr->father, *x=ptr;
					while (y!= nullptr and x==y->lc){
						x = y;
						y = y->father;
					}
					if(y== nullptr)throw invalid_iterator();
					ptr = y;
				}
				return const_iterator(temp,ori);
			}

			/**
			 * TODO --iter
			 */
			const_iterator& operator --(){
				if(ptr == nullptr){
					ptr = ori->head;
					if(ptr== nullptr)throw invalid_iterator();
					while (ptr->rc)ptr = ptr->rc;
				}
				else if(ptr->lc!= nullptr){
					knot* ans = ptr->lc;
					while(ans->rc!= nullptr)ans = ans->rc;
					ptr = ans;
				}
				else{
					knot* y = ptr->father,* x=ptr;
					while (y!= nullptr and x==y->lc){
						x = y;
						y = y->father;
					}
					if(y== nullptr)throw invalid_iterator();
					ptr = y;
				}
				return *this;
			}

			const value_type& operator *() const{return (ptr->value);}

			bool operator ==(const iterator& rhs) const{return ptr == rhs.ptr and ori == rhs.ori;}

			bool operator ==(const const_iterator& rhs) const{return ptr == rhs.ptr and ori == rhs.ori;}

			/**
			 * some other operator for iterator.
			 */
			bool operator !=(const iterator& rhs) const{return ptr != rhs.ptr or ori != rhs.ori;}

			bool operator !=(const const_iterator& rhs) const{return ptr != rhs.ptr or ori != rhs.ori;}

			/**
			 * for the support of it->first.
			 * See <http://kelvinh.github.io/blog/2013/11/20/overloading-of-member-access-operator-dash-greater-than-symbol-in-cpp/> for help.
			 */
			const value_type* operator ->() const noexcept{return &(ptr->value);}
		};

		/**
		 * TODO two constructors
		 */
		knot* copy(knot* other){
			if(other== nullptr)return nullptr;
			knot * ans;
			ans = new knot(nullptr, nullptr, nullptr, other->value, other->height);
			if(other->lc != nullptr){
				ans->lc = copy(other->lc);
				ans->lc->father = ans;
			}
			if(other->rc != nullptr){
				ans->rc = copy(other->rc);
				ans->rc->father = ans;
			}
			return ans;
		}
		void del(knot * other){
			if(other == nullptr)return;
			del(other->lc);
			del(other->rc);
			delete other;
		}
		map(){
			head = nullptr;
		}

		map(const map& other){
			head = copy(other.head);
			len = other.len;
		}

		/**
		 * TODO assignment operator
		 */
		map& operator =(const map& other){
			if(this == &other)return *this;
			del(head);
			len = other.len;
			head = copy(other.head);
			return *this;
		}

		/**
		 * TODO Destructors
		 */
		~map(){
			del(head);
		}

		/**
		 * TODO
		 * access specified element with bounds checking
		 * Returns a reference to the mapped value of the element with key equivalent to key.
		 * If no such element exists, an exception of type `index_out_of_bound'
		 */
		knot *f(const Key& key)const{
			knot* tar = head;
			while (tar != nullptr and
			       (Compare()(key, tar->value.first) or Compare()(tar->value.first, key))){
				if(Compare()(tar->value.first, key))tar = tar->rc;
				else tar = tar->lc;
			}
			if(tar == nullptr)return nullptr;
			return tar;
		}
		T& at(const Key& key){
			knot*ans = f(key);
			if(ans)return ans->value.second;
			else throw index_out_of_bound();
		}

		const T& at(const Key& key) const{
			knot*ans = f(key);
			if(ans)return ans->value.second;
			else throw index_out_of_bound();
		}

		/**
		 * TODO
		 * access specified element
		 * Returns a reference to the value that is mapped to a key equivalent to key,
		 *   performing an insertion if such key does not already exist.
		 */
		T& operator [](const Key& key){
			knot* tar = head,*father=head;
			while (tar != nullptr and
			       (Compare()(key, tar->value.first) or Compare()(tar->value.first, key))){
				if(Compare()(tar->value.first, key))father=tar,tar = tar->rc;
				else father=tar,tar = tar->lc;
			}
			if(tar)return tar->value.second;
			else{
				knot* a = i(value_type(key,T()), father);
				adjust(a->father);
				len++;
				if(head== nullptr)head = a;
				return a->value.second;
			}
		}

		/**
		 * behave like at() throw index_out_of_bound if such key does not exist.
		 */
		const T& operator [](const Key& key) const{
			knot* ans = f(key);
			if(ans)return ans->value.second;
			else throw index_out_of_bound();
		}
		/**
		 * return a iterator to the beginning
		 */
		iterator begin(){
			knot * tar = head;
			while(tar and tar->lc!= nullptr)tar = tar->lc;
			return iterator(tar, this);
		}

		const_iterator cbegin() const{
			knot * tar = head;
			while(tar and tar->lc!= nullptr)tar = tar->lc;
			return const_iterator(tar, this);
		}

		/**
		 * return a iterator to the end
		 * in fact, it returns past-the-end.
		 */
		iterator end(){
			return iterator(nullptr, this);
		}

		const_iterator cend() const{
			return const_iterator(nullptr, this);
		}

		/**
		 * checks whether the container is empty
		 * return true if empty, otherwise false.
		 */
		bool empty() const{return len==0;}

		/**
		 * returns the number of elements.
		 */
		size_t size() const{return len;}

		/**
		 * clears the contents
		 */
		void clear(){
			len = 0;
			del(head);
			head = nullptr;
		}

		/**
		 * insert an element.
		 * return a pair, the first of the pair is
		 *   the iterator to the new element (or the element that prevented the insertion),
		 *   the second one is true if insert successfully, or false.
		 */
		int height(knot * tar){
			if(tar== nullptr)return 0;
			return tar->height;
		}
		void LL(knot * tar){
			knot * a = tar->lc;
			tar->lc = a->rc;
			if(a->rc!= nullptr)a->rc->father = tar;
			a->rc = tar;
			a->father = tar->father;
			if(tar->father!= nullptr){
				if(tar->father->rc == tar)tar->father->rc = a;
				else tar->father->lc = a;
			}
			tar->father = a;
			tar->height = std::max(height(tar->lc), height(tar->rc))+1;
			a->height = std::max(height(a->lc), height(a->rc))+1;
		}
		void RR(knot * tar){
			knot * a = tar->rc;
			tar->rc = a->lc;
			if(a->lc!= nullptr)a->lc->father = tar;
			a->lc = tar;
			a->father = tar->father;
			if(tar->father!= nullptr){
				if(tar->father->rc == tar)tar->father->rc = a;
				else tar->father->lc = a;
			}
			tar->father = a;
			tar->height = std::max(height(tar->lc), height(tar->rc))+1;
			a->height = std::max(height(a->lc), height(a->rc))+1;
		}
		void LR(knot *tar){
			RR(tar->lc);
			LL(tar);
		}
		void RL(knot * tar){
			LL(tar->rc);
			RR(tar);
		}
		knot* i(const value_type& value, knot* tar){
			if(!tar){
				knot* ans = new knot(nullptr, nullptr, nullptr, value, 1);
				head = ans;
				return ans;
			}
			else if(Compare()(value.first, tar->value.first)){

				tar->lc = new knot(tar, nullptr, nullptr, value, 1);
				tar->height = std::max(height(tar->lc), height(tar->rc)) + 1;
				return tar->lc;

			}
			else{

				tar->rc = new knot(tar, nullptr, nullptr, value, 1);
				tar->height = std::max(height(tar->lc), height(tar->rc)) + 1;
				return tar->rc;

			}

		}
		pair<iterator, bool> insert(const value_type& value){
			knot* tar = head,*father=head;
			while (tar != nullptr and
			       (Compare()(value.first, tar->value.first) or Compare()(tar->value.first, value.first))){
				if(Compare()(tar->value.first, value.first))father=tar,tar = tar->rc;
				else father=tar,tar = tar->lc;
			}
			if(tar)return pair<iterator,bool>(iterator(tar,this), false);
			else{
				knot* a = i(value, father);
				adjust(a->father);
				len++;
				if(head== nullptr)head = a;
				return pair<iterator,bool>(iterator(a,this), true);
			}
		}

		/**
		 * erase the element at pos.
		 *
		 * throw if pos pointed to a bad element (pos == this->end() || pos points an element out of this)
		 */
		void erase(iterator pos){
			if(pos.ori != this)throw invalid_iterator();
			len--;
			knot* tar = pos.ptr;
			if(tar== nullptr)throw invalid_iterator();
			if(tar->lc == nullptr){
				if(tar->father!= nullptr){
					if(tar->father->lc == tar)tar->father->lc = tar->rc;
					else tar->father->rc = tar->rc;
				}
				else{
					if(tar == head)head = tar->rc;
				}
				if(tar->rc!= nullptr)tar->rc->father = tar->father;
				adjust(tar->father);
				delete tar;
			}
			else if(tar->rc == nullptr){
				if(tar->father!= nullptr){
					if(tar->father->lc == tar)tar->father->lc = tar->lc;
					else tar->father->rc = tar->lc;
				}
				else{
					if(tar == head)head = tar->lc;
				}
				tar->lc->father = tar->father;
				adjust(tar->father);
				delete tar;
			}
			else{
				iterator temp = ++pos;
				knot* y = temp.ptr;
				if(tar == head)head = y;
				if(y == tar->rc){
					y->father = tar->father;
					if(tar->father!= nullptr){
						if(tar->father->lc == tar)tar->father->lc = y;
						else tar->father->rc = y;
					}
					y->lc = tar->lc;
					if(y->lc!= nullptr)y->lc->father = y;
					adjust(y);
					delete tar;
				}
				else{
					knot* tt = y->father;
					y->father->lc = y->rc;
					if(y->rc!= nullptr)y->rc->father = y->father;
					y->father = tar->father;
					if(tar->father!= nullptr){
						if(tar->father->lc == tar)tar->father->lc = y;
						else tar->father->rc = y;
					}
					y->lc = tar->lc;
					if(y->lc!= nullptr)y->lc->father = y;
					y->rc = tar->rc;
					y->rc->father = y;
					adjust(tt);
					delete tar;
				}
			}

		}

		void adjust(knot* tar){
			if(tar == nullptr)return;
			if(height(tar->lc)- height(tar->rc)==2){
				tar->height = std::max( height(tar->lc), height(tar->rc))+1;
				if(height(tar->lc->lc)> height(tar->lc->rc))LL(tar);
				else LR(tar);
				if(tar == head)head = tar->father;
			}
			else if(height(tar->rc)- height(tar->lc)==2){
				tar->height = std::max( height(tar->lc), height(tar->rc))+1;
				if(height(tar->rc->lc)> height(tar->rc->rc))RL(tar);
				else RR(tar);
				if(tar == head)head = tar->father;
			}
			tar->height = std::max( height(tar->lc), height(tar->rc))+1;
			adjust(tar->father);
		}
		/**
		 * Returns the number of elements with key
		 *   that compares equivalent to the specified argument,
		 *   which is either 1 or 0
		 *     since this container does not allow duplicates.
		 * The default method of check the equivalence is !(a < b || b > a)
		 */
		size_t count(const Key& key) const{
			knot* ans = f(key);
			return (size_t)(ans != nullptr);
		}

		/**
		 * Finds an element with key equivalent to key.
		 * key value of the element to search for.
		 * Iterator to an element with key equivalent to key.
		 *   If no such element is found, past-the-end (see end()) iterator is returned.
		 */
		iterator find(const Key& key){
			knot* ans = f(key);
			if(ans){
				return iterator(ans,this);
			}else return this->end();
		}

		const_iterator find(const Key& key) const{
			knot* ans = f(key);
			if(ans){
				return const_iterator(ans, this);
			}else return this->cend();
		}
	};

	struct my_true_type {

	};

	struct my_false_type{

	};

	template<class Iterator>
	struct my_type_traits{
		typedef typename std::conditional<
			std::is_same<typename Iterator::iterator_category, std::output_iterator_tag>::value,
			my_true_type,
			my_false_type
		>::type iterator_assignable;
	};

}

#endif
