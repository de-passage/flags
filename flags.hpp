/********************************************************
 * Wrapper class and helpers for flag manipulation
 *
 * Author: Sylvain Leclercq <maisbiensurqueoui@gmail.com>
 * Version: 0.1.0
 *******************************************************/

#ifndef DPSG_FLAGS_HPP
#define DPSG_FLAGS_HPP

#include <iterator>
#include <type_traits>

#define DPSG_PP_NARG(...) \
	DPSG_PP_NARG_(__VA_ARGS__, DPSG_PP_RSEQ_N())

#define DPSG_PP_NARG_(...) \
	DPSG_PP_ARG_N(__VA_ARGS__)

#define DPSG_PP_ARG_N( \
		_1, _2, _3, _4, _5, _6, _7, _8,	N, ...) N

#define DPSG_PP_RSEQ_N()	\
	8, 7, 6, 5, 4, 3, 2, 1

#define DPSG_PP_EXPAND(count, ...)	\
	DPSG_PP_EXPAND_##count (__VA_ARGS__)

#define DPSG_PP_EXPAND_1(a) a = 1
#define DPSG_PP_EXPAND_2(a, b) DPSG_PP_EXPAND_1(a), b = 1 << 1
#define DPSG_PP_EXPAND_3(a, b, c) DPSG_PP_EXPAND_2(a, b), c = 1 << 2
#define DPSG_PP_EXPAND_4(a, b, c, d) DPSG_PP_EXPAND_3(a, b, c), d = 1 << 3
#define DPSG_PP_EXPAND_5(a, b, c, d, e) DPSG_PP_EXPAND_4(a, b, c, d), e = 1 << 4
#define DPSG_PP_EXPAND_6(a, b, c, d, e, f) DPSG_PP_EXPAND_5(a, b, c, d, e), f = 1 << 5
#define DPSG_PP_EXPAND_7(a, b, c, d, e, f, g) DPSG_PP_EXPAND_6(a, b, c, d, e, f), g = 1 << 6
#define DPSG_PP_EXPAND_8(a, b, c, d, e, f, g, h) DPSG_PP_EXPAND_7(a, b, c, d, e, f, g), h = 1 << 7

#define DPSG_DECLARE_FLAG_(TYPE, NAME, COUNT, ...) 					\
	class NAME														\
		: public dpsg::detail::BaseFlag<							\
			typename dpsg::detail::filter_type<TYPE>::type, COUNT> {\
		typedef BaseFlag<											\
			typename dpsg::detail::filter_type<TYPE>::type			\
			, COUNT> Base;											\
		public:														\
		enum value : 												\
				typename dpsg::detail::filter_type<TYPE>::type 		\
				{DPSG_PP_EXPAND(COUNT, __VA_ARGS__)}; 				\
		NAME() : Base(Base::Count) {}								\
		NAME(typename dpsg::detail::filter_type<TYPE>::type i) : 	\
				Base(i) {} 											\
	};

#define DPSG_DECLARE_FLAG(type, name, ...) DPSG_DECLARE_FLAG_(type, name, DPSG_PP_NARG(__VA_ARGS__), __VA_ARGS__)

namespace dpsg { namespace detail {

template<class T>
	struct filter_type { typedef typename std::remove_cv<typename std::make_unsigned<T>::type>::type type; };

template<class T, char C>
class BaseFlag {

	public:
		typedef T type;
	private:
		typedef BaseFlag<type, C> 	this_t;
		typedef this_t& 			this_r;
		typedef this_t*				this_p;
		typedef this_t const *	 	this_p_c;
		typedef char count_t;

	public:
		constexpr static count_t Count = C;

		template<typename P> class proxy_base;
		typedef proxy_base<this_p> proxy;
		typedef const proxy_base<this_p_c> const_proxy;

		template<typename P>
		class base_iterator : public std::iterator<std::random_access_iterator_tag, P> {
			typedef typename std::remove_cv<P>::type proxy_t;
			typedef typename proxy_t::pointer_t pointer_t;
			typedef base_iterator<P> iter;
			public:
				base_iterator(pointer_t r, count_t p) : _proxy(r ,1) { 
					move(_proxy._pos, p); 
				}
				
				inline proxy_t operator*() { 
					return _proxy; 
				}

				inline const proxy_t operator*() const { 
					return _proxy; 
				}

				inline iter& operator++() {
					move(_proxy._pos, 1); 
					return *this;
				}

				inline iter& operator++(int) {
					iter tmp = *this; move(); 
					return tmp;
				}

				inline bool operator==(const iter& it) { 
					return _proxy.is_same(it._proxy); 
				}

				inline bool operator!=(const iter& it) {
					return !(*this == it);
				}

				inline proxy_t* operator->() {
					return &_proxy;
				}

			private:
				proxy_t _proxy;
		};
		typedef base_iterator<proxy> iterator;
		typedef base_iterator<const_proxy> const_iterator;

		template<typename B> 
		class proxy_base {

			public:
				typedef B pointer_t;
				proxy_base(pointer_t r, type p) : _ref(r), _pos(p) {}
				operator bool() const { return is_set(); } 
				bool is_set() const { return _ref->is_set(_pos); }
				void set() { _ref->set(_pos); }
				void unset() { _ref->unset(_pos); }
				void toggle() { _ref->toggle(_pos); }
				proxy_base<pointer_t>& operator=(bool b) { b ? set() : unset(); }
			private:
				friend class base_iterator<proxy>;
				friend class base_iterator<const_proxy>;
				pointer_t _ref;
				type _pos;
				inline bool is_same(const proxy_base<pointer_t>& o)const {
					return o._pos == _pos && o._ref == _ref; 
				}
		};

		constexpr inline operator type() {
			return _value;
		}

		inline bool is_set(type v) const {
			return _value & v;
		}

		inline void set(type v) { 
			_value |= v;
		}

		inline void toggle(type v) {
			_value ^= v;
		}

		inline void unset(type v) { 
			_value =~ (~_value | v); 
		}

		inline proxy operator[](type i) { 
			return proxy(this, i); 
		}

		inline const_proxy operator[](type i) const { 
			return const_proxy(this, i); 
		}

		inline iterator begin() {
			return iterator(this, 0);
		}

		inline iterator end() {
			return iterator(this, Count);
		}

		inline const_iterator begin() const { 
			return const_iterator(this, 0);
		}

		inline const_iterator end() const { 
			return const_iterator(this, Count);
		}


	protected:
		BaseFlag(type i) : _value(i) {} 
		BaseFlag() = default;
	
	private: 
		T _value;
		static void move(type& p, count_t offset) {

			if(offset > 0) 
			{
				if(p == 0)
				{
					p = 1;
					if(--offset == 0) return;
				}
				if(p == ~Count) 
					return;
				else if(((p << offset) > (1 << (Count-1))) || (p << offset) == 0)
					p = ~Count;
				else 
					p <<= offset;
			}
			else if(offset < 0)
			{
				offset = -offset;
				p >> offset;
			}

		}
};

}}
#endif
