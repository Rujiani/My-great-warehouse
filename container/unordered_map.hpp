#ifndef UNORDERED_MAP_HPP_
#define UNORDERED_MAP_HPP_

#include <functional>   // for std::hash
#include <stdexcept>    // for std::out_of_range
#include <utility>      // for std::pair, std::move, std::swap
#include <iterator>     // for std::bidirectional_iterator_tag

namespace mgc {

/**
 * @brief A hash map implementation using separate chaining.
 *
 * This template class implements a hash map similar to std::unordered_map.
 * It uses separate chaining (via a linked list in each bucket) to resolve collisions.
 * Additionally, it maintains a global doubly linked list of nodes to support bidirectional iteration.
 *
 * @tparam Key      The key type. Must be default constructible.
 * @tparam Value    The mapped value type. Must be default constructible.
 * @tparam Hash     The hash function object type. Defaults to std::hash<Key>.
 */
template<typename Key, typename Value, typename Hash = std::hash<Key>>
class HashMap {
public:
    /// The type of key-value pair stored in the map.
    using value_type = std::pair<const Key, Value>;

private:
    /**
     * @brief Internal node structure.
     *
     * Each node is stored in a bucket (using the bucket_next pointer)
     * and is also linked in a global doubly linked list (using next/prev pointers) for iteration.
     */
    struct Node {
        value_type kv;    ///< The key-value pair.
        Node* bucket_next;///< Pointer to the next node in the same bucket.
        Node* next;       ///< Pointer to the next node in the global list.
        Node* prev;       ///< Pointer to the previous node in the global list.

        /**
         * @brief Constructs a Node with the given key and value.
         *
         * @param key   The key.
         * @param value The value.
         */
        Node(const Key &key, const Value &value)
            : kv(key, value), bucket_next(nullptr), next(nullptr), prev(nullptr) {}
    };

    Node** buckets;      ///< Dynamic array of bucket pointers.
    size_t capacity;     ///< Number of buckets.
    size_t count;        ///< Number of elements stored.
    Node* head;          ///< Head of the global doubly linked list of nodes.
    Node* tail;          ///< Tail of the global doubly linked list of nodes.
    Hash hashFunc;       ///< Hash function object.
    double max_load;     ///< Maximum load factor threshold before rehashing.

    /**
     * @brief Internal rehash function.
     *
     * Rebuilds the hash table with a new bucket capacity.
     *
     * @param new_cap The new number of buckets.
     */
    void rehash_internal(size_t new_cap) {
        // Allocate a new bucket array and initialize pointers.
        Node** new_buckets = new Node*[new_cap];
        for (size_t i = 0; i < new_cap; ++i)
            new_buckets[i] = nullptr;

        // Reassign each node to a new bucket.
        for (Node* cur = head; cur; cur = cur->next) {
            size_t idx = hashFunc(cur->kv.first) % new_cap;
            cur->bucket_next = new_buckets[idx];
            new_buckets[idx] = cur;
        }
        // Free the old bucket array and update member variables.
        delete[] buckets;
        buckets = new_buckets;
        capacity = new_cap;
    }

public:
    /**
     * @brief Const bidirectional iterator for HashMap.
     *
     * Allows read-only traversal of the map's elements.
     */
    class const_iterator {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type        = std::pair<const Key, Value>;
        using difference_type   = std::ptrdiff_t;
        using pointer           = const value_type*;
        using reference         = const value_type&;

        /**
         * @brief Default constructor.
         */
        const_iterator() : node(nullptr), map(nullptr) {}

        /**
         * @brief Constructs an iterator from a node pointer and associated map.
         *
         * @param n Pointer to the node.
         * @param m Pointer to the associated HashMap.
         */
        const_iterator(Node* n, const HashMap* m) : node(n), map(m) {}

        /**
         * @brief Dereference operator.
         *
         * @return Reference to the key-value pair.
         */
        reference operator*() const { return node->kv; }

        /**
         * @brief Arrow operator.
         *
         * @return Pointer to the key-value pair.
         */
        pointer operator->() const { return &(node->kv); }

        /**
         * @brief Pre-increment operator.
         *
         * @return Reference to the iterator after increment.
         */
        const_iterator& operator++() {
            if (node) { node = node->next; }
            return *this;
        }

        /**
         * @brief Post-increment operator.
         *
         * @return Iterator before increment.
         */
        const_iterator operator++(int) {
            const_iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        /**
         * @brief Pre-decrement operator.
         *
         * If the iterator is at end(), it moves to the tail.
         *
         * @return Reference to the iterator after decrement.
         * @throws std::out_of_range if decrement is not possible.
         */
        const_iterator& operator--() {
            if (node)
                node = node->prev;
            else if (map)
                node = map->tail;
            else
                throw std::out_of_range("Cannot decrement iterator");
            return *this;
        }

        /**
         * @brief Post-decrement operator.
         *
         * @return Iterator before decrement.
         */
        const_iterator operator--(int) {
            const_iterator tmp = *this;
            --(*this);
            return tmp;
        }

        /**
         * @brief Equality comparison.
         *
         * @param other Another iterator.
         * @return true if both iterators point to the same node.
         */
        bool operator==(const const_iterator &other) const { return node == other.node; }

        /**
         * @brief Inequality comparison.
         *
         * @param other Another iterator.
         * @return true if iterators do not point to the same node.
         */
        bool operator!=(const const_iterator &other) const { return node != other.node; }
    private:
        Node* node;          ///< Pointer to the current node.
        const HashMap* map;  ///< Pointer to the associated HashMap.
        friend class HashMap;
    };

    /**
     * @brief Mutable bidirectional iterator for HashMap.
     *
     * Allows modifying the mapped value.
     */
    class iterator {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type        = std::pair<const Key, Value>;
        using difference_type   = std::ptrdiff_t;
        using pointer           = value_type*;
        using reference         = value_type&;

        /**
         * @brief Default constructor.
         */
        iterator() : node(nullptr), map(nullptr) {}

        /**
         * @brief Constructs an iterator from a node pointer and associated map.
         *
         * @param n Pointer to the node.
         * @param m Pointer to the associated HashMap.
         */
        iterator(Node* n, const HashMap* m) : node(n), map(m) {}

        /**
         * @brief Dereference operator.
         *
         * @return Reference to the key-value pair.
         */
        reference operator*() const { return node->kv; }

        /**
         * @brief Arrow operator.
         *
         * @return Pointer to the key-value pair.
         */
        pointer operator->() const { return &(node->kv); }

        /**
         * @brief Pre-increment operator.
         *
         * @return Reference to the iterator after increment.
         */
        iterator& operator++() {
            if (node) { node = node->next; }
            return *this;
        }

        /**
         * @brief Post-increment operator.
         *
         * @return Iterator before increment.
         */
        iterator operator++(int) {
            iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        /**
         * @brief Pre-decrement operator.
         *
         * If the iterator is at end(), it moves to the tail.
         *
         * @return Reference to the iterator after decrement.
         * @throws std::out_of_range if decrement is not possible.
         */
        iterator& operator--() {
            if (node)
                node = node->prev;
            else if (map)
                node = map->tail;
            else
                throw std::out_of_range("Cannot decrement iterator");
            return *this;
        }

        /**
         * @brief Post-decrement operator.
         *
         * @return Iterator before decrement.
         */
        iterator operator--(int) {
            iterator tmp = *this;
            --(*this);
            return tmp;
        }

        /**
         * @brief Equality comparison.
         *
         * @param other Another iterator.
         * @return true if both iterators point to the same node.
         */
        bool operator==(const iterator &other) const { return node == other.node; }

        /**
         * @brief Inequality comparison.
         *
         * @param other Another iterator.
         * @return true if iterators do not point to the same node.
         */
        bool operator!=(const iterator &other) const { return node != other.node; }
    private:
        Node* node;          ///< Pointer to the current node.
        const HashMap* map;  ///< Pointer to the associated HashMap.
        friend class HashMap;
    };

    /**
     * @brief Constructs an empty HashMap with an initial bucket capacity and load factor threshold.
     *
     * @param init_cap Initial number of buckets (default is 11).
     * @param load     Maximum load factor before rehashing (default is 1.0).
     */
    explicit HashMap(size_t init_cap = 11, double load = 1.0)
        : capacity(init_cap), count(0), head(nullptr), tail(nullptr), max_load(load)
    {
        buckets = new Node*[capacity];
        for (size_t i = 0; i < capacity; ++i)
            buckets[i] = nullptr;
    }

    /**
     * @brief Copy constructor.
     *
     * Creates a deep copy of the given HashMap.
     *
     * @param other The HashMap to copy.
     */
    HashMap(const HashMap &other)
        : capacity(other.capacity), count(0), head(nullptr), tail(nullptr),
          hashFunc(other.hashFunc), max_load(other.max_load)
    {
        buckets = new Node*[capacity];
        for (size_t i = 0; i < capacity; ++i)
            buckets[i] = nullptr;
        // Copy nodes from the other map preserving global order.
        for (Node* cur = other.head; cur; cur = cur->next) {
            Node* newNode = new Node(cur->kv.first, cur->kv.second);
            // Append to global doubly linked list.
            newNode->prev = tail;
            newNode->next = nullptr;
            if (tail)
                tail->next = newNode;
            else
                head = newNode;
            tail = newNode;
            // Insert into appropriate bucket.
            size_t idx = hashFunc(newNode->kv.first) % capacity;
            newNode->bucket_next = buckets[idx];
            buckets[idx] = newNode;
            ++count;
        }
    }

    /**
     * @brief Move constructor.
     *
     * Transfers ownership of resources from the given HashMap.
     *
     * @param other The HashMap to move.
     */
    HashMap(HashMap &&other) noexcept
        : buckets(other.buckets), capacity(other.capacity), count(other.count),
          head(other.head), tail(other.tail), hashFunc(std::move(other.hashFunc)),
          max_load(other.max_load)
    {
        other.buckets = nullptr;
        other.capacity = 0;
        other.count = 0;
        other.head = other.tail = nullptr;
    }

    /**
     * @brief Destructor.
     *
     * Clears all elements and frees allocated resources.
     */
    ~HashMap(){
        clear();
        delete[] buckets;
    }

    /**
     * @brief Copy assignment operator.
     *
     * Replaces the contents of this HashMap with a deep copy of the given one.
     *
     * @param other The HashMap to copy.
     * @return Reference to this HashMap.
     */
    HashMap& operator=(const HashMap &other) {
        if (this != &other) {
            HashMap temp(other);
            swap(temp);
        }
        return *this;
    }

    /**
     * @brief Move assignment operator.
     *
     * Transfers ownership of resources from the given HashMap.
     *
     * @param other The HashMap to move.
     * @return Reference to this HashMap.
     */
    HashMap& operator=(HashMap &&other) noexcept {
        if (this != &other) {
            clear();
            delete[] buckets;
            buckets   = other.buckets;
            capacity  = other.capacity;
            count     = other.count;
            head      = other.head;
            tail      = other.tail;
            hashFunc  = std::move(other.hashFunc);
            max_load  = other.max_load;
            other.buckets = nullptr;
            other.capacity = 0;
            other.count = 0;
            other.head = other.tail = nullptr;
        }
        return *this;
    }

    /**
     * @brief Swaps the contents of this HashMap with another.
     *
     * @param other The HashMap to swap with.
     */
    void swap(HashMap &other) noexcept {
        std::swap(buckets, other.buckets);
        std::swap(capacity, other.capacity);
        std::swap(count, other.count);
        std::swap(head, other.head);
        std::swap(tail, other.tail);
        std::swap(hashFunc, other.hashFunc);
        std::swap(max_load, other.max_load);
    }

    /**
     * @brief Clears the HashMap.
     *
     * Deletes all nodes and resets the container to an empty state.
     */
    void clear() {
        Node* cur = head;
        while(cur) {
            Node* nxt = cur->next;
            delete cur;
            cur = nxt;
        }
        head = tail = nullptr;
        count = 0;
        for (size_t i = 0; i < capacity; ++i)
            buckets[i] = nullptr;
    }

    /**
     * @brief Returns the number of elements in the HashMap.
     *
     * @return The number of stored key-value pairs.
     */
    size_t size() const { return count; }

    /**
     * @brief Returns the current load factor.
     *
     * @return The ratio of the number of elements to the number of buckets.
     */
    double load_factor() const { return capacity ? static_cast<double>(count) / capacity : 0; }

    /**
     * @brief Manually rehashes the HashMap.
     *
     * Rebuilds the table with the specified number of buckets.
     *
     * @param new_cap The new bucket capacity.
     */
    void rehash(size_t new_cap) { rehash_internal(new_cap); }

    /**
     * @brief Inserts a key-value pair into the HashMap.
     *
     * If the key already exists, its value is updated.
     *
     * @param key   The key to insert.
     * @param value The value associated with the key.
     */
    void insert(const Key &key, const Value &value) {
        size_t idx = hashFunc(key) % capacity;
        // Check if the key already exists in the bucket.
        for (Node* cur = buckets[idx]; cur; cur = cur->bucket_next) {
            if (cur->kv.first == key) {
                cur->kv.second = value;
                return;
            }
        }
        // Create a new node.
        Node* newNode = new Node(key, value);
        // Insert into the bucket chain.
        newNode->bucket_next = buckets[idx];
        buckets[idx] = newNode;
        // Append to the global doubly linked list.
        newNode->prev = tail;
        newNode->next = nullptr;
        if (tail)
            tail->next = newNode;
        else
            head = newNode;
        tail = newNode;
        ++count;
        // Trigger rehashing if load factor exceeded.
        if (load_factor() > max_load)
            rehash_internal(capacity * 2 + 1);
    }

    /**
     * @brief Access operator.
     *
     * Returns a reference to the mapped value associated with the given key.
     * If the key is not found, a new element with a default-constructed value is inserted.
     *
     * @param key The key to access.
     * @return Reference to the associated value.
     */
    Value& operator[](const Key &key) {
        size_t idx = hashFunc(key) % capacity;
        for (Node* cur = buckets[idx]; cur; cur = cur->bucket_next) {
            if (cur->kv.first == key)
                return cur->kv.second;
        }
        // Key not found; create a new node with default value.
        Node* newNode = new Node(key, Value());
        newNode->bucket_next = buckets[idx];
        buckets[idx] = newNode;
        newNode->prev = tail;
        newNode->next = nullptr;
        if (tail)
            tail->next = newNode;
        else
            head = newNode;
        tail = newNode;
        ++count;
        if (load_factor() > max_load)
            rehash_internal(capacity * 2 + 1);
        return newNode->kv.second;
    }

    /**
     * @brief Erases the element with the given key.
     *
     * @param key The key of the element to erase.
     */
    void erase(const Key &key) {
        size_t idx = hashFunc(key) % capacity;
        Node* cur = buckets[idx];
        Node* prevBucket = nullptr;
        while (cur) {
            if (cur->kv.first == key) {
                // Remove from bucket chain.
                if (prevBucket)
                    prevBucket->bucket_next = cur->bucket_next;
                else
                    buckets[idx] = cur->bucket_next;
                // Remove from global doubly linked list.
                if (cur->prev)
                    cur->prev->next = cur->next;
                else
                    head = cur->next;
                if (cur->next)
                    cur->next->prev = cur->prev;
                else
                    tail = cur->prev;
                delete cur;
                --count;
                return;
            }
            prevBucket = cur;
            cur = cur->bucket_next;
        }
    }

    /**
     * @brief Finds an element by key.
     *
     * @param key The key to search for.
     * @return An iterator to the element if found, or end() if not found.
     */
    iterator find(const Key &key) {
        size_t idx = hashFunc(key) % capacity;
        for (Node* cur = buckets[idx]; cur; cur = cur->bucket_next)
            if (cur->kv.first == key)
                return iterator(cur, this);
        return end();
    }

    /**
     * @brief Finds an element by key (const version).
     *
     * @param key The key to search for.
     * @return A const_iterator to the element if found, or end() if not found.
     */
    const_iterator find(const Key &key) const {
        size_t idx = hashFunc(key) % capacity;
        for (Node* cur = buckets[idx]; cur; cur = cur->bucket_next)
            if (cur->kv.first == key)
                return const_iterator(cur, this);
        return end();
    }

    /**
     * @brief Returns an iterator to the first element.
     *
     * @return Iterator pointing to the first element.
     */
    iterator begin() { return iterator(head, this); }

    /**
     * @brief Returns an iterator past the last element.
     *
     * @return Iterator representing end().
     */
    iterator end()   { return iterator(nullptr, this); }

    /**
     * @brief Returns a const iterator to the first element.
     *
     * @return Const iterator pointing to the first element.
     */
    const_iterator begin() const { return const_iterator(head, this); }

    /**
     * @brief Returns a const iterator past the last element.
     *
     * @return Const iterator representing end().
     */
    const_iterator end() const   { return const_iterator(nullptr, this); }

    /**
     * @brief Returns a const iterator to the first element.
     *
     * @return Const iterator pointing to the first element.
     */
    const_iterator cbegin() const { return const_iterator(head, this); }

    /**
     * @brief Returns a const iterator past the last element.
     *
     * @return Const iterator representing end().
     */
    const_iterator cend() const   { return const_iterator(nullptr, this); }
};

}
#endif // UNORDERED_MAP_HPP_
