#ifndef SJTU_DEQUE_HPP
#define SJTU_DEQUE_HPP

#include <cstddef>
#include <stdexcept>
#include <new>

namespace sjtu {

template<class T>
class deque {
private:
    static const size_t BLOCK_SIZE = 512;
    
    struct Block {
        T* data;
        size_t capacity;
        
        Block() : data(nullptr), capacity(BLOCK_SIZE) {
            data = (T*)operator new(capacity * sizeof(T));
        }
        
        ~Block() {
            operator delete(data);
        }
        
        Block(const Block& other) : data(nullptr), capacity(other.capacity) {
            data = (T*)operator new(capacity * sizeof(T));
        }
        
        Block& operator=(const Block& other) {
            if (this != &other) {
                operator delete(data);
                capacity = other.capacity;
                data = (T*)operator new(capacity * sizeof(T));
            }
            return *this;
        }
    };
    
    Block** blocks;
    size_t block_count;
    size_t block_capacity;
    size_t front_block;
    size_t front_index;
    size_t back_block;
    size_t back_index;
    size_t element_count;
    
    void expand_blocks() {
        size_t new_capacity = block_capacity * 2;
        Block** new_blocks = new Block*[new_capacity];
        
        for (size_t i = 0; i < new_capacity; ++i) {
            new_blocks[i] = nullptr;
        }
        
        size_t offset = (new_capacity - block_count) / 2;
        for (size_t i = 0; i < block_count; ++i) {
            new_blocks[offset + i] = blocks[i];
        }
        
        front_block += offset;
        back_block += offset;
        
        delete[] blocks;
        blocks = new_blocks;
        block_capacity = new_capacity;
    }
    
public:
    class const_iterator;
    class iterator {
    private:
        deque* container;
        size_t block_idx;
        size_t elem_idx;
        
        friend class deque;
        friend class const_iterator;
        
    public:
        iterator() : container(nullptr), block_idx(0), elem_idx(0) {}
        
        iterator(deque* c, size_t b, size_t e) 
            : container(c), block_idx(b), elem_idx(e) {}
        
        iterator(const iterator& other) 
            : container(other.container), block_idx(other.block_idx), elem_idx(other.elem_idx) {}
        
        iterator operator+(const int& n) const {
            iterator result = *this;
            result += n;
            return result;
        }
        
        iterator operator-(const int& n) const {
            iterator result = *this;
            result -= n;
            return result;
        }
        
        int operator-(const iterator& rhs) const {
            if (container != rhs.container) {
                throw std::runtime_error("Invalid iterator operation");
            }
            
            int pos1 = (block_idx - container->front_block) * BLOCK_SIZE + elem_idx - container->front_index;
            int pos2 = (rhs.block_idx - container->front_block) * BLOCK_SIZE + rhs.elem_idx - container->front_index;
            return pos1 - pos2;
        }
        
        iterator& operator+=(const int& n) {
            if (n == 0) return *this;
            
            int current_pos = (block_idx - container->front_block) * BLOCK_SIZE + elem_idx - container->front_index;
            int new_pos = current_pos + n;
            
            if (new_pos < 0 || new_pos > (int)container->element_count) {
                throw std::runtime_error("Iterator out of bounds");
            }
            
            block_idx = container->front_block + new_pos / BLOCK_SIZE;
            elem_idx = container->front_index + new_pos % BLOCK_SIZE;
            
            if (elem_idx >= BLOCK_SIZE) {
                block_idx++;
                elem_idx -= BLOCK_SIZE;
            }
            
            return *this;
        }
        
        iterator& operator-=(const int& n) {
            return *this += (-n);
        }
        
        iterator operator++(int) {
            iterator temp = *this;
            ++(*this);
            return temp;
        }
        
        iterator& operator++() {
            elem_idx++;
            if (elem_idx >= BLOCK_SIZE) {
                block_idx++;
                elem_idx = 0;
            }
            return *this;
        }
        
        iterator operator--(int) {
            iterator temp = *this;
            --(*this);
            return temp;
        }
        
        iterator& operator--() {
            if (elem_idx == 0) {
                block_idx--;
                elem_idx = BLOCK_SIZE - 1;
            } else {
                elem_idx--;
            }
            return *this;
        }
        
        T& operator*() const {
            return container->blocks[block_idx]->data[elem_idx];
        }
        
        T* operator->() const noexcept {
            return &(container->blocks[block_idx]->data[elem_idx]);
        }
        
        bool operator==(const iterator& rhs) const {
            return container == rhs.container && block_idx == rhs.block_idx && elem_idx == rhs.elem_idx;
        }
        
        bool operator==(const const_iterator& rhs) const {
            return container == rhs.container && block_idx == rhs.block_idx && elem_idx == rhs.elem_idx;
        }
        
        bool operator!=(const iterator& rhs) const {
            return !(*this == rhs);
        }
        
        bool operator!=(const const_iterator& rhs) const {
            return !(*this == rhs);
        }
    };
    
    class const_iterator {
    private:
        const deque* container;
        size_t block_idx;
        size_t elem_idx;
        
        friend class deque;
        friend class iterator;
        
    public:
        const_iterator() : container(nullptr), block_idx(0), elem_idx(0) {}
        
        const_iterator(const deque* c, size_t b, size_t e) 
            : container(c), block_idx(b), elem_idx(e) {}
        
        const_iterator(const const_iterator& other) 
            : container(other.container), block_idx(other.block_idx), elem_idx(other.elem_idx) {}
        
        const_iterator(const iterator& other) 
            : container(other.container), block_idx(other.block_idx), elem_idx(other.elem_idx) {}
        
        const_iterator operator+(const int& n) const {
            const_iterator result = *this;
            result += n;
            return result;
        }
        
        const_iterator operator-(const int& n) const {
            const_iterator result = *this;
            result -= n;
            return result;
        }
        
        int operator-(const const_iterator& rhs) const {
            if (container != rhs.container) {
                throw std::runtime_error("Invalid iterator operation");
            }
            
            int pos1 = (block_idx - container->front_block) * BLOCK_SIZE + elem_idx - container->front_index;
            int pos2 = (rhs.block_idx - container->front_block) * BLOCK_SIZE + rhs.elem_idx - container->front_index;
            return pos1 - pos2;
        }
        
        const_iterator& operator+=(const int& n) {
            if (n == 0) return *this;
            
            int current_pos = (block_idx - container->front_block) * BLOCK_SIZE + elem_idx - container->front_index;
            int new_pos = current_pos + n;
            
            if (new_pos < 0 || new_pos > (int)container->element_count) {
                throw std::runtime_error("Iterator out of bounds");
            }
            
            block_idx = container->front_block + new_pos / BLOCK_SIZE;
            elem_idx = container->front_index + new_pos % BLOCK_SIZE;
            
            if (elem_idx >= BLOCK_SIZE) {
                block_idx++;
                elem_idx -= BLOCK_SIZE;
            }
            
            return *this;
        }
        
        const_iterator& operator-=(const int& n) {
            return *this += (-n);
        }
        
        const_iterator operator++(int) {
            const_iterator temp = *this;
            ++(*this);
            return temp;
        }
        
        const_iterator& operator++() {
            elem_idx++;
            if (elem_idx >= BLOCK_SIZE) {
                block_idx++;
                elem_idx = 0;
            }
            return *this;
        }
        
        const_iterator operator--(int) {
            const_iterator temp = *this;
            --(*this);
            return temp;
        }
        
        const_iterator& operator--() {
            if (elem_idx == 0) {
                block_idx--;
                elem_idx = BLOCK_SIZE - 1;
            } else {
                elem_idx--;
            }
            return *this;
        }
        
        const T& operator*() const {
            return container->blocks[block_idx]->data[elem_idx];
        }
        
        const T* operator->() const noexcept {
            return &(container->blocks[block_idx]->data[elem_idx]);
        }
        
        bool operator==(const iterator& rhs) const {
            return container == rhs.container && block_idx == rhs.block_idx && elem_idx == rhs.elem_idx;
        }
        
        bool operator==(const const_iterator& rhs) const {
            return container == rhs.container && block_idx == rhs.block_idx && elem_idx == rhs.elem_idx;
        }
        
        bool operator!=(const iterator& rhs) const {
            return !(*this == rhs);
        }
        
        bool operator!=(const const_iterator& rhs) const {
            return !(*this == rhs);
        }
    };
    
    deque() {
        block_capacity = 8;
        blocks = new Block*[block_capacity];
        for (size_t i = 0; i < block_capacity; ++i) {
            blocks[i] = nullptr;
        }
        
        block_count = 1;
        front_block = back_block = block_capacity / 2;
        blocks[front_block] = new Block();
        front_index = back_index = BLOCK_SIZE / 2;
        element_count = 0;
    }
    
    deque(const deque& other) {
        block_capacity = other.block_capacity;
        blocks = new Block*[block_capacity];
        for (size_t i = 0; i < block_capacity; ++i) {
            blocks[i] = nullptr;
        }
        
        block_count = other.block_count;
        front_block = other.front_block;
        front_index = other.front_index;
        back_block = other.back_block;
        back_index = other.back_index;
        element_count = 0;
        
        for (size_t i = front_block; i <= back_block; ++i) {
            if (other.blocks[i] != nullptr) {
                blocks[i] = new Block();
            }
        }
        
        for (size_t i = 0; i < other.element_count; ++i) {
            push_back(other.at(i));
        }
    }
    
    ~deque() {
        clear();
        for (size_t i = 0; i < block_capacity; ++i) {
            if (blocks[i] != nullptr) {
                delete blocks[i];
            }
        }
        delete[] blocks;
    }
    
    deque& operator=(const deque& other) {
        if (this == &other) return *this;
        
        clear();
        
        for (size_t i = 0; i < block_capacity; ++i) {
            if (blocks[i] != nullptr) {
                delete blocks[i];
            }
        }
        delete[] blocks;
        
        block_capacity = other.block_capacity;
        blocks = new Block*[block_capacity];
        for (size_t i = 0; i < block_capacity; ++i) {
            blocks[i] = nullptr;
        }
        
        block_count = other.block_count;
        front_block = other.front_block;
        front_index = other.front_index;
        back_block = other.back_block;
        back_index = other.back_index;
        element_count = 0;
        
        for (size_t i = front_block; i <= back_block; ++i) {
            if (other.blocks[i] != nullptr) {
                blocks[i] = new Block();
            }
        }
        
        for (size_t i = 0; i < other.element_count; ++i) {
            push_back(other.at(i));
        }
        
        return *this;
    }
    
    T& at(const size_t& pos) {
        if (pos >= element_count) {
            throw std::out_of_range("Index out of range");
        }
        
        size_t target_pos = pos + front_index;
        size_t target_block = front_block + target_pos / BLOCK_SIZE;
        size_t target_index = target_pos % BLOCK_SIZE;
        
        return blocks[target_block]->data[target_index];
    }
    
    const T& at(const size_t& pos) const {
        if (pos >= element_count) {
            throw std::out_of_range("Index out of range");
        }
        
        size_t target_pos = pos + front_index;
        size_t target_block = front_block + target_pos / BLOCK_SIZE;
        size_t target_index = target_pos % BLOCK_SIZE;
        
        return blocks[target_block]->data[target_index];
    }
    
    T& operator[](const size_t& pos) {
        return at(pos);
    }
    
    const T& operator[](const size_t& pos) const {
        return at(pos);
    }
    
    const T& front() const {
        if (element_count == 0) {
            throw std::runtime_error("Container is empty");
        }
        return blocks[front_block]->data[front_index];
    }
    
    const T& back() const {
        if (element_count == 0) {
            throw std::runtime_error("Container is empty");
        }
        return blocks[back_block]->data[back_index];
    }
    
    iterator begin() {
        return iterator(this, front_block, front_index);
    }
    
    const_iterator cbegin() const {
        return const_iterator(this, front_block, front_index);
    }
    
    iterator end() {
        size_t end_idx = back_index + 1;
        size_t end_block = back_block;
        
        if (element_count == 0) {
            return iterator(this, front_block, front_index);
        }
        
        if (end_idx >= BLOCK_SIZE) {
            end_block++;
            end_idx = 0;
        }
        
        return iterator(this, end_block, end_idx);
    }
    
    const_iterator cend() const {
        size_t end_idx = back_index + 1;
        size_t end_block = back_block;
        
        if (element_count == 0) {
            return const_iterator(this, front_block, front_index);
        }
        
        if (end_idx >= BLOCK_SIZE) {
            end_block++;
            end_idx = 0;
        }
        
        return const_iterator(this, end_block, end_idx);
    }
    
    bool empty() const {
        return element_count == 0;
    }
    
    size_t size() const {
        return element_count;
    }
    
    void clear() {
        while (element_count > 0) {
            pop_back();
        }
    }
    
    void push_back(const T& value) {
        if (element_count == 0) {
            new (&blocks[back_block]->data[back_index]) T(value);
            element_count++;
            return;
        }
        
        back_index++;
        if (back_index >= BLOCK_SIZE) {
            back_block++;
            back_index = 0;
            
            if (back_block >= block_capacity) {
                expand_blocks();
            }
            
            if (blocks[back_block] == nullptr) {
                blocks[back_block] = new Block();
                block_count++;
            }
        }
        
        new (&blocks[back_block]->data[back_index]) T(value);
        element_count++;
    }
    
    void pop_back() {
        if (element_count == 0) {
            throw std::runtime_error("Container is empty");
        }
        
        blocks[back_block]->data[back_index].~T();
        element_count--;
        
        if (element_count == 0) {
            back_index = front_index;
            back_block = front_block;
            return;
        }
        
        if (back_index == 0) {
            back_block--;
            back_index = BLOCK_SIZE - 1;
        } else {
            back_index--;
        }
    }
    
    void push_front(const T& value) {
        if (element_count == 0) {
            new (&blocks[front_block]->data[front_index]) T(value);
            element_count++;
            return;
        }
        
        if (front_index == 0) {
            front_block--;
            front_index = BLOCK_SIZE - 1;
            
            if (front_block < 0 || front_block >= block_capacity) {
                expand_blocks();
            }
            
            if (blocks[front_block] == nullptr) {
                blocks[front_block] = new Block();
                block_count++;
            }
        } else {
            front_index--;
        }
        
        new (&blocks[front_block]->data[front_index]) T(value);
        element_count++;
    }
    
    void pop_front() {
        if (element_count == 0) {
            throw std::runtime_error("Container is empty");
        }
        
        blocks[front_block]->data[front_index].~T();
        element_count--;
        
        if (element_count == 0) {
            front_index = back_index;
            front_block = back_block;
            return;
        }
        
        front_index++;
        if (front_index >= BLOCK_SIZE) {
            front_block++;
            front_index = 0;
        }
    }
    
    iterator insert(iterator pos, const T& value) {
        if (pos.container != this) {
            throw std::runtime_error("Invalid iterator");
        }
        
        if (pos == end()) {
            push_back(value);
            return iterator(this, back_block, back_index);
        }
        
        if (pos == begin()) {
            push_front(value);
            return begin();
        }
        
        size_t insert_pos = pos - begin();
        push_back(at(element_count - 1));
        
        for (size_t i = element_count - 2; i > insert_pos; --i) {
            at(i) = at(i - 1);
        }
        
        at(insert_pos) = value;
        
        return begin() + insert_pos;
    }
    
    iterator erase(iterator pos) {
        if (pos.container != this) {
            throw std::runtime_error("Invalid iterator");
        }
        
        if (pos == end()) {
            throw std::runtime_error("Cannot erase end iterator");
        }
        
        size_t erase_pos = pos - begin();
        
        for (size_t i = erase_pos; i < element_count - 1; ++i) {
            at(i) = at(i + 1);
        }
        
        pop_back();
        
        if (erase_pos >= element_count) {
            return end();
        }
        
        return begin() + erase_pos;
    }
};

}

#endif
