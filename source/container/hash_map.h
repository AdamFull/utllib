#pragma once

#include <standart_library.h>

namespace utl
{
	// Based on https://github.com/rigtorp/hash_map/blob/master/include/rigtorp/hash_map.h
	template<class _KTy, class _Ty, class _Hash = std::hash<_KTy>, 
		class _KeyEq = std::equal_to<void>,
		class _Allocator = std::allocator<std::pair<_KTy, _Ty>*>>
	class hash_map
	{
	public:
		using key_type = _KTy;
		using mapped_type = _Ty;
		using value_type = std::pair<_KTy, _Ty>;
		using size_type = std::size_t;
		using hasher = _Hash;
		using key_equal = _KeyEq;
		using allocator_type = _Allocator;
		using reference = value_type&;
		using const_reference = const value_type&;
		using buckets = std::vector<value_type*, allocator_type>;

        template <class _ContT, class _IterVal> 
        struct hm_iterator
        {
            friend class hash_map;
            using difference_type = std::ptrdiff_t;
            using value_type = _IterVal;
            using pointer = value_type*;
            using reference = value_type&;
            using iterator_category = std::forward_iterator_tag;

            bool operator==(const hm_iterator& other) const { return other.hm_ == hm_ && other.idx_ == idx_; }
            bool operator!=(const hm_iterator& other) const { return !(other == *this); }

            hm_iterator& operator++()
            {
                ++idx_;
                advance_past_empty();
                return *this;
            }

            reference operator*() const { return *hm_->buckets_[idx_]; }
            pointer operator->() const { return hm_->buckets_[idx_]; }

        private:
            explicit hm_iterator(_ContT* hm) : hm_(hm) { advance_past_empty(); }
            explicit hm_iterator(_ContT* hm, size_type idx) : hm_(hm), idx_(idx) {}

            template <class _OtherContT, class _OtherIterVal>
            hm_iterator(const hm_iterator<_OtherContT, _OtherIterVal>& other)
                : hm_(other.hm_), idx_(other.idx_) {}

            void advance_past_empty()
            {
                while (idx_ < hm_->buckets_.size() && !hm_->buckets_[idx_])
                    ++idx_;
            }

            _ContT* hm_{ nullptr };
            typename _ContT::size_type idx_{ 0 };
            friend _ContT;
        };

        using iterator = hm_iterator<hash_map, value_type>;
        using const_iterator = hm_iterator<const hash_map, const value_type>;

    public:
        hash_map() :
            hash_map(16ull)
        {
        }
        
        hash_map(size_type bucket_count, const allocator_type& alloc = allocator_type())
            : buckets_(alloc), freed_(alloc)
        {
            init(bucket_count);
        }

        hash_map(const hash_map& other, size_type bucket_count)
            : hash_map(bucket_count, other.get_allocator()) 
        {
            for (auto it = other.begin(); it != other.end(); ++it)
                insert(*it);
        }

        hash_map(const hash_map& other) 
            : hash_map(other, other.buckets_.size())
        {
        }

        hash_map& operator=(const hash_map& other)
        {
            if (this != &other)
            {
                clear();
                init(other.buckets_.size());

                for (auto it = other.begin(); it != other.end(); ++it)
                    insert(*it);
            }
            return *this;
        }

        hash_map(hash_map&& other)
        {
            buckets_ = std::move(other.buckets_);
            freed_ = std::move(other.freed_);
            size_ = other.size_;
            other.size_ = 0;
        }
        
        hash_map& operator=(hash_map&& other)
        {
            if (this != &other)
            {
                buckets_ = std::move(other.buckets_);
                freed_ = std::move(freed_);
                size_ = other.size_;
                other.size_ = 0;
            }
        
            return *this;
        }

        ~hash_map()
        {
            clear();

            for (auto& node : freed_)
            {
                if (node)
                {
                    std::free(node);
                    node = nullptr;
                }
            }
        }

        allocator_type get_allocator() const noexcept { return buckets_.get_allocator(); }

        // Iterators
        iterator begin() noexcept { return iterator(this); }
        const_iterator begin() const noexcept { return const_iterator(this); }
        const_iterator cbegin() const noexcept { return const_iterator(this); }

        iterator end() noexcept { return iterator(this, buckets_.size()); }
        const_iterator end() const noexcept { return const_iterator(this, buckets_.size()); }
        const_iterator cend() const noexcept  { return const_iterator(this, buckets_.size()); }

        // Capacity
        bool empty() const noexcept { return size() == 0; }
        size_type size() const noexcept { return size_; }
        size_type max_size() const noexcept { return buckets_.max_size() / 2; }

        // Modifiers
        void clear() noexcept 
        {
            for (auto& bucket : buckets_)
            {
                if (bucket)
                {
                    if constexpr (!std::is_trivial_v<key_type>)
                        bucket->first.~key_type();

                    if constexpr (!std::is_trivial_v<mapped_type>)
                        bucket->second.~mapped_type();

                    freed_.emplace_back(bucket);
                    bucket = nullptr;
                }
            }

            size_ = 0;
        }

        std::pair<iterator, bool> insert(const value_type& value) { return emplace_impl(value.first, value.second); }
        std::pair<iterator, bool> insert(value_type&& value) { return emplace_impl(value.first, std::move(value.second)); }

        template <class... _Args>
        std::pair<iterator, bool> emplace(_Args &&... args) { return emplace_impl(std::forward<_Args>(args)...); }

        iterator erase(iterator it) { return erase_impl(it); }
        size_type erase(const key_type& key) { return erase_impl(key); }

        void swap(hash_map& other) noexcept 
        {
            std::swap(buckets_, other.buckets_);
            std::swap(size_, other.size_);
        }

        // Lookup
        mapped_type& at(const key_type& key) { return at_impl(key); }
        const mapped_type& at(const key_type& key) const { return at_impl(key); }
        mapped_type& operator[](const key_type& key) { return emplace_impl(key, mapped_type()).first->second; }

        size_type count(const key_type& key) const { return count_impl(key); }

        iterator find(const key_type& key) { return find_impl(key); }
        const_iterator find(const key_type& key) const { return find_impl(key); }

        // Bucket interface
        size_type bucket_count() const noexcept { return buckets_.size(); }

        size_type max_bucket_count() const noexcept { return buckets_.max_size(); }

        // Hash policy
        void rehash(size_type count) 
        {
            count = std::max(count, size() * 2);

            buckets tmp = std::move(buckets_);
            size_ = 0ull;

            init(count);

            for (auto& bucket : tmp)
            {
                if (bucket)
                {
                    if constexpr (!std::is_copy_constructible_v<_Ty> && std::is_move_constructible_v<_Ty>)
                        insert(std::move(*bucket));
                    else
                    {
                        insert(*bucket);

                        if constexpr (!std::is_trivial_v<key_type>)
                            bucket->first.~key_type();

                        if constexpr (!std::is_trivial_v<mapped_type>)
                            bucket->second.~mapped_type();

                        freed_.emplace_back(bucket);
                        bucket = nullptr;
                    }
                }
            }
        }

        void reserve(size_type count) 
        {
            if (count * 2 > buckets_.size())
                rehash(count * 2);
        }

        // Observers
        hasher hash_function() const { return hasher(); }
        key_equal key_eq() const { return key_equal(); }

    private:
        void init(size_type bucket_count)
        {
            size_t pow2{ 1ull };
            while (pow2 < bucket_count)
                pow2 <<= 1ull;

            buckets_.resize(pow2, nullptr);
        }

        template<class... _Args>
        value_type* get_node(const key_type& key, _Args&&... args)
        {
            value_type* bucket{ nullptr };
            if (!freed_.empty())
            {
                bucket = freed_.back();
                freed_.pop_back();
            }
            else
                bucket = static_cast<value_type*>(std::calloc(1ull, sizeof(value_type)));

            bucket->first = key_type(key);
            bucket->second = mapped_type(std::forward<_Args>(args)...);

            return bucket;
        }

        template<class... _Args>
        std::pair<iterator, bool> emplace_impl(const key_type& key, _Args&&... args)
        {
            reserve(size_ + 1ull);
            for (size_t idx = key_to_idx(key);; idx = probe_next(idx))
            {
                auto& bucket = buckets_[idx];
                if (!bucket)
                {
                    bucket = get_node(key, std::forward<_Args>(args)...);
                    size_++;
                    return { iterator(this, idx), true };
                }
                else if (key_equal()(bucket->first, key))
                    return { iterator(this, idx), false };
            }
        }

        iterator erase_impl(iterator it)
        {
            size_t bucket = it.idx_;
            for (size_t idx = probe_next(bucket);; idx = probe_next(idx)) 
            {
                auto& current = buckets_[bucket];
                auto& next = buckets_[idx];

                if (!next)
                {
                    freed_.emplace_back(current);
                    current = nullptr;

                    size_--;

                    it.advance_past_empty();
                    return it;
                }

                size_t ideal = key_to_idx(next->first);
                if (diff(bucket, ideal) < diff(idx, ideal)) 
                {
                    // swap, bucket is closer to ideal than idx
                    std::swap(current, next);
                    bucket = idx;
                }
            }

            return end();
        }

        size_type erase_impl(const key_type& key)
        {
            auto it = find_impl(key);
            if (it != end()) 
            {
                erase_impl(it);
                return 1;
            }
            return 0;
        }

        mapped_type& at_impl(const key_type& key)
        {
            iterator it = find_impl(key);
            if (it != end())
                return it->second;
 
            throw std::out_of_range("hash_map::at");
        }

        const mapped_type& at_impl(const key_type& key) const
        { return const_cast<hash_map*>(this)->at_impl(key); }

        size_t count_impl(const key_type& key) const
        { return find_impl(key) == end() ? 0 : 1; }

        iterator find_impl(const key_type& key)
        {
            for (size_t idx = key_to_idx(key);; idx = probe_next(idx)) 
            {
                if (!buckets_[idx])
                    return end();

                if (key_equal()(buckets_[idx]->first, key))
                    return iterator(this, idx);
            }
        }

        const_iterator find_impl(const key_type& key) const
        { return const_cast<hash_map*>(this)->find_impl(key); }

        size_t key_to_idx(const key_type& key) const noexcept(noexcept(hasher()(key))) 
        {
            const size_t mask = buckets_.size() - 1ull;
            return hasher()(key) & mask;
        }

        size_t probe_next(size_t idx) const noexcept 
        {
            const size_t mask = buckets_.size() - 1ull;
            return (idx + 1) & mask;
        }

        size_t diff(size_t a, size_t b) const noexcept 
        {
            const size_t mask = buckets_.size() - 1ull;
            return (buckets_.size() + (a - b)) & mask;
        }

    private:
        buckets buckets_;
        buckets freed_;
        size_t size_ = 0;
	};
}