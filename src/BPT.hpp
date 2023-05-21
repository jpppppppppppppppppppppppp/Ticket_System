#ifndef _BPT_H
#define _BPT_H

#include <iostream>
#include <fstream>
#include <cstring>
#include <ctime>
#include "exceptions.hpp"
#include <vector>
using std::cout;
using std::cin;

template<int len>
struct my_string {
	char ch[len];

	my_string(){
		memset(ch,0,sizeof(ch));
	}

	my_string(const std::string& s){ memset(ch,0,sizeof(ch));strcpy(ch, s.c_str()); }

	my_string(char* c){ memset(ch,0,sizeof(ch));strcpy(ch, c); }
	inline my_string& operator = (const std::string & str){
		memset(ch,0,sizeof(ch));
		strcpy(ch,str.c_str());
		return *this;
	}
	inline my_string& operator = (const char * str){
		memset(ch,0,sizeof(ch));
		strcpy(ch,str);
		return *this;
	}
	inline friend bool operator <(const my_string& lhs, const my_string& rhs){
		return strcmp(lhs.ch, rhs.ch) < 0;
	}
	inline friend bool operator <(const my_string& lhs, const std::string& rhs){
		return strcmp(lhs.ch, rhs.c_str()) < 0;
	}
	inline friend bool operator <(const std::string& lhs,const my_string& rhs){
		return strcmp(lhs.c_str(), rhs.ch) < 0;
	}
	inline friend bool operator ==(const my_string& lhs, const my_string& rhs){
		return strcmp(lhs.ch, rhs.ch) == 0;
	}
	inline friend bool operator ==(const my_string& lhs, const std::string& rhs){
		return strcmp(lhs.ch, rhs.c_str()) == 0;
	}
	inline friend bool operator ==(const my_string& lhs, const char * rhs){
		return strcmp(lhs.ch, rhs) == 0;
	}
	inline friend bool operator ==(const std::string& lhs,const my_string& rhs){
		return strcmp(lhs.c_str(), rhs.ch) == 0;
	}
	inline friend std::ostream& operator << (std::ostream & io, const my_string& me){
		io << me.ch;
		return io;
	}
};

template<typename T1, typename T2>
T1* lower_bound(T1* begin, T1* end, const T2& tofind)
{
	int dist = end - begin;
	while (dist > 3)
	{
		T1* mid = begin + (dist >> 1);
		if (*mid < tofind)
			begin = mid + 1;
		else
			end = mid + 1;
		dist = end - begin;
	}
	while (begin != end && *begin<tofind) begin++;
	return begin;
}

template<typename T1, typename T2>
T1* upper_bound(T1* begin, T1* end, const T2& tofind)
{
	int dist = end - begin;
	while (dist > 3)
	{
		T1* mid = begin + (dist >> 1);
		if (tofind < *mid)
			end = mid + 1;
		else
			begin = mid + 1;
		dist = end - begin;
	}
	while (begin != end && !(tofind < *begin)) begin++;
	return begin;
}



template<typename data, typename key>
class io_sys {
public:
	struct list_node {
		list_node * pre= nullptr;
		list_node * next= nullptr;
		int index=0;
		data * T = nullptr;
		list_node(list_node * pp, list_node * nn ,int ii, data & TT):pre(pp), next(nn), index(ii){T = new data(TT);}
		list_node(list_node * pp, list_node * nn ,int ii):pre(pp), next(nn), index(ii){T = new data();}
		list_node(){}
	};
	list_node * head;
	list_node * tail;

	int block_num = 0;
	int root = 0;
	struct hash_node{
		int index;
		list_node * address= nullptr;
		hash_node * next= nullptr;
		hash_node(int ii, list_node * ll, hash_node* hh):index(ii), address(ll), next(hh){}
		hash_node(){}
	};
	static const int hash_size = 1024 * 1024 / sizeof(hash_node);
	hash_node hash_map[hash_size];
	int hash_map_size = 0;
	std::fstream io;
	std::fstream other;
	std::string filename;
	io_sys(std::string& file):filename(file){
		io.open(filename);
		if(!io.good()){
			io.open(filename, std::ios::out);
			io.close();
			io.open(filename);
			other.open(filename + "_help", std::ios::out);
			other.close();
			block_num = 0;
			root = 0;
		}else{
			other.open(filename + "_help");
			other.seekg(0);
			other.read(reinterpret_cast<char *>(&block_num), sizeof(int));
			other.read(reinterpret_cast<char *>(&root), sizeof(int));
			other.close();
		}
		head = new list_node;
		tail = new list_node;
		head->next = tail;
		tail->pre = head;
		head->pre = nullptr;
		tail->next = nullptr;
	}
	~io_sys(){
		list_node * tar = head->next;
		delete head;
		while (tar != tail){
			io.seekg((tar->index-1)*sizeof(data));
			io.write(reinterpret_cast<char *>(tar->T), sizeof(data));
			list_node * n = tar->next;
			delete tar->T;
			tar->T = nullptr;
			tar->next->pre = nullptr;
			delete tar;
			tar = n;
		}
		delete tail;
		for(int i = 0; i < hash_size; ++i){
			hash_node * del = hash_map[i].next;
			while (del){
				hash_node * n = del->next;
				delete del;
				del = n;
			}
		}
		io.close();
		other.open(filename + "_help");
		other.seekg(0);
		other.write(reinterpret_cast<char *>(&block_num), sizeof(int));
		other.write(reinterpret_cast<char *>(&root), sizeof(int));
		other.close();
	}
	void write(data* block, int index){
		hash_node* ptr = &(hash_map[index % hash_size]);
		if(ptr->next == nullptr){
			list_node* new_list_node = new list_node(head, head->next, index, (*block));
			new_list_node->next->pre = new_list_node;
			new_list_node->pre->next = new_list_node;
			hash_map[index % hash_size].next = new hash_node(index, new_list_node, nullptr);
			hash_map_size++;
			if(hash_map_size > hash_size){
				list_node* tar = tail->pre;

				tar->pre->next = tar->next;
				tar->next->pre = tar->pre;

				io.seekg((tar->index - 1) * sizeof(data));
				io.write(reinterpret_cast<char*>(tar->T), sizeof(data));

				hash_node* pptr = &(hash_map[tar->index % hash_size]);
				while (pptr->next != nullptr){
					if(pptr->next->index == tar->index){
						hash_node* del = pptr->next;
						pptr->next = del->next;
						delete del;
						delete tar->T;
						delete tar;
						break;
					}
					else{
						pptr = pptr->next;
					}
				}
				hash_map_size--;
				return;
			}
		}
		else{
			while (ptr->next != nullptr){
				if(ptr->next->index == index){
					list_node * tar = ptr->next->address;
					if(tar->pre != head){
						tar->pre->next = tar->next;
						tar->next->pre = tar->pre;
						head->next->pre = tar;
						tar->next = head->next;
						tar->pre = head;
						head->next = tar;
					}
					return;
				}
				else{
					ptr = ptr->next;
				}
			}
			if(hash_map_size == hash_size){
				list_node* tar = tail->pre;

				tar->pre->next = tar->next;
				tar->next->pre = tar->pre;

				io.seekg((tar->index - 1) * sizeof(data));
				io.write(reinterpret_cast<char*>(tar->T), sizeof(data));
				hash_node* pptr = &(hash_map[tar->index % hash_size]);
				while (pptr->next != nullptr){
					if(pptr->next->index == tar->index){
						hash_node* del = pptr->next;
						pptr->next = del->next;
						delete del;
						delete tar->T;
						delete tar;
						break;
					}
					else{
						pptr = pptr->next;
					}
				}
				hash_map_size--;
			}
			ptr = &(hash_map[index % hash_size]);
			list_node* new_list_node = new list_node(head, head->next, index, (*block));
			new_list_node->next->pre = new_list_node;
			new_list_node->pre->next = new_list_node;
			hash_node * new_hash_node = new hash_node(index, new_list_node, nullptr);
			new_hash_node->next = ptr->next;
			ptr->next = new_hash_node;
			hash_map_size++;
		}
	}
	data * read(int index){
		hash_node * ptr = &(hash_map[index % hash_size]);
		if(ptr->next == nullptr){
			list_node * new_list_node = new list_node(head, head->next, index);
			new_list_node->pre->next = new_list_node;
			new_list_node->next->pre = new_list_node;
			io.seekg(0);
			io.seekg((index-1)* sizeof(data));
			io.read(reinterpret_cast<char *>(new_list_node->T), sizeof(data));
			ptr->next = new hash_node(index, new_list_node, nullptr);
			hash_map_size++;
			if(hash_map_size > hash_size){
				list_node* tar = tail->pre;

				tar->pre->next = tar->next;
				tar->next->pre = tar->pre;

				io.seekg((tar->index - 1) * sizeof(data));
				io.write(reinterpret_cast<char*>(tar->T), sizeof(data));

				hash_node* pptr = &(hash_map[tar->index % hash_size]);
				while (pptr->next != nullptr){
					if(pptr->next->index == tar->index){
						hash_node* del = pptr->next;
						pptr->next = del->next;
						delete del;
						delete tar->T;
						delete tar;
						break;
					}
					else{
						pptr = pptr->next;
					}
				}
				hash_map_size--;
				return new_list_node->T;
			}
			return new_list_node->T;
		}else{
			while (ptr->next != nullptr){
				if(ptr->next->index == index){
					list_node * tar = ptr->next->address;
					if(tar->pre != head){
						tar->pre->next = tar->next;
						tar->next->pre = tar->pre;
						head->next->pre = tar;
						tar->next = head->next;
						tar->pre = head;
						head->next = tar;
					}
					return tar->T;
				}
				else{
					ptr = ptr->next;
				}
			}
			if(hash_map_size == hash_size){
				list_node* tar = tail->pre;

				tar->pre->next = tar->next;
				tar->next->pre = tar->pre;

				io.seekg((tar->index - 1) * sizeof(data));
				io.write(reinterpret_cast<char*>(tar->T), sizeof(data));
				hash_node* pptr = &(hash_map[tar->index % hash_size]);
				while (pptr->next != nullptr){
					if(pptr->next->index == tar->index){
						hash_node* del = pptr->next;
						pptr->next = del->next;
						delete del;
						delete tar->T;
						delete tar;
						break;
					}
					else{
						pptr = pptr->next;
					}
				}
				hash_map_size--;
			}
			ptr = &(hash_map[index % hash_size]);
			list_node* new_list_node = new list_node(head, head->next, index);
			new_list_node->next->pre = new_list_node;
			new_list_node->pre->next = new_list_node;
			io.seekg((index-1)* sizeof(data));
			io.read(reinterpret_cast<char *>(new_list_node->T), sizeof(data));
			hash_node * new_hash_node = new hash_node(index, new_list_node, nullptr);
			new_hash_node->next = ptr->next;
			ptr->next = new_hash_node;
			hash_map_size++;
			return new_list_node->T;
		}
	}
	data * get_new_block(){
		int index = ++block_num;
		list_node* new_list_node = new list_node(head, head->next, index);
		new_list_node->T->index = index;
		new_list_node->next->pre = new_list_node;
		new_list_node->pre->next = new_list_node;
		hash_map_size++;
		hash_node * ptr = &(hash_map[index % hash_size]);
		ptr->next = new hash_node(index, new_list_node, ptr->next);

		if(hash_map_size > hash_size){
			list_node* tar = tail->pre;

			tar->pre->next = tar->next;
			tar->next->pre = tar->pre;

			io.seekg((tar->index - 1) * sizeof(data));
			io.write(reinterpret_cast<char*>(tar->T), sizeof(data));

			hash_node* pptr = &(hash_map[tar->index % hash_size]);
			while (pptr->next != nullptr){
				if(pptr->next->index == tar->index){
					hash_node* del = pptr->next;
					pptr->next = del->next;
					delete del;
					delete tar->T;
					delete tar;
					break;
				}
				else{
					pptr = pptr->next;
				}
			}
			hash_map_size--;
			return new_list_node->T;
		}
		return new_list_node->T;
	}
	void delete_block(data * block, int index){
		hash_node * ptr = &(hash_map[index % hash_size]);
		while (ptr->next != nullptr){
			if(ptr->next->index == index){
				hash_node * del = ptr->next;
				ptr->next = del->next;
				list_node * tar = del->address;
				tar->pre->next = tar->next;
				tar->next->pre = tar->pre;
				hash_map_size--;
				delete tar->T;
				delete tar;
				delete del;
				break;
			}
		}
		if(index == block_num)block_num--;
	}
};

template <typename k, typename v>
class BPT {
public:
	struct kv {
		k key = k();
		v value = v();

		kv(){}

		kv(const k& K, const v& V) : key(K), value(V){}

		inline friend bool operator ==(const kv& lhs, const kv& rhs){
			return lhs.key == rhs.key;
		}

		inline friend bool operator <(const kv& lhs, const kv& rhs){
			return lhs.key < rhs.key;
		}

		inline friend bool operator <(const kv& lhs, const k& rhs){
			return lhs.key < rhs;
		}

		inline friend bool operator <(const k& lhs, const kv& rhs){
			return lhs < rhs.key;
		}
	};
	static constexpr int N = 100;
	static constexpr int M = N / 2;
	struct block {
		int size = 0;
		int father = 0;
		kv keys[N];
		int children[N];
		bool is_leaf = false;
		int next = 0;
		int index = 0;

		block(){}
	};

	io_sys<block, int> my_file;
	int root = 0;

	BPT(std::string filename) : my_file(filename){
		root = my_file.root;
	}

	~BPT(){
		my_file.root = root;
	}

	void split_child_block(block* tar, int ind, block* child){
		block* new_block = my_file.get_new_block();
		new_block->is_leaf = child->is_leaf;
		new_block->size = M;
		new_block->father = tar->index;
		child->size = M;
		for(int i = 0; i < M; ++i){
			new_block->keys[i] = child->keys[i + M];
		}
		if(!child->is_leaf){
			for(int i = 0; i < M; ++i){
				new_block->children[i] = child->children[i + M];
			}
		}
		else{
			new_block->next = child->next;
			child->next = new_block->index;
		}
		for(int i = tar->size - 1; i >= ind; i--){
			tar->children[i + 1] = tar->children[i];
			tar->keys[i + 1] = tar->keys[i];
		}
		tar->keys[ind + 1] = new_block->keys[new_block->size - 1];
		tar->children[ind + 1] = new_block->index;
		tar->keys[ind] = child->keys[child->size - 1];
		tar->children[ind] = child->index;
		tar->size++;
		my_file.write(tar, tar->index);
		my_file.write(child, child->index);
		my_file.write(new_block, new_block->index);
	}

	bool bool_Insert(block* cur, kv& value){
		kv* pos = lower_bound(cur->keys, cur->keys + cur->size, value);
		if(cur->size and *pos == value)return false;
		int ind = pos - cur->keys;
		if(cur->is_leaf){
			for(int i = cur->size - 1; i >= ind; --i){
				cur->keys[i + 1] = cur->keys[i];
			}
			cur->keys[ind] = value;
			cur->size++;
			my_file.write(cur, cur->index);
			if(cur->size == N)return true;
			else return false;
		}
		else{
			if(ind == cur->size){
				ind--;
				cur->keys[cur->size - 1] = value;
				my_file.write(cur, cur->index);
			}
			block* todo = my_file.read(cur->children[ind]);
			if(bool_Insert(todo, value)){
				split_child_block(cur, ind, todo);
			}
			if(cur->size == N)return true;
			else return false;
		}
	}

	void Insert(k& key, v value){
		kv new_kv(key, value);
		if(!root){
			block* new_block = my_file.get_new_block();
			root = new_block->index;
			new_block->size = 1;
			new_block->is_leaf = true;
			new_block->keys[0] = new_kv;
			return;
		}
		else{
			block* root_block = my_file.read(root);
			if(bool_Insert(root_block, new_kv)){
				block* new_block = my_file.get_new_block();
				new_block->is_leaf = false;
				root_block->father = new_block->index;
				new_block->children[0] = root_block->index;
				new_block->keys[0] = root_block->keys[root_block->size - 1];
				new_block->size = 1;
				root = new_block->index;
				split_child_block(new_block, 0, root_block);
			}
		}
	}

	virtual v* Find(k& key){
		v* ans = nullptr;
		if(root){
			block* tar = my_file.read(root);
			while (true){
				int pos = lower_bound(tar->keys, tar->keys + tar->size, key) - tar->keys;
				if(pos == tar->size)return ans;
				if(tar->is_leaf){
					int end = upper_bound(tar->keys, tar->keys + tar->size, key) - tar->keys;
					if(pos == end){
						break;
					}
					ans = &(tar->keys[pos].value);
					return ans;
				}
				else{
					tar = my_file.read(tar->children[pos]);
				}
			}
		}
		return ans;
	}

	void borrow_right(block* father, block* left_children, block* right_children, int ind){
		left_children->children[left_children->size] = right_children->children[0];
		left_children->keys[left_children->size] = right_children->keys[0];
		left_children->size++;
		right_children->size--;
		if(!right_children->is_leaf){
			block* changed_key = my_file.read(right_children->children[0]);
			changed_key->father = left_children->index;
			my_file.write(changed_key, changed_key->index);
		}
		for(int i = 0; i < right_children->size; ++i){
			right_children->keys[i] = right_children->keys[i + 1];
			right_children->children[i] = right_children->children[i + 1];
		}
		right_children->children[right_children->size] = 0;
		right_children->keys[right_children->size] = kv();
		father->keys[ind] = left_children->keys[left_children->size - 1];
		my_file.write(father, father->index);
		my_file.write(left_children, left_children->index);
		my_file.write(right_children, right_children->index);
	}

	void borrow_left(block* father, block* left_children, block* right_children, int ind){
		for(int i = right_children->size - 1; i >= 0; i--){
			right_children->keys[i + 1] = right_children->keys[i];
			right_children->children[i + 1] = right_children->children[i];
		}
		right_children->size++;
		right_children->children[0] = left_children->children[left_children->size - 1];
		right_children->keys[0] = left_children->keys[left_children->size - 1];
		left_children->size--;
		left_children->children[left_children->size] = 0;
		left_children->keys[left_children->size] = kv();
		if(!left_children->is_leaf){
			block* changed_key = my_file.read(right_children->children[0]);
			changed_key->father = right_children->index;
			my_file.write(changed_key, changed_key->index);
		}
		father->keys[ind] = left_children->keys[left_children->size - 1];
		my_file.write(father, father->index);
		my_file.write(left_children, left_children->index);
		my_file.write(right_children, right_children->index);
	}

	void merge_block(block* father, block* left_children, block* right_children, int ind){
		for(int i = 0; i < right_children->size; ++i){
			left_children->keys[left_children->size] = right_children->keys[i];
			left_children->children[left_children->size++] = right_children->children[i];
			if(!right_children->is_leaf){
				block* changed_key = my_file.read(right_children->children[i]);
				changed_key->father = left_children->index;
				my_file.write(changed_key, changed_key->index);
			}
		}
		if(right_children->is_leaf){
			left_children->next = right_children->next;
		}
		my_file.delete_block(right_children, right_children->index);
		for(int i = ind; i < father->size - 1; ++i){
			father->keys[i] = father->keys[i + 1];
			father->children[i] = father->children[i + 1];
		}
		father->keys[ind] = left_children->keys[left_children->size - 1];
		father->children[ind] = left_children->index;
		father->size--;
		father->children[father->size] = 0;
		father->keys[father->size] = kv();
		my_file.write(father, father->index);
		my_file.write(left_children, left_children->index);
	}

	bool bool_Delete(block* cur, kv& value){
		kv* pos = lower_bound(cur->keys, cur->keys + cur->size, value);
		if(cur->is_leaf){
			if(*pos == value){
				int ind = pos - cur->keys;
				for(int i = ind; i < cur->size - 1; ++i){
					cur->keys[i] = cur->keys[i + 1];
				}
				cur->size--;
				if(cur->size < M - 1)return true;
				else return false;
			}
			else return false;
		}
		else{
			int ind = pos - cur->keys;
			if(ind == cur->size)return false;
			block* todo = my_file.read(cur->children[ind]);
			bool f = bool_Delete(todo, value);
			cur->keys[ind] = todo->keys[todo->size - 1];
			if(f){
				block* left = nullptr, * right = nullptr;
				if(ind == 0){
					right = my_file.read(cur->children[ind + 1]);
				}
				else if(ind == cur->size - 1){
					left = my_file.read(cur->children[ind - 1]);
				}
				else{
					left = my_file.read(cur->children[ind - 1]);
					right = my_file.read(cur->children[ind + 1]);
				}
				if(left){
					if(left->size == M - 1)merge_block(cur, left, todo, ind - 1);
					else borrow_left(cur, left, todo, ind - 1);
				}
				else{
					if(right->size == M - 1)merge_block(cur, todo, right, ind);
					else borrow_right(cur, todo, right, ind);
				}
				if(cur->size < M - 1)return true;
				else return false;
			}
			else return false;
		}
	}

	void Delete(k& key, v& value){
		if(root){
			block* root_block = my_file.read(root);
			kv p(key, value);
			bool_Delete(root_block, p);
			if(root_block->size == 1 and !root_block->is_leaf){
				root = root_block->children[0];
				my_file.delete_block(root_block, root_block->index);
				block* new_root = my_file.read(root);
				new_root->father = 0;
				my_file.write(new_root, new_root->index);
			}
		}
	}

	inline bool empty(){
		return root == 0;
	}
};
template <typename k, typename v>
class Multiple_BPT{
public:
	struct kv {
		k key = k();
		v value = v();

		kv(){}

		kv(const k& K, const v& V) : key(K), value(V){}

		inline friend bool operator ==(const kv& lhs, const kv& rhs){
			return lhs.key == rhs.key and lhs.value == rhs.value;
		}

		inline friend bool operator <(const kv& lhs, const kv& rhs){
			if(!(lhs.key == rhs.key)) return lhs.key < rhs.key;
			else return lhs.value < rhs.value;
		}

		inline friend bool operator <(const kv& lhs, const k& rhs){
			return lhs.key < rhs;
		}

		inline friend bool operator <(const k& lhs, const kv& rhs){
			return lhs < rhs.key;
		}
	};
	static constexpr int N = 100;
	static constexpr int M = N / 2;
	struct block {
		int size = 0;
		int father = 0;
		kv keys[N];
		int children[N];
		bool is_leaf = false;
		int next = 0;
		int index = 0;

		block(){}
	};

	io_sys<block, int> my_file;
	int root = 0;

	Multiple_BPT(std::string filename) : my_file(filename){
		root = my_file.root;
	}

	~Multiple_BPT(){
		my_file.root = root;
	}

	void split_child_block(block* tar, int ind, block* child){
		block* new_block = my_file.get_new_block();
		new_block->is_leaf = child->is_leaf;
		new_block->size = M;
		new_block->father = tar->index;
		child->size = M;
		for(int i = 0; i < M; ++i){
			new_block->keys[i] = child->keys[i + M];
		}
		if(!child->is_leaf){
			for(int i = 0; i < M; ++i){
				new_block->children[i] = child->children[i + M];
			}
		}
		else{
			new_block->next = child->next;
			child->next = new_block->index;
		}
		for(int i = tar->size - 1; i >= ind; i--){
			tar->children[i + 1] = tar->children[i];
			tar->keys[i + 1] = tar->keys[i];
		}
		tar->keys[ind + 1] = new_block->keys[new_block->size - 1];
		tar->children[ind + 1] = new_block->index;
		tar->keys[ind] = child->keys[child->size - 1];
		tar->children[ind] = child->index;
		tar->size++;
		my_file.write(tar, tar->index);
		my_file.write(child, child->index);
		my_file.write(new_block, new_block->index);
	}

	bool bool_Insert(block* cur, kv& value){
		kv* pos = lower_bound(cur->keys, cur->keys + cur->size, value);
		if(cur->size and *pos == value)return false;
		int ind = pos - cur->keys;
		if(cur->is_leaf){
			for(int i = cur->size - 1; i >= ind; --i){
				cur->keys[i + 1] = cur->keys[i];
			}
			cur->keys[ind] = value;
			cur->size++;
			my_file.write(cur, cur->index);
			if(cur->size == N)return true;
			else return false;
		}
		else{
			if(ind == cur->size){
				ind--;
				cur->keys[cur->size - 1] = value;
				my_file.write(cur, cur->index);
			}
			block* todo = my_file.read(cur->children[ind]);
			if(bool_Insert(todo, value)){
				split_child_block(cur, ind, todo);
			}
			if(cur->size == N)return true;
			else return false;
		}
	}

	void Insert(k& key, v value){
		kv new_kv(key, value);
		if(!root){
			block* new_block = my_file.get_new_block();
			root = new_block->index;
			new_block->size = 1;
			new_block->is_leaf = true;
			new_block->keys[0] = new_kv;
			return;
		}
		else{
			block* root_block = my_file.read(root);
			if(bool_Insert(root_block, new_kv)){
				block* new_block = my_file.get_new_block();
				new_block->is_leaf = false;
				root_block->father = new_block->index;
				new_block->children[0] = root_block->index;
				new_block->keys[0] = root_block->keys[root_block->size - 1];
				new_block->size = 1;
				root = new_block->index;
				split_child_block(new_block, 0, root_block);
			}
		}
	}

	std::vector<v> Find(k& key){
		std::vector<v> ans;
		if(root){
			block* tar = my_file.read(root);
			while (true){
				int pos = lower_bound(tar->keys, tar->keys + tar->size, key) - tar->keys;
				if(pos == tar->size)return ans;
				if(tar->is_leaf){
					int end = upper_bound(tar->keys, tar->keys + tar->size, key) - tar->keys;
					if(pos == end){
						break;
					}
					for(int i = pos; i < end; ++i){
						ans.push_back(tar->keys[i].value);
					}
					if(tar->next){
						tar = my_file.read(tar->next);
					}
					else break;
				}
				else{
					tar = my_file.read(tar->children[pos]);
				}
			}
		}
		return ans;
	}

	void borrow_right(block* father, block* left_children, block* right_children, int ind){
		left_children->children[left_children->size] = right_children->children[0];
		left_children->keys[left_children->size] = right_children->keys[0];
		left_children->size++;
		right_children->size--;
		if(!right_children->is_leaf){
			block* changed_key = my_file.read(right_children->children[0]);
			changed_key->father = left_children->index;
			my_file.write(changed_key, changed_key->index);
		}
		for(int i = 0; i < right_children->size; ++i){
			right_children->keys[i] = right_children->keys[i + 1];
			right_children->children[i] = right_children->children[i + 1];
		}
		right_children->children[right_children->size] = 0;
		right_children->keys[right_children->size] = kv();
		father->keys[ind] = left_children->keys[left_children->size - 1];
		my_file.write(father, father->index);
		my_file.write(left_children, left_children->index);
		my_file.write(right_children, right_children->index);
	}

	void borrow_left(block* father, block* left_children, block* right_children, int ind){
		for(int i = right_children->size - 1; i >= 0; i--){
			right_children->keys[i + 1] = right_children->keys[i];
			right_children->children[i + 1] = right_children->children[i];
		}
		right_children->size++;
		right_children->children[0] = left_children->children[left_children->size - 1];
		right_children->keys[0] = left_children->keys[left_children->size - 1];
		left_children->size--;
		left_children->children[left_children->size] = 0;
		left_children->keys[left_children->size] = kv();
		if(!left_children->is_leaf){
			block* changed_key = my_file.read(right_children->children[0]);
			changed_key->father = right_children->index;
			my_file.write(changed_key, changed_key->index);
		}
		father->keys[ind] = left_children->keys[left_children->size - 1];
		my_file.write(father, father->index);
		my_file.write(left_children, left_children->index);
		my_file.write(right_children, right_children->index);
	}

	void merge_block(block* father, block* left_children, block* right_children, int ind){
		for(int i = 0; i < right_children->size; ++i){
			left_children->keys[left_children->size] = right_children->keys[i];
			left_children->children[left_children->size++] = right_children->children[i];
			if(!right_children->is_leaf){
				block* changed_key = my_file.read(right_children->children[i]);
				changed_key->father = left_children->index;
				my_file.write(changed_key, changed_key->index);
			}
		}
		if(right_children->is_leaf){
			left_children->next = right_children->next;
		}
		my_file.delete_block(right_children, right_children->index);
		for(int i = ind; i < father->size - 1; ++i){
			father->keys[i] = father->keys[i + 1];
			father->children[i] = father->children[i + 1];
		}
		father->keys[ind] = left_children->keys[left_children->size - 1];
		father->children[ind] = left_children->index;
		father->size--;
		father->children[father->size] = 0;
		father->keys[father->size] = kv();
		my_file.write(father, father->index);
		my_file.write(left_children, left_children->index);
	}

	bool bool_Delete(block* cur, kv& value){
		kv* pos = lower_bound(cur->keys, cur->keys + cur->size, value);
		if(cur->is_leaf){
			if(*pos == value){
				int ind = pos - cur->keys;
				for(int i = ind; i < cur->size - 1; ++i){
					cur->keys[i] = cur->keys[i + 1];
				}
				cur->size--;
				cur->keys[cur->size] = kv();
				if(cur->size < M - 1)return true;
				else return false;
			}
			else return false;
		}
		else{
			int ind = pos - cur->keys;
			if(ind == cur->size)return false;
			block* todo = my_file.read(cur->children[ind]);
			bool f = bool_Delete(todo, value);
			cur->keys[ind] = todo->keys[todo->size - 1];
			if(f){
				block* left = nullptr, * right = nullptr;
				if(ind == 0){
					right = my_file.read(cur->children[ind + 1]);
				}
				else if(ind == cur->size - 1){
					left = my_file.read(cur->children[ind - 1]);
				}
				else{
					left = my_file.read(cur->children[ind - 1]);
					right = my_file.read(cur->children[ind + 1]);
				}
				if(left){
					if(left->size == M - 1)merge_block(cur, left, todo, ind - 1);
					else borrow_left(cur, left, todo, ind - 1);
				}
				else{
					if(right->size == M - 1)merge_block(cur, todo, right, ind);
					else borrow_right(cur, todo, right, ind);
				}
				if(cur->size < M - 1)return true;
				else return false;
			}
			else return false;
		}
	}

	void Delete(k& key, v& value){
		if(root){
			block* root_block = my_file.read(root);
			kv p(key, value);
			bool_Delete(root_block, p);
			if(root_block->size == 1 and !root_block->is_leaf){
				root = root_block->children[0];
				my_file.delete_block(root_block, root_block->index);
				block* new_root = my_file.read(root);
				new_root->father = 0;
				my_file.write(new_root, new_root->index);
			}
		}
	}
	inline bool empty(){
		return root == 0;
	}
};

#endif