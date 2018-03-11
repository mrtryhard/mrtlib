#ifndef MRT_CONTAINERS_CIRCULAR_LIST_HPP_
#define MRT_CONTAINERS_CIRCULAR_LIST_HPP_

#include <algorithm>
#include <cstddef>
#include <initializer_list>
#include <iterator>

namespace mrt { namespace containers {
    template<typename U>
    class circular_iterator {
    public:
        using value_type = U;
        using pointer = U*;
        using const_pointer = const U*;
        using reference = U&;
        using const_reference = const U&;
        using iterator_category = std::random_access_iterator_tag;
        using size_type = const std::size_t;

    private:
        pointer value;
        pointer base;
        size_type max_size;

    private:
        pointer next(pointer position) const noexcept {
	        return (base + ((position - base + 1) % (max_size + 1))); 
        }

        pointer previous(pointer position) const noexcept {
            return (base + ((position - base - 1) % (max_size + 1)));
        }

    public:
        circular_iterator() = default;
        explicit circular_iterator(pointer val, pointer buffer, size_type max_size) : value{val}, base{buffer}, max_size{max_size} {}
        value_type operator*() const noexcept { return *value; }
        bool operator==(const circular_iterator<value_type>& other) { return value == other.value; }
        bool operator!=(const circular_iterator<value_type>& other) { return !(*this == other); }
            
        circular_iterator<value_type> operator++(int) {
            return circular_iterator<value_type>{ next(value), base, max_size };
        }

        circular_iterator<value_type> operator++() {
            return operator++(1);
        }

        circular_iterator<value_type> operator--(int) {
            return circular_iterator<value_type>{ previous(value), base, max_size };
        }

        circular_iterator<value_type> operator--() {
            return operator--(1);
        }
    };

    template<typename T>
    class circular_list {
    public:
        using value_type = T;
        using size_type = std::size_t;
        using pointer = value_type*;
        using const_pointer = const pointer;
        using reference = value_type&;
        using const_reference = const value_type&;
        using iterator = circular_iterator<value_type>;
        using const_iterator = const circular_iterator<value_type>;

    private:
        size_type max_size;        
        pointer buffer;
        pointer head;
        pointer tail;

    public:
        circular_list() = delete;

        explicit circular_list(size_type max_size) 
            : max_size{max_size}, 
            buffer{new value_type[max_size + 1]},
            head{buffer},
            tail{buffer} 
        {
        }
        
        explicit circular_list(std::initializer_list<value_type> list) 
            : max_size{list.size()},
            buffer{new value_type[max_size + 1]},
            head{buffer},
            tail{buffer}
        {
            try {
                for (value_type element : list) {
                    push(element);
                }
            } catch (...) {
                delete [] buffer;
                throw;
            }
        }

        template<typename It>
        circular_list(It first, It last)
            : max_size{std::distance(first, last)} ,
              buffer{new value_type[max_size + 1]},
              head{buffer},
              tail{buffer}
        {
            try {
                std::copy(first, last, std::begin(buffer));
            } catch (...) {
                delete [] buffer;
                throw;
            }
        }

        circular_list(circular_list&& other) noexcept
            : max_size{other.max_size},
            buffer{other.buffer},
            head{buffer},
            tail{buffer}
        {
            other.buffer = {};
            other.head = {};
            other.tail = {};
            other.max_size = {};
        }

        circular_list(circular_list& other) 
            : max_size{other.max_size},
            buffer{new value_type[max_size + 1]},
            head{buffer},
            tail{buffer}
        {
            try {
                std::copy(other.buffer, other.buffer + other.max_size, buffer);
            } catch (...) {
                delete [] buffer;
                throw;
            }
        }

        circular_list<value_type>& operator=(circular_list<value_type>& other) {
            if (this == &other) return *this;
            head = buffer + static_cast<int>((other.buffer - other.head));
            tail = buffer + static_cast<int>((other.buffer - other.tail));
            std::copy(buffer, buffer + max_size, buffer);

            return *this;
        }
        
        circular_list<value_type>& operator=(circular_list<value_type>&& other) {
            delete [] buffer;
            buffer = other.buffer;
            head = other.head;
            tail = other.tail;
            other.buffer = {};
            other.head = {};
            other.tail = {};
            other.max_size = {};

            return *this;
        }

        ~circular_list() {
            delete [] buffer;
        }

        reference front() noexcept {
            return *previous(head);
        }

        const_reference front() const noexcept {
            return *previous(head);
        }

        reference back() noexcept {
            return *tail;
        } 

        const_reference back() const noexcept {
            return *tail;
        }

        void pop() {
            tail = next(tail);
        }

        void push(const value_type& element) {
            *head = element;
            head = next(head);

            if(head == tail) {
		        tail = next(tail);
	        }
        }

        void push(value_type&& element) {
            *head = element;
            head = next(head);
            
            if(head == tail) {
		        tail = next(tail);
	        }
        }

        bool empty() const noexcept {
            return head == tail;
        }

        bool full() const noexcept {
            return next(head) == tail;
        }

        void clear() {
            head = buffer;
            tail = buffer;
        }

        size_type size() const noexcept {
            if (head >= tail) { 
                return static_cast<size_type>(head - tail);
            } else {
                return static_cast<size_type>((head + (max_size + 1) - tail));
            }
        }

        iterator begin() noexcept {
            return iterator{ previous(head), buffer, max_size };
        }

        const_iterator cbegin() const noexcept {
            return const_iterator{ previous(head), buffer, max_size };
        }

        iterator end() noexcept {
            return iterator{ next(tail), buffer, max_size };
        }

        const_iterator cend() const noexcept {
            return const_iterator{ next(tail), buffer, max_size };
        }

    //public:
        //bool operator==(const circular_list<T>& other) const  {
        //    return size() == other.size() && std::equal(begin(), end(), other.begin());
        //}
        //
        //bool operator!=(const Tableau &autre) const  {
        //    return !(*this == autre);
        //}

    private:
        const_pointer next(const_pointer position) const noexcept {
	        return (buffer + ((position - buffer + 1) % (max_size + 1))); 
        }

        const_pointer previous(const_pointer position) const noexcept {
            return (buffer + ((position - buffer - 1) % (max_size + 1)));
        }
    };
}}

#endif
